/**
* @file pycfunctions.cpp
*
* pycfunctions
*
*   pycfunctions.
*
* @author Ricardo <tsao.ricardo@iac.com.tw>
*/

#include "core_common.h"

#define USING_MP1

#if defined(USING_MP1)
/* Motion Platform 1 APIs*/

static PyObject*
emb_mp1_movl(PyObject *self, PyObject *args) {
	double x, y;

	if (!PyArg_ParseTuple(args, "dd", &x, &y)) {
		return NULL;
	}

	// cal set_mp1_movl

	printf("emb_mp1_movl(%3.2f, %3.2f)\n", x, y);
	return Py_BuildValue("");
}

static PyObject*
emb_mp1_movj(PyObject *self, PyObject *args) {
	double j1, j2;

	if (!PyArg_ParseTuple(args, "dd", &j1, &j2)) {
		return NULL;
	}
	// cal set_mp1_movj

	printf("emb_mp1_movj(%3.2f, %3.2f)\n", j1, j2);
	return Py_BuildValue("");
}

static PyObject*
emb_set_mp1_pos(PyObject *self, PyObject *args) {
	int mask = 0;
	double x = 0, y = 0, z1 = 0, z2 = 0, z3 = 0;

	if (!PyArg_ParseTuple(args, "dddddi", &x, &y, &z1, &z2, &z3, &mask)) {
		return NULL;
	}

	if ((mask & 0x01) == 0x01)
		printf("set pos.x = %4.2f\n", x);

	if ((mask & 0x02) == 0x02)
		printf("set pos.y = %4.2f\n", y);

	if ((mask & 0x04) == 0x04)
		printf("set pos.z1 = %4.2f\n", z1);

	if ((mask & 0x08) == 0x08)
		printf("set pos.z2 = %4.2f\n", z2);

	if ((mask & 0x10) == 0x10)
		printf("set pos.z3 = %4.2f\n", z3);

	// cal set_mp1_movl

	//printf("emb_mp1_movl(%3.2f, %3.2f)\n", x, y);
	return Py_BuildValue("");
	//Py_RETURN_NONE;
}

static PyObject*
emb_get_mp1_pos(PyObject *self, PyObject *args) {
	double x, y;

	printf("emb_get_mp1_pos\n");

	if (!PyArg_ParseTuple(args, "dd", &x, &y)) {
		return NULL;
	}

	// cal set_mp1_movl

	printf("emb_mp1_movl(%3.2f, %3.2f)\n", x, y);
	return Py_BuildValue("");
}
#endif

/* BEGIN - C API to present to our Python instance */
// Methods
static PyObject*
emb_testfunction(PyObject *self, PyObject *args) {
	double a, b, c;
	if (!PyArg_ParseTuple(args, "ddd", &a, &b, &c)) {
		return NULL;
	}
	return Py_BuildValue("d", a * b * c);
}

static PyObject*
emb_stringfunc(PyObject *self, PyObject *args) {
	char* str_data;
	if (!PyArg_ParseTuple(args, "s", &str_data)) {
		return NULL;
	}
	fprintf(stdout, "[Ricardo] COUT: %s", str_data);
	return Py_BuildValue("");
}

static PyObject*
emb_func1(PyObject *self, PyObject *args) {
	printf("emb_func1 \n");
	return Py_BuildValue("");
}

// Create a method list to give to our module
static PyMethodDef EmbMethods[] = {
	{ "testfunction", emb_testfunction, METH_VARARGS, "Multiply args." },
	{ "stringfunc", emb_stringfunc, METH_VARARGS, "C print a python str." },
	{ "func1", emb_func1, METH_VARARGS, "Test function." },
#if defined(USING_MP1)
	{ "mp1_movl", emb_mp1_movl, METH_VARARGS, "mp1_movl" },
	{ "mp1_movj", emb_mp1_movj, METH_VARARGS, "mp1_movj" },
	{ "set_mp1_pos", emb_set_mp1_pos, METH_VARARGS, "set_mp1_pos" },
#endif
	{ NULL, NULL, 0, NULL }
};

// Create a python module
static PyModuleDef EmbModule = {
	PyModuleDef_HEAD_INIT, "emb", NULL, -1, EmbMethods,
	NULL, NULL, NULL, NULL
};

PyObject*
PyInit_emb(void) {
	return PyModule_Create(&EmbModule);
}
/* END - C API */
