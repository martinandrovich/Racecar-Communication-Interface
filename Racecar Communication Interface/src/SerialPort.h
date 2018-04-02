#pragma once

#include <Windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>

#include "Defaults.h"

class SerialPort
{
	// Constructors & Destructor
public:	

	SerialPort();
	SerialPort(const char* _COMport);
	~SerialPort();

// Handles & Variables
private:

	HANDLE	handler;
	COMSTAT	status;
	DWORD	errors;

	const char* COMport = COM_PORT;
	bool connected = false;

	std::thread* listener;

	bool polling = false;
	bool listening = false;
	int  timeout = TIMEOUT;
	int  refresh_rate = REFRESH_RATE;
	int  data_length = DATA_LENGTH;

// Methods
public:

	void Connect();
	void Disconnect();
	void Flush();

	void WriteByte(uint8_t _byte);
	char ReadByte();
	void ReadBuffer(uint8_t& _buffer, int _length = DATA_LENGTH);

	bool Poll(int _length = DATA_LENGTH);
	void Listen(int _length, int _refresh);

	void Listener();	

	void ReadAllData();
	void ReadContinuousData();

	bool isConnected();
	
};
