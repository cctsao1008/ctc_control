/**
* @file rs485p1.h
*
* rs485p1
*
*   rs485p1.
*
* @author Ricardo <tsao.ricardo@iac.com.tw>
*/

#pragma once

#include "core_common.h"

int rsh_rs485p1_main(int argc, char *argv[]);

typedef struct _rs485p1_data {
	/* temp */
	uint16_t tc_pv;
	uint16_t tc_sv;
} rs485p1_data;
