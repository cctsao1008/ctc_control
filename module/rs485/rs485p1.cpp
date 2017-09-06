/**
* @file rs485p1.cpp
*
* rs485p1
*
*   rs485p1.
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

//extern int run;
enum { INIT, F1000HZ, F200HZ, F100HZ, F050HZ, F010HZ, F005HZ, F001HZ };
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

/* MQTT */
static mosquitto *mosq;

rs485p1 rs485p1_data;

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
	if ((frameCounter % COUNT_200HZ) == 0)
		frame_200Hz = true;

	/* frame_100HZ */
	if ((frameCounter % COUNT_100HZ) == 0)
		frame_100Hz = true;

	/* frame_50HZ */
	if ((frameCounter % COUNT_50HZ) == 0)
		frame_50Hz = true;

	/* frame_10HZ */
	if ((frameCounter % COUNT_10HZ) == 0)
		frame_10Hz = true;

	/* frame_5HZ */
	if ((frameCounter % COUNT_5HZ) == 0)
		frame_5Hz = true;

	/* frame_1HZ */
	if ((frameCounter % COUNT_1HZ) == 0)
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

/* SERVO  */
enum { SERVO_1 = 1, SERVO_2, SERVO_3, SERVO_4, SERVO_MAX };

static bool servo_enabled[SERVO_MAX];

int connect(modbus_t *ctx)
{
	//modbus_connect(ctx);

	if (modbus_connect(ctx) == -1) {
		fprintf(stderr, "Connection failed\n");
		modbus_free(ctx);
		return (-1);
	}

	return 0;
}

int disconnect(modbus_t *ctx)
{
	modbus_close(ctx);

	return 0;
}

int read_register(modbus_t *ctx, int slave, int addr, uint16_t *dest)
{
	int rc;

	modbus_connect(ctx);
	modbus_set_slave(ctx, slave);
	rc = modbus_read_registers(ctx, addr, 1, dest);
	modbus_close(ctx);

	return 0;
}

int read_input_register(modbus_t *ctx, int slave, int addr, uint16_t *dest)
{
	int rc;

	modbus_connect(ctx);
	modbus_set_slave(ctx, slave);
	rc = modbus_read_input_registers(ctx, addr, 1, dest);
	modbus_close(ctx);

	return 0;
}


int write_register(modbus_t *ctx, int slave, int addr, int value)
{
	int rc;

	modbus_connect(ctx);
	modbus_set_slave(ctx, slave);
	rc = modbus_write_register(ctx, addr, value);
	modbus_close(ctx);

	return 0;
}

uint16_t bswap8(uint16_t a)
{
	a = ((a & 0x0F) << 4) | ((a & 0xF0) >> 4);
	return a;
}

uint16_t bswap16(uint16_t a)
{
	a = ((a & 0x00FF) << 8) | ((a & 0xFF00) >> 8);
	return a;
}

uint32_t _bswap32(uint32_t a)
{
	a = ((a & 0x000000FF) << 24) |
		((a & 0x0000FF00) << 8) |
		((a & 0x00FF0000) >> 8) |
		((a & 0xFF000000) >> 24);
	return a;
}

uint64_t _bswap64(uint64_t a)
{
	a = ((a & 0x00000000000000FFULL) << 56) |
		((a & 0x000000000000FF00ULL) << 40) |
		((a & 0x0000000000FF0000ULL) << 24) |
		((a & 0x00000000FF000000ULL) << 8) |
		((a & 0x000000FF00000000ULL) >> 8) |
		((a & 0x0000FF0000000000ULL) >> 24) |
		((a & 0x00FF000000000000ULL) >> 40) |
		((a & 0xFF00000000000000ULL) >> 56);
	return a;
}

uint16_t servo_sync(modbus_t *ctx, uint8_t id)
{
	uint16_t data = 0xFFFF;

	modbus_set_slave(ctx, id);
	modbus_read_registers(ctx, 0x0900, 1, &data);

	return data;
}

void update_previous_time(uint64_t currentTime, uint8_t frame)
{
	switch (frame)
	{
		case INIT:

			previous1000HzTime = currentTime;
			previous200HzTime = currentTime;
			previous100HzTime = currentTime;
			previous50HzTime = currentTime;
			previous10HzTime = currentTime;
			previous5HzTime = currentTime;
			previous1HzTime = currentTime;

			break;

		case F1000HZ:
			deltaTime1000Hz = currentTime - previous1000HzTime;
			previous1000HzTime = currentTime;
			break;

		case F200HZ : 
			deltaTime200Hz = currentTime - previous200HzTime;
			previous200HzTime = currentTime;
			break;

		case F100HZ:
			deltaTime100Hz = currentTime - previous100HzTime;
			previous100HzTime = currentTime;
			break;

		case F050HZ:
			deltaTime50Hz = currentTime - previous50HzTime;
			previous50HzTime = currentTime;
			break;

		case F010HZ:
			deltaTime10Hz = currentTime - previous10HzTime;
			previous10HzTime = currentTime;
			break;

		case F005HZ:
			deltaTime5Hz = currentTime - previous5HzTime;
			previous5HzTime = currentTime;
			break;

		case F001HZ:
			deltaTime10Hz = currentTime - previous10HzTime;
			previous10HzTime = currentTime;
			break;

		default :
			break;


	}
}

void* rs485p1_thread_main(void* arg)
{
	/* TIMER */
	MMRESULT timer;

	/* TIME */
	uint64_t currentTime;

	/* LCM */
	//uint8_t mc_ttl = 0;
	//struct in_addr mc_addr;
	//int mc_port = htons(7667);

	//if (inet_pton("239.255.76.67", (struct in_addr*) &mc_addr) < 0)
	//	return 1;
	//inet_addr("127.0.0.1");

	// create the Multicast UDP socket
	//struct sockaddr_in read_addr, send_addr;

	//memset(&read_addr, 0, sizeof(read_addr));
	//read_addr.sin_family = AF_INET;
	//read_addr.sin_addr.s_addr = INADDR_ANY;
	//read_addr.sin_port = mc_port;

	//memset(&send_addr, 0, sizeof(read_addr));
	//send_addr.sin_family = AF_INET;
	//send_addr.sin_addr = mc_addr;
	//send_addr.sin_port = mc_port;

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

	if (ctx[0] == NULL) {
		fprintf(stderr, "Unable to allocate libmodbus context\n");
		return 0;
	}

	/* for PID controller */
	ctx[1] = modbus_new_rtu(MODBUS_SERIAL_DEV,
		19200,
		MODBUS_SERIAL_PARITY,
		MODBUS_SERIAL_DATABITS,
		MODBUS_SERIAL_STOPBITS);

	if (ctx[1] == NULL) {
		fprintf(stderr, "Unable to allocate libmodbus context\n");
		return 0;
	}

	modbus_set_debug(ctx[0], false);
	modbus_set_debug(ctx[1], false);

	/* TEMP */
	modbus_set_slave(ctx[1], 20);

	AO[1] = 20.0;
	write_register(ctx[1], 20, 0x0, (int)AO[1] * 10);

	/* ENABLE SERVO */
	servo_enabled[SERVO_1] = true;
	servo_enabled[SERVO_2] = true;
	servo_enabled[SERVO_3] = true;
	servo_enabled[SERVO_4] = true;

	modbus_set_response_timeout(ctx[0], 0, 100000UL);
	//modbus_set_response_timeout(ctx[0], 1, 0);

	/* S4 : DI contact control */
#if 0
	modbus_set_slave(ctx[0], SERVO_4);
	modbus_connect(ctx[0]);
	modbus_write_register(ctx[0], 0x061E, 0x0040);
	modbus_read_registers(ctx[0], 0x0630, 1, data);
	//log_info("addr = 0x0630, data = 0x%X", data[0]);
	data[0] = data[0] | 0x0040;
	modbus_write_register(ctx[0], 0x0630, data[0]);
	modbus_read_registers(ctx[0], 0x0630, 1, data);
	//log_info("addr = 0x0630, data = 0x%X", data[0]);
	modbus_close(ctx[0]);
#endif

	/* MQTT */
	mosquitto_lib_init();
	mosq = mosquitto_new("ctrl.rs485p1", true, NULL);

	if (NULL == mosq)
		printf("rs485p1 mqtt error\n");
	else
	{
		if (mosquitto_connect_async(mosq, "localhost", 1883, 60) == MOSQ_ERR_SUCCESS) {
			mosquitto_loop_start(mosq);
		}
	}

	/* TIMER */
	timer = timeSetEvent(1, 1, (LPTIMECALLBACK)timer_handler, DWORD(1), TIME_PERIODIC); // TIME_PERIODIC, TIME_ONESHO

	/* now initialized */
	commander_initialized = true;
	thread_running = true;

	currentTime = micros();

	update_previous_time(currentTime, INIT);


	/* Define a new and too short timeout! */
	modbus_set_response_timeout(ctx[0], 0, 10000);
	modbus_set_response_timeout(ctx[1], 0, 10000);

	log_info("driver_rs485_p1_thread_main already running");

	while (!thread_should_exit)
	{
		if (frame_5Hz)
		{
			frame_5Hz = false;

			currentTime = micros();
			update_previous_time(currentTime, F005HZ);

			if (deltaTime5Hz > 210000L)
				log_error("deltaTime5Hz (%4.2f) > 210 ms", deltaTime5Hz/1000.0f);

			//log_info("deltaTime5Hz = %.2f ms", deltaTime5Hz/1000.0f);

			/* keep servo driver connection*/
			modbus_connect(ctx[0]);

			// Servo driver SDE-075A2 750W, MP X axis
			if (servo_enabled[SERVO_1] == true)
			{
				//log_info("S1 ENABLED");
				data[0] = servo_sync(ctx[0], SERVO_1);
				//printf("S1 data[0] = 0x%04X \n", bswap8(data[0]));
				printf("S1 data[0] = 0x%04X \n", data[0]);
			}

			Sleep(10);

			// Servo driver SDE-040A2 400W, MP Y axis
			if (servo_enabled[SERVO_2] == true)
			{
				//log_info("S2 ENABLED");
				data[0] = servo_sync(ctx[0], SERVO_2);
				//printf("S2 data[0] = 0x%04X \n", bswap8(data[0]));
				printf("S1 data[0] = 0x%04X \n", data[0]);
			}

			Sleep(10);

			// Servo driver SDE-010A2 100W, Guild Way
			if (servo_enabled[SERVO_3] == true)
			{
				//log_info("S3 ENABLED");
				data[0] = servo_sync(ctx[0], SERVO_3);
				//printf("S3 data[0] = 0x%04X \n", bswap8(data[0]));
				printf("S1 data[0] = 0x%04X \n", data[0]);
			}

			Sleep(10);

			// Servo driver SDE-020A2 200W, Centrifugal
			if (servo_enabled[SERVO_4] == true)
			{
				//log_info("S4 ENABLED");
				data[0] = servo_sync(ctx[0], SERVO_4);
				//printf("S4 data[0] = 0x%04X \n", bswap8(data[0]));
				printf("S1 data[0] = 0x%04X \n", data[0]);
#if 0
				modbus_set_slave(ctx[0], SERVO_4);
				//modbus_read_registers(ctx[0], 0x0900, 1, data);
				int rc;
				data[0] = 0;
				rc = modbus_read_registers(ctx[0], 0x0100, 1, data);

				//log_info("data = 0x%X, rc = %d", data[0], rc);

				//if (rc != (-1))
				//	log_info("0x0100 = 0x%X", data[0]);
#else
				//read_servo_register(ctx[0], SERVO_4, 0x0100, 1, data);
#endif
			}

			Sleep(10);

			modbus_close(ctx[0]);

			executionTime5Hz = micros() - currentTime;

			if (executionTime5Hz > 100000L)
				log_error("executionTime5Hz (%4.2f) > 100 ms", executionTime5Hz / 1000.0f);

			//log_info("executionTime5Hz = %.2f ms", executionTime5Hz / 1000.0f);
		}

		if (frame_1Hz)
		{
			int rc = 0;
			frame_1Hz = false;

			currentTime = micros();
			update_previous_time(currentTime, F001HZ);

#if 1
			//rc = modbus_set_slave(ctx[1], 20);
			//rc = modbus_connect(ctx[1]);
			//rc = modbus_read_input_registers(ctx[1], 0x0, 1, data);
			//modbus_close(ctx[1]);
			read_input_register(ctx[1], 20, 0x0, data);

			char str[10];
#if 0
			// real data
			sprintf_s(str, "%3.1f", (float)data[0] * 0.1);
			//mosquitto_publish(mosq, NULL, AI_01, 64, str, 0, true);
			//log_info("FT3400 PV = %3.1f", (float)data[0] * 0.1);
#else
			// fake data
			//double tmp = (double)((rand() / (RAND_MAX + 1.0)) * (110.0 - 90.0) + 110.0); // 1~5 us
			double tmp = 90.0 + (double)rand() / ((double)RAND_MAX / (110.00 - 90.0));
			sprintf_s(str, "%3.1f", tmp * 0.1); // 9~27 us
			mosquitto_publish(mosq, NULL, "CONTROL/INPUT/AI/01", 64, str, 0, true); // 156~812 us
			log_info("tmp = %3.1f", tmp * 0.1);
			mosquitto_publish(mosq, NULL, "CONTROL/INPUT/SERVO/01", 16, "ON", 0, true);
			mosquitto_publish(mosq, NULL, "CONTROL/INPUT/SERVO/02", 16, "ON", 0, true);
			mosquitto_publish(mosq, NULL, "CONTROL/INPUT/SERVO/03", 16, "ON", 0, true);
			mosquitto_publish(mosq, NULL, "CONTROL/INPUT/SERVO/04", 16, "ON", 0, true);
#endif
#endif
			executionTime1Hz = micros() - currentTime;
			//log_info("deltaTime1Hz = %d us", deltaTime1Hz);
			//log_info("executionTime1Hz = %d us", executionTime1Hz);
		}

		Sleep(1);
	}

	modbus_free(ctx[0]);
	modbus_free(ctx[1]);

	log_info("kill timer");
	timeKillEvent(timer);

	thread_running = false;

	return 0;
}

int rsh_rs485p1_main(int argc, char *argv[])
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

		pthread_create(&pid, NULL, &rs485p1_thread_main, NULL);

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

	/* commands needing the app to run below */
	if (!thread_running) {
		log_info("rs485p1 not started");
		return 1;
	}

	if (!strcmp(argv[1], "ac")) {
		log_info("[commander] pub, argc = %d", argc);

		if (argc < 3)
		{
			log_info("error");

			return 0;
		}

#if 0
		printf("params = \n");

		for (int i = 2; i < argc; i++)
		{
			printf("(%d) %s\n", i, argv[i]);
		}
#else
		char topic[128] = { 0 };
		char payload[128] = { 0 };

		for (int i = 2; i < argc; i++)
		{
			if (!strcmp(argv[i], "-w"))
			{
				printf("www");
			}

			if (!strcmp(argv[i], "-r"))
			{
				double val = 0;

				if (argv[i + 1] != NULL) {
					val = atof((const char*)argv[i + 1]);

					if ((val < 8) || (val > 25))
						log_error("invalid input, val = %4.1f", val);
					else
					{
						rs485p1_data.tc.sv = (uint16_t)val;
						log_info("val = %4.1f", val);
					}
				}
				else {
					log_error("invalid input, not a number!");
				}
			}
		}

		//mosquitto_publish(mosq, NULL, topic, sizeof(payload), payload, 0, true);
#endif
	}

	return 0;
}
