#include <EPICSRFModulatorInterface.h>
#include <RFModulator.h>
#include <RFModulatorPVRecords.h>
#include <GlobalFunctions.h>


EPICSRFModulatorInterface::EPICSRFModulatorInterface()
{
	//updateFunctionMap[RFModulatorRecords::ERROR_WORD_1] = this->update_ERROR_WORD_1;
	//updateFunctionMap[RFModulatorRecords::ERROR_WORD_2] = this->update_ERROR_WORD_2;
;
}

EPICSRFModulatorInterface::~EPICSRFModulatorInterface()
{
}



void EPICSRFModulatorInterface::retrieveUpdateFunctionForRecord(const pvStruct& pv)const
{
	if (GlobalFunctions::entryExists(updateFunctionMap, pv.pvRecord))
	{
		pv.updateFunction = updateFunctionMap.at(pv.pvRecord);
	}
	else
	{
		messenger.printDebugMessage("!!WARNING!! NO UPDATE FUNCTION FOUND FOR: " + pv.pvRecord);
		pv.updateFunction = nullptr;
	}
}


void EPICSRFModulatorInterface::update_HOLD_RF_ON()
{

}
// GUN void
void EPICSRFModulatorInterface::update_GUN_MOD_RESET(const struct event_handler_args args)
{

}
void EPICSRFModulatorInterface::update_GUN_MOD_STATE_SET(const struct event_handler_args args)
{

}
void EPICSRFModulatorInterface::update_GUN_MOD_STATE_SET_READ(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_GUN_MOD_MAIN_STATE_READ(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_GUN_MOD_ERR_SVAL(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_GUN_MOD_ILOCK1(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_GUN_MOD_ILOCK2(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_GUN_MOD_ILOCK3(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_GUN_MOD_ILOCK4(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_GUN_MOD_ILOCK5(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_GUN_MOD_WARMUPT(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_GUN_MOD_MAGPS1_CURR_READ(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_GUN_MOD_MAGPS2_CURR_READ(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_GUN_MOD_MAGPS3_CURR_READ(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_GUN_MOD_MAGPS4_CURR_READ(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_GUN_MOD_MAGPS1_VOLT_READ(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_GUN_MOD_MAGPS2_VOLT_READ(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_GUN_MOD_MAGPS3_VOLT_READ(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_GUN_MOD_MAGPS4_VOLT_READ(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_GUN_MOD_HVPS1_CURR_READ(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_GUN_MOD_HVPS2_CURR_READ(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_GUN_MOD_HVPS3_CURR_READ(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_GUN_MOD_HVPS1_VOLT_READ(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_GUN_MOD_HVPS2_VOLT_READ(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_GUN_MOD_HVPS3_VOLT_READ(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_GUN_MOD_CT_READ(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_GUN_MOD_CVD_READ(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_GUN_MOD_PULSE_WIDTH_READ(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_GUN_MOD_PULSE_WIDTH_FWHM_READ(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_GUN_MOD_IONP_PRESSURE_READ(const struct event_handler_args args){}
// L01
void EPICSRFModulatorInterface::update_ON(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_SYSTEM_STATE_PUT(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_HVPS_VOLTAGE_SET(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_HVPS_VOLTAGE_LOW_ALARM_SET(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_HVPS_VOLTAGE_HI_ALARM_SET(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_L01_RESET(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_SYSTEM_STATE_READ(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_HVPS_VOLTAGE_SET_READ(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_HVPS_VOLTAGE_LOW_ALARM_SET_READ(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_HVPS_VOLTAGE_HI_ALARM_SET_READ(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_HVPS_VOLTAGE_READ(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_HVPS_CURRENT_READ(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_HEATER_VOLTAGE(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_HEATER_CURRENT(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_RESET_VOLTAGE(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_RESET_CURRENT(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_ION_PUMP_VOLTAGE(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_ION_PUMP_CURRENT(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_SUPPLY_TEMP(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_SUPPLY_PRESSURE(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_RETURN_TEMP(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_RETURN_PRESSURE(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_BODY_FLOW_RATE(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_COLLECTOR_FLOW_RATE(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_SOLENOID_FLOW_RATE(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_TANK_FLOW_RATE(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_COLLECTOR_RETURN_RATE(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_BODY_RETURN_TEMPERATURE(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_SOLENOID_1_VOLTAGE(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_SOLENOID_2_VOLTAGE(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_SOLENOID_3_VOLTAGE(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_SOLENOID_1_CURRENT(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_SOLENOID_2_CURRENT(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_SOLENOID_3_CURRENT(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_L01_FAULT(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_ERROR_WORD_1(const struct event_handler_args args)
{
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelString(RFModulatorRecords::ERROR_DESC_1, getTimeStampStringPair(args));

	

}
void EPICSRFModulatorInterface::update_ERROR_WORD_2(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_ERROR_WORD_3(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_ERROR_WORD_4(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_ERROR_WORD_5(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_ERROR_WORD_6(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_ERROR_WORD_7(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_ERROR_WORD_8(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_ERROR_WORD_9(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_ERROR_WORD_10(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_ERROR_WORD_11(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_ERROR_WORD_12(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_ERROR_WORD_13(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_ERROR_WORD_14(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_ERROR_WORD_15(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_ERROR_WORD_16(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_ERROR_WORD_17(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_ERROR_WORD_18(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_ERROR_WORD_19(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_ERROR_WORD_20(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_ERROR_DESC_1(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_ERROR_DESC_2(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_ERROR_DESC_3(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_ERROR_DESC_4(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_ERROR_DESC_5(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_ERROR_DESC_6(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_ERROR_DESC_7(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_ERROR_DESC_8(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_ERROR_DESC_9(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_ERROR_DESC_10(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_ERROR_DESC_11(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_ERROR_DESC_12(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_ERROR_DESC_13(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_ERROR_DESC_14(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_ERROR_DESC_15(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_ERROR_DESC_16(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_ERROR_DESC_17(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_ERROR_DESC_18(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_ERROR_DESC_19(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_ERROR_DESC_20(const struct event_handler_args args){}


