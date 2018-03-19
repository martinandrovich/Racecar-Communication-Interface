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

public:
	SerialPort(const char*);
	~SerialPort();

// Handles & Variables
private:
	HANDLE handler;
	COMSTAT status;
	DWORD errors;

	bool connected;
	const char* COMport;

public:
	bool isConnected();

// Methods
public:
	bool Init();

	void Connect(const char* COMport);
	void Disconnect();
	bool WriteData(uint8_t);
};

