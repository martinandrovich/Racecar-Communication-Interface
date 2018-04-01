#pragma once

#include <Windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>

class SerialPort
{
// Default Values

#define COM_PORT		"\\\\.\\COM5"
#define	TIMEOUT			2000
#define BUFFER_LENGTH	1
#define	DATA_LENGTH		4
#define	REFRESH_RATE	50

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

	void WriteByte(uint8_t _byte);
	char ReadByte();
	void ReadBuffer(uint8_t& _buffer, int _length);

	void Poll(int _length, int _timeout);
	void Listen(int _length, int _refresh);

	void Listener();
	void ClearBuffer();

	void ReadAllData();
	void ReadContinuousData();
	
	int  getQueue();

	bool isConnected();
	
};
