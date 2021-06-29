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
	extern std::vector<std::string> magnetRecordList;
}

namespace MagnetPSUValueToState
{
	// add a converter here 
	extern std::map<std::string, STATE> magnetPSUStrToSTATE;// = { GETSETI, READI, RILK, RPOWER, SETI, SPOWER };
	extern std::map<int, STATE> magnetPSUIntToSTATE;// = { GETSETI, READI, RILK, RPOWER, SETI, SPOWER };
}

namespace MagnetState /* TODO THIS IS ALSO DEFINE IN TH EMAGNET CLASS :(  */ 
//TODO: i don't think this is used anymore, DJS re-done in magnet and magnetFactory objects
{
	struct MagnetSnapshottruct
	{   // proviude a default constructor
		MagnetSnapshottruct() :
			numMags(GlobalConstants::zero_sizet)
			//, 
			//machineArea(HWC_ENUM::MACHINE_AREA::UNKNOWN_AREA) 
		{};
		size_t numMags;
		//HWC_ENUM::MACHINE_AREA machineArea;
		std::vector<std::string> magNames;
		std::vector<STATE> psuStates;
		std::vector<double> siValues, riValues;
//#ifdef BUILD_DLL
//		boost::python::list magNames_Py;
//		boost::python::list psuStates_Py;
//		boost::python::list riValues_Py;
//		boost::python::list siValues_Py;
//#endif
	};
}

#endif