#ifndef GLOBAL_CONSTANTS_H_
#define GLOBAL_CONSTANTS_H_
#include <limits>
#include <map>
#include <string>
#include <chrono>
#include "GlobalTypeEnums.h"
#include "GlobalStateEnums.h"


// where i sthe best to put this ??
// http://forums.codeguru.com/showthread.php?369447-can-t-assign-numeric_limits-lt-double-gt-max()
#ifdef max
#undef max
#endif
#ifdef min
#undef min
#endif

/*! @defgroup utils Utilities
 *  @brief A collection of classes for providing often used functions, constants, and enums throughout CATAP.
 *  @{*/
/*!
	Namespace to define constants that are used throughout CATAP for ease-of-use.
*/

namespace GlobalConstants 
{
	const std::map<std::string, TYPE> stringToTypeMap =
	{
		// dummy, default types
			{ENUM_TO_STRING(TYPE::UNKNOWN_TYPE), TYPE::UNKNOWN_TYPE},
			{ENUM_TO_STRING(TYPE::HORIZONTAL), TYPE::HORIZONTAL},
			{ENUM_TO_STRING(TYPE::VERTICAL), TYPE::VERTICAL},
				// Machine Areas
				{ENUM_TO_STRING(TYPE::UNKNOWN_AREA), TYPE::UNKNOWN_AREA},
				{ENUM_TO_STRING(TYPE::ALL_VELA_CLARA), TYPE::ALL_VELA_CLARA},
				{ENUM_TO_STRING(TYPE::BA1), TYPE::BA1},
				{ENUM_TO_STRING(TYPE::BA2), TYPE::BA2},
				{ENUM_TO_STRING(TYPE::VELA), TYPE::VELA},
				{ENUM_TO_STRING(TYPE::VELA_GUN), TYPE::VELA_GUN},
				{ENUM_TO_STRING(TYPE::CLARA_GUN), TYPE::CLARA_GUN},
				{ENUM_TO_STRING(TYPE::HRRG_GUN), TYPE::HRRG_GUN},
				{ENUM_TO_STRING(TYPE::LRRG_GUN), TYPE::LRRG_GUN},
				{ENUM_TO_STRING(TYPE::L01), TYPE::L01},
				{ENUM_TO_STRING(TYPE::S01), TYPE::S01},
				{ENUM_TO_STRING(TYPE::S02), TYPE::S02},
				{ENUM_TO_STRING(TYPE::C2V), TYPE::C2V},
				{ENUM_TO_STRING(TYPE::CLARA_PH1), TYPE::CLARA_PH1},
				{ENUM_TO_STRING(TYPE::CLARA_2_BA1), TYPE::CLARA_2_BA1},
				{ENUM_TO_STRING(TYPE::CLARA_2_BA1_BA2), TYPE::CLARA_2_BA1_BA2},
				// Magnets 
				{ENUM_TO_STRING(TYPE::MAGNET), TYPE::MAGNET},
				{ENUM_TO_STRING(TYPE::QUADRUPOLE), TYPE::QUADRUPOLE},
				{ENUM_TO_STRING(TYPE::DIPOLE), TYPE::DIPOLE},
				{ENUM_TO_STRING(TYPE::HVCOR), TYPE::HVCOR},
				{ENUM_TO_STRING(TYPE::VCOR), TYPE::VCOR},
				{ENUM_TO_STRING(TYPE::HCOR), TYPE::HCOR},
				{ENUM_TO_STRING(TYPE::SOLENOID), TYPE::SOLENOID},
				// screens
				{ENUM_TO_STRING(TYPE::VELA_PNEUMATIC), TYPE::VELA_PNEUMATIC},
				{ENUM_TO_STRING(TYPE::VELA_HV_MOVER), TYPE::VELA_HV_MOVER},
				{ENUM_TO_STRING(TYPE::VELA_V_MOVER), TYPE::VELA_V_MOVER},
				{ENUM_TO_STRING(TYPE::CLARA_HV_MOVER), TYPE::CLARA_HV_MOVER},
				{ENUM_TO_STRING(TYPE::CLARA_V_MOVER), TYPE::CLARA_V_MOVER},
				{ENUM_TO_STRING(TYPE::CLARA_PNEUMATIC), TYPE::CLARA_PNEUMATIC},
				{ENUM_TO_STRING(TYPE::PNEUMATIC), TYPE::PNEUMATIC},
				{ENUM_TO_STRING(TYPE::UNKNOWN_SCREEN_TYPE), TYPE::UNKNOWN_SCREEN_TYPE}

		};
	


	
	/*! Minimum possible value of a double */
	const double double_min = std::numeric_limits<double>::min();
	/*! Maximum possible value of a double */
	const double double_max = std::numeric_limits<double>::max();

	/*! Double-type variable for 0.0 */
	const double zero_double = 0.0;
	/*! Double-type variable for 1.0 */
	const double one_double = 1.0;
	/*! Double-type variable for 0.1 */
	const double zero_point_one_double = 0.1;

	/*! Unsigned short-type variable for 0 */
	const unsigned short zero_ushort = 0;
	/*! Unsigned short-type variable for 1 */
	const unsigned short one_ushort = 1;

	/*! Integer-type variable for 0 */
	const int zero_int= 0;
	/*! Integer-type variable for 1 */
	const int one_int = 1;

	//const epicsUInt16 zero_epicsUInt16= 0;
	//const epicsUInt16 one_epicsUInt16 = 1;



	/*! Unsigned Integer-type for 0 */
	const size_t zero_sizet = 0;
	/*! Unsigned Integer-type for 1 */
	const size_t one_sizet = 1;
	const size_t eighty_sizet = 80;

	/*! Unsigned Short-type to represent ACTIVATE to EPICS */
	const unsigned short EPICS_ACTIVATE = 1;
	/*! Unsigned Short-type to represent SEND to EPICS */
	const unsigned short EPICS_SEND = 0;
	/*! Unsigned Short-type to represent RESET to EPICS after ACTIVATE and SEND have been sent */
	const unsigned short EPICS_RESET = 1;

	/*! 2 second pause */
	const std::chrono::milliseconds STANDARD_PAUSE(2000);
	/*! 1 second pause */
	const std::chrono::milliseconds PAUSE_1000(1000);
	/*! 0.5 second pause */
	const std::chrono::milliseconds PAUSE_500(500);
	/*! 0.3 second pause */
	const std::chrono::milliseconds PAUSE_300(300);
	/*! 0.05 second pause */
	const std::chrono::milliseconds PAUSE_50(50);
	/*! 0.002 second pause */
	const std::chrono::milliseconds PAUSE_2(2);
	/*! 0.001 second pause */
	const std::chrono::milliseconds PAUSE_1(1);

	/*! Arithmetic time-type capable of representing time 0 */
	const time_t TIMET_0 = 0;
	/*! Arithmetic time-type capable of representing time 45 */
	const time_t TIMET_45 = 45;
	/*! Arithmetic time-type capable of representing time 60 */
	const time_t TIMET_60 = 60;

	const std::string COMMENT = "COMMENT";
	const std::string TIMESTAMP = "TIMESTAMP";
	const std::string HEADER = "HEADER";
	const std::string FILE_ALIAS = "FILE_ALIAS";

	const std::string DBURT_FILEPATH = "\\\\fed.cclrc.ac.uk\\org\\NLab\\ASTeC\\Projects\\VELA\\Snapshots\\DBURT\\";
	const std::string DUMMY_NAME = "DUMMY_NAME";


	// when calling setup for a factory we pass "version" variable, 
	// so far this is not used practically, but we are keeping it  in case fo futre need
	const std::string nominal = "nominal";

}
/*! @}*/
#endif // GLOBAL_CONSTANTS_H_
