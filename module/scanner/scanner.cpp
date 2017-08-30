/**
* @file core_scanner.cpp
*
* core_scanner
*
*   core_scanner.
*
* @author Wesley <lin.wesley@iac.com.tw>
*/

#include "core_common.h"
#include "c_serial.h"

static c_serial_port_t* scan_port[3];
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
    //handle = &handle_scanner;

	//----------------------Scan0 initialize-------------------------------
	/*
	* Allocate the serial port struct.
	* This defaults to 9600-8-N-1
	*/
	if (c_serial_new(&scan_port[0], NULL) < 0){
		fprintf(stderr, "ERROR: Unable to create new Scan0 port\n");
		return -1;
	}

	/*
	* The port name is the device to open(/dev/ttyS0 on Linux,
	* COM1 on Windows)
	*/
	if (c_serial_set_port_name(scan_port[0], "COM20") < 0){
		fprintf(stderr, "ERROR : can't set Scan0 name\n");
		return -1;
	}

	/*
	* Set various serial port settings.  These are the default.
	*/
	c_serial_set_baud_rate(scan_port[0], CSERIAL_BAUD_9600);
	c_serial_set_data_bits(scan_port[0], CSERIAL_BITS_8);
	c_serial_set_stop_bits(scan_port[0], CSERIAL_STOP_BITS_1);
	c_serial_set_parity(scan_port[0], CSERIAL_PARITY_NONE);
	c_serial_set_flow_control(scan_port[0], CSERIAL_FLOW_NONE);

	printf("Baud rate is %d\n", c_serial_get_baud_rate(scan_port[0]));

	int status = c_serial_open(scan_port[0]);

	if (status < 0){
		fprintf(stderr, "ERROR: Can't open serial port\n");
		return -1;
	}
	//-----------------------Scan1 initialize-----------------------------
	//TBD
	//-----------------------Scan2 initialize-----------------------------
    //TBD
    //pthread_create(&pid, NULL, &core_scanner_thread_main, NULL);
    return 0;
}

int core_scanner_close(void* handle)
{
    return 0;
}

int core_scanner_ioctl(void* handle, int cmd, unsigned long arg)
{
    //if (handle == NULL)
    //    return (-1);

	uint8_t trigger_cmd[2] = { 0x1b, 0x31 };
	int trigger_cmd_length = 2;
	int status;
	int Ack_length = 10;
	uint8_t Ack[10];
	int available_num = 0;
	Scan_Data* data = (Scan_Data*)handle;
	switch (cmd)
	{
	case SCANNER_READ:
		//feedback_data.length = 0;
		Sleep(2000);
		available_num = 0;
		c_serial_get_available(scan_port[arg], &available_num);
		if (available_num > 0)
		{
			available_num--;
			data->length = available_num;
			status = c_serial_read_data(scan_port[arg], data->content, &(data->length), NULL);
			if (status < 0){
				fprintf(stderr, "ERROR: Can't Read Scanner1 data\n");
				return -1;
			}
			/*printf("Got %d bytes of data\n", data->length);
			for (int x = 0; x < data->length; x++){
			printf("    0x%02X (ASCII: %c)\n", data->content[x], data->content[x]);
			}*/
		}
		else
		{
			return -1;
		}
		// put your code here...

		break;

	case SCANNER_TRIGGER:
		// put your code here...
		status = 0;
		status = c_serial_write_data(scan_port[arg], trigger_cmd, &trigger_cmd_length);
		if (status < 0)
		{
			fprintf(stderr, "ERROR: Can't send Trigger command to Scanner1\n");
			return -1;
		}

		status = c_serial_read_data(scan_port[arg], Ack, &Ack_length, NULL);
		if (status < 0){
			fprintf(stderr, "ERROR: Can't Read Scanner1 Ack\n");
			return -1;
		}
		if (Ack[0] != 0x06)
		{
			return -1;
		}
		break;

	default:
		// put your code here...

		break;
	}
	return 0;
}

