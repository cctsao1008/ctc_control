/**
* @file core_modbus.h
*
* core_modbus
*
*   core_modbus.
*
* @author Ricardo <tsao.ricardo@iac.com.tw>
*/

#pragma once

int core_modbus_open(void);
void* core_modbus_thread_main(void* arg);
