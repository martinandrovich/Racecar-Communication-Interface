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
		START	= 0x10,
		STOP	= 0x11,
		DATA1	= 0x01,
		DATA2	= 0x02,
		VAR1	= 0x12
	};

// Methods
public:

	void Connect();
	void Disconnect();
	bool IsConnected();

	void SendCommand(TYPE _type, COMMAND _command, uint8_t _data);
	void ParseCommand(std::vector<int>& _command);

	SerialPort& GetSerialController();

	void ParseStream(int _data);

	const auto& GetData(int& _variable) const;

	void SetSpeed(const int& _speed);

// Variables & Data
private:
	
	std::vector<int> command_buffer;
	SerialPort serial_port;

	int dutycycle;
	bool polling = false;

	float accl_data[3];
	float gyro_data[3];
};

