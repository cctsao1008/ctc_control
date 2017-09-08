/**
* @file core_system.cpp
*
* core_system
*
*   core_system.
*
* @author Ricardo <tsao.ricardo@iac.com.tw>
*/

#include "core_common.h"

platform_data pd;

uint64_t micros(void) {
	LARGE_INTEGER freq, tick;

	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&tick);

	return (tick.QuadPart * 1000000 / freq.QuadPart);
}

#if 0
int rsh_core_python_main(int argc, char *argv[])
{
	PyObject *pName, *pModule;

	wchar_t *program = Py_DecodeLocale("ctc_python", NULL);

	if (program == NULL) {
		return 0;
	}

	Py_SetProgramName(program);

	PyImport_AppendInittab("emb", &PyInit_emb);

	Py_Initialize();

	PyRun_SimpleString(
		"from time import time\n"
		"the_time = time()\n"
		"print(f'Time1 is {the_time}')\n"
		);

	PyRun_SimpleString(
		"import sys\n"
		"sys.path.append('./')\n"
		);

	//strcpy_s(command, "argv[1]");
	char scritp_file_name[128];
	sprintf(scritp_file_name, "scripts.%s", argv[1]);
	pName = PyUnicode_FromString(scritp_file_name);
	pModule = PyImport_Import(pName);

	if (pModule != NULL) {
		printf("PyImport_Import ok...\n");
	}
	else {
		PyErr_Print();
		fprintf(stderr, "Failed to load module - %s\n", scritp_file_name);
		return 1;
	}

	Py_DECREF(pModule);

	PyMem_RawFree(program);

	Py_FinalizeEx();

	return 0;
}
#endif
