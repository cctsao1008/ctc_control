/**
* @file emb_guideway.cpp
*
* emb_guideway
*
*   emb_guideway.
*
* @author Ricardo <tsao.ricardo@iac.com.tw>
*/

#include "core_common.h"
#include "interpreter_common.h"

PyObject*
set_lgw_pos(PyObject *self, PyObject *args) {
	double val;

	if (!PyArg_ParseTuple(args, "d", &val)) {
		return NULL;
	}

	so.lgw.pos_x = val;

	printf("%s (%f)\n", __FUNCTION__, val);

	return Py_BuildValue("");
}

PyObject*
set_lgw_rpm(PyObject *self, PyObject *args) {
	double val;

	if (!PyArg_ParseTuple(args, "d", &val)) {
		return NULL;
	}

	so.lgw.rpm = val;

	printf("%s (%f)\n", __FUNCTION__, val);

	return Py_BuildValue("");
}

PyObject*
set_lgw_ramp(PyObject *self, PyObject *args) {
	double val;

	if (!PyArg_ParseTuple(args, "d", &val)) {
		return NULL;
	}

	printf("%s (%f)\n", __FUNCTION__, val);

	return Py_BuildValue("");
}

PyObject*
set_lgw_pulse(PyObject *self, PyObject *args) {
	double val;

	if (!PyArg_ParseTuple(args, "d", &val)) {
		return NULL;
	}

	printf("%s (%f)\n", __FUNCTION__, val);

	return Py_BuildValue("");
}
