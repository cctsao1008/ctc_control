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
PyObject* set_sp_wait(PyObject *self, PyObject *args)
{
	while (get_pumpMutex())
	{
		Sleep(200);
	}

	return Py_BuildValue("s", "[INFO] Syringe Pump Wait Finished");
}

PyObject* set_sp_init(PyObject *self, PyObject *args)
{
	std::string command = "rs485p2 sp init";

	while (get_pumpMutex())
	{
		Sleep(200);
	}

	//printf("%s\n", command.c_str());
	rsh_command(command.c_str());

	return Py_BuildValue("s", "[INFO] Syringe Pump init Finished");
}

PyObject* set_sp_ab(PyObject *self, PyObject *args)
{
	double amount = 0.0;
	
	if (!PyArg_ParseTuple(args, "d", &amount)) {
		return Py_BuildValue("s", "[ERROR] Syringe Pump Absrob Failed");
	}

	std::string command = "rs485p2 sp ab ";
	command += NumberToString(amount);

	while (get_pumpMutex())
	{
		Sleep(200);
	}

	//printf("%s\n", command.c_str());
	rsh_command(command.c_str());

	return Py_BuildValue("s", "[INFO] Syringe Pump Absrob Finished");
}

PyObject* set_sp_dr(PyObject *self, PyObject *args) 
{
	double amount = 0.0;
	
	if (!PyArg_ParseTuple(args, "d", &amount)) {
		return Py_BuildValue("s", "[ERROR] Syringe Pump Drain Failed");
	}

	std::string command = "rs485p2 sp dr ";
	command += NumberToString(amount);

	while (get_pumpMutex())
	{
		Sleep(200);
	}

	//printf("%s\n", command.c_str());
	rsh_command(command.c_str());

	return Py_BuildValue("s", "[INFO] Syringe Pump Drain Finished");
}

PyObject* set_sp_pip(PyObject *self, PyObject *args) 
{
	double amount = 0.0;
	unsigned int time = 0;

	if (!PyArg_ParseTuple(args, "di", &amount, &time)) {
		return Py_BuildValue("s", "[ERROR] Syringe Pump Pipetting Failed");
	}

	std::string command = "rs485p2 sp pip ";
	command += NumberToString(amount) + " " + NumberToString(time);

	while (get_pumpMutex())
	{
		Sleep(200);
	}

	//printf("%s\n", command.c_str());
	rsh_command(command.c_str());

	return Py_BuildValue("s", "[INFO] Syringe Pump Pipetting Finished");
}

#endif