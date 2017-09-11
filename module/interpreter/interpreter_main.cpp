/**
* @file interpreter.cpp
*
* interpreter
*
*   interpreter.
*
* @author Ricardo <tsao.ricardo@iac.com.tw>
*/

#include "core_common.h"

extern PyObject* PyInit_emb(void);

static pthread_t pid;

void* interpreter_thread_main(void* arg)
{
	PyObject *pName, *pModule;

	wchar_t *program = Py_DecodeLocale("ctc_python", NULL);

	if (program == NULL) {
		return 0;
	}

	Py_SetProgramName(program);

	PyImport_AppendInittab("emb", &PyInit_emb);

	Py_Initialize();

#if 0
	PyRun_SimpleString(
		"from time import time\n"
		"the_time = time()\n"
		"print(f'Time1 is {the_time}')\n"
		);

	PyRun_SimpleString(
		"import sys\n"
		"sys.path.append('./')\n"
		);
#endif

	//strcpy_s(command, "argv[1]");
	char scritp_file_name[128];
	//printf("%s \n", (char*)arg);
	//sprintf(scritp_file_name, "scripts.%s", (char*)arg);
	sprintf(scritp_file_name, "ctc-startup");
	//return 0;
	//pName = PyUnicode_FromString(scritp_file_name);
	pName = PyUnicode_FromString("scripts.ctc-startup");
	pModule = PyImport_Import(pName);

	if (pModule != NULL) {
		printf("PyImport_Import ok...\n");
	}
	else {
		PyErr_Print();
		fprintf(stderr, "Failed to load module - %s\n", scritp_file_name);
		return 0;
	}

	PyObject* pFunction = PyObject_GetAttrString(pModule, (char*)"main");

	//PyObject_CallObject(pFunction, NULL);
	PyObject_CallFunction(pFunction, NULL);

	Py_DECREF(pModule);

	PyMem_RawFree(program);

	Py_FinalizeEx();

	return 0;
}

void* thread_main(void* arg)
{
	PyObject *pName, *pModule;

	wchar_t *program = Py_DecodeLocale("ctc_python", NULL);

	if (program == NULL) {
		return 0;
	}

	Py_SetProgramName(program);

	PyImport_AppendInittab("emb", &PyInit_emb);

	Py_Initialize();

#if 0
	PyRun_SimpleString(
		"from time import time\n"
		"the_time = time()\n"
		"print(f'Time1 is {the_time}')\n"
		);

	PyRun_SimpleString(
		"import sys\n"
		"sys.path.append('./')\n"
		);
#endif

	//printf("%s \n", (char *)arg);
	//strcpy_s(command, "argv[1]");
	char scritp_file_name[128];
	//printf("%s \n", (char*)arg);
	//sprintf(scritp_file_name, "scripts.%s", (char*)arg);
	//sprintf(scritp_file_name, "ctc-startup");
	sprintf(scritp_file_name, (char *)arg);
	//return 0;
	pName = PyUnicode_FromString(scritp_file_name);
	//pName = PyUnicode_FromString("scripts.ctc-startup");
	pModule = PyImport_Import(pName);

	if (pModule != NULL) {
		printf("PyImport_Import ok...\n");
	}
	else {
		PyErr_Print();
		fprintf(stderr, "Failed to load module - %s\n", scritp_file_name);
		return 0;
	}

	//PyObject * pFunc = PyObject_GetAttrString(pModule, "main");
	//PyEval_CallObject(pFunc, NULL);

	Py_DECREF(pModule);

	PyMem_RawFree(program);

	Py_FinalizeEx();
}

static char file[256] = { 0 };

int rsh_interpreter_main(int argc, char *argv[])
{
	if (argc < 2) {
		log_info("missing command");
		return 0;
	}

	memcpy(file, argv[1], strlen(argv[1]));

	pthread_create(&pid, NULL, &thread_main, file);

	return 0;
}
