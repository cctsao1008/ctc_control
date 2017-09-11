/**********************************************
 * @file emb_syringepump.cpp           	      *
 *                                            *
 * Python Syringe Pump function 			  *
 *                                            *
 * @author Joshua <jan.joshua@iac.com.tw>     *
 **********************************************/

#include "core_common.h"
#include "interpreter_common.h"

#if defined(USING_SYRI)

// Syringe Pump Control
PyObject* set_sp_ab(PyObject *self, PyObject *args)
{
	double amount = 0.0;
	printf("set_sp_ab \n");
	if (!PyArg_ParseTuple(args, "d", &amount)) {
		return Py_BuildValue("s", "[ERROR] Syringe Pump Absrob Failed");
	}
	return Py_BuildValue("s", "[INFO] Syringe Pump Absrob Finished");
}

PyObject* set_sp_dr(PyObject *self, PyObject *args) 
{
	double amount = 0.0;
	printf("set_sp_dr \n");
	if (!PyArg_ParseTuple(args, "d", &amount)) {
		return Py_BuildValue("s", "[ERROR] Syringe Pump Drain Failed");
	}
	return Py_BuildValue("s", "[INFO] Syringe Pump Drain Finished");
}

PyObject* set_sp_pip(PyObject *self, PyObject *args) 
{
	double amount = 0.0;
	printf("set_sp_pip \n");
	if (!PyArg_ParseTuple(args, "d", &amount)) {
		return Py_BuildValue("s", "[ERROR] Syringe Pump Pipetting Failed");
	}
	return Py_BuildValue("s", "[INFO] Syringe Pump Pipetting Finished");
}

#endif