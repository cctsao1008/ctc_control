/*********************************************
 * @file Washer.cpp                    		 *
 *                                           *
 * Washer Contorl     	 		             *
 *                                           *
 * @author Joshua <jan.joshua@iac.com.tw>    *
 *********************************************/

#include "core_common.h"

// Constructor
Washer::Washer(RS485Port* PortPtr)
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

Washer::Washer(double Pos, double RotateSpeed, double ShakeSpeed, RS485Port* PortPtr)
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
Washer::~Washer ()
{
	// Release memory if it's needed
} 

// Information
double Washer::getPositon() const
{
	return _armPosition;
}

double Washer::getRotateSpeed() const
{
	return _rotateSpeed;
}

double Washer::getShakeSpeed() const
{
	return _shakeSpeed;
}

// Action
// Direction: 1 => positive, 2 => negative
bool Washer::moveArm(double Degree, uint8_t Direction)
{
	uint8_t address = '\x00';
	int steps = (int)Degree * 100.0 / 1.8;
	Message* msgPtr = trans2RTCMD(NumberToString(_shakeSpeed).c_str(), address, Direction, NumberToString(steps).c_str(), "0", "0");
	
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
	if (c_serial_write_data(_rs485Port->getPortHandle(), msgPtr->content, &msgPtr->length) < 0)
	{
		printf("ShakeMachine: moveArm TX ERROR\n");
	}
	else
	{
		GetLocalTime(&msgSend);
		printf("Address %.2X Message Send at %02d:%02d:%02d.%03d\n", address, msgSend.wHour, msgSend.wMinute, msgSend.wSecond, msgSend.wMilliseconds);
	}
	//ReleaseMutex(_ghMutex); // try

	bool controlRecieved = false;
	unsigned int timeout = 0;
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
				break;
			}
			delete[] feedback->content;
			delete feedback;
		}
		///*
		//WaitForSingleObject(_ghMutex, INFINITE); // try
		if (!controlRecieved)
		{
			if (timeout > 5)
			{
				printf("Address %.2X Timeout too many times\n", address);
				printf("Please check COM Port\n");
				return false;
			}
			else
			{
				timeout += 1;
			}

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
	uint8_t address = '\x00';
	int steps = (int)Circle * 400.0 / 60.0;
	Message* msgPtr = trans2RTCMD(NumberToString(_shakeSpeed).c_str(), address, Direction, NumberToString(steps).c_str(), "0", "0");

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
		printf("Washer: rotateGripper TX ERROR\n");
	}
	else
	{
		GetLocalTime(&msgSend);
		printf("Address %.2X Message Send at %02d:%02d:%02d.%03d\n", address, msgSend.wHour, msgSend.wMinute, msgSend.wSecond, msgSend.wMilliseconds);
	}

	bool controlRecieved = false;
	unsigned int timeout = 0;
	while (true)
	{
		feedback = _rs485Port->getControlerMsg(address);

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
			if (timeout > 5)
			{
				printf("Address %.2X Timeout too many times\n", address);
				printf("Please check COM Port\n");
				return false;
			}
			else
			{
				timeout += 1;
			}

			clock_t dwCurrent = clock();
			if (dwCurrent - dwMsgSend > 500)
			{
				printf("Address %.2X not recieve B1\n", address);
				printf("Washer: rotateGripper TX Timeout\n");
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
			}
		}
		Sleep(100);
	}
	delete[]  msgPtr->content;

	return true;
}

bool Washer::shakeMachine(double Degree, unsigned int Times)
{
	while(Times-- > 0)
	{
		printf("Degree: %f, Shake Cycle: %d\n", Degree, (5 - Times));// try
		WaitForSingleObject(_ghMutex, INFINITE);
		if (!moveArm(Degree, PositiveExecute))
		{
			// Error Message
			return false;
		}
		ReleaseMutex(_ghMutex);
		Sleep(100);

		WaitForSingleObject(_ghMutex, INFINITE);
		if (!moveArm(Degree, NegativeExecute))
		{
			// Error Message
			return false;
		}
		ReleaseMutex(_ghMutex);
		Sleep(100);
	}

	return true;
}

// Modifier
bool Washer::initDriver()
{
	return true;
}

bool Washer::setRotateSpeed(double RotateSpeed)
{
	_rotateFreq = RotateSpeed;
	_rotateSpeed = RotateSpeed;
	return true;
}

bool Washer::setShakeSpeed(double ShakeSpeed)
{
	_shakeFreq  = (int) ShakeSpeed * 100.0 / 1.8;
	_shakeSpeed = ShakeSpeed;
	return true;
}