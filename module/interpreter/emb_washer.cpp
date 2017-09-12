/**********************************************
 * @file emb_washer.cpp                	      *
 *                                            *
 * Python Washer Function   				  *
 *                                            *
 * @author Joshua <jan.joshua@iac.com.tw>     *
 **********************************************/

#include "core_common.h"
#include "interpreter_common.h"

#if defined(USING_WASH)

// Washer Control
PyObject* set_wm_ma(PyObject *self, PyObject *args) 
{
	double degree = 0.0;
	int direction = 0;
	printf("set_wm_ma \n");
	if (!PyArg_ParseTuple(args, "di", &degree, &direction)) {
		return Py_BuildValue("s", "[ERROR] Washer Move Arm Failed");
	}

	std::string command = "rs485p2 WM MA ";
	command += NumberToString(degree) + " " + NumberToString(direction);

	printf("%s\n", command.c_str());
	rsh_command(command.c_str());

	return Py_BuildValue("s", "[INFO] Washer Move Arm Finished");
}

PyObject* set_wm_sh(PyObject *self, PyObject *args) 
{
	double degree = 0.0;
	int time = 0;
	printf("set_wm_sh \n");
	if (!PyArg_ParseTuple(args, "di", &degree, &time)) {
		return Py_BuildValue("s", "[ERROR] Washer Shake Recieve Wrong Parameters");
	}
	
	std::string command = "rs485p2 WM SH ";
	command += NumberToString(degree) + " " + NumberToString(time);

	printf("%s\n", command.c_str());
	rsh_command(command.c_str());

	/*
	RS485Port p2 = RS485Port("COM6");
	Washer wm = Washer(&p2);
	printf("%p\n", washer);
	if (!wm.shakeMachine(degree, time))
	{
		return Py_BuildValue("s", "[ERROR] Washer Shake Failed");
	}
	*/
	return Py_BuildValue("s", "[INFO] Washer Shake Finished");
}

PyObject* set_wm_rg(PyObject *self, PyObject *args) 
{
	double degree = 0.0;
	int direction = 0;
	printf("set_wm_rg \n");
	if (!PyArg_ParseTuple(args, "di", &degree, &direction)) {
		return Py_BuildValue("s", "[ERROR] Washer Rotate Gripper Failed");
	}

	std::string command = "rs485p2 WM RG ";
	command += NumberToString(degree) + " " + NumberToString(direction);

	printf("%s\n", command.c_str());
	rsh_command(command.c_str());

	return Py_BuildValue("s", "[INFO] Washer Rotate Gripper Finished");
}

#endif