#pragma once

// Header Includes
#include "Console.h"
#include "SerialPort.h"
#include "Controller.h"

// Definitions
#define VERSION "1.0.1"
#define COM_PORT "\\\\.\\COM3"
#define DATA_LENGTH 1
#define DEFAULT_WAIT 2000

// Declarations
extern Console MainConsole;
extern SerialPort MainSerialPort;
extern Controller MainController;
