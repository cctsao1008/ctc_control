/*********************************************
 * @file core_shakemachine.cpp               *
 *                                           *
 * Shake Machine Contorl                     *
 *                                           *
 * @author Joshua <jan.joshua@iac.com.tw>    *
 *********************************************/

#include "stdafx.h"
#include "core_common.h"
#include "core_bianneng.h"
#include "core_shakemachine.h"

// Constructor
ShakeMachine::ShakeMachine(RS485Port* PortPtr)
{
	if (initDriver())
	{
		_armPosition = 0.0;
		_rotateSpeed = RPM197;
		_shakeSpeed = RPM197;
		_rs485Port = PortPtr;
		_ghMutex = _rs485Port->getMutex();
	}
}

ShakeMachine::ShakeMachine(double Pos, double RotateSpeed, double ShakeSpeed, RS485Port* PortPtr)
{
    if(initDriver())
    {
		_armPosition = Pos;
		_rotateSpeed = RotateSpeed;
    	_shakeSpeed  = ShakeSpeed;
		_rs485Port = PortPtr;
		_ghMutex = _rs485Port->getMutex();
    }
    else
    {
    	// Error Message
    }
    
}

// Destructor
ShakeMachine::~ShakeMachine ()
{
	// Release memory if it's needed
} 

// Information
double ShakeMachine ::getPositon() const
{
	return _armPosition;
}

double ShakeMachine::getRotateSpeed() const
{
	return _rotateSpeed;
}

double ShakeMachine::getShakeSpeed() const
{
	return _shakeSpeed;
}

// Action
// Direction: 1 => positive, 2 => negative
bool ShakeMachine::moveArm(double Degree, uint8_t Direction)
{
	uint8_t address = '\x00';
	int steps = (int)Degree * 100.0 / 1.8;
	Message* msgPtr = trans2RTCMD(NumberToString(_shakeSpeed).c_str(), (char*)&address, Direction, NumberToString(steps).c_str(), "0", "0");
	
	if (Direction == PositiveExecute && (_armPosition + Degree) > 90.0)
	{
		printf("ShakeMachine: moveArm Error, Over 90.0 degree\n");
		return false;
	}
	else if (Direction == NegativeExecute && (_armPosition - Degree) < 0.0)
	{
		printf("ShakeMachine: moveArm Error, Over 90.0 degree\n");
		return false;
	}

	Message* feedback = NULL;
	SYSTEMTIME msgSend;
	clock_t dwMsgSend = clock();
	if (c_serial_write_data(_rs485Port->getPortHandle(), msgPtr->content, &msgPtr->length) < 0)
	{
		printf("ShakeMachine: moveArm TX ERROR\n");
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
				if (Direction == PositiveExecute)
				{
					_armPosition += Degree;
					printf("moveArm Positive Finished\n");
				}
				else
				{
					_armPosition -= Degree;
					printf("moveArm Negative Finished\n");
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

bool ShakeMachine::rotateGripper(double Circle, uint8_t Direction)
{
	uint8_t address = '\x00';
	int steps = (int)Circle * 400.0 / 60.0;
	Message* msgPtr = trans2RTCMD(NumberToString(_shakeSpeed).c_str(), (char*)&address, Direction, NumberToString(steps).c_str(), "0", "0");

	/*
	if (Direction == PositiveExecute && (_position + Degree) > 90.0)
	{
		printf("ShakeMachine: moveArm Error, Over 90.0 degree");
		return false;
	}
	else if (Direction == NegativeExecute && (_position - Degree) < 0.0)
	{
		printf("ShakeMachine: moveArm Error, Over 90.0 degree");
		return false;
	}
	*/

	Message* feedback = NULL;
	SYSTEMTIME msgSend;
	clock_t dwMsgSend = clock();
	if (c_serial_write_data(_rs485Port->getPortHandle(), msgPtr->content, &msgPtr->length) < 0)
	{
		printf("ShakeMachine: rotateGripper TX ERROR\n");
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
				if (Direction == PositiveExecute)
				{
					_gripperPosition += Circle;
					printf("rotateGripper Positive Finished\n");
				}
				else
				{
					_gripperPosition -= Circle;
					printf("rotateGripper Negative Finished\n");
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

bool ShakeMachine::shakeMachine(double Degree, unsigned int Times)
{
	while(Times-- > 0)
	{
		if (!moveArm(Degree, PositiveExecute))
		{
			// Error Message
			return false;
		}

		Sleep(100);

		if (!moveArm(Degree, NegativeExecute))
		{
			// Error Message
			return false;
		}

		Sleep(100);
	}

	return true;
}

// Modifier
bool ShakeMachine::initDriver()
{
	return true;
}

bool ShakeMachine::setRotateSpeed(double RotateSpeed)
{
	_rotateFreq = RotateSpeed;
	_rotateSpeed = RotateSpeed;
	return true;
}

bool ShakeMachine::setShakeSpeed(double ShakeSpeed)
{
	_shakeFreq  = (int) ShakeSpeed * 100.0 / 1.8;
	_shakeSpeed = ShakeSpeed;
	return true;
}