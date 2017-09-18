/*********************************************
* @file core_mariadb.h                       *
*                                            *
* Mariadb API                                *
*                                            *
* @author Joshua <jan.joshua@iac.com.tw>     *
*********************************************/
#ifndef _CORE_MARIADB_H_
#define _CORE_MARIADB_H_

#include "mysql.h"
#include "core_common.h"

class MySQL_API
{
public:

	/* Constructor & Destructor */
	MySQL_API();
	~MySQL_API();

	/* Initialize Tables' Fields */
	void Init();

	/* Basic Operation*/
	bool Insert(std::string Table, std::vector<std::string> Content);
	bool Query(std::string Table, std::vector<std::string> &Constrain, std::vector<std::string> &Value);

	/* Get Result Search*/
	std::vector<std::string> getField(std::string Table);
	std::map <int, std::vector<std::string>> getResult();
	bool MySQL_API::pubResult();

private:
	// MySQL API Variables
	MYSQL		*_conn;
	MYSQL_FIELD *_field;
	MYSQL_RES	*_result;
	MYSQL_ROW	_row;

	// Return Variables 
	std::map <std::string, std::vector<std::string>>	_field_name;
	std::map <int, std::vector<std::string>>			_query_result;
};

class splitstring : public std::string 
{
public:
	splitstring(char *s) : std::string(s) { };
	std::vector<std::string>& split(char delim, int rep = 0);
private:
	std::vector<std::string> flds;
};

/**
Mutexes For db Thread
*/
typedef struct _dbThreadMutex
{
	bool _dbMutex;
} dbThreadMutex;

bool get_dbMutex();

void rsh_db_mutex_init();

int rsh_db_main(int argc, char *argv[]);

static dbThreadMutex dbMutex;

static MySQL_API mariadb;



#endif // CORE_MARIADB_H_