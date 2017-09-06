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
