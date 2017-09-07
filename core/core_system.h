/**
* @file core_system.h
*
* core_system
*
*   core_system.
*
* @author Ricardo <tsao.ricardo@iac.com.tw>
*/

#pragma once

#include "core_common.h"

uint64_t micros(void);

typedef struct _mp
{
	double sys_elaps;

	/* motion platform */
	double pitch[2];

	double vel_sv[2];
	double pos_sv[2];

	double vel_pv[2];
	double pos_pv[2];

	double pos_queue[2];
} mp;

typedef struct _platform_data
{
	double sys_elaps;

	/* motion platform 1 */
	mp mp1;
} platform_data;
