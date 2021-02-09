#include <EPICSRFModulatorInterface.h>
#include <RFModulator.h>
#include <RFModulatorPVRecords.h>
#include <GlobalFunctions.h>

LoggingSystem EPICSRFModulatorInterface::messenger;

EPICSRFModulatorInterface::EPICSRFModulatorInterface()
{
	messenger = LoggingSystem(true, true);
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
		//messenger.printDebugMessage("!!WARNING!! NO UPDATE FUNCTION FOUND FOR: " + pv.pvRecord);
		pv.updateFunction = nullptr;
	}
}


void EPICSRFModulatorInterface::update_HOLD_RF_ON()
{

}
// GUN void
void EPICSRFModulatorInterface::update_GUN_MOD_RESET(const struct event_handler_args args)
{
	messenger.printMessage("update_GUN_MOD_RESET");

}
void EPICSRFModulatorInterface::update_GUN_MOD_STATE_SET(const struct event_handler_args args)
{

}
void EPICSRFModulatorInterface::update_GUN_MOD_STATE_SET_READ(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_GUN_MOD_MAIN_STATE_READ(const struct event_handler_args args)
{
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);

	std::pair<epicsTimeStamp, unsigned short > r = getTimeStampEnumPair(args);
	mod->main_state_string.first = r.first;
	mod->main_state.first = r.first;
	switch (r.second)
		{
		case 0:
			mod->main_state_string.second = "Init/not conne.";
			mod->main_state.second = STATE::NOT_CONNECTED;
			break;
		case 1:
			mod->main_state.second = STATE::STANDYBY_INTERLOCK;
			mod->main_state_string.second = "Standby Interl.";
			break;
		case 2:
			mod -> main_state.second = STATE::OFF;
			mod -> main_state_string.second = "Off";
			break;
		case 3:
			mod -> main_state.second = STATE::OFF_REQUEST;
			mod -> main_state_string.second = "Off Request";
			break;
		case 4:
			mod -> main_state.second = STATE::HV_INTERLOCK;
			mod -> main_state_string.second = "HV Intrlock";
			break;
		case 5:
			mod -> main_state.second = STATE::STANDBY_REQUEST;
			mod -> main_state_string.second = "Standby Request";
			break;
		case 6:
			mod -> main_state.second = STATE::STANDBY;
			mod -> main_state_string.second = "Standby";
			break;
		case 7:
			mod -> main_state.second = STATE::HV_OFF_REQUEST;
			mod -> main_state_string.second = "HV Off Requ.";
			break;
		case 8:
			mod -> main_state.second = STATE::RF_ON_INTERLOCK;
			mod -> main_state_string.second = "RF On Interl.";
			break;
		case 9:
			mod -> main_state.second = STATE::HV_REQUEST;
			mod -> main_state_string.second = "HV Request";
			break;
		case 10:
			mod -> main_state.second = STATE::HV_ON;
			mod -> main_state_string.second = "HV On";
			break;
		case 11:
			mod -> main_state.second = STATE::RF_OFF_REQUEST;
			mod -> main_state_string.second = "RF Off Req.";
			break;
		case 12:
			mod -> main_state.second = STATE::RF_ON_REQUEST;
			mod -> main_state_string.second = "RF On Request";
			break;
		case 13:
			mod -> main_state.second = STATE::RF_ON;
			mod -> main_state_string.second = "RF On";
			break;
		default:
			mod -> main_state.second = STATE::UNKNOWN_STATE;
			mod -> main_state_string.second = "Unknown";
			break;
	}
	messenger.printMessage(mod->getHardwareName(), " state changed to ", ENUM_TO_STRING(mod->main_state.second));
}
void EPICSRFModulatorInterface::update_GUN_MOD_ERR_SVAL(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_GUN_MOD_ILOCK1(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_GUN_MOD_ILOCK2(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_GUN_MOD_ILOCK3(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_GUN_MOD_ILOCK4(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_GUN_MOD_ILOCK5(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_GUN_MOD_WARMUPT(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_GUN_MOD_MAGPS1_CURR_READ(const struct event_handler_args args)
{
	messenger.printMessage("update_GUN_MOD_MAGPS1_CURR_READ");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelDouble(RFModulatorRecords::GUN_MOD_MAGPS1_CURR_READ, getTimeStampDoublePair(args));
}
void EPICSRFModulatorInterface::update_GUN_MOD_MAGPS2_CURR_READ(const struct event_handler_args args)
{
	messenger.printMessage("update_GUN_MOD_MAGPS2_CURR_READ");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelDouble(RFModulatorRecords::GUN_MOD_MAGPS2_CURR_READ, getTimeStampDoublePair(args));
}
void EPICSRFModulatorInterface::update_GUN_MOD_MAGPS3_CURR_READ(const struct event_handler_args args)
{
	messenger.printMessage("update_GUN_MOD_MAGPS3_CURR_READ");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelDouble(RFModulatorRecords::GUN_MOD_MAGPS3_CURR_READ, getTimeStampDoublePair(args));
}
void EPICSRFModulatorInterface::update_GUN_MOD_MAGPS4_CURR_READ(const struct event_handler_args args)
{
	messenger.printMessage("update_GUN_MOD_MAGPS4_CURR_READ");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelDouble(RFModulatorRecords::GUN_MOD_MAGPS4_CURR_READ, getTimeStampDoublePair(args));
}
void EPICSRFModulatorInterface::update_GUN_MOD_MAGPS1_VOLT_READ(const struct event_handler_args args)
{
	messenger.printMessage("update_GUN_MOD_MAGPS1_VOLT_READ");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelDouble(RFModulatorRecords::GUN_MOD_MAGPS1_VOLT_READ, getTimeStampDoublePair(args));
}
void EPICSRFModulatorInterface::update_GUN_MOD_MAGPS2_VOLT_READ(const struct event_handler_args args)
{
	messenger.printMessage("update_HVPS_VOLTAGE_SET");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelDouble(RFModulatorRecords::GUN_MOD_MAGPS2_VOLT_READ, getTimeStampDoublePair(args));
}
void EPICSRFModulatorInterface::update_GUN_MOD_MAGPS3_VOLT_READ(const struct event_handler_args args)
{
	messenger.printMessage("update_GUN_MOD_MAGPS3_VOLT_READ");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelDouble(RFModulatorRecords::GUN_MOD_MAGPS3_VOLT_READ, getTimeStampDoublePair(args));
}
void EPICSRFModulatorInterface::update_GUN_MOD_MAGPS4_VOLT_READ(const struct event_handler_args args)
{
	messenger.printMessage("update_GUN_MOD_MAGPS4_VOLT_READ");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelDouble(RFModulatorRecords::GUN_MOD_MAGPS4_VOLT_READ, getTimeStampDoublePair(args));
}
void EPICSRFModulatorInterface::update_GUN_MOD_HVPS1_CURR_READ(const struct event_handler_args args)
{
	messenger.printMessage("update_GUN_MOD_HVPS1_CURR_READ");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelDouble(RFModulatorRecords::GUN_MOD_HVPS1_CURR_READ, getTimeStampDoublePair(args));
}
void EPICSRFModulatorInterface::update_GUN_MOD_HVPS2_CURR_READ(const struct event_handler_args args)
{
	messenger.printMessage("update_GUN_MOD_HVPS2_CURR_READ");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelDouble(RFModulatorRecords::GUN_MOD_HVPS2_CURR_READ, getTimeStampDoublePair(args));
}
void EPICSRFModulatorInterface::update_GUN_MOD_HVPS3_CURR_READ(const struct event_handler_args args)
{
	messenger.printMessage("update_GUN_MOD_HVPS3_CURR_READ");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelDouble(RFModulatorRecords::GUN_MOD_HVPS3_CURR_READ, getTimeStampDoublePair(args));
}
void EPICSRFModulatorInterface::update_GUN_MOD_HVPS1_VOLT_READ(const struct event_handler_args args)
{
	messenger.printMessage("update_GUN_MOD_HVPS1_VOLT_READ");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelDouble(RFModulatorRecords::GUN_MOD_HVPS1_VOLT_READ, getTimeStampDoublePair(args));
}
void EPICSRFModulatorInterface::update_GUN_MOD_HVPS2_VOLT_READ(const struct event_handler_args args)
{
	messenger.printMessage("update_GUN_MOD_HVPS2_VOLT_READ");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelDouble(RFModulatorRecords::GUN_MOD_HVPS2_VOLT_READ, getTimeStampDoublePair(args));
}
void EPICSRFModulatorInterface::update_GUN_MOD_HVPS3_VOLT_READ(const struct event_handler_args args)
{
	messenger.printMessage("update_GUN_MOD_HVPS3_VOLT_READ");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelDouble(RFModulatorRecords::GUN_MOD_HVPS3_VOLT_READ, getTimeStampDoublePair(args));
}
void EPICSRFModulatorInterface::update_GUN_MOD_CT_READ(const struct event_handler_args args)
{
	messenger.printMessage("update_GUN_MOD_CT_READ");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelDouble(RFModulatorRecords::GUN_MOD_CT_READ, getTimeStampDoublePair(args));
}
void EPICSRFModulatorInterface::update_GUN_MOD_CVD_READ(const struct event_handler_args args)
{
	messenger.printMessage("update_GUN_MOD_CVD_READ");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelDouble(RFModulatorRecords::GUN_MOD_CVD_READ, getTimeStampDoublePair(args));
}
void EPICSRFModulatorInterface::update_GUN_MOD_PULSE_WIDTH_READ(const struct event_handler_args args)
{
	messenger.printMessage("update_GUN_MOD_PULSE_WIDTH_READ");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelDouble(RFModulatorRecords::GUN_MOD_PULSE_WIDTH_READ, getTimeStampDoublePair(args));
}
void EPICSRFModulatorInterface::update_GUN_MOD_PULSE_WIDTH_FWHM_READ(const struct event_handler_args args)
{
	messenger.printMessage("update_GUN_MOD_PULSE_WIDTH_FWHM_READ");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelDouble(RFModulatorRecords::GUN_MOD_PULSE_WIDTH_FWHM_READ, getTimeStampDoublePair(args));
}
void EPICSRFModulatorInterface::update_GUN_MOD_IONP_PRESSURE_READ(const struct event_handler_args args)
{
	messenger.printMessage("update_GUN_MOD_IONP_PRESSURE_READ");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelDouble(RFModulatorRecords::GUN_MOD_IONP_PRESSURE_READ, getTimeStampDoublePair(args));
}
// L01
void EPICSRFModulatorInterface::update_ON(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_SYSTEM_STATE_PUT(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_HVPS_VOLTAGE_SET(const struct event_handler_args args)
{
	messenger.printMessage("update_HVPS_VOLTAGE_SET");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelDouble(RFModulatorRecords::ERROR_WORD_1, getTimeStampDoublePair(args));
}
void EPICSRFModulatorInterface::update_HVPS_VOLTAGE_LOW_ALARM_SET(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_HVPS_VOLTAGE_HI_ALARM_SET(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_L01_RESET(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_SYSTEM_STATE_READ(const struct event_handler_args args)
{
	messenger.printMessage("update_SYSTEM_STATE_READ NEEDS TO BE WRITTEN");
	//RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	//mod->updateLowLevelString(RFModulatorRecords::SYSTEM_STATE_READ, getTimeStampStringPair(args));
}
void EPICSRFModulatorInterface::update_HVPS_VOLTAGE_SET_READ(const struct event_handler_args args)
{
	messenger.printMessage("update_HVPS_VOLTAGE_SET_READ");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelDouble(RFModulatorRecords::HVPS_VOLTAGE_SET_READ, getTimeStampDoublePair(args));
}
void EPICSRFModulatorInterface::update_HVPS_VOLTAGE_LOW_ALARM_SET_READ(const struct event_handler_args args)
{
	messenger.printMessage("update_HVPS_VOLTAGE_LOW_ALARM_SET_READ");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelDouble(RFModulatorRecords::HVPS_VOLTAGE_LOW_ALARM_SET_READ, getTimeStampDoublePair(args));
}
void EPICSRFModulatorInterface::update_HVPS_VOLTAGE_HI_ALARM_SET_READ(const struct event_handler_args args)
{
	messenger.printMessage("update_HVPS_VOLTAGE_HI_ALARM_SET_READ");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelDouble(RFModulatorRecords::HVPS_VOLTAGE_HI_ALARM_SET_READ, getTimeStampDoublePair(args));
}
void EPICSRFModulatorInterface::update_HVPS_VOLTAGE_READ(const struct event_handler_args args)
{
	messenger.printMessage("update_HVPS_VOLTAGE_READ");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelDouble(RFModulatorRecords::HVPS_VOLTAGE_READ, getTimeStampDoublePair(args));
}
void EPICSRFModulatorInterface::update_HVPS_CURRENT_READ(const struct event_handler_args args)
{
	messenger.printMessage("update_HVPS_CURRENT_READ");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelDouble(RFModulatorRecords::HVPS_CURRENT_READ, getTimeStampDoublePair(args));
}
void EPICSRFModulatorInterface::update_HEATER_VOLTAGE(const struct event_handler_args args)
{
	messenger.printMessage("update_HEATER_VOLTAGE");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelDouble(RFModulatorRecords::HEATER_VOLTAGE, getTimeStampDoublePair(args));
}
void EPICSRFModulatorInterface::update_HEATER_CURRENT(const struct event_handler_args args)
{
	messenger.printMessage("update_HEATER_CURRENT");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelDouble(RFModulatorRecords::HEATER_CURRENT, getTimeStampDoublePair(args));
}
void EPICSRFModulatorInterface::update_RESET_VOLTAGE(const struct event_handler_args args)
{
	messenger.printMessage("update_RESET_VOLTAGE");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelDouble(RFModulatorRecords::RESET_VOLTAGE, getTimeStampDoublePair(args));
}
void EPICSRFModulatorInterface::update_RESET_CURRENT(const struct event_handler_args args)
{
	messenger.printMessage("update_RESET_CURRENT");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelDouble(RFModulatorRecords::RESET_CURRENT, getTimeStampDoublePair(args));
}
void EPICSRFModulatorInterface::update_ION_PUMP_VOLTAGE(const struct event_handler_args args)
{
	messenger.printMessage("update_ION_PUMP_VOLTAGE");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelDouble(RFModulatorRecords::ION_PUMP_VOLTAGE, getTimeStampDoublePair(args));
}
void EPICSRFModulatorInterface::update_ION_PUMP_CURRENT(const struct event_handler_args args)
{
	messenger.printMessage("update_ION_PUMP_CURRENT");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelDouble(RFModulatorRecords::ION_PUMP_CURRENT, getTimeStampDoublePair(args));
}
void EPICSRFModulatorInterface::update_SUPPLY_TEMP(const struct event_handler_args args)
{
	messenger.printMessage("update_SUPPLY_TEMP");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelDouble(RFModulatorRecords::SUPPLY_TEMP, getTimeStampDoublePair(args));
}
void EPICSRFModulatorInterface::update_SUPPLY_PRESSURE(const struct event_handler_args args)
{
	messenger.printMessage("update_SUPPLY_PRESSURE");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelDouble(RFModulatorRecords::SUPPLY_PRESSURE, getTimeStampDoublePair(args));
}
void EPICSRFModulatorInterface::update_RETURN_TEMP(const struct event_handler_args args)
{
	messenger.printMessage("update_RETURN_TEMP");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelDouble(RFModulatorRecords::RETURN_TEMP, getTimeStampDoublePair(args));
}
void EPICSRFModulatorInterface::update_RETURN_PRESSURE(const struct event_handler_args args)
{
	messenger.printMessage("update_RETURN_PRESSURE");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelDouble(RFModulatorRecords::RETURN_PRESSURE, getTimeStampDoublePair(args));
}
void EPICSRFModulatorInterface::update_BODY_FLOW_RATE(const struct event_handler_args args)
{
	messenger.printMessage("update_BODY_FLOW_RATE");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelDouble(RFModulatorRecords::BODY_FLOW_RATE, getTimeStampDoublePair(args));
}
void EPICSRFModulatorInterface::update_COLLECTOR_FLOW_RATE(const struct event_handler_args args)
{
	messenger.printMessage("update_COLLECTOR_FLOW_RATE");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelDouble(RFModulatorRecords::COLLECTOR_FLOW_RATE, getTimeStampDoublePair(args));
}
void EPICSRFModulatorInterface::update_SOLENOID_FLOW_RATE(const struct event_handler_args args)
{
	messenger.printMessage("update_SOLENOID_3_CURRENT");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelDouble(RFModulatorRecords::SOLENOID_3_CURRENT, getTimeStampDoublePair(args));
}
void EPICSRFModulatorInterface::update_TANK_FLOW_RATE(const struct event_handler_args args)
{
	messenger.printMessage("update_TANK_FLOW_RATE");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelDouble(RFModulatorRecords::TANK_FLOW_RATE, getTimeStampDoublePair(args));
}
void EPICSRFModulatorInterface::update_COLLECTOR_RETURN_RATE(const struct event_handler_args args)
{
	messenger.printMessage("update_COLLECTOR_RETURN_RATE");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelDouble(RFModulatorRecords::COLLECTOR_RETURN_RATE, getTimeStampDoublePair(args));
}
void EPICSRFModulatorInterface::update_BODY_RETURN_TEMPERATURE(const struct event_handler_args args)
{
	messenger.printMessage("update_BODY_RETURN_TEMPERATURE");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelDouble(RFModulatorRecords::BODY_RETURN_TEMPERATURE, getTimeStampDoublePair(args));
}
void EPICSRFModulatorInterface::update_SOLENOID_1_VOLTAGE(const struct event_handler_args args)
{
	messenger.printMessage("update_SOLENOID_1_VOLTAGE");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelDouble(RFModulatorRecords::SOLENOID_1_VOLTAGE, getTimeStampDoublePair(args));
}
void EPICSRFModulatorInterface::update_SOLENOID_2_VOLTAGE(const struct event_handler_args args)
{
	messenger.printMessage("update_SOLENOID_2_VOLTAGE");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelDouble(RFModulatorRecords::SOLENOID_2_VOLTAGE, getTimeStampDoublePair(args));
}
void EPICSRFModulatorInterface::update_SOLENOID_3_VOLTAGE(const struct event_handler_args args)
{
	messenger.printMessage("update_SOLENOID_3_VOLTAGE");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelDouble(RFModulatorRecords::SOLENOID_3_VOLTAGE, getTimeStampDoublePair(args));
}
void EPICSRFModulatorInterface::update_SOLENOID_1_CURRENT(const struct event_handler_args args)
{
	messenger.printMessage("update_SOLENOID_3_CURRENT");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelDouble(RFModulatorRecords::SOLENOID_3_CURRENT, getTimeStampDoublePair(args));
}
void EPICSRFModulatorInterface::update_SOLENOID_2_CURRENT(const struct event_handler_args args)
{
	messenger.printMessage("update_SOLENOID_3_CURRENT");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelDouble(RFModulatorRecords::SOLENOID_3_CURRENT, getTimeStampDoublePair(args));
}
void EPICSRFModulatorInterface::update_SOLENOID_3_CURRENT(const struct event_handler_args args)
{
	messenger.printMessage("update_SOLENOID_3_CURRENT");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelDouble(RFModulatorRecords::SOLENOID_3_CURRENT, getTimeStampDoublePair(args));
}
void EPICSRFModulatorInterface::update_L01_FAULT(const struct event_handler_args args){}
void EPICSRFModulatorInterface::update_ERROR_WORD_1(const struct event_handler_args args)
{
	messenger.printMessage("update_ERROR_WORD_1");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelString(RFModulatorRecords::ERROR_WORD_1, getTimeStampStringPair(args));
}
void EPICSRFModulatorInterface::update_ERROR_WORD_2(const struct event_handler_args args)
{
	messenger.printMessage("update_ERROR_WORD_2");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelString(RFModulatorRecords::ERROR_WORD_2, getTimeStampStringPair(args));
}
void EPICSRFModulatorInterface::update_ERROR_WORD_3(const struct event_handler_args args)
{
	messenger.printMessage("update_ERROR_WORD_3");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelString(RFModulatorRecords::ERROR_WORD_3, getTimeStampStringPair(args));
}
void EPICSRFModulatorInterface::update_ERROR_WORD_4(const struct event_handler_args args)
{
	messenger.printMessage("update_ERROR_WORD_4");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelString(RFModulatorRecords::ERROR_WORD_4, getTimeStampStringPair(args));
}
void EPICSRFModulatorInterface::update_ERROR_WORD_5(const struct event_handler_args args)
{
	messenger.printMessage("update_ERROR_WORD_5");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelString(RFModulatorRecords::ERROR_WORD_5, getTimeStampStringPair(args));
}
void EPICSRFModulatorInterface::update_ERROR_WORD_6(const struct event_handler_args args)
{
	messenger.printMessage("update_ERROR_WORD_6");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelString(RFModulatorRecords::ERROR_WORD_6, getTimeStampStringPair(args));
}
void EPICSRFModulatorInterface::update_ERROR_WORD_7(const struct event_handler_args args)
{
	messenger.printMessage("update_ERROR_WORD_7");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelString(RFModulatorRecords::ERROR_WORD_7, getTimeStampStringPair(args));
}
void EPICSRFModulatorInterface::update_ERROR_WORD_8(const struct event_handler_args args)
{
	messenger.printMessage("update_ERROR_WORD_8");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelString(RFModulatorRecords::ERROR_WORD_8, getTimeStampStringPair(args));
}
void EPICSRFModulatorInterface::update_ERROR_WORD_9(const struct event_handler_args args)
{
	messenger.printMessage("update_ERROR_WORD_9");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelString(RFModulatorRecords::ERROR_WORD_9, getTimeStampStringPair(args));
}
void EPICSRFModulatorInterface::update_ERROR_WORD_10(const struct event_handler_args args)
{
	messenger.printMessage("update_ERROR_WORD_10");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelString(RFModulatorRecords::ERROR_WORD_10, getTimeStampStringPair(args));
}
void EPICSRFModulatorInterface::update_ERROR_WORD_11(const struct event_handler_args args)
{
	messenger.printMessage("update_ERROR_WORD_11");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelString(RFModulatorRecords::ERROR_WORD_11, getTimeStampStringPair(args));
}
void EPICSRFModulatorInterface::update_ERROR_WORD_12(const struct event_handler_args args)
{
	messenger.printMessage("update_ERROR_WORD_12");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelString(RFModulatorRecords::ERROR_WORD_12, getTimeStampStringPair(args));
}
void EPICSRFModulatorInterface::update_ERROR_WORD_13(const struct event_handler_args args)
{
	messenger.printMessage("update_ERROR_WORD_13");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelString(RFModulatorRecords::ERROR_WORD_13, getTimeStampStringPair(args));
}
void EPICSRFModulatorInterface::update_ERROR_WORD_14(const struct event_handler_args args)
{
	messenger.printMessage("update_ERROR_WORD_14");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelString(RFModulatorRecords::ERROR_WORD_14, getTimeStampStringPair(args));
}
void EPICSRFModulatorInterface::update_ERROR_WORD_15(const struct event_handler_args args)
{
	messenger.printMessage("update_ERROR_WORD_15");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelString(RFModulatorRecords::ERROR_WORD_15, getTimeStampStringPair(args));
}
void EPICSRFModulatorInterface::update_ERROR_WORD_16(const struct event_handler_args args)
{
	messenger.printMessage("update_ERROR_WORD_16");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelString(RFModulatorRecords::ERROR_WORD_16, getTimeStampStringPair(args));
}
void EPICSRFModulatorInterface::update_ERROR_WORD_17(const struct event_handler_args args)
{
	messenger.printMessage("update_ERROR_WORD_17");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelString(RFModulatorRecords::ERROR_WORD_17, getTimeStampStringPair(args));
}
void EPICSRFModulatorInterface::update_ERROR_WORD_18(const struct event_handler_args args)
{
	messenger.printMessage("update_ERROR_WORD_18");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelString(RFModulatorRecords::ERROR_WORD_18, getTimeStampStringPair(args));
}
void EPICSRFModulatorInterface::update_ERROR_WORD_19(const struct event_handler_args args)
{
	messenger.printMessage("update_ERROR_WORD_19");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelString(RFModulatorRecords::ERROR_WORD_19, getTimeStampStringPair(args));
}
void EPICSRFModulatorInterface::update_ERROR_WORD_20(const struct event_handler_args args)
{
	messenger.printMessage("update_ERROR_WORD_20");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelString(RFModulatorRecords::ERROR_WORD_20, getTimeStampStringPair(args));
}
void EPICSRFModulatorInterface::update_ERROR_DESC_1(const struct event_handler_args args)
{
	messenger.printMessage("update_ERROR_DESC_1");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelString(RFModulatorRecords::ERROR_DESC_1, getTimeStampStringPair(args));
}
void EPICSRFModulatorInterface::update_ERROR_DESC_2(const struct event_handler_args args)
{
	messenger.printMessage("update_ERROR_DESC_2");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelString(RFModulatorRecords::ERROR_DESC_2, getTimeStampStringPair(args));
}
void EPICSRFModulatorInterface::update_ERROR_DESC_3(const struct event_handler_args args)
{
	messenger.printMessage("update_ERROR_DESC_3");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelString(RFModulatorRecords::ERROR_DESC_3, getTimeStampStringPair(args));
}
void EPICSRFModulatorInterface::update_ERROR_DESC_4(const struct event_handler_args args)
{
	messenger.printMessage("update_ERROR_DESC_4");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelString(RFModulatorRecords::ERROR_DESC_4, getTimeStampStringPair(args));
}
void EPICSRFModulatorInterface::update_ERROR_DESC_5(const struct event_handler_args args)
{
	messenger.printMessage("update_ERROR_DESC_5");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelString(RFModulatorRecords::ERROR_DESC_5, getTimeStampStringPair(args));
}
void EPICSRFModulatorInterface::update_ERROR_DESC_6(const struct event_handler_args args)
{
	messenger.printMessage("update_ERROR_DESC_6");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelString(RFModulatorRecords::ERROR_DESC_6, getTimeStampStringPair(args));
}
void EPICSRFModulatorInterface::update_ERROR_DESC_7(const struct event_handler_args args)
{
	messenger.printMessage("update_ERROR_DESC_7");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelString(RFModulatorRecords::ERROR_DESC_7, getTimeStampStringPair(args));
}
void EPICSRFModulatorInterface::update_ERROR_DESC_8(const struct event_handler_args args)
{
	messenger.printMessage("update_ERROR_DESC_8");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelString(RFModulatorRecords::ERROR_DESC_8, getTimeStampStringPair(args));
}
void EPICSRFModulatorInterface::update_ERROR_DESC_9(const struct event_handler_args args)
{
	messenger.printMessage("update_ERROR_DESC_9");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelString(RFModulatorRecords::ERROR_DESC_9, getTimeStampStringPair(args));
}
void EPICSRFModulatorInterface::update_ERROR_DESC_10(const struct event_handler_args args)
{
	messenger.printMessage("update_ERROR_DESC_10");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelString(RFModulatorRecords::ERROR_DESC_10, getTimeStampStringPair(args));
}
void EPICSRFModulatorInterface::update_ERROR_DESC_11(const struct event_handler_args args)
{
	messenger.printMessage("update_ERROR_DESC_11");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelString(RFModulatorRecords::ERROR_DESC_11, getTimeStampStringPair(args));
}
void EPICSRFModulatorInterface::update_ERROR_DESC_12(const struct event_handler_args args)
{
	messenger.printMessage("update_ERROR_DESC_12");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelString(RFModulatorRecords::ERROR_DESC_12, getTimeStampStringPair(args));
}
void EPICSRFModulatorInterface::update_ERROR_DESC_13(const struct event_handler_args args)
{
	messenger.printMessage("update_ERROR_DESC_13");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelString(RFModulatorRecords::ERROR_DESC_13, getTimeStampStringPair(args));
}
void EPICSRFModulatorInterface::update_ERROR_DESC_14(const struct event_handler_args args)
{
	messenger.printMessage("update_ERROR_DESC_14");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelString(RFModulatorRecords::ERROR_DESC_14, getTimeStampStringPair(args));
}
void EPICSRFModulatorInterface::update_ERROR_DESC_15(const struct event_handler_args args)
{
	messenger.printMessage("update_ERROR_DESC_15");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelString(RFModulatorRecords::ERROR_DESC_15, getTimeStampStringPair(args));
}
void EPICSRFModulatorInterface::update_ERROR_DESC_16(const struct event_handler_args args)
{
	messenger.printMessage("update_ERROR_DESC_16");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelString(RFModulatorRecords::ERROR_DESC_16, getTimeStampStringPair(args));
}
void EPICSRFModulatorInterface::update_ERROR_DESC_17(const struct event_handler_args args)
{
	messenger.printMessage("update_ERROR_DESC_17");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelString(RFModulatorRecords::ERROR_DESC_17, getTimeStampStringPair(args));
}
void EPICSRFModulatorInterface::update_ERROR_DESC_18(const struct event_handler_args args)
{
	messenger.printMessage("update_ERROR_DESC_18");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelString(RFModulatorRecords::ERROR_DESC_18, getTimeStampStringPair(args));
}
void EPICSRFModulatorInterface::update_ERROR_DESC_19(const struct event_handler_args args)
{
	messenger.printMessage("update_ERROR_DESC_19");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelString(RFModulatorRecords::ERROR_DESC_19, getTimeStampStringPair(args));
}
void EPICSRFModulatorInterface::update_ERROR_DESC_20(const struct event_handler_args args)
{
	messenger.printMessage("update_ERROR_DESC_20");
	RFModulator* mod = getHardwareFromArgs<RFModulator>(args);
	mod->updateLowLevelString(RFModulatorRecords::ERROR_DESC_20, getTimeStampStringPair(args));
}


