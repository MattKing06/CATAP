#ifndef SHUTTER_RECORDS
#define SHUTTER_RECORDS
#include <string>
#include <vector>

#include "GlobalStateEnums.h"
#include "GlobalConstants.h"


namespace ShutterRecords
{
	/*! Used to search for "Open" key in pv_record_map */
	extern std::string const Open;
	/*! Used to search for "Close" key in pv_record_map */
	extern std::string const Close;
	/*! Used to search for "State" key in pv_record_map */
	extern std::string const State;
	/*! Used to search for "Cmi" key in pv_record_map */
	extern std::string const Cmi;
	extern std::vector<std::string> shutterRecordList;
}

namespace ShutterState
{
	struct ShutterStateStruct
	{   // proviude a default constructor
		ShutterStateStruct() :
			numShutters(GlobalConstants::zero_sizet)
			//, 
			//machineArea(HWC_ENUM::MACHINE_AREA::UNKNOWN_AREA) 
		{};
		size_t numShutters;
	};
}

#endif