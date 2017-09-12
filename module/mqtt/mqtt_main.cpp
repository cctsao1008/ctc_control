/**
* @file mqtt_receiver.cpp
*
* mqtt_receiver
*
*   mqtt_receiver.
*
* @author Ricardo <tsao.ricardo@iac.com.tw>
*/

#include "core_common.h"
#include "mqtt.h"

extern uint8_t mqtt_receiver(const char* topic, const char* cmd);
static void* mqtt_thread_main(void* arg);
static void on_message(struct mosquitto *, void *, const struct mosquitto_message * msg);

/* pthreads */
static pthread_t pid;

/* mqtt */
static mosquitto *mosq;

/* flags */
static bool commander_initialized = false;
static volatile bool thread_should_exit = false;	/**< daemon exit flag */
static volatile bool thread_running = false;		/**< daemon status flag */

int rsh_mqtt_main(int argc, char *argv[])
{
	if (argc < 2) {
		log_info("[mqtt] missing command");
		return 0;
	}

	if (!strcmp(argv[1], "start")) {

		if (thread_running) {
			log_info("[mqtt] already running");
			/* this is not an error */
			return 0;
		}

		thread_should_exit = false;

		pthread_create(&pid, NULL, &mqtt_thread_main, NULL);

		return 0;
	}

	if (!strcmp(argv[1], "stop")) {

		if (!thread_running) {
			log_info("[mqtt] already stopped");
			return 0;
		}

		thread_should_exit = true;

		while (thread_running) {
			Sleep(200);
			//printf(".");
		}

		log_info("[mqtt] terminated.");

		return 0;
	}

	return 0;
}

static void* mqtt_thread_main(void* arg)
{
	/* not yet initialized */
	commander_initialized = false;

	/* mqtt */
	mosquitto_lib_init();
	mosq = mosquitto_new("ctrl.mqtt", true, NULL);

	if (mosquitto_connect_async(mosq, "localhost", 1883, 60) == MOSQ_ERR_SUCCESS) {
		mosquitto_loop_start(mosq);
	}

	/* message callback */
	mosquitto_message_callback_set(mosq, on_message);

	mosquitto_subscribe(mosq, NULL, TOPIC_PYC_CMD, 0);
	mosquitto_subscribe(mosq, NULL, TOPIC_RSH_CMD, 0);

	/* now initialized */
	commander_initialized = true;
	thread_running = true;

	while (!thread_should_exit)
	{
		//log_info("[mqtt] mqtt_thread_main running");
		Sleep(2000);
	}

	mosquitto_disconnect(mosq);

	/* call after mosquitto_disconnect */
	mosquitto_loop_stop(mosq, false);
	mosquitto_lib_cleanup();

	thread_running = false;

	return 0;
}

static void on_message(struct mosquitto *, void *, const struct mosquitto_message * msg)
{
	//log_info("mqtt message handler");

	printf("topic = %s, payload = %s\n", msg->topic, (char*)msg->payload);

	if (strcmp(msg->topic, TOPIC_PYC_CMD) == 0)
	{
		printf("python command received.\n");
		mqtt_receiver(TOPIC_PYC_CMD, (char*)msg->payload);
	}
	else if (strcmp(msg->topic, TOPIC_RSH_CMD) == 0)
	{
		printf("rsh command received.\n");
		mqtt_receiver(TOPIC_RSH_CMD, (char*)msg->payload);
	}
	else /* default: */
	{
		printf("topic = %s, payload = %s\n", msg->topic, (char*)msg->payload);
	}
}