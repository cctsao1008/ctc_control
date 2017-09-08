/**********************************************
 * @file rs485p2.h                  	      *
 *                                            *
 * RS485 Port2 Handle Thread				  *
 *                                            *
 * @author Joshua <jan.joshua@iac.com.tw>     *
 **********************************************/

#ifndef _RS485P2_H_
#define _RS485P2_H_

#include "core_common.h"

class RS485Port
{
public:
	// Constructor
    RS485Port(const char* COM);

    // Destructor
    ~RS485Port();

	// setting
	bool setRS485Port(c_serial_port_t* Port);

	// getting
	c_serial_port_t* getPortHandle();
	HANDLE getMutex();

	//Thread
	bool initThread();
	void clearMsg(uint8_t Address);
	Message* getControlerMsg(uint8_t Address);
	static DWORD WINAPI ReadWriteMsg(LPVOID ThreadParameter);

private:
	// Port
	const char*							_name;		// port name
	c_serial_port_t*					_port;		// pointer to the COMPort class be used
	/* variables used for manage the thread */
	HANDLE	_hThread;
	DWORD	_dwThreadId;
	DWORD	_dwThreadParam;
	/* variables used for passing message between thread and main process*/
	Message*				_msg;
	uint8_t*				_content;
	Parameter				_threadParameter;
	std::vector<uint8_t> _unknownAnswer;				 // try
	std::vector<uint8_t> _unknownAddress;				 // try
	std::map<uint8_t, std::vector<Message>> _recieveMap; // try
	/* mutex lock */
	HANDLE _ghMutex;
};

/**
Arguments For Thread
*/
typedef struct _argument 
{
	int _argc;
	char** _argv;

} Argument;

void rs485p2_memory_clean(Argument* ThreadParameter);

DWORD WINAPI rs485p2_thread_main(LPVOID ThreadParameter);

int rsh_rs485p2_main(int argc, char *argv[]);

static RS485Port* rs485p2;

extern void initRS485P2para();

#endif // _RS485P2_H_