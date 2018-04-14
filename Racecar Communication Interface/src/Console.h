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

#include "Defaults.h"

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

	void setLevel(LogLevel logLevel);
	void setForceNewLine(bool forceNewLine);

	void init();
	void log(const std::string& msg, LogLevel = Normal, bool newline = false);
	void in();
	void input();

	std::string outputLastError();

	void executeCommand(const std::string& _command);

// Variables
private:
	LogLevel logLevel = Normal;
	bool forceNewLine = false;
};

