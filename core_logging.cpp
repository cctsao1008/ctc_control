/**
* @file core_logging.cpp
*
* core_logging
*
*   core_logging.
*
* @author Ricardo <tsao.ricardo@iac.com.tw>
*/

#include <windows.h>

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#ifndef WIN32
#include <syslog.h>
#endif
#include <time.h>

#include "core_logging.h"

int log_vprintf(int priority, const char *fmt, va_list va)
{
	time_t now = time(NULL);
	static time_t last_flush = 0;

	if (priority){
		switch (priority){
			case LOG_DEBUG:
				break;
			case LOG_ERR:
				break;
			case LOG_WARNING:
				break;
			//case LOG_NOTICE:
			//	break;
			case LOG_INFO:
				break;

			default:
				break;
		}
	}
	return ERR_SUCCESS;
}

void core_log_printf(int level, const char *fmt, ...)
{
	va_list va;

	va_start(va, fmt);
	log_vprintf(level, fmt, va);
	va_end(va);
}
