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
	return Py_BuildValue("s", "[INFO] Washer Move Arm Finished");
}

PyObject* set_wm_sh(PyObject *self, PyObject *args) 
{
	double degree = 0.0;
	int time = 0;
	printf("set_wm_sh \n");
	if (!PyArg_ParseTuple(args, "di", &degree, &time)) {
		return Py_BuildValue("s", "[ERROR] Washer Shake Failed");
	}
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
	return Py_BuildValue("s", "[INFO] Washer Rotate Gripper Finished");
}

#endif