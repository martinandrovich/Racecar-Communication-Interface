#include "Console.h"

Console::Console()
{
}

void Console::Log(const char* msg, LogLevel i_LogLevel, bool i_NewLine)
{

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);	
	const char* prefix = "";

	switch(i_LogLevel)
	{
		case 0:
			prefix = "[ERROR]: ";
			SetConsoleTextAttribute(hConsole, (0x0006));
			break;
		case 1:
			prefix = "[WARNING]: ";
			SetConsoleTextAttribute(hConsole, (FOREGROUND_RED));
			break;
		case 2:
			prefix = "";
			SetConsoleTextAttribute(hConsole, (FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN));
			break;
	}	

	if (i_LogLevel <= this->m_LogLevel)
	{		
		if (i_NewLine  || this->m_ForceNewLine)
			std::cout << std::endl << prefix << msg << std::endl;
		else
			std::cout << prefix << msg << std::endl;
			
	}
		
	
}

void Console::SetLevel(LogLevel i_LogLevel)
{
	this->m_LogLevel = i_LogLevel;
}


void Console::SetForceNewLine(bool i_ForceNewLine)
{
	this->m_ForceNewLine = i_ForceNewLine;
}

Console::~Console()
{
}
