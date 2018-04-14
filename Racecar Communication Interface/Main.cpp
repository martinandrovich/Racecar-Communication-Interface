// Racecar Communication Interface
// Version 1.0.4

#include "src\Common.h"
#include "src\Defaults.h"

// Initialize Objects
Console		mainConsole;
Controller	mainController;

int main(void)
{
	// Init
	mainConsole.init();

	// Config Console
	mainConsole.setLevel(Console::Normal);
	mainConsole.setForceNewLine(true);

	// Input Loop
	while(true)
		mainConsole.input();

	// Exit
	return 0;
}