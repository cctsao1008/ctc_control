/**********************************************
 * @file rs485p2.h                  	      *
 *                                            *
 * RS485 Port2 Handle Thread				  *
 *                                            *
 * @author Joshua <jan.joshua@iac.com.tw>     *
 **********************************************/

#include "core_common.h"

/* RS485Port Class */
RS485Port::RS485Port(const char* COM)
{
    int status;

    /*
    * Allocate the serial port struct.
    * This defaults to 9600-8-N-1
    */
    if (c_serial_new(&_port, NULL) < 0){
        fprintf(stderr, "[ERROR]: RS485Port unable to create new serial port\n");
    }

    /*
    * The port name is the device to open(/dev/ttyS0 on Linux,
    * COM1 on Windows)
    */
    if (c_serial_set_port_name(_port, COM) < 0){
        fprintf(stderr, "[ERROR]: RS485Port can't set port name\n");
    }

    /*
    * Set various serial port settings.  These are the default.
    */
    c_serial_set_baud_rate(_port, CSERIAL_BAUD_9600);
    c_serial_set_data_bits(_port, CSERIAL_BITS_8);
    c_serial_set_stop_bits(_port, CSERIAL_STOP_BITS_1);
    c_serial_set_parity(_port, CSERIAL_PARITY_NONE);
    c_serial_set_flow_control(_port, CSERIAL_FLOW_NONE);

    /*
    * We want to get all line flags when they change
    */
    c_serial_set_serial_line_change_flags(_port, CSERIAL_LINE_FLAG_ALL);

    status = c_serial_open(_port);
    if (status < 0){
        fprintf(stderr, "[ERROR]: RS485Port can't open serial port\n");
    }

	_name = COM;
	//fprintf(stdout, "[INFO]: RS485Port successfully established %s port\n", _name);
	initThread();
	//fprintf(stdout, "[INFO]: RS485Port successfully established thread\n");
}

RS485Port::~RS485Port()
{
    // Release memory if it's needed
    c_serial_free(_port);
	//fprintf(stdout, "[INFO]: RS485Port successfully released %s\n", _name);

	// exit thread
	DWORD dwExit;
	GetExitCodeThread(_hThread, &dwExit);
	TerminateThread(_hThread, dwExit);
	CloseHandle(_hThread);
	//fprintf(stdout, "[INFO]: RS485Port successfully exited thread\n");

	// finish
	//fprintf(stdout, "[INFO]: RS485Port successfully deconstructed\n");
}

bool RS485Port::setRS485Port(c_serial_port_t *Port)
{
	_port = Port;
    return true;
}

c_serial_port_t* RS485Port::getPortHandle()
{
	return _port;
}

HANDLE RS485Port::getMutex()
{
	return _ghMutex;
}

// Thread
bool RS485Port::initThread()
{	
	// init _recieveMap
	_recieveMap.insert(std::pair <uint8_t, std::vector<Message>>('\x00', std::vector<Message>())); // try
	_recieveMap.insert(std::pair <uint8_t, std::vector<Message>>('\x01', std::vector<Message>())); // try

	// init thread parameter
	_ghMutex = CreateMutex(NULL, FALSE, (LPCSTR) _name);
    _threadParameter._ghMutex = &_ghMutex;
    _threadParameter._port = _port;
	_threadParameter._unknownAnswer = &_unknownAnswer;		// try
	_threadParameter._unknownAddress = &_unknownAddress;	// try
	_threadParameter._recieveMap = &_recieveMap;			// try
	

    _hThread = NULL;

    _hThread = CreateThread(NULL,                       // default security attributes
                                0,                      // use default stack size
		 &RS485Port::ReadWriteMsg,                      // thread function
        (LPVOID)&_threadParameter,                      // argument to thread function
                                0,                      // use default creation flags
                            NULL);                      // returns the thread identifier

    if (_hThread == NULL)
    {
		fprintf(stderr, "[ERROR]: RS485Port: recieveMsg Thread Cannot Be Created\n");
        return false;
    }

    //fprintf(stdout, "[INFO]: RS485Port: Create Thread Successed\n");

    return true;
}

Message* RS485Port::getControlerMsg(uint8_t Address)
{
	/*
    if (!_recieveQueue.empty())
    {
        _content[0] = _recieveQueue.front().content[0];
        _content[1] = _recieveQueue.front().content[1];
        _msg.content = _content;
        _msg.length = 2;
        _recieveQueue.erase(_recieveQueue.begin());
        return &_msg;
    }
	*/

	// try
	//printf("Address: %.2X\n", Address);
	//WaitForSingleObject(_ghMutex, INFINITE);
	if (!_recieveMap.find(Address)->second.empty())
	{
		WaitForSingleObject(_ghMutex, INFINITE);
		//printf("Address: %.2X get into if condition\n", Address);
		_msg = new Message;
		_content = new uint8_t[2];
		_content[0] = _recieveMap.find(Address)->second.front().content[0];
		_content[1] = _recieveMap.find(Address)->second.front().content[1];
		_msg->content = _content;
		_msg->length = 2;
		fprintf(stdout, "getControlerMsg(uint8_t %.2X) ", Address);
		for (int i = 0; i < _msg->length; i++)
		{
			printf("%.2X ", _msg->content[i]);
		}
		printf("\n");
		delete [] _recieveMap.find(Address)->second.front().content; // delete array
		_recieveMap.find(Address)->second.erase(_recieveMap.find(Address)->second.begin());
		ReleaseMutex(_ghMutex);
		return _msg;
	}
	//ReleaseMutex(_ghMutex);

    return NULL;
}

DWORD WINAPI RS485Port::ReadWriteMsg(LPVOID ThreadParameter)
{
    c_serial_port_t* port = ((Parameter*)ThreadParameter)->_port;
    std::vector<Message>* recieveQueue = ((Parameter*)ThreadParameter)->_recieveQueue;
	std::vector<uint8_t>* unknownAnswer = ((Parameter*)ThreadParameter)->_unknownAnswer;			  // try
	std::vector<uint8_t>* unknownAddress = ((Parameter*)ThreadParameter)->_unknownAddress;			  // try
	std::map<uint8_t, std::vector<Message>>* recieveMap = ((Parameter*)ThreadParameter)->_recieveMap; // try
    Message Reciece;
    uint8_t content[2] = { '\x00' };
    Reciece.content = content;
    Reciece.length = 0;

    while (true)
    {
		WaitForSingleObject(((Parameter*)ThreadParameter)->_ghMutex, INFINITE);
        c_serial_get_available(port, &Reciece.length);

        if (Reciece.length >= 2)
        {
			//WaitForSingleObject(((Parameter*)ThreadParameter)->_ghMutex, INFINITE);
            //printf("Available Bytes: %d\n", Reciece.length);
            Reciece.length = 2;
			c_serial_read_data(port, content, &Reciece.length, NULL);
		
			///*
            for (int i = 0; i < Reciece.length; i++)
            {
				fprintf(stdout, "%.2X ", content[i]);
            }
			fprintf(stdout, "\n");
			//*/

			for (int i = 0; i < Reciece.length; i++)
			{
				if (recieveMap->find(content[i]) != recieveMap->end())
				{
					unknownAddress->push_back(content[i]);
				}
				else
				{
					unknownAnswer->push_back(content[i]);
				}
			}

			/*
			if (recieveMap->find(Reciece.content[0]) != recieveMap->end())
			{
				unknownAddress->push_back(Reciece.content[0]);
			}
			else
			{
				unknownAnswer->push_back(Reciece.content[0]);
			}

			if (recieveMap->find(Reciece.content[1]) != recieveMap->end())
			{
				unknownAddress->push_back(Reciece.content[1]);
			}
			else
			{
				unknownAnswer->push_back(Reciece.content[1]);
			}
			*/

			for (int i = 0; i < Reciece.length; i++)
			{
				Reciece.content[i] = (uint8_t) '\x00';
			}

			//printf("unknownAnswer->size(): %d, unknownAddress->size(): %d\n", unknownAnswer->size(), unknownAddress->size());
			//WaitForSingleObject(((Parameter*)ThreadParameter)->_ghMutex, INFINITE);
			while (unknownAnswer->size() > 0 && unknownAddress->size() > 0)
			{
				Reciece.length = 2;
				Reciece.content = new uint8_t[2];
				Reciece.content[0] = unknownAnswer->front();
				Reciece.content[1] = unknownAddress->front();
				
				fprintf(stdout, "Reciece.content: ");
				for (int i = 0; i < Reciece.length; i++)
				{
					fprintf(stdout, "%.2X ", Reciece.content[i]);
				}
				fprintf(stdout, "\n");
				
				unknownAnswer->erase(unknownAnswer->begin());
				unknownAddress->erase(unknownAddress->begin());
				recieveMap->find(Reciece.content[1])->second.push_back(Reciece);
				//printf("recieveMap->find(Reciece.content[1])->second.size(): %d\n", recieveMap->find(Reciece.content[1])->second.size());
				Reciece.content = NULL;
			}
			Reciece.content = content;
			Reciece.content = content;
			//ReleaseMutex(((Parameter*)ThreadParameter)->_ghMutex);
        }
		ReleaseMutex(((Parameter*)ThreadParameter)->_ghMutex);
        Sleep(100);
    }

    return 0;
}


static BianNeng bianneng = BianNeng();
static RS485Port rs485p2 = RS485Port("COM6");
static SyringePump syringepump = SyringePump(&rs485p2);
static Washer washer = Washer(&rs485p2);
static MicroscopeXY microscopexy = MicroscopeXY(&rs485p2);


void rs485p2_memory_clean(Argument* ThreadParameter)
{
	for (int i = 0; i < (ThreadParameter)->_argc; i++)
	{
		delete[](ThreadParameter)->_argv[i];
	}
	delete[](ThreadParameter)->_argv;
}

DWORD WINAPI rs485p2_thread_main(LPVOID ThreadParameter)
{
	int argc = ((Argument *)ThreadParameter)->_argc;
	char** argv = ((Argument *)ThreadParameter)->_argv;
	
	log_info("[commander] pub, argc = %d", argc);

	/*
	for (int i = 0; i < argc; i++)
	{
		printf("%s ", argv[i]);
	}
	printf("\n");
	*/

	if (argc < 2) 
	{
		log_info("missing command");
		rs485p2_memory_clean((Argument *)ThreadParameter);
		return 0;
	}

	// BN stands for BengNeng Motor Controler
	if (!strcmp(argv[1], "BN"))
	{
		if (argc != 8)
		{
			log_info("error");
			rs485p2_memory_clean((Argument *)ThreadParameter);
			return 0;
		}

		Message* msg = bianneng.trans2RTCMD(argv[2], (uint8_t)atoi(argv[3]), (*argv[4] - '0'), argv[5], argv[6], argv[7]);
		for (int i = 0; i < msg->length; i++)
		{
			printf("%.2X ", msg->content[i]);
		}
		printf("\n");
		rs485p2_memory_clean((Argument *)ThreadParameter);
		return 0;
	}

	// SP stands for Syringe Pump
	if (!strcmp(argv[1], "SP"))
	{
		if (argc < 3)
		{
			log_info("error");
			rs485p2_memory_clean((Argument *)ThreadParameter);
			return 0;
		}

		// AB stands for Absorb
		if (!strcmp(argv[2], "AB"))
		{
			syringepump.absorbVolume(atof(argv[3]));
			rs485p2_memory_clean((Argument *)ThreadParameter);
			return 0;
		}

		// DR stands for Drain
		if (!strcmp(argv[2], "DR"))
		{
			syringepump.drainVolume(atof(argv[3]));
			rs485p2_memory_clean((Argument *)ThreadParameter);
			return 0;
		}

		if (argc != 5)
		{
			log_info("error");
			rs485p2_memory_clean((Argument *)ThreadParameter);
			return 0;
		}

		// PIP stands for Pipetting
		if (!strcmp(argv[2], "PIP"))
		{
			syringepump.pipetteVolume(atof(argv[3]), atoi(argv[4]));
			rs485p2_memory_clean((Argument *)ThreadParameter);
			return 0;
		}

		return 0;
	}

	// WM stands for Washer Machine
	if (!strcmp(argv[1], "WM"))
	{
		if (argc != 5)
		{
			log_info("error");
			rs485p2_memory_clean((Argument *)ThreadParameter);
			return 0;
		}

		// MA stands for Move Arm
		if (!strcmp(argv[2], "MA"))
		{
			washer.moveArm(atof(argv[3]), (uint8_t)atoi(argv[4]));
			rs485p2_memory_clean((Argument *)ThreadParameter);
			return 0;
		}

		// SH stands for Shake
		if (!strcmp(argv[2], "SH"))
		{
			washer.shakeMachine(atof(argv[3]), (unsigned int)atoi(argv[4]));
			return 0;
		}

		// RG stands for Rotate Gripper
		if (!strcmp(argv[2], "RG"))
		{
			washer.rotateGripper(atof(argv[3]), (uint8_t)atoi(argv[4]));
			rs485p2_memory_clean((Argument *)ThreadParameter);
			return 0;
		}
		rs485p2_memory_clean((Argument *)ThreadParameter);
		return 0;
	}

	// MXY stands for Microscope XY Table
	if (!strcmp(argv[1], "MXY"))
	{
		if (argc != 5)
		{
			log_info("error");
			rs485p2_memory_clean((Argument *)ThreadParameter);
			return 0;
		}

		// MV stands for Move to (X, Y)
		if (!strcmp(argv[2], "MV"))
		{
			microscopexy.move2Pos(atof(argv[3]), atof(argv[4]));
			rs485p2_memory_clean((Argument *)ThreadParameter);
			return 0;
		}
		rs485p2_memory_clean((Argument *)ThreadParameter);
		return 0;
	}
	rs485p2_memory_clean((Argument *)ThreadParameter);
	return 0;
}

int rsh_rs485p2_main(int argc, char *argv[])
{
	HANDLE hThread = NULL;
	Argument *threadParameter = new Argument;

	threadParameter->_argc = argc;
	threadParameter->_argv = new char *[argc];

	for (int i = 0; i < argc; i++)
	{
		int length = 0;
		while (argv[i][length] != '\0')
		{
			length++;
		}
		threadParameter->_argv[i] = new char[(length + 1)];

		for (int j = 0; j < (length + 1); j++)
		{
			threadParameter->_argv[i][j] = argv[i][j];
		}
	}
	
	hThread = CreateThread(NULL,                       // default security attributes
							  0,					   // use default stack size
		   &rs485p2_thread_main,                       // thread function
	   (LPVOID) threadParameter,                       // argument to thread function
							  0,                       // use default creation flags
						   NULL);                      // returns the thread identifier
	
	if (hThread == NULL)
	{
		fprintf(stderr, "[ERROR]: rsh_rs485p2_main: rs485p2_thread_main Thread Cannot Be Created\n");
		return false;
	}

	return 0;
}