#include "Controller.h"
#include "Common.h"


// ###################################################################################################
// Constructor & Destructor

Controller::Controller() {}

Controller::~Controller() {}

// ###################################################################################################
// Connectivity Methods

void Controller::connect()
{
	this->serialPort.connect();
}

void Controller::disconnect()
{
	this->serialPort.disconnect();
}

bool Controller::isConnected()
{
	return this->serialPort.isConnected();
}

SerialPort& Controller::getSerialController()
{
	return this->serialPort;
}

// ###################################################################################################
// Communication Protocol

void Controller::sendTelegram(TYPE type, COMMAND command, uint8_t data)
{
	mainConsole.log("Sending telegram.", Console::Normal, false);
	printf("[TYPE: 0x%X] | [COMMAND: 0x%X] | [DATA: 0x%X]\n", type, command, data);

	// !!! Show contents of sent telegram? i.e. Sending telegram: [0xAA | 0x11 | 0x00].

	this->serialPort.writeByte(type);
	this->serialPort.writeByte(command);
	this->serialPort.writeByte(data);
}

void Controller::parseTelegram(const uint8_t * telegram)
{
	// Check TYPE to be REPLY
	if (telegram[0] != TYPE::REPLY)
	{
		mainConsole.log("Recieved telegram is of incompatible TYPE.", Console::Error);
		serialPort.flush();
		return;
	}

	// If looking for specific COMMAND and not found, then return.
	if (this->specifier != UNDEF && telegram[1] != this->specifier)
		return;

	// Assemble Data:
	uint16_t dataLong = (telegram[2] << 8) | telegram[3];

	mainConsole.log("\nTelegram recieved:", Console::Info);
	printf("Recieved [TYPE: 0x%X] with [COMMAND: 0x%X] and [DATA: 0x%X]\n", telegram[0], telegram[1], dataLong);

	// Select appropriate function
	switch (telegram[1])
	{
		case COMMAND::MAPP:
			break;

		case COMMAND::AUTO:
			break;
	}

	// Reset listener specifier & console
	this->specifier == Controller::UNDEF;
	
	if (this->listening)
		mainConsole.in();
}

bool Controller::set(COMMAND var, int value, bool verify)
{
	// Send SET telegram
	this->sendTelegram(Controller::SET, var, value);

	// Verify
	if (verify && value != 0)
	{
		Sleep(PAUSE);
		std::cout << "Verifying data..." << std::endl;

		if (value == get(var))
			return true;
		else
			return false;
	}
	else
	{
		return true;
	}
		
}

int Controller::get(COMMAND var, int timeout)
{

	if (this->listening)
	{
		mainConsole.log("Cannot GET while listener is active.", Console::Error);
		return 0;
	}

	// Send GET telegram
	this->sendTelegram(Controller::GET, var);

	// Setup poll
	this->polling = true;

	auto timeStarted = chronoClock::now();
	auto timeCurrent = chronoClock::now();
	auto timeDelta   = std::chrono::duration_cast<std::chrono::milliseconds>(timeCurrent - timeStarted).count();	

	// Start polling
	while (this->polling)
	{
		// Check timeout
		timeCurrent = chronoClock::now();
		timeDelta   = std::chrono::duration_cast<std::chrono::milliseconds>(timeCurrent - timeStarted).count();

		if (timeDelta == timeout)
			this->polling = false;		

		// Check buffer
		if (this->serialPort.poll())
		{
			
			uint8_t buffer[BUFFER_MAX_LENGTH];
			this->serialPort.readBuffer(*buffer);

			if (buffer[0] == TYPE::REPLY && buffer[1] == var)
			{
				this->polling = false;
				uint16_t dataLong = (buffer[2] << 8) | buffer[3];
				return dataLong;
			}
		}

	}

	mainConsole.log("Could not GET; the function timed out.", Console::Error);
	this->serialPort.flush();
	return 0;
}

void Controller::listen(COMMAND var, int refresh)
{
	this->specifier = var;
	this->listening = true;
	this->serialPort.listen(4, 50);
}

void Controller::listenRaw()
{
	this->listening = true;
	this->serialPort.listen(1, 50);
}
	
// ###################################################################################################
// Device Control

void Controller::pollData(COMMAND var)
{
	mainConsole.log("Starting data polling; press ESC to stop.\n", Console::Info);

	Sleep(2000);
	
	this->polling = true;

	int a = 0;
	int b = 0;

	while (true)
	{
		if (GetAsyncKeyState(VK_ESCAPE))
			break;

		a += 1;
		b += 7;

		std::cout << "\r" << "Speed: " << a << " | Distance: " << b;;
	}

	this->polling = false;
	std::cout << std::endl;
}

void Controller::setSpeed(const int& speedPercentage)
{
	this->serialPort.writeByte(speedPercentage);
}

void Controller::setDutyCycle(const int& dutyCycle)
{
	this->set(Controller::START, dutyCycle);
}

void Controller::setBroadcast(BROADCAST mode)
{
	this->set(Controller::BROD, mode);
}

void Controller::enableAutonomous()
{
	this->set(Controller::AUTO);
}

void Controller::enableMapping()
{
	this->set(Controller::MAPP);
}

void Controller::enableTrajectory()
{
	this->set(Controller::TRAJ);
}

void Controller::enableBreaktest(int distance)
{
	this->set(Controller::TEST, distance);
}

void Controller::getMapping()
{
	this->sendTelegram(Controller::GET, Controller::MAPP, 0);
}

void Controller::getTrajectory()
{
	this->sendTelegram(Controller::GET, Controller::TRAJ, 0);
}