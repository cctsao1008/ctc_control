/**
* @file mqtt.h
*
* mqtt
*
*   mqtt.
*
* @author Ricardo <tsao.ricardo@iac.com.tw>
*/

#pragma once

#include "core_common.h"

#define TOPIC_PYC_CMD "CONTROL/PYC_CMD"
#define TOPIC_RSH_CMD "CONTROL/RSH_CMD"

int rsh_mqtt_main(int argc, char *argv[]);
uint8_t mqtt_publish(const char *topic, int payloadlen, const void *payload);
