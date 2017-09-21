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
//#define USING_SYRI
//#define USING_MICR
//#define USING_WASH


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
	{ "set_sp_wait", set_sp_wait, METH_VARARGS, "Syringe Pump Wait function." },
#endif
	/* Linear Guideway Control API */
#if defined(USING_LGW)
	{ "set_lgw_pos", set_lgw_pos, METH_VARARGS, "set_gw_pos" },
	{ "set_lgw_rpm", set_lgw_rpm, METH_VARARGS, "set_lgw_rpm" },
	{ "set_lgw_ramp", set_lgw_ramp, METH_VARARGS, "set_lgw_ramp" },
	{ "set_lgw_pulse", set_lgw_pulse, METH_VARARGS, "set_lgw_pulse" },
#endif
	/* Microscope XY Table Control API */
#if defined(USING_MPS)
	{ "set_mps_pos", set_mps_pos, METH_VARARGS, "Microscope XY Table Move to Position function." },
	{ "set_mps_mov", set_mps_mov, METH_VARARGS, "Microscope XY Table Move the vector function." },
	{ "set_mps_wait", set_mps_wait, METH_VARARGS, "Microscope XY Table Wait Move function." },
	{ "set_mps_init", set_mps_init, METH_VARARGS, "Microscope XY Table Init function." },
#endif
	/* Microscope XY Table Control API */
#if defined(USING_WASH)
	{ "set_wm_ma", set_wm_ma, METH_VARARGS, "Washer Move Arm function." },
	{ "set_wm_sh", set_wm_sh, METH_VARARGS, "Washer Shake function." },
	{ "set_wm_rg", set_wm_rg, METH_VARARGS, "Washer Rotate Gripper function." },
#endif
	/* MariaDB API */
#if defined(USING_DB)
	{ "set_db_query", set_db_query, METH_VARARGS, "MariaDB Query function." },
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

uint8_t pyc_command(const char *cmd)
{
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

	//PyRun_SimpleString(
	//	"from time import time\n"
	//	"the_time = time()\n"
	//	"print(f'Time1 is {the_time}')\n"
	//	);

	PyRun_SimpleString(
		"import emb\n"
		);

	PyRun_SimpleString(
		cmd
		);

	//PyRun_SimpleString(
	//	"import sys\n"
	//	"sys.path.append('./')\n"
	//	);

	PyMem_RawFree(program);

	Py_FinalizeEx();

	return 0;
}
