/*********************************************
 * @file Washer.cpp                    		 *
 *                                           *
 * Washer Contorl     	 		             *
 *                                           *
 * @author Joshua <jan.joshua@iac.com.tw>    *
 *********************************************/

#include "core_common.h"
//#include <typeinfo>

// Constructor
Washer::Washer(RS485Port* PortPtr)
{
	if (initDriver())
	{
		_armPosition = 0.0;
		_rotateSpeed = RPM50;
		_shakeSpeed = RPM930;
		_rs485Port = PortPtr;
		_ghMutex = _rs485Port->getMutex();
	}
}

Washer::Washer(double Pos, double RotateSpeed, double ShakeSpeed, RS485Port* PortPtr)
{
	if (initDriver())
	{
		_armPosition = Pos;
		_rotateSpeed = RotateSpeed;
		_shakeSpeed = ShakeSpeed;
		_rs485Port = PortPtr;
		_ghMutex = _rs485Port->getMutex();
	}
	else
	{
		// Error Message
	}

}

// Destructor
Washer::~Washer()
{
	// Release memory if it's needed
}

// Information
double Washer::getPositon() const
{
	return _armPosition;
}

unsigned int Washer::getRotateSpeed() const
{
	return _rotateSpeed;
}

unsigned int Washer::getShakeSpeed() const
{
	return _shakeSpeed;
}

// Action
// Direction: 1 => positive, 2 => negative
bool Washer::moveArm(double Degree, uint8_t Direction)
{
	uint8_t address = '\x00';
	int steps = (int)Degree * 90.0 / 0.9;
	//printf("%s\n", typeid(_shakeSpeed).name());
	Message* msgPtr = trans2RTCMD(NumberToString(_shakeSpeed).c_str(), address, Direction, NumberToString(steps).c_str(), "200", "0");

	if (Direction == PositiveExecute && (_armPosition + Degree) > 90.0)
	{
		printf("Washer: moveArm Error, Over 90.0 degree\n");
		return false;
	}
	else if (Direction == NegativeExecute && (_armPosition - Degree) < 0.0)
	{
		printf("Washer: moveArm Error, Over 90.0 degree\n");
		return false;
	}

	Message* feedback = NULL;
	SYSTEMTIME msgSend;
	clock_t dwMsgSend = clock();

	//WaitForSingleObject(_ghMutex, INFINITE); // try
	_rs485Port->clearMsg(address);
	if (c_serial_write_data(_rs485Port->getPortHandle(), msgPtr->content, &msgPtr->length) < 0)
	{
		printf("Washer: moveArm TX ERROR\n");
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
				if (Direction == PositiveExecute)
				{
					_armPosition += Degree;
					printf("Address %.2X moveArm Positive Finished\n", address);
				}
				else
				{
					_armPosition -= Degree;
					printf("Address %.2X moveArm Negative Finished\n", address);
				}
				_rs485Port->clearMsg(address);
				break;
			}

			if (feedback->content[0] == (uint8_t) '\xA1' && feedback->content[1] == address)
			{
				_armPosition += Degree;
				printf("Address %.2X moveArm Positive to the Limited\n", address);
				break;
			}

			if (feedback->content[0] == (uint8_t) '\xA0' && feedback->content[1] == address)
			{
				_armPosition -= Degree;
				printf("Address %.2X moveArm Negative to the Limited\n", address);
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
			if (dwCurrent - dwMsgSend > 500)
			{
				printf("Address %.2X not recieve B1\n", address);
				printf("Washer: moveArm TX Timeout\n");
				if (c_serial_write_data(_rs485Port->getPortHandle(), msgPtr->content, &msgPtr->length) < 0)
				{
					printf("Washer: moveArm TX ERROR\n");
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

bool Washer::rotateGripper(double Circle, uint8_t Direction)
{
	uint8_t address = '\x01';
	int steps = (int)Circle * 400.0;
	Message* msgPtr = trans2RTCMD(NumberToString(_rotateSpeed).c_str(), address, Direction, NumberToString(steps).c_str(), "200", "0");

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

	WaitForSingleObject(_ghMutex, INFINITE);
	if (c_serial_write_data(_rs485Port->getPortHandle(), msgPtr->content, &msgPtr->length) < 0)
	{
		printf("Washer: rotateGripper TX ERROR\n");
	}
	else
	{
		GetLocalTime(&msgSend);
		printf("Address %.2X Message Send at %02d:%02d:%02d.%03d\n", address, msgSend.wHour, msgSend.wMinute, msgSend.wSecond, msgSend.wMilliseconds);
	}
	ReleaseMutex(_ghMutex);

	bool controlRecieved = false;
	while (true)
	{
		WaitForSingleObject(_ghMutex, INFINITE);
		feedback = _rs485Port->getControlerMsg(address);
		ReleaseMutex(_ghMutex);

		if (feedback != NULL)
		{
			printf("Address %.2X recieve: ", address);
			for (int i = 0; i < feedback->length; i++)
			{
				printf("%.2X ", feedback->content[i]);
			}
			SYSTEMTIME systemtime;
			GetLocalTime(&systemtime);
			printf(" currentDateTime() = %02d:%02d:%02d.%03d\n", systemtime.wHour, systemtime.wMinute, systemtime.wSecond, systemtime.wMilliseconds);

			if (feedback->content[0] == (uint8_t) '\xB1' && feedback->content[1] == address)
			{
				controlRecieved = true;
				printf("Address %.2X Controler Received\n", address);
				continue;
			}

			if (feedback->content[0] == (uint8_t) '\xB0' && feedback->content[1] == address)
			{
				if (Direction == PositiveExecute)
				{
					_gripperPosition += Circle;
					printf("Address %.2X rotateGripper Positive Finished\n", address);
				}
				else
				{
					_gripperPosition -= Circle;
					printf("Address %.2X rotateGripper Negative Finished\n", address);
				}
				break;
			}
			delete[] feedback->content;
			delete feedback;
		}

		if (!controlRecieved)
		{
			clock_t dwCurrent = clock();
			if (dwCurrent - dwMsgSend > 500)
			{
				printf("Address %.2X not recieve B1\n", address);
				printf("Washer: rotateGripper TX Timeout\n");
				WaitForSingleObject(_ghMutex, INFINITE);
				if (c_serial_write_data(_rs485Port->getPortHandle(), msgPtr->content, &msgPtr->length) < 0)
				{
					printf("Washer: rotateGripper TX ERROR\n");
				}
				else
				{
					dwMsgSend = clock();
					GetLocalTime(&msgSend);
					printf("Address %.2X Message Send at %02d:%02d:%02d.%03d\n", address, msgSend.wHour, msgSend.wMinute, msgSend.wSecond, msgSend.wMilliseconds);
				}
				ReleaseMutex(_ghMutex);
			}
		}
		Sleep(100);
	}
	delete[]  msgPtr->content;

	return true;
}

bool Washer::shakeMachine(double Degree, unsigned int Times)
{
	while (Times-- > 0)
	{
		printf("Degree: %f, MoveArm %d\n", Degree, (5 - Times));// try
		//WaitForSingleObject(_ghMutex, INFINITE);
		if (!moveArm(Degree, PositiveExecute))
		{
			// Error Message
			return false;
		}
		//ReleaseMutex(_ghMutex);
		Sleep(200);

		//WaitForSingleObject(_ghMutex, INFINITE);
		if (!moveArm(Degree, NegativeExecute))
		{
			// Error Message
			return false;
		}
		//ReleaseMutex(_ghMutex);
		Sleep(200);
	}

	return true;
}

// Modifier
bool Washer::initDriver()
{
	return true;
}

bool Washer::setRotateSpeed(unsigned int RotateSpeed)
{
	_rotateSpeed = RotateSpeed;
	return true;
}

bool Washer::setShakeSpeed(unsigned int ShakeSpeed)
{
	_shakeSpeed = ShakeSpeed;
	return true;
}