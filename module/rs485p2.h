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
	RS485Port();
    RS485Port(const char* COM);

    // Destructor
    ~RS485Port();

	// setting
	bool setRS485Port(c_serial_port_t* Port);
	bool openRS485Port(const char* COM);

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
Arguments For rsh Thread
*/
typedef struct _argument 
{
	int _argc;
	char** _argv;
} Argument;

/**
Mutexes For rs485p2 Thread
*/
typedef struct _rs485p2ThreadMutex
{
	bool _pumpMutex;
	bool _washerMutex;
	bool _mpsMutex;
} rs485p2ThreadMutex;

bool get_pumpMutex();
bool get_washerMutex();
bool get_mspMutex();

void rs485p2_memory_clean(Argument* ThreadParameter);

DWORD WINAPI rs485p2_thread_main(LPVOID ThreadParameter);

int rsh_rs485p2_main(int argc, char *argv[]);

void rsh_rs485p2_mutex_init();

static RS485Port* rs485p2;

static rs485p2ThreadMutex rs485p2Mutex;

void initRS485P2para();

#endif // _RS485P2_H_