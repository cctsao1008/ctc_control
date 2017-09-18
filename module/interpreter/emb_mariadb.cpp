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

	std::vector<std::string> values;
	std::vector<std::string> constrain;
	mariadb.Query("hospital_info", constrain, values);
	mariadb.pubResult();
	return Py_BuildValue("s", "[INFO] MariaDB Query Finished");
}

#endif