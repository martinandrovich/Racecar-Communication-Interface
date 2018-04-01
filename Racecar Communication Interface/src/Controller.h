#pragma once

#include <string>
#include <vector>

#include "SerialPort.h"

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
		VAR1	= 0x12
	};

// Methods
public:

	// Communication
	void Connect();
	void Disconnect();
	bool IsConnected();

	SerialPort& GetSerialController();

	void SendTelegram(TYPE _type, COMMAND _command, uint8_t& _data);
	void ParseTelegram(uint8_t& _telegram);

	void ParseCommand(std::vector<int>& _command);
	void ParseStream(int _data);
	
	// !!! Get & Listen -> default ALL, else specify (maybe array?)
	void Set(COMMAND _var);
	int  Get(COMMAND _var = ALL, int _timeout = 2000);
	void Listen(void(*_callback), COMMAND _var = ALL, int _refresh = 50);
	void ListenRaw(bool _autoparse = false);

	// Device Control (Functions)
	void SetSpeed(const int& _speed);

	void PollData(COMMAND _var = ALL);

// Variables & Data
private:
	
	std::vector<int> command_buffer;
	SerialPort serial_port;

	int  dutycycle;
	bool polling = false;
	bool listening = false;

	float accl_data[3];
	float gyro_data[3];
};

