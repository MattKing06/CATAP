#ifndef MAGNET_RECORDS
#define MAGNET_RECORDS
#include <string>
#include <vector>

#include "GlobalStateEnums.h"
#include "GlobalConstants.h"


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

namespace MagnetPSUValueToState
{
	// add a converter here 
	
	extern std::map<std::string, STATE> magnetPSUStrToSTATE;// = { GETSETI, READI, RILK, RPOWER, SETI, SPOWER };

	

	extern std::map<int, STATE> magnetPSUIntToSTATE;// = { GETSETI, READI, RILK, RPOWER, SETI, SPOWER };
}




namespace MagnetState
{
	struct magnetStateStruct
	{   // proviude a default constructor
		magnetStateStruct() :
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