#include "Console.h"
#include "Common.h"

char receivedString[DATA_LENGTH];

std::string GetLastErrorAsString()
{
	//Get the error message, if any.
	DWORD errorMessageID = ::GetLastError();
	if (errorMessageID == 0)
		return std::string(); //No error message has been recorded

	LPSTR messageBuffer = nullptr;
	size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

	std::string message(messageBuffer, size);

	//Free the buffer.
	LocalFree(messageBuffer);

	return message;
}

SerialPort::SerialPort(const char* _COMport)
{
	this->connected = false;
	this->COMport = _COMport;
}

void SerialPort::Connect()
{
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
				MainConsole.Log("Connection established.", Console::Info, true);

				this->connected = true;

				PurgeComm(this->handler, PURGE_RXCLEAR | PURGE_TXCLEAR);
				Sleep(DEFAULT_WAIT);
			}
		}
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
		//std::cout << GetLastErrorAsString();
		MainConsole.Log("Data could not be written.", Console::Error, true);
		ClearCommError(this->handler, &this->errors, &this->status);
	}
	else
	{
		MainConsole.Log("Data sent.");
	}	
	
	return;
}

// Broken...
void SerialPort::ReadData()
{

	if (!isConnected())
	{
		MainConsole.Log("No active connection.", Console::Error, true);
		return;
	}

	DWORD bytesRead;

	char receivedString[DATA_LENGTH];
	char *buffer = receivedString;

	unsigned int toRead = 0;

	ClearCommError(this->handler, &this->errors, &this->status);

	// Determine buffer size
	if (this->status.cbInQue > 0) {

		if (this->status.cbInQue > DATA_LENGTH)
			toRead = DATA_LENGTH;

		else
			toRead = this->status.cbInQue;
	}

	MainConsole.Log("Attempting to read data...");

	if (ReadFile(this->handler, buffer, toRead, &bytesRead, NULL))
		printf("%s", bytesRead);

	else
	{
		MainConsole.Log("Data could not be read.", Console::Error, true);
		GetLastErrorAsString();
	}
	
}

int SerialPort::readSerialPort(char *buffer, unsigned int buf_size)
{
	DWORD bytesRead;
	unsigned int toRead = 0;
	
	ClearCommError(this->handler, &this->errors, &this->status);

	if (this->status.cbInQue > 0) {
		if (this->status.cbInQue > buf_size) {
			toRead = buf_size;
		}
		else toRead = this->status.cbInQue;
	}

	if (ReadFile(this->handler, buffer, toRead, &bytesRead, NULL)) return bytesRead;

	return 0;
}

void SerialPort::TempRead() {
	if (this->isConnected())
		{
		int hasRead = this->readSerialPort(receivedString, DATA_LENGTH);
		
		if (hasRead)
		{
			printf("%s\n", receivedString);
			printf("%X\n", receivedString);
		}
		else
			MainConsole.Log("Error occured reading data.", Console::Error, true);
	}
}

SerialPort::~SerialPort()
{
	if (this->connected)
	{
		this->connected = false;
		CloseHandle(this->handler);
	}
}
