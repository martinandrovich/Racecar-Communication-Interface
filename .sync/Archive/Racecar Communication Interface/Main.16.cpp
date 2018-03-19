// Racecar Communication Interface
// Version 1.0.1
//
// Created by Martin Androvich

#include "src\Common.h"

Console MainConsole;
SerialPort MainSerialPort(COM_PORT);
Controller MainController;

int main(void)
{
	// Init
	MainConsole.InitLog();

	// Config Console
	MainConsole.SetLevel(Console::Info);
	MainConsole.SetForceNewLine(true);

	// Input Loop
	while(1)
		MainConsole.Input();

	// Exit
	return 0;
}