/**
* @file emb_motion_platform_main.cpp
*
* emb_motion_platform_main
*
*   emb_motion_platform_main.
*
* @author Ricardo <tsao.ricardo@iac.com.tw>
*/

#include "core_common.h"
#include "interpreter_common.h"

#if defined(USING_MP1)
/* Motion Platform 1 APIs*/

PyObject*
mp1_movl(PyObject *self, PyObject *args) {
	double x, y;

	if (!PyArg_ParseTuple(args, "dd", &x, &y)) {
		return NULL;
	}

	// cal set_mp1_movl

	printf("%s (x = %3.2f, y = %3.2f)\n", __FUNCTION__, x, y);
	return Py_BuildValue("");
}

PyObject*
mp1_movj(PyObject *self, PyObject *args) {
	double j1, j2;

	if (!PyArg_ParseTuple(args, "dd", &j1, &j2)) {
		return NULL;
	}
	// cal set_mp1_movj

	printf("%s (x = %3.2f, y = %3.2f)\n", __FUNCTION__, j1, j2);
	return Py_BuildValue("");
}

PyObject*
set_mp1_pos(PyObject *self, PyObject *args) {
	int mask;
	double x, y, z1, z2, z3;

	if (!PyArg_ParseTuple(args, "dddddi", &x, &y, &z1, &z2, &z3, &mask)) {
		return NULL;
	}

	if ((mask & 0x01) == 0x01)
	{

	}
		

	if ((mask & 0x02) == 0x02)
	{

	}

	if ((mask & 0x04) == 0x04)
	{

	}

	if ((mask & 0x08) == 0x08)
	{

	}

	if ((mask & 0x10) == 0x10)
	{
		
	}

	printf("%s (x = %4.2f, y = %4.2f, z1 = %4.2f, z2 = %4.2f, z3 = %4.2f, mask =0x%02X)\n",
		__FUNCTION__, x, y, z1, z2, z3, mask);

	// cal set_mp1_movl

	//printf("emb_mp1_movl(%3.2f, %3.2f)\n", x, y);
	return Py_BuildValue("");
	//Py_RETURN_NONE;
}

PyObject*
get_mp1_pos(PyObject *self, PyObject *args) {
	double x, y;

	printf("get_mp1_pos\n");

	if (!PyArg_ParseTuple(args, "dd", &x, &y)) {
		return NULL;
	}

	// cal set_mp1_movl

	printf("%s (x = %3.2f, y = %3.2f)\n", __FUNCTION__, x, y);
	return Py_BuildValue("");
}
#endif
