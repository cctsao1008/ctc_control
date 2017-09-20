/**
* @file interpreter_common.h
*
* interpreter_common
*
*   interpreter_common.
*
* @author Ricardo <tsao.ricardo@iac.com.tw>
*/

#pragma once

#define USING_MP1
#define USING_CENT
#define USING_SYRI
#define USING_MPS
#define USING_WASH
#define USING_DB
#define USING_LGW

PyObject* mp1_movj(PyObject *self, PyObject *args);
PyObject* mp1_movl(PyObject *self, PyObject *args);

PyObject* set_mp1_pos(PyObject *self, PyObject *args);
PyObject* set_cen_rpm(PyObject *self, PyObject *args);
PyObject* set_cen_rcf(PyObject *self, PyObject *args);
PyObject* set_cen_temp(PyObject *self, PyObject *args);
PyObject* set_cen_ctrl(PyObject *self, PyObject *args);
PyObject* set_cen_duration(PyObject *self, PyObject *args);

// Joshua Add Start

// Syringe Pump Control
PyObject* set_sp_ab(PyObject *self, PyObject *args);
PyObject* set_sp_dr(PyObject *self, PyObject *args);
PyObject* set_sp_pip(PyObject *self, PyObject *args);
PyObject* set_sp_wait(PyObject *self, PyObject *args);

// Washer Control
PyObject* set_wm_ma(PyObject *self, PyObject *args);
PyObject* set_wm_sh(PyObject *self, PyObject *args);
PyObject* set_wm_rg(PyObject *self, PyObject *args);

// Microscope XY Control
PyObject* set_mps_wait(PyObject *self, PyObject *args);
PyObject* set_mps_mov(PyObject *self, PyObject *args);


// MariaDB
PyObject* set_db_query(PyObject *self, PyObject *args);

// Joshua Add Finished

/* Linear Guideway Control API */
PyObject* set_lgw_pos(PyObject *self, PyObject *args);
PyObject* set_lgw_rpm(PyObject *self, PyObject *args);
PyObject* set_lgw_ramp(PyObject *self, PyObject *args);
PyObject* set_lgw_pulse(PyObject *self, PyObject *args);
