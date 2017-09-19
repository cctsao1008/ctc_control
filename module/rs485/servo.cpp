/**
* @file mqtt-bridge.cpp
*
* mqtt-bridge
*
*   mqtt-bridge.
*
* @author Ricardo <tsao.ricardo@iac.com.tw>
*/

#include "core_common.h"
#include "servo.h"

uint16_t servo_sync(modbus_t *ctx, uint8_t id)
{
	uint16_t data = 0xFFFF;

	modbus_set_slave(ctx, id);
	modbus_read_registers(ctx, 0x0900, 1, &data);

	return data;
}

uint16_t loop_servo_1(modbus_t *ctx)
{
	return 0;
}

uint16_t loop_servo_2(modbus_t *ctx)
{
	return 0;
}

uint16_t loop_servo_3(modbus_t *ctx)
{
	return 0;
}

uint16_t loop_servo_4(modbus_t *ctx)
{
	uint16_t data;

	data = servo_sync(ctx, SERVO_4);

	//printf("s4 : data = 0x%X\n", data);

	return data;
}