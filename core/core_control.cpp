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
    //if (event == CTRL_C_EVENT)
	//	log_warn("Ctrl-C handler\n"); // do cleanup

    //OutputDebugString("console_ctrl_handler.");
	//log_warn("console_ctrl_handler.");
    run = 0;

    //Sleep(5000);

#if 1
	printf("\n");
    for (int i = 0; i < 20; i++)
    {
        //printf("%d\n", i);
        printf(".", i);
        Sleep(20);
    }
#else
    while (vote)
    {
		//log_info("vote = %d", vote);
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

/** Print the value os a json object or array.
* @param json The handler of the json object or array. */
static void dump(json_t const* json) {

	jsonType_t const type = json_getType(json);
	if (type != JSON_OBJ && type != JSON_ARRAY) {
		puts("error");
		return;
	}

	printf("%s", type == JSON_OBJ ? " {" : " [");

	json_t const* child;
	for (child = json_getChild(json); child != 0; child = json_getSibling(child)) {

		jsonType_t propertyType = json_getType(child);
		char const* name = json_getName(child);
		if (name) printf(" \"%s\": ", name);

		if (propertyType == JSON_OBJ || propertyType == JSON_ARRAY)
			dump(child);

		else {
			char const* value = json_getValue(child);
			if (value) {
				bool const text = JSON_TEXT == json_getType(child);
				char const* fmt = text ? " \"%s\"" : " %s";
				printf(fmt, value);
				bool const last = !json_getSibling(child);
				if (!last) putchar(',');
			}
		}
	}

	printf("%s \n\n", type == JSON_OBJ ? " }" : " ]");

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

	strcpy_s(command, "commander start");
	rsh_command(command);


	//log_info("ctc control is running.");

    /* now initialized */
    thread_running = true;

	char str[] = "{\n"
		"\t\"firstName\": \"Bidhan\",\n"
		"\t\"lastName\": \"Chatterjee\",\n"
		"\t\"age\": 40,\n"
		"\t\"address\": {\n"
		"\t\t\"streetAddress\": \"144 J B Hazra Road\",\n"
		"\t\t\"city\": \"Burdwan\",\n"
		"\t\t\"state\": \"Paschimbanga\",\n"
		"\t\t\"postalCode\": \"713102\"\n"
		"\t},\n"
		"\t\"phoneList\": [\n"
		"\t\t{ \"type\": \"personal\", \"number\": \"09832209761\" },\n"
		"\t\t{ \"type\": \"fax\", \"number\": \"91-342-2567692\" }\n"
		"\t]\n"
		"}\n";
	puts(str);
	json_t mem[32];
	json_t const* json = json_create(str, mem, sizeof mem / sizeof *mem);
	if (!json) {
		puts("Error json create.");
		return EXIT_FAILURE;
	}
	puts("Print JSON:");
	dump(json);

	Sleep(100);

    while (thread_running) {
		// Load config files, if any.

		// Run command loop.
		rsh_loop();

		// Perform any shutdown/cleanup.
    }

	strcpy_s(command, "core_commander_main stop");
	rsh_command(command);
	log_info("ctc control are going to exit.");
	Sleep(3000);

    return 0;
}
