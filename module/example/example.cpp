/**
* @file example.cpp
*
* example
*
*   example.
*
* @author Ricardo <tsao.ricardo@iac.com.tw>
*/

#include "core_common.h"

static pthread_t pid;

/* flags */
static bool commander_initialized = false;
static volatile bool thread_should_exit = false;	/**< daemon exit flag */
static volatile bool thread_running = false;		/**< daemon status flag */

static void WINAPI timer_handler(UINT wTimerID, UINT msg, DWORD dwUser, DWORD dwl, DWORD dw2)
{
	log_info("[example] timer handler");

	uint64_t currentTime = micros();

	log_info("[example] currentTime = %ld", currentTime);

	return;
}

void* example_thread_main(void* arg)
{
	MMRESULT timer;

	/* not yet initialized */
	commander_initialized = false;

	log_info("[example] create timer thread");
	timer = timeSetEvent(1000, 1, (LPTIMECALLBACK)timer_handler, DWORD(1), TIME_PERIODIC); // TIME_PERIODIC, TIME_ONESHO

	/* now initialized */
	commander_initialized = true;
	thread_running = true;

	while (!thread_should_exit)
	{
		log_info("[example] example thread main");
		Sleep(2000);
	}

	log_info("[example] kill timer thread");
	timeKillEvent(timer);

	thread_running = false;

	return 0;
}

int rsh_example_main(int argc, char *argv[])
{
	if (argc < 2) {
		log_info("[example] missing command");
		return 0;
	}

	if (!strcmp(argv[1], "start")) {

		if (thread_running) {
			log_info("[example] already running");
			/* this is not an error */
			return 0;
		}

		thread_should_exit = false;

		pthread_create(&pid, NULL, &example_thread_main, NULL);

		return 0;
	}

	if (!strcmp(argv[1], "stop")) {

		if (!thread_running) {
			log_info("[example] already stopped");
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
