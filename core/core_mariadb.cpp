/*********************************************
* @file core_mariadb.cpp                     *
*                                            *
* Mariadb API                                *
*                                            *
* @author Joshua <jan.joshua@iac.com.tw>     *
*********************************************/
#include "core_common.h"


/* Constructor & Destructor */
MySQL_API::MySQL_API()
{
	_conn = mysql_init(NULL);
	mysql_real_connect(_conn, "localhost", "root", "P@ssw0rd", "ctc", 3306, NULL, 0);
	Init();
};

MySQL_API::~MySQL_API()
{
	_field_name.clear();
	_query_result.clear();
	mysql_free_result(_result);
	mysql_close(_conn);
};

/* Initialize Tables' Fields */
void MySQL_API::Init()
{
	std::vector<std::string> field;
	/* Shared Tables */
	_field_name.insert(std::make_pair("hospital_info", field));
	_field_name["hospital_info"].push_back("id");
	_field_name["hospital_info"].push_back("name");
	_field_name["hospital_info"].push_back("tele");
	_field_name["hospital_info"].push_back("address");
	_field_name.insert(std::make_pair("patient_info", field));
	_field_name["patient_info"].push_back("name");
	_field_name["patient_info"].push_back("sex");
	_field_name["patient_info"].push_back("birthday");
	_field_name["patient_info"].push_back("medical_num");
	_field_name["patient_info"].push_back("cancer_history");
	_field_name["patient_info"].push_back("cancer_type");
	_field_name["patient_info"].push_back("diagnose_date");
	_field_name["patient_info"].push_back("hospital_id");
	_field_name["patient_info"].push_back("collector");
	_field_name["patient_info"].push_back("collect_date");
	_field_name["patient_info"].push_back("collect_time");
	_field_name["patient_info"].push_back("sample_state");
	_field_name["patient_info"].push_back("recieve_date");
	_field_name["patient_info"].push_back("recieve_time");
	_field_name["patient_info"].push_back("analysis_date");
	_field_name["patient_info"].push_back("analysis_time");
	_field_name["patient_info"].push_back("patient_num");
	_field_name["patient_info"].push_back("note");
	_field_name["patient_info"].push_back("ctc_num");
	_field_name.insert(std::make_pair("slide_info", field));
	_field_name["slide_info"].push_back("slide_id");
	_field_name["slide_info"].push_back("finished_stamp");
	_field_name["slide_info"].push_back("sample_num");
	_field_name["slide_info"].push_back("ctc_num_1");
	_field_name["slide_info"].push_back("ctc_num_2");
	_field_name["slide_info"].push_back("ctc_num_3");
	_field_name["slide_info"].push_back("ctc_num_4");
	_field_name["slide_info"].push_back("ctc_num_5");
	_field_name["slide_info"].push_back("ctc_num_6");
	_field_name["slide_info"].push_back("ctc_num_7");
	_field_name["slide_info"].push_back("ctc_num_8");
	/* Analysis Tables */
	_field_name.insert(std::make_pair("single_picture_info", field));
	_field_name["single_picture_info"].push_back("ctc_num");
	_field_name["single_picture_info"].push_back("picture_num");
	_field_name["single_picture_info"].push_back("fitc_cell_num");
	_field_name["single_picture_info"].push_back("dapi_cell_num");
	_field_name["single_picture_info"].push_back("df_cell_num");
	_field_name.insert(std::make_pair("single_picture_cell_info", field));
	_field_name["single_picture_cell_info"].push_back("ctc_num");
	_field_name["single_picture_cell_info"].push_back("picture_num");
	_field_name["single_picture_cell_info"].push_back("x");
	_field_name["single_picture_cell_info"].push_back("y");
	_field_name["single_picture_cell_info"].push_back("length");
	_field_name["single_picture_cell_info"].push_back("height");
	_field_name.insert(std::make_pair("merge_picture_cell_info", field));
	_field_name["merge_picture_cell_info"].push_back("ctc_num");
	_field_name["merge_picture_cell_info"].push_back("x");
	_field_name["merge_picture_cell_info"].push_back("y");
	_field_name["merge_picture_cell_info"].push_back("length");
	_field_name["merge_picture_cell_info"].push_back("height");
	_field_name.insert(std::make_pair("image_analysis", field));
	_field_name["image_analysis"].push_back("ctc_num");
	_field_name["image_analysis"].push_back("dapi_value");
	_field_name["image_analysis"].push_back("fitc_value");
	_field_name["image_analysis"].push_back("dapi_cell_brightness");
	_field_name["image_analysis"].push_back("fitc_cell_brightness");
	_field_name["image_analysis"].push_back("dapi_cell_size");
	_field_name["image_analysis"].push_back("fitc_cell_size");
	_field_name["image_analysis"].push_back("dapi_path");
	_field_name["image_analysis"].push_back("fitc_path");
	_field_name["image_analysis"].push_back("dapi_result_path");
	_field_name["image_analysis"].push_back("fitc_result_path");
	_field_name["image_analysis"].push_back("ctc_result_path");
	_field_name["image_analysis"].push_back("merge_path");
	_field_name["image_analysis"].push_back("merge_fitc_num");
	_field_name["image_analysis"].push_back("merge_dapi_num");
	_field_name["image_analysis"].push_back("merge_ctc_num");
	_field_name["image_analysis"].push_back("start_time");
	_field_name["image_analysis"].push_back("finish_time");
	_field_name.insert(std::make_pair("microscope_device", field));
	_field_name["microscope_device"].push_back("ctc_num");
	_field_name["microscope_device"].push_back("start_time");
	_field_name["microscope_device"].push_back("finish_time");
	_field_name["microscope_device"].push_back("led_brightness");
	_field_name["microscope_device"].push_back("exposed_time");
	_field_name["microscope_device"].push_back("x_length");
	_field_name["microscope_device"].push_back("x_step");
	_field_name["microscope_device"].push_back("y_length");
	_field_name["microscope_device"].push_back("y_step");
	/* Centrifigure Table */
	_field_name.insert(std::make_pair("system_info", field));
	_field_name["system_info"].push_back("info_id");
	_field_name["system_info"].push_back("component");
	_field_name["system_info"].push_back("timestamp");
	_field_name["system_info"].push_back("alert_msg");
	_field_name["system_info"].push_back("tube_id");
	_field_name["system_info"].push_back("process");
	_field_name["system_info"].push_back("action");
	_field_name.insert(std::make_pair("bloodtube_info", field));
	_field_name["bloodtube_info"].push_back("info_id");
	_field_name["bloodtube_info"].push_back("timestamp");
	_field_name["bloodtube_info"].push_back("tube_id");
	_field_name["bloodtube_info"].push_back("cur_pos");
	_field_name["bloodtube_info"].push_back("tar_pos");
	_field_name["bloodtube_info"].push_back("successed_step");
	_field_name["bloodtube_info"].push_back("successed_action");
	_field_name["bloodtube_info"].push_back("cur_step");
	_field_name["bloodtube_info"].push_back("cur_action");
	_field_name["bloodtube_info"].push_back("cur_state");
	_field_name.insert(std::make_pair("reagent_info", field));
	_field_name["reagent_info"].push_back("id");
	_field_name["reagent_info"].push_back("rest");
	_field_name["reagent_info"].push_back("pm1");
	_field_name["reagent_info"].push_back("pm2");
	_field_name["reagent_info"].push_back("sol1");
	_field_name["reagent_info"].push_back("sol2");
	_field_name["reagent_info"].push_back("sol3");
	_field_name["reagent_info"].push_back("r_q");
	_field_name["reagent_info"].push_back("r_r");
	_field_name.insert(std::make_pair("barcode_info", field));
	_field_name["barcode_info"].push_back("ctc_num");
	_field_name["barcode_info"].push_back("timestamp1");
	_field_name["barcode_info"].push_back("tube_id2");
	_field_name["barcode_info"].push_back("timestamp2");
	_field_name["barcode_info"].push_back("tube_id3");
	_field_name["barcode_info"].push_back("timestamp3");
	_field_name["barcode_info"].push_back("tube_id4");
	_field_name["barcode_info"].push_back("timestamp4");
	_field_name["barcode_info"].push_back("tube_id5");
	_field_name["barcode_info"].push_back("timestamp5");
	_field_name["barcode_info"].push_back("tube_id6");
	_field_name["barcode_info"].push_back("timestamp6");
	_field_name["barcode_info"].push_back("slide_id");
	_field_name["barcode_info"].push_back("timestamp7");
	_field_name["barcode_info"].push_back("pos");
	
};

/* Basic Operation*/
bool MySQL_API::Insert(std::string Table, std::vector<std::string> Content)
{
	std::string query = "INSERT INTO " + Table + " (";
	for (unsigned int i = 0; i < _field_name[Table].size(); i++)
	{
		if ((i + 1) != _field_name[Table].size())
		{
			query += _field_name[Table][i] + ", ";
		}
		else
		{
			query += _field_name[Table][i] + ") VALUES (";
		}
	}

	for (unsigned int i = 0; i < Content.size(); i++)
	{
		if ((i + 1) != Content.size())
		{
			query += "'" + Content[i] + "', ";
		}
		else
		{
			query += "'" + Content[i] + "');";
		}
	}

	//std::cout << query << std::endl;

	if (!mysql_query(_conn, query.c_str()))
	{
		//std::cout << "Yes" << std::endl;
		return true;
	}

	return false;
};

bool MySQL_API::Query(std::string Table, std::vector<std::string> &Constrain, std::vector<std::string> &Value)
{
	std::string query = "SELECT * FROM " + Table;

	if (Constrain.size() == 0)
	{
		query += ";";
	}
	else
	{
		query += " WHERE ";

		for (unsigned int i = 0; i < Constrain.size(); i++)
		{
			if ((i + 1) != Constrain.size())
			{
				query += Constrain[i] + " = '" + Value[i] + "' AND ";
			}
			else
			{
				query += Constrain[i] + " = '" + Value[i] + "';";
			}
		}
	}

	//std::cout << query << std::endl;

	if (!mysql_query(_conn, query.c_str()))
	{
		//std::cout << "Yes" << std::endl;

		_result = mysql_store_result(_conn);

		int count = 0;
		while ((_row = mysql_fetch_row(_result)) != NULL)
		{
			std::vector<std::string> row;
			for (unsigned int i = 0; i < _field_name[Table].size(); i++)
			{
				row.push_back(_row[i]);
			}
			_query_result.insert(std::make_pair(count, row));
			count += 1;
		}

		return true;
	}

	return false;
};

/* Get Result Search*/
std::vector<std::string> MySQL_API::getField(std::string Table)
{
	return _field_name[Table];
};

std::map <int, std::vector<std::string>> MySQL_API::getResult()
{
	return _query_result;
};

bool MySQL_API::pubResult()
{
	if (_query_result.size() > 0)
	{
		for (int i = 0; i < _query_result.size(); i++)
		{
			std::string row = "";
			for (int j = 0; j < _query_result[i].size(); j++)
			{
				row += _query_result[i][j];
				if (i != (_query_result[i].size() - 1))
				{
					row += " ";
				}
			}
			mqtt_publish("query_result", row.length(), row.c_str());
			Sleep(1000);
		}
	}

	return true;
};

// split: receives a char delimiter; returns a vector of strings
// By default ignores repeated delimiters, unless argument rep == 1.
std::vector<std::string>& splitstring::split(char delim, int rep) 
{
	if (!flds.empty())
	{
		flds.clear();  // empty vector if necessary
	}
	
	std::string work = data();
	std::string buf = "";
	
	int i = 0;
	
	while (i < work.length()) 
	{
		if (work[i] != delim)
		{
			buf += work[i];
		}
		else if (rep == 1) 
		{
			flds.push_back(buf);
			buf = "";
		}
		else if (buf.length() > 0) 
		{
			flds.push_back(buf);
			buf = "";
		}
		i++;
	}
	
	if (!buf.empty())
	{
		flds.push_back(buf);
	}
	
	return flds;
}

bool get_dbMutex()
{
	return dbMutex._dbMutex;
}

void rsh_db_mutex_init()
{
	static bool init = false;

	if (!init)
	{
		dbMutex._dbMutex = false;
		init = true;
	}
}

#if defined(USING_DB)
// rsh db main
int rsh_db_main(int argc, char *argv[])
{
	rsh_db_mutex_init();

	if (!strcmp(argv[1], "db"))
	{
		while (dbMutex._dbMutex)
		{
			Sleep(200);
		}

		dbMutex._dbMutex = true;
	}

	if (!strcmp(argv[1], "query"))
	{
		if (argc != 3 && argc != 5)
		{
			log_info("error");
			return 0;
		}

		if (argc == 3)
		{
			std::vector<std::string> _values;
			std::vector<std::string> _constrain;
			mariadb.Query(argv[2], _constrain, _values);
			//mariadb.pubResult();
			return 0;
		}
		else
		{
			splitstring values(argv[3]);
			splitstring constrain(argv[4]);

			std::vector<std::string> _values = values.split(';');
			std::vector<std::string> _constrain = constrain.split(';');
			mariadb.Query(argv[2], _constrain, _values);
			//mariadb.pubResult();
			return 0;
		}
	}


	return 0;
}
#endif