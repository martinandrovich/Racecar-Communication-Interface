#include "SerialPort.h"
#include "Common.h"

char receivedString[DATA_LENGTH];

// Constructor
SerialPort::SerialPort(const char* _COMport)
	: connected(false), COMport(_COMport) {}


// Create HANDLE and connect to specified COMport
void SerialPort::Connect()
{
	if (this->connected)
		Disconnect();

	this->connected = false;

	MainConsole.Log("Connecting...", Console::Info, true);

	// Create Handler
	this->handler = CreateFileA(static_cast<LPCSTR>(COMport),
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);

	// Initialize Handler
	if (this->handler == INVALID_HANDLE_VALUE)
	{
		if (GetLastError() == ERROR_FILE_NOT_FOUND)
		{
			MainConsole.Log("Handle could not be attatched; the specified COM port is unavailable.", Console::Error, true);
		}
		else
		{	
			MainConsole.Log("Connection failed.", Console::Error, true);
			//std::cout << GetLastErrorAsString() << std::endl;
		}
	}
	else
	{
		DCB dcbSerialParameters = { 0 };

		if (!GetCommState(this->handler, &dcbSerialParameters))
		{
			MainConsole.Log("Connection failed; failed to retrieve current serial parameters.", Console::Error, true);
		}
		else
		{
			dcbSerialParameters.BaudRate = CBR_9600;
			dcbSerialParameters.ByteSize = 8;
			dcbSerialParameters.StopBits = ONESTOPBIT;
			dcbSerialParameters.Parity = NOPARITY;
			dcbSerialParameters.fDtrControl = DTR_CONTROL_ENABLE;

			if (!SetCommState(handler, &dcbSerialParameters))
			{
				MainConsole.Log("Connection failed; Could not set Serial port parameters.", Console::Error, true);
			}
			else
			{
				this->connected = true;

				PurgeComm(this->handler, PURGE_RXCLEAR | PURGE_TXCLEAR);
				Sleep(DEFAULT_WAIT);

				MainConsole.Log("Connection established.", Console::Info, true);
			}
		}
	}
	
}

void SerialPort::Disconnect()
{
	if (this->connected)
	{
		MainConsole.Log("Closing connection...");

		this->connected = false;
		CloseHandle(this->handler);
	}
}

bool SerialPort::isConnected()
{
	return connected;
}

void SerialPort::WriteData(uint8_t _byte)
{
	
	DWORD transmittedBytes;
	char bytes[] = { _byte };

	if (!isConnected())
	{
		MainConsole.Log("No active connection.", Console::Error, true);
		return;
	}

	MainConsole.Log("Attempting to write data...");

	if (!WriteFile(this->handler, bytes, DATA_LENGTH, &transmittedBytes, NULL))
	{
		MainConsole.Log("Data could not be written.", Console::Error, true);
		MainConsole.OutputLastError();
		ClearCommError(this->handler, &this->errors, &this->status);
	}
	else
	{
		MainConsole.Log("Data sent.");
	}	
	
	return;
}

int SerialPort::ReadData()
{

	// Check connection
	if (!isConnected())
	{
		MainConsole.Log("No active connection.", Console::Error, true);
		return 0;
	}

	// Check queue

	ClearCommError(this->handler, &this->errors, &this->status);

	if (this->status.cbInQue == 0) {
		MainConsole.Log("Empty data buffer queue.", Console::Warning, true);
		return 0;
	}

	DWORD bytesread;
	char buffer[1];

	if (ReadFile(this->handler, buffer, 1, &bytesread, NULL))
	{
		return buffer[0];
	}
	else
	{
		MainConsole.Log("Error occured reading data.", Console::Error, true);
		MainConsole.OutputLastError();
		return 0;
	}
	
}

void SerialPort::ReadAllData()
{	
	int buffer = 1;

	while (buffer)
	{
		buffer = ReadData();
		if (buffer)
			printf("0x%X (%c)\n", buffer, buffer);
	}

}


SerialPort::~SerialPort()
{
	this->Disconnect();
}
