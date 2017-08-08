/**
* @file core_scanner.cpp
*
* core_scanner
*
*   core_scanner.
*
* @author Ricardo <tsao.ricardo@iac.com.tw>
*/

#include "core_common.h"

static pthread_t pid;

extern int run;
extern int vote;

void* core_scanner_thread_main(void* arg)
{
    while (run)
    {
        Sleep(1000);
    }

    return 0;
}

int core_scanner_open(void* handle)
{
    pthread_create(&pid, NULL, &core_scanner_thread_main, NULL);
    return 0;
}

int core_scanner_ioctl(void* handle, int cmd, unsigned long arg)
{
    if (handle == NULL)
        return (-1);

    switch(cmd)
    {
        case 0x0:

        break;
        
        case 0x1 :

        break;

        default :

        break;
    }
    return 0;
}

