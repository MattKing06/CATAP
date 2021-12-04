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

		{ENUM_TO_STRING(TYPE::HRRG), TYPE::HRRG},
		{ENUM_TO_STRING(TYPE::LRRG), TYPE::LRRG},
		{ENUM_TO_STRING(TYPE::CLARA_HRRG), TYPE::CLARA_HRRG},
		{ENUM_TO_STRING(TYPE::CLARA_LRRG), TYPE::CLARA_LRRG},
		{ENUM_TO_STRING(TYPE::VELA_HRRG), TYPE::VELA_HRRG},
		{ENUM_TO_STRING(TYPE::VELA_LRRG), TYPE::VELA_LRRG},

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

		// TODO 
		// do we need this many >>> probably get rid of LAS and LASER,
		{ENUM_TO_STRING(TYPE::LAS), TYPE::LAS},
		{ENUM_TO_STRING(TYPE::LASER), TYPE::LASER},
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

		// camera stuff 
		{ENUM_TO_STRING(TYPE::CAMERA_TYPE), TYPE::CAMERA_TYPE},
		{"Camera", TYPE::CAMERA_TYPE},
		{"CAMERA", TYPE::CAMERA_TYPE},
		{"camera", TYPE::CAMERA_TYPE},
		{ENUM_TO_STRING(TYPE::CLARA_CAMERA), TYPE::CLARA_CAMERA},
		{ENUM_TO_STRING(TYPE::VELA_CAMERA), TYPE::VELA_CAMERA},
		{"CLARA_CAM", TYPE::CLARA_CAMERA},
		{"VELA_CAM", TYPE::VELA_CAMERA},
		{"clara_cam", TYPE::CLARA_CAMERA},
		{"vela_cam", TYPE::VELA_CAMERA},

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

		{ENUM_TO_STRING(TYPE::SHUTTER), TYPE::SHUTTER},
		{"Shutter", TYPE::SHUTTER},
		{"shutter", TYPE::SHUTTER},

		{ENUM_TO_STRING(TYPE::SCREEN), TYPE::SCREEN},
		{"Screen", TYPE::SCREEN},
		{"screen", TYPE::SCREEN},

		{ENUM_TO_STRING(TYPE::LASER_MIRROR), TYPE::LASER_MIRROR},
		{"LaserMirror", TYPE::LASER_MIRROR},
		{"laser_mirror", TYPE::LASER_MIRROR},
				
		{ENUM_TO_STRING(TYPE::LLRF_TYPE), TYPE::LLRF_TYPE},
		{"LLRF", TYPE::LLRF_TYPE},
		{"llrf", TYPE::LLRF_TYPE},
		
		{ENUM_TO_STRING(TYPE::LINAC_PID), TYPE::LINAC_PID},
		{"LinacPID", TYPE::LINAC_PID},
		{"linacPID", TYPE::LINAC_PID},
		{"LINAC_PID", TYPE::LINAC_PID},

		// RF PROTECTION
		{ENUM_TO_STRING(TYPE::RF_PROTECTION), TYPE::RF_PROTECTION},
		{"RFProtection", TYPE::RF_PROTECTION},
		{"rfprotection", TYPE::RF_PROTECTION},
		{ENUM_TO_STRING(TYPE::GENERAL), TYPE::GENERAL},
		{ENUM_TO_STRING(TYPE::ENABLE), TYPE::ENABLE},
		{ENUM_TO_STRING(TYPE::RESET), TYPE::RESET},
		{ENUM_TO_STRING(TYPE::TEST), TYPE::TEST},


		{ENUM_TO_STRING(TYPE::ENERGYMETER), TYPE::ENERGYMETER},
		{"LaserEnergyMeter", TYPE::ENERGYMETER},
		{"LEM", TYPE::ENERGYMETER},

		{ENUM_TO_STRING(TYPE::HWP), TYPE::HWP},
		{"LaserHWP", TYPE::HWP},
		{"LHW", TYPE::HWP},
		
		
		{ ENUM_TO_STRING(TYPE::LIGHTING), TYPE::LIGHTING},
		{ "Lighting", TYPE::LIGHTING},

		// magnet types
		{ENUM_TO_STRING(TYPE::QUADRUPOLE), TYPE::QUADRUPOLE},
		{ENUM_TO_STRING(TYPE::DIPOLE), TYPE::DIPOLE},
		{ENUM_TO_STRING(TYPE::CORRECTOR), TYPE::CORRECTOR},
		{ENUM_TO_STRING(TYPE::VERTICAL_CORRECTOR), TYPE::VERTICAL_CORRECTOR},
		{ENUM_TO_STRING(TYPE::HORIZONTAL_CORRECTOR), TYPE::HORIZONTAL_CORRECTOR},
		{ENUM_TO_STRING(TYPE::SOLENOID), TYPE::SOLENOID},
		{ENUM_TO_STRING(TYPE::BUCKING_SOLENOID), TYPE::BUCKING_SOLENOID},
		// magnet PSU reverse types
		{ENUM_TO_STRING(TYPE::NR), TYPE::NR},
		{ENUM_TO_STRING(TYPE::BIPOLAR), TYPE::BIPOLAR},
		{ENUM_TO_STRING(TYPE::NR_GANGED), TYPE::NR_GANGED},
		{ENUM_TO_STRING(TYPE::POSITIVE_ONLY), TYPE::POSITIVE_ONLY},
		// screen
		{ENUM_TO_STRING(TYPE::VELA_PNEUMATIC), TYPE::VELA_PNEUMATIC},
		{ENUM_TO_STRING(TYPE::VELA_HV_MOVER), TYPE::VELA_HV_MOVER},
		{ENUM_TO_STRING(TYPE::VELA_V_MOVER), TYPE::VELA_V_MOVER},
		{ENUM_TO_STRING(TYPE::CLARA_HV_MOVER), TYPE::CLARA_HV_MOVER},
		{ENUM_TO_STRING(TYPE::CLARA_V_MOVER), TYPE::CLARA_V_MOVER},
		{ENUM_TO_STRING(TYPE::CLARA_PNEUMATIC), TYPE::CLARA_PNEUMATIC},
		{ENUM_TO_STRING(TYPE::PNEUMATIC), TYPE::PNEUMATIC},
		{ENUM_TO_STRING(TYPE::UNKNOWN_SCREEN_TYPE), TYPE::UNKNOWN_SCREEN_TYPE},

		{ENUM_TO_STRING(TYPE::HORIZONTAL), TYPE::HORIZONTAL},
		{ENUM_TO_STRING(TYPE::VERTICAL), TYPE::VERTICAL},


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

		{"Stage", TYPE::STAGE},
		{ENUM_TO_STRING(TYPE::STAGE), TYPE::STAGE},
				
		{ENUM_TO_STRING(TYPE::LLRF_TYPE), TYPE::LLRF_TYPE},
		{"LLRF", TYPE::LLRF_TYPE},
		{ ENUM_TO_STRING(TYPE::POWER), TYPE::POWER },
		{ ENUM_TO_STRING(TYPE::PHASE), TYPE::PHASE },
		
		{ "RFModulator", TYPE::RF_MODULATOR },
		{ ENUM_TO_STRING(TYPE::RF_MODULATOR), TYPE::RF_MODULATOR },

				
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
		{ENUM_TO_STRING(TYPE::UNKNOWN_SCREEN_TYPE), TYPE::UNKNOWN_SCREEN_TYPE},
		{ENUM_TO_STRING(TYPE::RFHEARTBEAT), TYPE::RFHEARTBEAT},
		{"RFHeartbeat", TYPE::RFHEARTBEAT}

	};

	const std::map<std::string, STATE> stringToStateMap =
	{
			{ENUM_TO_STRING(STATE::ON), STATE::ON},
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
			{ENUM_TO_STRING(STATE::MOVING), STATE::MOVING},
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

			{ENUM_TO_STRING(STATE::ACQUIRING), STATE::ACQUIRING},
			{ENUM_TO_STRING(STATE::NOT_ACQUIRING), STATE::NOT_ACQUIRING},
			{ENUM_TO_STRING(STATE::ANALYSING), STATE::ANALYSING},
			{ENUM_TO_STRING(STATE::CAPTURING), STATE::CAPTURING},
			{ENUM_TO_STRING(STATE::WRITING), STATE::WRITING},
			{ENUM_TO_STRING(STATE::NOT_WRITING), STATE::NOT_WRITING},
			//{ENUM_TO_STRING(STATE::SAVING), STATE::SAVING},
			//{ENUM_TO_STRING(STATE::NOT_SAVING), STATE::NOT_SAVING},
			{ENUM_TO_STRING(STATE::WRITE_CHECK_OK), STATE::WRITE_CHECK_OK},
			{ENUM_TO_STRING(STATE::WRITE_CHECK_ERROR), STATE::WRITE_CHECK_ERROR},
			{ENUM_TO_STRING(STATE::TIMEOUT), STATE::TIMEOUT},
			{ENUM_TO_STRING(STATE::USING_FLOOR), STATE::USING_FLOOR},
			{ENUM_TO_STRING(STATE::NOT_USING_FLOOR), STATE::NOT_USING_FLOOR},
			{ENUM_TO_STRING(STATE::USING_NPOINT), STATE::USING_NPOINT},
			{ENUM_TO_STRING(STATE::NOT_USING_NPOINT), STATE::NOT_USING_NPOINT},
			{ENUM_TO_STRING(STATE::USING_BACKGROUND), STATE::USING_BACKGROUND},
			{ENUM_TO_STRING(STATE::NOT_USING_BACKGROUND), STATE::NOT_USING_BACKGROUND},
			{ENUM_TO_STRING(STATE::ACTIVE), STATE::ACTIVE},
			{ENUM_TO_STRING(STATE::NOT_ACTIVE), STATE::NOT_ACTIVE},
			{ENUM_TO_STRING(STATE::INTERNAL), STATE::INTERNAL},
			{ENUM_TO_STRING(STATE::EXTERNAL), STATE::EXTERNAL},
			{ENUM_TO_STRING(STATE::PASSIVE), STATE::PASSIVE},
			{ENUM_TO_STRING(STATE::EVENT), STATE::EVENT},
			{ENUM_TO_STRING(STATE::IO_INTR), STATE::IO_INTR},
			{ENUM_TO_STRING(STATE::TEN), STATE::TEN},
			{ENUM_TO_STRING(STATE::FIVE), STATE::FIVE},
			{ENUM_TO_STRING(STATE::TWO), STATE::TWO},
			{ENUM_TO_STRING(STATE::ONE), STATE::ONE},
			{ENUM_TO_STRING(STATE::ZERO_POINT_FIVE), STATE::ZERO_POINT_FIVE},
			{ENUM_TO_STRING(STATE::ZERO_POINT_TWO), STATE::ZERO_POINT_TWO},
			{ENUM_TO_STRING(STATE::ZERO_POINT_ONE), STATE::ZERO_POINT_ONE},
			{ENUM_TO_STRING(STATE::ZERO_POINT_ZERO_FIVE), STATE::ZERO_POINT_ZERO_FIVE},
			{ENUM_TO_STRING(STATE::NOW), STATE::NOW},
			{ENUM_TO_STRING(STATE::EXTERNAL), STATE::EXTERNAL},
			{ENUM_TO_STRING(STATE::INTERNAL), STATE::INTERNAL},
			{ENUM_TO_STRING(STATE::ACTIVE), STATE::ACTIVE},
			{ENUM_TO_STRING(STATE::NOT_ACTIVE), STATE::NOT_ACTIVE},

			{ENUM_TO_STRING(STATE::YES), STATE::YES},
			{ENUM_TO_STRING(STATE::NO), STATE::NO},


			// gun mod
			{ENUM_TO_STRING(STATE::OFF_REQUEST), STATE::OFF_REQUEST},
			{ENUM_TO_STRING(STATE::HV_INTERLOCK), STATE::HV_INTERLOCK},
			{ENUM_TO_STRING(STATE::STANDBY_REQUEST), STATE::STANDBY_REQUEST},
			{ENUM_TO_STRING(STATE::STANDBY), STATE::STANDBY},
			{ENUM_TO_STRING(STATE::HV_OFF_REQUEST), STATE::HV_OFF_REQUEST},
			{ENUM_TO_STRING(STATE::RF_ON_INTERLOCK), STATE::RF_ON_INTERLOCK},
			{ENUM_TO_STRING(STATE::HV_REQUEST), STATE::HV_REQUEST},
			{ENUM_TO_STRING(STATE::HV_ON), STATE::HV_ON},
			{ENUM_TO_STRING(STATE::RF_OFF_REQUEST), STATE::RF_OFF_REQUEST},
			{ENUM_TO_STRING(STATE::RF_ON_REQUEST), STATE::RF_ON_REQUEST},
			{ENUM_TO_STRING(STATE::RF_ON), STATE::RF_ON},
			{ENUM_TO_STRING(STATE::GUN_MOD_STATE_SET), STATE::GUN_MOD_STATE_SET},
			{ENUM_TO_STRING(STATE::SET_OFF), STATE::SET_OFF},
			{ENUM_TO_STRING(STATE::SET_STANDBY), STATE::SET_STANDBY},
			{ENUM_TO_STRING(STATE::SET_HV_ON), STATE::SET_HV_ON},
			{ENUM_TO_STRING(STATE::SET_RF_ON), STATE::SET_RF_ON},
			{ENUM_TO_STRING(STATE::UNKNOWN_SET_STATE), STATE::UNKNOWN_SET_STATE},
			{ENUM_TO_STRING(STATE::HOLD_RF_ON), STATE::HOLD_RF_ON},
			{ ENUM_TO_STRING(STATE::HOLD_RF_ON_CON), STATE::HOLD_RF_ON_CON},
			{ ENUM_TO_STRING(STATE::MANUAL_OPERATION), STATE::MANUAL_OPERATION },
			{ ENUM_TO_STRING(STATE::CLEAR_FAULT_LOG), STATE::CLEAR_FAULT_LOG}
	};






	/*! Minimum possible value of a double */
	const double double_min = std::numeric_limits<double>::min();
	/*! Maximum possible value of a double */
	const double double_max = std::numeric_limits<double>::max();

	/*! Minimum possible value of a int */
	const int int_min = std::numeric_limits<int>::min();
	/*! Maximum possible value of a int */
	const int int_max = std::numeric_limits<int>::max();


	/*! Double-type variable for 0.0 */
	const double zero_double = 0.0;
	/*! Double-type variable for 1.0 */
	const double one_double = 1.0;
	const double ten_double = 10.0;
	/*! Double-type variable for 0.1 */
	const double zero_point_one_double = 0.1;
	
	const double one_hundred_double = 100.0;
	const double one_hundred_and_eighty_double = 180.0;

	/*! Unsigned short-type variable for 0 */
	const unsigned short zero_ushort = 0;
	/*! Unsigned short-type variable for 1 */
	const unsigned short one_ushort = 1;
	/*! Unsigned short-type variable for 2 */
	const unsigned short two_ushort = 2;
	/*! Unsigned short-type variable for 3 */
	const unsigned short three_ushort = 3;
	/*! Unsigned short-type variable for 4 */
	const unsigned short four_ushort = 4;
	/*! Unsigned short-type variable for 5 */
	const unsigned short five_ushort = 5;
	/*! Unsigned short-type variable for 6 */
	const unsigned short six_ushort = 6;
	/*! Unsigned short-type variable for 7 */
	const unsigned short seven_ushort = 7;
	/*! Unsigned short-type variable for 8 */
	const unsigned short eight_ushort = 8;
	/*! Unsigned short-type variable for 9 */
	const unsigned short nine_ushort = 9;
	/*! Unsigned short-type variable for 10 */
	const unsigned short ten_ushort = 10;
	/*! Unsigned short-type variable for 11 */
	const unsigned short eleven_ushort = 11;
	/*! Unsigned short-type variable for 12 */
	const unsigned short twelve_ushort = 12;

	//// epics versions 
	///*! epicsUInt16 variable for 0 */
	//const epicsUInt16 zero_epicsUInt16 = 0;
	///*! epicsUInt16 variable for 1 */
	//const epicsUInt16 one_epicsUInt16 = 1;


	/*! Minimum possible value of a unsigned short */
	const unsigned short ushort_min = std::numeric_limits<unsigned short>::min();
	/*! Maximum possible value of a unsigned short */
	const unsigned short ushort_max = std::numeric_limits<unsigned short>::max();

	/*! Minimum possible value of a long */
	const long long_min = std::numeric_limits<unsigned short>::min();
	/*! Maximum possible value of a long */
	const long long_max = std::numeric_limits<unsigned short>::max();

	/*! Maximum possible value of a long */
	const size_t size_zero = 0;

	/*! Integer-type variable for 0 */
	const int zero_int= 0;
	/*! Integer-type variable for 1 */
	const int one_int = 1;
	/*! Integer-type variable for 1 */
	const int two_int = 2;

	/*! Minimum possible value of a double */
	const char char_min = std::numeric_limits<char>::min();


	/*! Unsigned size_t-type for 0 */
	const size_t zero_sizet = 0;
	/*! Unsigned size_t-type for 1 */
	const size_t one_sizet = 1;
	/*! Unsigned size_t-type for 2 */
	const size_t two_sizet = 2;
	/*! Unsigned size_t-type for 3 */
	const size_t three_sizet = 3;
	/*! Unsigned size_t-type for 5 */
	const size_t five_sizet = 5;
	/*! Unsigned size_t-type for 12 */
	const size_t twelve_sizet = 12;
	/*! Unsigned size_t-type for 80 */
	const size_t eighty_sizet = 80;
	/*! Unsigned size_t-type for 100 */
	const size_t one_hundred_sizet = 100;
	/*! Unsigned Integer-type for 999999 */
	const size_t nine99999 = 999999;
	const size_t sizet_max = std::numeric_limits<size_t>::max();

	const long zero_long = 0;
	const long one_long = 1;
	
	const unsigned int zero_uint = 0;
	const unsigned int one_uint = 1;
	const unsigned int two_uint = 2;
	const unsigned int three_uint = 3;

	const unsigned short zero_us = 0;
	const unsigned short one_us = 1;
	const unsigned short two_us = 2;
	const unsigned short three_us = 3;

	/*! Unsigned Short-type to represent ACTIVATE to EPICS */
	const unsigned short EPICS_ACTIVATE = one_us;
	/*! Unsigned Short-type to represent SEND to EPICS */
	const unsigned short EPICS_SEND = zero_us;
	/*! Unsigned Short-type to represent RESET to EPICS after ACTIVATE and SEND have been sent */
	const unsigned short EPICS_RESET = one_us;

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



	const std::string UNKNOWN = "UNKNOWN";
	const std::string nominal = "nominal";
	// MSVC DOES NOT LIKE CONST STRING TRUE/FALSE SO HAD TO RENAME THIS VAR //
	const std::string TRUE_STR  = "TRUE";
	const std::string FALSE_STR  = "FALSE";
	
	


	// legacy magnet dburt stuff 
	const std::string VELA_CLARA_DBURT_ALIAS_V1 = "VELA-CLARA DBURT ALIAS FILE v1";
	const std::string DBURT_HEADER_V2 = "VELA DBURT (MAGNET SAVE FILE) v2.0";
	const std::string VELA_MAGNET_SAVE_FILE_v1 = "VELA MAGNET SAVE FILE v1.0"; 
	const std::string DBURT_HEADER_V3 = "VELA-CLARA DBURT (MAGNET SAVE FILE) v3";
	const std::string DBURT_HEADER_V4 = "VELA-CLARA DBURT (MAGNET SAVE FILE) v4";
	const std::string dotdburt= ".dburt";
	const std::string dotDBURT = ".DBURT";
	const std::string DBURT_HEADER_DT = "DATE_TIME:";
	const std::string DBURT_HEADER_COM = "COMMENTS:";
	const std::string DBURT_HEADER_AREA = "BEAM_AREA:";
	const std::string DBURT_HEADER_KEYW = "KEY_WORDS:";
	const std::string COMMENT = "COMMENT";
	const std::string TIMESTAMP = "TIMESTAMP";
	const std::string HEADER = "HEADER";
	const std::string FILE_ALIAS = "FILE_ALIAS";
	const std::string DBURT_FILEPATH = "\\\\fed.cclrc.ac.uk\\org\\NLab\\ASTeC\\Projects\\VELA\\Snapshots\\DBURT\\";
	const std::string DUMMY_NAME = "DUMMY_NAME";
	const std::string DBURT_EOF_V1 = "END OF FILE";
	const std::string DBURT_EOF_V3 = "END_OF_DATA"; 
	const std::string DBURT_EOF_V4 = "END_OF_DATA";
	const std::string END_OF_LINE = ";";
	const char EQUALS_SIGN_C = '=';
	const char COLON_C = ':';
	const char DOUBLE_QUOTE_C = '"';
	const char SPACE_C = ' ';
	const char TAB_C = '\t';
	const char COMMA_C = ',';
	const char SLASH_SLASH_C = '\\';
	const std::string SLASH_SLASH = "\\";
	// end







	// LLRF stuff to sort out  TODO MOVE the LLRFRECORDS

	//const std::string LRRG_CAVITY_REVERSE_PHASE = "LRRG_CAVITY_REVERSE_PHASE";
	//const std::string HRRG_CAVITY_REVERSE_PHASE = "HRRG_CAVITY_REVERSE_PHASE";
	//const std::string L01_CAVITY_REVERSE_PHASE = "L01_CAVITY_REVERSE_PHASE";
	//const std::string CAVITY_REVERSE_PHASE = "CAVITY_REVERSE_PHASE";

	//const std::string LRRG_CAVITY_FORWARD_PHASE = "LRRG_CAVITY_FORWARD_PHASE";
	//const std::string HRRG_CAVITY_FORWARD_PHASE = "HRRG_CAVITY_FORWARD_PHASE";
	//const std::string L01_CAVITY_FORWARD_PHASE = "L01_CAVITY_FORWARD_PHASE";
	//const std::string CAVITY_FORWARD_PHASE = "CAVITY_FORWARD_PHASE";

	//const std::string LRRG_CAVITY_REVERSE_POWER = "LRRG_CAVITY_REVERSE_POWER";
	//const std::string HRRG_CAVITY_REVERSE_POWER = "HRRG_CAVITY_REVERSE_POWER";
	//const std::string L01_CAVITY_REVERSE_POWER = "L01_CAVITY_REVERSE_POWER";
	//const std::string CAVITY_REVERSE_POWER = "CAVITY_REVERSE_POWER";

	//const std::string LRRG_CAVITY_FORWARD_POWER = "LRRG_CAVITY_FORWARD_POWER";
	//const std::string HRRG_CAVITY_FORWARD_POWER = "HRRG_CAVITY_FORWARD_POWER";
	//const std::string L01_CAVITY_FORWARD_POWER = "L01_CAVITY_FORWARD_POWER";
	//const std::string CAVITY_FORWARD_POWER = "CAVITY_FORWARD_POWER";

	//const std::string CAVITY_PROBE_POWER = "CAVITY_PROBE_POWER";
	//const std::string HRRG_CAVITY_PROBE_POWER = "HRRG_CAVITY_PROBE_POWER";
	//const std::string L01_CAVITY_PROBE_POWER = "L01_CAVITY_PROBE_POWER";

	//const std::string CAVITY_PROBE_PHASE = "CAVITY_PROBE_PHASE";
	//const std::string HRRG_CAVITY_PROBE_PHASE = "HRRG_CAVITY_PROBE_PHASE";
	//const std::string L01_CAVITY_PROBE_PHASE = "L01_CAVITY_PROBE_PHASE";

	//const std::string KLYSTRON_REVERSE_PHASE = "KLYSTRON_REVERSE_PHASE";
	//const std::string KLYSTRON_REVERSE_POWER = "KLYSTRON_REVERSE_POWER";
	//const std::string KLYSTRON_FORWARD_PHASE = "KLYSTRON_FORWARD_PHASE";
	//const std::string KLYSTRON_FORWARD_POWER = "KLYSTRON_FORWARD_POWER";


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



	//const std::string CH1_AMP_DER = "CH1_AMP_DER";
	//const std::string CH2_AMP_DER = "CH2_AMP_DER";
	//const std::string CH3_AMP_DER = "CH3_AMP_DER";
	//const std::string CH4_AMP_DER = "CH4_AMP_DER";
	//const std::string CH5_AMP_DER = "CH5_AMP_DER";
	//const std::string CH6_AMP_DER = "CH6_AMP_DER";
	//const std::string CH7_AMP_DER = "CH7_AMP_DER";
	//const std::string CH8_AMP_DER = "CH8_AMP_DER";
	//const std::string CH1_PHASE_DER = "CH1_PHASE_DER";
	//const std::string CH2_PHASE_DER = "CH2_PHASE_DER";
	//const std::string CH3_PHASE_DER = "CH3_PHASE_DER";
	//const std::string CH4_PHASE_DER = "CH4_PHASE_DER";
	//const std::string CH5_PHASE_DER = "CH5_PHASE_DER";
	//const std::string CH6_PHASE_DER = "CH6_PHASE_DER";
	//const std::string CH7_PHASE_DER = "CH7_PHASE_DER";
	//const std::string CH8_PHASE_DER = "CH8_PHASE_DER";
	//const std::string CH1_PHASE_REM = "CH1_PHASE_REM";
	//const std::string CH2_PHASE_REM = "CH2_PHASE_REM";
	//const std::string CH3_PHASE_REM = "CH3_PHASE_REM";
	//const std::string CH4_PHASE_REM = "CH4_PHASE_REM";
	//const std::string CH5_PHASE_REM = "CH5_PHASE_REM";
	//const std::string CH6_PHASE_REM = "CH6_PHASE_REM";
	//const std::string CH7_PHASE_REM = "CH7_PHASE_REM";
	//const std::string CH8_PHASE_REM = "CH8_PHASE_REM";
	//const std::string CH1_PWR_LOC = "CH1_PWR_LOC";
	//const std::string CH2_PWR_LOC = "CH2_PWR_LOC";
	//const std::string CH3_PWR_LOC = "CH3_PWR_LOC";
	//const std::string CH4_PWR_LOC = "CH4_PWR_LOC";
	//const std::string CH5_PWR_LOC = "CH5_PWR_LOC";
	//const std::string CH6_PWR_LOC = "CH6_PWR_LOC";
	//const std::string CH7_PWR_LOC = "CH7_PWR_LOC";
	//const std::string CH8_PWR_LOC = "CH8_PWR_LOC";
	//const std::string CH1_PWR_REM = "CH1_PWR_REM";
	//const std::string CH2_PWR_REM = "CH2_PWR_REM";
	//const std::string CH3_PWR_REM = "CH3_PWR_REM";
	//const std::string CH4_PWR_REM = "CH4_PWR_REM";
	//const std::string CH5_PWR_REM = "CH5_PWR_REM";
	//const std::string CH6_PWR_REM = "CH6_PWR_REM";
	//const std::string CH7_PWR_REM = "CH7_PWR_REM";
	//const std::string CH8_PWR_REM = "CH8_PWR_REM";



	//const std::string ONE_RECORD_KLYSTRON_FORWARD_POWER = "ONE_RECORD_KLYSTRON_FORWARD_POWER";
	//const std::string ONE_RECORD_KLYSTRON_FORWARD_PHASE = "ONE_RECORD_KLYSTRON_FORWARD_PHASE";
	//const std::string ONE_RECORD_KLYSTRON_REVERSE_POWER = "ONE_RECORD_KLYSTRON_REVERSE_POWER";
	//const std::string ONE_RECORD_KLYSTRON_REVERSE_PHASE = "ONE_RECORD_KLYSTRON_REVERSE_PHASE";
	//const std::string ONE_RECORD_LRRG_CAVITY_FORWARD_POWER = "ONE_RECORD_LRRG_CAVITY_FORWARD_POWER";
	//const std::string ONE_RECORD_LRRG_CAVITY_FORWARD_PHASE = "ONE_RECORD_LRRG_CAVITY_FORWARD_PHASE";
	//const std::string ONE_RECORD_LRRG_CAVITY_REVERSE_POWER = "ONE_RECORD_LRRG_CAVITY_REVERSE_POWER";
	//const std::string ONE_RECORD_LRRG_CAVITY_REVERSE_PHASE = "ONE_RECORD_LRRG_CAVITY_REVERSE_PHASE";
	//const std::string ONE_RECORD_HRRG_CAVITY_FORWARD_POWER = "ONE_RECORD_HRRG_CAVITY_FORWARD_POWER";
	//const std::string ONE_RECORD_HRRG_CAVITY_FORWARD_PHASE = "ONE_RECORD_HRRG_CAVITY_FORWARD_PHASE";
	//const std::string ONE_RECORD_HRRG_CAVITY_REVERSE_POWER = "ONE_RECORD_HRRG_CAVITY_REVERSE_POWER";
	//const std::string ONE_RECORD_HRRG_CAVITY_REVERSE_PHASE = "ONE_RECORD_HRRG_CAVITY_REVERSE_PHASE";
	//const std::string ONE_RECORD_HRRG_CAVITY_PROBE_POWER   = "ONE_RECORD_HRRG_CAVITY_PROBE_POWER";
	//const std::string ONE_RECORD_HRRG_CAVITY_PROBE_PHASE   = "ONE_RECORD_HRRG_CAVITY_PROBE_PHASE";

	//const std::string ONE_RECORD_L01_CAVITY_FORWARD_POWER = "ONE_RECORD_L01_CAVITY_FORWARD_POWER";
	//const std::string ONE_RECORD_L01_CAVITY_FORWARD_PHASE = "ONE_RECORD_L01_CAVITY_FORWARD_PHASE";
	//const std::string ONE_RECORD_L01_CAVITY_REVERSE_POWER = "ONE_RECORD_L01_CAVITY_REVERSE_POWER";
	//const std::string ONE_RECORD_L01_CAVITY_REVERSE_PHASE = "ONE_RECORD_L01_CAVITY_REVERSE_PHASE";
	//const std::string ONE_RECORD_L01_CAVITY_PROBE_POWER   = "ONE_RECORD_L01_CAVITY_PROBE_POWER";
	//const std::string ONE_RECORD_L01_CAVITY_PROBE_PHASE   = "ONE_RECORD_L01_CAVITY_PROBE_PHASE";

	//const std::string ONE_RECORD_CALIBRATION_POWER = "ONE_RECORD_CALIBRATION_POWER";
	//const std::string ONE_RECORD_CALIBRATION_PHASE = "ONE_RECORD_CALIBRATION_PHASE";
	//														   































































	//const std::string 	CH1_PWR_REM_SCAN   = "CH1_PWR_REM_SCAN";
	//const std::string 	CH1_PWR_REM_ACQM   = "CH1_PWR_REM_ACQM";
	//const std::string 	CH1_PWR_REM		   = "CH1_PWR_REM";
	//const std::string 	CH1_PHASE_REM_SCAN = "CH1_PHASE_REM_SCAN";
	//const std::string 	CH1_PHASE_REM_ACQM = "CH1_PHASE_REM_ACQM";
	//const std::string 	CH1_PHASE_REM	   = "CH1_PHASE_REM";
	//const std::string 	CH1_AMP_DER_SCAN   = "CH1_AMP_DER_SCAN";
	//const std::string 	CH1_AMP_DER_ACQM   = "CH1_AMP_DER_ACQM";
	//const std::string 	CH1_PHASE_DER_SCAN = "CH1_PHASE_DER_SCAN";
	//const std::string 	CH1_PHASE_DER_ACQM = "CH1_PHASE_DER_ACQM";
	//const std::string 	CH1_PWR_LOC_SCAN   = "CH1_PWR_LOC_SCAN";
	//const std::string 	CH1_PWR_LOC_ACQM   = "CH1_PWR_LOC_ACQM";

	//const std::string 	CH2_PWR_REM_SCAN = "CH2_PWR_REM_SCAN";
	//const std::string 	CH2_PWR_REM_ACQM = "CH2_PWR_REM_ACQM";
	//const std::string 	CH2_PWR_REM = "CH2_PWR_REM";
	//const std::string 	CH2_PHASE_REM_SCAN = "CH2_PHASE_REM_SCAN";
	//const std::string 	CH2_PHASE_REM_ACQM = "CH2_PHASE_REM_ACQM";
	//const std::string 	CH2_PHASE_REM = "CH2_PHASE_REM";
	//const std::string 	CH2_AMP_DER_SCAN = "CH2_AMP_DER_SCAN";
	//const std::string 	CH2_AMP_DER_ACQM = "CH2_AMP_DER_ACQM";
	//const std::string 	CH2_PHASE_DER_SCAN = "CH2_PHASE_DER_SCAN";
	//const std::string 	CH2_PHASE_DER_ACQM = "CH2_PHASE_DER_ACQM";
	//const std::string 	CH2_PWR_LOC_SCAN = "CH2_PWR_LOC_SCAN";
	//const std::string 	CH2_PWR_LOC_ACQM = "CH2_PWR_LOC_ACQM";

	//const std::string 	CH3_PWR_REM_SCAN = "CH3_PWR_REM_SCAN";
	//const std::string 	CH3_PWR_REM_ACQM = "CH3_PWR_REM_ACQM";
	//const std::string 	CH3_PWR_REM = "CH3_PWR_REM";
	//const std::string 	CH3_PHASE_REM_SCAN = "CH3_PHASE_REM_SCAN";
	//const std::string 	CH3_PHASE_REM_ACQM = "CH3_PHASE_REM_ACQM";
	//const std::string 	CH3_PHASE_REM = "CH3_PHASE_REM";
	//const std::string 	CH3_AMP_DER_SCAN = "CH3_AMP_DER_SCAN";
	//const std::string 	CH3_AMP_DER_ACQM = "CH3_AMP_DER_ACQM";
	//const std::string 	CH3_PHASE_DER_SCAN = "CH3_PHASE_DER_SCAN";
	//const std::string 	CH3_PHASE_DER_ACQM = "CH3_PHASE_DER_ACQM";
	//const std::string 	CH3_PWR_LOC_SCAN = "CH3_PWR_LOC_SCAN";
	//const std::string 	CH3_PWR_LOC_ACQM = "CH3_PWR_LOC_ACQM";

	//const std::string 	CH4_PWR_REM_SCAN = "CH4_PWR_REM_SCAN";
	//const std::string 	CH4_PWR_REM_ACQM = "CH4_PWR_REM_ACQM";
	//const std::string 	CH4_PWR_REM = "CH4_PWR_REM";
	//const std::string 	CH4_PHASE_REM_SCAN = "CH4_PHASE_REM_SCAN";
	//const std::string 	CH4_PHASE_REM_ACQM = "CH4_PHASE_REM_ACQM";
	//const std::string 	CH4_PHASE_REM = "CH4_PHASE_REM";
	//const std::string 	CH4_AMP_DER_SCAN = "CH4_AMP_DER_SCAN";
	//const std::string 	CH4_AMP_DER_ACQM = "CH4_AMP_DER_ACQM";
	//const std::string 	CH4_PHASE_DER_SCAN = "CH4_PHASE_DER_SCAN";
	//const std::string 	CH4_PHASE_DER_ACQM = "CH4_PHASE_DER_ACQM";
	//const std::string 	CH4_PWR_LOC_SCAN = "CH4_PWR_LOC_SCAN";
	//const std::string 	CH4_PWR_LOC_ACQM = "CH4_PWR_LOC_ACQM";
	//					  
	//const std::string 	CH5_PWR_REM_SCAN = "CH5_PWR_REM_SCAN";
	//const std::string 	CH5_PWR_REM_ACQM = "CH5_PWR_REM_ACQM";
	//const std::string 	CH5_PWR_REM = "CH5_PWR_REM";
	//const std::string 	CH5_PHASE_REM_SCAN = "CH5_PHASE_REM_SCAN";
	//const std::string 	CH5_PHASE_REM_ACQM = "CH5_PHASE_REM_ACQM";
	//const std::string 	CH5_PHASE_REM = "CH5_PHASE_REM";
	//const std::string 	CH5_AMP_DER_SCAN = "CH5_AMP_DER_SCAN";
	//const std::string 	CH5_AMP_DER_ACQM = "CH5_AMP_DER_ACQM";
	//const std::string 	CH5_PHASE_DER_SCAN = "CH5_PHASE_DER_SCAN";
	//const std::string 	CH5_PHASE_DER_ACQM = "CH5_PHASE_DER_ACQM";
	//const std::string 	CH5_PWR_LOC_SCAN = "CH5_PWR_LOC_SCAN";
	//const std::string 	CH5_PWR_LOC_ACQM = "CH5_PWR_LOC_ACQM";

	//const std::string 	CH6_PWR_REM_SCAN = "CH6_PWR_REM_SCAN";
	//const std::string 	CH6_PWR_REM_ACQM = "CH6_PWR_REM_ACQM";
	//const std::string 	CH6_PWR_REM = "CH6_PWR_REM";
	//const std::string 	CH6_PHASE_REM_SCAN = "CH6_PHASE_REM_SCAN";
	//const std::string 	CH6_PHASE_REM_ACQM = "CH6_PHASE_REM_ACQM";
	//const std::string 	CH6_PHASE_REM = "CH6_PHASE_REM";
	//const std::string 	CH6_AMP_DER_SCAN = "CH6_AMP_DER_SCAN";
	//const std::string 	CH6_AMP_DER_ACQM = "CH6_AMP_DER_ACQM";
	//const std::string 	CH6_PHASE_DER_SCAN = "CH6_PHASE_DER_SCAN";
	//const std::string 	CH6_PHASE_DER_ACQM = "CH6_PHASE_DER_ACQM";
	//const std::string 	CH6_PWR_LOC_SCAN = "CH6_PWR_LOC_SCAN";
	//const std::string 	CH6_PWR_LOC_ACQM = "CH6_PWR_LOC_ACQM";

	//const std::string 	CH7_PWR_REM_SCAN = "CH7_PWR_REM_SCAN";
	//const std::string 	CH7_PWR_REM_ACQM = "CH7_PWR_REM_ACQM";
	//const std::string 	CH7_PWR_REM = "CH7_PWR_REM";
	//const std::string 	CH7_PHASE_REM_SCAN = "CH7_PHASE_REM_SCAN";
	//const std::string 	CH7_PHASE_REM_ACQM = "CH7_PHASE_REM_ACQM";
	//const std::string 	CH7_PHASE_REM = "CH7_PHASE_REM";
	//const std::string 	CH7_AMP_DER_SCAN = "CH7_AMP_DER_SCAN";
	//const std::string 	CH7_AMP_DER_ACQM = "CH7_AMP_DER_ACQM";
	//const std::string 	CH7_PHASE_DER_SCAN = "CH7_PHASE_DER_SCAN";
	//const std::string 	CH7_PHASE_DER_ACQM = "CH7_PHASE_DER_ACQM";
	//const std::string 	CH7_PWR_LOC_SCAN = "CH7_PWR_LOC_SCAN";
	//const std::string 	CH7_PWR_LOC_ACQM = "CH7_PWR_LOC_ACQM";

	//const std::string 	CH8_PWR_REM_SCAN = "CH8_PWR_REM_SCAN";
	//const std::string 	CH8_PWR_REM_ACQM = "CH8_PWR_REM_ACQM";
	//const std::string 	CH8_PWR_REM = "CH8_PWR_REM";
	//const std::string 	CH8_PHASE_REM_SCAN = "CH8_PHASE_REM_SCAN";
	//const std::string 	CH8_PHASE_REM_ACQM = "CH8_PHASE_REM_ACQM";
	//const std::string 	CH8_PHASE_REM = "CH8_PHASE_REM";
	//const std::string 	CH8_AMP_DER_SCAN = "CH8_AMP_DER_SCAN";
	//const std::string 	CH8_AMP_DER_ACQM = "CH8_AMP_DER_ACQM";
	//const std::string 	CH8_PHASE_DER_SCAN = "CH8_PHASE_DER_SCAN";
	//const std::string 	CH8_PHASE_DER_ACQM = "CH8_PHASE_DER_ACQM";
	//const std::string 	CH8_PWR_LOC_SCAN = "CH8_PWR_LOC_SCAN";
	//const std::string 	CH8_PWR_LOC_ACQM = "CH8_PWR_LOC_ACQM";

	


	//const std::string LLRF_CH1_INTERLOCK = "LLRF_CH1_INTERLOCK";
	//const std::string LLRF_CH2_INTERLOCK = "LLRF_CH2_INTERLOCK";
	//const std::string LLRF_CH3_INTERLOCK = "LLRF_CH3_INTERLOCK";
	//const std::string LLRF_CH4_INTERLOCK = "LLRF_CH4_INTERLOCK";
	//const std::string LLRF_CH5_INTERLOCK = "LLRF_CH5_INTERLOCK";
	//const std::string LLRF_CH6_INTERLOCK = "LLRF_CH6_INTERLOCK";
	//const std::string LLRF_CH7_INTERLOCK = "LLRF_CH7_INTERLOCK";
	//const std::string LLRF_CH8_INTERLOCK = "LLRF_CH8_INTERLOCK";

	// TODO i doubt we need these 
	const std::string CH1_INTERLOCK_STATUS = "CH1_INTERLOCK_STATUS";
	const std::string CH1_INTERLOCK_ENABLE = "CH1_INTERLOCK_ENABLE";
	const std::string CH1_INTERLOCK_U = "CH1_INTERLOCK_U";
	const std::string CH1_INTERLOCK_P = "CH1_INTERLOCK_P";
	const std::string CH1_INTERLOCK_PDBM = "CH1_INTERLOCK_PDBM";
	const std::string CH2_INTERLOCK_STATUS = "CH2_INTERLOCK_STATUS";
	const std::string CH2_INTERLOCK_ENABLE = "CH2_INTERLOCK_ENABLE";
	const std::string CH2_INTERLOCK_U = "CH2_INTERLOCK_U";
	const std::string CH2_INTERLOCK_P = "CH2_INTERLOCK_P";
	const std::string CH2_INTERLOCK_PDBM = "CH2_INTERLOCK_PDBM";
	const std::string CH3_INTERLOCK_STATUS = "CH3_INTERLOCK_STATUS";
	const std::string CH3_INTERLOCK_ENABLE = "CH3_INTERLOCK_ENABLE";
	const std::string CH3_INTERLOCK_U = "CH3_INTERLOCK_U";
	const std::string CH3_INTERLOCK_P = "CH3_INTERLOCK_P";
	const std::string CH3_INTERLOCK_PDBM = "CH3_INTERLOCK_PDBM";
	const std::string CH4_INTERLOCK_STATUS = "CH4_INTERLOCK_STATUS";
	const std::string CH4_INTERLOCK_ENABLE = "CH4_INTERLOCK_ENABLE";
	const std::string CH4_INTERLOCK_U = "CH4_INTERLOCK_U";
	const std::string CH4_INTERLOCK_P = "CH4_INTERLOCK_P";
	const std::string CH4_INTERLOCK_PDBM = "CH4_INTERLOCK_PDBM";
	const std::string CH5_INTERLOCK_STATUS = "CH5_INTERLOCK_STATUS";
	const std::string CH5_INTERLOCK_ENABLE = "CH5_INTERLOCK_ENABLE";
	const std::string CH5_INTERLOCK_U = "CH5_INTERLOCK_U";
	const std::string CH5_INTERLOCK_P = "CH5_INTERLOCK_P";
	const std::string CH5_INTERLOCK_PDBM = "CH5_INTERLOCK_PDBM";
	const std::string CH6_INTERLOCK_STATUS = "CH6_INTERLOCK_STATUS";
	const std::string CH6_INTERLOCK_ENABLE = "CH6_INTERLOCK_ENABLE";
	const std::string CH6_INTERLOCK_U = "CH6_INTERLOCK_U";
	const std::string CH6_INTERLOCK_P = "CH6_INTERLOCK_P";
	const std::string CH6_INTERLOCK_PDBM = "CH6_INTERLOCK_PDBM";
	const std::string CH7_INTERLOCK_STATUS = "CH7_INTERLOCK_STATUS";
	const std::string CH7_INTERLOCK_ENABLE = "CH7_INTERLOCK_ENABLE";
	const std::string CH7_INTERLOCK_U = "CH7_INTERLOCK_U";
	const std::string CH7_INTERLOCK_P = "CH7_INTERLOCK_P";
	const std::string CH7_INTERLOCK_PDBM = "CH7_INTERLOCK_PDBM";
	const std::string CH8_INTERLOCK_STATUS = "CH8_INTERLOCK_STATUS";
	const std::string CH8_INTERLOCK_ENABLE = "CH8_INTERLOCK_ENABLE";
	const std::string CH8_INTERLOCK_U = "CH8_INTERLOCK_U";
	const std::string CH8_INTERLOCK_P = "CH8_INTERLOCK_P";
	const std::string CH8_INTERLOCK_PDBM = "CH8_INTERLOCK_PDBM";




};
/*! @}*/
#endif // GLOBAL_CONSTANTS_H_
