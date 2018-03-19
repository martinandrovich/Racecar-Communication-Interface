#pragma once

#include <string>
#include <vector>

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

	void SendCommand(TYPE _type, COMMAND _command, uint8_t _data);

	void ParseStream(int _data);
	void ParseCommand(std::vector<int>& _command);

	/*uint8_t AwaitReply(COMMAND _command);

	void SetSpeed(int& _speed);
	void ForceBreak();

	void RefreshVelocity();
	void RefreshAcceleration();
	void RefreshAll();
*/
	const auto& GetData(int& _variable) const;


// Variables & Data
private:
	
	// Buffer for incoming commands
	std::vector<int> command_buffer;

	int dutycycle;

	float accl_data[3];
	float gyro_data[3];
};

