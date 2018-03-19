#include "SerialPort.h"
#include "Console.h"

SerialPort::SerialPort(const char* portName)
{

	this->connected = false;

	// Create Handler
	this->handler = CreateFileA(static_cast<LPCSTR>(portName),
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
		if (GetLastError() == ERROR_FILE_NOT_FOUND) {

			//printf("Handle was not attached. Port %s is not available\n", portName);
		}
		else
		{
			MainConsole.Log("Test");
			printf("Connection failed.\n");
			//std::cout << GetLastErrorAsString() << std::endl;
		}
	}
	else
	{
		DCB dcbSerialParameters = { 0 };

		if (!GetCommState(this->handler, &dcbSerialParameters)) {
			printf("Failed to retrieve current serial parameters.");
		}
		else {

			dcbSerialParameters.BaudRate = CBR_9600;
			dcbSerialParameters.ByteSize = 8;
			dcbSerialParameters.StopBits = ONESTOPBIT;
			dcbSerialParameters.Parity = NOPARITY;
			dcbSerialParameters.fDtrControl = DTR_CONTROL_ENABLE;

			if (!SetCommState(handler, &dcbSerialParameters))
			{
				printf("Could not set Serial port parameters.\n");
			}
			else
			{
				this->connected = true;
				PurgeComm(this->handler, PURGE_RXCLEAR | PURGE_TXCLEAR);
				Sleep(DEFAULT_WAIT);
			}
		}
	}

}



SerialPort::~SerialPort()
{
}
