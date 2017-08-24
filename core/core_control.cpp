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

int run = 1;
int vote = 0;

/* flags */
static volatile bool thread_should_exit = false;    /**< daemon exit flag */
static volatile bool thread_running = false;        /**< daemon status flag */

HANDLE ghMutex;

void WINAPI console_ctrl_handler(DWORD event)
{
    if (event == CTRL_C_EVENT)
		log_warn("Ctrl-C handled\n"); // do cleanup

    OutputDebugString("console_ctrl_handler.");
	log_warn("console_ctrl_handler.");
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

	log_warn("ctc_control is going to stop......\n");
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

int main(int argc, char *argv[])
{
	char command[64] = {0};

	log_set_level(0);

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
		log_error("ctc control is already running.");
        CloseHandle(ghMutex);

        return 0;
    }

    if (!SetConsoleCtrlHandler((PHANDLER_ROUTINE)console_ctrl_handler, TRUE))
    {
        // unable to install handler... 
        // display message to the user
		log_error("unable to install handler.");
        return (-1);
    }

	strcpy_s(command, "core_commander_main start");
	rsh_command(command);


	//log_info("ctc control is running.");

    /* now initialized */
    thread_running = true;

	Sleep(1000);

    while (thread_running) {
		// Load config files, if any.

		// Run command loop.
		rsh_loop();

		// Perform any shutdown/cleanup.
    }

	log_info("ctc control are going to exit.");

    return 0;
}
