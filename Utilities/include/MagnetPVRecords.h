#ifndef MAGNET_RECORDS
#define MAGNET_RECORDS
#include <string>
#include <vector>
namespace MagnetRecords
{
	//enum PVs
	//{
	//	GETSETI,
	//	READI,
	//	RILK,
	//	RPOWER,
	//	SETI,
	//	SPOWER
	//};
	/*Defined at std::string const so that the value of the variable cannot be changed.*/
	extern std::string const GETSETI; // = "GETSETI";
	extern std::string const READI; // = "READI";
	extern std::string const RILK; // = "RILK";
	extern std::string const RPOWER; // = "RPOWER";
	extern std::string const SETI; // = "SETI";
	extern std::string const SPOWER; // = "SPOWER";

	extern std::vector<std::string> magnetRecordList;// = { GETSETI, READI, RILK, RPOWER, SETI, SPOWER };
}
#endif