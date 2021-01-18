#include "EPICSLLRFInterface.h"
#include "LLRFPVRecords.h"
#include "GlobalFunctions.h"
#include "GlobalConstants.h"
#include "GlobalStateEnums.h"


LoggingSystem EPICSLLRFInterface::messenger;

EPICSLLRFInterface::EPICSLLRFInterface():
EPICSInterface()
{
	messenger = LoggingSystem(true, true);
}

EPICSLLRFInterface::EPICSLLRFInterface(const EPICSLLRFInterface& copyInterface)
{
	messenger.printDebugMessage("EPICSLLRFInterface Copy Constructor Called");
}

EPICSLLRFInterface::~EPICSLLRFInterface()
{
}



void EPICSLLRFInterface::retrieveupdateFunctionForRecord(pvStruct& pvStruct) const
{
	if (GlobalFunctions::entryExists(updateFunctionMap, pvStruct.pvRecord))
	{
		pvStruct.updateFunction = updateFunctionMap.at(pvStruct.pvRecord);
	}
	else
	{
		messenger.printDebugMessage("!!ERROR!! NO UPDATE FUNCTION FOUND FOR: \"" + pvStruct.pvRecord + "\"" );
	}
}



// testest 13-01-2021
void EPICSLLRFInterface::update_HEART_BEAT(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_HEART_BEAT");
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	updateTimeStampDoublePair(args, recastLLRF->heartbeat);
	messenger.printDebugMessage("update_HEART_BEAT FOR: " + recastLLRF->getHardwareName(), " ", recastLLRF->heartbeat.second);

}
void EPICSLLRFInterface::update_AMP_FF(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	updateTimeStampDoublePair(args, recastLLRF->amp_ff);
	messenger.printDebugMessage("update_AMP_FF FOR: " + recastLLRF->getHardwareName(), " ", recastLLRF->amp_ff.second);
}
void EPICSLLRFInterface::update_TRIG_SOURCE(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	const struct dbr_time_enum* tv = (const struct dbr_time_enum*)(args.dbr);
	recastLLRF->trig_state.first = tv->stamp;
	switch (tv->value)
	{
	case 0:		recastLLRF->trig_state.second = STATE::OFF; break;
	case 1:		recastLLRF->trig_state.second = STATE::EXTERNAL; break;
	case 2:		recastLLRF->trig_state.second = STATE::INTERNAL; break;
	}
	messenger.printDebugMessage("update_TRIG_SOURCE FOR: " + recastLLRF->getHardwareName(), 
		" ",  ENUM_TO_STRING(recastLLRF->trig_state.second));
}
void EPICSLLRFInterface::update_AMP_SP(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	updateTimeStampDoublePair(args, recastLLRF->amp_sp);
	messenger.printDebugMessage("update_AMP_SP FOR: " + recastLLRF->getHardwareName(), 
		" ", recastLLRF->amp_sp.second);
}
void EPICSLLRFInterface::update_PHI_FF(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	updateTimeStampDoublePair(args, recastLLRF->phi_ff);
	messenger.printDebugMessage("update_PHI_FF FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->phi_ff.second);
}
void EPICSLLRFInterface::update_PHI_SP(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	updateTimeStampDoublePair(args, recastLLRF->phi_ff);
	messenger.printDebugMessage("update_PHI_SP FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->phi_sp.second);
}
void EPICSLLRFInterface::update_RF_OUTPUT(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	updateTimeStampBoolPair(args, recastLLRF->rf_output);
	messenger.printDebugMessage("update_RF_OUTPUT FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->rf_output.second);
}
void EPICSLLRFInterface::update_FF_PHASE_LOCK_STATE(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	updateTimeStampBoolPair(args, recastLLRF->ff_phase_lock);
	messenger.printDebugMessage("update_FF_PHASE_LOCK_STATE FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->ff_phase_lock.second);
}
void EPICSLLRFInterface::update_FF_AMP_LOCK_STATE(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	updateTimeStampBoolPair(args, recastLLRF->ff_amp_lock);
	messenger.printDebugMessage("update_FF_AMP_LOCK_STATE FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->ff_amp_lock.second);
}
void EPICSLLRFInterface::update_TIME_VECTOR(const struct event_handler_args args)
{
	// mutex from updateLLRFValue()
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	const dbr_double_t* p_data = (const dbr_double_t*)args.dbr;
	std::copy(p_data, p_data + recastLLRF->time_vector.size(), recastLLRF->time_vector.begin());
}
void EPICSLLRFInterface::update_PULSE_OFFSET(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	updateTimeStampDoublePair(args, recastLLRF->pulse_offset);
	messenger.printDebugMessage("update_PULSE_OFFSET FOR: " + recastLLRF->getHardwareName(), " ", recastLLRF->pulse_offset.second);
}
void EPICSLLRFInterface::update_LLRF_PULSE_DURATION(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_LLRF_PULSE_DURATION");
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	updateTimeStampDoublePair(args, recastLLRF->llrf_pulse_duration);
	messenger.printDebugMessage("update_LLRF_PULSE_DURATION FOR: " + recastLLRF->getHardwareName(), " ", recastLLRF->llrf_pulse_duration.second);
}
void EPICSLLRFInterface::update_INTERLOCK(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	const struct dbr_time_enum* tv = (const struct dbr_time_enum*)(args.dbr);
	recastLLRF->interlock_state.first = tv->stamp;
	switch (tv->value)
	{
	case 0:
		recastLLRF->interlock_state.second = STATE::NOT_ACTIVE;
		break;
	case 1:
		recastLLRF->interlock_state.second = STATE::ACTIVE;
		break;
	default:
		recastLLRF->interlock_state.second = STATE::UNKNOWN;
	}
	messenger.printDebugMessage("update_INTERLOCK FOR: " + recastLLRF->getHardwareName(),
		" ", ENUM_TO_STRING(recastLLRF->interlock_state.second));
}
void EPICSLLRFInterface::update_PULSE_SHAPE(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	// mutex from updateLLRFValue()
	const dbr_double_t* p_data = (const dbr_double_t*)args.dbr;
	std::copy(p_data, p_data + recastLLRF->pulse_shape.second.size(), recastLLRF->pulse_shape.second.begin());
}





void EPICSLLRFInterface::update_LLRF_TRACES(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_LLRF_TRACES");
}
void EPICSLLRFInterface::update_LLRF_TRACES_SCAN(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_LLRF_TRACES_SCAN");
}
void EPICSLLRFInterface::update_LLRF_TRACES_ACQM(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_LLRF_TRACES_ACQM");
}






void EPICSLLRFInterface::update_CH1_PWR_REM(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH1_PWR_REM");
}
void EPICSLLRFInterface::update_CH2_PWR_REM(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH2_PWR_REM");
}
void EPICSLLRFInterface::update_CH3_PWR_REM(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH3_PWR_REM");
}
void EPICSLLRFInterface::update_CH4_PWR_REM(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH4_PWR_REM");
}
void EPICSLLRFInterface::update_CH5_PWR_REM(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH5_PWR_REM");
}
void EPICSLLRFInterface::update_CH6_PWR_REM(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH6_PWR_REM");
}
void EPICSLLRFInterface::update_CH7_PWR_REM(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH7_PWR_REM");
}
void EPICSLLRFInterface::update_CH8_PWR_REM(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH8_PWR_REM");
}
void EPICSLLRFInterface::update_CH1_PHASE_REM(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH1_PHASE_REM");
}
void EPICSLLRFInterface::update_CH2_PHASE_REM(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH2_PHASE_REM");
}
void EPICSLLRFInterface::update_CH3_PHASE_REM(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH3_PHASE_REM");
}
void EPICSLLRFInterface::update_CH4_PHASE_REM(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH4_PHASE_REM");
}
void EPICSLLRFInterface::update_CH5_PHASE_REM(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH5_PHASE_REM");
}
void EPICSLLRFInterface::update_CH6_PHASE_REM(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH6_PHASE_REM");
}
void EPICSLLRFInterface::update_CH7_PHASE_REM(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH7_PHASE_REM");
}
void EPICSLLRFInterface::update_CH8_PHASE_REM(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH8_PHASE_REM");
}









void EPICSLLRFInterface::update_CH1_INTERLOCK_STATUS(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	updateTimeStampBoolPair(args, recastLLRF->all_trace_interlocks.at(GlobalConstants::LLRF_CH1_INTERLOCK).status);
	//messenger.printDebugMessage("update_CH1_INTERLOCK_STATUS FOR: " + recastLLRF->getHardwareName(),
	//	" ", recastLLRF->all_trace_interlocks.at(GlobalConstants::LLRF_CH1_INTERLOCK).status.second);
}
void EPICSLLRFInterface::update_CH2_INTERLOCK_STATUS(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	updateTimeStampBoolPair(args, recastLLRF->all_trace_interlocks.at(GlobalConstants::LLRF_CH2_INTERLOCK).status);
	//messenger.printDebugMessage("update_CH2_INTERLOCK_STATUS FOR: " + recastLLRF->getHardwareName(),
	//	" ", recastLLRF->all_trace_interlocks.at(GlobalConstants::LLRF_CH2_INTERLOCK).status.second);
}
void EPICSLLRFInterface::update_CH3_INTERLOCK_STATUS(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	updateTimeStampBoolPair(args, recastLLRF->all_trace_interlocks.at(GlobalConstants::LLRF_CH3_INTERLOCK).status);
	//messenger.printDebugMessage("update_CH3_INTERLOCK_STATUS FOR: " + recastLLRF->getHardwareName(),
	//	" ", recastLLRF->all_trace_interlocks.at(GlobalConstants::LLRF_CH3_INTERLOCK).status.second);
}
void EPICSLLRFInterface::update_CH4_INTERLOCK_STATUS(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	updateTimeStampBoolPair(args, recastLLRF->all_trace_interlocks.at(GlobalConstants::LLRF_CH4_INTERLOCK).status);
	//messenger.printDebugMessage("update_CH4_INTERLOCK_STATUS FOR: " + recastLLRF->getHardwareName(),
	//	" ", recastLLRF->all_trace_interlocks.at(GlobalConstants::LLRF_CH4_INTERLOCK).status.second);
}
void EPICSLLRFInterface::update_CH5_INTERLOCK_STATUS(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	updateTimeStampBoolPair(args, recastLLRF->all_trace_interlocks.at(GlobalConstants::LLRF_CH5_INTERLOCK).status);
	//messenger.printDebugMessage("update_CH5_INTERLOCK_STATUS FOR: " + recastLLRF->getHardwareName(),
	//	" ", recastLLRF->all_trace_interlocks.at(GlobalConstants::LLRF_CH5_INTERLOCK).status.second);
}
void EPICSLLRFInterface::update_CH6_INTERLOCK_STATUS(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	updateTimeStampBoolPair(args, recastLLRF->all_trace_interlocks.at(GlobalConstants::LLRF_CH6_INTERLOCK).status);
	//messenger.printDebugMessage("update_CH6_INTERLOCK_STATUS FOR: " + recastLLRF->getHardwareName(),
	//	" ", recastLLRF->all_trace_interlocks.at(GlobalConstants::LLRF_CH6_INTERLOCK).status.second);
}
void EPICSLLRFInterface::update_CH7_INTERLOCK_STATUS(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	updateTimeStampBoolPair(args, recastLLRF->all_trace_interlocks.at(GlobalConstants::LLRF_CH7_INTERLOCK).status);
	/*messenger.printDebugMessage("update_CH7_INTERLOCK_STATUS FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_interlocks.at(GlobalConstants::LLRF_CH7_INTERLOCK).status.second);*/
}
void EPICSLLRFInterface::update_CH8_INTERLOCK_STATUS(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	updateTimeStampBoolPair(args, recastLLRF->all_trace_interlocks.at(GlobalConstants::LLRF_CH8_INTERLOCK).status);
	//messenger.printDebugMessage("update_CH8_INTERLOCK_STATUS FOR: " + recastLLRF->getHardwareName(),
	//	" ", recastLLRF->all_trace_interlocks.at(GlobalConstants::LLRF_CH8_INTERLOCK).status.second);
}



void EPICSLLRFInterface::update_CH1_INTERLOCK_ENABLE(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	updateTimeStampBoolPair(args, recastLLRF->all_trace_interlocks.at(GlobalConstants::LLRF_CH1_INTERLOCK).enable);
	messenger.printDebugMessage("update_CH1_INTERLOCK_ENABLE FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_interlocks.at(GlobalConstants::LLRF_CH1_INTERLOCK).enable.second);
}
void EPICSLLRFInterface::update_CH2_INTERLOCK_ENABLE(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	updateTimeStampBoolPair(args, recastLLRF->all_trace_interlocks.at(GlobalConstants::LLRF_CH2_INTERLOCK).enable);
	messenger.printDebugMessage("update_CH2_INTERLOCK_ENABLE FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_interlocks.at(GlobalConstants::LLRF_CH2_INTERLOCK).enable.second);
}
void EPICSLLRFInterface::update_CH3_INTERLOCK_ENABLE(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	updateTimeStampBoolPair(args, recastLLRF->all_trace_interlocks.at(GlobalConstants::LLRF_CH3_INTERLOCK).enable);
	messenger.printDebugMessage("update_CH3_INTERLOCK_ENABLE FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_interlocks.at(GlobalConstants::LLRF_CH3_INTERLOCK).enable.second);
}
void EPICSLLRFInterface::update_CH4_INTERLOCK_ENABLE(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	updateTimeStampBoolPair(args, recastLLRF->all_trace_interlocks.at(GlobalConstants::LLRF_CH4_INTERLOCK).enable);
	messenger.printDebugMessage("update_CH4_INTERLOCK_ENABLE FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_interlocks.at(GlobalConstants::LLRF_CH4_INTERLOCK).enable.second);
}
void EPICSLLRFInterface::update_CH5_INTERLOCK_ENABLE(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	updateTimeStampBoolPair(args, recastLLRF->all_trace_interlocks.at(GlobalConstants::LLRF_CH5_INTERLOCK).enable);
	messenger.printDebugMessage("update_CH5_INTERLOCK_ENABLE FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_interlocks.at(GlobalConstants::LLRF_CH5_INTERLOCK).enable.second);
}
void EPICSLLRFInterface::update_CH6_INTERLOCK_ENABLE(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	updateTimeStampBoolPair(args, recastLLRF->all_trace_interlocks.at(GlobalConstants::LLRF_CH6_INTERLOCK).enable);
	messenger.printDebugMessage("update_CH6_INTERLOCK_ENABLE FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_interlocks.at(GlobalConstants::LLRF_CH6_INTERLOCK).enable.second);
}
void EPICSLLRFInterface::update_CH7_INTERLOCK_ENABLE(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	updateTimeStampBoolPair(args, recastLLRF->all_trace_interlocks.at(GlobalConstants::LLRF_CH7_INTERLOCK).enable);
	messenger.printDebugMessage("update_CH7_INTERLOCK_ENABLE FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_interlocks.at(GlobalConstants::LLRF_CH7_INTERLOCK).enable.second);
}
void EPICSLLRFInterface::update_CH8_INTERLOCK_ENABLE(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	updateTimeStampBoolPair(args, recastLLRF->all_trace_interlocks.at(GlobalConstants::LLRF_CH8_INTERLOCK).enable);
	messenger.printDebugMessage("update_CH8_INTERLOCK_ENABLE FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_interlocks.at(GlobalConstants::LLRF_CH8_INTERLOCK).enable.second);
}


void EPICSLLRFInterface::update_CH1_INTERLOCK_U(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	updateTimeStampDoublePair(args, recastLLRF->all_trace_interlocks.at(GlobalConstants::LLRF_CH1_INTERLOCK).u_level);
	messenger.printDebugMessage("CH1_INTERLOCK_U FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_interlocks.at(GlobalConstants::LLRF_CH1_INTERLOCK).u_level.second);
}
void EPICSLLRFInterface::update_CH2_INTERLOCK_U(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	updateTimeStampDoublePair(args, recastLLRF->all_trace_interlocks.at(GlobalConstants::LLRF_CH2_INTERLOCK).u_level);
	messenger.printDebugMessage("CH2_INTERLOCK_U FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_interlocks.at(GlobalConstants::LLRF_CH2_INTERLOCK).u_level.second);
}
void EPICSLLRFInterface::update_CH3_INTERLOCK_U(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	updateTimeStampDoublePair(args, recastLLRF->all_trace_interlocks.at(GlobalConstants::LLRF_CH3_INTERLOCK).u_level);
	messenger.printDebugMessage("CH3_INTERLOCK_U FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_interlocks.at(GlobalConstants::LLRF_CH3_INTERLOCK).u_level.second);
}
void EPICSLLRFInterface::update_CH4_INTERLOCK_U(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	updateTimeStampDoublePair(args, recastLLRF->all_trace_interlocks.at(GlobalConstants::LLRF_CH4_INTERLOCK).u_level);
	messenger.printDebugMessage("CH4_INTERLOCK_U FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_interlocks.at(GlobalConstants::LLRF_CH4_INTERLOCK).u_level.second);
}
void EPICSLLRFInterface::update_CH5_INTERLOCK_U(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	updateTimeStampDoublePair(args, recastLLRF->all_trace_interlocks.at(GlobalConstants::LLRF_CH5_INTERLOCK).u_level);
	messenger.printDebugMessage("CH5_INTERLOCK_U FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_interlocks.at(GlobalConstants::LLRF_CH5_INTERLOCK).u_level.second);
}
void EPICSLLRFInterface::update_CH6_INTERLOCK_U(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	updateTimeStampDoublePair(args, recastLLRF->all_trace_interlocks.at(GlobalConstants::LLRF_CH6_INTERLOCK).u_level);
	messenger.printDebugMessage("CH6_INTERLOCK_U FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_interlocks.at(GlobalConstants::LLRF_CH6_INTERLOCK).u_level.second);
}
void EPICSLLRFInterface::update_CH7_INTERLOCK_U(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	updateTimeStampDoublePair(args, recastLLRF->all_trace_interlocks.at(GlobalConstants::LLRF_CH7_INTERLOCK).u_level);
	messenger.printDebugMessage("CH7_INTERLOCK_U FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_interlocks.at(GlobalConstants::LLRF_CH7_INTERLOCK).u_level.second);
}
void EPICSLLRFInterface::update_CH8_INTERLOCK_U(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	updateTimeStampDoublePair(args, recastLLRF->all_trace_interlocks.at(GlobalConstants::LLRF_CH8_INTERLOCK).u_level);
	messenger.printDebugMessage("CH8_INTERLOCK_U FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_interlocks.at(GlobalConstants::LLRF_CH8_INTERLOCK).u_level.second);
}



void EPICSLLRFInterface::update_CH1_INTERLOCK_P(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	updateTimeStampDoublePair(args, recastLLRF->all_trace_interlocks.at(GlobalConstants::LLRF_CH8_INTERLOCK).p_level);
	messenger.printDebugMessage("CH1_INTERLOCK_P FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_interlocks.at(GlobalConstants::LLRF_CH8_INTERLOCK).p_level.second);
}
void EPICSLLRFInterface::update_CH2_INTERLOCK_P(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	updateTimeStampDoublePair(args, recastLLRF->all_trace_interlocks.at(GlobalConstants::LLRF_CH8_INTERLOCK).p_level);
	messenger.printDebugMessage("CH2_INTERLOCK_P FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_interlocks.at(GlobalConstants::LLRF_CH8_INTERLOCK).p_level.second);
}
void EPICSLLRFInterface::update_CH3_INTERLOCK_P(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	updateTimeStampDoublePair(args, recastLLRF->all_trace_interlocks.at(GlobalConstants::LLRF_CH8_INTERLOCK).p_level);
	messenger.printDebugMessage("CH3_INTERLOCK_P FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_interlocks.at(GlobalConstants::LLRF_CH8_INTERLOCK).p_level.second);
}
void EPICSLLRFInterface::update_CH4_INTERLOCK_P(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	updateTimeStampDoublePair(args, recastLLRF->all_trace_interlocks.at(GlobalConstants::LLRF_CH8_INTERLOCK).p_level);
	messenger.printDebugMessage("CH4_INTERLOCK_P FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_interlocks.at(GlobalConstants::LLRF_CH8_INTERLOCK).p_level.second);
}
void EPICSLLRFInterface::update_CH5_INTERLOCK_P(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	updateTimeStampDoublePair(args, recastLLRF->all_trace_interlocks.at(GlobalConstants::LLRF_CH8_INTERLOCK).p_level);
	messenger.printDebugMessage("CH5_INTERLOCK_P FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_interlocks.at(GlobalConstants::LLRF_CH8_INTERLOCK).p_level.second);
}
void EPICSLLRFInterface::update_CH6_INTERLOCK_P(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	updateTimeStampDoublePair(args, recastLLRF->all_trace_interlocks.at(GlobalConstants::LLRF_CH8_INTERLOCK).p_level);
	messenger.printDebugMessage("CH6_INTERLOCK_P FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_interlocks.at(GlobalConstants::LLRF_CH8_INTERLOCK).p_level.second);
}
void EPICSLLRFInterface::update_CH7_INTERLOCK_P(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	updateTimeStampDoublePair(args, recastLLRF->all_trace_interlocks.at(GlobalConstants::LLRF_CH8_INTERLOCK).p_level);
	messenger.printDebugMessage("CH7_INTERLOCK_P FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_interlocks.at(GlobalConstants::LLRF_CH8_INTERLOCK).p_level.second);
}
void EPICSLLRFInterface::update_CH8_INTERLOCK_P(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	updateTimeStampDoublePair(args, recastLLRF->all_trace_interlocks.at(GlobalConstants::LLRF_CH8_INTERLOCK).p_level);
	messenger.printDebugMessage("CH8_INTERLOCK_P FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_interlocks.at(GlobalConstants::LLRF_CH8_INTERLOCK).p_level.second);
}


void EPICSLLRFInterface::update_CH1_INTERLOCK_PDBM(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	updateTimeStampDoublePair(args, recastLLRF->all_trace_interlocks.at(GlobalConstants::LLRF_CH1_INTERLOCK).pdbm_level);
	messenger.printDebugMessage("CH1_INTERLOCK_PDBM FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_interlocks.at(GlobalConstants::LLRF_CH1_INTERLOCK).pdbm_level.second);
}
void EPICSLLRFInterface::update_CH2_INTERLOCK_PDBM(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	updateTimeStampDoublePair(args, recastLLRF->all_trace_interlocks.at(GlobalConstants::LLRF_CH2_INTERLOCK).pdbm_level);
	messenger.printDebugMessage("CH2_INTERLOCK_PDBM FOR: " + recastLLRF->getHardwareName(), 
	" ", recastLLRF->all_trace_interlocks.at(GlobalConstants::LLRF_CH2_INTERLOCK).pdbm_level.second);
}
void EPICSLLRFInterface::update_CH3_INTERLOCK_PDBM(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	updateTimeStampDoublePair(args, recastLLRF->all_trace_interlocks.at(GlobalConstants::LLRF_CH3_INTERLOCK).pdbm_level);
	messenger.printDebugMessage("CH3_INTERLOCK_PDBM FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_interlocks.at(GlobalConstants::LLRF_CH3_INTERLOCK).pdbm_level.second);
}
void EPICSLLRFInterface::update_CH4_INTERLOCK_PDBM(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	updateTimeStampDoublePair(args, recastLLRF->all_trace_interlocks.at(GlobalConstants::LLRF_CH4_INTERLOCK).pdbm_level);
	messenger.printDebugMessage("CH4_INTERLOCK_PDBM FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_interlocks.at(GlobalConstants::LLRF_CH4_INTERLOCK).pdbm_level.second);
}
void EPICSLLRFInterface::update_CH5_INTERLOCK_PDBM(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	updateTimeStampDoublePair(args, recastLLRF->all_trace_interlocks.at(GlobalConstants::LLRF_CH5_INTERLOCK).pdbm_level);
	messenger.printDebugMessage("CH5_INTERLOCK_PDBM FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_interlocks.at(GlobalConstants::LLRF_CH5_INTERLOCK).pdbm_level.second);
}
void EPICSLLRFInterface::update_CH6_INTERLOCK_PDBM(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	updateTimeStampDoublePair(args, recastLLRF->all_trace_interlocks.at(GlobalConstants::LLRF_CH6_INTERLOCK).pdbm_level);
	messenger.printDebugMessage("CH6_INTERLOCK_PDBM FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_interlocks.at(GlobalConstants::LLRF_CH6_INTERLOCK).pdbm_level.second);
}
void EPICSLLRFInterface::update_CH7_INTERLOCK_PDBM(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	updateTimeStampDoublePair(args, recastLLRF->all_trace_interlocks.at(GlobalConstants::LLRF_CH7_INTERLOCK).pdbm_level);
	messenger.printDebugMessage("CH7_INTERLOCK_PDBM FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_interlocks.at(GlobalConstants::LLRF_CH7_INTERLOCK).pdbm_level.second);
}
void EPICSLLRFInterface::update_CH8_INTERLOCK_PDBM(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	updateTimeStampDoublePair(args, recastLLRF->all_trace_interlocks.at(GlobalConstants::LLRF_CH8_INTERLOCK).pdbm_level);
	messenger.printDebugMessage("CH8_INTERLOCK_PDBM FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_interlocks.at(GlobalConstants::LLRF_CH8_INTERLOCK).pdbm_level.second);
}


//CH1_PWR_REM_ACQM
//CH1_PHASE_REM_ACQM
//CH1_AMP_DER_ACQM
//CH1_PHASE_DER_ACQM
//CH1_PWR_LOC_ACQM



void EPICSLLRFInterface::update_CH1_PWR_REM_ACQM(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	update_trace_ACQM(args, recastLLRF->all_trace_acqm.at(GlobalConstants::CH1_PWR_REM));
	messenger.printDebugMessage("update_CH1_PWR_REM_ACQM FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_acqm.at(GlobalConstants::CH1_PWR_REM).second);
}
void EPICSLLRFInterface::update_CH2_PWR_REM_ACQM(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	update_trace_ACQM(args, recastLLRF->all_trace_acqm.at(GlobalConstants::CH2_PWR_REM));
	messenger.printDebugMessage("update_CH2_PWR_REM_ACQM FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_acqm.at(GlobalConstants::CH2_PWR_REM).second);
}
void EPICSLLRFInterface::update_CH3_PWR_REM_ACQM(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	update_trace_ACQM(args, recastLLRF->all_trace_acqm.at(GlobalConstants::CH3_PWR_REM));
	messenger.printDebugMessage("update_CH3_PWR_REM_ACQM FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_acqm.at(GlobalConstants::CH3_PWR_REM).second);
}
void EPICSLLRFInterface::update_CH4_PWR_REM_ACQM(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	update_trace_ACQM(args, recastLLRF->all_trace_acqm.at(GlobalConstants::CH4_PWR_REM));
	messenger.printDebugMessage("update_CH4_PWR_REM_ACQM FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_acqm.at(GlobalConstants::CH4_PWR_REM).second);
}
void EPICSLLRFInterface::update_CH5_PWR_REM_ACQM(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	update_trace_ACQM(args, recastLLRF->all_trace_acqm.at(GlobalConstants::CH5_PWR_REM));
	messenger.printDebugMessage("update_CH5_PWR_REM_ACQM FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_acqm.at(GlobalConstants::CH5_PWR_REM).second);
}
void EPICSLLRFInterface::update_CH6_PWR_REM_ACQM(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	update_trace_ACQM(args, recastLLRF->all_trace_acqm.at(GlobalConstants::CH6_PWR_REM));
	messenger.printDebugMessage("update_CH6_PWR_REM_ACQM FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_acqm.at(GlobalConstants::CH6_PWR_REM).second);
}
void EPICSLLRFInterface::update_CH7_PWR_REM_ACQM(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	update_trace_ACQM(args, recastLLRF->all_trace_acqm.at(GlobalConstants::CH7_PWR_REM));
	messenger.printDebugMessage("update_CH7_PWR_REM_ACQM FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_acqm.at(GlobalConstants::CH7_PWR_REM).second);
}
void EPICSLLRFInterface::update_CH8_PWR_REM_ACQM(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	update_trace_ACQM(args, recastLLRF->all_trace_acqm.at(GlobalConstants::CH8_PWR_REM));
	messenger.printDebugMessage("update_CH8_PWR_REM_ACQM FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_acqm.at(GlobalConstants::CH8_PWR_REM).second);
}

void EPICSLLRFInterface::update_CH1_PHASE_REM_ACQM(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	update_trace_ACQM(args, recastLLRF->all_trace_acqm.at(GlobalConstants::CH1_PHASE_REM));
	messenger.printDebugMessage("update_CH1_PHASE_REM_ACQM FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_acqm.at(GlobalConstants::CH1_PHASE_REM).second);
}
void EPICSLLRFInterface::update_CH2_PHASE_REM_ACQM(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	update_trace_ACQM(args, recastLLRF->all_trace_acqm.at(GlobalConstants::CH2_PHASE_REM));
	messenger.printDebugMessage("update_CH2_PHASE_REM_ACQM FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_acqm.at(GlobalConstants::CH2_PHASE_REM).second);
}
void EPICSLLRFInterface::update_CH3_PHASE_REM_ACQM(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	update_trace_ACQM(args, recastLLRF->all_trace_acqm.at(GlobalConstants::CH3_PHASE_REM));
	messenger.printDebugMessage("update_CH3_PHASE_REM_ACQM FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_acqm.at(GlobalConstants::CH3_PHASE_REM).second);
}
void EPICSLLRFInterface::update_CH4_PHASE_REM_ACQM(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	update_trace_ACQM(args, recastLLRF->all_trace_acqm.at(GlobalConstants::CH4_PHASE_REM));
	messenger.printDebugMessage("update_CH4_PHASE_REM_ACQM FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_acqm.at(GlobalConstants::CH4_PHASE_REM).second);
}
void EPICSLLRFInterface::update_CH5_PHASE_REM_ACQM(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	update_trace_ACQM(args, recastLLRF->all_trace_acqm.at(GlobalConstants::CH5_PHASE_REM));
	messenger.printDebugMessage("update_CH5_PHASE_REM_ACQM FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_acqm.at(GlobalConstants::CH5_PHASE_REM).second);
}
void EPICSLLRFInterface::update_CH6_PHASE_REM_ACQM(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	update_trace_ACQM(args, recastLLRF->all_trace_acqm.at(GlobalConstants::CH6_PHASE_REM));
	messenger.printDebugMessage("update_CH6_PHASE_REM_ACQM FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_acqm.at(GlobalConstants::CH6_PHASE_REM).second);
}
void EPICSLLRFInterface::update_CH7_PHASE_REM_ACQM(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	update_trace_ACQM(args, recastLLRF->all_trace_acqm.at(GlobalConstants::CH7_PHASE_REM));
	messenger.printDebugMessage("update_CH7_PHASE_REM_ACQM FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_acqm.at(GlobalConstants::CH7_PHASE_REM).second);
}
void EPICSLLRFInterface::update_CH8_PHASE_REM_ACQM(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	update_trace_ACQM(args, recastLLRF->all_trace_acqm.at(GlobalConstants::CH8_PHASE_REM));
	messenger.printDebugMessage("update_CH8_PHASE_REM_ACQM FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_acqm.at(GlobalConstants::CH8_PHASE_REM).second);
}

void EPICSLLRFInterface::update_CH1_AMP_DER_ACQM(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	update_trace_ACQM(args, recastLLRF->all_trace_acqm.at(GlobalConstants::CH1_AMP_DER));
	messenger.printDebugMessage("update_CH1_AMP_DER_ACQM FOR: " + recastLLRF->getHardwareName(),
		" ", ENUM_TO_STRING(recastLLRF->all_trace_acqm.at(GlobalConstants::CH1_AMP_DER).second));
}
void EPICSLLRFInterface::update_CH2_AMP_DER_ACQM(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	update_trace_ACQM(args, recastLLRF->all_trace_acqm.at(GlobalConstants::CH2_AMP_DER));
	messenger.printDebugMessage("update_CH2_AMP_DER_ACQM FOR: " + recastLLRF->getHardwareName(),
		" ",  ENUM_TO_STRING(recastLLRF->all_trace_acqm.at(GlobalConstants::CH2_AMP_DER).second));
}
void EPICSLLRFInterface::update_CH3_AMP_DER_ACQM(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	update_trace_ACQM(args, recastLLRF->all_trace_acqm.at(GlobalConstants::CH3_AMP_DER));
	messenger.printDebugMessage("update_CH3_AMP_DER_ACQM FOR: " + recastLLRF->getHardwareName(),
		" ", ENUM_TO_STRING(recastLLRF->all_trace_acqm.at(GlobalConstants::CH3_AMP_DER).second));
}
void EPICSLLRFInterface::update_CH4_AMP_DER_ACQM(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	update_trace_ACQM(args, recastLLRF->all_trace_acqm.at(GlobalConstants::CH4_AMP_DER));
	messenger.printDebugMessage("update_CH4_AMP_DER_ACQM FOR: " + recastLLRF->getHardwareName(),
		" ", ENUM_TO_STRING(recastLLRF->all_trace_acqm.at(GlobalConstants::CH4_AMP_DER).second));
}
void EPICSLLRFInterface::update_CH5_AMP_DER_ACQM(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	update_trace_ACQM(args, recastLLRF->all_trace_acqm.at(GlobalConstants::CH5_AMP_DER));
	messenger.printDebugMessage("update_CH5_AMP_DER_ACQM FOR: " + recastLLRF->getHardwareName(),
		" ", ENUM_TO_STRING(recastLLRF->all_trace_acqm.at(GlobalConstants::CH5_AMP_DER).second));
}
void EPICSLLRFInterface::update_CH6_AMP_DER_ACQM(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	update_trace_ACQM(args, recastLLRF->all_trace_acqm.at(GlobalConstants::CH6_AMP_DER));
	messenger.printDebugMessage("update_CH6_AMP_DER_ACQM FOR: " + recastLLRF->getHardwareName(),
		" ", ENUM_TO_STRING(recastLLRF->all_trace_acqm.at(GlobalConstants::CH6_AMP_DER).second));
}
void EPICSLLRFInterface::update_CH7_AMP_DER_ACQM(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	update_trace_ACQM(args, recastLLRF->all_trace_acqm.at(GlobalConstants::CH7_AMP_DER));
	messenger.printDebugMessage("update_CH7_AMP_DER_ACQM FOR: " + recastLLRF->getHardwareName(),
		" ", ENUM_TO_STRING(recastLLRF->all_trace_acqm.at(GlobalConstants::CH7_AMP_DER).second));
}
void EPICSLLRFInterface::update_CH8_AMP_DER_ACQM(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	update_trace_ACQM(args, recastLLRF->all_trace_acqm.at(GlobalConstants::CH8_AMP_DER));
	messenger.printDebugMessage("update_CH8_AMP_DER_ACQM FOR: " + recastLLRF->getHardwareName(),
		" ", ENUM_TO_STRING(recastLLRF->all_trace_acqm.at(GlobalConstants::CH8_AMP_DER).second));
}

void EPICSLLRFInterface::update_CH1_PHASE_DER_ACQM(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	update_trace_ACQM(args, recastLLRF->all_trace_acqm.at(GlobalConstants::CH1_PHASE_DER));
	messenger.printDebugMessage("update_CH1_PHASE_DER_ACQM FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_acqm.at(GlobalConstants::CH1_PHASE_DER).second);
}
void EPICSLLRFInterface::update_CH2_PHASE_DER_ACQM(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	update_trace_ACQM(args, recastLLRF->all_trace_acqm.at(GlobalConstants::CH2_PHASE_DER));
	messenger.printDebugMessage("update_CH2_PHASE_DER_ACQM FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_acqm.at(GlobalConstants::CH2_PHASE_DER).second);
}
void EPICSLLRFInterface::update_CH3_PHASE_DER_ACQM(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	update_trace_ACQM(args, recastLLRF->all_trace_acqm.at(GlobalConstants::CH3_PHASE_DER));
	messenger.printDebugMessage("update_CH3_PHASE_DER_ACQM FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_acqm.at(GlobalConstants::CH3_PHASE_DER).second);
}
void EPICSLLRFInterface::update_CH4_PHASE_DER_ACQM(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	update_trace_ACQM(args, recastLLRF->all_trace_acqm.at(GlobalConstants::CH4_PHASE_DER));
	messenger.printDebugMessage("update_CH4_PHASE_DER_ACQM FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_acqm.at(GlobalConstants::CH4_PHASE_DER).second);
}
void EPICSLLRFInterface::update_CH5_PHASE_DER_ACQM(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	update_trace_ACQM(args, recastLLRF->all_trace_acqm.at(GlobalConstants::CH5_PHASE_DER));
	messenger.printDebugMessage("update_CH5_PHASE_DER_ACQM FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_acqm.at(GlobalConstants::CH5_PHASE_DER).second);
}
void EPICSLLRFInterface::update_CH6_PHASE_DER_ACQM(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	update_trace_ACQM(args, recastLLRF->all_trace_acqm.at(GlobalConstants::CH6_PHASE_DER));
	messenger.printDebugMessage("update_CH6_PHASE_DER_ACQM FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_acqm.at(GlobalConstants::CH6_PHASE_DER).second);
}
void EPICSLLRFInterface::update_CH7_PHASE_DER_ACQM(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	update_trace_ACQM(args, recastLLRF->all_trace_acqm.at(GlobalConstants::CH7_PHASE_DER));
	messenger.printDebugMessage("update_CH7_PHASE_DER_ACQM FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_acqm.at(GlobalConstants::CH7_PHASE_DER).second);
}
void EPICSLLRFInterface::update_CH8_PHASE_DER_ACQM(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	update_trace_ACQM(args, recastLLRF->all_trace_acqm.at(GlobalConstants::CH8_PHASE_DER));
	messenger.printDebugMessage("update_CH8_PHASE_DER_ACQM FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_acqm.at(GlobalConstants::CH8_PHASE_DER).second);
}






void EPICSLLRFInterface::update_trace_ACQM(const struct event_handler_args& args, std::pair<epicsTimeStamp, STATE >& acqm)
{
	std::pair<epicsTimeStamp, int> pairToUpdate = getTimeStampUShortPair(args);
	acqm.first = pairToUpdate.first;
	switch (pairToUpdate.second)
	{
	case GlobalConstants::zero_int: acqm.second = STATE::UNKNOWN_ACQM; break;
	case GlobalConstants::one_int:  acqm.second = STATE::ACQM_NOW; break;
	case GlobalConstants::two_int:  acqm.second = STATE::ACQM_EVENT; break;
	default:
		acqm.second = STATE::UNKNOWN_ACQM;
	}
}


void EPICSLLRFInterface::update_CH1_PHASE_REM_SCAN(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	update_trace_SCAN(args, recastLLRF->all_trace_scan.at(GlobalConstants::CH1_PHASE_REM));
	messenger.printDebugMessage("update_CH1_PHASE_REM_SCAN FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_scan.at(GlobalConstants::CH1_PHASE_REM).second);
}
void EPICSLLRFInterface::update_CH2_PHASE_REM_SCAN(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	update_trace_SCAN(args, recastLLRF->all_trace_scan.at(GlobalConstants::CH2_PHASE_REM));
	messenger.printDebugMessage("update_CH2_PHASE_REM_SCAN FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_scan.at(GlobalConstants::CH2_PHASE_REM).second);
}
void EPICSLLRFInterface::update_CH3_PHASE_REM_SCAN(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	update_trace_SCAN(args, recastLLRF->all_trace_scan.at(GlobalConstants::CH3_PHASE_REM));
	messenger.printDebugMessage("update_CH3_PHASE_REM_SCAN FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_scan.at(GlobalConstants::CH3_PHASE_REM).second);
}
void EPICSLLRFInterface::update_CH4_PHASE_REM_SCAN(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	update_trace_SCAN(args, recastLLRF->all_trace_scan.at(GlobalConstants::CH4_PHASE_REM));
	messenger.printDebugMessage("update_CH4_PHASE_REM_SCAN FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_scan.at(GlobalConstants::CH4_PHASE_REM).second);
}
void EPICSLLRFInterface::update_CH5_PHASE_REM_SCAN(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	update_trace_SCAN(args, recastLLRF->all_trace_scan.at(GlobalConstants::CH5_PHASE_REM));
	messenger.printDebugMessage("update_CH5_PHASE_REM_SCAN FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_scan.at(GlobalConstants::CH5_PHASE_REM).second);
}
void EPICSLLRFInterface::update_CH6_PHASE_REM_SCAN(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	update_trace_SCAN(args, recastLLRF->all_trace_scan.at(GlobalConstants::CH6_PHASE_REM));
	messenger.printDebugMessage("update_CH6_PHASE_REM_SCAN FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_scan.at(GlobalConstants::CH6_PHASE_REM).second);
}
void EPICSLLRFInterface::update_CH7_PHASE_REM_SCAN(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	update_trace_SCAN(args, recastLLRF->all_trace_scan.at(GlobalConstants::CH7_PHASE_REM));
	messenger.printDebugMessage("update_CH7_PHASE_REM_SCAN FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_scan.at(GlobalConstants::CH7_PHASE_REM).second);
}
void EPICSLLRFInterface::update_CH8_PHASE_REM_SCAN(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	update_trace_SCAN(args, recastLLRF->all_trace_scan.at(GlobalConstants::CH8_PHASE_REM));
	messenger.printDebugMessage("update_CH8_PHASE_REM_SCAN FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_scan.at(GlobalConstants::CH8_PHASE_REM).second);
}

void EPICSLLRFInterface::update_CH1_PWR_REM_SCAN(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	update_trace_SCAN(args, recastLLRF->all_trace_scan.at(GlobalConstants::CH1_PWR_REM));
	messenger.printDebugMessage("update_CH1_PWR_REM_SCAN FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_scan.at(GlobalConstants::CH1_PWR_REM).second);
}
void EPICSLLRFInterface::update_CH2_PWR_REM_SCAN(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	update_trace_SCAN(args, recastLLRF->all_trace_scan.at(GlobalConstants::CH2_PWR_REM));
	messenger.printDebugMessage("update_CH2_PWR_REM_SCAN FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_scan.at(GlobalConstants::CH2_PWR_REM).second);
}
void EPICSLLRFInterface::update_CH3_PWR_REM_SCAN(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	update_trace_SCAN(args, recastLLRF->all_trace_scan.at(GlobalConstants::CH3_PWR_REM));
	messenger.printDebugMessage("update_CH3_PWR_REM_SCAN FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_scan.at(GlobalConstants::CH3_PWR_REM).second);
}
void EPICSLLRFInterface::update_CH4_PWR_REM_SCAN(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	update_trace_SCAN(args, recastLLRF->all_trace_scan.at(GlobalConstants::CH4_PWR_REM));
	messenger.printDebugMessage("update_CH4_PWR_REM_SCAN FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_scan.at(GlobalConstants::CH4_PWR_REM).second);
}
void EPICSLLRFInterface::update_CH5_PWR_REM_SCAN(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	update_trace_SCAN(args, recastLLRF->all_trace_scan.at(GlobalConstants::CH5_PWR_REM));
	messenger.printDebugMessage("update_CH5_PWR_REM_SCAN FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_scan.at(GlobalConstants::CH5_PWR_REM).second);
}
void EPICSLLRFInterface::update_CH6_PWR_REM_SCAN(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	update_trace_SCAN(args, recastLLRF->all_trace_scan.at(GlobalConstants::CH6_PWR_REM));
	messenger.printDebugMessage("update_CH6_PWR_REM_SCAN FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_scan.at(GlobalConstants::CH6_PWR_REM).second);
}
void EPICSLLRFInterface::update_CH7_PWR_REM_SCAN(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	update_trace_SCAN(args, recastLLRF->all_trace_scan.at(GlobalConstants::CH7_PWR_REM));
	messenger.printDebugMessage("update_CH7_PWR_REM_SCAN FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_scan.at(GlobalConstants::CH7_PWR_REM).second);
}
void EPICSLLRFInterface::update_CH8_PWR_REM_SCAN(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	update_trace_SCAN(args, recastLLRF->all_trace_scan.at(GlobalConstants::CH8_PWR_REM));
	messenger.printDebugMessage("update_CH8_PWR_REM_SCAN FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_scan.at(GlobalConstants::CH8_PWR_REM).second);
}

void EPICSLLRFInterface::update_CH1_AMP_DER_SCAN(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	update_trace_SCAN(args, recastLLRF->all_trace_scan.at(GlobalConstants::CH1_AMP_DER));
	messenger.printDebugMessage("TEST HERE");
	messenger.printDebugMessage("update_CH1_AMP_DER_SCAN FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_scan.at(GlobalConstants::CH1_AMP_DER).second);
	messenger.printDebugMessage("TEST FIN");
}
void EPICSLLRFInterface::update_CH2_AMP_DER_SCAN(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	update_trace_SCAN(args, recastLLRF->all_trace_scan.at(GlobalConstants::CH2_AMP_DER));
	messenger.printDebugMessage("update_CH2_AMP_DER_SCAN FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_scan.at(GlobalConstants::CH2_AMP_DER).second);
}
void EPICSLLRFInterface::update_CH3_AMP_DER_SCAN(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	update_trace_SCAN(args, recastLLRF->all_trace_scan.at(GlobalConstants::CH3_AMP_DER));
	messenger.printDebugMessage("update_CH3_AMP_DER_SCAN FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_scan.at(GlobalConstants::CH3_AMP_DER).second);
}
void EPICSLLRFInterface::update_CH4_AMP_DER_SCAN(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	update_trace_SCAN(args, recastLLRF->all_trace_scan.at(GlobalConstants::CH4_AMP_DER));
	messenger.printDebugMessage("update_CH4_AMP_DER_SCAN FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_scan.at(GlobalConstants::CH4_AMP_DER).second);
}
void EPICSLLRFInterface::update_CH5_AMP_DER_SCAN(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	update_trace_SCAN(args, recastLLRF->all_trace_scan.at(GlobalConstants::CH5_AMP_DER));
	messenger.printDebugMessage("update_CH5_AMP_DER_SCAN FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_scan.at(GlobalConstants::CH5_AMP_DER).second);
}
void EPICSLLRFInterface::update_CH6_AMP_DER_SCAN(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	update_trace_SCAN(args, recastLLRF->all_trace_scan.at(GlobalConstants::CH6_AMP_DER));
	messenger.printDebugMessage("update_CH6_AMP_DER_SCAN FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_scan.at(GlobalConstants::CH6_AMP_DER).second);
}
void EPICSLLRFInterface::update_CH7_AMP_DER_SCAN(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	update_trace_SCAN(args, recastLLRF->all_trace_scan.at(GlobalConstants::CH7_AMP_DER));
	messenger.printDebugMessage("update_CH7_AMP_DER_SCAN FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_scan.at(GlobalConstants::CH7_AMP_DER).second);
}
void EPICSLLRFInterface::update_CH8_AMP_DER_SCAN(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	update_trace_SCAN(args, recastLLRF->all_trace_scan.at(GlobalConstants::CH8_AMP_DER));
	messenger.printDebugMessage("update_CH8_AMP_DER_SCAN FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_scan.at(GlobalConstants::CH8_AMP_DER).second);
}

void EPICSLLRFInterface::update_CH1_PHASE_DER_SCAN(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	update_trace_SCAN(args, recastLLRF->all_trace_scan.at(GlobalConstants::CH1_PHASE_DER));
	messenger.printDebugMessage("update_CH1_PHASE_DER_SCAN FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_scan.at(GlobalConstants::CH1_PHASE_DER).second);
}
void EPICSLLRFInterface::update_CH2_PHASE_DER_SCAN(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	update_trace_SCAN(args, recastLLRF->all_trace_scan.at(GlobalConstants::CH2_PHASE_DER));
	messenger.printDebugMessage("update_CH2_PHASE_DER_SCAN FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_scan.at(GlobalConstants::CH2_PHASE_DER).second);
}
void EPICSLLRFInterface::update_CH3_PHASE_DER_SCAN(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	update_trace_SCAN(args, recastLLRF->all_trace_scan.at(GlobalConstants::CH3_PHASE_DER));
	messenger.printDebugMessage("update_CH3_PHASE_DER_SCAN FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_scan.at(GlobalConstants::CH3_PHASE_DER).second);
}
void EPICSLLRFInterface::update_CH4_PHASE_DER_SCAN(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	update_trace_SCAN(args, recastLLRF->all_trace_scan.at(GlobalConstants::CH4_PHASE_DER));
	messenger.printDebugMessage("update_CH4_PHASE_DER_SCAN FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_scan.at(GlobalConstants::CH4_PHASE_DER).second);
}
void EPICSLLRFInterface::update_CH5_PHASE_DER_SCAN(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	update_trace_SCAN(args, recastLLRF->all_trace_scan.at(GlobalConstants::CH5_PHASE_DER));
	messenger.printDebugMessage("update_CH5_PHASE_DER_SCAN FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_scan.at(GlobalConstants::CH5_PHASE_DER).second);
}
void EPICSLLRFInterface::update_CH6_PHASE_DER_SCAN(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	update_trace_SCAN(args, recastLLRF->all_trace_scan.at(GlobalConstants::CH6_PHASE_DER));
	messenger.printDebugMessage("update_CH6_PHASE_DER_SCAN FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_scan.at(GlobalConstants::CH6_PHASE_DER).second);
}
void EPICSLLRFInterface::update_CH7_PHASE_DER_SCAN(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	update_trace_SCAN(args, recastLLRF->all_trace_scan.at(GlobalConstants::CH7_PHASE_DER));
	messenger.printDebugMessage("update_CH7_PHASE_DER_SCAN FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_scan.at(GlobalConstants::CH7_PHASE_DER).second);
}
void EPICSLLRFInterface::update_CH8_PHASE_DER_SCAN(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	update_trace_SCAN(args, recastLLRF->all_trace_scan.at(GlobalConstants::CH8_PHASE_DER));
	messenger.printDebugMessage("update_CH8_PHASE_DER_SCAN FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_scan.at(GlobalConstants::CH8_PHASE_DER).second);
}

void EPICSLLRFInterface::update_CH1_PWR_LOC_SCAN(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	update_trace_SCAN(args, recastLLRF->all_trace_acqm.at(GlobalConstants::CH1_PWR_LOC));
	messenger.printDebugMessage("update_CH1_PHASE_DER_SCAN FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_scan.at(GlobalConstants::CH1_PWR_LOC).second);
}
void EPICSLLRFInterface::update_CH2_PWR_LOC_SCAN(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	update_trace_SCAN(args, recastLLRF->all_trace_scan.at(GlobalConstants::CH2_PWR_LOC));
	messenger.printDebugMessage("update_CH2_PWR_LOC_SCAN FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_scan.at(GlobalConstants::CH2_PWR_LOC).second);
}
void EPICSLLRFInterface::update_CH3_PWR_LOC_SCAN(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	update_trace_SCAN(args, recastLLRF->all_trace_scan.at(GlobalConstants::CH3_PWR_LOC));
	messenger.printDebugMessage("update_CH3_PWR_LOC_SCAN FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_scan.at(GlobalConstants::CH3_PWR_LOC).second);
}
void EPICSLLRFInterface::update_CH4_PWR_LOC_SCAN(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	update_trace_SCAN(args, recastLLRF->all_trace_scan.at(GlobalConstants::CH4_PWR_LOC));
	messenger.printDebugMessage("update_CH4_PWR_LOC_SCAN FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_scan.at(GlobalConstants::CH4_PWR_LOC).second);
}
void EPICSLLRFInterface::update_CH5_PWR_LOC_SCAN(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	update_trace_SCAN(args, recastLLRF->all_trace_scan.at(GlobalConstants::CH5_PWR_LOC));
	messenger.printDebugMessage("update_CH5_PWR_LOC_SCAN FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_scan.at(GlobalConstants::CH5_PWR_LOC).second);
}
void EPICSLLRFInterface::update_CH6_PWR_LOC_SCAN(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	update_trace_SCAN(args, recastLLRF->all_trace_scan.at(GlobalConstants::CH6_PWR_LOC));
	messenger.printDebugMessage("update_CH6_PWR_LOC_SCAN FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_scan.at(GlobalConstants::CH6_PWR_LOC).second);
}
void EPICSLLRFInterface::update_CH7_PWR_LOC_SCAN(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	update_trace_SCAN(args, recastLLRF->all_trace_scan.at(GlobalConstants::CH7_PWR_LOC));
	messenger.printDebugMessage("update_CH7_PWR_LOC_SCAN FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_scan.at(GlobalConstants::CH7_PWR_LOC).second);
}
void EPICSLLRFInterface::update_CH8_PWR_LOC_SCAN(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	update_trace_SCAN(args, recastLLRF->all_trace_scan.at(GlobalConstants::CH8_PWR_LOC));
	messenger.printDebugMessage("update_CH8_PWR_LOC_SCAN FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_scan.at(GlobalConstants::CH8_PWR_LOC).second);
}




void EPICSLLRFInterface::update_CH1_PWR_LOC_ACQM(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	update_trace_ACQM(args, recastLLRF->all_trace_acqm.at(GlobalConstants::CH1_PWR_LOC));
	messenger.printDebugMessage("update_CH1_PHASE_DER_ACQM FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_acqm.at(GlobalConstants::CH1_PWR_LOC).second);
}
void EPICSLLRFInterface::update_CH2_PWR_LOC_ACQM(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	update_trace_ACQM(args, recastLLRF->all_trace_acqm.at(GlobalConstants::CH2_PWR_LOC));
	messenger.printDebugMessage("update_CH2_PWR_LOC_SCAN FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_acqm.at(GlobalConstants::CH2_PWR_LOC).second);
}
void EPICSLLRFInterface::update_CH3_PWR_LOC_ACQM(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	update_trace_ACQM(args, recastLLRF->all_trace_acqm.at(GlobalConstants::CH3_PWR_LOC));
	messenger.printDebugMessage("update_CH3_PWR_LOC_SCAN FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_acqm.at(GlobalConstants::CH3_PWR_LOC).second);
}
void EPICSLLRFInterface::update_CH4_PWR_LOC_ACQM(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	update_trace_ACQM(args, recastLLRF->all_trace_acqm.at(GlobalConstants::CH4_PWR_LOC));
	messenger.printDebugMessage("update_CH4_PWR_LOC_SCAN FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_acqm.at(GlobalConstants::CH4_PWR_LOC).second);
}
void EPICSLLRFInterface::update_CH5_PWR_LOC_ACQM(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	update_trace_ACQM(args, recastLLRF->all_trace_acqm.at(GlobalConstants::CH5_PWR_LOC));
	messenger.printDebugMessage("update_CH5_PWR_LOC_SCAN FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_acqm.at(GlobalConstants::CH5_PWR_LOC).second);
}
void EPICSLLRFInterface::update_CH6_PWR_LOC_ACQM(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	update_trace_ACQM(args, recastLLRF->all_trace_acqm.at(GlobalConstants::CH6_PWR_LOC));
	messenger.printDebugMessage("update_CH6_PWR_LOC_SCAN FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_acqm.at(GlobalConstants::CH6_PWR_LOC).second);
}
void EPICSLLRFInterface::update_CH7_PWR_LOC_ACQM(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	update_trace_ACQM(args, recastLLRF->all_trace_acqm.at(GlobalConstants::CH7_PWR_LOC));
	messenger.printDebugMessage("update_CH7_PWR_LOC_SCAN FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_acqm.at(GlobalConstants::CH7_PWR_LOC).second);
}
void EPICSLLRFInterface::update_CH8_PWR_LOC_ACQM(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	update_trace_ACQM(args, recastLLRF->all_trace_acqm.at(GlobalConstants::CH8_PWR_LOC));
	messenger.printDebugMessage("update_CH8_PWR_LOC_SCAN FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_acqm.at(GlobalConstants::CH8_PWR_LOC).second);
}














//
//
//
//void EPICSLLRFInterface::update_CH2_PWR_REM_SCAN(const struct event_handler_args args)
//{
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	update_trace_ACQM(args, recastLLRF->all_trace_acqm.at(GlobalConstants::CH1_PHASE_REM));
//	messenger.printDebugMessage("update_CH8_PHASE_DER_ACQM FOR: " + recastLLRF->getHardwareName(),
//		" ", recastLLRF->all_trace_acqm.at(GlobalConstants::CH1_PHASE_REM).second);
//}
//void EPICSLLRFInterface::update_CH2_PHASE_REM_SCAN(const struct event_handler_args args)
//{
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	update_trace_ACQM(args, recastLLRF->all_trace_acqm.at(GlobalConstants::CH1_PHASE_REM_SCAN));
//	messenger.printDebugMessage("update_CH8_PHASE_DER_ACQM FOR: " + recastLLRF->getHardwareName(),
//		" ", recastLLRF->all_trace_acqm.at(GlobalConstants::CH1_PHASE_REM_SCAN).second);
//}
////void EPICSLLRFInterface::update_CH2_AMP_DER_SCAN(const struct event_handler_args args)
////{
////	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
////	update_trace_ACQM(args, recastLLRF->all_trace_acqm.at(GlobalConstants::CH1_PHASE_REM_SCAN));
////	messenger.printDebugMessage("update_CH8_PHASE_DER_ACQM FOR: " + recastLLRF->getHardwareName(),
////		" ", recastLLRF->all_trace_acqm.at(GlobalConstants::CH1_PHASE_REM_SCAN).second);
////}
////void EPICSLLRFInterface::update_CH2_PHASE_DER_SCAN(const struct event_handler_args args)
////{
////	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
////	update_trace_ACQM(args, recastLLRF->all_trace_acqm.at(GlobalConstants::CH1_PHASE_REM_SCAN));
////	messenger.printDebugMessage("update_CH8_PHASE_DER_ACQM FOR: " + recastLLRF->getHardwareName(),
////		" ", recastLLRF->all_trace_acqm.at(GlobalConstants::CH1_PHASE_REM_SCAN).second);
////}
//void EPICSLLRFInterface::update_CH2_PWR_LOC_SCAN(const struct event_handler_args args)
//{
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	update_trace_ACQM(args, recastLLRF->all_trace_acqm.at(GlobalConstants::CH1_PHASE_REM_SCAN));
//	messenger.printDebugMessage("update_CH8_PHASE_DER_ACQM FOR: " + recastLLRF->getHardwareName(),
//		" ", recastLLRF->all_trace_acqm.at(GlobalConstants::CH1_PHASE_REM_SCAN).second);
//}
//void EPICSLLRFInterface::update_CH3_PWR_REM_SCAN(const struct event_handler_args args)
//{
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	update_trace_ACQM(args, recastLLRF->all_trace_acqm.at(GlobalConstants::CH1_PHASE_REM_SCAN));
//	messenger.printDebugMessage("update_CH8_PHASE_DER_ACQM FOR: " + recastLLRF->getHardwareName(),
//		" ", recastLLRF->all_trace_acqm.at(GlobalConstants::CH1_PHASE_REM_SCAN).second);
//}
//void EPICSLLRFInterface::update_CH3_PHASE_REM_SCAN(const struct event_handler_args args)
//{
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	update_trace_ACQM(args, recastLLRF->all_trace_acqm.at(GlobalConstants::CH1_PHASE_REM_SCAN));
//	messenger.printDebugMessage("update_CH8_PHASE_DER_ACQM FOR: " + recastLLRF->getHardwareName(),
//		" ", recastLLRF->all_trace_acqm.at(GlobalConstants::CH1_PHASE_REM_SCAN).second);
//}
//void EPICSLLRFInterface::update_CH3_AMP_DER_SCAN(const struct event_handler_args args)
//{
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	update_trace_ACQM(args, recastLLRF->all_trace_acqm.at(GlobalConstants::CH1_PHASE_REM_SCAN));
//	messenger.printDebugMessage("update_CH8_PHASE_DER_ACQM FOR: " + recastLLRF->getHardwareName(),
//		" ", recastLLRF->all_trace_acqm.at(GlobalConstants::CH1_PHASE_REM_SCAN).second);
//}
//void EPICSLLRFInterface::update_CH3_PHASE_DER_SCAN(const struct event_handler_args args)
//{
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	update_trace_ACQM(args, recastLLRF->all_trace_acqm.at(GlobalConstants::CH1_PHASE_REM_SCAN));
//	messenger.printDebugMessage("update_CH8_PHASE_DER_ACQM FOR: " + recastLLRF->getHardwareName(),
//		" ", recastLLRF->all_trace_acqm.at(GlobalConstants::CH1_PHASE_REM_SCAN).second);
//}
//void EPICSLLRFInterface::update_CH3_PWR_LOC_SCAN(const struct event_handler_args args)
//{
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	update_trace_ACQM(args, recastLLRF->all_trace_acqm.at(GlobalConstants::CH1_PHASE_REM_SCAN));
//	messenger.printDebugMessage("update_CH8_PHASE_DER_ACQM FOR: " + recastLLRF->getHardwareName(),
//		" ", recastLLRF->all_trace_acqm.at(GlobalConstants::CH1_PHASE_REM_SCAN).second);
//}
//void EPICSLLRFInterface::update_CH4_PWR_REM_SCAN(const struct event_handler_args args)
//{
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	update_trace_ACQM(args, recastLLRF->all_trace_acqm.at(GlobalConstants::CH1_PHASE_REM_SCAN));
//	messenger.printDebugMessage("update_CH8_PHASE_DER_ACQM FOR: " + recastLLRF->getHardwareName(),
//		" ", recastLLRF->all_trace_acqm.at(GlobalConstants::CH1_PHASE_REM_SCAN).second);
//}
//void EPICSLLRFInterface::update_CH4_PHASE_REM_SCAN(const struct event_handler_args args)
//{
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	update_trace_ACQM(args, recastLLRF->all_trace_acqm.at(GlobalConstants::CH1_PHASE_REM_SCAN));
//	messenger.printDebugMessage("update_CH8_PHASE_DER_ACQM FOR: " + recastLLRF->getHardwareName(),
//		" ", recastLLRF->all_trace_acqm.at(GlobalConstants::CH1_PHASE_REM_SCAN).second);
//}
//void EPICSLLRFInterface::update_CH4_AMP_DER_SCAN(const struct event_handler_args args)
//{
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	update_trace_ACQM(args, recastLLRF->all_trace_acqm.at(GlobalConstants::CH1_PHASE_REM_SCAN));
//	messenger.printDebugMessage("update_CH8_PHASE_DER_ACQM FOR: " + recastLLRF->getHardwareName(),
//		" ", recastLLRF->all_trace_acqm.at(GlobalConstants::CH1_PHASE_REM_SCAN).second);
//}
//void EPICSLLRFInterface::update_CH4_PHASE_DER_SCAN(const struct event_handler_args args)
//{
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	update_trace_ACQM(args, recastLLRF->all_trace_acqm.at(GlobalConstants::CH1_PHASE_REM_SCAN));
//	messenger.printDebugMessage("update_CH8_PHASE_DER_ACQM FOR: " + recastLLRF->getHardwareName(),
//		" ", recastLLRF->all_trace_acqm.at(GlobalConstants::CH1_PHASE_REM_SCAN).second);
//}
//void EPICSLLRFInterface::update_CH4_PWR_LOC_SCAN(const struct event_handler_args args)
//{
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	update_trace_ACQM(args, recastLLRF->all_trace_acqm.at(GlobalConstants::CH1_PHASE_REM_SCAN));
//	messenger.printDebugMessage("update_CH8_PHASE_DER_ACQM FOR: " + recastLLRF->getHardwareName(),
//		" ", recastLLRF->all_trace_acqm.at(GlobalConstants::CH1_PHASE_REM_SCAN).second);
//}
//void EPICSLLRFInterface::update_CH5_PWR_REM_SCAN(const struct event_handler_args args)
//{
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	update_trace_ACQM(args, recastLLRF->all_trace_acqm.at(GlobalConstants::CH1_PHASE_REM_SCAN));
//	messenger.printDebugMessage("update_CH8_PHASE_DER_ACQM FOR: " + recastLLRF->getHardwareName(),
//		" ", recastLLRF->all_trace_acqm.at(GlobalConstants::CH1_PHASE_REM_SCAN).second);
//}
////void EPICSLLRFInterface::update_CH5_PHASE_REM_SCAN(const struct event_handler_args args)
////{
////	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
////	update_trace_ACQM(args, recastLLRF->all_trace_acqm.at(GlobalConstants::CH1_PHASE_REM_SCAN));
////	messenger.printDebugMessage("update_CH8_PHASE_DER_ACQM FOR: " + recastLLRF->getHardwareName(),
////		" ", recastLLRF->all_trace_acqm.at(GlobalConstants::CH1_PHASE_REM_SCAN).second);
////}
//void EPICSLLRFInterface::update_CH5_AMP_DER_SCAN(const struct event_handler_args args)
//{
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	update_trace_ACQM(args, recastLLRF->all_trace_acqm.at(GlobalConstants::CH1_PHASE_REM_SCAN));
//	messenger.printDebugMessage("update_CH8_PHASE_DER_ACQM FOR: " + recastLLRF->getHardwareName(),
//		" ", recastLLRF->all_trace_acqm.at(GlobalConstants::CH1_PHASE_REM_SCAN).second);
//}
//void EPICSLLRFInterface::update_CH5_PHASE_DER_SCAN(const struct event_handler_args args)
//{
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	update_trace_ACQM(args, recastLLRF->all_trace_acqm.at(GlobalConstants::CH1_PHASE_REM_SCAN));
//	messenger.printDebugMessage("update_CH8_PHASE_DER_ACQM FOR: " + recastLLRF->getHardwareName(),
//		" ", recastLLRF->all_trace_acqm.at(GlobalConstants::CH1_PHASE_REM_SCAN).second);
//}
//void EPICSLLRFInterface::update_CH5_PWR_LOC_SCAN(const struct event_handler_args args)
//{
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	update_trace_ACQM(args, recastLLRF->all_trace_acqm.at(GlobalConstants::CH1_PHASE_REM_SCAN));
//	messenger.printDebugMessage("update_CH8_PHASE_DER_ACQM FOR: " + recastLLRF->getHardwareName(),
//		" ", recastLLRF->all_trace_acqm.at(GlobalConstants::CH1_PHASE_REM_SCAN).second);
//}
//void EPICSLLRFInterface::update_CH6_PWR_REM_SCAN(const struct event_handler_args args)
//{
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	update_trace_ACQM(args, recastLLRF->all_trace_acqm.at(GlobalConstants::CH1_PHASE_REM_SCAN));
//	messenger.printDebugMessage("update_CH8_PHASE_DER_ACQM FOR: " + recastLLRF->getHardwareName(),
//		" ", recastLLRF->all_trace_acqm.at(GlobalConstants::CH1_PHASE_REM_SCAN).second);
//}
//void EPICSLLRFInterface::update_CH6_PHASE_REM_SCAN(const struct event_handler_args args)
//{
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	update_trace_ACQM(args, recastLLRF->all_trace_acqm.at(GlobalConstants::CH1_PHASE_REM_SCAN));
//	messenger.printDebugMessage("update_CH8_PHASE_DER_ACQM FOR: " + recastLLRF->getHardwareName(),
//		" ", recastLLRF->all_trace_acqm.at(GlobalConstants::CH1_PHASE_REM_SCAN).second);
//}
//void EPICSLLRFInterface::update_CH6_AMP_DER_SCAN(const struct event_handler_args args)
//{
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	update_trace_ACQM(args, recastLLRF->all_trace_acqm.at(GlobalConstants::CH1_PHASE_REM_SCAN));
//	messenger.printDebugMessage("update_CH8_PHASE_DER_ACQM FOR: " + recastLLRF->getHardwareName(),
//		" ", recastLLRF->all_trace_acqm.at(GlobalConstants::CH1_PHASE_REM_SCAN).second);
//}
//void EPICSLLRFInterface::update_CH6_PHASE_DER_SCAN(const struct event_handler_args args)
//{
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	update_trace_ACQM(args, recastLLRF->all_trace_acqm.at(GlobalConstants::CH1_PHASE_REM_SCAN));
//	messenger.printDebugMessage("update_CH8_PHASE_DER_ACQM FOR: " + recastLLRF->getHardwareName(),
//		" ", recastLLRF->all_trace_acqm.at(GlobalConstants::CH1_PHASE_REM_SCAN).second);
//}
//void EPICSLLRFInterface::update_CH6_PWR_LOC_SCAN(const struct event_handler_args args)
//{
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	update_trace_ACQM(args, recastLLRF->all_trace_acqm.at(GlobalConstants::CH1_PHASE_REM_SCAN));
//	messenger.printDebugMessage("update_CH8_PHASE_DER_ACQM FOR: " + recastLLRF->getHardwareName(),
//		" ", recastLLRF->all_trace_acqm.at(GlobalConstants::CH1_PHASE_REM_SCAN).second);
//}
//void EPICSLLRFInterface::update_CH7_PWR_REM_SCAN(const struct event_handler_args args)
//{
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	update_trace_ACQM(args, recastLLRF->all_trace_acqm.at(GlobalConstants::CH1_PHASE_REM_SCAN));
//	messenger.printDebugMessage("update_CH8_PHASE_DER_ACQM FOR: " + recastLLRF->getHardwareName(),
//		" ", recastLLRF->all_trace_acqm.at(GlobalConstants::CH1_PHASE_REM_SCAN).second);
//}
//void EPICSLLRFInterface::update_CH7_PHASE_REM_SCAN(const struct event_handler_args args)
//{
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	update_trace_ACQM(args, recastLLRF->all_trace_acqm.at(GlobalConstants::CH1_PHASE_REM_SCAN));
//	messenger.printDebugMessage("update_CH8_PHASE_DER_ACQM FOR: " + recastLLRF->getHardwareName(),
//		" ", recastLLRF->all_trace_acqm.at(GlobalConstants::CH1_PHASE_REM_SCAN).second);
//}
//void EPICSLLRFInterface::update_CH7_AMP_DER_SCAN(const struct event_handler_args args)
//{
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	update_trace_ACQM(args, recastLLRF->all_trace_acqm.at(GlobalConstants::CH1_PHASE_REM_SCAN));
//	messenger.printDebugMessage("update_CH8_PHASE_DER_ACQM FOR: " + recastLLRF->getHardwareName(),
//		" ", recastLLRF->all_trace_acqm.at(GlobalConstants::CH1_PHASE_REM_SCAN).second);
//}
//void EPICSLLRFInterface::update_CH7_PHASE_DER_SCAN(const struct event_handler_args args)
//{
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	update_trace_ACQM(args, recastLLRF->all_trace_acqm.at(GlobalConstants::CH1_PHASE_REM_SCAN));
//	messenger.printDebugMessage("update_CH8_PHASE_DER_ACQM FOR: " + recastLLRF->getHardwareName(),
//		" ", recastLLRF->all_trace_acqm.at(GlobalConstants::CH1_PHASE_REM_SCAN).second);
//}
//void EPICSLLRFInterface::update_CH7_PWR_LOC_SCAN(const struct event_handler_args args)
//{
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	update_trace_ACQM(args, recastLLRF->all_trace_acqm.at(GlobalConstants::CH1_PHASE_REM_SCAN));
//	messenger.printDebugMessage("update_CH8_PHASE_DER_ACQM FOR: " + recastLLRF->getHardwareName(),
//		" ", recastLLRF->all_trace_acqm.at(GlobalConstants::CH1_PHASE_REM_SCAN).second);
//}
//void EPICSLLRFInterface::update_CH8_PWR_REM_SCAN(const struct event_handler_args args)
//{
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	update_trace_ACQM(args, recastLLRF->all_trace_acqm.at(GlobalConstants::CH1_PHASE_REM_SCAN));
//	messenger.printDebugMessage("update_CH8_PHASE_DER_ACQM FOR: " + recastLLRF->getHardwareName(),
//		" ", recastLLRF->all_trace_acqm.at(GlobalConstants::CH1_PHASE_REM_SCAN).second);
//}
//void EPICSLLRFInterface::update_CH8_PHASE_REM_SCAN(const struct event_handler_args args)
//{
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	update_trace_ACQM(args, recastLLRF->all_trace_acqm.at(GlobalConstants::CH1_PHASE_REM_SCAN));
//	messenger.printDebugMessage("update_CH8_PHASE_DER_ACQM FOR: " + recastLLRF->getHardwareName(),
//		" ", recastLLRF->all_trace_acqm.at(GlobalConstants::CH1_PHASE_REM_SCAN).second);
//}
//void EPICSLLRFInterface::update_CH8_AMP_DER_SCAN(const struct event_handler_args args)
//{
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	update_trace_ACQM(args, recastLLRF->all_trace_acqm.at(GlobalConstants::CH1_PHASE_REM_SCAN));
//	messenger.printDebugMessage("update_CH8_PHASE_DER_ACQM FOR: " + recastLLRF->getHardwareName(),
//		" ", recastLLRF->all_trace_acqm.at(GlobalConstants::CH1_PHASE_REM_SCAN).second);
//}
//void EPICSLLRFInterface::update_CH8_PHASE_DER_SCAN(const struct event_handler_args args)
//{
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	update_trace_ACQM(args, recastLLRF->all_trace_acqm.at(GlobalConstants::CH1_PHASE_REM_SCAN));
//	messenger.printDebugMessage("update_CH8_PHASE_DER_ACQM FOR: " + recastLLRF->getHardwareName(),
//		" ", recastLLRF->all_trace_acqm.at(GlobalConstants::CH1_PHASE_REM_SCAN).second);
//}
//void EPICSLLRFInterface::update_CH8_PWR_LOC_SCAN(const struct event_handler_args args)
//{
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	update_trace_ACQM(args, recastLLRF->all_trace_acqm.at(GlobalConstants::CH1_PHASE_REM_SCAN));
//	messenger.printDebugMessage("update_CH8_PHASE_DER_ACQM FOR: " + recastLLRF->getHardwareName(),
//		" ", recastLLRF->all_trace_acqm.at(GlobalConstants::CH1_PHASE_REM_SCAN).second);
//}


void EPICSLLRFInterface::update_trace_SCAN(const struct event_handler_args& args, std::pair<epicsTimeStamp, STATE >& acqm)
{
	std::pair<epicsTimeStamp, int> pairToUpdate = getTimeStampUShortPair(args);
	acqm.first = pairToUpdate.first;
	switch (pairToUpdate.second)
	{
		case 0:			acqm.second = STATE::PASSIVE; break;
		case 1:			acqm.second = STATE::EVENT; break;
		case 2:			acqm.second = STATE::IO_INTR; break;
		case 3:			acqm.second = STATE::TEN; break;
		case 4:			acqm.second = STATE::FIVE; break;
		case 5:			acqm.second = STATE::TWO; break;
		case 6:			acqm.second = STATE::ONE; break;
		case 7:			acqm.second = STATE::ZERO_POINT_FIVE; break;
		case 8:			acqm.second = STATE::ZERO_POINT_TWO; break;
		case 9:			acqm.second = STATE::ZERO_POINT_ONE; break;
		case 10:		acqm.second = STATE::ZERO_POINT_ZERO_FIVE; break;
		default:		acqm.second = STATE::UNKNOWN;
		}

}


//void LLRF::updateSCAN(const std::string& ch, const struct event_handler_args& args)
//{
//	const struct dbr_time_enum* tv = (const struct dbr_time_enum*)(args.dbr);
//	// TODO cross check values with real ones, i think this is correct (32bit 64 bit oddities???)
//	STATE new_state;
//	switch ((unsigned long)tv->value)
//	{
//	case 0:
//		new_state = STATE::PASSIVE; break;
//	case 1:
//		new_state = STATE::EVENT; break;
//	case 2:
//		new_state = STATE::IO_INTR; break;
//	case 3:
//		new_state = STATE::TEN; break;
//	case 4:
//		new_state = STATE::FIVE; break;
//	case 5:
//		new_state = STATE::TWO; break;
//	case 6:
//		new_state = STATE::ONE; break;
//	case 7:
//		new_state = STATE::ZERO_POINT_FIVE; break;
//	case 8:
//		new_state = STATE::ZERO_POINT_TWO; break;
//	case 9:
//		new_state = STATE::ZERO_POINT_ONE; break;
//	case 10:
//		new_state = STATE::ZERO_POINT_ZERO_FIVE; break;
//	default:
//		new_state = STATE::UNKNOWN;
//	}
//	if (GlobalFunctions::entryExists(all_trace_scan, ch))
//	{
//		all_trace_scan.at(ch) = new_state;
//		std::string trace = getTraceFromChannelData(ch);
//		if (GlobalFunctions::entryExists(trace_data_map, trace))
//		{
//			trace_data_map.at(trace).acqm = new_state;
//		}
//	}
//	//TODO update SCAN in actual TRACES we monitor ... 
//
//}
