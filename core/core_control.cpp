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

BOOL WINAPI console_ctrl_handler(DWORD fdwCtrlType)
{
	switch (fdwCtrlType)
	{ 
		// Handle the CTRL-C signal. 
	case CTRL_C_EVENT: 
		printf( "Ctrl-C event\n\n" );
		Beep( 750, 300 ); 
		Sleep(5000);
		for (int i = 0; i < 20; i++)
		{
			//printf("%d\n", i);
			printf(".", i);
			Sleep(20);
		}
		return( TRUE );

		// CTRL-CLOSE: confirm that the user wants to exit. 
	case CTRL_CLOSE_EVENT: 
		Beep( 600, 200 ); 
		printf( "Ctrl-Close event\n\n" );
		Sleep(5000);
		return( TRUE ); 

		// Pass other signals to the next handler. 
	case CTRL_BREAK_EVENT: 
		Beep( 900, 200 ); 
		printf( "Ctrl-Break event\n\n" );
		Sleep(5000);
		return FALSE; 

	case CTRL_LOGOFF_EVENT: 
		Beep( 1000, 200 ); 
		printf( "Ctrl-Logoff event\n\n" );
		Sleep(5000);
		return FALSE; 

	case CTRL_SHUTDOWN_EVENT: 
		Beep( 750, 500 ); 
		printf( "Ctrl-Shutdown event\n\n" );
		Sleep(5000);
		return FALSE; 

	default: 
		Sleep(5000);
		return FALSE; 
	} 
#if 0
    if (event == CTRL_C_EVENT)
		log_warn("Ctrl-C handler\n"); // do cleanup


    //OutputDebugString("console_ctrl_handler.");
	//log_warn("console_ctrl_handler.");

	printf("console_ctrl_handler\n");
	//return;

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
#endif
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

int load_param(const char * filename, char * content)
{
	pd.cent.rpm = 0.0f;
	pd.cent.rcf = 0.0f;
	pd.cent.duration = 0.0f;
	pd.cent.on = 0.0f;
	pd.cent.temp = 25.0f;
    
#if 0
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
#endif

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
	//puts(str);



	FILE *pf;
	long fsize;
	char * buffer;
	size_t result;

	//f = fopen("D:\\vs2013\\ctc_project\\ctc_control_gitlab\\Debug\\ctc-config.json", "r");
	pf = fopen("Debug\\ctc-config.json", "r");

	if (pf) {
		printf("passed\n");

		// obtain file size:
		fseek(pf, 0, SEEK_END);
		fsize = ftell(pf);
		rewind(pf);

		printf("fsize = %d\n", fsize);

		// allocate memory to contain the whole file:
		buffer = (char*)malloc(sizeof(char)*fsize);
		//if (buffer == NULL) { fputs("Memory error", stderr); exit(2); }
		if (buffer == NULL) { fputs("Memory error\n", stderr); }

		// copy the file into the buffer:
		result = fread(buffer, 1, fsize, pf);
		printf("result = %d\n\n", result);
		//puts(buffer);
		//if (result != lSize) { fputs("Reading error", stderr); exit(3); }
		if (result != fsize) { fputs("Reading error\n", stderr); }

		/* the whole file is now loaded in the memory buffer. */

		// terminate
		fclose(pf);
		//free(buffer);
	}
	else {
		printf("error : file not found (Debug\\ctc-config.json)\n");
		//fclose(pf);
		return 0;
	}

	json_t mem[32];
	//json_t const* json = json_create(str, mem, sizeof mem / sizeof *mem);
	json_t const* json = json_create(buffer, mem, sizeof mem / sizeof *mem);
	if (!json) {
		puts("Error json create.");
		//return EXIT_FAILURE;
	}

	json_t const* firstName = json_getProperty(json, "firstName");
	if (!firstName || JSON_TEXT != json_getType(firstName)) {
		puts("Error, the first name property is not found.");
		//return EXIT_FAILURE;
	}
	char const* firstNameVal = json_getValue(firstName);
	printf("Fist Name: %s.\n", firstNameVal);

	char const* lastName = json_getPropertyValue(json, "lastName");
	if (!lastName) {
		puts("Error, the last name property is not found.");
		//return EXIT_FAILURE;
	}
	printf("Last Name: %s.\n", lastName);

	json_t const* age = json_getProperty(json, "age");
	if (!age || JSON_INTEGER != json_getType(age)) {
		puts("Error, the age property is not found.");
		//return EXIT_FAILURE;
	}
	int const ageVal = (int)json_getInteger(age);
	printf("Age: %d.\n", ageVal);

	json_t const* phoneList = json_getProperty(json, "phoneList");
	if (!phoneList || JSON_ARRAY != json_getType(phoneList)) {
		puts("Error, the phone list property is not found.");
		//return EXIT_FAILURE;
	}

	json_t const* phone;
	for (phone = json_getChild(phoneList); phone != 0; phone = json_getSibling(phone)) {
		if (JSON_OBJ == json_getType(phone)) {
			char const* phoneNumber = json_getPropertyValue(phone, "number");
			if (phoneNumber) printf("Number: %s.\n", phoneNumber);
		}
	}

	printf("\n\n");
	//strcpy(content, buffer);
	free(buffer);

	return 0;
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

    //if (!SetConsoleCtrlHandler((PHANDLER_ROUTINE)console_ctrl_handler, TRUE))
	if (!SetConsoleCtrlHandler((PHANDLER_ROUTINE)console_ctrl_handler, TRUE))
    {
        // unable to install handler... 
        // display message to the user
		log_error("unable to install handler.");
        //return (-1);
    }


	//char buffer1[MAX_PATH];
	//char buffer2[MAX_PATH];
	//GetCurrentDirectory(MAX_PATH, buffer1);
	//printf("%s \n", buffer1);

	//strcpy_s(buffer2, "%s\dll", buffer1);
	//sprintf(buffer2, "%s\\dll", buffer1);
	//printf("%s \n", buffer2);
	//SetDllDirectory(buffer2);

	//strcpy_s(command, "commander start");
	//rsh_command(command);
	strcpy_s(command, "mqtt start");
	rsh_command(command);

	//log_info("ctc control is running.");

    /* now initialized */
    thread_running = true;

	load_param(NULL, NULL);

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
