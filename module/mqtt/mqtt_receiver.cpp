/**
* @file mqtt_receiver.cpp
*
* mqtt_receiver
*
*   mqtt_receiver.
*
* @author Ricardo <tsao.ricardo@iac.com.tw>
*/

#include "core_common.h"
#include "interpreter.h"

uint8_t mqtt_receiver(const char* topic, const char* cmd)
{
	if (strcmp(topic, TOPIC_PYC_CMD) == 0)
	{
		printf("exec python command.\n");
		pyc_command(cmd);
	}
	else if (strcmp(topic, TOPIC_RSH_CMD) == 0)
	{
		printf("exec rsh command.\n");
		rsh_command(cmd);
	}
	else /* default: */
	{
		printf("invalid topic (%s)\n", topic);
	}

	return 0;
}
