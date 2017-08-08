/**********************************************
 * @file core_bianneng.cpp                    *
 *                                            *
 * BianNeng Controler Command Format Transfer *
 *                                            *
 * @author Joshua <jan.joshua@iac.com.tw>     *
 **********************************************/

#include "core_common.h"
#include "core_bianneng.h"

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