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
        printf("Ctrl-C handled\n"); // do cleanup

    OutputDebugString("CCS: console_ctrl_handler.");
    printf("CCS: console_ctrl_handler.\n");
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
        printf("vote = %d\n", vote);
        Sleep(200);
    }

    printf("stoping.\n");
    for (int i = 0; i < 30; i++)
    {
        //printf("%d\n", i);
        printf(".", i);
        Sleep(100);
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
    printf("CCS: core_control_thread_main running...\n");

    while (run) {
        core_control_update(arg);
        //printf("core_control_thread_main(%d)\n", *(unsigned int*)arg);
        Sleep(500);
    }

    printf("CCS: core_control_thread_main exit.\n");

    return NULL;
}

//int _tmain(int argc, _TCHAR* argv[])
//int _tmain(int argc, char *argv[])
int main(int argc, char *argv[])
{
    pthread_t pid;

#if defined(WIN32) || defined(__CYGWIN__)
    if (argc == 2){
        if (!strcmp(argv[1], "run")){
            printf("CCS: calling service_run...\n");
            core_service_run();
            return 0;
        }
        else if (!strcmp(argv[1], "install")){
            printf("CCS: calling service_install...\n");
            core_service_install();
            return 0;
        }
        else if (!strcmp(argv[1], "uninstall")){
            printf("CCS: calling service_uninstall...\n");
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
        printf("CCS: ctc control is already running, exit...\n");
        CloseHandle(ghMutex);

        return 0;
    }

    printf("CCS: SetConsoleCtrlHandler\n");

    if (!SetConsoleCtrlHandler((PHANDLER_ROUTINE)console_ctrl_handler, TRUE))
    {
        // unable to install handler... 
        // display message to the user
        printf("CCS: Unable to install handler!\n");
        return (-1);
    }

    OutputDebugString("CCS: call mqtt_open...");
    core_mqtt_open();

    OutputDebugString("CCS: call core_python_open...");
    core_python_open();
    core_python_close();

    OutputDebugString("CCS: call core_modbus_open...");
    core_modbus_open();

    OutputDebugString("CCS: call core_serialport_open...");
    core_serialport_open();

    OutputDebugString("CCS: call core_scanner_open...");
    core_scanner_open(NULL);

    unsigned int arg = 0;

    pthread_create(&pid, NULL, &core_control_thread_main, (void *)&arg);

    printf("CCS: ctc control is running.\n");

    /* now initialized */
    thread_running = true;

    while (thread_running) {
        OutputDebugString("CCS: ctc control is sleeping...");
        arg++;
        Sleep(1000);
    }

    core_mqtt_close();

    OutputDebugString("CCS: ctc control are going to exit...");

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
