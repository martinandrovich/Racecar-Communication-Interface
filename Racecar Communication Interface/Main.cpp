// Racecar Communication Interface
// Version 1.0.2
//
// Created by Martin Androvich

#include "src\Common.h"

// Initialize Objects
Console MainConsole;
Controller MainController;

int main(void)
{
	// Init
	MainConsole.InitLog();

	// Config Console
	MainConsole.SetLevel(Console::Info);
	MainConsole.SetForceNewLine(true);

	// Just a test

	// Test of stream parsing
	MainController.ParseStream(0x35);
	MainController.ParseStream(0xAA);
	MainController.ParseStream(0x21);

	// Test of stream parsing 2
	MainController.ParseStream(0xAA);
	MainController.ParseStream(0x21);
	MainController.ParseStream(0x34);

	// Input Loop
	while(1)
		MainConsole.Input();

	// Exit
	return 0;
}