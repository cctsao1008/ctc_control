/**
* @file emb_gripper.cpp
*
* emb_gripper
*
*   emb_gripper.
*
* @author Ricardo <tsao.ricardo@iac.com.tw>
*/

#include "core_common.h"
#include "interpreter_common.h"

#if defined(USING_GRIP)
/* relative centrifugal force */
#define DEFAULT_RPM_MIN    0
#define DEFAULT_RPM_MAX    6000

#define DEFAULT_RCF_MIN    0
#define DEFAULT_RCF_MAX    10000

/* gripper control */
PyObject*
set_2f_grip_on(PyObject *self, PyObject *args) {
	double id;

	if (!PyArg_ParseTuple(args, "d", &id)) {
		return NULL;
	}

	//so.cent.rpm = rpm;

	printf("%s (%f)\n", __FUNCTION__, id);

	return Py_BuildValue("");
}

PyObject*
set_2f_grip_off(PyObject *self, PyObject *args) {
	double id;

	if (!PyArg_ParseTuple(args, "d", &id)) {
		return NULL;
	}

	//so.cent.rpm = rpm;

	printf("%s (%f)\n", __FUNCTION__, id);

	return Py_BuildValue("");
}
#endif
