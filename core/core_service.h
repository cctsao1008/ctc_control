/**
* @file core_service.h
*
* core_service
*
*   core_service.
*
* @author Ricardo <tsao.ricardo@iac.com.tw>
*/

#pragma once
/* ============================================================
* Window service related functions
* ============================================================ */
#if defined(WIN32) || defined(__CYGWIN__)
void core_service_install(void);
void core_service_uninstall(void);
void core_service_run(void);
#endif