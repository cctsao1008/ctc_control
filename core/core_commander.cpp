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

static pthread_t pid;

/* flags */
static bool commander_initialized = false;
static volatile bool thread_should_exit = false;	/**< daemon exit flag */
static volatile bool thread_running = false;		/**< daemon status flag */

/* MQTT */
static mosquitto *mosq;

static double AI[32];
static double AO[32];

static void WINAPI timer_handler(UINT wTimerID, UINT msg, DWORD dwUser, DWORD dwl, DWORD dw2)
{
	
}

static void message_handler(struct mosquitto *, void *, const struct mosquitto_message * msg)
{
    //log_info("mqtt message handler");

    char* data = (char*)(msg->payload);

    if(strcmp(msg->topic, "NONE") == 0)
    {

    }
    else if(strcmp(msg->topic, AO_01) == 0)
    {
        // do something else
        //log_info("AO_01");

        double val = 0;
        
        if (msg->payload != NULL) {
            val = atof((const char*)msg->payload);

            if ((val < 8) || (val > 25))
                log_error("invalid input.");
            else
            {
                AO[1] = val;
                //log_info("val = %4.1f", val);
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

void CALLBACK timer_cb(HWND hwnd, UINT uMsg, UINT timerId, DWORD dwTime)
{
  printf("Hello");
}

void* core_commander_thread_main(void* arg)
{
	/* TIMER */
	MMRESULT timer;

    /* TIME */
    uint64_t currentTime;

    currentTime = micros();

	timer = timeSetEvent(1000, 1, (LPTIMECALLBACK)timer_handler, DWORD(1), TIME_PERIODIC); // TIME_PERIODIC, TIME_ONESHO

    /* MQTT */
    mosquitto_lib_init();
    mosq = mosquitto_new("ctrl.commander", true, NULL);

    //if (mosquitto_connect(mosq, "localhost", 1883, 60 == MOSQ_ERR_SUCCESS) {
    //if (mosquitto_connect_async(mosq, "10.101.4.35", 1883, 60) == MOSQ_ERR_SUCCESS) {
    if (mosquitto_connect_async(mosq, "localhost", 1883, 60) == MOSQ_ERR_SUCCESS) {
        mosquitto_loop_start(mosq);
    }

    // message callback
    mosquitto_message_callback_set(mosq, message_handler);
    mosquitto_subscribe(mosq, NULL, AO_01, 0);
    mosquitto_subscribe(mosq, NULL, AO_02, 0);

    //log_info("core_commander_thread_main running.");

	/* now initialized */
	commander_initialized = true;
	thread_running = true;

	while (!thread_should_exit)
    {          
        Sleep(1);
    }

	timeKillEvent(timer);
    
    mosquitto_disconnect(mosq);
    
    /* call after mosquitto_disconnect */
    mosquitto_loop_stop(mosq, false);
    mosquitto_lib_cleanup();

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

		return 0;
	}

	if (!strcmp(argv[1], "stop")) {

		if (!thread_running) {
			log_info("commander already stopped");
			return 0;
		}

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
