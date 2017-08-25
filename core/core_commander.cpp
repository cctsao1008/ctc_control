/**
* @file core_commander.cpp
*
* core_commander
*
*   core_commander.
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

/* MQTT */
#define AI_01 "CONTROL/INPUT/AI/01"
#define AI_02 "CONTROL/INPUT/AI/02"
#define AI_03 "CONTROL/INPUT/AI/03"
#define AI_04 "CONTROL/INPUT/AI/04"
#define AI_05 "CONTROL/INPUT/AI/05"
#define AI_06 "CONTROL/INPUT/AI/06"
#define AI_07 "CONTROL/INPUT/AI/07"
#define AI_08 "CONTROL/INPUT/AI/08"

#define AO_01 "CONTROL/OUTPUT/AO/01"
#define AO_02 "CONTROL/OUTPUT/AO/02"
#define AO_03 "CONTROL/OUTPUT/AO/03"
#define AO_04 "CONTROL/OUTPUT/AO/04"
#define AO_05 "CONTROL/OUTPUT/AO/05"
#define AO_06 "CONTROL/OUTPUT/AO/06"
#define AO_07 "CONTROL/OUTPUT/AO/07"
#define AO_08 "CONTROL/OUTPUT/AO/08"

extern int run;
extern int vote;

static pthread_t pid;
static MMRESULT timer;

/* flags */
static bool commander_initialized = false;
static volatile bool thread_should_exit = false;	/**< daemon exit flag */
static volatile bool thread_running = false;		/**< daemon status flag */

/* TIME */
static struct timeval t;
static long t_curr;
static uint32_t msticks = 0;

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

/* MQTT */
static mosquitto *mosq;

static double AI[32];
static double AO[32];

/* MODBUS */
static modbus_t  *ctx[2];

/* SERVO SDE */
#define S1 0
#define S2 1
#define S3 2
#define S4 3
#define S5 4
#define S6 5
#define S7 6

static bool servo_enabled[7];

static uint64_t micros(void) {
    LARGE_INTEGER freq, tick;

    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&tick);

    return (tick.QuadPart * 1000000 / freq.QuadPart);
}

static void WINAPI timer_handler(UINT wTimerID, UINT msg, DWORD dwUser, DWORD dwl, DWORD dw2)
{
    static DWORD start = GetTickCount(), end;
    static LARGE_INTEGER freq, time;
    static LARGE_INTEGER start_time;
    static LARGE_INTEGER end_time;
    static uint64_t currentTime;

    bool debug = false;

	if (thread_running){
#if 0
        clock_gettime(CLOCK_MONOTONIC, &t);
        log_info("usec = %ld", (t.tv_usec - t_curr));
        log_info("msec = %f", (t.tv_usec - t_curr)/1000.0);
        log_info("sec = %f", (t.tv_usec - t_curr) / 1000000.0);
        //t_curr = t.tv_usec;

        log_info("t.tv_sec = %ld", t.tv_sec);
        log_info("t.tv_usec = %lld", t.tv_usec);
#endif

#if 0
        DWORD end = GetTickCount();
        log_info("tick = %d", end - start);
        start = end;
#endif

#if 0
        QueryPerformanceFrequency(&freq);
        QueryPerformanceCounter(&end_time);
        log_info("tick = %f", (end_time.QuadPart - start_time.QuadPart) * 1000.0f / freq.QuadPart);
        log_info("round tick = %f", round((end_time.QuadPart - start_time.QuadPart) * 1000.0f / freq.QuadPart));
        start_time = end_time;
#endif

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

static void message_handler(struct mosquitto *, void *, const struct mosquitto_message * msg)
{
    log_info("CCS: on_message.");

    char* data = (char*)(msg->payload);

    if(strcmp(msg->topic, "NONE") == 0)
    {

    }
    else if(strcmp(msg->topic, AO_01) == 0)
    {
        // do something else
        log_info("AO_01");

        double val = 0;
        
        if (msg->payload != NULL) {
            val = atof((const char*)msg->payload);

            if ((val < 8) || (val > 25))
                log_error("invalid input.");
            else
            {
                AO[1] = val;
                log_info("val = %4.1f", val);
            }
        }
        else {
            log_error("invalid input.");
        }
    }
    else if(strcmp(msg->topic, AO_02) == 0)
    {
        // do something else
        log_info("AO_02");
    }
    else if(strcmp(msg->topic, AO_03) == 0)
    {
        // do something else
        log_info("AO_02");
    }
    else if(strcmp(msg->topic, AO_04) == 0)
    {
        // do something else
        log_info("AO_02");
    }
    else if(strcmp(msg->topic, AO_05) == 0)
    {
        // do something else
        log_info("AO_02");
    }
    /* more else if clauses */
    else /* default: */
    {
        printf("topic = %s, payload = %s\n", msg->topic, (char*)msg->payload);
    }

}

static uint8_t mqtt_publisher(modbus_t *ctx)
{

}

void* core_commander_thread_main(void* arg)
{
    /* TIME */
    uint64_t currentTime;
    srand((unsigned int)time(NULL));

    currentTime = micros();
    previous1000HzTime = currentTime;
    previous200HzTime = currentTime;
    previous100HzTime = currentTime;
    previous50HzTime = currentTime;
    previous10HzTime = currentTime;
    previous5HzTime = currentTime;
    previous1HzTime = currentTime;

    /* MQTT */
    mosquitto_lib_init();
    mosq = mosquitto_new("ctc_control", true, NULL);

    //if (mosquitto_connect(mosq, "localhost", 1883, 60 == MOSQ_ERR_SUCCESS) {
    //if (mosquitto_connect_async(mosq, "10.101.4.35", 1883, 60) == MOSQ_ERR_SUCCESS) {
    if (mosquitto_connect_async(mosq, "localhost", 1883, 60) == MOSQ_ERR_SUCCESS) {
        mosquitto_loop_start(mosq);
    }

    // message callback
    mosquitto_message_callback_set(mosq, message_handler);
    mosquitto_subscribe(mosq, NULL, AO_01, 0);
    mosquitto_subscribe(mosq, NULL, AO_02, 0);

    /* MODBUS */
    uint16_t data[64];

    //log_info("libmodbus version %s (%06X)", LIBMODBUS_VERSION_STRING, LIBMODBUS_VERSION_HEX);

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
    //modbus_set_error_recovery(ctx,
    //                          MODBUS_ERROR_RECOVERY_LINK |
    //                          MODBUS_ERROR_RECOVERY_PROTOCOL);

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

    vote++;

    //log_info("core_commander_thread_main running.");

	/* now initialized */
	commander_initialized = true;
	thread_running = true;

	while (!thread_should_exit)
    {
        if (frame_5Hz)
        {
            frame_5Hz = false;

            //log_info("update rate 5hz");

            /* update ticks*/
            currentTime = micros();
            deltaTime5Hz = currentTime - previous5HzTime;
            previous5HzTime = currentTime;

            // put your code here.
            /* keep servo driver connection*/
            modbus_connect(ctx[0]);

            // Servo driver SDE-075A2 750W, MP X axis
            if (servo_enabled[S1] == true)
            {
                //log_info("S1 ENABLED");
                modbus_set_slave(ctx[0], 1);
                modbus_read_registers(ctx[0], 0x0900, 1, data);
            }

            // Servo driver SDE-040A2 400W, MP Y axis
            if (servo_enabled[S2] == true)
            {
                //log_info("S2 ENABLED");
                modbus_set_slave(ctx[0], 2);
                modbus_read_registers(ctx[0], 0x0900, 1, data);
            }

            // Servo driver SDE-010A2 100W, Guild Way
            if (servo_enabled[S3] == true)
            {
                //log_info("S3 ENABLED");
                modbus_set_slave(ctx[0], 3);
                modbus_read_registers(ctx[0], 0x0900, 1, data);
            }

            // Servo driver SDE-020A2 200W, Centrifugal
            if (servo_enabled[S4] == true)
            {
                //log_info("S4 ENABLED");
                modbus_set_slave(ctx[0], 4);
                //modbus_read_registers(ctx[0], 0x0900, 1, data);
                int rc;
                data[0] = 0;
                rc = modbus_read_registers(ctx[0], 0x0100, 1, data);

                //log_info("data = 0x%X, rc = %d", data[0], rc);

                //if (rc != (-1))
                //	log_info("0x0100 = 0x%X", data[0]);
            }

            modbus_close(ctx[0]);

            //log_info("deltaTime5Hz = %d us", deltaTime5Hz);
            //log_info("executionTime5Hz = %d us", executionTime5Hz);
        }

        if (frame_1Hz)
        {
            frame_1Hz = false;

            //log_info("update rate 1hz");

            /* update ticks*/
            currentTime = micros();
            deltaTime1Hz = currentTime - previous1HzTime;
            previous1HzTime = currentTime;

            modbus_set_slave(ctx[1], 20);
            modbus_connect(ctx[1]);
            modbus_read_input_registers(ctx[1], 0x0, 1, data);
            modbus_close(ctx[1]);

            char str[10];
#if 1
            // real data
            sprintf_s(str, "%3.1f", (float)data[0] * 0.1);
            mosquitto_publish(mosq, NULL, AI_01, 64, str, 0, true);
            //log_info("FT3400 PV = %3.1f", (float)data[0] * 0.1);
#else
            // fake data
            //double tmp = (double)((rand() / (RAND_MAX + 1.0)) * (110.0 - 90.0) + 110.0); // 1~5 us
            double tmp = 90.0 + (double)rand() / ((double)RAND_MAX / (110.00 - 90.0));
            sprintf_s(str, "%3.1f", tmp * 0.1); // 9~27 us
            mosquitto_publish(mosq, NULL, AI_01, 64, str, 0, true); // 156~812 us
            log_info("tmp = %3.1f", tmp * 0.1);
#endif
            executionTime1Hz = micros() - currentTime;
            //log_info("deltaTime1Hz = %d us", deltaTime1Hz);
            //log_info("executionTime1Hz = %d us", executionTime1Hz);
        }

        Sleep(1);
    }

    modbus_free(ctx[0]);
    modbus_free(ctx[1]);
    
    mosquitto_disconnect(mosq);
    
    /* call after mosquitto_disconnect */
    mosquitto_loop_stop(mosq, false);
    mosquitto_lib_cleanup();

    vote--;

	thread_running = false;

    //log_info("core_commander_thread_main exit.");

    return 0;
}

int rsh_core_commander_main(int argc, char *argv[])
{
	int c;

	optind = 0;

	if (argc < 2) {
		log_info("missing command");
		return 0;
	}

	while ((c = getopt(argc, argv, "tm:")) != -1) {
		switch (c) {

		case 't':
			log_info("getopt t, %s", argv[2]);
			break;

		case 'm':
			log_info("getopt m");
			break;

		default:
			log_info("getopt default");
			//usage(nullptr);
			return 0;
		}
	}

	if (!strcmp(argv[1], "start")) {

		if (thread_running) {
			log_info("already running");
			/* this is not an error */
			return 0;
		}

		thread_should_exit = false;

		pthread_create(&pid, NULL, &core_commander_thread_main, NULL);

		timer = timeSetEvent(1, 1, (LPTIMECALLBACK)timer_handler, DWORD(1), TIME_PERIODIC); // TIME_PERIODIC, TIME_ONESHO

		return 0;
	}

	if (!strcmp(argv[1], "stop")) {

		if (!thread_running) {
			log_info("commander already stopped");
			return 0;
		}

		timeKillEvent(timer);

		thread_should_exit = true;

		//while (thread_running) {
		//	Sleep(200);
		//	printf(".");
		//}

		//log_info("terminated.");

		return 0;
	}

	/* commands needing the app to run below */
	if (!thread_running) {
		log_info("commander not started");
		return 1;
	}

	if (!strcmp(argv[1], "status")) {
		if (thread_running) {
			log_info("running");
			return 0;
		}
		else {
			log_info("not running");
			return 1;
		}
	}

	if (!strcmp(argv[1], "pub")) {
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
			if (!strcmp(argv[i], "-t"))
				strcpy_s(topic, argv[i + 1]);

			if (!strcmp(argv[i], "-m"))
				strcpy_s(payload, argv[i + 1]);

		}

		mosquitto_publish(mosq, NULL, topic, sizeof(payload), payload, 0, true);
#endif
	}

	return 0;
}

#if 0
int action_pippet_control(void* data)
{
    // put your code here.
    return 0;
}

int action_platform_motion(void* data)
{
    // put your code here.
    return 0;
}

int action_tool_control(void* data)
{
    // put your code here.
    return 0;
}

int action_centrifuge_control(void* data)
{
    // put your code here.
    return 0;
}
#endif
