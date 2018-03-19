#pragma once
#include <iostream>
#include <string>
#include <Windows.h>

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

	void Log(const char* msg, LogLevel = Info, bool _newline = false);
	void Input();

	void ExecuteCommand(std::string _command);

// Variables
private:
	LogLevel loglevel = Info;
	bool forcenewline = false;
};

