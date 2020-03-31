#ifndef CHARGE_RECORDS
#define CHARGE_RECORDS
#include <string>
#include <vector>

#include "GlobalStateEnums.h"
#include "GlobalConstants.h"


namespace ChargeRecords
{
	/*Defined at std::string const so that the value of the variable cannot be changed.*/
	extern std::string const Q; // = "Q";
	
	extern std::vector<std::string> chargeRecordList;
}

namespace ChargeState
{
	struct chargeStateStruct
	{   // proviude a default constructor
		chargeStateStruct() :
			numCharges(GlobalConstants::zero_sizet)
			//, 
			//machineArea(HWC_ENUM::MACHINE_AREA::UNKNOWN_AREA) 
		{};
		size_t numCharges;
		//HWC_ENUM::MACHINE_AREA machineArea;
		std::vector<std::string> chargeNames;
		std::vector<STATE> chargeStates;
		//#ifdef BUILD_DLL
		//		boost::python::list magNames_Py;
		//		boost::python::list psuStates_Py;
		//		boost::python::list riValues_Py;
		//		boost::python::list siValues_Py;
		//#endif
	};
}

#endif