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
		GET			= 0xAA,
		SET			= 0x55,
		REPLY		= 0xBB,
		ERR			= 0XEE,
	};

	enum COMMAND
	{
		UNDEF		= 0x00,
		START		= 0x10,
		STOP		= 0x11,
		AUTO		= 0x12,
		MAPP		= 0x13,
		BROD		= 0x14,
		TRAJ		= 0x16,
		TEST		= 0x17,
		TACHOM		= 0x18,
	};

	enum BROADCAST
	{
		OFF			= 0x00,		  // 000	
		ALL			= 0b001 << 3, // 001
		TACHO		= 0b011 << 3, // 011
		VELOC		= 0b111 << 3, // 111
		ACCLR		= 0b101 << 3, // 101
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
	void listen(COMMAND var = UNDEF, int refresh = REFRESH_RATE);
	void listenRaw();

	// Device Control (Methods)
	void setSpeed(const int& speedPercentage);
	void setDutyCycle(const int& dutyCycle);
	void setBroadcast(BROADCAST mode);
	void enableAutonomous();
	void enableMapping();
	void enableTrajectory();
	void enableBreaktest(int distance);
	void getMapping();
	void getTrajectory();

	void pollData(COMMAND var = UNDEF);

// Variables & Data
private:
	
	SerialPort serialPort;

	bool polling = false;
	bool listening = false;

	COMMAND specifier = UNDEF;

};

