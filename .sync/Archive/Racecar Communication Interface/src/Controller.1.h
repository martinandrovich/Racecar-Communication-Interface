#pragma once

#include <string>

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
		GET = 0xAA,
		SET = 0x55,
		REPLY = 0xBB
	};

	enum COMMAND
	{
		START = 0x10,
		STOP = 0x11,
		DATA1 = 0x01,
		DATA2 = 0x02,
		VAR1 = 0x12
	};

// Methods
public:

	void Connect();

	void SendManualCommand(const std::string& _command);

	void SetSpeed(int _speed);
	void ForceBreak();

	void RefreshVelocity();
	void RefreshAcceleration();
	void RefreshAll();

	const auto& GetData(int _variable) const; 

// Variables or Measured Data  ???
private:
	int dutycycle;

	// Mutable because they might need to be refreshed, even when called by a const method.

	mutable int velocity;  // ???
	int acceleration;
	int distance;
	int total_distance;

// Sensor Data
// Just ideas.
private:
	float accl_data[3];
	float gyro_data[3];

};

