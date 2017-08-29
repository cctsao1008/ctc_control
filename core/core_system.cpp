/**
* @file core_system.cpp
*
* core_system
*
*   core_system.
*
* @author Ricardo <tsao.ricardo@iac.com.tw>
*/

#include "core_common.h"

/* rs485 p1*/
int rpm = 0;

/* rs485 p2*/

uint64_t micros(void) {
	LARGE_INTEGER freq, tick;

	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&tick);

	return (tick.QuadPart * 1000000 / freq.QuadPart);
}


