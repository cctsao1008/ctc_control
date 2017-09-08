/**********************************************
 * @file core_bianneng.h                      *
 *                                            *
 * BianNeng Controler Command Format Transfer *
 *                                            *
 * @author Joshua <jan.joshua@iac.com.tw>     *
 **********************************************/

#ifndef _CORE_BIANNENG_H_
#define _CORE_BIANNENG_H_

#include "core_common.h"

#define Prescaler254Hz	256
#define Prescaler256Hz	1000
#define Prescaler261Hz	2000
#define Prescaler265Hz	3000
#define Prescaler269Hz	4000
#define Prescaler274Hz	5000
#define Prescaler279Hz	6000
#define Prescaler283Hz	7000
#define Prescaler288Hz	8000
#define Prescaler293Hz	9000
#define Prescaler299Hz	10000
#define Prescaler304Hz	11000
#define Prescaler310Hz	12000
#define Prescaler316Hz	13000
#define Prescaler322Hz	14000
#define Prescaler328Hz	15000
#define Prescaler335Hz	16000
#define Prescaler342Hz	17000
#define Prescaler350Hz	18000
#define Prescaler356Hz	19000
#define Prescaler363Hz	20000
#define Prescaler372Hz	21000
#define Prescaler381Hz	22000
#define Prescaler390Hz	23000
#define Prescaler400Hz	24000
#define Prescaler409Hz	25000
#define Prescaler420Hz	26000
#define Prescaler431Hz	27000
#define Prescaler441Hz	28000
#define Prescaler454Hz	29000
#define Prescaler466Hz	30000
#define Prescaler480Hz	31000
#define Prescaler495Hz	32000
#define Prescaler510Hz	33000
#define Prescaler527Hz	34000
#define Prescaler544Hz	35000
#define Prescaler561Hz	36000
#define Prescaler582Hz	37000
#define Prescaler602Hz	38000
#define Prescaler625Hz	39000
#define Prescaler650Hz	40000
#define Prescaler675Hz	41000
#define Prescaler704Hz	42000
#define Prescaler735Hz	43000
#define Prescaler769Hz	44000
#define Prescaler807Hz	45000
#define Prescaler848Hz	46000
#define Prescaler891Hz	47000
#define Prescaler940Hz	48000
#define Prescaler997Hz	49000
#define Prescaler1060Hz 50000
#define Prescaler1130Hz 51000
#define Prescaler1220Hz 52000
#define Prescaler1310Hz 53000
#define Prescaler1430Hz 54000
#define Prescaler1560Hz 55000
#define Prescaler1720Hz 56000
#define Prescaler1920Hz 57000
#define Prescaler2170Hz 58000
#define Prescaler2500Hz 59000
#define Prescaler2900Hz 60000
#define Prescaler3570Hz 61000
#define Prescaler4540Hz 62000
#define Prescaler6200Hz 63000


#define RPM38	Prescaler256Hz
#define RPM39	Prescaler261Hz
#define RPM40	Prescaler274Hz
#define RPM42	Prescaler279Hz
#define RPM43	Prescaler288Hz
#define RPM44	Prescaler293Hz
#define RPM45	Prescaler299Hz
#define RPM46	Prescaler304Hz
#define RPM47	Prescaler316Hz
#define RPM48	Prescaler322Hz
#define RPM49	Prescaler328Hz
#define RPM50	Prescaler335Hz
#define RPM51	Prescaler342Hz
#define RPM53	Prescaler356Hz
#define RPM54	Prescaler363Hz
#define RPM56	Prescaler372Hz
#define RPM57	Prescaler381Hz
#define RPM59	Prescaler390Hz
#define RPM60	Prescaler400Hz
#define RPM61	Prescaler409Hz
#define RPM63	Prescaler420Hz
#define RPM65	Prescaler431Hz
#define RPM66	Prescaler441Hz
#define RPM68	Prescaler454Hz
#define RPM70	Prescaler466Hz
#define RPM72	Prescaler480Hz
#define RPM74	Prescaler495Hz
#define RPM77	Prescaler510Hz
#define RPM79	Prescaler527Hz
#define RPM82	Prescaler544Hz
#define RPM84	Prescaler561Hz
#define RPM87	Prescaler582Hz
#define RPM90	Prescaler602Hz
#define RPM94	Prescaler625Hz
#define RPM98	Prescaler650Hz
#define RPM101	Prescaler675Hz
#define RPM106	Prescaler704Hz
#define RPM110	Prescaler735Hz
#define RPM115	Prescaler769Hz
#define RPM121	Prescaler807Hz
#define RPM127	Prescaler848Hz
#define RPM134	Prescaler891Hz
#define RPM141	Prescaler940Hz
#define RPM150	Prescaler997Hz
#define RPM159	Prescaler1060Hz
#define RPM170	Prescaler1130Hz
#define RPM183	Prescaler1220Hz
#define RPM197	Prescaler1310Hz
#define RPM215  Prescaler1430Hz
#define	RPM234	Prescaler1560Hz
#define	RPM258	Prescaler1720Hz
#define	RPM288	Prescaler1920Hz
#define	RPM326	Prescaler2170Hz
#define	RPM375	Prescaler2500Hz
#define	RPM435	Prescaler2900Hz
#define	RPM536	Prescaler3570Hz
#define	RPM681	Prescaler4540Hz
#define	RPM930	Prescaler6200Hz

#define Prescaler1066Hz 50085	// estimate Prescaler
#define Prescaler1193Hz 51700	// estimate Prescaler
#define Prescaler1355Hz 53375	// estimate Prescaler
#define Prescaler1460Hz 54230	// estimate Prescaler
#define RPM160	Prescaler1066Hz	// estimate Prescaler
#define RPM180	Prescaler1193Hz	// estimate Prescaler
#define RPM200	Prescaler1355Hz	// estimate Prescaler
#define RPM220  Prescaler1460Hz // estimate Prescaler

#define PositiveExecute '\x01'
#define NegativeExecute '\x02'
#define StopExecute		'\x03'

/**
Motor Driver Message Structure
*/
typedef struct _message {
	int length;
	uint8_t* content;
} Message;

/**
Queue For Thread As Parameter
*/
typedef struct _parameter {
	HANDLE*					_ghMutex;
	c_serial_port_t*		_port;
	std::vector<Message>*	_recieveQueue;
	std::vector<uint8_t>* _unknownAnswer;				  // try
	std::vector<uint8_t>* _unknownAddress;				  // try
	std::map<uint8_t, std::vector<Message>>* _recieveMap; // try
} Parameter;

/**
Convert Number tTo String
*/
template <typename T> std::string NumberToString(T Number)
{
	std::ostringstream ss;
	ss << Number;
	return ss.str();
}

// BianNeng Motor Controler
class BianNeng
{

public:

	// Constructor
	BianNeng();

	// Destructor
	~BianNeng();

	// Command Transfer
	void Chararray2Hexarray(const char * Input, uint8_t * Output, int Length);
	uint8_t countCRC(uint8_t * Data, int Length);
	Message* trans2RTCMD(const char* Prescaler, const uint8_t Address, char Direction
		, const char* TotalPulse, const char* AcceleratePulse, const char* BreakPulse);
	Message* trans2PRCMD(const char* Prescaler, const uint8_t Address, const char* TotalPulse
		, const char* AcceleratePulse, const char* BreakPulse
		, const char* PositiveStay, const char* NegativeStay);
	Message* trans2SQCMD(const uint8_t Address, const char* Content);

private:
	// Private Variable
	Message _msg;
	uint8_t* RT_CMD;
	uint8_t* PR_CMD;
	uint8_t* SQ_CMD;
};

static BianNeng* bianneng;
#endif // _CORE_BIANNENG_H_