/*********************************************
 * @file core_syringepump.cpp                *
 *                                           *
 * Syringe Pump Contorl                      *
 *                                           *
 * @author Joshua <jan.joshua@iac.com.tw>    *
 *********************************************/

#include "stdafx.h"
#include "core_common.h"
#include "core_bianneng.h"
#include "core_syringepump.h"

// Constructor
SyringePump::SyringePump(RS485Port* PortPtr)
{
    if(initDriver())
    {
		_volume.insert(std::pair <unsigned int, double>(5, 0.0));
		_volume.insert(std::pair <unsigned int, double>(20, 0.0));
		_speed = RPM197;
		_rs485Port = PortPtr;
    }
    else
    {
    	// Error Message
    }
    
}

SyringePump::SyringePump(std::map<unsigned int, double> Volume, unsigned int Speed, RS485Port* PortPtr)
{
    if(initDriver())
    {
    	_volume = Volume;
    	_speed = Speed;
		_rs485Port = PortPtr;
    }
    else
    {
    	// Error Message
    }
    
}

// Destructor
SyringePump::~SyringePump()
{
	// Release memory if it's needed
} 

// Information
const double SyringePump::getVolume(unsigned int Size)
{
	return _volume.find(Size)->second;
}

const unsigned int SyringePump::getSpeed()
{
	return _speed;
}

// Action
bool SyringePump::absorbVolume(double Volume)
{
	uint8_t address = '\x00';
	Message* msgPtr = NULL;

	if (Volume < 0)
	{
		printf("[ERROR] SyringePump : In absorbVolume(), Volume should be bigger than 0 and smaller than 20ml");
		return false;
	}
	else if (Volume <= 0.13)
	{
		unsigned int pulse = (unsigned int) (Volume / 0.0004154);
		address = _address.find(5)->second;
		_volume.find(5)->second += Volume;
		msgPtr = trans2RTCMD(NumberToString(_speed).c_str(), (char*)&_address.find(5)->second, PositiveExecute, NumberToString(pulse).c_str(), "120", "0");
	}
	else if (Volume <= 20.0)
	{
		unsigned int pulse = (unsigned int)(Volume / 0.0016616);
		address = _address.find(20)->second;
		_volume.find(20)->second += Volume;
		msgPtr = trans2RTCMD(NumberToString(_speed).c_str(), (char*)&_address.find(20)->second, PositiveExecute, NumberToString(pulse).c_str(), "120", "0");
	}
	else
	{
		printf("[ERROR] SyringePump : In absorbVolume(), Volume should be bigger than 0 and smaller than 20ml");
		return false;
	}

	Message* feedback = NULL;
	SYSTEMTIME msgSend;
	clock_t dwMsgSend = clock();
	if (c_serial_write_data(_rs485Port->getPortHandle(), msgPtr->content, &msgPtr->length) < 0)
	{
		printf("SyringePump: absorbVolume TX ERROR\n");
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
				printf("absorbVolume Finished\n");
				break;
			}

			if (feedback->content[0] == (uint8_t) '\xB1' && feedback->content[1] == (uint8_t) '\x00')
			{
				controlRecieved = true;
				printf("Controler Received\n");
				continue;
			}
		}
		/*
		else if (!controlRecieved)
		{
			clock_t dwCurrent = clock();
			if (dwCurrent - dwMsgSend > 200)
			{
				printf("SyringePump: absorbVolume TX Timeout\n");
				if (c_serial_write_data(_port, msgPtr->content, &msgPtr->length) < 0)
				{
					printf("SyringePump: drainVolume TX ERROR\n");
				}
				else
				{
					dwMsgSend = clock();
					GetLocalTime(&msgSend);
					printf("Message Resend at %02d:%02d:%02d.%03d\n", msgSend.wHour, msgSend.wMinute, msgSend.wSecond, msgSend.wMilliseconds);
				}
			}
		}
		*/
		Sleep(100);
	}
	

	return true;
}

bool SyringePump::drainVolume(double Volume)
{
	uint8_t address = '\x00';
	Message* msgPtr = NULL;

	if (Volume < 0)
	{
		printf("[ERROR] SyringePump : In absorbVolume(), Volume should be bigger than 0 and smaller than 20ml");
		return false;
	}
	else if (Volume <= 0.13)
	{
		unsigned int pulse = (unsigned int)(Volume / 0.0004154);
		address = _address.find(5)->second;
		_volume.find(5)->second -= Volume;
		msgPtr = trans2RTCMD(NumberToString(_speed).c_str(), (char*)&_address.find(5)->second, NegativeExecute, NumberToString(pulse).c_str(), "120", "0");
	}
	else if (Volume <= 20.0)
	{
		unsigned int pulse = (unsigned int)(Volume / 0.0016616);
		address = _address.find(20)->second;
		_volume.find(20)->second -= Volume;
		msgPtr = trans2RTCMD(NumberToString(_speed).c_str(), (char*)&_address.find(20)->second, NegativeExecute, NumberToString(pulse).c_str(), "120", "0");
		
	}
	else
	{
		printf("[ERROR] SyringePump : In absorbVolume(), Volume should be bigger than 0 and smaller than 20ml");
		return false;
	}

	Message* feedback = NULL;
	SYSTEMTIME msgSend;
	clock_t dwMsgSend = clock();
	if (c_serial_write_data(_rs485Port->getPortHandle(), msgPtr->content, &msgPtr->length) < 0)
	{
		printf("SyringePump: drainVolume TX ERROR\n");
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
				printf("drainVolume Finished\n");
				break;
			}

			if (feedback->content[0] == (uint8_t) '\xB1' && feedback->content[1] == (uint8_t) '\x00')
			{
				controlRecieved = true;
				printf("Controler Received\n");
				continue;
			}
		}
		/*
		else if (!controlRecieved)
		{
			clock_t dwCurrent = clock();
			if (dwCurrent - dwMsgSend > 200)
			{
				printf("SyringePump: absorbVolume TX Timeout\n");
				if (c_serial_write_data(_port, msgPtr->content, &msgPtr->length) < 0)
				{
					printf("SyringePump: drainVolume TX ERROR\n");
				}
				else
				{
					dwMsgSend = clock();
					GetLocalTime(&msgSend);
					printf("Message Resend at %02d:%02d:%02d.%03d\n", msgSend.wHour, msgSend.wMinute, msgSend.wSecond, msgSend.wMilliseconds);
				}
			}
		}
		*/
		Sleep(100);
	}

	return true;
}

bool SyringePump::pipetteVolume(double Volume, unsigned int Times)
{
	while(Times-- > 0)
	{
		if(!absorbVolume(Volume))
		{
			// Error Message
			return false;
		}

		Sleep(100);

		if (!drainVolume(Volume))
		{
			// Error Message
			return false;
		}

		Sleep(100);
	}

	return true;
}

// Modifier
bool SyringePump::initDriver()
{
	_address.insert(std::pair <unsigned int, uint8_t>(5,  '\x00'));
	_address.insert(std::pair <unsigned int, uint8_t>(20, '\x00'));

	return true;
}

bool SyringePump::setSpeed(unsigned int Speed)
{
	_speed = Speed;
	return true;
}

