/**
* @file driver_rs485_p1.cpp
*
* driver_rs485_p1
*
*   driver_rs485_p1.
*
* @author Ricardo <tsao.ricardo@iac.com.tw>
*/

#include "core_common.h"

/* TIME */
#define FRAME_COUNT   1000

#define COUNT_200HZ   5         // Number of 1000 Hz frames for 200 Hz Loop
#define COUNT_100HZ   10        // Number of 1000 Hz frames for 100 Hz Loop
#define COUNT_50HZ    20        // Number of 1000 Hz frames for  50 Hz Loop
#define COUNT_10HZ    100       // Number of 1000 Hz frames for  10 Hz Loop
#define COUNT_5HZ     200       // Number of 1000 Hz frames for   5 Hz Loop
#define COUNT_1HZ     1000      // Number of 1000 Hz frames for   1 Hz Loop

extern int run;

static uint16_t frameCounter = 0;

static bool frame_200Hz = false;
static bool frame_100Hz = false;
static bool frame_50Hz = false;
static bool frame_10Hz = false;
static bool frame_5Hz = false;
static bool frame_1Hz = false;

static uint64_t deltaTime1000Hz, executionTime1000Hz, previous1000HzTime;
static uint64_t deltaTime200Hz, executionTime200Hz, previous200HzTime;
static uint64_t deltaTime100Hz, executionTime100Hz, previous100HzTime;
static uint64_t deltaTime50Hz, executionTime50Hz, previous50HzTime;
static uint64_t deltaTime10Hz, executionTime10Hz, previous10HzTime;
static uint64_t deltaTime5Hz, executionTime5Hz, previous5HzTime;
static uint64_t deltaTime1Hz, executionTime1Hz, previous1HzTime;

static uint64_t micros(void) {
	LARGE_INTEGER freq, tick;

	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&tick);

	return (tick.QuadPart * 1000000 / freq.QuadPart);
}

static void WINAPI timer_handler(UINT wTimerID, UINT msg, DWORD dwUser, DWORD dwl, DWORD dw2)
{
	static uint64_t currentTime;

	bool debug = false;

	if (run){

#if 1
		frameCounter++;

		//log_info("frame_1000Hz %d", deltaTime1000Hz);

		if (frameCounter > FRAME_COUNT) {
			frameCounter = 1;
		}

		currentTime = micros();
		deltaTime1000Hz = currentTime - previous1000HzTime;
		previous1000HzTime = currentTime;

		/* frame_200HZ */
		if ((frameCounter % COUNT_200HZ) == 0 && frame_200Hz != true)
			frame_200Hz = true;

		/* frame_100HZ */
		if ((frameCounter % COUNT_100HZ) == 0 && frame_100Hz != true)
			frame_100Hz = true;

		/* frame_50HZ */
		if ((frameCounter % COUNT_50HZ) == 0 && frame_50Hz != true)
			frame_50Hz = true;

		/* frame_10HZ */
		if ((frameCounter % COUNT_10HZ) == 0 && frame_10Hz != true)
			frame_10Hz = true;

		/* frame_5HZ */
		if ((frameCounter % COUNT_5HZ) == 0 && frame_5Hz != true)
			frame_5Hz = true;

		/* frame_1HZ */
		if ((frameCounter % COUNT_1HZ) == 0 && frame_1Hz != true)
			frame_1Hz = true;


		executionTime1000Hz = micros() - currentTime;
#endif
	}

	return;
}

//void* driver_rs485_p1_thread_main(void* arg)
//{
//	return;
//}

int driver_rs485_p1_main(int argc, char *argv[])
{
	/* TIME */
	//MMRESULT timer;
	//uint64_t currentTime;

	/*
	currentTime = micros();
	previous1000HzTime = currentTime;
	previous200HzTime = currentTime;
	previous100HzTime = currentTime;
	previous50HzTime = currentTime;
	previous10HzTime = currentTime;
	previous5HzTime = currentTime;
	previous1HzTime = currentTime;

	timer = timeSetEvent(1, 1, (LPTIMECALLBACK)timer_handler, DWORD(1), TIME_PERIODIC); // TIME_PERIODIC, TIME_ONESHO
	*/

	return 0;
}
