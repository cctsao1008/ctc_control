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
    printf("CCS: Available ports:\n");
    while (port_list[x] != NULL){
        printf("%s\n", port_list[x]);
        x++;
    }

    c_serial_free_serial_ports_list(port_list);

    /*
    * Allocate the serial port struct.
    * This defaults to 9600-8-N-1
    */
    if (c_serial_new(&m_port[0], NULL) < 0){
        fprintf(stderr, "CCS: ERROR: Unable to create new serial port\n");
        return 1;
    }

    /*
    * The port name is the device to open(/dev/ttyS0 on Linux,
    * COM1 on Windows)
    */
    if (c_serial_set_port_name(m_port[0], "COM65") < 0){
        fprintf(stderr, "CCS: ERROR : can't set port name\n");
    }

    /*
    * Set various serial port settings.  These are the default.
    */
    c_serial_set_baud_rate(m_port[0], CSERIAL_BAUD_9600);
    c_serial_set_data_bits(m_port[0], CSERIAL_BITS_8);
    c_serial_set_stop_bits(m_port[0], CSERIAL_STOP_BITS_1);
    c_serial_set_parity(m_port[0], CSERIAL_PARITY_NONE);
    c_serial_set_flow_control(m_port[0], CSERIAL_FLOW_NONE);

    printf("Baud rate is %d\n", c_serial_get_baud_rate(m_port[0]));

    status = c_serial_open(m_port[0]);

    if (status < 0){
        fprintf(stderr, "ERROR: Can't open serial port\n");
        return 1;
    }

    /*
    * Allocate the serial port struct.
    * This defaults to 9600-8-N-1
    */
    if (c_serial_new(&m_port[1], NULL) < 0){
        fprintf(stderr, "CCS: ERROR: Unable to create new serial port\n");
        return 1;
    }

    /*
    * The port name is the device to open(/dev/ttyS0 on Linux,
    * COM1 on Windows)
    */
    if (c_serial_set_port_name(m_port[1], "COM6") < 0){
        fprintf(stderr, "CCS: ERROR : can't set port name\n");
    }

    /*
    * Set various serial port settings.  These are the default.
    */
    c_serial_set_baud_rate(m_port[1], CSERIAL_BAUD_9600);
    c_serial_set_data_bits(m_port[1], CSERIAL_BITS_8);
    c_serial_set_stop_bits(m_port[1], CSERIAL_STOP_BITS_1);
    c_serial_set_parity(m_port[1], CSERIAL_PARITY_NONE);
    c_serial_set_flow_control(m_port[1], CSERIAL_FLOW_NONE);

    printf("Baud rate is %d\n", c_serial_get_baud_rate(m_port[1]));

    status = c_serial_open(m_port[1]);

    if (status < 0){
        fprintf(stderr, "ERROR: Can't open serial port\n");
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
        printf("CCS: core_serialport_thread_tx.\n");
        c_serial_write_data(m_port[0], (void *)tx_buf, &len);
        Sleep(500);
    }

    printf("CCS: core_serialport_thread_tx exit.\n");

    return 0;
}

void* core_serialport_thread_rx(void* arg)
{
    int len = sizeof(rx_buf), av = 0;

    unsigned int count = 0;

    printf("CCS: core_serialport_thread_rx.\n");

    while (run)
    {
        c_serial_get_available(m_port[0], &av);

        if (av > 0)
        {
            printf("CCS: core_serialport_thread_rx_%04d ( av = %d).\n", count++, av);
            c_serial_read_data(m_port[0], rx_buf, &len, NULL);

            for (int i = 0; i < av; i++)
            {
                printf("%X ", rx_buf[i]);
            }
            printf("\n");
        }

        Sleep(100);
    }

    printf("CCS: core_serialport_thread_rx exit.\n");

    return 0;
}

void* core_serialport_thread_main(void* arg)
{
    printf("CCS: core_serialport_thread_main running...\n");

    //pthread_create(&pid[0], NULL, &core_serialport_thread_tx, (void *)&arg);
    //pthread_create(&pid[0], NULL, &core_serialport_thread_rx, (void *)&arg);

    while (run)
    {
        Sleep(1000);
    }

    printf("CCS: core_serialport_thread_main exit.\n");

    return 0;
}
