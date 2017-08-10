
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
#include "stdio.h"
#include <map>
#include <time.h>
#include <math.h>
#include <vector>
#include <sstream>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "c_serial.h"
#include "log.h"
#include "pthread.h"

// core functions
#include "core_service.h"
#include "core_mqtt.h"
#include "core_python.h"
#include "core_actuator.h"
#include "core_action.h"
#include "core_diagnostic.h"
#include "core_modbus.h"
#include "core_serialport.h"
#include "core_scanner.h"

// drviers
#include "drv_servo_sde.h"
