#include "Console.h"
#include "Common.h"

Console::Console()
{
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

	else if (_command == "connect")
		MainSerialPort.Connect();

	else if (_command == "write 0")
		MainSerialPort.WriteData(0x00);

	else if (_command == "write asci5")
		MainSerialPort.WriteData(0x35);

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
