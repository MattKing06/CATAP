#ifndef LASER_HWP_RECORDS
#define LASER_HWP_RECORDS
#include <string>
#include <vector>

#include "GlobalStateEnums.h"
#include "GlobalConstants.h"


namespace LaserHWPRecords
{
	/*Defined at std::string const so that the value of the variable cannot be changed.*/
	extern std::string const RPOS; // = "RA2";
	extern std::string const MABS; // = "RD1";

	extern std::vector<std::string> laserRecordList;
	extern std::map<std::string, TYPE> laserTypeList;
	extern std::vector<std::string> laserHWPRecordList;
	extern std::map<std::string, TYPE> laserHWPList;
	extern std::map<std::string, TYPE> laserHWPTypeToEnum;
}

namespace LaserHWPState
{
	struct laserHWPStateStruct
	{   // proviude a default constructor
		laserHWPStateStruct() :
			numLasers(GlobalConstants::zero_sizet)
			//, 
			//machineArea(HWC_ENUM::MACHINE_AREA::UNKNOWN_AREA) 
		{};
		size_t numLasers;
		//HWC_ENUM::MACHINE_AREA machineArea;
		std::vector<std::string> laserNames;
		std::vector<STATE> laserStates;
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