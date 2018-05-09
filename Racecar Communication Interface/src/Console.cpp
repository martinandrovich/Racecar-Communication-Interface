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

	else if (command == "test")
		mainController.pollData(Controller::UNDEF);

	else if (!mainController.isConnected())
		this->log("Cannot execute command without COM connection.", Warning);

	// Set command [UPDATE NEEDED]
	else if (parsedCommand[0] == "set") {

		// Set speed
		if (parsedCommand[1] == "dutycycle")
			mainController.setDutyCycle(std::stoul(parsedCommand[2], nullptr, 0));

		// Set broadcast mode
		if (parsedCommand[1] == "broadcast")
		{
			if (parsedCommand[2] == "none")
				mainController.setBroadcast(Controller::OFF);

			if (parsedCommand[2] == "all")
				mainController.setBroadcast(Controller::ALL);

			if (parsedCommand[2] == "tacho")
				mainController.setBroadcast(Controller::TACHO);

			if (parsedCommand[2] == "veloc")
				mainController.setBroadcast(Controller::VELOC);

			if (parsedCommand[2] == "acclr")
				mainController.setBroadcast(Controller::ACCLR);
		}

		// Enable autonomous
		if (parsedCommand[1] == "auto")
			mainController.enableAutonomous();
		
		// Enable mapping
		if (parsedCommand[1] == "mapping")
			mainController.enableMapping();

		// Enable trajectory
		if (parsedCommand[1] == "trajectory")
			mainController.enableTrajectory();

		// Enable test mode
		if (parsedCommand[1] == "breaktest")
			mainController.enableBreaktest(std::stoul(parsedCommand[2], nullptr, 0));

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
		if (parsedCommand[1] == "tacho")
			mainController.sendTelegram(Controller::GET, Controller::TACHOM);

		// Get mapping data stream
		if (parsedCommand[1] == "mapping")
			mainController.getMapping();

		// Get trajectory data stream
		if (parsedCommand[1] == "trajectory")
			mainController.getTrajectory();

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
			mainController.listen(Controller::COMMAND::UNDEF);

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

		// Flush the buffer
		if (parsedCommand[1] == "flush")
		{
			mainController.getSerialController().flush();
		}
			
	}	

	else
		this->log("Unrecognized command.", Warning);

}

Console::~Console()
{
}
