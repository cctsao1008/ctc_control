/**
* @file core_scanner.h
*
* core_scanner
*
*   core_scanner.
*
* @author Ricardo <tsao.ricardo@iac.com.tw>
*/

#pragma once

#define SCANNER_READ 0x0
#define SCANNER_WRITE 0x1


int core_scanner_open(void* handle);
int core_scanner_close(void* handle);

