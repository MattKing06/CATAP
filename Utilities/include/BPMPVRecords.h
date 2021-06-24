#ifndef BPM_RECORDS
#define BPM_RECORDS
#include <string>
#include <vector>

#include "GlobalStateEnums.h"
#include "GlobalConstants.h"


namespace BPMRecords
{
	/*Defined at std::string const so that the value of the variable cannot be changed.*/
	extern std::string const X; // = "X";
	extern std::string const Y; // = "Y";
	extern std::string const RA1; // = "RA1";
	extern std::string const RA2; // = "RA2";
	extern std::string const RD1; // = "RD1";
	extern std::string const RD2; // = "RD2";
	extern std::string const SA1; // = "SA1";
	extern std::string const SA2; // = "SA2";
	extern std::string const SD1; // = "SD1";
	extern std::string const SD2; // = "SD2";
	extern std::string const AWAK; // = "AWAK";
	extern std::string const RDY; // = "RDY";
	extern std::string const DATA; // = "DATA:B2V.VALA";

	extern std::vector<std::string> bpmRecordList;
	extern std::vector<std::string> bpmRecordListVirtual;
}

namespace BPMState
{
	struct bpmStateStruct
	{   // proviude a default constructor
		bpmStateStruct() :
			numBPMs(GlobalConstants::zero_sizet)
			//, 
			//machineArea(HWC_ENUM::MACHINE_AREA::UNKNOWN_AREA) 
		{};
		size_t numBPMs;
		//HWC_ENUM::MACHINE_AREA machineArea;
		std::vector<std::string> bpmNames;
		std::vector<STATE> bpmStates;
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
