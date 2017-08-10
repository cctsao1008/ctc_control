/**
* @file core_control.cpp
*
* core_control
*
*   core_control.
*
* @author Ricardo <tsao.ricardo@iac.com.tw>
*/

#include "core_common.h"
#include <signal.h>

#include "log.h"

//DWORD WINAPI core_control_thread_main(LPVOID lpParam);

int run = 1;
int vote = 0;

/* flags */
static bool commander_initialized = false;
static volatile bool thread_should_exit = false;    /**< daemon exit flag */
static volatile bool thread_running = false;        /**< daemon status flag */

HANDLE ghMutex;

void WINAPI console_ctrl_handler(DWORD event)
{
    if (event == CTRL_C_EVENT)
		log_warn("Ctrl-C handled\n"); // do cleanup

    OutputDebugString("console_ctrl_handler.");
	log_info("console_ctrl_handler.");
    core_mqtt_close();
    run = 0;

    //Sleep(5000);

#if 0
    for (int i = 0; i < 30; i++)
    {
        //printf("%d\n", i);
        printf(".", i);
        Sleep(100);
    }
#else
    while (vote)
    {
		log_info("vote = %d", vote);
        Sleep(500);
    }

	log_info("stoping......\n");
    for (int i = 0; i < 60; i++)
    {
        //printf("%d\n", i);
        printf(">", i);
        Sleep(50);
    }
    printf("\n");
#endif
    Sleep(1000);
    //exit(0);
    thread_running = false;
}

int core_control_update(void* arg)
{
    //if (NULL != arg)
    //	printf("CCS: core_control_update(%d)\n", *(unsigned int*)arg);

    return 0;
}

void* core_control_thread_main(void* arg) {
	log_info("core_control_thread_main running.");

    while (run) {
        core_control_update(arg);
        //printf("core_control_thread_main(%d)\n", *(unsigned int*)arg);
        Sleep(500);
    }

	log_info("core_control_thread_main exit.");

    return NULL;
}

//int _tmain(int argc, _TCHAR* argv[])
//int _tmain(int argc, char *argv[])
int main(int argc, char *argv[])
{
    pthread_t pid;

	log_set_level(0);

/*
log_trace("Hello %s", "world");
log_debug("Hello %s", "world");
log_info("Hello %s", "world");
log_warn("Hello %s", "world");
log_error("Hello %s", "world");
log_fatal("Hello %s", "world");
*/


#if defined(WIN32) || defined(__CYGWIN__)
    if (argc == 2){
        if (!strcmp(argv[1], "run")){
			log_info("calling service_run.");
            core_service_run();
            return 0;
        }
        else if (!strcmp(argv[1], "install")){
			log_info("calling service_install.");
            core_service_install();
            return 0;
        }
        else if (!strcmp(argv[1], "uninstall")){
			log_info("calling service_uninstall.");
            core_service_uninstall();
            return 0;
        }
    }
#endif

    //pthread_mutex_t mutex;

    ghMutex = CreateMutex(
        NULL,              // default security attributes
        FALSE,             // initially not owned
        "ctc_control");    // unnamed mutex

    DWORD error = GetLastError();

    if (error == ERROR_ALREADY_EXISTS)
    {
		log_info("ctc control is already running, exit.");
        CloseHandle(ghMutex);

        return 0;
    }

	log_info("SetConsoleCtrlHandler.");

    if (!SetConsoleCtrlHandler((PHANDLER_ROUTINE)console_ctrl_handler, TRUE))
    {
        // unable to install handler... 
        // display message to the user
		log_trace("unable to install handler!");
        return (-1);
    }

    OutputDebugString("call mqtt_open.");
    core_mqtt_open();

    OutputDebugString("call core_python_open.");
    core_python_open();
    core_python_close();

    OutputDebugString("call core_modbus_open.");
    core_modbus_open();
	drv_servo_sde_open();

    OutputDebugString("call core_serialport_open.");
    core_serialport_open();

    OutputDebugString("call core_scanner_open.");
    core_scanner_open(NULL);

    unsigned int arg = 0;

    pthread_create(&pid, NULL, &core_control_thread_main, (void *)&arg);

	log_info("ctc control is running.");

    /* now initialized */
    thread_running = true;

    while (thread_running) {
        OutputDebugString("ctc control is sleeping.");
        arg++;
        Sleep(1000);
    }

    core_mqtt_close();

    OutputDebugString("ctc control are going to exit.");

    return 0;
}

/*
example code
*/
#if 0
DWORD WINAPI commander_thread_main(LPVOID lpParam)
{
    printf("CCS: commander_thread_main\n");

    //mosquitto_subscribe(mosq, NULL, "control/msg1", 0);

    while (!thread_should_exit) {
        //printf("commander_thread_main running...\n");
        //printf("mosquitto_publish\n");
        //mosquitto_publish(mosq, NULL, "control/msg2", strlen("hello ruby!!!"), "hello ruby!!!", 0, true);
        Sleep(5000);
        //mosquitto_loop(mosq, -1, 1);
    }

    //Sleep(3000);
    thread_running = false;

    return 0;
}

HANDLE                g_ServiceStopEvent = INVALID_HANDLE_VALUE;

DWORD WINAPI ServiceWorkerThread(LPVOID lpParam)
{
    OutputDebugString(_T("CCS: ServiceWorkerThread: Entry"));

    //  Periodically check if the service has been requested to stop
    while (WaitForSingleObject(g_ServiceStopEvent, 0) != WAIT_OBJECT_0)
    {
        /*
        * Perform main service function here
        */

        //  Simulate some work by sleeping
        Sleep(3000);
    }

    OutputDebugString(_T("CCS: ServiceWorkerThread: Exit"));

    return ERROR_SUCCESS;
}
#endif

#if 0
/* control group 1 */
CreateThread(
    NULL,                   // default security attributes
    0,                      // use default stack size  
    commander_thread_main,  // thread function name
    NULL,                   // argument to thread function 
    0,                      // use default creation flags 
    NULL);                  // returns the thread identifier 
#endif
