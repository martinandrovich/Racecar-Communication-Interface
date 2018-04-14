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

	std::thread* listenerThread;

	bool polling = false;
	bool listening = false;
	int  timeout = TIMEOUT;
	int  refreshRate = REFRESH_RATE;
	int  dataLength = DATA_LENGTH;
	int* callbackFunction = nullptr;

// Methods
public:

	void connect();
	void disconnect();
	void flush();

	void writeByte(uint8_t byte);
	char readByte();
	void readBuffer(uint8_t& buffer, int length = DATA_LENGTH);

	bool poll(int length = DATA_LENGTH);
	void listen(int length = DATA_LENGTH, int refresh = REFRESH_RATE);

	void listener();	

	void readAllBytes();
	void readContinuousData();

	bool isConnected();
	
};
