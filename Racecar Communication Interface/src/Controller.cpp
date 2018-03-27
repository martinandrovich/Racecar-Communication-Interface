#include "Controller.h"
#include "Common.h"


// ###################################################################################################
// Constructor & Destructor

Controller::Controller()
{
	// Initialize objects
	this->command_buffer = std::vector<int>(3);
	this->serial_port = SerialPort(COM_PORT);
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

void Controller::ParseCommand(std::vector<int>& _command)
{

	if (_command[0] != TYPE::REPLY)
		return;

	switch (_command[1])
	{
		case COMMAND::DATA1:
			break;

		case COMMAND::DATA2:
			break;

		default:
			return;
	}

}

void Controller::ParseStream(int _data)
{
	// Load data into buffer
	for (int i = 0; command_buffer.size() > i; i++)
	{
		if (command_buffer[i] == NULL)
		{
			command_buffer[i] = _data;
			break;
		}
	}

	// If buffer full, output & clear
	if (command_buffer[2] != NULL)
	{
		MainConsole.Log("Command recieved:", Console::Normal, true);

		// Could be done better with an overload for << operator
		for (auto i : command_buffer)
			printf("0x%X (%c) ", i, i);
			//std::cout << i << ' ';

		std::cout << std::endl;

		//// CALL Execute command

		// Fill vector with NULL
		std::fill(command_buffer.begin(), command_buffer.end(), NULL);

		return;
	}

}
	
// ###################################################################################################
// Device Control

void Controller::PollInformation()
{
	MainConsole.Log("Starting information poll; press ESC to stop.\n", Console::Info);
	
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
	serial_port.WriteData(_speed);
}


//const auto& Controller::GetData(int& _variable) const
//{
//	return this->velocity;
//}