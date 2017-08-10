/**
* @file core_modbus.cpp
*
* core_modbus
*
*   core_modbus.
*
* @author Ricardo <tsao.ricardo@iac.com.tw>
*/

#include "pthread.h"
#include "modbus.h"
#include "stdio.h"
//#include <windows.h>

#include "core_common.h"
//#include "modbus.h"

#define MODBUS_SERIAL_DEV           "COM2"
#define MODBUS_SERIAL_BAUDRATE      9600    /* 9600, 38400, 115200, ... */
#define MODBUS_SERIAL_PARITY        'N'     /* 'N', 'E', or 'O' */
#define MODBUS_SERIAL_DATABITS      8       /* 5, 6, 7, or 8 */
#define MODBUS_SERIAL_STOPBITS      1       /* 1 or 2 */
#define MODBUS_DEVICE_ID            1
#define MODBUS_TIMEOUT_SEC          3
#define MODBUS_TIMEOUT_USEC         0
#define MODBUS_DEBUG                ON
#define MODBUS_RO_BITS              32
#define MODBUS_RW_BITS              32
#define MODBUS_RO_REGISTERS         64
#define MODBUS_RW_REGISTERS         64

void* core_modbus_thread_main(void* arg);

static modbus_t            *ctx;
static modbus_mapping_t    *data_mapping;
static struct timeval      timeout;
static int                 ret, go = 1;
static uint8_t             query[MODBUS_TCP_MAX_ADU_LENGTH];

static pthread_t pid;

extern int run;
extern int vote;

int core_modbus_open(void)
{
	log_info("core_modbus_open.");

    ctx = modbus_new_rtu(MODBUS_SERIAL_DEV,
        MODBUS_SERIAL_BAUDRATE,
        MODBUS_SERIAL_PARITY,
        MODBUS_SERIAL_DATABITS,
        MODBUS_SERIAL_STOPBITS);

    if (ctx == NULL) {
		log_error("Unable to create the libmodbus context.");
        exit(-1);
    }

    /* set slave device ID */
    modbus_set_slave(ctx, MODBUS_DEVICE_ID);

    /* Debug mode */
    //modbus_set_debug(ctx, MODBUS_DEBUG);

    data_mapping = modbus_mapping_new(MODBUS_RO_BITS, MODBUS_RW_BITS,
        MODBUS_RO_REGISTERS, MODBUS_RW_REGISTERS);

    if (data_mapping == NULL) {
		log_error("Failed to allocate the mapping: %s", modbus_strerror(errno));
        modbus_free(ctx);
        return -1;
    }

    pthread_create(&pid, NULL, &core_modbus_thread_main, NULL);

    /* FIXME: initiate all data here */

    return 0;
}

int core_modbus_close(void)
{
    return 0;
}

void* core_modbus_update(void* arg)
{
    return 0;
}

void* core_modbus_thread_main(void* arg)
{
    uint16_t data[64];

    /* open serial interface */
    modbus_connect(ctx);

	log_info("modbus connected.");

	vote++;

    while (run) {
#if 0
        ret = modbus_receive(ctx, query);
        if (ret >= 0) {
            /* IF SOME ERROR OCCOR */
            /* modbus_reply_exception(ctx, query, MODBUS_EXCEPTION_XXXX); */
            /* ELSE */
            modbus_reply(ctx, query, ret, data_mapping);
        }
        else {
            /* Connection closed by the client or server */
            break;
        }
#endif

        modbus_read_registers(ctx, 1, 10, data);
        Sleep(2000);
    }

	vote--;

    //printf("CCS : core_modbus_thread_main exit. %s\n", modbus_strerror(errno));
	log_info("core_modbus_thread_main exit.");

    modbus_mapping_free(data_mapping);

    modbus_free(ctx);

    return 0;
}