// Racecar Communication Interface
// Version 1.0.0
//
// Created by Martin Androvich


#define VERSION	1.0

#include "src\Console.h"
#include "src\SerialPort.h"

// Init
const char* versionNumber = "1.0.0";
Console MainConsole;

int main(void)
{
	// Init
	printf("RCI Version %s \n", versionNumber);
		
	// Config Console
	MainConsole.SetLevel(Console::Info);
	MainConsole.SetForceNewLine(false);

	// Print Test Data
	MainConsole.Log("This is some info.", Console::Info, true);
	MainConsole.Log("This is a warning.", Console::Warning);
	MainConsole.Log("This is an error!", Console::Error);

	std::cin.get();

	// Exit
	return 0;
}