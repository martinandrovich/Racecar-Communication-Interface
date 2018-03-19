#include "Controller.h"
#include "Common.h"


Controller::Controller()
{
	// Initialize buffer vector
	this->command_buffer = std::vector<int>(3);
}


Controller::~Controller()
{
}

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