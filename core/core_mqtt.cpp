/**
* @file core_mqtt.cpp
*
* core_mqtt
*
*   core_mqtt.
*
* @author Ricardo <tsao.ricardo@iac.com.tw>
*/

#include "core_common.h"
#include "mosquitto.h"

DWORD WINAPI core_mqtt_loop_thread(LPVOID lpParam);
DWORD WINAPI core_mqtt_pub(LPVOID lpParam);

void core_mqtt_sub(void);

extern int run;

static struct mosquitto *mosq;

static unsigned long count = 0;

int core_mqtt_open(void)
{
    mosquitto_lib_init();
    mosq = mosquitto_new("ctc_control", true, NULL);

    OutputDebugString("CCS: connecting to mqtt broker...");

    //if (mosquitto_connect(mosq, "localhost", 1883, 60 == MOSQ_ERR_SUCCESS) {
    if (mosquitto_connect_async(mosq, "localhost", 1883, 60) == MOSQ_ERR_SUCCESS) {
        mosquitto_loop_start(mosq);

#if 0
        core_mqtt_sub();

        CreateThread(
            NULL,                   // default security attributes
            0,                      // use default stack size  
            core_mqtt_pub,      // thread function name
            NULL,                   // argument to thread function 
            0,                      // use default creation flags 
            NULL);
#endif

        OutputDebugString("CCS: mqtt broker connected...");
    }
    else {
        OutputDebugString("CCS: mqtt broker is not running...");
    }

    return 0;
}

void core_mqtt_close(void)
{
    mosquitto_disconnect(mosq);

    // call after mosquitto_disconnect.
    mosquitto_loop_stop(mosq, false);
    mosquitto_lib_cleanup();
}

void on_subscribe(struct mosquitto *mosq, void *obj, int mid, int qos_count, const int *granted_qos)
{
    OutputDebugString("CCS: on_subscribe.");
    //printf("CCS: subscribe_callback\n");
    //mosquitto_disconnect(mosq);
}

void on_publish(struct mosquitto *mosq, void *userdata, int mid)
{
    OutputDebugString("CCS: on_publish.");
}

void on_message(struct mosquitto *, void *, const struct mosquitto_message * msg)
{
    OutputDebugString("CCS: on_message.");

    char* data = (char*)(msg->payload);

    if (strcmp(msg->topic, "control/cmd") == 0)
    {
        //for (int i = 0; i < (msg->payloadlen); i++)
        //printf("%X ", *(char*)(msg->payload));

        for (int i = 0; i < (msg->payloadlen); i++)
            printf("%X ", data[i]);
    }
    else if (strcmp(msg->topic, "xxx") == 0)
    {
        // do something else
    }
    /* more else if clauses */
    else /* default: */
    {
        printf("CCS: [%05d] topic = %s, payload = %s\n", count++, msg->topic, (char*)msg->payload);
    }
    
}

void core_mqtt_sub(void)
{
    // subscribe callback
    mosquitto_subscribe_callback_set(mosq, on_subscribe);

    // publish callback
    mosquitto_publish_callback_set(mosq, on_publish);

    // message callback
    mosquitto_message_callback_set(mosq, on_message);

    mosquitto_subscribe(mosq, NULL, "control/msg1", 0);
    mosquitto_subscribe(mosq, NULL, "control/msg2", 0);
    mosquitto_subscribe(mosq, NULL, "control/msg3", 0);
    mosquitto_subscribe(mosq, NULL, "control/cmd", 0);

    // button message
    mosquitto_subscribe(mosq, NULL, "control/btn", 0);
}

//void mqtt_pub(void)
DWORD WINAPI core_mqtt_pub(LPVOID lpParam)
{
    clock_t timestamp;   /* Line 8 */
    //char buf[32];
    char str[32];

    SYSTEMTIME st;
    GetLocalTime(&st);

    while (1) {
        timestamp = GetTickCount();

        sprintf_s(str, "CCS : time %d", timestamp);
        //printf("hello %s", str);
        //_ltoa(timestamp, (char *)&buf, 10);   // here 2 means binary
        //mosquitto_publish(mosq, NULL, "control/server/timestamp", sizeof(clock_t), (void *)&timestamp, 0, true);
        mosquitto_publish(mosq, NULL, "control/server/timestamp", sizeof(str), str, 0, true);
        OutputDebugString(str);
        Sleep(2000);
    }
}

DWORD WINAPI core_mqtt_loop_thread(LPVOID lpParam)
{
    int cycle_time = 100; // 100ms
    //while (!thread_should_exit) {
    while (run) {
        //OutputDebugString("CTC Control Service: core_mqtt_loop_thread...");
        mosquitto_loop(mosq, -1, 1);
        Sleep(cycle_time);
    }

    return 0;
}

/*
    example code
*/
#if 0
STARTUPINFO si;
PROCESS_INFORMATION pi;

ZeroMemory(&si, sizeof(si));
si.cb = sizeof(si);
ZeroMemory(&pi, sizeof(pi));

LPTSTR cWinDir = new TCHAR[MAX_PATH];
GetCurrentDirectory(MAX_PATH, cWinDir);

//printf("%s\n", cWinDir);
//cout << &cWinDir;

//LPTSTR sConLin = wcscat(cWinDir, L"\\..\\Debug\\mosquitto.exe -p 1234");
LPTSTR sConLin = wcscat(cWinDir, L"\\mosquitto.exe -v");
//printf("%s\n", sConLin);

if (!CreateProcess(NULL,   // No module name (use command line)
    sConLin,        // Command line
    NULL,           // Process handle not inheritable
    NULL,           // Thread handle not inheritable
    FALSE,          // Set handle inheritance to FALSE
    0,              // No creation flags
    NULL,           // Use parent's environment block
    NULL,           // Use parent's starting directory 
    &si,            // Pointer to STARTUPINFO structure
    &pi)           // Pointer to PROCESS_INFORMATION structure
    )
{
    printf("CCS: CreateProcess failed (%d).\n", GetLastError());
    return 0;
}

Sleep(200);
#endif

#if 0
try {
    //...
}
catch (...) {
    //...
}
#endif

#if 0
CreateThread(
    NULL,                   // default security attributes
    0,                      // use default stack size  
    mqtt_loop_thread,       // thread function name
    NULL,                   // argument to thread function 
    0,                      // use default creation flags 
    NULL);                  // returns the thread identifier 
#endif
