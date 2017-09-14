/*********************************************
 * @file core_microscope_xy.cpp              *
 *                                           *
 * Microscope XY Table Contorl               *
 *                                           *
 * @author Joshua <jan.joshua@iac.com.tw>    *
 *********************************************/

#include "core_common.h"

// Constructor
MicroscopeXY::MicroscopeXY()
{
	if (initDriver())
	{
		_position.X = 0;
		_position.Y = 0;
		_position.Z = 0;
		_speed = RPM197;
	}
	else
	{
		// Error Message
	}

}

MicroscopeXY::MicroscopeXY(RS485Port* PortPtr)
{
	if (initDriver())
	{
		_position.X = 0;
		_position.Y = 0;
		_position.Z = 0;
		_speed = RPM197;
		_rs485Port = PortPtr;
		_ghMutex = _rs485Port->getMutex();;
	}
	else
	{
		// Error Message
	}

}

MicroscopeXY::MicroscopeXY(Position Pos, RS485Port* PortPtr)
{
	if (initDriver())
	{
		_position.X = Pos.X;
		_position.Y = Pos.Y;
		_position.Z = Pos.Z;
		_rs485Port = PortPtr;
		_ghMutex = _rs485Port->getMutex();
	}
	else
	{
		// Error Message
	}
}

MicroscopeXY::MicroscopeXY(double X, double Y, RS485Port* PortPtr)
{
	if (initDriver())
	{
		_position.X = X;
		_position.Y = Y;
		_position.Z = 0;
		_rs485Port = PortPtr;
		_ghMutex = _rs485Port->getMutex();
	}
	else
	{
		// Error Message
	}
}

// Destructor
MicroscopeXY::~MicroscopeXY()
{
	// Release memory if it's needed
}

// Information
Position MicroscopeXY::getPositon() const
{
	return _position;
}

double MicroscopeXY::getX() const
{
	return _position.X;
}

double MicroscopeXY::getY() const
{
	return _position.Y;
}

double MicroscopeXY::getSpeed() const
{
	return _speed;
}

// Action
bool MicroscopeXY::move2Pos(Position Pos)
{
	double xLength = _position.X - Pos.X;
	uint8_t xDirection;
	double yLength = _position.Y - Pos.Y;
	uint8_t yDirection;

	WaitForSingleObject(_ghMutex, INFINITE);
	if (xLength > 0)
	{
		xDirection = NegativeExecute;
	}
	else
	{
		xLength = 0 - xLength;
		xDirection = PositiveExecute;
	}

	if (!moveX(xLength, xDirection)) // If motor did not move successfully
	{
		// Error Message
		return false;
	}
	ReleaseMutex(_ghMutex);
	Sleep(500);

	WaitForSingleObject(_ghMutex, INFINITE);
	if (yLength > 0)
	{
		yDirection = NegativeExecute;
	}
	else
	{
		yLength = 0 - yLength;
		yDirection = PositiveExecute;
	}

	if (!moveY(yLength, yDirection)) // If motor did not move successfully
	{
		// Error Message
		return false;
	}
	ReleaseMutex(_ghMutex);
	Sleep(500);

	return true;
}

bool MicroscopeXY::move2Pos(double X, double Y)
{
	double xLength = _position.X - X;
	uint8_t xDirection;
	double yLength = _position.Y - Y;
	uint8_t yDirection;

	///*
	WaitForSingleObject(_ghMutex, INFINITE);
	if (xLength > 0)
	{
		xDirection = NegativeExecute;
	}
	else
	{
		xLength = 0 - xLength;
		xDirection = PositiveExecute;
	}

	if (!moveX(xLength, xDirection)) // If motor did not move successfully
	{
		// Error Message
		return false;
	}
	Sleep(500);
	ReleaseMutex(_ghMutex);

	//*/

	WaitForSingleObject(_ghMutex, INFINITE);
	if (yLength > 0)
	{
		yDirection = NegativeExecute;
	}
	else
	{
		yLength = 0 - yLength;
		yDirection = PositiveExecute;
	}

	if (!moveY(yLength, yDirection)) // If motor did not move successfully
	{
		// Error Message
		return false;
	}
	Sleep(500);
	ReleaseMutex(_ghMutex);

	return true;
}

// Modifier
bool MicroscopeXY::initDriver()
{
	if (false) // If motor driver cannot be init
	{
		// Error Message
		return false;
	}

	return true;
}

bool MicroscopeXY::setSpeed(double Speed)
{
	if (false) // If motor driver cannot set speed successfully
	{
		// Error Message
		return false;
	}

	return true;
}

bool MicroscopeXY::setPort(RS485Port* PortPtr)
{
	_rs485Port = PortPtr;
	_ghMutex = _rs485Port->getMutex();
	return true;
}

// Combined with the driver
bool MicroscopeXY::moveX(double X, uint8_t Direction)
{
	uint8_t address = '\x03';
	int steps = (int)(X * 400.0);
	Message* msgPtr = trans2RTCMD(NumberToString(_speed).c_str(), address, Direction, NumberToString(steps).c_str(), "50", "50");

	/*
	if (Direction == PositiveExecute && (_position.X + X) > 90.0)
	{
	printf("ShakeMachine: moveArm Error, Over 90.0 degree\n");
	return false;
	}
	else if (Direction == NegativeExecute && (_armPosition - Degree) < 0.0)
	{
	printf("ShakeMachine: moveArm Error, Over 90.0 degree\n");
	return false;
	}
	*/

	Message* feedback = NULL;
	SYSTEMTIME msgSend;
	clock_t dwMsgSend = clock();

	//WaitForSingleObject(_ghMutex, INFINITE); // try
	_rs485Port->clearMsg(address);
	if (c_serial_write_data(_rs485Port->getPortHandle(), msgPtr->content, &msgPtr->length) < 0)
	{
		printf("MicroscopeXY: moveX TX ERROR\n");
	}
	else
	{
		GetLocalTime(&msgSend);
		printf("Address %.2X Message Send at %02d:%02d:%02d.%03d\n", address, msgSend.wHour, msgSend.wMinute, msgSend.wSecond, msgSend.wMilliseconds);
	}
	//ReleaseMutex(_ghMutex); // try

	bool controlRecieved = false;
	while (true)
	{
		feedback = _rs485Port->getControlerMsg(address);

		if (feedback != NULL)
		{
			//WaitForSingleObject(_ghMutex, INFINITE); // try
			printf("Address %.2X recieve: ", address);
			for (int i = 0; i < feedback->length; i++)
			{
				printf("%.2X ", feedback->content[i]);
			}
			SYSTEMTIME systemtime;
			GetLocalTime(&systemtime);
			printf(" currentDateTime() = %02d:%02d:%02d.%03d\n", systemtime.wHour, systemtime.wMinute, systemtime.wSecond, systemtime.wMilliseconds);
			//ReleaseMutex(_ghMutex); // try

			/*
			if (feedback->content[0] == (uint8_t) '\xB5' && feedback->content[1] == address)
			{
			WaitForSingleObject(_ghMutex, INFINITE); // try
			printf("Resend Command\n");
			c_serial_write_data(_rs485Port->getPortHandle(), msgPtr->content, &msgPtr->length);
			ReleaseMutex(_ghMutex); // try
			continue;
			}
			*/

			if (feedback->content[0] == (uint8_t) '\xB1' && feedback->content[1] == address)
			{
				controlRecieved = true;
				printf("Address %.2X Controler Received\n", address);
				continue;
			}

			if (feedback->content[0] == (uint8_t) '\xB0' && feedback->content[1] == address)
			{
				printf("Address %.2X moveX Finished\n", address);
				_rs485Port->clearMsg(address);
				break;
			}
			if (feedback->content[0] == (uint8_t) '\xA0' && feedback->content[1] == address)
			{
				printf("Address %.2X moveX Finished\n", address);
				_rs485Port->clearMsg(address);
				break;
			}
			if (feedback->content[0] == (uint8_t) '\xA1' && feedback->content[1] == address)
			{
				printf("Address %.2X moveX Finished\n", address);
				_rs485Port->clearMsg(address);
				break;
			}
			delete[] feedback->content;
			delete feedback;
		}
		///*
		//WaitForSingleObject(_ghMutex, INFINITE); // try
		if (!controlRecieved)
		{
			clock_t dwCurrent = clock();
			if (dwCurrent - dwMsgSend > 1000)
			{
				printf("Address %.2X not recieve B1\n", address);
				printf("MicroscopeXY: moveX TX Timeout\n");
				if (c_serial_write_data(_rs485Port->getPortHandle(), msgPtr->content, &msgPtr->length) < 0)
				{
					printf("MicroscopeXY: moveX TX ERROR\n");
				}
				else
				{
					dwMsgSend = clock();
					GetLocalTime(&msgSend);
					printf("Address %.2X Message Send at %02d:%02d:%02d.%03d\n", address, msgSend.wHour, msgSend.wMinute, msgSend.wSecond, msgSend.wMilliseconds);
				}
			}
		}
		//ReleaseMutex(_ghMutex); // try
		//*/
		Sleep(100);
	}
	delete[]  msgPtr->content;

	return true;
}

bool MicroscopeXY::moveY(double Y, uint8_t Direction)
{
	uint8_t address = '\x02';
	int steps = (int)(Y * 400.0);
	Message* msgPtr = trans2RTCMD(NumberToString(_speed).c_str(), address, Direction, NumberToString(steps).c_str(), "50", "50");

	/*
	if (Direction == PositiveExecute && (_position.X + X) > 90.0)
	{
	printf("ShakeMachine: moveArm Error, Over 90.0 degree\n");
	return false;
	}
	else if (Direction == NegativeExecute && (_armPosition - Degree) < 0.0)
	{
	printf("ShakeMachine: moveArm Error, Over 90.0 degree\n");
	return false;
	}
	*/

	Message* feedback = NULL;
	SYSTEMTIME msgSend;
	clock_t dwMsgSend = clock();

	//WaitForSingleObject(_ghMutex, INFINITE); // try
	_rs485Port->clearMsg(address);
	if (c_serial_write_data(_rs485Port->getPortHandle(), msgPtr->content, &msgPtr->length) < 0)
	{
		printf("MicroscopeXY: moveY TX ERROR\n");
	}
	else
	{
		GetLocalTime(&msgSend);
		printf("Address %.2X Message Send at %02d:%02d:%02d.%03d\n", address, msgSend.wHour, msgSend.wMinute, msgSend.wSecond, msgSend.wMilliseconds);
	}
	//ReleaseMutex(_ghMutex); // try

	bool controlRecieved = false;
	while (true)
	{
		feedback = _rs485Port->getControlerMsg(address);

		if (feedback != NULL)
		{
			//WaitForSingleObject(_ghMutex, INFINITE); // try
			printf("Address %.2X recieve: ", address);
			for (int i = 0; i < feedback->length; i++)
			{
				printf("%.2X ", feedback->content[i]);
			}
			SYSTEMTIME systemtime;
			GetLocalTime(&systemtime);
			printf(" currentDateTime() = %02d:%02d:%02d.%03d\n", systemtime.wHour, systemtime.wMinute, systemtime.wSecond, systemtime.wMilliseconds);
			//ReleaseMutex(_ghMutex); // try

			/*
			if (feedback->content[0] == (uint8_t) '\xB5' && feedback->content[1] == address)
			{
			WaitForSingleObject(_ghMutex, INFINITE); // try
			printf("Resend Command\n");
			c_serial_write_data(_rs485Port->getPortHandle(), msgPtr->content, &msgPtr->length);
			ReleaseMutex(_ghMutex); // try
			continue;
			}
			*/

			if (feedback->content[0] == (uint8_t) '\xB1' && feedback->content[1] == address)
			{
				controlRecieved = true;
				printf("Address %.2X Controler Received\n", address);
				continue;
			}

			if (feedback->content[0] == (uint8_t) '\xB0' && feedback->content[1] == address)
			{
				printf("Address %.2X moveY Finished\n", address);
				_rs485Port->clearMsg(address);
				break;
			}
			if (feedback->content[0] == (uint8_t) '\xA0' && feedback->content[1] == address)
			{
				printf("Address %.2X moveX Finished\n", address);
				_rs485Port->clearMsg(address);
				break;
			}
			if (feedback->content[0] == (uint8_t) '\xA1' && feedback->content[1] == address)
			{
				printf("Address %.2X moveX Finished\n", address);
				_rs485Port->clearMsg(address);
				break;
			}
			delete[] feedback->content;
			delete feedback;
		}
		///*
		//WaitForSingleObject(_ghMutex, INFINITE); // try
		if (!controlRecieved)
		{
			clock_t dwCurrent = clock();
			if (dwCurrent - dwMsgSend > 1000)
			{
				printf("Address %.2X not recieve B1\n", address);
				printf("MicroscopeXY: moveY TX Timeout\n");
				if (c_serial_write_data(_rs485Port->getPortHandle(), msgPtr->content, &msgPtr->length) < 0)
				{
					printf("MicroscopeXY: moveY TX ERROR\n");
				}
				else
				{
					dwMsgSend = clock();
					GetLocalTime(&msgSend);
					printf("Address %.2X Message Send at %02d:%02d:%02d.%03d\n", address, msgSend.wHour, msgSend.wMinute, msgSend.wSecond, msgSend.wMilliseconds);
				}
			}
		}
		//ReleaseMutex(_ghMutex); // try
		//*/
		Sleep(100);
	}
	delete[]  msgPtr->content;

	return true;
}