/*********************************************
* @file core_mariadb.cpp                     *
*                                            *
* Mariadb API                                *
*                                            *
* @author Joshua <jan.joshua@iac.com.tw>     *
*********************************************/

#include "stdafx.h"
#include "core_common.h"
#include "core_mariadb.h"


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
	_field_name.insert(std::make_pair("hospital_info", field));
	_field_name["hospital_info"].push_back("id");
	_field_name["hospital_info"].push_back("name");
	_field_name["hospital_info"].push_back("tele");
	_field_name["hospital_info"].push_back("address");
	_field_name.insert(std::make_pair("image_analysis", field));
	_field_name["image_analysis"].push_back("blood_id");
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
	_field_name["image_analysis"].push_back("df_result_path");
	_field_name["image_analysis"].push_back("spliced_path");
	_field_name["image_analysis"].push_back("fitc_cell_num");
	_field_name["image_analysis"].push_back("dapi_cell_num");
	_field_name["image_analysis"].push_back("df_cell_num");
	_field_name["image_analysis"].push_back("spliced_cell_num");
	_field_name["image_analysis"].push_back("whether_error");
	_field_name["image_analysis"].push_back("picture_id");
	_field_name["image_analysis"].push_back("start_time");
	_field_name["image_analysis"].push_back("finish_time");
	_field_name.insert(std::make_pair("barcode_info", field));
	_field_name["barcode_info"].push_back("name");
	_field_name["barcode_info"].push_back("sex");
	_field_name["barcode_info"].push_back("birthday");
	_field_name["barcode_info"].push_back("patient_num");
	_field_name["barcode_info"].push_back("barcode");
	_field_name["barcode_info"].push_back("cancer_history");
	_field_name["barcode_info"].push_back("cancer_type");
	_field_name["barcode_info"].push_back("diagnose_time");
	_field_name["barcode_info"].push_back("hospital_id");
	_field_name["barcode_info"].push_back("collector");
	_field_name["barcode_info"].push_back("collect_time");
	_field_name["barcode_info"].push_back("sample_state");
	_field_name["barcode_info"].push_back("recieve_time");
	_field_name["barcode_info"].push_back("analysis_time");
	_field_name["barcode_info"].push_back("note");
	_field_name.insert(std::make_pair("microscope_device", field));
	_field_name["microscope_device"].push_back("info_id");
	_field_name["microscope_device"].push_back("start_time");
	_field_name["microscope_device"].push_back("finish_time");
	_field_name["microscope_device"].push_back("led_brightness");
	_field_name["microscope_device"].push_back("exposed_time");
	_field_name["microscope_device"].push_back("x_length");
	_field_name["microscope_device"].push_back("x_step");
	_field_name["microscope_device"].push_back("y_length");
	_field_name["microscope_device"].push_back("y_step");
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
	_field_name.insert(std::make_pair("tubechange_info", field));
	_field_name["reagent_info"].push_back("tube_id1");
	_field_name["reagent_info"].push_back("timestamp1");
	_field_name["reagent_info"].push_back("tube_id2");
	_field_name["reagent_info"].push_back("timestamp2");
	_field_name["reagent_info"].push_back("tube_id3");
	_field_name["reagent_info"].push_back("timestamp3");
	_field_name["reagent_info"].push_back("tube_id4");
	_field_name["reagent_info"].push_back("timestamp4");
	_field_name["reagent_info"].push_back("tube_id5");
	_field_name["reagent_info"].push_back("timestamp5");
	_field_name["reagent_info"].push_back("tube_id6");
	_field_name["reagent_info"].push_back("timestamp6");
	_field_name["reagent_info"].push_back("slide_id");
	_field_name["reagent_info"].push_back("timestamp7");
	_field_name["reagent_info"].push_back("pos");
	_field_name.insert(std::make_pair("slide_info", field));
	_field_name["reagent_info"].push_back("slide_id");
	_field_name["reagent_info"].push_back("finished_stamp");
	_field_name["reagent_info"].push_back("sample_num");
	_field_name["reagent_info"].push_back("tube_id1");
	_field_name["reagent_info"].push_back("tube_id2");
	_field_name["reagent_info"].push_back("tube_id3");
	_field_name["reagent_info"].push_back("tube_id4");
	_field_name["reagent_info"].push_back("tube_id5");
	_field_name["reagent_info"].push_back("tube_id6");
	_field_name["reagent_info"].push_back("tube_id7");
	_field_name["reagent_info"].push_back("tube_id8");
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

bool MySQL_API::Query(std::string Table, std::vector<std::string> Constrain, std::vector<std::string> Value)
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