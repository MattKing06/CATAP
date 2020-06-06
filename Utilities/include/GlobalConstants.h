#ifndef GLOBAL_CONSTANTS_H_
#define GLOBAL_CONSTANTS_H_
#include <limits>
#include <map>
#include <string>
#include <chrono>
#include "GlobalTypeEnums.h"
#include "GlobalStateEnums.h"

// where is the best to put this ??
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
		{ENUM_TO_STRING(TYPE::UNKNOWN_TYPE), TYPE::UNKNOWN_TYPE},

		// Machine areas 
		{ENUM_TO_STRING(TYPE::UNKNOWN_AREA), TYPE::UNKNOWN_AREA},
		{ENUM_TO_STRING(TYPE::ALL_VELA_CLARA), TYPE::ALL_VELA_CLARA},
		{ENUM_TO_STRING(TYPE::BA1), TYPE::BA1},
		{ENUM_TO_STRING(TYPE::BA2), TYPE::BA2},
		{ENUM_TO_STRING(TYPE::VELA), TYPE::VELA},
		{ENUM_TO_STRING(TYPE::VELA_GUN), TYPE::VELA_GUN},
		{ENUM_TO_STRING(TYPE::CLARA_GUN), TYPE::CLARA_GUN},
		{ENUM_TO_STRING(TYPE::HRRG_GUN), TYPE::HRRG_GUN},
		{ENUM_TO_STRING(TYPE::LRRG_GUN), TYPE::LRRG_GUN},
		{ENUM_TO_STRING(TYPE::GUN), TYPE::GUN},
		{ENUM_TO_STRING(TYPE::L01), TYPE::L01},
		{ENUM_TO_STRING(TYPE::S01), TYPE::S01},
		{ENUM_TO_STRING(TYPE::S02), TYPE::S02},
		{ENUM_TO_STRING(TYPE::C2V), TYPE::C2V},
		{ENUM_TO_STRING(TYPE::INJ), TYPE::INJ},
		{ENUM_TO_STRING(TYPE::CLARA_PH1), TYPE::CLARA_PH1},
		{ENUM_TO_STRING(TYPE::CLARA_2_BA1), TYPE::CLARA_2_BA1},
		{ENUM_TO_STRING(TYPE::CLARA_2_BA1_BA2), TYPE::CLARA_2_BA1_BA2},
		{ENUM_TO_STRING(TYPE::LAS), TYPE::LAS},
		{ENUM_TO_STRING(TYPE::CLARA_LASER), TYPE::CLARA_LASER},
		{ENUM_TO_STRING(TYPE::VELA_LASER), TYPE::VELA_LASER},
		{ENUM_TO_STRING(TYPE::VELA_INJ), TYPE::VELA_INJ},
		{ENUM_TO_STRING(TYPE::VELA_2_BA1), TYPE::VELA_2_BA1},
		{ENUM_TO_STRING(TYPE::VELA_2_BA1_BA2), TYPE::VELA_2_BA1_BA2},
						
		// Hardware types
		{ENUM_TO_STRING(TYPE::MAGNET), TYPE::MAGNET},
		{"Magnet", TYPE::MAGNET},
		{ENUM_TO_STRING(TYPE::VALVE), TYPE::VALVE},
		{"Valve", TYPE::VALVE},
		{ENUM_TO_STRING(TYPE::CAMERA_TYPE), TYPE::CAMERA_TYPE},
		{"Camera", TYPE::CAMERA_TYPE},
		{"CAMERA", TYPE::CAMERA_TYPE},
		{"camera", TYPE::CAMERA_TYPE},

		{ENUM_TO_STRING(TYPE::BPM_TYPE), TYPE::BPM_TYPE},
		{"BPM", TYPE::BPM_TYPE},
		{"bpm", TYPE::BPM_TYPE},
		{"IMG", TYPE::IMG_TYPE},

		{ENUM_TO_STRING(TYPE::IMG_TYPE), TYPE::IMG_TYPE},
		{"IMG", TYPE::IMG_TYPE},
		{"img", TYPE::IMG_TYPE},

		{ENUM_TO_STRING(TYPE::CHARGE), TYPE::CHARGE},
		{"Charge", TYPE::CHARGE},
		{"charge", TYPE::CHARGE},

		{ENUM_TO_STRING(TYPE::SCREEN), TYPE::SCREEN},
		{"Screen", TYPE::SCREEN},
		{"screen", TYPE::SCREEN},


				{ENUM_TO_STRING(TYPE::VALVE), TYPE::VALVE},
				{"Valve", TYPE::VALVE},

				{ENUM_TO_STRING(TYPE::BPM_TYPE), TYPE::BPM_TYPE},
				{"BPM", TYPE::BPM_TYPE},
				
				{"IMG", TYPE::IMG_TYPE},
				{ENUM_TO_STRING(TYPE::IMG_TYPE), TYPE::IMG_TYPE},
				
				{"Charge", TYPE::CHARGE},
				{ENUM_TO_STRING(TYPE::CHARGE), TYPE::CHARGE},
			
				{"Screen", TYPE::SCREEN},
				{ENUM_TO_STRING(TYPE::SCREEN), TYPE::SCREEN},
				
				{ENUM_TO_STRING(TYPE::LLRF_TYPE), TYPE::LLRF_TYPE},
				{"LLRF", TYPE::LLRF_TYPE},
				
				//{ENUM_TO_STRING(TYPE::HVCOR), TYPE::HVCOR},
				{ENUM_TO_STRING(TYPE::VERTICAL_CORRECTOR), TYPE::VERTICAL_CORRECTOR},
				{ENUM_TO_STRING(TYPE::HORIZONTAL_CORRECTOR), TYPE::HORIZONTAL_CORRECTOR},
				{ENUM_TO_STRING(TYPE::QUADRUPOLE), TYPE::QUADRUPOLE},
				{ENUM_TO_STRING(TYPE::DIPOLE), TYPE::DIPOLE},
				{ENUM_TO_STRING(TYPE::SOLENOID), TYPE::SOLENOID},
				{ENUM_TO_STRING(TYPE::BUCKING_SOLENOID), TYPE::BUCKING_SOLENOID},
				{ENUM_TO_STRING(TYPE::NR), TYPE::NR},
				{ENUM_TO_STRING(TYPE::BIPOLAR), TYPE::BIPOLAR},
				{ENUM_TO_STRING(TYPE::NR_GANGED), TYPE::NR_GANGED},
				{ENUM_TO_STRING(TYPE::POSITIVE_ONLY), TYPE::POSITIVE_ONLY},
				{ENUM_TO_STRING(TYPE::VELA_PNEUMATIC), TYPE::VELA_PNEUMATIC},
				{ENUM_TO_STRING(TYPE::VELA_HV_MOVER), TYPE::VELA_HV_MOVER},
				{ENUM_TO_STRING(TYPE::VELA_V_MOVER), TYPE::VELA_V_MOVER},
				{ENUM_TO_STRING(TYPE::CLARA_HV_MOVER), TYPE::CLARA_HV_MOVER},
				{ENUM_TO_STRING(TYPE::CLARA_V_MOVER), TYPE::CLARA_V_MOVER},
				{ENUM_TO_STRING(TYPE::CLARA_PNEUMATIC), TYPE::CLARA_PNEUMATIC},
				{ENUM_TO_STRING(TYPE::PNEUMATIC), TYPE::PNEUMATIC},
				{ENUM_TO_STRING(TYPE::UNKNOWN_SCREEN_TYPE), TYPE::UNKNOWN_SCREEN_TYPE}
	};

	const std::map<std::string, STATE> stringToSateMap =
	{
			{ENUM_TO_STRING(STATE::OFF), STATE::OFF},
			{ENUM_TO_STRING(STATE::UNKNOWN_NAME), STATE::UNKNOWN_NAME},
			{ENUM_TO_STRING(STATE::SUCCESS), STATE::SUCCESS},
			{ENUM_TO_STRING(STATE::FAIL), STATE::FAIL},
			{ENUM_TO_STRING(STATE::OFFLINE), STATE::OFFLINE},
			{ENUM_TO_STRING(STATE::PHYSICAL), STATE::PHYSICAL},
			{ENUM_TO_STRING(STATE::VIRTUAL), STATE::VIRTUAL},
			{ENUM_TO_STRING(STATE::ERR), STATE::ERR},
			{ENUM_TO_STRING(STATE::OK), STATE::OK},
			{ENUM_TO_STRING(STATE::GOOD), STATE::GOOD},
			{ENUM_TO_STRING(STATE::BAD), STATE::BAD},
			{ENUM_TO_STRING(STATE::OPEN), STATE::OPEN},
			{ENUM_TO_STRING(STATE::CLOSED), STATE::CLOSED},
			{ENUM_TO_STRING(STATE::NONLINEAR), STATE::NONLINEAR},
			{ENUM_TO_STRING(STATE::UNKNOWN), STATE::UNKNOWN},
			{ENUM_TO_STRING(STATE::ENABLED), STATE::ENABLED},
			{ENUM_TO_STRING(STATE::DISABLED), STATE::DISABLED},
			{ENUM_TO_STRING(STATE::HRETRACTED), STATE::HRETRACTED},
			{ENUM_TO_STRING(STATE::HSLIT1), STATE::HSLIT1},
			{ENUM_TO_STRING(STATE::HSLIT2), STATE::HSLIT2},
			{ENUM_TO_STRING(STATE::HSLIT3), STATE::HSLIT3},
			// Magnets 
			{ENUM_TO_STRING(STATE::HAPT1), STATE::HAPT1},
			{ENUM_TO_STRING(STATE::HAPT2), STATE::HAPT2},
			{ENUM_TO_STRING(STATE::HAPT3), STATE::HAPT3},
			{ENUM_TO_STRING(STATE::VRETRACTED), STATE::VRETRACTED},
			{ENUM_TO_STRING(STATE::VMAX), STATE::VMAX},
			{ENUM_TO_STRING(STATE::VRF), STATE::VRF},
			{ENUM_TO_STRING(STATE::VMIRROR), STATE::VMIRROR},
			// screens
			{ENUM_TO_STRING(STATE::VYAG), STATE::VYAG},
			{ENUM_TO_STRING(STATE::VGRAT), STATE::VGRAT},
			{ENUM_TO_STRING(STATE::VCOL), STATE::VCOL},
			{ENUM_TO_STRING(STATE::VSLIT1), STATE::VSLIT1},
			{ENUM_TO_STRING(STATE::RETRACTED), STATE::RETRACTED},
			{ENUM_TO_STRING(STATE::YAG), STATE::YAG},
			{ENUM_TO_STRING(STATE::RF), STATE::RF},
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
	/*! Unsigned Integer-type for 2 */
	const size_t two_sizet = 2;
	/*! Unsigned Integer-type for 80 */
	const size_t eighty_sizet = 80;
	/*! Unsigned Integer-type for 999999 */
	const size_t nine99999 = 999999;



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


	const std::string nominal = "nominal";
	
	



	const std::string LRRG_CAVITY_REVERSE_PHASE = "LRRG_CAVITY_REVERSE_PHASE";
	const std::string HRRG_CAVITY_REVERSE_PHASE = "HRRG_CAVITY_REVERSE_PHASE";
	const std::string L01_CAVITY_REVERSE_PHASE = "L01_CAVITY_REVERSE_PHASE";
	const std::string CAVITY_REVERSE_PHASE = "CAVITY_REVERSE_PHASE";

	const std::string LRRG_CAVITY_FORWARD_PHASE = "LRRG_CAVITY_FORWARD_PHASE";
	const std::string HRRG_CAVITY_FORWARD_PHASE = "HRRG_CAVITY_FORWARD_PHASE";
	const std::string L01_CAVITY_FORWARD_PHASE = "L01_CAVITY_FORWARD_PHASE";
	const std::string CAVITY_FORWARD_PHASE = "CAVITY_FORWARD_PHASE";

	const std::string LRRG_CAVITY_REVERSE_POWER = "LRRG_CAVITY_REVERSE_POWER";
	const std::string HRRG_CAVITY_REVERSE_POWER = "HRRG_CAVITY_REVERSE_POWER";
	const std::string L01_CAVITY_REVERSE_POWER = "L01_CAVITY_REVERSE_POWER";
	const std::string CAVITY_REVERSE_POWER = "CAVITY_REVERSE_POWER";

	const std::string LRRG_CAVITY_FORWARD_POWER = "LRRG_CAVITY_FORWARD_POWER";
	const std::string HRRG_CAVITY_FORWARD_POWER = "HRRG_CAVITY_FORWARD_POWER";
	const std::string L01_CAVITY_FORWARD_POWER = "L01_CAVITY_FORWARD_POWER";
	const std::string CAVITY_FORWARD_POWER = "CAVITY_FORWARD_POWER";

	const std::string CAVITY_PROBE_POWER = "CAVITY_PROBE_POWER";
	const std::string HRRG_CAVITY_PROBE_POWER = "HRRG_CAVITY_PROBE_POWER";
	const std::string L01_CAVITY_PROBE_POWER = "L01_CAVITY_PROBE_POWER";

	const std::string CAVITY_PROBE_PHASE = "CAVITY_PROBE_PHASE";
	const std::string HRRG_CAVITY_PROBE_PHASE = "HRRG_CAVITY_PROBE_PHASE";
	const std::string L01_CAVITY_PROBE_PHASE = "L01_CAVITY_PROBE_PHASE";

	const std::string KLYSTRON_REVERSE_PHASE = "KLYSTRON_REVERSE_PHASE";
	const std::string KLYSTRON_REVERSE_POWER = "KLYSTRON_REVERSE_POWER";
	const std::string KLYSTRON_FORWARD_PHASE = "KLYSTRON_FORWARD_PHASE";
	const std::string KLYSTRON_FORWARD_POWER = "KLYSTRON_FORWARD_POWER";


	const std::string  KRPOW = "KRPOW";
	const std::string  KRPHA = "KRPHA";
	const std::string  KFPOW = "KFPOW";
	const std::string  KFPHA = "KFPHA";
	const std::string  CRPOW = "CRPOW";
	const std::string  CRPHA = "CRPHA";
	const std::string  CPPOW = "CPPOW";
	const std::string  CPPHA = "CPPHA";
	const std::string  CFPOW = "CFPOW";
	const std::string  CFPHA = "CFPHA";


}
/*! @}*/
#endif // GLOBAL_CONSTANTS_H_
