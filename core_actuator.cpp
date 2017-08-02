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

int control_motor(void* data)
{
	// call driver.
	// motion platform
	// centrifuge
	// syringe pump
	// washer
}

int control_dio(void* data)
{
	// call driver.
	// magnetic switch
	// led
	// buzzer
	// buttom
}

int control_ac(void* data)
{
	// call driver.
}

int control_griper(void* data)
{
	// call driver.
	// 2finger
	// 3finger
}
