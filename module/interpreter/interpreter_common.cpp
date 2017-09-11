/**
* @file interpreter_common.cpp
*
* interpreter_common
*
*   interpreter_common.
*
* @author Ricardo <tsao.ricardo@iac.com.tw>
*/

#include "core_common.h"
#include "interpreter_common.h"

//#define USING_MP1
//#define USING_CENT
#define USING_SYRI
#define USING_MICR
#define USING_WASH

// Joshua Add
// Syringe Pump Control
static PyObject*
set_sp_ab(PyObject *self, PyObject *args)
{
	double amount = 0.0;
	printf("set_sp_ab \n");
	if (!PyArg_ParseTuple(args, "d", &amount)) {
		return Py_BuildValue("s", "[ERROR] Syringe Pump Absrob Failed");
	}
	return Py_BuildValue("s", "[INFO] Syringe Pump Absrob Finished");
}

static PyObject*
set_sp_dr(PyObject *self, PyObject *args) {
	double amount = 0.0;
	printf("set_sp_dr \n");
	if (!PyArg_ParseTuple(args, "d", &amount)) {
		return Py_BuildValue("s", "[ERROR] Syringe Pump Drain Failed");
	}
	return Py_BuildValue("s", "[INFO] Syringe Pump Drain Finished");
}


static PyObject*
set_sp_pip(PyObject *self, PyObject *args) {
	double amount = 0.0;
	printf("set_sp_pip \n");
	if (!PyArg_ParseTuple(args, "d", &amount)) {
		return Py_BuildValue("s", "[ERROR] Syringe Pump Pipetting Failed");
	}
	return Py_BuildValue("s", "[INFO] Syringe Pump Pipetting Finished");
}

// Microscope XY Control
static PyObject*
set_mxy_mv(PyObject *self, PyObject *args) {
	double x = 0.0;
	double y = 0.0;
	printf("set_mxy_mv \n");
	if (!PyArg_ParseTuple(args, "dd", &x, &y)) {
		return Py_BuildValue("s", "[ERROR] Microscope XY Table Move Failed");
	}
	initRS485P2para();
	microscopexy->move2Pos(x, y);
	return Py_BuildValue("s", "[INFO] Microscope XY Table Move Finished");
}

// Washer Control
static PyObject*
set_wm_ma(PyObject *self, PyObject *args) {
	double degree = 0.0;
	int direction = 0;
	printf("set_wm_ma \n");
	if (!PyArg_ParseTuple(args, "di", &degree, &direction)) {
		return Py_BuildValue("s", "[ERROR] Washer Move Arm Failed");
	}
	return Py_BuildValue("s", "[INFO] Washer Move Arm Finished");
}

static PyObject*
set_wm_sh(PyObject *self, PyObject *args) {
	double degree = 0.0;
	int time = 0;
	printf("set_wm_sh \n");
	if (!PyArg_ParseTuple(args, "di", &degree, &time)) {
		return Py_BuildValue("s", "[ERROR] Washer Shake Failed");
	}
	return Py_BuildValue("s", "[INFO] Washer Shake Finished");
}

static PyObject*
set_wm_rg(PyObject *self, PyObject *args) {
	double degree = 0.0;
	int direction = 0;
	printf("set_wm_rg \n");
	if (!PyArg_ParseTuple(args, "di", &degree, &direction)) {
		return Py_BuildValue("s", "[ERROR] Washer Rotate Gripper Failed");
	}
	return Py_BuildValue("s", "[INFO] Washer Rotate Gripper Finished");
}

// Joshua Add End

/* BEGIN - C API to present to our Python instance */
// Create a method list to give to our module
static PyMethodDef EmbMethods[] = {
#if defined(USING_MP1)
	{ "mp1_movl", mp1_movl, METH_VARARGS, "mp1_movl" },
	{ "mp1_movj", mp1_movj, METH_VARARGS, "mp1_movj" },
	{ "set_mp1_pos", set_mp1_pos, METH_VARARGS, "set_mp1_pos" },
#endif
	/* centrifugal  control apis */
#if defined(USING_CENT)
	{ "set_cen_rpm", set_cen_rpm, METH_VARARGS, "set_cen_rpm" },
	{ "set_cen_rcf", set_cen_rcf, METH_VARARGS, "set_cen_rcf" },
	{ "set_cen_temp", set_cen_temp, METH_VARARGS, "set_cen_temp" },
	{ "set_cen_duration", set_cen_duration, METH_VARARGS, "set_cen_duration" },
	{ "set_cen_ctrl", set_cen_ctrl, METH_VARARGS, "set_cen_ctrl" },
#endif
	/* Syringe Pump Control API */
#if defined(USING_SYRI)
	{ "set_sp_ab", set_sp_ab, METH_VARARGS, "Syringe Pump Absorb function." },
	{ "set_sp_dr", set_sp_dr, METH_VARARGS, "Syringe Pump Drain function." },
	{ "set_sp_pip", set_sp_pip, METH_VARARGS, "Syringe Pump Pipetting function." },
#endif
	/* Microscope XY Table Control API */
#if defined(USING_MICR)
	{ "set_mxy_mv", set_mxy_mv, METH_VARARGS, "Microscope XY Table Move to Position function." },
#endif
	/* Microscope XY Table Control API */
#if defined(USING_WASH)
	{ "set_wm_ma", set_wm_ma, METH_VARARGS, "Washer Move Arm function." },
	{ "set_wm_sh", set_wm_sh, METH_VARARGS, "Washer Shake function." },
	{ "set_wm_rg", set_wm_rg, METH_VARARGS, "Washer Rotate Gripper function." },
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
