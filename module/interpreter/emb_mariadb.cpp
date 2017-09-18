/**********************************************
* @file emb_mariadb.cpp		          	      *
*                                             *
* Python Mariadb Function            		  *
*                                             *
* @author Joshua <jan.joshua@iac.com.tw>      *
***********************************************/

#include "core_common.h"
#include "interpreter_common.h"

#if defined(USING_DB)

// MariaDB Function
PyObject* set_db_query(PyObject *self, PyObject *args)
{
	char* table_ptr;
	char* values_ptr;
	char* constrain_ptr;

	if (!PyArg_ParseTuple(args, "sss", &table_ptr, &values_ptr, &constrain_ptr)) {
		return Py_BuildValue("s", "[ERROR] Microscope XY Table Move Failed");
	}

	std::string command = "db query ";
	command += table_ptr;
	command += " ";
	command += constrain_ptr;
	command += " ";
	command += values_ptr;

	printf("command %s excuting\n", command.c_str());

	rsh_command(command.c_str());

	/*
	printf("%s|%s|%s\n", table_ptr, values_ptr, constrain_ptr);

	splitstring values((char*) values_ptr);
	splitstring constrain((char*) constrain_ptr);

	std::vector<std::string> _values = values.split(';');
	std::vector<std::string> _constrain = constrain.split(';');

	mariadb.Query(table, _constrain, _values);
	mariadb.pubResult();
	*/

	return Py_BuildValue("s", "[INFO] MariaDB Query Finished");
}

#endif