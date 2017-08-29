
/**
* @file core_common.h
*
* core_common
*
*   core_common.
*
* @author Ricardo <tsao.ricardo@iac.com.tw>
*/

#include <windows.h>
#include <Mmsystem.h>
#pragma comment(lib, "Winmm.lib")


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

// core functions
#include "core_service.h"
#include "core_rsh.h"
//#include "core_mqtt.h"
#include "core_python.h"
#include "core_actuator.h"
#include "core_commander.h"
#include "core_diagnostic.h"
//#include "core_modbus.h"
#include "core_serialport.h"
#include "core_scanner.h"
#include "core_rsh.h"
#include "core_example.h"
#include "core_system.h"

#include "rs485p1.h"
//#include "driver_rs485_p2.h"
