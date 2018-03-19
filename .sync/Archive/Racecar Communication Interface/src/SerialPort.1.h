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

private:
	HANDLE handler;
	COMSTAT status;
	DWORD errors;

	bool connected;

public:
	SerialPort();
	~SerialPort();
};

