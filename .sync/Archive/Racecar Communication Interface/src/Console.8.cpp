#include "Console.h"
#include "Common.h"

Console::Console()
{
}

void Console::InitLog()
{
	system("CLS");
	printf("RCI Version %s \n", VERSION);
}

void Console::Log(const std::string& _msg, LogLevel _loglevel, bool _newline)
{

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);	
	std::string prefix = "";

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

void Console::ExecuteCommand(const std::string& _command)
{
	if (_command == "")
		MainConsole.Log("NULL command.", Warning);

	else if (_command == "exit")
	{
		MainSerialPort.Disconnect();
		while (MainSerialPort.isConnected())
			; // Wait
		exit(0);
	}


	else if (_command == "cls")
		InitLog();

	else if (_command == "connect")
		MainSerialPort.Connect();

	else if (_command.find("write") != std::string::npos) {

		std::istringstream buf(_command);
		std::istream_iterator<std::string> beg(buf), end;
		std::vector<std::string> tokens(beg, end);

		// NEEDS ERROR MANAGEMENT!
		// If tokens empty, then it crashes.

		MainSerialPort.WriteData(std::stoul(tokens[1], nullptr, 0));
	}

	else if (_command == "read")
	{
		int read_data = MainSerialPort.ReadData();
		if (read_data)
			printf("0x%X (%c)\n", read_data, read_data);
	}

	else if (_command == "readall")
		MainSerialPort.ReadAllData();

	else
		MainConsole.Log("Unrecognized command.", Warning);

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
