/*********************************************
 * @file core_microscope_xy.h                *
 *                                           *
 * Microscope XY Table Contorl               *
 *                                           *
 * @author Joshua <jan.joshua@iac.com.tw>    *
 *********************************************/

#ifndef _CORE_MICROSCOPE_XY_H_
#define _CORE_MICROSCOPE_XY_H_

#include "core_common.h"

typedef struct _position 
{
    double X;
    double Y;
    double Z;
} Position;

class MicroscopeXY : public BianNeng
{

public:

    // Constructor
	MicroscopeXY();
	MicroscopeXY(RS485Port* PortPtr);
	MicroscopeXY(Position Pos, RS485Port* PortPtr);
	MicroscopeXY(double X, double Y, RS485Port* PortPtr);

    // Destructor
    ~MicroscopeXY();

    // Information
    Position getPositon() const;
    double getX() const;
    double getY() const;
    double getSpeed() const;

	// Move to (0, 0)
	bool initmps();

    // Move to specific position
    bool move2Pos(Position Pos);
    bool move2Pos(double X, double Y);
	
	// Move vector
	bool moveXY(double X, double Y);

    // Modifier
    bool initDriver();
    bool setSpeed(double Speed);
	bool setPort(RS485Port* PortPtr);

private:
    // Combined with the driver
	bool moveX(double X, uint8_t Direction);
	bool moveY(double Y, uint8_t Direction);
    // Status
    Position _position; // unit is m
    double _speed;		// unit is Hz //unit is rpm
	// Ptr
	RS485Port* _rs485Port;
	/* mutex lock */
	HANDLE _ghMutex;

};

static MicroscopeXY* microscopexy;
//static MicroscopeXY microscopexy = MicroscopeXY();

#endif // _XYZTABLE_H_