#include "SerialPort.h"
#include "Common.h"


// ###################################################################################################
// Serial Port

// Constructors & Destructor

SerialPort::SerialPort()
	: connected(false), COMport(COM_PORT) {}

SerialPort::SerialPort(const char* _COMport)
	: connected(false), COMport(_COMport) {}

SerialPort::~SerialPort()
{
	this->disconnect();

	if (this->listenerThread != nullptr)
	{
		this->listenerThread->join();
		delete this->listenerThread;
	}	
}

// Create HANDLE and connect to specified COMport
void SerialPort::connect()
{
	if (this->connected)
		this->disconnect();

	this->connected = false;

	mainConsole.log("Connecting...", Console::Normal, true);

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
			mainConsole.log("Handle could not be attatched; the specified COM port is unavailable.", Console::Error, true);
		}
		else
		{	
			mainConsole.log("Connection failed.", Console::Error, true);
			//std::cout << GetLastErrorAsString() << std::endl;
		}
	}
	else
	{
		DCB dcbSerialParameters = { 0 };

		if (!GetCommState(this->handler, &dcbSerialParameters))
		{
			mainConsole.log("Connection failed; failed to retrieve current serial parameters.", Console::Error, true);
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
				mainConsole.log("Connection failed; Could not set Serial port parameters.", Console::Error, true);
			}
			else
			{
				this->connected = true;

				PurgeComm(this->handler, PURGE_RXCLEAR | PURGE_TXCLEAR);
				Sleep(500);

				mainConsole.log("Connection established.", Console::Info, true);
			}
		}
	}
	
}

void SerialPort::disconnect()
{
	if (this->connected)
	{
		mainConsole.log("Closing connection...");

		this->connected = false;
		this->listening = false;

		CloseHandle(this->handler);
	}
}

bool SerialPort::isConnected()
{
	return this->connected;
}

void SerialPort::writeByte(uint8_t byte)
{
	
	DWORD transmittedBytes;
	char bytes[] = { byte };

	if (!this->connected)
	{
		mainConsole.log("No active connection.", Console::Error, true);
		return;
	}

	if (!WriteFile(this->handler, bytes, 1, &transmittedBytes, NULL))
	{
		mainConsole.log("Data could not be written.", Console::Error, true);
		mainConsole.outputLastError();
		ClearCommError(this->handler, &this->errors, &this->status);
	}

}

char SerialPort::readByte()
{

	// Check connection
	if (!this->connected)
	{
		mainConsole.log("No active connection.", Console::Error, true);
		return 0;
	}

	// Check queue
	ClearCommError(this->handler, &this->errors, &this->status);

	if (this->status.cbInQue == 0) {
		mainConsole.log("Empty data buffer queue.", Console::Warning, true);
		return 0;
	}

	// Read buffer
	DWORD bytesread;
	uint8_t buffer[1];

	if (ReadFile(this->handler, buffer, 1, &bytesread, NULL))
	{
		return buffer[0];
	}
	else
	{
		mainConsole.log("Error occured reading data.", Console::Error, true);
		mainConsole.outputLastError();
		return 0;
	}
	
}

void SerialPort::readBuffer(uint8_t& buffer, int length)
{
	// Check connection
	if (!isConnected())
	{
		mainConsole.log("No active connection.", Console::Error, true);
		return;
	}

	// Check queue
	ClearCommError(this->handler, &this->errors, &this->status);

	if (this->status.cbInQue == 0) {
		mainConsole.log("Empty data buffer queue.", Console::Warning, true);
		return;
	}

	// Read buffer
	DWORD bytesread;

	if (!ReadFile(this->handler, &buffer, length, &bytesread, NULL))
	{
		mainConsole.log("Error occured reading data.", Console::Error, true);
		mainConsole.outputLastError();
		return;
	}
}

void SerialPort::flush()
{
	PurgeComm(handler, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
	mainConsole.log("Buffer was flushed.", Console::Normal);
}

void SerialPort::readAllBytes()
{	
	int buffer;

	while (buffer = readByte())
	{
		if (buffer)
			printf("0x%X (%c)\n", buffer, buffer);
	}

}

void SerialPort::readContinuousData()
{
	
	mainConsole.log("Starting continuous buffer reading; press ESC to stop.\n", Console::Info);
	//Sleep(2000);

	while (true)
	{
		if (GetAsyncKeyState(VK_ESCAPE))
			break;

		readAllBytes();
	}

}

// ###################################################################################################
// Poller & Listener

bool SerialPort::poll(int length)
{
	
	ClearCommError(this->handler, &this->errors, &this->status);
	
	if (this->status.cbInQue >= length)
		return true;
	else
		return false;
}


void SerialPort::listen(int length, int refresh)
{
	
	mainConsole.log("Initializing listener, type \"listener stop\" to stop.", Console::Info);
	
	// Pause listener if running
	if (this->listening)
		this->listening = false;

	// Clear buffer
	this->flush();

	// Set settings
	this->refreshRate = refresh;
	this->dataLength = length;

	// Start listening
	this->listening = true;

	// Initialize Listener thread if needed
	if (this->listenerThread == nullptr)
		this->listenerThread = new std::thread(&SerialPort::listener, this);
	
}

void SerialPort::listener()
{
	using namespace std::literals::chrono_literals;
	auto threadID = std::this_thread::get_id();

	std::cout << "Listener thread created.\n";

	while (this->listening)
	{

		if (GetAsyncKeyState(VK_ESCAPE))
		{
			mainConsole.log("Listener has been stopped.", Console::Info);
			this->listening = false;
			break;
		}

		if (this->poll(dataLength))
		{

			uint8_t buffer[BUFFER_MAX_LENGTH];

			// Read buffer
			readBuffer(*buffer, dataLength);
			buffer[dataLength] = '\0';

			// Handle recieved data
			if (this->dataLength == 1)
			{
				printf("BYTE: 0x%X\n", buffer[0]);
				std::cout << std::endl << ">> ";
			}
			else
			{
				mainController.parseTelegram(buffer);	
			}

		}

		std::this_thread::sleep_for(50ms);
	}
}