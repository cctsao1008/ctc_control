/**
* @file core_serialport.cpp
*
* core_serialport
*
*   core_serialport.
*
* @author Ricardo <tsao.ricardo@iac.com.tw>
*/

#include "core_common.h"
#include "c_serial.h"

static c_serial_port_t* m_port[4];
static int status;
static pthread_t pid;

int core_serialport_open(void)
{
    /*
    * Since no port was provided, print the available ports
    */
    const char** port_list = c_serial_get_serial_ports_list();
    int x = 0;
	log_info("CCS: Available ports:");
    while (port_list[x] != NULL){
		log_info("%s", port_list[x]);
        x++;
    }

    c_serial_free_serial_ports_list(port_list);

    /*
    * Allocate the serial port struct.
    * This defaults to 9600-8-N-1
    */
    if (c_serial_new(&m_port[0], NULL) < 0){
		log_error("CCS: Unable to create new serial port");
        return 1;
    }

    /*
    * The port name is the device to open(/dev/ttyS0 on Linux,
    * COM1 on Windows)
    */
    if (c_serial_set_port_name(m_port[0], "COM65") < 0){
        //fprintf(stderr, "CCS: ERROR : can't set port name\n");
		log_error("CCS: can't set port name");
    }

    /*
    * Set various serial port settings.  These are the default.
    */
    c_serial_set_baud_rate(m_port[0], CSERIAL_BAUD_9600);
    c_serial_set_data_bits(m_port[0], CSERIAL_BITS_8);
    c_serial_set_stop_bits(m_port[0], CSERIAL_STOP_BITS_1);
    c_serial_set_parity(m_port[0], CSERIAL_PARITY_NONE);
    c_serial_set_flow_control(m_port[0], CSERIAL_FLOW_NONE);

	log_info("Baud rate is %d\n", c_serial_get_baud_rate(m_port[0]));

    status = c_serial_open(m_port[0]);

    if (status < 0){
		log_error("Can't open serial port");
        return 1;
    }

    /*
    * Allocate the serial port struct.
    * This defaults to 9600-8-N-1
    */
    if (c_serial_new(&m_port[1], NULL) < 0){
		log_error("Unable to create new serial port");
        return 1;
    }

    /*
    * The port name is the device to open(/dev/ttyS0 on Linux,
    * COM1 on Windows)
    */
    if (c_serial_set_port_name(m_port[1], "COM6") < 0){
		log_error("can't set port name");
    }

    /*
    * Set various serial port settings.  These are the default.
    */
    c_serial_set_baud_rate(m_port[1], CSERIAL_BAUD_9600);
    c_serial_set_data_bits(m_port[1], CSERIAL_BITS_8);
    c_serial_set_stop_bits(m_port[1], CSERIAL_STOP_BITS_1);
    c_serial_set_parity(m_port[1], CSERIAL_PARITY_NONE);
    c_serial_set_flow_control(m_port[1], CSERIAL_FLOW_NONE);

	log_info("Baud rate is %d", c_serial_get_baud_rate(m_port[1]));

    status = c_serial_open(m_port[1]);

    if (status < 0){
		log_error("Can't open serial port");
        return 1;
    }

    pthread_create(&pid, NULL, &core_serialport_thread_main, NULL);

    return 0;
}

extern int run;
extern int vote;

const unsigned char tx_buf[32] = { 0xBA, 0x01, 0xF6, 0x18, 0x00, 0x03, 0x01, 0x31,
                    0x00, 0x02, 0x58, 0x00, 0x64, 0x00, 0xC8, 0x90, 0xFE};
unsigned char rx_buf[32] = { 0 };

void* core_serialport_thread_tx(void* arg)
{
    //int len = sizeof(tx_buf);
    int len = 17;

    while (run)
    {
		log_info("CCS: core_serialport_thread_tx.");
        c_serial_write_data(m_port[0], (void *)tx_buf, &len);
        Sleep(500);
    }

	log_info("CCS: core_serialport_thread_tx exit.");

    return 0;
}

void* core_serialport_thread_rx(void* arg)
{
    int len = sizeof(rx_buf), av = 0;

    unsigned int count = 0;

	log_info("CCS: core_serialport_thread_rx.");

    while (run)
    {
        c_serial_get_available(m_port[0], &av);

        if (av > 0)
        {
			log_info("CCS: core_serialport_thread_rx_%04d ( av = %d).", count++, av);
            c_serial_read_data(m_port[0], rx_buf, &len, NULL);

            for (int i = 0; i < av; i++)
            {
                printf("%X ", rx_buf[i]);
            }
            printf("\n");
        }

        Sleep(100);
    }

	log_info("CCS: core_serialport_thread_rx exit.");

    return 0;
}

void* core_serialport_thread_main(void* arg)
{
	log_info("CCS: core_serialport_thread_main running.");

    //pthread_create(&pid[0], NULL, &core_serialport_thread_tx, (void *)&arg);
    //pthread_create(&pid[0], NULL, &core_serialport_thread_rx, (void *)&arg);

    while (run)
    {
        Sleep(1000);
    }

	log_info("CCS: core_serialport_thread_main exit.");

    return 0;
}
