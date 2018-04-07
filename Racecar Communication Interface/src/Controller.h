#pragma once

#include <string>
#include <vector>
#include <chrono>

#include "SerialPort.h"
#include "Defaults.h"

using Clock = std::chrono::high_resolution_clock;

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
	void Connect();
	void Disconnect();
	bool IsConnected();

	SerialPort& GetSerialController();

	void SendTelegram(TYPE _type, COMMAND _command, uint8_t _data = 0);
	void ParseTelegram(const uint8_t * _telegram);
	
	bool Set(COMMAND _var, int _value = 0, bool _verify = false);
	int  Get(COMMAND _var, int _timeout = TIMEOUT);
	void Listen(COMMAND _var = ALL, int _refresh = REFRESH_RATE);
	void ListenRaw(bool _autoparse = false);

	// Device Control (Methods)
	void SetSpeed(const int& _speed);
	void GetSpeed();

	void PollData(COMMAND _var = ALL);

// Variables & Data
private:
	
	SerialPort serial_port;

	bool polling = false;
	bool listening = false;

	COMMAND specifier = ALL;

};

