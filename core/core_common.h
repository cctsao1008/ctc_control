
/**
* @file core_common.h
*
* core_common
*
*   core_common.
*
* @author Ricardo <tsao.ricardo@iac.com.tw>
*/

#include <Winsock2.h>
#include <windows.h>
#include <Mmsystem.h>
#pragma comment(lib, "Winmm.lib")
#include <Winsock2.h>


#include <map>
#include <time.h>
#include <math.h>
#include <vector>
#include <sstream>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
//#include <iostream>
#include "c_serial.h"
#include "log.h"
#include "pthread.h"
#include "modbus.h"
#include "getopt.h"
#include "mosquitto.h"
#include "tiny-json.h"
#include "lcmlite.h"
#include "mqtt.h"
#include "config.h"

#ifdef _DEBUG
#undef _DEBUG
#include <python.h>
#define _DEBUG
#else
#include <python.h>
#endif

// core functions
#include "core_system.h"
#include "core_service.h"
#include "core_rsh.h"
//#include "core_mqtt.h"
//#include "core_python.h"
#include "core_actuator.h"
#include "core_commander.h"
//#include "core_diagnostic.h"
//#include "core_modbus.h"
//#include "core_serialport.h"
#include "core_rsh.h"
#include "core_mariadb.h"
#include "core_memorywriter.h"

#include "washer.h"
#include "syringepump.h"
#include "microscope_xy.h"
#include "scanner.h"
#include "rs485p1.h"
#include "rs485p2.h"
#include "bianneng.h"
#include "interpreter.h"
#include "example.h"

#pragma warning(disable:4996)
