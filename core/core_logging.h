/**
* @file core_logging.h
*
* core_logging
*
*   core_logging.
*
* @author Ricardo <tsao.ricardo@iac.com.tw>
*/

#pragma once

/* Log types */
#define LOG_NONE 0x00
#define LOG_INFO 0x01
#define LOG_WARNING 0x04
#define LOG_ERR 0x08
#define LOG_DEBUG 0x10
#define LOG_ALL 0xFFFF

/* Error values */
enum err_t {
	ERR_CONN_PENDING = -1,
	ERR_SUCCESS = 0,
	ERR_NOMEM = 1,
	ERR_PROTOCOL = 2,
	ERR_INVAL = 3,
	ERR_NO_CONN = 4,
	ERR_CONN_REFUSED = 5,
	ERR_NOT_FOUND = 6,
	ERR_CONN_LOST = 7,
	ERR_TLS = 8,
	ERR_PAYLOAD_SIZE = 9,
	ERR_NOT_SUPPORTED = 10,
	ERR_AUTH = 11,
	ERR_ACL_DENIED = 12,
	ERR_UNKNOWN = 13,
	ERR_ERRNO = 14,
	ERR_EAI = 15,
	ERR_PROXY = 16
};

