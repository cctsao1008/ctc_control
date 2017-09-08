/*********************************************
 * @file SyringePump.cpp                     *
 *                                           *
 * Syringe Pump Contorl                      *
 *                                           *
 * @author Joshua <jan.joshua@iac.com.tw>    *
 *********************************************/

#ifndef _SYRINGEPUMP_H_
#define _SYRINGEPUMP_H_

#include "core_common.h"

class SyringePump : public BianNeng
{

public:

    // Constructor
	SyringePump(RS485Port* PortPtr);
	SyringePump(std::map<unsigned int, double> Volume, unsigned int Speed, RS485Port* PortPtr);

    // Destructor
    ~SyringePump();

    // Information
	const double getVolume(unsigned int Size);
	const unsigned int getSpeed();

    // Action
	bool absorbVolume(double Volume);
	bool drainVolume(double Volume);
	bool pipetteVolume(double Volume, unsigned int Times);

    // Modifier
    bool initDriver();
	bool setSpeed(unsigned int Speed);
	

private:
    // Status
	std::map<unsigned int, uint8_t>		_address;	// record the address of different pump
	std::map<unsigned int, double>		_volume;	// the volume of air in piston, unit is ml
	unsigned int						_speed;		// unit is rpm but real value is prescaler of controler
	// Ptr
	RS485Port* _rs485Port;
	/* mutex lock */
	HANDLE _ghMutex;
	
};

static SyringePump* syringepump;

#endif // _SYRINGEPUMP_H_