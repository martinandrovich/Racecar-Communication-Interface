#pragma once

#include <Windows.h>
#include <iostream>
#include <string>
#include <vector>

class SerialPort
{
// Constructor & Destructor
public:
	SerialPort(const char* _COMport);
	~SerialPort();

// Handles & Variables
private:
	HANDLE handler;
	COMSTAT status;
	DWORD errors;

	bool connected;
	const char* COMport;

// Methods
public:
	void Connect();
	void Disconnect();

	void WriteData(uint8_t _byte);
	void ReadAllData();
	int ReadData();
	
	// Create an eventbased array buffer, that collects data stream and calls _eventFunction when data is added.
	void SetArrayBuffer(std::vector<int>& _arraybuffer, void(*_eventfunction));

	// Set a function to handle (parse) incoming data
	void SetStreamHandler(void(*_eventfunction(int)));

	bool isConnected();
};

