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
#define USING_MICR
#define USING_WASH

PyObject*
mp1_movj(PyObject *self, PyObject *args);

PyObject*
mp1_movl(PyObject *self, PyObject *args);

PyObject*
set_mp1_pos(PyObject *self, PyObject *args);

PyObject*
set_cen_rpm(PyObject *self, PyObject *args);

PyObject*
set_cen_rcf(PyObject *self, PyObject *args);

PyObject*
set_cen_temp(PyObject *self, PyObject *args);

PyObject*
set_cen_ctrl(PyObject *self, PyObject *args);

PyObject*
set_cen_duration(PyObject *self, PyObject *args);

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

// Joshua Add Finished