#include "Console.h"
#include "Common.h"

#include <vector>
#include <sstream>
#include <iterator>

Console::Console()
{
}

void Console::InitLog()
{
	system("CLS");
	printf("RCI Version %s \n", VERSION);
}

void Console::Log(const char* msg, LogLevel _loglevel, bool _newline)
{

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);	
	const char* prefix = "";

	switch(_loglevel)
	{
		case 0:
			prefix = "[ERROR]: ";
			SetConsoleTextAttribute(hConsole, (FOREGROUND_RED));
			break;
		case 1:
			prefix = "[WARNING]: ";
			SetConsoleTextAttribute(hConsole, (0x0006));
			break;
		case 2:
			prefix = "";
			SetConsoleTextAttribute(hConsole, (FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN));
			break;
	}	

	if (_loglevel <= this->loglevel)
	{		
		if (_newline  || this->forcenewline)
			std::cout << std::endl << prefix << msg << std::endl;
		else
			std::cout << prefix << msg << std::endl;
	}

	SetConsoleTextAttribute(hConsole, (FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN));
}

void Console::Input()
{
	char command[255];

	std::cout << std::endl << ">> ";
	std::cin.getline(command, sizeof(command));

	ExecuteCommand(command);
}

void Console::ExecuteCommand(std::string _command)
{
	if (_command == "")
		MainConsole.Log("NULL command.", Warning);

	else if (_command == "exit")
		exit(0);

	else if (_command == "cls")
		InitLog();

	else if (_command == "connect")
		MainSerialPort.Connect();

	else if (_command.find("write") != std::string::npos) {
		
		std::istringstream buf(_command);
		std::istream_iterator<std::string> beg(buf), end;
		std::vector<std::string> tokens(beg, end);

		//int x = std::stoul(tokens[1] , nullptr, 0);

		// NEEDS ERROR MANAGEMENT!
		// If tokens empty, then it crashes.
		
		MainSerialPort.WriteData(std::stoul(tokens[1], nullptr, 0));
	}

	else if (_command == "read")
		MainSerialPort.TempRead();

	else if (_command == "read toggle")
		return; // toggle flow reading ???

	else
		MainConsole.Log("Unrecognized command.", Warning);

}


void Console::SetLevel(LogLevel _loglevel)
{
	this->loglevel = _loglevel;
}


void Console::SetForceNewLine(bool _forcenewline)
{
	this->forcenewline = _forcenewline;
}

Console::~Console()
{
}
