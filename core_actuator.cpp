/**
* @file core_actuator.cpp
*
* core_actuator
*
*   core_actuator.
*
* @author Ricardo <tsao.ricardo@iac.com.tw>
*/

#include "core_common.h"

extern int run;
extern int vote;

void* core_actuator_thread_main(void* arg)
{
	printf("CCS: core_actuator_thread_main running...\n");
	vote++;

	while (run)
	{
		// put your code here.
		Sleep(5000);
	}

	printf("CCS: core_actuator_thread_main exit.\n");
	vote--;

	return 0;
}

int control_syringe_pump(void* data)
{
	// call driver.
}

int control_x_axis(void* data)
{
	// call driver.
}

int control_y_axis(void* data)
{
	// call driver.
}

int control_z_axis(void* data)
{
	// call driver.
}

int control_magnetic_switch(void* data)
{
	// call driver.
}

int control_ac_temperature(void* data)
{
	// call driver.
}

int control_griper_2finger(void* data)
{
	// call driver.
}

int control_griper_3finger(void* data)
{
	// call driver.
}

