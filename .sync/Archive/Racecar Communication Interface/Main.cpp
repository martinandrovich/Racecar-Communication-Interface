// Racecar Communication Interface
// Version 1.0.0
//
// Created by Martin Androvich

#include "src\Console.h"
#include "src\SerialPort.h"

#define VERSION "1.0.0"
#define COM_PORT "\\\\.\\COM4"

Console MainConsole;
SerialPort MainSerialPort(COM_PORT);

int main(void)
{
	// Init
	printf("RCI Version %s \n", VERSION);
		
	// Config Console
	MainConsole.SetLevel(Console::Info);
	MainConsole.SetForceNewLine(true);

	// Input
	while(1)
		MainConsole.Input();

	// Exit
	return 0;
}