/**********************************************
 * @file emb_microscopexy.cpp          	      *
 *                                            *
 * Python Microscope XY Table Function		  *
 *                                            *
 * @author Joshua <jan.joshua@iac.com.tw>     *
 **********************************************/

#include "core_common.h"
#include "interpreter_common.h"

#if defined(USING_MICR)

// Microscope XY Control
PyObject* set_mps_wait(PyObject *self, PyObject *args)
{
	while (get_mspMutex())
	{
		Sleep(200);
	}

	return Py_BuildValue("s", "[INFO] Microscope XY Table Wait Finished");
}

PyObject* set_mps_mov(PyObject *self, PyObject *args) 
{
	double x = 0.0;
	double y = 0.0;

	if (!PyArg_ParseTuple(args, "dd", &x, &y)) {
		return Py_BuildValue("s", "[ERROR] Microscope XY Table Move Failed");
	}
	
	std::string command = "rs485p2 mps mov ";
	command += NumberToString(x) + " " + NumberToString(y);

	while (get_mspMutex())
	{
		Sleep(200);
	}

	printf("command %s excuting", command);

	//printf("%s\n", command.c_str());
	rsh_command(command.c_str());

	return Py_BuildValue("s", "[INFO] Microscope XY Table Move Finished");
}

#endif
