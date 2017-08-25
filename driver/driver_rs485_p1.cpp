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

static pthread_t pid;

/* flags */
static bool commander_initialized = false;
static volatile bool thread_should_exit = false;	/**< daemon exit flag */
static volatile bool thread_running = false;		/**< daemon status flag */

static uint64_t micros(void) {
	LARGE_INTEGER freq, tick;

	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&tick);

	return (tick.QuadPart * 1000000 / freq.QuadPart);
}

static void WINAPI timer_handler(UINT wTimerID, UINT msg, DWORD dwUser, DWORD dwl, DWORD dw2)
{
	static uint64_t currentTime;

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

	return;
}

/* MODBUS */
#define MODBUS_SERIAL_DEV           "COM2"
//#define MODBUS_SERIAL_DEV           "COM5"
#define MODBUS_SERIAL_BAUDRATE      19200    /* 9600, 38400, 115200, ... 
#define MODBUS_SERIAL_BAUDRATE      115200    /* 9600, 38400, 115200, ... */
#define MODBUS_SERIAL_PARITY        'N'     /* 'N', 'E', or 'O' */
#define MODBUS_SERIAL_DATABITS      8       /* 5, 6, 7, or 8 */
#define MODBUS_SERIAL_STOPBITS      2       /* 1 or 2 */
#define MODBUS_DEVICE_ID            100
#define MODBUS_TIMEOUT_SEC          3
#define MODBUS_TIMEOUT_USEC         0
#define MODBUS_DEBUG                OFF
#define MODBUS_RO_BITS              32
#define MODBUS_RW_BITS              32
#define MODBUS_RO_REGISTERS         64
#define MODBUS_RW_REGISTERS         64

static double AI[32];
static double AO[32];

/* SERVO SDE */
#define S1 0
#define S2 1
#define S3 2
#define S4 3
#define S5 4
#define S6 5
#define S7 6

static bool servo_enabled[7];

void* driver_rs485_p1_thread_main(void* arg)
{
	/* TIMER */
	MMRESULT timer;

	/* TIME */
	uint64_t currentTime;

	/* MODBUS */
	static modbus_t  *ctx[2];

	/* not yet initialized */
	commander_initialized = false;
	/* MODBUS */
	uint16_t data[64];

	/* for servo driver */
	ctx[0] = modbus_new_rtu(MODBUS_SERIAL_DEV,
		115200,
		MODBUS_SERIAL_PARITY,
		MODBUS_SERIAL_DATABITS,
		MODBUS_SERIAL_STOPBITS);

	/* for PID controller */
	ctx[1] = modbus_new_rtu(MODBUS_SERIAL_DEV,
		19200,
		MODBUS_SERIAL_PARITY,
		MODBUS_SERIAL_DATABITS,
		MODBUS_SERIAL_STOPBITS);

	modbus_set_debug(ctx[0], false);
	modbus_set_debug(ctx[1], false);

	/* TEMP */
	modbus_set_slave(ctx[1], 20);
	modbus_connect(ctx[1]);
	AO[1] = 20.0;
	modbus_write_register(ctx[1], 0x0, (int)AO[1] * 10);
	modbus_close(ctx[1]);

	/* SERVO */
	servo_enabled[S1] = true;
	servo_enabled[S2] = true;
	servo_enabled[S3] = true;
	servo_enabled[S4] = true;

	modbus_set_response_timeout(ctx[0], 0, 100000UL);
	//modbus_set_response_timeout(ctx[0], 1, 0);

	/* S4 : DI contact control */
	modbus_set_slave(ctx[0], 4);
	modbus_connect(ctx[0]);
	modbus_write_register(ctx[0], 0x061E, 0x0040);
	modbus_read_registers(ctx[0], 0x0630, 1, data);
	//log_info("addr = 0x0630, data = 0x%X", data[0]);
	data[0] = data[0] | 0x0040;
	modbus_write_register(ctx[0], 0x0630, data[0]);
	modbus_read_registers(ctx[0], 0x0630, 1, data);
	//log_info("addr = 0x0630, data = 0x%X", data[0]);
	modbus_close(ctx[0]);

	timer = timeSetEvent(1, 1, (LPTIMECALLBACK)timer_handler, DWORD(1), TIME_PERIODIC); // TIME_PERIODIC, TIME_ONESHO

	/* now initialized */
	commander_initialized = true;
	thread_running = true;

	currentTime = micros();
	previous1000HzTime = currentTime;
	previous200HzTime = currentTime;
	previous100HzTime = currentTime;
	previous50HzTime = currentTime;
	previous10HzTime = currentTime;
	previous5HzTime = currentTime;
	previous1HzTime = currentTime;

	log_info("driver_rs485_p1_thread_main already running");

	while (!thread_should_exit)
	{
		if (frame_5Hz)
		{
			frame_5Hz = false;

			currentTime = micros();
			deltaTime5Hz = currentTime - previous5HzTime;
			previous5HzTime = currentTime;

			log_info("deltaTime5Hz = %ld", deltaTime5Hz);

			executionTime5Hz = micros() - currentTime;
			//log_info("executionTime5Hz = %ld", executionTime5Hz);
		}

		Sleep(1);
	}

	log_info("kill timer");
	timeKillEvent(timer);

	thread_running = false;

	return 0;
}

int rsh_driver_rs485_p1_main(int argc, char *argv[])
{
	if (argc < 2) {
		log_info("missing command");
		return 0;
	}

	if (!strcmp(argv[1], "start")) {

		if (thread_running) {
			log_info("already running");
			/* this is not an error */
			return 0;
		}

		thread_should_exit = false;

		pthread_create(&pid, NULL, &driver_rs485_p1_thread_main, NULL);

		return 0;
	}

	if (!strcmp(argv[1], "stop")) {

		if (!thread_running) {
			log_info("already stopped");
			return 0;
		}

		thread_should_exit = true;

		while (thread_running) {
			Sleep(200);
			printf(".");
		}

		log_info("[example] terminated.");

		return 0;
	}

	return 0;
}
