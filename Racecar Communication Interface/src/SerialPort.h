#pragma once

#include <Windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <thread>

class SerialPort
{
// Constructors & Destructor
public:

	SerialPort();
	SerialPort(const char* _COMport);
	~SerialPort();

// Structures
private:

	struct Listener {

		Listener();
		~Listener();

		static const int a = 5;

		static const int refresh_rate = 50;
		bool listening = false;

		static void ret();

		void Listen();
		void Stop();
		void SetHandler(void(*_eventfunction));

		bool isListening();


	};

// Handles & Variables
private:

	HANDLE handler;
	COMSTAT status;
	DWORD errors;

	Listener listener;

	bool connected;
	const char* COMport;

// Methods
public:

	void Connect();
	void Disconnect();

	void SomeFunction();

	void WriteData(uint8_t _byte);
	void ReadAllData();
	void ReadContinuousData();
	int ReadData();
	
	// Create an eventbased array buffer, that collects data stream and calls _eventFunction when data is added.
	void SetArrayBuffer(std::vector<int>& _arraybuffer, void(*_eventfunction));

	// Set a function to handle (parse) incoming data
	void SetStreamHandler();
	void SetStreamHandler(void(*_eventfunction(int)));

	bool isConnected();
	
};
