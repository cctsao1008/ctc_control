/*********************************************
 * @file XYZTable.h                          *
 *                                           *
 * XYZ Table Contorl                         *
 *                                           *
 * @author Joshua <jan.joshua@iac.com.tw>    *
 *********************************************/

#ifndef _CORE_XYZTABLE_H_
#define _CORE_XYZTABLE_H_

#include "c_serial.h"
#include "core_common.h"
#include "core_bianneng.h"

typedef struct _position 
{
    double X;
    double Y;
    double Z;
} Position;

class XYZTable : public BianNeng
{

public:

    // Constructor
	XYZTable(RS485Port* PortPtr);
	XYZTable(Position Pos, RS485Port* PortPtr);
	XYZTable(double X, double Y, double Z, RS485Port* PortPtr);

    // Destructor
    ~XYZTable();

    // Information
    Position getPositon() const;
    double getX() const;
    double getY() const;
    double getZ() const;
    double getSpeed() const;

    // Action
    bool move2Pos(Position Pos);
    bool move2Pos(double X, double Y, double Z);

    // Modifier
    bool initDriver();
    bool setSpeed(double Speed);

private:
    // Combined with the driver
	bool moveX(double X, uint8_t Direction);
	bool moveY(double Y, uint8_t Direction);
	bool moveZ(double Z, uint8_t Direction);
    // Status
    Position _position; // unit is m
    double _speed;		// unit is Hz //unit is rpm
	// Ptr
	RS485Port* _rs485Port;
	/* mutex lock */
	HANDLE _ghMutex;

};

#endif // _XYZTABLE_H_