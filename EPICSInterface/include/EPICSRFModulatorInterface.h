#pragma once
#include <EPICSInterface.h>
#include <RFModulatorPVRecords.h>
#include "EPICSInterface.h"
#include "RFModulator.h"

typedef void(*updateFunctionPtr)(struct event_handler_args args);

class EPICSRFModulatorInterface : public EPICSInterface
{
public:
	EPICSRFModulatorInterface();
	~EPICSRFModulatorInterface();
	std::string ownerName;

	static LoggingSystem messenger;

	void retrieveUpdateFunctionForRecord(const pvStruct& pv)const;

	static void update_HOLD_RF_ON();
	// GUN void
	static void update_GUN_MOD_RESET(const struct event_handler_args args);
	static void update_GUN_MOD_STATE_SET(const struct event_handler_args args);
	static void update_GUN_MOD_STATE_SET_READ(const struct event_handler_args args);
	static void update_GUN_MOD_MAIN_STATE_READ(const struct event_handler_args args);
	static void update_GUN_MOD_ERR_SVAL(const struct event_handler_args args);
	static void update_GUN_MOD_ILOCK1(const struct event_handler_args args);
	static void update_GUN_MOD_ILOCK2(const struct event_handler_args args);
	static void update_GUN_MOD_ILOCK3(const struct event_handler_args args);
	static void update_GUN_MOD_ILOCK4(const struct event_handler_args args);
	static void update_GUN_MOD_ILOCK5(const struct event_handler_args args);
	static void update_GUN_MOD_WARMUPT(const struct event_handler_args args);
	static void update_GUN_MOD_MAGPS1_CURR_READ(const struct event_handler_args args);
	static void update_GUN_MOD_MAGPS2_CURR_READ(const struct event_handler_args args);
	static void update_GUN_MOD_MAGPS3_CURR_READ(const struct event_handler_args args);
	static void update_GUN_MOD_MAGPS4_CURR_READ(const struct event_handler_args args);
	static void update_GUN_MOD_MAGPS1_VOLT_READ(const struct event_handler_args args);
	static void update_GUN_MOD_MAGPS2_VOLT_READ(const struct event_handler_args args);
	static void update_GUN_MOD_MAGPS3_VOLT_READ(const struct event_handler_args args);
	static void update_GUN_MOD_MAGPS4_VOLT_READ(const struct event_handler_args args);
	static void update_GUN_MOD_HVPS1_CURR_READ(const struct event_handler_args args);
	static void update_GUN_MOD_HVPS2_CURR_READ(const struct event_handler_args args);
	static void update_GUN_MOD_HVPS3_CURR_READ(const struct event_handler_args args);
	static void update_GUN_MOD_HVPS1_VOLT_READ(const struct event_handler_args args);
	static void update_GUN_MOD_HVPS2_VOLT_READ(const struct event_handler_args args);
	static void update_GUN_MOD_HVPS3_VOLT_READ(const struct event_handler_args args);
	static void update_GUN_MOD_CT_READ(const struct event_handler_args args);
	static void update_GUN_MOD_CVD_READ(const struct event_handler_args args);
	static void update_GUN_MOD_PULSE_WIDTH_READ(const struct event_handler_args args);
	static void update_GUN_MOD_PULSE_WIDTH_FWHM_READ(const struct event_handler_args args);
	static void update_GUN_MOD_IONP_PRESSURE_READ(const struct event_handler_args args);
	// L01
	static void update_ON(const struct event_handler_args args);
	static void update_SYSTEM_STATE_PUT(const struct event_handler_args args);
	static void update_HVPS_VOLTAGE_SET(const struct event_handler_args args);
	static void update_HVPS_VOLTAGE_LOW_ALARM_SET(const struct event_handler_args args);
	static void update_HVPS_VOLTAGE_HI_ALARM_SET(const struct event_handler_args args);
	static void update_L01_RESET(const struct event_handler_args args);
	static void update_SYSTEM_STATE_READ(const struct event_handler_args args);
	static void update_HVPS_VOLTAGE_SET_READ(const struct event_handler_args args);
	static void update_HVPS_VOLTAGE_LOW_ALARM_SET_READ(const struct event_handler_args args);
	static void update_HVPS_VOLTAGE_HI_ALARM_SET_READ(const struct event_handler_args args);
	static void update_HVPS_VOLTAGE_READ(const struct event_handler_args args);
	static void update_HVPS_CURRENT_READ(const struct event_handler_args args);
	static void update_HEATER_VOLTAGE(const struct event_handler_args args);
	static void update_HEATER_CURRENT(const struct event_handler_args args);
	static void update_RESET_VOLTAGE(const struct event_handler_args args);
	static void update_RESET_CURRENT(const struct event_handler_args args);
	static void update_ION_PUMP_VOLTAGE(const struct event_handler_args args);
	static void update_ION_PUMP_CURRENT(const struct event_handler_args args);
	static void update_SUPPLY_TEMP(const struct event_handler_args args);
	static void update_SUPPLY_PRESSURE(const struct event_handler_args args);
	static void update_RETURN_TEMP(const struct event_handler_args args);
	static void update_RETURN_PRESSURE(const struct event_handler_args args);
	static void update_BODY_FLOW_RATE(const struct event_handler_args args);
	static void update_COLLECTOR_FLOW_RATE(const struct event_handler_args args);
	static void update_SOLENOID_FLOW_RATE(const struct event_handler_args args);
	static void update_TANK_FLOW_RATE(const struct event_handler_args args);
	static void update_COLLECTOR_RETURN_RATE(const struct event_handler_args args);
	static void update_BODY_RETURN_TEMPERATURE(const struct event_handler_args args);
	static void update_SOLENOID_1_VOLTAGE(const struct event_handler_args args);
	static void update_SOLENOID_2_VOLTAGE(const struct event_handler_args args);
	static void update_SOLENOID_3_VOLTAGE(const struct event_handler_args args);
	static void update_SOLENOID_1_CURRENT(const struct event_handler_args args);
	static void update_SOLENOID_2_CURRENT(const struct event_handler_args args);
	static void update_SOLENOID_3_CURRENT(const struct event_handler_args args);
	static void update_L01_FAULT(const struct event_handler_args args);
	static void update_ERROR_WORD_1(const struct event_handler_args args);
	static void update_ERROR_WORD_2(const struct event_handler_args args);
	static void update_ERROR_WORD_3(const struct event_handler_args args);
	static void update_ERROR_WORD_4(const struct event_handler_args args);
	static void update_ERROR_WORD_5(const struct event_handler_args args);
	static void update_ERROR_WORD_6(const struct event_handler_args args);
	static void update_ERROR_WORD_7(const struct event_handler_args args);
	static void update_ERROR_WORD_8(const struct event_handler_args args);
	static void update_ERROR_WORD_9(const struct event_handler_args args);
	static void update_ERROR_WORD_10(const struct event_handler_args args);
	static void update_ERROR_WORD_11(const struct event_handler_args args);
	static void update_ERROR_WORD_12(const struct event_handler_args args);
	static void update_ERROR_WORD_13(const struct event_handler_args args);
	static void update_ERROR_WORD_14(const struct event_handler_args args);
	static void update_ERROR_WORD_15(const struct event_handler_args args);
	static void update_ERROR_WORD_16(const struct event_handler_args args);
	static void update_ERROR_WORD_17(const struct event_handler_args args);
	static void update_ERROR_WORD_18(const struct event_handler_args args);
	static void update_ERROR_WORD_19(const struct event_handler_args args);
	static void update_ERROR_WORD_20(const struct event_handler_args args);
	static void update_ERROR_DESC_1(const struct event_handler_args args);
	static void update_ERROR_DESC_2(const struct event_handler_args args);
	static void update_ERROR_DESC_3(const struct event_handler_args args);
	static void update_ERROR_DESC_4(const struct event_handler_args args);
	static void update_ERROR_DESC_5(const struct event_handler_args args);
	static void update_ERROR_DESC_6(const struct event_handler_args args);
	static void update_ERROR_DESC_7(const struct event_handler_args args);
	static void update_ERROR_DESC_8(const struct event_handler_args args);
	static void update_ERROR_DESC_9(const struct event_handler_args args);
	static void update_ERROR_DESC_10(const struct event_handler_args args);
	static void update_ERROR_DESC_11(const struct event_handler_args args);
	static void update_ERROR_DESC_12(const struct event_handler_args args);
	static void update_ERROR_DESC_13(const struct event_handler_args args);
	static void update_ERROR_DESC_14(const struct event_handler_args args);
	static void update_ERROR_DESC_15(const struct event_handler_args args);
	static void update_ERROR_DESC_16(const struct event_handler_args args);
	static void update_ERROR_DESC_17(const struct event_handler_args args);
	static void update_ERROR_DESC_18(const struct event_handler_args args);
	static void update_ERROR_DESC_19(const struct event_handler_args args);
	static void update_ERROR_DESC_20(const struct event_handler_args args);

	//std::map<std::string, updateFunctionPtr> updateFunctionMap;

    // NB this map is to replace the if-else tree typically used. 
   // it seesm in VS you can only have 127 if-else statements 
   // https://stackoverflow.com/questions/11508013/blocks-nested-too-deeply
	std::map<std::string, updateFunctionPtr> updateFunctionMap = {

	{RFModulatorRecords::GUN_MOD_STATE_SET_READ    ,this->update_GUN_MOD_STATE_SET_READ},
	{RFModulatorRecords::GUN_MOD_MAIN_STATE_READ   ,this->update_GUN_MOD_MAIN_STATE_READ},
	{RFModulatorRecords::GUN_MOD_ERR_SVAL		   ,this->update_GUN_MOD_ERR_SVAL},
	{RFModulatorRecords::GUN_MOD_WARMUPT		   ,this->update_GUN_MOD_WARMUPT},



	{RFModulatorRecords::GUN_MOD_ILOCK1                ,this->update_GUN_MOD_ILOCK1},
	{RFModulatorRecords::GUN_MOD_ILOCK2                ,this->update_GUN_MOD_ILOCK2},
	{RFModulatorRecords::GUN_MOD_ILOCK3                ,this->update_GUN_MOD_ILOCK3},
	{RFModulatorRecords::GUN_MOD_ILOCK4                ,this->update_GUN_MOD_ILOCK4},
	{RFModulatorRecords::GUN_MOD_ILOCK5                ,this->update_GUN_MOD_ILOCK5},
	{RFModulatorRecords::GUN_MOD_MAGPS1_CURR_READ                ,this->update_GUN_MOD_MAGPS1_CURR_READ},
	{RFModulatorRecords::GUN_MOD_MAGPS2_CURR_READ				 ,this->update_GUN_MOD_MAGPS2_CURR_READ},
	{RFModulatorRecords::GUN_MOD_MAGPS3_CURR_READ				 ,this->update_GUN_MOD_MAGPS3_CURR_READ},
	{RFModulatorRecords::GUN_MOD_MAGPS4_CURR_READ				 ,this->update_GUN_MOD_MAGPS4_CURR_READ},
	{RFModulatorRecords::GUN_MOD_MAGPS1_VOLT_READ				 ,this->update_GUN_MOD_MAGPS1_VOLT_READ},
	{RFModulatorRecords::GUN_MOD_MAGPS2_VOLT_READ				 ,this->update_GUN_MOD_MAGPS2_VOLT_READ},
	{RFModulatorRecords::GUN_MOD_MAGPS3_VOLT_READ				 ,this->update_GUN_MOD_MAGPS3_VOLT_READ},
	{RFModulatorRecords::GUN_MOD_MAGPS4_VOLT_READ				 ,this->update_GUN_MOD_MAGPS4_VOLT_READ},
	{RFModulatorRecords::GUN_MOD_HVPS1_CURR_READ 				 ,this->update_GUN_MOD_HVPS1_CURR_READ },
	{RFModulatorRecords::GUN_MOD_HVPS2_CURR_READ 				 ,this->update_GUN_MOD_HVPS2_CURR_READ },
	{RFModulatorRecords::GUN_MOD_HVPS3_CURR_READ 				 ,this->update_GUN_MOD_HVPS3_CURR_READ },
	{RFModulatorRecords::GUN_MOD_HVPS1_VOLT_READ 				 ,this->update_GUN_MOD_HVPS1_VOLT_READ },
	{RFModulatorRecords::GUN_MOD_HVPS2_VOLT_READ 				 ,this->update_GUN_MOD_HVPS2_VOLT_READ },
	{RFModulatorRecords::GUN_MOD_HVPS3_VOLT_READ 				 ,this->update_GUN_MOD_HVPS3_VOLT_READ },
	{RFModulatorRecords::GUN_MOD_CT_READ  						 ,this->update_GUN_MOD_CT_READ		   },
	{RFModulatorRecords::GUN_MOD_CVD_READ 						 ,this->update_GUN_MOD_CVD_READ		   },
	{RFModulatorRecords::GUN_MOD_PULSE_WIDTH_READ 				 ,this->update_GUN_MOD_PULSE_WIDTH_READ},
	{RFModulatorRecords::GUN_MOD_PULSE_WIDTH_FWHM_READ 			 ,this->update_GUN_MOD_PULSE_WIDTH_FWHM_READ},
	{RFModulatorRecords::GUN_MOD_IONP_PRESSURE_READ    			 ,this->update_GUN_MOD_IONP_PRESSURE_READ	},
	{RFModulatorRecords::SYSTEM_STATE_PUT,                 this->update_SYSTEM_STATE_PUT},
	{RFModulatorRecords::HVPS_VOLTAGE_SET,  			   this->update_HVPS_VOLTAGE_SET},
	{RFModulatorRecords::HVPS_VOLTAGE_LOW_ALARM_SET_READ,  this->update_HVPS_VOLTAGE_LOW_ALARM_SET_READ},
	{RFModulatorRecords::HVPS_VOLTAGE_HI_ALARM_SET_READ,   this->update_HVPS_VOLTAGE_HI_ALARM_SET_READ},
	{RFModulatorRecords::SYSTEM_STATE_READ,				   this->update_SYSTEM_STATE_READ},
	{RFModulatorRecords::HVPS_VOLTAGE_SET_READ,			   this->update_HVPS_VOLTAGE_SET_READ},
	{RFModulatorRecords::HVPS_VOLTAGE_LOW_ALARM_SET,	   this->update_HVPS_VOLTAGE_LOW_ALARM_SET},
	{RFModulatorRecords::HVPS_VOLTAGE_HI_ALARM_SET,		   this->update_HVPS_VOLTAGE_HI_ALARM_SET},
	{RFModulatorRecords::HVPS_VOLTAGE_READ,				   this->update_HVPS_VOLTAGE_READ},
	{RFModulatorRecords::HVPS_CURRENT_READ,				   this->update_HVPS_CURRENT_READ},
	{RFModulatorRecords::HEATER_VOLTAGE,				   this->update_HEATER_VOLTAGE},
	{RFModulatorRecords::HEATER_CURRENT,				   this->update_HEATER_CURRENT},
	{RFModulatorRecords::RESET_VOLTAGE,					   this->update_RESET_VOLTAGE},
	{RFModulatorRecords::RESET_CURRENT,					   this->update_RESET_CURRENT},
	{RFModulatorRecords::ION_PUMP_VOLTAGE,				   this->update_ION_PUMP_VOLTAGE},
	{RFModulatorRecords::ION_PUMP_CURRENT,				   this->update_ION_PUMP_CURRENT},
	{RFModulatorRecords::SUPPLY_TEMP,					   this->update_SUPPLY_TEMP},
	{RFModulatorRecords::SUPPLY_PRESSURE,				   this->update_SUPPLY_PRESSURE},
	{RFModulatorRecords::RETURN_TEMP,					   this->update_RETURN_TEMP},
	{RFModulatorRecords::RETURN_PRESSURE,				   this->update_RETURN_PRESSURE},
	{RFModulatorRecords::BODY_FLOW_RATE,				   this->update_BODY_FLOW_RATE},
	{RFModulatorRecords::COLLECTOR_FLOW_RATE,			   this->update_COLLECTOR_FLOW_RATE},
	{RFModulatorRecords::SOLENOID_FLOW_RATE,			   this->update_SOLENOID_FLOW_RATE},
	{RFModulatorRecords::TANK_FLOW_RATE,				   this->update_TANK_FLOW_RATE},
	{RFModulatorRecords::COLLECTOR_RETURN_RATE,			   this->update_COLLECTOR_RETURN_RATE},
	{RFModulatorRecords::BODY_RETURN_TEMPERATURE,		   this->update_BODY_RETURN_TEMPERATURE},
	{RFModulatorRecords::SOLENOID_1_VOLTAGE,			   this->update_SOLENOID_1_VOLTAGE},
	{RFModulatorRecords::SOLENOID_2_VOLTAGE,			   this->update_SOLENOID_2_VOLTAGE},
	{RFModulatorRecords::SOLENOID_3_VOLTAGE,			   this->update_SOLENOID_3_VOLTAGE},
	{RFModulatorRecords::SOLENOID_1_CURRENT,			   this->update_SOLENOID_1_CURRENT},
	{RFModulatorRecords::SOLENOID_2_CURRENT,			   this->update_SOLENOID_2_CURRENT},
	{RFModulatorRecords::SOLENOID_3_CURRENT,			   this->update_SOLENOID_3_CURRENT},
	{RFModulatorRecords::ERROR_WORD_1 , this->update_ERROR_WORD_1},
	{RFModulatorRecords::ERROR_WORD_2 , this->update_ERROR_WORD_2},
	{RFModulatorRecords::ERROR_WORD_3 , this->update_ERROR_WORD_3},
	{RFModulatorRecords::ERROR_WORD_4 , this->update_ERROR_WORD_4},
	{RFModulatorRecords::ERROR_WORD_5 , this->update_ERROR_WORD_5},
	{RFModulatorRecords::ERROR_WORD_6 , this->update_ERROR_WORD_6},
	{RFModulatorRecords::ERROR_WORD_7 , this->update_ERROR_WORD_7},
	{RFModulatorRecords::ERROR_WORD_8 , this->update_ERROR_WORD_8},
	{RFModulatorRecords::ERROR_WORD_9 , this->update_ERROR_WORD_9},
	{RFModulatorRecords::ERROR_WORD_10 , this->update_ERROR_WORD_10},
	{RFModulatorRecords::ERROR_WORD_11 , this->update_ERROR_WORD_11},
	{RFModulatorRecords::ERROR_WORD_12 , this->update_ERROR_WORD_12},
	{RFModulatorRecords::ERROR_WORD_13 , this->update_ERROR_WORD_13},
	{RFModulatorRecords::ERROR_WORD_14 , this->update_ERROR_WORD_14},
	{RFModulatorRecords::ERROR_WORD_15 , this->update_ERROR_WORD_15},
	{RFModulatorRecords::ERROR_WORD_16 , this->update_ERROR_WORD_16},
	{RFModulatorRecords::ERROR_WORD_17 , this->update_ERROR_WORD_17},
	{RFModulatorRecords::ERROR_WORD_18 , this->update_ERROR_WORD_18},
	{RFModulatorRecords::ERROR_WORD_19 , this->update_ERROR_WORD_19},
	{RFModulatorRecords::ERROR_WORD_20 , this->update_ERROR_WORD_20},
	{RFModulatorRecords::ERROR_DESC_1 , this->update_ERROR_DESC_1},
	{RFModulatorRecords::ERROR_DESC_2 , this->update_ERROR_DESC_2},
	{RFModulatorRecords::ERROR_DESC_3 , this->update_ERROR_DESC_3},
	{RFModulatorRecords::ERROR_DESC_4 , this->update_ERROR_DESC_4},
	{RFModulatorRecords::ERROR_DESC_5 , this->update_ERROR_DESC_5},
	{RFModulatorRecords::ERROR_DESC_6 , this->update_ERROR_DESC_6},
	{RFModulatorRecords::ERROR_DESC_7 , this->update_ERROR_DESC_7},
	{RFModulatorRecords::ERROR_DESC_8 , this->update_ERROR_DESC_8},
	{RFModulatorRecords::ERROR_DESC_9 , this->update_ERROR_DESC_9},
	{RFModulatorRecords::ERROR_DESC_10 , this->update_ERROR_DESC_10},
	{RFModulatorRecords::ERROR_DESC_11 , this->update_ERROR_DESC_11},
	{RFModulatorRecords::ERROR_DESC_12 , this->update_ERROR_DESC_12},
	{RFModulatorRecords::ERROR_DESC_13 , this->update_ERROR_DESC_13},
	{RFModulatorRecords::ERROR_DESC_14 , this->update_ERROR_DESC_14},
	{RFModulatorRecords::ERROR_DESC_15 , this->update_ERROR_DESC_15},
	{RFModulatorRecords::ERROR_DESC_16 , this->update_ERROR_DESC_16},
	{RFModulatorRecords::ERROR_DESC_17 , this->update_ERROR_DESC_17},
	{RFModulatorRecords::ERROR_DESC_18 , this->update_ERROR_DESC_18},
	{RFModulatorRecords::ERROR_DESC_19 , this->update_ERROR_DESC_19},
	{RFModulatorRecords::ERROR_DESC_20 , this->update_ERROR_DESC_20},
	{RFModulatorRecords::GUN_MOD_RESET , this->update_GUN_MOD_RESET},
	{RFModulatorRecords::SYSTEM_STATE_READ,         this->update_SYSTEM_STATE_READ },
	{RFModulatorRecords::HVPS_VOLTAGE_SET_READ,		this->update_HVPS_VOLTAGE_SET_READ },
	{RFModulatorRecords::HVPS_VOLTAGE_LOW_ALARM_SET,this->update_HVPS_VOLTAGE_LOW_ALARM_SET },
	{RFModulatorRecords::HVPS_VOLTAGE_HI_ALARM_SET, this->update_HVPS_VOLTAGE_HI_ALARM_SET },
	{RFModulatorRecords::HVPS_VOLTAGE_READ,			this->update_HVPS_VOLTAGE_READ },
	{RFModulatorRecords::HVPS_CURRENT_READ,			this->update_HVPS_CURRENT_READ },
	{RFModulatorRecords::HEATER_VOLTAGE,			this->update_HEATER_VOLTAGE },
	{RFModulatorRecords::HEATER_CURRENT,			this->update_HEATER_CURRENT },
	{RFModulatorRecords::RESET_VOLTAGE,				this->update_RESET_VOLTAGE },
	{RFModulatorRecords::RESET_CURRENT,				this->update_RESET_CURRENT },
	{RFModulatorRecords::ION_PUMP_VOLTAGE,			this->update_ION_PUMP_VOLTAGE },
	{RFModulatorRecords::ION_PUMP_CURRENT,			this->update_ION_PUMP_CURRENT },
	{RFModulatorRecords::SUPPLY_TEMP,				this->update_SUPPLY_TEMP },
	{RFModulatorRecords::SUPPLY_PRESSURE,			this->update_SUPPLY_PRESSURE },
	{RFModulatorRecords::RETURN_TEMP,				this->update_RETURN_TEMP },
	{RFModulatorRecords::RETURN_PRESSURE,			this->update_RETURN_PRESSURE },
	{RFModulatorRecords::BODY_FLOW_RATE,			this->update_BODY_FLOW_RATE },
	{RFModulatorRecords::COLLECTOR_FLOW_RATE,		this->update_COLLECTOR_FLOW_RATE },
	{RFModulatorRecords::SOLENOID_FLOW_RATE,		this->update_SOLENOID_FLOW_RATE },
	{RFModulatorRecords::TANK_FLOW_RATE,			this->update_TANK_FLOW_RATE },
	{RFModulatorRecords::COLLECTOR_RETURN_RATE,		this->update_COLLECTOR_RETURN_RATE },
	{RFModulatorRecords::BODY_RETURN_TEMPERATURE,	this->update_BODY_RETURN_TEMPERATURE },
	{RFModulatorRecords::SOLENOID_1_VOLTAGE,		this->update_SOLENOID_1_VOLTAGE },
	{RFModulatorRecords::SOLENOID_2_VOLTAGE,		this->update_SOLENOID_2_VOLTAGE },
	{RFModulatorRecords::SOLENOID_3_VOLTAGE,		this->update_SOLENOID_3_VOLTAGE },
	{RFModulatorRecords::SOLENOID_1_CURRENT,		this->update_SOLENOID_1_CURRENT },
	{RFModulatorRecords::SOLENOID_2_CURRENT,		this->update_SOLENOID_2_CURRENT },
	{RFModulatorRecords::SOLENOID_3_CURRENT,		this->update_SOLENOID_3_CURRENT }




	};
    

};
