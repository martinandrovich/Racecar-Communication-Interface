#pragma once

#include <string>
#include <vector>
#include <chrono>

#include "SerialPort.h"
#include "Defaults.h"

using chronoClock = std::chrono::high_resolution_clock;

class Controller
{

// Constructor & Destructor
public:
	Controller();
	~Controller();

// Protocol Definitions
public:

	enum TYPE
	{
		GET		= 0xAA,
		SET		= 0x55,
		REPLY	= 0xBB,
		ERR		= 0XEE
	};

	enum COMMAND
	{
		ALL		= 0x00,
		START	= 0x10,
		STOP	= 0x11,
		DATA1	= 0x01,
		DATA2	= 0x02,
		DATA3	= 0x03,
		VAR1	= 0x12
	};

// Methods
public:

	// Communication
	void connect();
	void disconnect();
	bool isConnected();

	SerialPort& getSerialController();

	void sendTelegram(TYPE type, COMMAND command, uint8_t data = 0);
	void parseTelegram(const uint8_t * telegram);
	
	bool set(COMMAND var, int value = 0, bool verify = false);
	int  get(COMMAND var, int timeout = TIMEOUT);
	void listen(COMMAND var = ALL, int refresh = REFRESH_RATE);
	void listenRaw();

	// Device Control (Methods)
	void setSpeed(const int& speedPercentage);
	void setDutyCycle(const int& dutyCycle);
	void getSpeed();

	void pollData(COMMAND var = ALL);

// Variables & Data
private:
	
	SerialPort serialPort;

	bool polling = false;
	bool listening = false;

	COMMAND specifier = ALL;

};

