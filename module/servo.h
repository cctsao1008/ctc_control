/**
* @file servo.h
*
* servo
*
*   servo.
*
* @author Ricardo <tsao.ricardo@iac.com.tw>
*/

#pragma once

#include "core_common.h"

/* SERVO  */
enum { SERVO_1 = 1, SERVO_2, SERVO_3, SERVO_4, SERVO_MAX };

uint16_t loop_servo_1(modbus_t *ctx);
uint16_t loop_servo_2(modbus_t *ctx);
uint16_t loop_servo_3(modbus_t *ctx);
uint16_t loop_servo_4(modbus_t *ctx);
