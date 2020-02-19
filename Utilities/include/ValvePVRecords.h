#ifndef VALVE_RECORDS
#define VALVE_RECORDS
#include <string>
#include <vector>

#include "GlobalStateEnums.h"
#include "GlobalConstants.h"

/*! @addtogroup utils*/
/*!@{*/

/*!
	Namespace to define keys of "pv_record_map" found in MasterLattice Config Files for Valves.
*/
namespace ValveRecords
{
	/*Defined at std::string const so that the value of the variable cannot be changed.*/

	/*! Used to search for "On" key in pv_record_map */
	extern std::string const On;
	/*! Used to search for "Off" key in pv_record_map */
	extern std::string const Off;
	/*! Used to search for "Sta" key in pv_record_map */
	extern std::string const Sta;


	extern std::vector<std::string> valveRecordList;
}
/*! @}*/

/*! @addtogroup utils*/
/*!@{*/

/*!
	Namespace that holds the valveStateStruct
*/
namespace ValveState
{
	/*! Used to store states (OPEN,CLOSED,ERR) for multiple Valve objects. */
	struct valveStateStruct
	{   // proviude a default constructor
		valveStateStruct() :
			numValves(GlobalConstants::zero_sizet){};
		size_t numValves;
		std::vector<std::string> valveNames;
		std::vector<STATE> valveStates;
	};
}
/*! @}*/
#endif