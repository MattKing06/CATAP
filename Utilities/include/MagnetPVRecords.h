#ifndef MAGNET_RECORDS
#define MAGNET_RECORDS
#include <string>
#include <vector>

#include "GlobalStateEnums.h"
#include "GlobalConstants.h"


namespace MagnetRecords
{
	/*Defined at std::string const so that the value of the variable cannot be changed.*/
	extern std::string const GETSETI; // = "GETSETI";
	extern std::string const READI; // = "READI";
	extern std::string const RILK; // = "RILK";
	extern std::string const RPOWER; // = "RPOWER";
	extern std::string const SETI; // = "SETI";
	extern std::string const SPOWER; // = "SPOWER";
	extern std::string const K_DIP_P; // = "GETSETI";
	extern std::string const INT_STR_MM; // = "READI";
	extern std::string const INT_STR; // = "RILK";
	extern std::string const K_SET_P; // = "RPOWER";
	extern std::string const K_ANG; // = "SETI";
	extern std::string const K_MRAD; // = "SPOWER";
	extern std::string const K_VAL; // = "SPOWER";
	extern std::string const ILK_RESET; // = "SPOWER";
	extern std::vector<std::string> magnetRecordList;
	extern std::vector<std::string> magnetMonitorRecordList;

};


#endif