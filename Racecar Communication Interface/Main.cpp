// Racecar Communication Interface
// Version 1.0.4

#include "src\Common.h"
#include "src\Defaults.h"

// Initialize Objects
Console MainConsole;
Controller MainController;

int main(void)
{
	// Init
	MainConsole.Init();

	// Config Console
	MainConsole.SetLevel(Console::Normal);
	MainConsole.SetForceNewLine(true);

	// Input Loop
	while(true)
		MainConsole.Input();

	// Exit
	return 0;
}