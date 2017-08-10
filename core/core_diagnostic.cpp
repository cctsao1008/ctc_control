/**
* @file core_diagnostic.cpp
*
* core_diagnostic
*
*   core_diagnostic.
*
* @author Ricardo <tsao.ricardo@iac.com.tw>
*/

#include "core_common.h"

extern int run;
extern int vote;

void* core_diagnostic_thread_main(void* arg)
{
	printf("CCS: core_diagnostic_thread_main running...\n");
	vote++;

	while (run)
	{
		// put your code here.
		Sleep(1000);
	}

	printf("CCS: core_diagnostic_thread_main exit.\n");
	vote--;

	return 0;
}
