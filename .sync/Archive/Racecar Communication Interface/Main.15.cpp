// Racecar Communication Interface
// Version 1.0.0
//
// Created by Martin Androvich

#include "src\Common.h"

Console MainConsole;
SerialPort MainSerialPort(COM_PORT);

int main(void)
{
	// Init
	MainConsole.InitLog();

	// Config Console
	MainConsole.SetLevel(Console::Info);
	MainConsole.SetForceNewLine(true);

	// Input
	while(1)
		MainConsole.Input();

	// Exit
	return 0;
}