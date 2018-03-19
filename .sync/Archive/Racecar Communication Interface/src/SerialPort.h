#pragma once

#include <cstdint>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <string>
#include <iostream>

#define DATA_LENGTH 255
#define DEFAULT_WAIT 2000

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
	void WriteData(uint8_t);
	void ReadData();

	void TempRead();

	int readSerialPort(char *buffer, unsigned int buf_size);

	bool isConnected();
};

