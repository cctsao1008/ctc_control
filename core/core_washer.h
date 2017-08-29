/*********************************************
 * @file Washer.h                    		 *
 *                                           *
 * Washer Contorl     	 		             *
 *                                           *
 * @author Joshua <jan.joshua@iac.com.tw>    *
 *********************************************/

#ifndef _CORE_WASHER_H_
#define _CORE_WASHER_H_

#include "c_serial.h"
#include "core_common.h"
#include "core_bianneng.h"

class Washer : public BianNeng
{

public:

    // Constructor
	Washer(RS485Port* PortPtr);
	Washer(double Pos, double RotateSpeed, double ShakeSpeed, RS485Port* PortPtr);

    // Destructor
    ~Washer();

    // Information
    double getPositon() const;
	double getRotateSpeed() const;
	double getShakeSpeed() const;

    // Action
	bool moveArm(double Degree, uint8_t Direction);
	bool rotateGripper(double Circle, uint8_t Direction);
    bool shakeMachine(double Degree, unsigned int Times);

    // Modifier
    bool initDriver();
	bool setRotateSpeed(double RotateSpeed);
	bool setShakeSpeed(double ShakeSpeed);

private:
    // Status
    double _armPosition;		// unit is degree in range of 0 degree ~ 90 degree
	double _gripperPosition;	// unit is circle
    double _shakeSpeed;			// unit is Hz //unit is degree
	double _rotateSpeed;		// unit is Hz //unit is rpm
	int _shakeFreq;				// uint is Hz, the frequecy of pwm pulses given by Shake Controler
	int _rotateFreq;			// uint is Hz, the frequecy of pwm pulses given by Rotate Controler
	// Ptr
	RS485Port* _rs485Port;
	/* mutex lock */
	HANDLE _ghMutex;
};

#endif // CORE_WASHER_H_