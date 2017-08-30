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

typedef struct _tc {
	/* temp */
	uint16_t pv;
	uint16_t sv;
} tc;

typedef struct _rs485p1 {
	/* temp */
	tc tc;
} rs485p1;
