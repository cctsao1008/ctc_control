/**********************************************
 * @file core_bianneng.cpp                    *
 *                                            *
 * BianNeng Controler Command Format Transfer *
 *                                            *
 * @author Joshua <jan.joshua@iac.com.tw>     *
 **********************************************/

#include "stdafx.h"
#include "core_common.h"
#include "core_bianneng.h"

/* BianNeng Class */
BianNeng::BianNeng()
{
    // Initial class if it's needed 
}

BianNeng::~BianNeng()
{
    // Release memory if it's needed
}

/**
    Char * to Hex *
*/
void BianNeng::Chararray2Hexarray(const char * Input, uint8_t * Output, int Length)
{
    int count = 0;
    uint8_t buffer[4] = {'\x00'};
    unsigned int number  = atoi(Input);

    // Converts a value from int to bytes
    memcpy(buffer, &number, sizeof(buffer));
    
    // Reverse string
    //strrev(buffer); // it would be trouble when meet with 0x00
                      // due to being regarded as stop of array

    for(count = 0; count < Length; count++)
    {
        Output[Length - count - 1] = buffer[count];
    }
    // if I don't use buffer, it may cause RT_CMD be overwritten
    // And I need to use length to know the size of uint8_t array 
    //memcpy(output, buffer, length);
}

/**
    Counting CRC Byte
*/
uint8_t BianNeng::countCRC(uint8_t * Data, int Length)
{
    int count = 0;
    uint8_t result = '\x00';

    for(count = 0; count < Length; count++)
    {
        result = result ^ Data[count];
    }

    return result;
}

/**
    BianNeng Motor Controler Real Time Commad Transfer & Transmit
*/
Message* BianNeng::trans2RTCMD(const char* Prescaler, const char* Address, char Direction
                        , const char* TotalPulse, const char* AcceleratePulse, const char* BreakPulse)
{
    /*
    static uint8_t RT_CMD[17] = {'\xBA', '\x01', '\x00', '\x00', '\x00', '\x03'
                            , '\x01', '\x31', '\x00', '\x00', '\x00', '\x00'
                            , '\x00', '\x00', '\x00', '\x00', '\xFE'};
    */

    RT_CMD[0] = '\xBA';
    RT_CMD[1] = '\x01';
    RT_CMD[2] = '\x00';
    RT_CMD[3] = '\x00';
    RT_CMD[4] = '\x00';
    RT_CMD[5] = '\x03';
    RT_CMD[6] = '\x01';
    RT_CMD[7] = '\x31';
    RT_CMD[8] = '\x00';
    RT_CMD[9] = '\x00';
    RT_CMD[10] = '\x00';
    RT_CMD[11] = '\x00';
    RT_CMD[12] = '\x00';
    RT_CMD[13] = '\x00';
    RT_CMD[14] = '\x00';
    RT_CMD[15] = '\x00';
    RT_CMD[16] = '\xFE';

    uint8_t prescaler[2] = {0};
    uint8_t address = 0;
    uint8_t total_pulse[3] = {0};
    uint8_t accelerate_pulse[2] = {0};
    uint8_t break_pulse[2] = {0};

    Chararray2Hexarray(Prescaler, prescaler, sizeof(prescaler));
    Chararray2Hexarray(Address, &address, sizeof(address));
    Chararray2Hexarray(TotalPulse, total_pulse, sizeof(total_pulse));
    Chararray2Hexarray(AcceleratePulse, accelerate_pulse, sizeof(accelerate_pulse));
    Chararray2Hexarray(BreakPulse, break_pulse, sizeof(break_pulse));
    
    RT_CMD[2] = prescaler[0];
    RT_CMD[3] = prescaler[1];
    RT_CMD[4] = address;
    RT_CMD[6] = Direction;
    RT_CMD[8] = total_pulse[0];
    RT_CMD[9] = total_pulse[1];
    RT_CMD[10] = total_pulse[2];
    RT_CMD[11] = accelerate_pulse[0];
    RT_CMD[12] = accelerate_pulse[1];
    RT_CMD[13] = break_pulse[0];
    RT_CMD[14] = break_pulse[1];
    RT_CMD[15] = countCRC(RT_CMD, 15);

    _msg.length =  18;
    _msg.content = RT_CMD;

    /*
    for (int i = 0; i < _msg.length; i++)
    {
        printf("%.2X ", _msg.content[i]);
    }
    printf("\n");
    */

    return &_msg;
}

/**
    BianNeng Motor Controler Parameter Reserved Commad Transfer & Transmit
*/
Message* BianNeng::trans2PRCMD(const char* Prescaler, const char* Address, const char* TotalPulse
                        , const char* AcceleratePulse, const char* BreakPulse
                        , const char* PositiveStay, const char* NegativeStay)
{
    /*
    static uint8_t PR_CMD[21] = {'\xBA', '\x01', '\x00', '\x00', '\x00', '\x03'
                            , '\x01', '\x31', '\x00', '\x00', '\x00', '\x00'
                            , '\x00', '\x00', '\x00', '\x00', '\x00', '\x00'
                            , '\x00', '\x00', '\xFE'};
    */

    PR_CMD[0] = '\xBA';
    PR_CMD[1] = '\x01';
    PR_CMD[2] = '\x00';
    PR_CMD[3] = '\x00';
    PR_CMD[4] = '\x00';
    PR_CMD[5] = '\x03';
    PR_CMD[6] = '\x01';
    PR_CMD[7] = '\x31';
    PR_CMD[8] = '\x00';
    PR_CMD[9] = '\x00';
    PR_CMD[10] = '\x00';
    PR_CMD[11] = '\x00';
    PR_CMD[12] = '\x00';
    PR_CMD[13] = '\x00';
    PR_CMD[14] = '\x00';
    PR_CMD[15] = '\x00';
    PR_CMD[16] = '\x00';
    PR_CMD[17] = '\x00';
    PR_CMD[18] = '\x00';
    PR_CMD[19] = '\x00';
    PR_CMD[20] = '\xFE';

    uint8_t prescaler[2] = {0};
    uint8_t address = 0;
    uint8_t total_pulse[3] = {0};
    uint8_t accelerate_pulse[2] = {0};
    uint8_t break_pulse[2] = {0};
    uint8_t positive_stay[2] = {0};
    uint8_t negative_stay[2] = {0};

    Chararray2Hexarray(Prescaler, prescaler, sizeof(prescaler));
    Chararray2Hexarray(Address, &address, sizeof(address));
    Chararray2Hexarray(TotalPulse, total_pulse, sizeof(total_pulse));
    Chararray2Hexarray(AcceleratePulse, accelerate_pulse, sizeof(accelerate_pulse));
    Chararray2Hexarray(BreakPulse, break_pulse, sizeof(break_pulse));
    Chararray2Hexarray(PositiveStay, positive_stay, sizeof(positive_stay));
    Chararray2Hexarray(NegativeStay, negative_stay, sizeof(negative_stay));
    
    PR_CMD[2] = prescaler[0];
    PR_CMD[3] = prescaler[1];
    PR_CMD[4] = address;
    PR_CMD[8] = total_pulse[0];
    PR_CMD[9] = total_pulse[1];
    PR_CMD[10] = total_pulse[2];
    PR_CMD[11] = accelerate_pulse[0];
    PR_CMD[12] = accelerate_pulse[1];
    PR_CMD[13] = break_pulse[0];
    PR_CMD[14] = break_pulse[1];
    PR_CMD[15] = positive_stay[0];
    PR_CMD[16] = positive_stay[1];
    PR_CMD[17] = negative_stay[0];
    PR_CMD[18] = negative_stay[1];
    PR_CMD[19] = countCRC(PR_CMD, 19);

    _msg.length = 21;
    _msg.content = PR_CMD;

    return &_msg;
}

/**
    BianNeng Motor Controler Status Query Commad Transfer & Transmit
*/
Message* BianNeng::trans2SQCMD(const char* Address, const char* Content)
{
    /*
    static uint8_t SQ_CMD[5] = { '\xB6', '\x00', '\x00', '\x00', '\xFE' };
    */

    SQ_CMD[0] = '\xB6';
    SQ_CMD[1] = '\x00';
    SQ_CMD[2] = '\x00';
    SQ_CMD[3] = '\x00';
    SQ_CMD[4] = '\xFE';

    uint8_t address = 0;
    uint8_t content = 0;

    Chararray2Hexarray(Address, &address, sizeof(address));
    Chararray2Hexarray(Content, &content, sizeof(content));

    SQ_CMD[1] = address;
    SQ_CMD[2] = content;
    SQ_CMD[3] = countCRC(SQ_CMD, 3);

    _msg.length = 5;
    _msg.content = SQ_CMD;

    return &_msg;
}

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
    fprintf(stdout, "[INFO]: RS485Port successfully established %s port\n", _name);
    initThread();
    fprintf(stdout, "[INFO]: RS485Port successfully established thread\n");
}

RS485Port::~RS485Port()
{
    // Release memory if it's needed
    c_serial_free(_port);
    fprintf(stdout, "[INFO]: RS485Port successfully released %s\n", _name);

    // exit thread
    DWORD dwExit;
    GetExitCodeThread(_hThread, &dwExit);
    TerminateThread(_hThread, dwExit);
    CloseHandle(_hThread);
    fprintf(stdout, "[INFO]: RS485Port successfully exited thread\n");

    // finish
    fprintf(stdout, "[INFO]: RS485Port successfully deconstructed\n");
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
    _ghMutex = CreateMutex(NULL, FALSE, (LPCWSTR) _name);
    _threadParameter._ghMutex = &_ghMutex;
    _threadParameter._port = _port;
    _threadParameter._recieveQueue = &_recieveQueue;

    _hThread = NULL;

    _hThread = CreateThread(NULL,                       // default security attributes
                                0,                      // use default stack size
         &RS485Port::ReadWriteMsg,                      // thread function
        (LPVOID)&_threadParameter,                      // argument to thread function
                                0,                      // use default creation flags
                            NULL);                      // returns the thread identifier

    if (_hThread == NULL)
    {
        printf("[ERROR] COMPort: recieveMsg Thread Cannot Be Created\n");
        return false;
    }

    printf("[INFO] COMPort: Create Thread Successed\n");

    return true;
}

Message* RS485Port::getControlerMsg()
{
    if (!_recieveQueue.empty())
    {
        _content[0] = _recieveQueue.front().content[0];
        _content[1] = _recieveQueue.front().content[1];
        _msg.content = _content;
        _msg.length = 2;
        _recieveQueue.erase(_recieveQueue.begin());
        return &_msg;
    }

    return NULL;
}

DWORD WINAPI RS485Port::ReadWriteMsg(LPVOID ThreadParameter)
{
    c_serial_port_t* port = ((Parameter*)ThreadParameter)->_port;
    std::vector<Message>* recieveQueue = ((Parameter*)ThreadParameter)->_recieveQueue;
    Message Reciece;
    uint8_t content[2] = { '\x00' };
    Reciece.content = content;
    Reciece.length = 0;

    while (true)
    {
        c_serial_get_available(port, &Reciece.length);

        if (Reciece.length >= 2)
        {
            //printf("Available Bytes: %d\n", Reciece.length);
            Reciece.length = 2;
            c_serial_read_data(port, content, &Reciece.length, NULL);
            /*
            for (int i = 0; i < Reciece.length; i++)
            {
                printf("%.2X ", content[i]);
            }
            printf("\n");
            */
            WaitForSingleObject(((Parameter*)ThreadParameter)->_ghMutex, INFINITE);
            recieveQueue->push_back(Reciece);
            ReleaseMutex(((Parameter*)ThreadParameter)->_ghMutex);
        }

        Sleep(100);
    }

    return 0;
}