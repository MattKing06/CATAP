#include <RFModulatorPVRecords.h>




namespace RFModulatorRecords
{

std::string const HOLD_RF_ON = "HOLD_RF_ON";
// GUN
std::string const GUN_MOD_RESET = "GUN_MOD_RESET";
std::string const GUN_MOD_STATE_SET = "GUN_MOD_STATE_SET";
std::string const GUN_MOD_STATE_SET_READ = "GUN_MOD_STATE_SET_READ";
std::string const GUN_MOD_MAIN_STATE_READ = "GUN_MOD_MAIN_STATE_READ";
std::string const GUN_MOD_ERR_SVAL = "GUN_MOD_ERR_SVAL";
std::string const GUN_MOD_ILOCK1 = "GUN_MOD_ILOCK1";
std::string const GUN_MOD_ILOCK2 = "GUN_MOD_ILOCK2";
std::string const GUN_MOD_ILOCK3 = "GUN_MOD_ILOCK3";
std::string const GUN_MOD_ILOCK4 = "GUN_MOD_ILOCK4";
std::string const GUN_MOD_ILOCK5 = "GUN_MOD_ILOCK5";
std::string const GUN_MOD_WARMUPT = "GUN_MOD_WARMUPT";
std::string const GUN_MOD_MAGPS1_CURR_READ = "GUN_MOD_MAGPS1_CURR_READ";
std::string const GUN_MOD_MAGPS2_CURR_READ = "GUN_MOD_MAGPS2_CURR_READ";
std::string const GUN_MOD_MAGPS3_CURR_READ = "GUN_MOD_MAGPS3_CURR_READ";
std::string const GUN_MOD_MAGPS4_CURR_READ = "GUN_MOD_MAGPS4_CURR_READ";
std::string const GUN_MOD_MAGPS1_VOLT_READ = "GUN_MOD_MAGPS1_VOLT_READ";
std::string const GUN_MOD_MAGPS2_VOLT_READ = "GUN_MOD_MAGPS2_VOLT_READ";
std::string const GUN_MOD_MAGPS3_VOLT_READ = "GUN_MOD_MAGPS3_VOLT_READ";
std::string const GUN_MOD_MAGPS4_VOLT_READ = "GUN_MOD_MAGPS4_VOLT_READ";
std::string const GUN_MOD_HVPS1_CURR_READ = "GUN_MOD_HVPS1_CURR_READ";
std::string const GUN_MOD_HVPS2_CURR_READ = "GUN_MOD_HVPS2_CURR_READ";
std::string const GUN_MOD_HVPS3_CURR_READ = "GUN_MOD_HVPS3_CURR_READ";
std::string const GUN_MOD_HVPS1_VOLT_READ = "GUN_MOD_HVPS1_VOLT_READ";
std::string const GUN_MOD_HVPS2_VOLT_READ = "GUN_MOD_HVPS2_VOLT_READ";
std::string const GUN_MOD_HVPS3_VOLT_READ = "GUN_MOD_HVPS3_VOLT_READ";
std::string const GUN_MOD_CT_READ = "GUN_MOD_CT_READ";
std::string const GUN_MOD_CVD_READ = "GUN_MOD_CVD_READ";
std::string const GUN_MOD_PULSE_WIDTH_READ = "GUN_MOD_PULSE_WIDTH_READ";
std::string const GUN_MOD_PULSE_WIDTH_FWHM_READ = "GUN_MOD_PULSE_WIDTH_FWHM_READ";
std::string const GUN_MOD_IONP_PRESSURE_READ = "GUN_MOD_IONP_PRESSURE_READ";
// L01
std::string const ON = "ON";
std::string const SYSTEM_STATE_PUT = "SYSTEM_STATE_PUT";
std::string const HVPS_VOLTAGE_SET = "HVPS_VOLTAGE_SET";
std::string const L01_MOD_WARMUPT = "L01_MOD_WARMUPT";
std::string const HVPS_VOLTAGE_LOW_ALARM_SET = "HVPS_VOLTAGE_LOW_ALARM_SET";
std::string const HVPS_VOLTAGE_HI_ALARM_SET = "HVPS_VOLTAGE_HI_ALARM_SET";
std::string const L01_RESET = "L01_RESET";
std::string const SYSTEM_STATE_READ = "SYSTEM_STATE_READ";
std::string const HVPS_VOLTAGE_SET_READ = "HVPS_VOLTAGE_SET_READ";
std::string const HVPS_VOLTAGE_LOW_ALARM_SET_READ = "HVPS_VOLTAGE_LOW_ALARM_SET_READ";
std::string const HVPS_VOLTAGE_HI_ALARM_SET_READ = "HVPS_VOLTAGE_HI_ALARM_SET_READ";
std::string const HVPS_VOLTAGE_READ = "HVPS_VOLTAGE_READ";
std::string const HVPS_CURRENT_READ = "HVPS_CURRENT_READ";
std::string const HEATER_VOLTAGE = "HEATER_VOLTAGE";
std::string const HEATER_CURRENT = "HEATER_CURRENT";
std::string const RESET_VOLTAGE = "RESET_VOLTAGE";
std::string const RESET_CURRENT = "RESET_CURRENT";
std::string const ION_PUMP_VOLTAGE = "ION_PUMP_VOLTAGE";
std::string const ION_PUMP_CURRENT = "ION_PUMP_CURRENT";
std::string const SUPPLY_TEMP = "SUPPLY_TEMP";
std::string const SUPPLY_PRESSURE = "SUPPLY_PRESSURE";
std::string const RETURN_TEMP = "RETURN_TEMP";
std::string const RETURN_PRESSURE = "RETURN_PRESSURE";
std::string const BODY_FLOW_RATE = "BODY_FLOW_RATE";
std::string const COLLECTOR_FLOW_RATE = "COLLECTOR_FLOW_RATE";
std::string const SOLENOID_FLOW_RATE = "SOLENOID_FLOW_RATE";
std::string const TANK_FLOW_RATE = "TANK_FLOW_RATE";
std::string const COLLECTOR_RETURN_RATE = "COLLECTOR_RETURN_RATE";
std::string const BODY_RETURN_TEMPERATURE = "BODY_RETURN_TEMPERATURE";
std::string const SOLENOID_1_VOLTAGE = "SOLENOID_1_VOLTAGE";
std::string const SOLENOID_2_VOLTAGE = "SOLENOID_2_VOLTAGE";
std::string const SOLENOID_3_VOLTAGE = "SOLENOID_3_VOLTAGE";
std::string const SOLENOID_1_CURRENT = "SOLENOID_1_CURRENT";
std::string const SOLENOID_2_CURRENT = "SOLENOID_2_CURRENT";
std::string const SOLENOID_3_CURRENT = "SOLENOID_3_CURRENT";
std::string const L01_FAULT = "L01_FAULT";
std::string const ERROR_WORD_1 = "ERROR_WORD_1";
std::string const ERROR_WORD_2 = "ERROR_WORD_2";
std::string const ERROR_WORD_3 = "ERROR_WORD_3";
std::string const ERROR_WORD_4 = "ERROR_WORD_4";
std::string const ERROR_WORD_5 = "ERROR_WORD_5";
std::string const ERROR_WORD_6 = "ERROR_WORD_6";
std::string const ERROR_WORD_7 = "ERROR_WORD_7";
std::string const ERROR_WORD_8 = "ERROR_WORD_8";
std::string const ERROR_WORD_9 = "ERROR_WORD_9";
std::string const ERROR_WORD_10 = "ERROR_WORD_10";
std::string const ERROR_WORD_11 = "ERROR_WORD_11";
std::string const ERROR_WORD_12 = "ERROR_WORD_12";
std::string const ERROR_WORD_13 = "ERROR_WORD_13";
std::string const ERROR_WORD_14 = "ERROR_WORD_14";
std::string const ERROR_WORD_15 = "ERROR_WORD_15";
std::string const ERROR_WORD_16 = "ERROR_WORD_16";
std::string const ERROR_WORD_17 = "ERROR_WORD_17";
std::string const ERROR_WORD_18 = "ERROR_WORD_18";
std::string const ERROR_WORD_19 = "ERROR_WORD_19";
std::string const ERROR_WORD_20 = "ERROR_WORD_20";
std::string const ERROR_DESC_1 = "ERROR_DESC_1";
std::string const ERROR_DESC_2 = "ERROR_DESC_2";
std::string const ERROR_DESC_3 = "ERROR_DESC_3";
std::string const ERROR_DESC_4 = "ERROR_DESC_4";
std::string const ERROR_DESC_5 = "ERROR_DESC_5";
std::string const ERROR_DESC_6 = "ERROR_DESC_6";
std::string const ERROR_DESC_7 = "ERROR_DESC_7";
std::string const ERROR_DESC_8 = "ERROR_DESC_8";
std::string const ERROR_DESC_9 = "ERROR_DESC_9";
std::string const ERROR_DESC_10 = "ERROR_DESC_10";
std::string const ERROR_DESC_11 = "ERROR_DESC_11";
std::string const ERROR_DESC_12 = "ERROR_DESC_12";
std::string const ERROR_DESC_13 = "ERROR_DESC_13";
std::string const ERROR_DESC_14 = "ERROR_DESC_14";
std::string const ERROR_DESC_15 = "ERROR_DESC_15";
std::string const ERROR_DESC_16 = "ERROR_DESC_16";
std::string const ERROR_DESC_17 = "ERROR_DESC_17";
std::string const ERROR_DESC_18 = "ERROR_DESC_18";
std::string const ERROR_DESC_19 = "ERROR_DESC_19";
std::string const ERROR_DESC_20 = "ERROR_DESC_20";


// gun mod has a hex code that can be converted to a message 
std::string const HEX_STATE_MESSAGE = "HEX_STATE_MESSAGE";

	std::vector<std::string> const low_level_strings = { 
	//gun strings
	GUN_MOD_ILOCK1,	GUN_MOD_ILOCK2,	GUN_MOD_ILOCK3,
	GUN_MOD_ILOCK4,	GUN_MOD_ILOCK5, GUN_MOD_ERR_SVAL,
	HEX_STATE_MESSAGE,
	// L01 strings
	SYSTEM_STATE_READ,
	ERROR_WORD_1,	ERROR_WORD_2,
	ERROR_WORD_3,	ERROR_WORD_4,
	ERROR_WORD_5,	ERROR_WORD_6,
	ERROR_WORD_7,	ERROR_WORD_8,
	ERROR_WORD_9,	ERROR_WORD_10,
	ERROR_WORD_11,	ERROR_WORD_12,
	ERROR_WORD_13,	ERROR_WORD_14,
	ERROR_WORD_15,	ERROR_WORD_16,
	ERROR_WORD_17,	ERROR_WORD_18,
	ERROR_WORD_19,	ERROR_WORD_20,
	ERROR_DESC_1,	ERROR_DESC_2,
	ERROR_DESC_3,	ERROR_DESC_4,
	ERROR_DESC_5,	ERROR_DESC_6,
	ERROR_DESC_7,	ERROR_DESC_8,
	ERROR_DESC_9,	ERROR_DESC_10,
	ERROR_DESC_11,	ERROR_DESC_12,
	ERROR_DESC_13,	ERROR_DESC_14,
	ERROR_DESC_15,	ERROR_DESC_16,
	ERROR_DESC_17,  ERROR_DESC_18,
	ERROR_DESC_19,	ERROR_DESC_20 };

	std::vector<std::string> const low_level_values = {
		GUN_MOD_MAGPS1_CURR_READ,		GUN_MOD_MAGPS2_CURR_READ,
		GUN_MOD_MAGPS3_CURR_READ,		GUN_MOD_MAGPS4_CURR_READ,
		GUN_MOD_MAGPS1_VOLT_READ,		GUN_MOD_MAGPS2_VOLT_READ,
		GUN_MOD_MAGPS3_VOLT_READ,		GUN_MOD_MAGPS4_VOLT_READ,
		GUN_MOD_HVPS1_CURR_READ, GUN_MOD_HVPS2_CURR_READ,
		GUN_MOD_HVPS3_CURR_READ,	GUN_MOD_HVPS1_VOLT_READ,
		GUN_MOD_HVPS2_VOLT_READ,		GUN_MOD_HVPS3_VOLT_READ,
		GUN_MOD_CT_READ,		GUN_MOD_CVD_READ,
		GUN_MOD_PULSE_WIDTH_READ,		GUN_MOD_PULSE_WIDTH_FWHM_READ,
		GUN_MOD_IONP_PRESSURE_READ,
		HVPS_VOLTAGE_SET,	HVPS_VOLTAGE_LOW_ALARM_SET,
	HVPS_VOLTAGE_HI_ALARM_SET,	L01_MOD_WARMUPT, GUN_MOD_WARMUPT,
	HVPS_VOLTAGE_SET_READ,	HVPS_VOLTAGE_LOW_ALARM_SET_READ,
	HVPS_VOLTAGE_HI_ALARM_SET_READ,	HVPS_VOLTAGE_READ,
	HVPS_CURRENT_READ,	HEATER_VOLTAGE,
	HEATER_CURRENT,	RESET_VOLTAGE,
	RESET_CURRENT,	ION_PUMP_VOLTAGE,
	ION_PUMP_CURRENT,		SUPPLY_TEMP,
	SUPPLY_PRESSURE,	RETURN_TEMP,
	RETURN_PRESSURE,	BODY_FLOW_RATE,
	COLLECTOR_FLOW_RATE,	SOLENOID_FLOW_RATE,
	TANK_FLOW_RATE,	COLLECTOR_RETURN_RATE,
	BODY_RETURN_TEMPERATURE,
	SOLENOID_1_VOLTAGE,	SOLENOID_2_VOLTAGE,
	SOLENOID_3_VOLTAGE,	SOLENOID_1_CURRENT,
	SOLENOID_2_CURRENT,	SOLENOID_3_CURRENT };





std::vector<std::string> const rf_mod_records_list =
{
	HOLD_RF_ON,
	GUN_MOD_RESET,
	GUN_MOD_STATE_SET,
	GUN_MOD_STATE_SET_READ,
	GUN_MOD_MAIN_STATE_READ,
	GUN_MOD_ERR_SVAL,
	GUN_MOD_ILOCK1,
	GUN_MOD_ILOCK2,
	GUN_MOD_ILOCK3,
	GUN_MOD_ILOCK4,
	GUN_MOD_ILOCK5,
	GUN_MOD_WARMUPT,
	GUN_MOD_MAGPS1_CURR_READ,
	GUN_MOD_MAGPS2_CURR_READ,
	GUN_MOD_MAGPS3_CURR_READ,
	GUN_MOD_MAGPS4_CURR_READ,
	GUN_MOD_MAGPS1_VOLT_READ,
	GUN_MOD_MAGPS2_VOLT_READ,
	GUN_MOD_MAGPS3_VOLT_READ,
	GUN_MOD_MAGPS4_VOLT_READ,
	GUN_MOD_HVPS1_CURR_READ,
	GUN_MOD_HVPS2_CURR_READ,
	GUN_MOD_HVPS3_CURR_READ,
	GUN_MOD_HVPS1_VOLT_READ,
	GUN_MOD_HVPS2_VOLT_READ,
	GUN_MOD_HVPS3_VOLT_READ,
	GUN_MOD_CT_READ,
	GUN_MOD_CVD_READ,
	GUN_MOD_PULSE_WIDTH_READ,
	GUN_MOD_PULSE_WIDTH_FWHM_READ,
	GUN_MOD_IONP_PRESSURE_READ,
	ON,
	SYSTEM_STATE_PUT,
	HVPS_VOLTAGE_SET,
	HVPS_VOLTAGE_LOW_ALARM_SET,
	HVPS_VOLTAGE_HI_ALARM_SET,
	L01_RESET,
	L01_MOD_WARMUPT,
	SYSTEM_STATE_READ,
	HVPS_VOLTAGE_SET_READ,
	HVPS_VOLTAGE_LOW_ALARM_SET_READ,
	HVPS_VOLTAGE_HI_ALARM_SET_READ,
	HVPS_VOLTAGE_READ,
	HVPS_CURRENT_READ,
	HEATER_VOLTAGE,
	HEATER_CURRENT,
	RESET_VOLTAGE,
	RESET_CURRENT,
	ION_PUMP_VOLTAGE,
	ION_PUMP_CURRENT,
	SUPPLY_TEMP,
	SUPPLY_PRESSURE,
	RETURN_TEMP,
	RETURN_PRESSURE,
	BODY_FLOW_RATE,
	COLLECTOR_FLOW_RATE,
	SOLENOID_FLOW_RATE,
	TANK_FLOW_RATE,
	COLLECTOR_RETURN_RATE,
	BODY_RETURN_TEMPERATURE,
	SOLENOID_1_VOLTAGE,
	SOLENOID_2_VOLTAGE,
	SOLENOID_3_VOLTAGE,
	SOLENOID_1_CURRENT,
	SOLENOID_2_CURRENT,
	SOLENOID_3_CURRENT,
	L01_FAULT,
	ERROR_WORD_1,
	ERROR_WORD_2,
	ERROR_WORD_3,
	ERROR_WORD_4,
	ERROR_WORD_5,
	ERROR_WORD_6,
	ERROR_WORD_7,
	ERROR_WORD_8,
	ERROR_WORD_9,
	ERROR_WORD_10,
	ERROR_WORD_11,
	ERROR_WORD_12,
	ERROR_WORD_13,
	ERROR_WORD_14,
	ERROR_WORD_15,
	ERROR_WORD_16,
	ERROR_WORD_17,
	ERROR_WORD_18,
	ERROR_WORD_19,
	ERROR_WORD_20,
	ERROR_DESC_1,
	ERROR_DESC_2,
	ERROR_DESC_3,
	ERROR_DESC_4,
	ERROR_DESC_5,
	ERROR_DESC_6,
	ERROR_DESC_7,
	ERROR_DESC_8,
	ERROR_DESC_9,
	ERROR_DESC_10,
	ERROR_DESC_11,
	ERROR_DESC_12,
	ERROR_DESC_13,
	ERROR_DESC_14,
	ERROR_DESC_15,
	ERROR_DESC_16,
	ERROR_DESC_17,
	ERROR_DESC_18,
	ERROR_DESC_19,
	ERROR_DESC_20};


	std::vector<std::string> const rf_mod_monitor_records_list =
	{
		//HOLD_RF_ON,
		GUN_MOD_RESET,
		//GUN_MOD_STATE_SET,
		GUN_MOD_STATE_SET_READ,
		GUN_MOD_MAIN_STATE_READ,
		GUN_MOD_ERR_SVAL,
		GUN_MOD_ILOCK1,
		GUN_MOD_ILOCK2,
		GUN_MOD_ILOCK3,
		GUN_MOD_ILOCK4,
		GUN_MOD_ILOCK5,
		GUN_MOD_WARMUPT,
		GUN_MOD_MAGPS1_CURR_READ,
		GUN_MOD_MAGPS2_CURR_READ,
		GUN_MOD_MAGPS3_CURR_READ,
		GUN_MOD_MAGPS4_CURR_READ,
		GUN_MOD_MAGPS1_VOLT_READ,
		GUN_MOD_MAGPS2_VOLT_READ,
		GUN_MOD_MAGPS3_VOLT_READ,
		GUN_MOD_MAGPS4_VOLT_READ,
		GUN_MOD_HVPS1_CURR_READ,
		GUN_MOD_HVPS2_CURR_READ,
		GUN_MOD_HVPS3_CURR_READ,
		GUN_MOD_HVPS1_VOLT_READ,
		GUN_MOD_HVPS2_VOLT_READ,
		GUN_MOD_HVPS3_VOLT_READ,
		GUN_MOD_CT_READ,
		GUN_MOD_CVD_READ,
		GUN_MOD_PULSE_WIDTH_READ,
		GUN_MOD_PULSE_WIDTH_FWHM_READ,
		GUN_MOD_IONP_PRESSURE_READ,     
		//ON,
		SYSTEM_STATE_PUT,
		HVPS_VOLTAGE_SET,
		HVPS_VOLTAGE_LOW_ALARM_SET_READ,
		HVPS_VOLTAGE_HI_ALARM_SET_READ,
		//L01_RESET,
		SYSTEM_STATE_READ,
		HVPS_VOLTAGE_SET_READ,
		HVPS_VOLTAGE_LOW_ALARM_SET,
		HVPS_VOLTAGE_HI_ALARM_SET,
		HVPS_VOLTAGE_READ,
		HVPS_CURRENT_READ,
		HEATER_VOLTAGE,
		HEATER_CURRENT,
		RESET_VOLTAGE,
		RESET_CURRENT,
		ION_PUMP_VOLTAGE,
		ION_PUMP_CURRENT,
		SUPPLY_TEMP,
		SUPPLY_PRESSURE,
		RETURN_TEMP,
		RETURN_PRESSURE,
		BODY_FLOW_RATE,
		COLLECTOR_FLOW_RATE,
		SOLENOID_FLOW_RATE,
		TANK_FLOW_RATE,
		COLLECTOR_RETURN_RATE,
		BODY_RETURN_TEMPERATURE,
		SOLENOID_1_VOLTAGE,
		SOLENOID_2_VOLTAGE,
		SOLENOID_3_VOLTAGE,
		SOLENOID_1_CURRENT,
		SOLENOID_2_CURRENT,
		SOLENOID_3_CURRENT,
		L01_MOD_WARMUPT,
		L01_FAULT,
		ERROR_WORD_1,
		ERROR_WORD_2,
		ERROR_WORD_3,
		ERROR_WORD_4,
		ERROR_WORD_5,
		ERROR_WORD_6,
		ERROR_WORD_7,
		ERROR_WORD_8,
		ERROR_WORD_9,
		ERROR_WORD_10,
		ERROR_WORD_11,
		ERROR_WORD_12,
		ERROR_WORD_13,
		ERROR_WORD_14,
		ERROR_WORD_15,
		ERROR_WORD_16,
		ERROR_WORD_17,
		ERROR_WORD_18,
		ERROR_WORD_19,
		ERROR_WORD_20,
		ERROR_DESC_1,
		ERROR_DESC_2,
		ERROR_DESC_3,
		ERROR_DESC_4,
		ERROR_DESC_5,
		ERROR_DESC_6,
		ERROR_DESC_7,
		ERROR_DESC_8,
		ERROR_DESC_9,
		ERROR_DESC_10,
		ERROR_DESC_11,
		ERROR_DESC_12,
		ERROR_DESC_13,
		ERROR_DESC_14,
		ERROR_DESC_15,
		ERROR_DESC_16,
		ERROR_DESC_17,
		ERROR_DESC_18,
		ERROR_DESC_19,
		ERROR_DESC_20
 };

}