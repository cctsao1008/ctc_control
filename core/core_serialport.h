/**
* @file core_serialport.h
*
* core_serialport
*
*   core_serialport.
*
* @author Ricardo <tsao.ricardo@iac.com.tw>
*/

#pragma once

int core_serialport_open(void);
void* core_serialport_thread_main(void* arg);
