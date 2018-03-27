#pragma once

#include <Windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <sstream>
#include <iterator>
#include <cstdint>
#include <stdio.h>
#include <stdlib.h>

class Console
{
// Constructor & Destructor
public:
	Console();
	~Console();


// Logging Levels
public:
	enum LogLevel
	{
		Error,
		Warning,
		Info,
		Normal
	};

// Methods
public:
	void SetLevel(LogLevel);
	void SetForceNewLine(bool);

	void Init();
	void Log(const std::string& _msg, LogLevel = Normal, bool _newline = false);
	void Input();

	std::string OutputLastError();

	void ExecuteCommand(const std::string& _command);

// Variables
private:
	LogLevel loglevel = Normal;
	bool forcenewline = false;
};

