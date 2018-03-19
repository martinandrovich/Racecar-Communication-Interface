#pragma once

#include <Windows.h>
#include <iostream>
#include <string>

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
	void ReadData();

	void TempRead();

	int readSerialPort(char *buffer, unsigned int buf_size);

	bool isConnected();
};

