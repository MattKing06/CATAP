#ifndef RF_MODULATOR_RECORDS
#define RF_MODULATOR_RECORDS
#include <string>
#include <vector>

#include "GlobalStateEnums.h"
#include "GlobalConstants.h"


namespace RFModulatorRecords
{
	// non-specific
	extern std::string const HOLD_RF_ON;
	// GUN
	extern std::string const GUN_MOD_RESET;
	extern std::string const GUN_MOD_STATE_SET;
	extern std::string const GUN_MOD_STATE_SET_READ;
	extern std::string const GUN_MOD_MAIN_STATE_READ;
	extern std::string const GUN_MOD_ERR_SVAL;
	extern std::string const GUN_MOD_ILOCK1;
	extern std::string const GUN_MOD_ILOCK2;
	extern std::string const GUN_MOD_ILOCK3;
	extern std::string const GUN_MOD_ILOCK4;
	extern std::string const GUN_MOD_ILOCK5;
	extern std::string const GUN_MOD_WARMUPT;
	extern std::string const GUN_MOD_MAGPS1_CURR_READ;
	extern std::string const GUN_MOD_MAGPS2_CURR_READ;
	extern std::string const GUN_MOD_MAGPS3_CURR_READ;
	extern std::string const GUN_MOD_MAGPS4_CURR_READ;
	extern std::string const GUN_MOD_MAGPS1_VOLT_READ;
	extern std::string const GUN_MOD_MAGPS2_VOLT_READ;
	extern std::string const GUN_MOD_MAGPS3_VOLT_READ;
	extern std::string const GUN_MOD_MAGPS4_VOLT_READ;
	extern std::string const GUN_MOD_HVPS1_CURR_READ;
	extern std::string const GUN_MOD_HVPS2_CURR_READ;
	extern std::string const GUN_MOD_HVPS3_CURR_READ;
	extern std::string const GUN_MOD_HVPS1_VOLT_READ;
	extern std::string const GUN_MOD_HVPS2_VOLT_READ;
	extern std::string const GUN_MOD_HVPS3_VOLT_READ;
	extern std::string const GUN_MOD_CT_READ;
	extern std::string const GUN_MOD_CVD_READ;
	extern std::string const GUN_MOD_PULSE_WIDTH_READ;
	extern std::string const GUN_MOD_PULSE_WIDTH_FWHM_READ;
	extern std::string const GUN_MOD_IONP_PRESSURE_READ;
	// L01
	extern std::string const ON;
	extern std::string const SYSTEM_STATE_PUT;
	extern std::string const L01_MOD_WARMUPT;
	extern std::string const HVPS_VOLTAGE_SET;
	extern std::string const HVPS_VOLTAGE_LOW_ALARM_SET;
	extern std::string const HVPS_VOLTAGE_HI_ALARM_SET;
	extern std::string const L01_RESET;
	extern std::string const SYSTEM_STATE_READ ;
	extern std::string const HVPS_VOLTAGE_SET_READ;
	extern std::string const HVPS_VOLTAGE_LOW_ALARM_SET_READ;
	extern std::string const HVPS_VOLTAGE_HI_ALARM_SET_READ;
	extern std::string const HVPS_VOLTAGE_READ;
	extern std::string const HVPS_CURRENT_READ;
	extern std::string const HEATER_VOLTAGE;
	extern std::string const HEATER_CURRENT;
	extern std::string const RESET_VOLTAGE;
	extern std::string const RESET_CURRENT;
	extern std::string const ION_PUMP_VOLTAGE;
	extern std::string const ION_PUMP_CURRENT;
	extern std::string const SUPPLY_TEMP;
	extern std::string const SUPPLY_PRESSURE;
	extern std::string const RETURN_TEMP;
	extern std::string const RETURN_PRESSURE;
	extern std::string const BODY_FLOW_RATE;
	extern std::string const COLLECTOR_FLOW_RATE;
	extern std::string const SOLENOID_FLOW_RATE;
	extern std::string const TANK_FLOW_RATE;
	extern std::string const COLLECTOR_RETURN_RATE;
	extern std::string const BODY_RETURN_TEMPERATURE;
	extern std::string const SOLENOID_1_VOLTAGE;
	extern std::string const SOLENOID_2_VOLTAGE;
	extern std::string const SOLENOID_3_VOLTAGE;
	extern std::string const SOLENOID_1_CURRENT;
	extern std::string const SOLENOID_2_CURRENT;
	extern std::string const SOLENOID_3_CURRENT;
	extern std::string const L01_FAULT;
	extern std::string const ERROR_WORD_1;
	extern std::string const ERROR_WORD_2;
	extern std::string const ERROR_WORD_3;
	extern std::string const ERROR_WORD_4;
	extern std::string const ERROR_WORD_5;
	extern std::string const ERROR_WORD_6;
	extern std::string const ERROR_WORD_7;
	extern std::string const ERROR_WORD_8;
	extern std::string const ERROR_WORD_9;
	extern std::string const ERROR_WORD_10;
	extern std::string const ERROR_WORD_11;
	extern std::string const ERROR_WORD_12;
	extern std::string const ERROR_WORD_13;
	extern std::string const ERROR_WORD_14;
	extern std::string const ERROR_WORD_15;
	extern std::string const ERROR_WORD_16;
	extern std::string const ERROR_WORD_17;
	extern std::string const ERROR_WORD_18;
	extern std::string const ERROR_WORD_19;
	extern std::string const ERROR_WORD_20;
	extern std::string const ERROR_DESC_1;
	extern std::string const ERROR_DESC_2;
	extern std::string const ERROR_DESC_3;
	extern std::string const ERROR_DESC_4;
	extern std::string const ERROR_DESC_5;
	extern std::string const ERROR_DESC_6;
	extern std::string const ERROR_DESC_7;
	extern std::string const ERROR_DESC_8;
	extern std::string const ERROR_DESC_9;
	extern std::string const ERROR_DESC_10;
	extern std::string const ERROR_DESC_11;
	extern std::string const ERROR_DESC_12;
	extern std::string const ERROR_DESC_13;
	extern std::string const ERROR_DESC_14;
	extern std::string const ERROR_DESC_15;
	extern std::string const ERROR_DESC_16;
	extern std::string const ERROR_DESC_17;
	extern std::string const ERROR_DESC_18;
	extern std::string const ERROR_DESC_19;
	extern std::string const ERROR_DESC_20;

	// gun mod has a hex code that can be converted to a message 
	extern std::string const HEX_STATE_MESSAGE;


	extern std::vector<std::string> const rf_mod_records_list;
	extern std::vector<std::string> const rf_mod_monitor_records_list;

	extern std::vector<std::string> const low_level_strings;
	extern std::vector<std::string> const low_level_values;




}

namespace RFModulatorState
{
	struct RFModulatorStateStruct
	{   // proviude a default constructor
		RFModulatorStateStruct() :
			numRFModulators(GlobalConstants::zero_sizet)
			//, 
			//machineArea(HWC_ENUM::MACHINE_AREA::UNKNOWN_AREA) 
		{};
		size_t numRFModulators;

	};
}

#endif