#include "SerialPort.h"
#include "Common.h"


// ###################################################################################################
// Serial Port

// https://www.codeproject.com/Articles/2682/Serial-Communication-in-Windows

// Constructors & Destructor

SerialPort::SerialPort()
	: connected(false), COMport(COM_PORT) {}

SerialPort::SerialPort(const char* _COMport)
	: connected(false), COMport(_COMport) {}

SerialPort::~SerialPort()
{
	this->Disconnect();
	listener->join();
	delete listener;
}

// Create HANDLE and connect to specified COMport
void SerialPort::Connect()
{
	if (this->connected)
		Disconnect();

	this->connected = false;

	MainConsole.Log("Connecting...", Console::Normal, true);

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
				Sleep(TIMEOUT);

				MainConsole.Log("Connection established.", Console::Normal, true);
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
		this->listening = false;

		CloseHandle(this->handler);
	}
}

bool SerialPort::isConnected()
{
	return connected;
}

void SerialPort::WriteByte(uint8_t _byte)
{
	
	DWORD transmittedBytes;
	char bytes[] = { _byte };

	if (!isConnected())
	{
		MainConsole.Log("No active connection.", Console::Error, true);
		return;
	}

	MainConsole.Log("Attempting to write data...");

	if (!WriteFile(this->handler, bytes, 1, &transmittedBytes, NULL))
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

int SerialPort::ReadByte()
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
		//MainConsole.Log("Empty data buffer queue.", Console::Warning, true);
		return 0;
	}

	DWORD bytesread;
	char buffer[1];

	// !!! Try passing buffer as ref instead of value (&)

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
		buffer = ReadByte();
		if (buffer)
			printf("0x%X (%c)\n", buffer, buffer);
	}

}

void SerialPort::ReadContinuousData()
{
	
	MainConsole.Log("Starting continuous buffer reading; press ESC to stop.\n", Console::Info);
	//Sleep(2000);

	while (true)
	{
		if (GetAsyncKeyState(VK_ESCAPE))
			break;

		ReadAllData();
	}

}

void SerialPort::Listener()
{
	auto thID = std::this_thread::get_id();

	std::cout << "Listener thread created." << std::endl;
	using namespace std::literals::chrono_literals;

	while (listening)
	{

		ClearCommError(this->handler, &this->errors, &this->status);

		if (this->status.cbInQue >= data_length)
		{
			MainConsole.Log("Telegram recieved:\n", Console::Info);
			ReadAllData();
			std::cout << std::endl << ">> ";
		}

		std::this_thread::sleep_for(50ms);
	}
}

void SerialPort::Listen(int _length, int _refresh)
{
	if (listener != nullptr)
	{
		MainConsole.Log("A listener thread already exists.", Console::Warning);
		return;
	}
	
	MainConsole.Log("Initializing listener, type \"listener pause\" to toggle pause.", Console::Info);

	Sleep(2000);

	this->listening = true;
	listener = new std::thread(&SerialPort::Listener, this);
}