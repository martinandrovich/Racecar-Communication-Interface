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
		Error = 0,
		Warning = 1,
		Info = 2
	};

// Methods
public:
	void SetLevel(LogLevel);
	void SetForceNewLine(bool);

	void Log(const char* msg, LogLevel = Info, bool NewLine = false);
	void Input();

// Variables
private:
	LogLevel m_LogLevel = Info;
	bool m_ForceNewLine = false;
};

