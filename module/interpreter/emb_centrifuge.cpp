/**
* @file emb_centrifuge.cpp
*
* emb_centrifuge
*
*   emb_centrifuge.
*
* @author Ricardo <tsao.ricardo@iac.com.tw>
*/

#include "core_common.h"
#include "interpreter_common.h"

#if defined(USING_CENT)
/* relative centrifugal force */
#define DEFAULT_RPM_MIN    0
#define DEFAULT_RPM_MAX    6000

#define DEFAULT_RCF_MIN    0
#define DEFAULT_RCF_MAX    10000

/* centrifugal control */
PyObject*
set_cen_rpm(PyObject *self, PyObject *args) {
	double rpm;

	if (!PyArg_ParseTuple(args, "d", &rpm)) {
		return NULL;
	}

	so.cent.rpm = rpm;

	printf("%s (rpm = %f)\n", __FUNCTION__, rpm);

	return Py_BuildValue("");
}

PyObject*
set_cen_rcf(PyObject *self, PyObject *args) {
	double rcf;

	if (!PyArg_ParseTuple(args, "d", &rcf)) {
		return NULL;
	}

	if ((rcf > DEFAULT_RCF_MIN) && (rcf < DEFAULT_RCF_MAX)) {
		printf("%s (rcf = %f)\n", __FUNCTION__, rcf);
		so.cent.rcf = rcf;
	}
	else {
		printf("%s invalid rcf value! %f", __FUNCTION__, rcf);
	}
 

	return Py_BuildValue("");
}

PyObject*
set_cen_temp(PyObject *self, PyObject *args) {
	double temp;

	if (!PyArg_ParseTuple(args, "d", &temp)) {
		return NULL;
	}

	printf("%s (temp = %f)\n", __FUNCTION__, temp);

	so.cent.temp = temp;

	return Py_BuildValue("");
}

PyObject*
set_cen_duration(PyObject *self, PyObject *args) {
	double duration;


	if (!PyArg_ParseTuple(args, "d", &duration)) {
		return NULL;
	}

	printf("%s (duration = %f)\n", __FUNCTION__, duration);

	so.cent.duration = duration;

	return Py_BuildValue("");
}

PyObject*
set_cen_ctrl(PyObject *self, PyObject *args) {
	double on;

	if (!PyArg_ParseTuple(args, "d", &on)) {
		return NULL;
	}

	if (on > 0)
	{
		printf("%s (on = %f)\n", __FUNCTION__, on);

		so.cent.on = 1.0f;
	}
	else
		so.cent.on = 0.0f;

	return Py_BuildValue("");
}
#endif
