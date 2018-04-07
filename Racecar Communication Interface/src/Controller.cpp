#include "Controller.h"
#include "Common.h"


// ###################################################################################################
// Constructor & Destructor

Controller::Controller() {}

Controller::~Controller() {}

// ###################################################################################################
// Connectivity Methods

void Controller::Connect()
{
	serial_port.Connect();
}

void Controller::Disconnect()
{
	serial_port.Disconnect();
}

bool Controller::IsConnected()
{
	return serial_port.isConnected();
}

SerialPort& Controller::GetSerialController()
{
	return serial_port;
}

// ###################################################################################################
// Communication Protocol

void Controller::SendTelegram(TYPE _type, COMMAND _command, uint8_t _data)
{
	MainConsole.Log("Sending telegram.", Console::Normal, false);

	// !!! Show contents of sent telegram? i.e. Sending telegram: [0xAA | 0x11 | 0x00].

	serial_port.WriteByte(_type);
	serial_port.WriteByte(_command);
	serial_port.WriteByte(_data);
}

void Controller::ParseTelegram(const uint8_t * _telegram)
{
	// Check TYPE to be REPLY
	if (_telegram[0] != TYPE::REPLY)
	{
		MainConsole.Log("Recieved telegram is of incompatible TYPE.", Console::Error);
		serial_port.Flush();
		return;
	}

	// If looking for specific COMMAND and not found, then return.
	if (specifier != ALL && _telegram[1] != specifier)
		return;

	// Assemble Data:
	uint16_t data = (_telegram[2] << 8) | _telegram[3];

	printf("Recieved [TYPE: 0x%X] with [COMMAND: 0x%X] and [DATA: 0x%X]\n", _telegram[0], _telegram[1], data);

	// Select appropriate function
	switch (_telegram[1])
	{
		case COMMAND::DATA1:
			break;

		case COMMAND::DATA2:
			break;

		default:
			return;
	}

	// Reset listener specifier
	specifier == Controller::ALL;
}

bool Controller::Set(COMMAND _var, int _value, bool _verify)
{
	// Send SET telegram
	SendTelegram(Controller::SET, _var, _value);

	// Verify
	if (_verify && _value != 0)
	{
		Sleep(PAUSE);
		std::cout << "Verifying data..." << std::endl;

		if (_value == Get(_var))
			return true;
		else
			return false;
	}
	else
		return true;
}

int Controller::Get(COMMAND _var, int _timeout)
{

	// !!! NEEDS LISTENER MANAGEMENT / ERRROR HANDLING! :(

	// Send GET telegram
	SendTelegram(Controller::GET, _var);

	// Setup poll
	polling = true;
	uint8_t buffer[BUFFER_MAX_LENGTH];

	auto time_started = Clock::now();
	auto time_current = Clock::now();
	auto time_delta   = std::chrono::duration_cast<std::chrono::milliseconds>(time_current - time_started).count();	

	// Start polling
	while (polling)
	{
		// Check timeout
		time_current = Clock::now();
		time_delta   = std::chrono::duration_cast<std::chrono::milliseconds>(time_current - time_started).count();

		if (time_delta == _timeout)
			polling = false;		

		// Check buffer
		if (serial_port.Poll())
		{

			serial_port.ReadBuffer(*buffer);

			if (buffer[0] == TYPE::REPLY && buffer[1] == _var)
			{
				polling = false;
				uint16_t data = (buffer[2] << 8) | buffer[3];
				return data;
			}
		}		

	}

	MainConsole.Log("Could not GET; Poll timed out.", Console::Error);
	serial_port.Flush();
	return 0;
}

void Controller::Listen(COMMAND _var, int _refresh)
{
	specifier = _var;
	serial_port.Listen();
}
	
// ###################################################################################################
// Device Control

void Controller::PollData(COMMAND _var)
{
	MainConsole.Log("Starting data polling; press ESC to stop.\n", Console::Info);

	Sleep(2000);
	
	this->polling = true;

	int a = 0;
	int b = 0;

	while (true)
	{
		if (GetAsyncKeyState(VK_ESCAPE))
			break;

		a += 1;
		b += 7;

		std::cout << "\r" << "Speed: " << a << " | Distance: " << b;;
	}

	std::cout << std::endl;
}

void Controller::SetSpeed(const int& _speed)
{
	serial_port.WriteByte(_speed);
}