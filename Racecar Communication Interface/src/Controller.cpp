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
		MainConsole.Log("Command recieved:", Console::Info, true);

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
	


//const auto& Controller::GetData(int& _variable) const
//{
//	return this->velocity;
//}