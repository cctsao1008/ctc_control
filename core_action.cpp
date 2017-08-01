/**
* @file core_action.cpp
*
* core_action
*
*   core_action.
*
* @author Ricardo <tsao.ricardo@iac.com.tw>
*/

#include "core_common.h"

extern int run;
extern int vote;

void* core_action_thread_main(void* arg)
{
	printf("CCS: core_action_thread_main running...\n");
	vote++;

	while (run)
	{
		// put your code here.
		Sleep(5000);
	}

	printf("CCS: core_action_thread_main exit.\n");
	vote--;

	return 0;
}

int action_pippet_control(void* data)
{
	// put your code here.
}

int action_platform_motion(void* data)
{
	// put your code here.
}

int action_tool_control(void* data)
{
	// put your code here.
}
