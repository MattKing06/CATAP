#ifndef VALVE_RECORDS
#define VALVE_RECORDS
#include <string>
#include <vector>

#include "GlobalStateEnums.h"
#include "GlobalConstants.h"


namespace ValveRecords
{
	/*Defined at std::string const so that the value of the variable cannot be changed.*/
	extern std::string const On;
	extern std::string const Off;
	extern std::string const Sta;


	extern std::vector<std::string> valveRecordList;
}

namespace ValveState
{
	struct valveStateStruct
	{   // proviude a default constructor
		valveStateStruct() :
			numValves(GlobalConstants::zero_sizet){};
		size_t numValves;
		std::vector<std::string> valveNames;
		std::vector<STATE> valveStates;
	};
}

#endif