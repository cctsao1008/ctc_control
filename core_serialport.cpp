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

c_serial_port_t* m_port;

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
	if (c_serial_new(&m_port, NULL) < 0){
		fprintf(stderr, "CCS: ERROR: Unable to create new serial port\n");
		return 1;
	}

	/*
	* The port name is the device to open(/dev/ttyS0 on Linux,
	* COM1 on Windows)
	*/
	if (c_serial_set_port_name(m_port, "COM4") < 0){
		fprintf(stderr, "CCS: ERROR : can't set port name\n");
	}

	return 0;
}
