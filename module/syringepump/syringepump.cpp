/*********************************************
 * @file SyringePump.cpp                     *
 *                                           *
 * Syringe Pump Contorl                      *
 *                                           *
 * @author Joshua <jan.joshua@iac.com.tw>    *
 *********************************************/

#include "core_common.h"

// Constructor
SyringePump::SyringePump(RS485Port* PortPtr)
{
    if(initDriver())
    {
		_volume.insert(std::pair <unsigned int, double>(5, 0.0));
		_volume.insert(std::pair <unsigned int, double>(20, 0.0));
		_speed = RPM197;
		_rs485Port = PortPtr;
		_ghMutex = _rs485Port->getMutex();
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
		_ghMutex = _rs485Port->getMutex();
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
		msgPtr = trans2RTCMD(NumberToString(_speed).c_str(), _address.find(5)->second, PositiveExecute, NumberToString(pulse).c_str(), "120", "0");
	}
	else if (Volume <= 20.0)
	{
		unsigned int pulse = (unsigned int)(Volume / 0.0016616);
		address = _address.find(20)->second;
		_volume.find(20)->second += Volume;
		msgPtr = trans2RTCMD(NumberToString(_speed).c_str(), _address.find(20)->second, PositiveExecute, NumberToString(pulse).c_str(), "120", "0");
	}
	else
	{
		printf("[ERROR] SyringePump : In absorbVolume(), Volume should be bigger than 0 and smaller than 20ml");
		return false;
	}

	Message* feedback = NULL;
	SYSTEMTIME msgSend;
	clock_t dwMsgSend = clock();

	//WaitForSingleObject(_ghMutex, INFINITE); // try
	if (c_serial_write_data(_rs485Port->getPortHandle(), msgPtr->content, &msgPtr->length) < 0)
	{
		printf("SyringePump: absorbVolume TX ERROR\n");
	}
	else
	{ 
		GetLocalTime(&msgSend);
		printf("Address %.2X Message Send at %02d:%02d:%02d.%03d\n",address, msgSend.wHour, msgSend.wMinute, msgSend.wSecond, msgSend.wMilliseconds);
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
				printf("Address %.2X absorbVolume Finished\n", address);
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
			
				printf("Address %.2X not recieve B1\n", address);
				printf("SyringePump: absorbVolume TX Timeout\n");
				if (c_serial_write_data(_rs485Port->getPortHandle(), msgPtr->content, &msgPtr->length) < 0)
				{
					printf("SyringePump: absorbVolume TX ERROR\n");
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

bool SyringePump::drainVolume(double Volume)
{
	uint8_t address = '\x00';
	Message* msgPtr = NULL;

	if (Volume < 0)
	{
		printf("[ERROR] SyringePump : In drainVolume(), Volume should be bigger than 0 and smaller than 20ml");
		return false;
	}
	else if (Volume <= 0.13)
	{
		unsigned int pulse = (unsigned int)(Volume / 0.0004154);
		address = _address.find(5)->second;
		_volume.find(5)->second -= Volume;
		msgPtr = trans2RTCMD(NumberToString(_speed).c_str(), _address.find(5)->second, NegativeExecute, NumberToString(pulse).c_str(), "120", "0");
	}
	else if (Volume <= 20.0)
	{
		unsigned int pulse = (unsigned int)(Volume / 0.0016616);
		address = _address.find(20)->second;
		_volume.find(20)->second -= Volume;
		msgPtr = trans2RTCMD(NumberToString(_speed).c_str(), _address.find(20)->second, NegativeExecute, NumberToString(pulse).c_str(), "120", "0");
		
	}
	else
	{
		printf("[ERROR] SyringePump : In drainVolume(), Volume should be bigger than 0 and smaller than 20ml");
		return false;
	}

	Message* feedback = NULL;
	SYSTEMTIME msgSend;
	clock_t dwMsgSend = clock();

	//WaitForSingleObject(_ghMutex, INFINITE); // try
	if (c_serial_write_data(_rs485Port->getPortHandle(), msgPtr->content, &msgPtr->length) < 0)
	{
		printf("SyringePump: drainVolume TX ERROR\n");
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
				printf("Address %.2X drainVolume Finished\n", address);
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

				printf("Address %.2X not recieve B1\n", address);
				printf("SyringePump: drainbVolume TX Timeout\n");
				if (c_serial_write_data(_rs485Port->getPortHandle(), msgPtr->content, &msgPtr->length) < 0)
				{
					printf("SyringePump: drainVolume TX ERROR\n");
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

bool SyringePump::pipetteVolume(double Volume, unsigned int Times)
{
	while(Times-- > 0)
	{
		printf("Volume: %f, Pipette %d\n", Volume, (20 - Times));// try
		WaitForSingleObject(_ghMutex, INFINITE);
		if(!absorbVolume(Volume))
		{
			// Error Message
			return false;
		}
		ReleaseMutex(_ghMutex);
		Sleep(100);

		//printf("Volume: %f\n", Volume); // try
		WaitForSingleObject(_ghMutex, INFINITE);
		if (!drainVolume(Volume))
		{
			// Error Message
			return false;
		}
		ReleaseMutex(_ghMutex);
		Sleep(100);
	}

	printf("Volume: %f Pipetting Finished\n", Volume);

	return true;
}

// Modifier
bool SyringePump::initDriver()
{
	_address.insert(std::pair <unsigned int, uint8_t>(5,  '\x01'));
	_address.insert(std::pair <unsigned int, uint8_t>(20, '\x00'));

	return true;
}

bool SyringePump::setSpeed(unsigned int Speed)
{
	_speed = Speed;
	return true;
}

