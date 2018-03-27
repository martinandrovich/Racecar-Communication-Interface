#include "Console.h"
#include "Common.h"

Console::Console()
{
}

// ###################################################################################################
// Logging

void Console::Init()
{
	system("CLS");
	printf("RCI Version %s \n", VERSION);
}

void Console::SetLevel(LogLevel _loglevel)
{
	this->loglevel = _loglevel;
}

void Console::SetForceNewLine(bool _forcenewline)
{
	this->forcenewline = _forcenewline;
}

void Console::Log(const std::string& _msg, LogLevel _loglevel, bool _newline)
{

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);	
	std::string prefix = "";

	switch(_loglevel)
	{
		case LogLevel::Error:
			prefix = "[ERROR]: ";
			SetConsoleTextAttribute(hConsole, (FOREGROUND_RED));
			break;
		case LogLevel::Warning:
			prefix = "[WARNING]: ";
			SetConsoleTextAttribute(hConsole, (0x0006));
			break;
		case LogLevel::Info:
			prefix = "[ALERT]: ";
			SetConsoleTextAttribute(hConsole, (FOREGROUND_GREEN));
			break;
		case LogLevel::Normal:
			prefix = "";
			SetConsoleTextAttribute(hConsole, (FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN));
			break;
	}	

	if (_loglevel <= this->loglevel)
	{		
		if (_newline  || this->forcenewline)
			std::cout << std::endl << prefix << _msg << std::endl;
		else
			std::cout << prefix << _msg << std::endl;
	}

	SetConsoleTextAttribute(hConsole, (FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN));
}

void Console::Input()
{
	std::string command;

	std::cout << std::endl << ">> ";
	std::getline(std::cin, command);

	ExecuteCommand(command);
}

std::string Console::OutputLastError()
{
	//Get the error message, if any.
	DWORD errorMessageID = ::GetLastError();
	if (errorMessageID == 0)
		return std::string(); //No error message has been recorded

	LPSTR messageBuffer = nullptr;
	size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

	std::string message(messageBuffer, size);

	//Free the buffer.
	LocalFree(messageBuffer);

	return message;
}


// ###################################################################################################
// Command Parser

void Console::ExecuteCommand(const std::string& _command)
{
	// Split _command by spaces into (dynamic) array
	std::istringstream iss(_command);
	std::vector<std::string> parsedcommand(std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>());

	/*auto command_type = parsedcommand[0];
	auto command_arg1 = parsedcommand[1];*/

	// !!!
	// Needs checking of command (input string) bounds

	if (_command == "")
		MainConsole.Log("NULL command.", Warning);

	else if (_command == "exit")
	{
		MainController.Disconnect();
		while (MainController.IsConnected())
			; // Wait
		exit(0);
	}

	else if (_command == "cls")
		Init();

	else if (_command == "connect")
		MainController.Connect();

	// Set command
	else if (parsedcommand[0] == "set") {

		// Set speed
		if (parsedcommand[1] == "speed")
			MainController.GetSerialController().WriteData(std::stoul(parsedcommand[2], nullptr, 0));

		// Set variable1
		if (parsedcommand[1] == "variable1")
			MainController.GetSerialController().WriteData(std::stoul(parsedcommand[2], nullptr, 0));
	}

	// Get command [TODO]
	else if (parsedcommand[0] == "get") {

		// Get all variables in their current state [TODO]
		if (parsedcommand[1] == "all")
			MainController.PollInformation();

		// Get specific variable in its current state [TODO]
		if (parsedcommand[1] == "variable1")
			MainController.GetSerialController().WriteData(std::stoul(parsedcommand[2], nullptr, 0));
	}

	// Poll command
	else if (parsedcommand[0] == "poll") {

		// Poll all variables in their current state
		if (parsedcommand[1] == "all")
			MainController.PollInformation();

		// Poll specific variable in its current state [TODO]
		if (parsedcommand[1] == "variable1")
			MainController.GetSerialController().WriteData(std::stoul(parsedcommand[2], nullptr, 0));
	}

	// Read command
	else if (parsedcommand[0] == "read") {

		// Read latest byte
		if (parsedcommand[1] == "last")
		{
			int read_data = MainController.GetSerialController().ReadData();
			if (read_data)
				printf("0x%X (%c)\n", read_data, read_data);
		}

		// Read whole buffer
		if (parsedcommand[1] == "buffer")
		{
			MainController.GetSerialController().ReadAllData();
		}
			
		// Start polling data
		if (parsedcommand[1] == "cont")
		{
			MainController.GetSerialController().ReadContinuousData();
		}
	}	

	else
		MainConsole.Log("Unrecognized command.", Warning);

}

Console::~Console()
{
}
