#include "Console.h"
#include "Common.h"

Console::Console()
{
}

// ###################################################################################################
// Logging

void Console::init()
{
	system("CLS");
	printf("RCI Version %s \n", VERSION);
}

void Console::setLevel(LogLevel logLevel)
{
	this->logLevel = logLevel;
}

void Console::setForceNewLine(bool forceNewLine)
{
	this->forceNewLine = forceNewLine;
}

void Console::log(const std::string& msg, LogLevel logLevel, bool newLine)
{

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);	
	std::string prefix = "";

	switch(logLevel)
	{
		case LogLevel::Error:
			prefix = "[ERROR]: ";
			SetConsoleTextAttribute(hConsole, (FOREGROUND_RED));
			break;
		case LogLevel::Warning:
			prefix = "[WARNING]: ";
			SetConsoleTextAttribute(hConsole, (0x0006));
			break;
		case LogLevel::Info:
			//prefix = "[ALERT]: ";
			SetConsoleTextAttribute(hConsole, (FOREGROUND_GREEN));
			break;
		case LogLevel::Normal:
			prefix = "";
			SetConsoleTextAttribute(hConsole, (FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN));
			break;
	}	

	if (logLevel <= this->logLevel)
	{		
		if (newLine || this->forceNewLine)
			std::cout << std::endl << prefix << msg << std::endl;
		else
			std::cout << prefix << msg << std::endl;
	}

	SetConsoleTextAttribute(hConsole, (FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN));
}

void Console::in()
{
	std::cout << std::endl << ">> ";
}

void Console::input()
{
	std::string command;

	in();
	std::getline(std::cin, command);

	executeCommand(command);
}

std::string Console::outputLastError()
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


// ###################################################################################################
// Command Parser

void Console::executeCommand(const std::string& command)
{
	
	// Split _command by spaces into (dynamic) array
	std::istringstream iss(command);
	std::vector<std::string> parsedCommand(std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>());

	/*auto command_type = parsedcommand[0];
	auto command_arg1 = parsedcommand[1];*/

	// !!!
	// Needs checking of command (input string) bounds

	if (command == "")
		this->log("NULL Command.", Warning);

	else if (command == "exit")
	{
		mainController.disconnect();
		while (mainController.isConnected())
			; // Wait
		exit(0);
	}

	else if (command == "cls")
		init();

	else if (command == "connect")
		mainController.connect();

	else if (!mainController.isConnected())
		this->log("Cannot execute command without COM connection.", Warning);

	// Test commands for MIC board
	else if (parsedCommand[0] == "test") {

		// Test #1 (SET 0x00_01 -> LED_OFF)
		if (parsedCommand[1] == "ledoff")
			mainController.set(Controller::DATA1);

		// Test #2 (SET 0x00_02 -> LED_ON)
		if (parsedCommand[1] == "ledon")
			mainController.set(Controller::DATA2);

		// Test #3A (SET 0x00_03 -> SET_LED)
		if (parsedCommand[1] == "setled")
			mainController.set(Controller::DATA3, std::stoul(parsedCommand[2], nullptr, 0));

		// Test #3B (SET 0x00_03 -> SET_LED) w/ Verification
		if (parsedCommand[1] == "setledver")
		{
			int  setVal = std::stoul(parsedCommand[2], nullptr, 0);
			bool result = mainController.set(Controller::DATA3, setVal, true);

			if (result)
				printf("\n[DATA: 0x%X] was sent!\n", setVal);
			else
				printf("\nData verification failed.");
		}			

		// Test #4 (SET 0x00_12 -> DO_ECHO)
		if (parsedCommand[1] == "echo")
			mainController.sendTelegram(Controller::SET, Controller::VAR1);

		// Test #5 (GET 0x00_12 -> SEND_TEST_TELEGRAM)
		if (parsedCommand[1] == "gettele")
			printf("Recieved [DATA: 0x%X] \n", mainController.get(Controller::VAR1));

		// Test #6 (GET 0x00_03 -> GET_LED)
		if (parsedCommand[1] == "getled")
			printf("Recieved [DATA: 0x%X] \n", mainController.get(Controller::DATA3));
	}

	// Set command [UPDATE NEEDED]
	else if (parsedCommand[0] == "set") {

		// Set speed
		if (parsedCommand[1] == "speed")
			mainController.getSerialController().writeByte(std::stoul(parsedCommand[2], nullptr, 0));

		// Set variable1
		if (parsedCommand[1] == "variable1")
			mainController.getSerialController().writeByte(std::stoul(parsedCommand[2], nullptr, 0));
	}

	// Get command [TODO]
	else if (parsedCommand[0] == "get") {

		// Get all variables in their current state [TODO]
		if (parsedCommand[1] == "all")
			mainController.pollData();

		// Get specific variable in its current state [TODO]
		if (parsedCommand[1] == "variable1")
			mainController.getSerialController().writeByte(std::stoul(parsedCommand[2], nullptr, 0));
	}

	// Listen command
	else if (parsedCommand[0] == "listen") {

		// Listen for all variables in their current state
		if (parsedCommand[1] == "all")
			mainController.listen();

		// Listen for any single bytes
		if (parsedCommand[1] == "raw")
			mainController.listenRaw();

		// Listen for a specific variable in its current state
		if (parsedCommand[1] == "var1")
			mainController.listen(Controller::COMMAND::VAR1);

	}

	// Read command [DEPRECATED]
	else if (parsedCommand[0] == "read") {

		// Read latest byte
		if (parsedCommand[1] == "last")
		{
			int read_data = mainController.getSerialController().readByte();
			if (read_data)
				printf("0x%X (%c)\n", read_data, read_data);
		}

		// Read whole buffer
		if (parsedCommand[1] == "buffer")
		{
			mainController.getSerialController().readAllBytes();
		}
			
	}	

	else
		this->log("Unrecognized command.", Warning);

}

Console::~Console()
{
}
