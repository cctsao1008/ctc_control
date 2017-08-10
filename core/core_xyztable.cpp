/*********************************************
 * @file XYZTable.cpp                        *
 *                                           *
 * XYZ Table Contorl                         *
 *                                           *
 * @author Joshua <jan.joshua@iac.com.tw>    *
 *********************************************/
#include "stdafx.h"
#include "core_common.h"
#include "core_bianneng.h"
#include "core_xyztable.h"

// Constructor
XYZTable::XYZTable(RS485Port* PortPtr)
{
	if (initDriver())
	{
		_position.X = 0;
		_position.Y = 0;
		_position.Z = 0;
		_rs485Port = PortPtr;
		_ghMutex = _rs485Port->getMutex();;
	}
	else
	{
		// Error Message
	}

}

XYZTable::XYZTable(Position Pos, RS485Port* PortPtr)
{
	if(initDriver())
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

XYZTable::XYZTable(double X, double Y, double Z, RS485Port* PortPtr)
{
	if(initDriver())
    {
    	_position.X = X;
    	_position.Y = Y;
    	_position.Z = Z;
		_rs485Port = PortPtr;
		_ghMutex = _rs485Port->getMutex();
    }
    else
    {
    	// Error Message
    }
}

// Destructor
XYZTable::~XYZTable() 
{
	// Release memory if it's needed
} 

// Information
Position XYZTable::getPositon() const
{
	return _position;
}

double XYZTable::getX() const
{
	return _position.X;
}

double XYZTable::getY() const
{
	return _position.Y;
}

double XYZTable::getZ() const
{
	return _position.Z;
}

double XYZTable::getSpeed() const
{
	return _speed;
}

// Action
bool XYZTable::move2Pos(Position Pos)
{
	double xLength = _position.X - Pos.X;
	uint8_t xDirection;
	double yLength = _position.Y - Pos.Y;
	uint8_t yDirection;
	double zLength = _position.Z - Pos.Z;
	uint8_t zDirection;

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

	if (zLength > 0)
	{
		zDirection = NegativeExecute;
	}
	else
	{
		zLength = 0 - zLength;
		zDirection = PositiveExecute;
	}

	if (!moveZ(zLength, zDirection)) // If motor did not move successfully
	{
		// Error Message
		return false;
	}

	Sleep(500);

	return true;
}

bool XYZTable::move2Pos(double X, double Y, double Z)
{
	double xLength = _position.X - X;
	uint8_t xDirection;
	double yLength = _position.Y - Y;
	uint8_t yDirection;
	double zLength = _position.Z - Z;
	uint8_t zDirection;

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

	if (zLength > 0)
	{
		zDirection = NegativeExecute;
	}
	else
	{
		zLength = 0 - zLength;
		zDirection = PositiveExecute;
	}

	if (!moveZ(zLength, zDirection)) // If motor did not move successfully
	{
		// Error Message
		return false;
	}

	Sleep(500);

	return true;
}

// Modifier
bool XYZTable::initDriver()
{
	if(false) // If motor driver cannot be init
	{
		// Error Message
		return false;
	}

	return true;
}

bool XYZTable::setSpeed(double Speed)
{
	if(false) // If motor driver cannot set speed successfully
	{
		// Error Message
		return false;
	}

	return true;
}

// Combined with the driver
bool XYZTable::moveX(double X, uint8_t Direction)
{
	uint8_t address = '\x00';
	int steps = (int)X * 40000.0;
	Message* msgPtr = trans2RTCMD(NumberToString(_speed).c_str(), (char*)&address, Direction, NumberToString(steps).c_str(), "0", "0");

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
	if (c_serial_write_data(_rs485Port->getPortHandle(), msgPtr->content, &msgPtr->length) < 0)
	{
		printf("XYZTable: moveX TX ERROR\n");
	}
	else
	{
		GetLocalTime(&msgSend);
		printf("Message Send at %02d:%02d:%02d.%03d\n", msgSend.wHour, msgSend.wMinute, msgSend.wSecond, msgSend.wMilliseconds);
	}

	bool controlRecieved = false;
	while (true)
	{
		WaitForSingleObject(_ghMutex, INFINITE);
		feedback = _rs485Port->getControlerMsg();
		ReleaseMutex(_ghMutex);
		if (feedback != NULL)
		{
			for (int i = 0; i < feedback->length; i++)
			{
				printf("%.2X ", feedback->content[i]);
			}
			SYSTEMTIME systemtime;
			//GetSystemTime(&systemtime);
			GetLocalTime(&systemtime);
			printf(" currentDateTime() = %02d:%02d:%02d.%03d\n", systemtime.wHour, systemtime.wMinute, systemtime.wSecond, systemtime.wMilliseconds);
			//printf("\n");

			if (feedback->content[0] == (uint8_t) '\xB5' && feedback->content[1] == (uint8_t) '\x00')
			{
				printf("Resend Command\n");
				c_serial_write_data(_rs485Port->getPortHandle(), msgPtr->content, &msgPtr->length);
				continue;
			}

			if (feedback->content[0] == (uint8_t) '\xB0' && feedback->content[1] == (uint8_t) '\x00')
			{
				printf("moveX Finished\n");
				if (Direction == PositiveExecute)
				{
					_position.X += X;
				}
				else if (Direction == NegativeExecute)
				{
					_position.X -= X;
				}
				break;
			}

			if (feedback->content[0] == (uint8_t) '\xB1' && feedback->content[1] == (uint8_t) '\x00')
			{
				controlRecieved = true;
				printf("Controler Received\n");
				continue;
			}
		}
		Sleep(100);
	}
 
	return true;
}

bool XYZTable::moveY(double Y, uint8_t Direction)
{
	uint8_t address = '\x00';
	int steps = (int)Y * 40000.0;
	Message* msgPtr = trans2RTCMD(NumberToString(_speed).c_str(), (char*)&address, Direction, NumberToString(steps).c_str(), "0", "0");

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
	if (c_serial_write_data(_rs485Port->getPortHandle(), msgPtr->content, &msgPtr->length) < 0)
	{
		printf("XYZTable: moveY TX ERROR\n");
	}
	else
	{
		GetLocalTime(&msgSend);
		printf("Message Send at %02d:%02d:%02d.%03d\n", msgSend.wHour, msgSend.wMinute, msgSend.wSecond, msgSend.wMilliseconds);
	}

	bool controlRecieved = false;
	while (true)
	{
		WaitForSingleObject(_ghMutex, INFINITE);
		feedback = _rs485Port->getControlerMsg();
		ReleaseMutex(_ghMutex);
		if (feedback != NULL)
		{
			for (int i = 0; i < feedback->length; i++)
			{
				printf("%.2X ", feedback->content[i]);
			}
			SYSTEMTIME systemtime;
			//GetSystemTime(&systemtime);
			GetLocalTime(&systemtime);
			printf(" currentDateTime() = %02d:%02d:%02d.%03d\n", systemtime.wHour, systemtime.wMinute, systemtime.wSecond, systemtime.wMilliseconds);
			//printf("\n");

			if (feedback->content[0] == (uint8_t) '\xB5' && feedback->content[1] == (uint8_t) '\x00')
			{
				printf("Resend Command\n");
				c_serial_write_data(_rs485Port->getPortHandle(), msgPtr->content, &msgPtr->length);
				continue;
			}

			if (feedback->content[0] == (uint8_t) '\xB0' && feedback->content[1] == (uint8_t) '\x00')
			{
				printf("moveY Finished\n");
				if (Direction == PositiveExecute)
				{
					_position.Y += Y;
				}
				else if (Direction == NegativeExecute)
				{
					_position.Y -= Y;
				}
				break;
			}

			if (feedback->content[0] == (uint8_t) '\xB1' && feedback->content[1] == (uint8_t) '\x00')
			{
				controlRecieved = true;
				printf("Controler Received\n");
				continue;
			}
		}
		Sleep(100);
	}

	return true;
}

bool XYZTable::moveZ(double Z, uint8_t Direction)
{
	uint8_t address = '\x00';
	int steps = (int)Z * 40000.0;
	Message* msgPtr = trans2RTCMD(NumberToString(_speed).c_str(), (char*)&address, Direction, NumberToString(steps).c_str(), "0", "0");

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
	if (c_serial_write_data(_rs485Port->getPortHandle(), msgPtr->content, &msgPtr->length) < 0)
	{
		printf("XYZTable: moveZ TX ERROR\n");
	}
	else
	{
		GetLocalTime(&msgSend);
		printf("Message Send at %02d:%02d:%02d.%03d\n", msgSend.wHour, msgSend.wMinute, msgSend.wSecond, msgSend.wMilliseconds);
	}

	bool controlRecieved = false;
	while (true)
	{
		WaitForSingleObject(_ghMutex, INFINITE);
		feedback = _rs485Port->getControlerMsg();
		ReleaseMutex(_ghMutex);
		if (feedback != NULL)
		{
			for (int i = 0; i < feedback->length; i++)
			{
				printf("%.2X ", feedback->content[i]);
			}
			SYSTEMTIME systemtime;
			//GetSystemTime(&systemtime);
			GetLocalTime(&systemtime);
			printf(" currentDateTime() = %02d:%02d:%02d.%03d\n", systemtime.wHour, systemtime.wMinute, systemtime.wSecond, systemtime.wMilliseconds);
			//printf("\n");

			if (feedback->content[0] == (uint8_t) '\xB5' && feedback->content[1] == (uint8_t) '\x00')
			{
				printf("Resend Command\n");
				c_serial_write_data(_rs485Port->getPortHandle(), msgPtr->content, &msgPtr->length);
				continue;
			}

			if (feedback->content[0] == (uint8_t) '\xB0' && feedback->content[1] == (uint8_t) '\x00')
			{
				printf("moveZ Finished\n");
				if (Direction == PositiveExecute)
				{
					_position.Z += Z;
				}
				else if (Direction == NegativeExecute)
				{
					_position.Z -= Z;
				}
				break;
			}

			if (feedback->content[0] == (uint8_t) '\xB1' && feedback->content[1] == (uint8_t) '\x00')
			{
				controlRecieved = true;
				printf("Controler Received\n");
				continue;
			}
		}
		Sleep(100);
	}

	return true;
}