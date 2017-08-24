/**
* @file core_modbus.cpp
*
* core_modbus
*
*   core_modbus.
*
* @author Ricardo <tsao.ricardo@iac.com.tw>
*/

#if 1
#include "core_common.h"

#define MODBUS_SERIAL_DEV           "COM2"
//#define MODBUS_SERIAL_DEV           "COM5"
#define MODBUS_SERIAL_BAUDRATE      115200    /* 9600, 38400, 115200, ... */
#define MODBUS_SERIAL_PARITY        'N'     /* 'N', 'E', or 'O' */
#define MODBUS_SERIAL_DATABITS      8       /* 5, 6, 7, or 8 */
#define MODBUS_SERIAL_STOPBITS      2       /* 1 or 2 */
#define MODBUS_DEVICE_ID            1
#define MODBUS_TIMEOUT_SEC          3
#define MODBUS_TIMEOUT_USEC         0
#define MODBUS_DEBUG                ON
#define MODBUS_RO_BITS              32
#define MODBUS_RW_BITS              32
#define MODBUS_RO_REGISTERS         64
#define MODBUS_RW_REGISTERS         64

void* core_modbus_thread_main(void* arg);

static modbus_t            *ctx = NULL;
static modbus_mapping_t    *data_mapping;
static struct timeval      timeout;
static int                 ret, go = 1;
static uint8_t             query[MODBUS_TCP_MAX_ADU_LENGTH];

static pthread_t pid;

extern int run;
extern int vote;

int core_modbus_open(void* handle)
{
	log_info("core_modbus_open.");

	if (ctx != NULL)
	{
		handle = ctx;
		return 0;
	}

	//log_info("ctx == NULL.");

	ctx = modbus_new_rtu(MODBUS_SERIAL_DEV,
		MODBUS_SERIAL_BAUDRATE,
		MODBUS_SERIAL_PARITY,
		MODBUS_SERIAL_DATABITS,
		MODBUS_SERIAL_STOPBITS);

	printf("modbus_new_rtu %x\n", ctx);

#if 0
	ctx = modbus_new_rtu(MODBUS_SERIAL_DEV,
		MODBUS_SERIAL_BAUDRATE,
		MODBUS_SERIAL_PARITY,
		MODBUS_SERIAL_DATABITS,
		MODBUS_SERIAL_STOPBITS);

	printf("modbus_new_rtu %x\n", ctx);
#endif

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

	Sleep(30);
    pthread_create(&pid, NULL, &core_modbus_thread_main, NULL);

	uint16_t data[64];

	/* open serial interface */
	modbus_connect(ctx);

	log_info("modbus connected.");

	/* DI contact control */
	modbus_write_register(ctx, 0x061E, 0x0040);
	Sleep(50);

	modbus_read_registers(ctx, 0x0630, 1, data);
	Sleep(50);
	log_info("addr = 0x0630, data = 0x%X", data[0]);

	data[0] = data[0] | 0x0040;
	modbus_write_register(ctx, 0x0630, data[0]);
	Sleep(50);

	modbus_read_registers(ctx, 0x0630, 1, data);
	Sleep(50);
	log_info("addr = 0x0630, data = 0x%X", data[0]);

start:

	//// RPM START
	/* To quit JOG mode by 0x0000 written at address ¡§0x0901¡¨. */
	log_info("Step 6: To quit this mode by 0000h written at address 0901h.");
	modbus_write_register(ctx, 0x0901, 0);
	Sleep(50);

	// Step 1: Check the drive without any alarm occurrence or Servo ON activated.
	log_info("Step 1: Check the drive without any alarm occurrence or Servo ON activated.");
	modbus_read_registers(ctx, 0x0900, 1, data); // 
	log_info("addr = 0x0900, data = 0x%X", data[0]);
	Sleep(50);

	// Step 2: Write 0x0003 to perform this mode.
	log_info("Step 2: Write 0x0003 to perform this mode.");
	modbus_write_register(ctx, 0x0901, 0x0003); // enter JOG mode
	Sleep(50);

	// Step 3: To set acceleration/deceleration time constant of JOG test(suitable for positioning test).
	log_info("Step 3: To set acceleration/deceleration time constant of JOG test(suitable for positioning test).");
	modbus_write_register(ctx, 0x0902, 2500);
	Sleep(50);

	// Step 4: JOG speed command
	log_info("Step 4: JOG speed command");

	modbus_write_register(ctx, 0x0903, 3000);
	Sleep(50);


	// Step 5: JOG operation
	log_info("Step 5: JOG operation");
	modbus_write_register(ctx, 0x0904, 1);

	for (int i = 0; i < 300; i++)
	{
		modbus_read_registers(ctx, 0x0020, 2, data); // Pulses of Z phase
		uint16_t z_pulsu = (uint32_t)data[0] | (uint32_t)data[1] << 16;
		//log_info("addr = 0x0020, data = %d", abs(z_pulsu));
		Sleep(10);
	}

	modbus_write_register(ctx, 0x0903, 0);

	for (int i = 0; i < 200; i++)
	{
		modbus_read_registers(ctx, 0x0020, 2, data); // Pulses of Z phase
		uint16_t z_pulsu = (uint32_t)data[0] | (uint32_t)data[1] << 16;
		//log_info("addr = 0x0020, data = %d", abs(z_pulsu));
		Sleep(10);
	}

	modbus_write_register(ctx, 0x0904, 0);
	Sleep(50);

	// Step 6: To quit this mode by 0000h written at address 0901h.
	log_info("Step 6: To quit this mode by 0000h written at address 0901h.");
	modbus_write_register(ctx, 0x0901, 0);
	Sleep(50);

	//// RPM END

	Sleep(1000);

	/* Positioning test , go home */
	modbus_read_registers(ctx, 0x0020, 2, data); // Pulses of Z phase

	int16_t z_pulsu = (uint32_t)data[0] | (uint32_t)data[1] << 16;
	log_info("addr = 0x0020, data = %d", z_pulsu);

	/* Step 1: Check the drive without any alarm occurrence or Servo ON activated. */
	log_info("Step 1: Check the drive without any alarm occurrence or Servo ON activated.");
	modbus_read_registers(ctx, 0x0900, 1, data); // 
	log_info("addr = 0x0900, data = 0x%X", data[0]);
	Sleep(2000);

	/* Step 2: Write 0x0004 at address ¡§0x0901¡¨ to perform this positioning test. */
	log_info("Step 2: Write 0x0004 at address ¡§0x0901¡¨ to perform this positioning test.");
	modbus_write_register(ctx, 0x0901, 0x0004); // enter JOG mode
	Sleep(50);

	/* Step 3: To set the acceleration/deceleration time constant of positioning test. */
	log_info("Step 3: To set acceleration/deceleration time constant of JOG test(suitable for positioning test).");
	modbus_write_register(ctx, 0x0902, 500); // 200ms
	Sleep(50);

	/* Step 4: To set speed command of positioning test.(Refer to JOG test mentioned above) */
	modbus_write_register(ctx, 0x0903, 500); // 500RPM
	Sleep(50);

	/* Step 5: To set the command pulses. */
	uint32_t pos_pulse = 0;
	pos_pulse = (uint32_t)fabs(z_pulsu * 419.4304f);

	log_info("z_pulsu = %d", pos_pulse);
	data[0] = 0xFFFF & pos_pulse;
	log_info("data[0] = %d", data[0]);
	data[1] = 0xFFFF & (pos_pulse >> 16);
	log_info("data[1] = %d", data[1]);
	modbus_write_register(ctx, 0x0905, data[0]);
	modbus_write_register(ctx, 0x0906, data[1]);
	Sleep(50);

	/* Step 6: Positioning test operation. */
	if (z_pulsu < 0)
		modbus_write_register(ctx, 0x0907, 1);
	else
		modbus_write_register(ctx, 0x0907, 2);

	Sleep(50);

	for (int i = 0; i < 100; i++)
	{
		modbus_read_registers(ctx, 0x0020, 2, data); // Pulses of Z phase
		uint16_t z_pulsu = (uint32_t)data[0] | (uint32_t)data[1] << 16;
		log_info("addr = 0x0020, data = %d", abs(z_pulsu));
		Sleep(10);
	}

	modbus_write_register(ctx, 0x0902, 1000);
	modbus_write_register(ctx, 0x0903, 50);
	Sleep(50);

	for (int i = 0; i < 4; i++)
	{
		log_info("POSISTION");
		pos_pulse = 1048576L;
		log_info("pos_pulse = %d", pos_pulse);
		data[0] = 0xFFFF & pos_pulse;
		log_info("data[0] = %d", data[0]);
		data[1] = 0xFFFF & (pos_pulse >> 16);
		log_info("data[1] = %d", data[1]);
		modbus_write_register(ctx, 0x0905, data[0]);
		modbus_write_register(ctx, 0x0906, data[1]);

		modbus_write_register(ctx, 0x0907, 2);

		for (int i = 0; i < 150; i++)
		{
			modbus_read_registers(ctx, 0x0900, 1, data);
			Sleep(10);
		}
	}

	/* Step 8: To quit this mode by 0x0000 written at address ¡§0x0901¡¨. */
	log_info("Step 6: To quit this mode by 0000h written at address 0901h.");
	modbus_write_register(ctx, 0x0901, 0);
	Sleep(50);

	goto start;

#if 1
	while (run) {

		modbus_read_registers(ctx, 0x0630, 1, data);
		Sleep(50);
		//log_info("addr = 0x0630, data = 0x%X", data[0]);

		modbus_read_registers(ctx, 0x0020, 2, data); // Pulses of Z phase
		uint16_t z_pulsu = (uint32_t)data[0] | (uint32_t)data[1] << 16;
		//log_info("addr = 0x0020, data = %d", z_pulsu);
		Sleep(1000);
	}

	goto cleanup;
#endif

	// Step 1: Check the drive without any alarm occurrence or Servo ON activated.
	log_info("Step 1: Check the drive without any alarm occurrence or Servo ON activated.");
	modbus_read_registers(ctx, 0x0900, 1, data); // 
	log_info("addr = 0x0900, data = 0x%X", data[0]);
	Sleep(2000);

	// Step 2: Write 0x0003 to perform this mode.
	log_info("Step 2: Write 0x0003 to perform this mode.");
	modbus_write_register(ctx, 0x0901, 0x0003); // enter JOG mode
	Sleep(50);

	// Step 3: To set acceleration/deceleration time constant of JOG test(suitable for positioning test).
	log_info("Step 3: To set acceleration/deceleration time constant of JOG test(suitable for positioning test).");
	modbus_write_register(ctx, 0x0902, 200); // 200ms
	Sleep(50);

	// Step 4: JOG speed command
	log_info("Step 4: JOG speed command");

	modbus_write_register(ctx, 0x0903, 1000); // 200RPM
	Sleep(50);


	// Step 5: JOG operation
	log_info("Step 5: JOG operation");
	modbus_write_register(ctx, 0x0904, 1);

	for (int i = 0; i < 50; i++)
	{
		modbus_read_registers(ctx, 0x0020, 2, data); // Pulses of Z phase
		uint16_t z_pulsu = (uint32_t)data[0] | (uint32_t)data[1] << 16;
		log_info("addr = 0x0020, data = %d", abs(z_pulsu));
		Sleep(10);
	}

	//Sleep(10000);

	modbus_write_register(ctx, 0x0904, 0);
	Sleep(50);

	// Step 6: To quit this mode by 0000h written at address 0901h.
	log_info("Step 6: To quit this mode by 0000h written at address 0901h.");
	modbus_write_register(ctx, 0x0901, 0);
	Sleep(50);

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
	}

cleanup:
    /* FIXME: initiate all data here */

	handle = ctx;

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
    //uint16_t data[64];

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
		/*
		modbus_read_registers(ctx, 0x0100, 1, data);
		log_info("addr = 0x100, data = 0x%X", data[0]); // warring number

		modbus_read_registers(ctx, 0x0200, 1, data);
		log_info("addr = 0x200, data = 0x%X", data[0]); // servo on/off

		modbus_read_registers(ctx, 0x0204, 1, data);
		log_info("addr = 0x204, data = 0x%X", data[0]); // I/O status

		modbus_read_registers(ctx, 0x0206, 1, data);
		log_info("addr = 0x206, data = 0x%X", data[0]); // I/O status
		*/
		//modbus_read_registers(ctx, 0x0020, 2, data);
		//log_info("addr = 0x0020, data = %d", (uint32_t)data[0] | (uint32_t)data[1]<<16);

        Sleep(1000);
    }

	vote--;

    //printf("CCS : core_modbus_thread_main exit. %s\n", modbus_strerror(errno));
	log_info("core_modbus_thread_main exit.");

    modbus_mapping_free(data_mapping);

    modbus_free(ctx);

    return 0;
}
#endif
