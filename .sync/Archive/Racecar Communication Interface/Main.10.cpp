// Racecar Communication Interface
// Version 1.0.0
//
// Created by Martin Androvich

#include "src\Console.h"
#include "src\SerialPort.h"

Console MainConsole;
const char* versionNumber = "1.0.0";

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

	MainConsole.Input();

	// Exit
	return 0;
}