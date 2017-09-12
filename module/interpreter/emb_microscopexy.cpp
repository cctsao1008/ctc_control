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
PyObject* set_mxy_mv(PyObject *self, PyObject *args) 
{
	double x = 0.0;
	double y = 0.0;
	//printf("set_mxy_mv \n");
	if (!PyArg_ParseTuple(args, "dd", &x, &y)) {
		return Py_BuildValue("s", "[ERROR] Microscope XY Table Move Failed");
	}
	
	std::string command = "rs485p2 MXY MV ";
	command += NumberToString(x) + " " + NumberToString(y);

	//printf("%s\n", command.c_str());
	rsh_command(command.c_str());

	/*
	RS485Port p2 = RS485Port("COM12");
	MicroscopeXY mxy = MicroscopeXY(&p2);
	if (!mxy.move2Pos(x, y))
	{
		return Py_BuildValue("s", "[ERROR] Microscope XY Table Move Failed");
	}
	*/
	return Py_BuildValue("s", "[INFO] Microscope XY Table Move Finished");
}

#endif
