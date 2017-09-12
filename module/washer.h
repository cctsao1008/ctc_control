/*********************************************
 * @file Washer.h                    		 *
 *                                           *
 * Washer Contorl     	 		             *
 *                                           *
 * @author Joshua <jan.joshua@iac.com.tw>    *
 *********************************************/


#ifndef _CORE_WASHER_H_
#define _CORE_WASHER_H_

#include "core_common.h"

class Washer : public BianNeng
{

public:

	// Constructor
	Washer(RS485Port* PortPtr);
	Washer(double Pos, unsigned int RotateSpeed, unsigned int ShakeSpeed, RS485Port* PortPtr);

	// Destructor
	~Washer();

	// Information
	double getPositon() const;
	unsigned int getRotateSpeed() const;
	unsigned int getShakeSpeed() const;

	// Action
	bool moveArm(double Degree, uint8_t Direction);
	bool rotateGripper(double Circle, uint8_t Direction);
	bool shakeMachine(double Degree, unsigned int Times);
	
	// Modifier
	bool initDriver();
	bool setRotateSpeed(unsigned int RotateSpeed);
	bool setShakeSpeed(unsigned int ShakeSpeed);

private:
	// Status
	double _armPosition;			// unit is degree in range of 0 degree ~ 90 degree
	double _gripperPosition;		// unit is circle
	unsigned int _shakeSpeed;		// unit is Hz //unit is degree
	unsigned int _rotateSpeed;		// unit is Hz //unit is rpm
	// Ptr
	RS485Port* _rs485Port;
	/* mutex lock */
	HANDLE _ghMutex;
};

static Washer* washer;
//static Washer washer = Washer(&rs485p2);

#endif // CORE_WASHER_H_
