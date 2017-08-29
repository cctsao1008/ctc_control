/**
* @file core_python.cpp
*
* core_python
*
*   core_python.
*
* @author Ricardo <tsao.ricardo@iac.com.tw>
*/

#ifdef _DEBUG
#undef _DEBUG
#include <python.h>
#define _DEBUG
#else
#include <python.h>
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
    { NULL, NULL, 0, NULL }
};

// Create a python module
static PyModuleDef EmbModule = {
    PyModuleDef_HEAD_INIT, "emb", NULL, -1, EmbMethods,
    NULL, NULL, NULL, NULL
};

static PyObject*
PyInit_emb(void) {
    return PyModule_Create(&EmbModule);
}
/* END - C API */

int core_python_open(void)
{
    PyObject *pName, *pModule;

    wchar_t *program = Py_DecodeLocale("ctc_control", NULL);

    if (program == NULL) {
        return 0;
    }

    Py_SetProgramName(program);

    PyImport_AppendInittab("emb", &PyInit_emb);

    Py_Initialize();

    PyRun_SimpleString(
        "from time import time\n"
        "the_time = time()\n"
        "print(f'Time1 is {the_time}')\n"
        );

    PyRun_SimpleString(
        "import sys\n"
        "sys.path.append('./')\n"
        );

    pName = PyUnicode_FromString("startup");
    pModule = PyImport_Import(pName);

    if (pModule != NULL) {
        printf("CCS: PyImport_Import ok...\n");
    }
    else {
        PyErr_Print();
        fprintf(stderr, "Failed to load module - %s\n", "startup");
        return 1;
    }

    Py_DECREF(pModule);

    PyMem_RawFree(program);

    return 0;
}

int core_python_close(void)
{
	if (Py_FinalizeEx() < 0)
		return (-1);

		return 0;
}

int rsh_core_python_main(int argc, char *argv[])
{
    PyObject *pName, *pModule;

    wchar_t *program = Py_DecodeLocale("ctc_python", NULL);

    if (program == NULL) {
        return 0;
    }

    Py_SetProgramName(program);

    PyImport_AppendInittab("emb", &PyInit_emb);

    Py_Initialize();

    PyRun_SimpleString(
        "from time import time\n"
        "the_time = time()\n"
        "print(f'Time1 is {the_time}')\n"
        );

    PyRun_SimpleString(
        "import sys\n"
        "sys.path.append('./')\n"
        );

    pName = PyUnicode_FromString("scripts.startup");
    pModule = PyImport_Import(pName);

    if (pModule != NULL) {
        printf("CCS: PyImport_Import ok...\n");
    }
    else {
        PyErr_Print();
        fprintf(stderr, "Failed to load module - %s\n", "startup");
        return 1;
    }

    Py_DECREF(pModule);

    PyMem_RawFree(program);

	Py_FinalizeEx();

    return 0;
}

