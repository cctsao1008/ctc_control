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

int handle_scanner;

void* core_scanner_thread_main(void* arg)
{
    while (run)
    {
        // put your code here...
        Sleep(1000);
    }

    return 0;
}

int core_scanner_open(void* handle)
{
    // put your code here...
    handle = &handle_scanner;
    
    pthread_create(&pid, NULL, &core_scanner_thread_main, NULL);
    return 0;
}

int core_scanner_close(void* handle)
{
    return 0;
}

int core_scanner_ioctl(void* handle, int cmd, unsigned long arg)
{
    if (handle == NULL)
        return (-1);

    switch(cmd)
    {
        case SCANNER_READ :
            if(arg == 0)
            // put your code here...
            if(arg == 1)
            // put your code here...
            if(arg == 2)
            // put your code here...

        break;
        
        case SCANNER_WRITE :
            // put your code here...

        break;

        default :
            // put your code here...

        break;
    }
    return 0;
}

