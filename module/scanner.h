/**
* @file core_scanner.h
*
* core_scanner
*
*   core_scanner.
*
* @author Wesley <lin.wesley@iac.com.tw>
*/

#pragma once

#define SCANNER_READ 0x0
#define SCANNER_TRIGGER 0x1

typedef struct scan_data {
	int length;
	uint8_t content[50];
} Scan_Data;

int core_scanner_open(void* handle);
int core_scanner_close(void* handle);
int core_scanner_ioctl(void* handle, int cmd, unsigned long arg);

