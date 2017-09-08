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
#define USING_CENT
#define USING_SYRI
#define USING_MICR
#define USING_WASH

#if defined(USING_MP1)
/* Motion Platform 1 APIs*/

static PyObject*
mp1_movl(PyObject *self, PyObject *args) {
	double x, y;

	if (!PyArg_ParseTuple(args, "dd", &x, &y)) {
		return NULL;
	}

	// cal set_mp1_movl

	printf("%s (x = %3.2f, y = %3.2f)\n", __FUNCTION__, x, y);
	return Py_BuildValue("");
}

static PyObject*
mp1_movj(PyObject *self, PyObject *args) {
	double j1, j2;

	if (!PyArg_ParseTuple(args, "dd", &j1, &j2)) {
		return NULL;
	}
	// cal set_mp1_movj

	printf("%s (x = %3.2f, y = %3.2f)\n", __FUNCTION__, j1, j2);
	return Py_BuildValue("");
}

static PyObject*
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

static PyObject*
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

#if defined(USING_CENT)
/* relative centrifugal force */
#define DEFAULT_RPM_MIN    0
#define DEFAULT_RPM_MAX    6000

#define DEFAULT_RCF_MIN    0
#define DEFAULT_RCF_MAX    10000

/* centrifugal control */
static PyObject*
set_cen_rpm(PyObject *self, PyObject *args) {
	double rpm;

	if (!PyArg_ParseTuple(args, "d", &rpm)) {
		return NULL;
	}

	pd.cent.rpm = rpm;

	printf("%s (rpm = %f)\n", __FUNCTION__, rpm);

	return Py_BuildValue("");
}

static PyObject*
set_cen_rcf(PyObject *self, PyObject *args) {
	double rcf;

	if (!PyArg_ParseTuple(args, "d", &rcf)) {
		return NULL;
	}

	if ((rcf > DEFAULT_RCF_MIN) && (rcf < DEFAULT_RCF_MAX)) {
		printf("%s (rcf = %f)\n", __FUNCTION__, rcf);
		pd.cent.rcf = rcf;
	}
	else {
		printf("%s invalid rcf value! %f", __FUNCTION__, rcf);
	}
 

	return Py_BuildValue("");
}

static PyObject*
set_cen_temp(PyObject *self, PyObject *args) {
	double temp;

	if (!PyArg_ParseTuple(args, "d", &temp)) {
		return NULL;
	}

	printf("%s (temp = %f)\n", __FUNCTION__, temp);

	pd.cent.temp = temp;

	return Py_BuildValue("");
}

static PyObject*
set_cen_duration(PyObject *self, PyObject *args) {
	double duration;


	if (!PyArg_ParseTuple(args, "d", &duration)) {
		return NULL;
	}

	printf("%s (duration = %f)\n", __FUNCTION__, duration);

	pd.cent.duration = duration;

	return Py_BuildValue("");
}

static PyObject*
set_cen_ctrl(PyObject *self, PyObject *args) {
	double on;

	if (!PyArg_ParseTuple(args, "d", &on)) {
		return NULL;
	}

	if (on > 0)
	{
		printf("%s (on = %f)\n", __FUNCTION__, on);

		pd.cent.on = 1.0f;
	}
	else
		pd.cent.on = 0.0f;

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

// Create a method list to give to our module
static PyMethodDef EmbMethods[] = {
	{ "testfunction", emb_testfunction, METH_VARARGS, "Multiply args." },
	{ "stringfunc", emb_stringfunc, METH_VARARGS, "C print a python str." },
	{ "func1", emb_func1, METH_VARARGS, "Test function." },
	/* motion platform 1 control apis */
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
