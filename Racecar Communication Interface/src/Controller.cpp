#include "Controller.h"
#include "Common.h"


// ###################################################################################################
// Constructor & Destructor

Controller::Controller()
{
	// Initialize objects
	this->command_buffer = std::vector<int>(3);
	//this->serial_port = SerialPort(COM_PORT);
}

Controller::~Controller()
{
}

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

void Controller::SendTelegram(TYPE _type, COMMAND _command, uint8_t& _data)
{
	MainConsole.Log("Sending telegram.", Console::Normal);

	serial_port.WriteByte(_type);
	serial_port.WriteByte(_command);
	serial_port.WriteByte(_data);
}

void Controller::ParseTelegram(const uint8_t * _telegram, int _length)
{
	if (_telegram[0] != TYPE::REPLY)
	{
		MainConsole.Log("Recieved telegram is of incompatible TYPE.", Console::Error);
		// ClearBuffer();
		return;
	}

	switch (_telegram[1])
	{
		case COMMAND::DATA1:
			break;

		case COMMAND::DATA2:
			break;

		default:
			return;
	}

	// DATA:
	// uint16_t value = (highByte << 8) | lowByte;

	// IF ERROR:
	// ClearBuffer();

}

void Controller::ListenRaw(bool _autoparse)
{
	;
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