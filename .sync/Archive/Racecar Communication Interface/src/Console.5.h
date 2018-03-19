#pragma once

#include <Windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iterator>
#include <cstdint>
#include <stdio.h>
#include <stdlib.h>

class Console
{
public:
	Console();
	~Console();

	enum LogLevel
	{
		Error,
		Warning,
		Info
	};

// Methods
public:
	void SetLevel(LogLevel);
	void SetForceNewLine(bool);

	void InitLog();
	void Log(const std::string &_msg, LogLevel = Info, bool _newline = false);
	void Input();

	std::string OutputLastError();

	void ExecuteCommand(const std::string &_command);

// Variables
private:
	LogLevel loglevel = Info;
	bool forcenewline = false;
};

