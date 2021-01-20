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
	if (GlobalFunctions::entryExists(updateFucntionMap, pvStruct.pvRecord))
	{
		pvStruct.updateFunction = updateFucntionMap.at(pvStruct.pvRecord);
	}
	else
	{
		messenger.printDebugMessage("!!WARNING!! NO UPDATE FUNCTION FOUND FOR: " + pvStruct.pvRecord);
	}
}

void EPICSLLRFInterface::update_HEART_BEAT(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_KEEP_ALIVE");
	
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
		" ", recastLLRF->trig_state.second);
}

void EPICSLLRFInterface::update_PULSE_SHAPE(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_PULSE_SHAPE");
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
	messenger.printDebugMessage("update_PULSE_OFFSET");
}
void EPICSLLRFInterface::update_PULSE_LENGTH(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_PULSE_LENGTH");
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
	messenger.printDebugMessage("update_INTERLOCK");
}



// probably don't need this 
//void EPICSLLRFInterface::update_PULSE_SHAPE_APPLY(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_PULSE_SHAPE_APPLY");
//}



//
//void EPICSLLRFInterface::update_LLRF_TRACES(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_LLRF_TRACES");
//}
//void EPICSLLRFInterface::update_LLRF_TRACES_SCAN(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_LLRF_TRACES_SCAN");
//}
//void EPICSLLRFInterface::update_LLRF_TRACES_ACQM(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_LLRF_TRACES_ACQM");
//}
//
//
//void EPICSLLRFInterface::update_CH1_PWR_REM(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH1_PWR_REM");
//}
//void EPICSLLRFInterface::update_CH2_PWR_REM(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH2_PWR_REM");
//}
//void EPICSLLRFInterface::update_CH3_PWR_REM(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH3_PWR_REM");
//}
//void EPICSLLRFInterface::update_CH4_PWR_REM(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH4_PWR_REM");
//}
//void EPICSLLRFInterface::update_CH5_PWR_REM(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH5_PWR_REM");
//}
//void EPICSLLRFInterface::update_CH6_PWR_REM(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH6_PWR_REM");
//}
//void EPICSLLRFInterface::update_CH7_PWR_REM(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH7_PWR_REM");
//}
//void EPICSLLRFInterface::update_CH8_PWR_REM(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH8_PWR_REM");
//}
//void EPICSLLRFInterface::update_CH1_PHASE_REM(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH1_PHASE_REM");
//}
//void EPICSLLRFInterface::update_CH2_PHASE_REM(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH2_PHASE_REM");
//}
//void EPICSLLRFInterface::update_CH3_PHASE_REM(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH3_PHASE_REM");
//}
//void EPICSLLRFInterface::update_CH4_PHASE_REM(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH4_PHASE_REM");
//}
//void EPICSLLRFInterface::update_CH5_PHASE_REM(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH5_PHASE_REM");
//}
//void EPICSLLRFInterface::update_CH6_PHASE_REM(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH6_PHASE_REM");
//}
//void EPICSLLRFInterface::update_CH7_PHASE_REM(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH7_PHASE_REM");
//}
//void EPICSLLRFInterface::update_CH8_PHASE_REM(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH8_PHASE_REM");
//}
//
//
//
//
//
//
//
//
//
//void EPICSLLRFInterface::update_CH1_INTERLOCK_STATUS(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH1_INTERLOCK_STATUS");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateInterLockStatus(LLRFRecords::CH1_INTERLOCK_STATUS, args);
//}
//void EPICSLLRFInterface::update_CH2_INTERLOCK_STATUS(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH2_INTERLOCK_STATUS");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateInterLockStatus(LLRFRecords::LLRF_CH2_INTERLOCK, args);
//}
//void EPICSLLRFInterface::update_CH3_INTERLOCK_STATUS(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH3_INTERLOCK_STATUS");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateInterLockStatus(LLRFRecords::LLRF_CH3_INTERLOCK, args);
//}
//void EPICSLLRFInterface::update_CH4_INTERLOCK_STATUS(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH4_INTERLOCK_STATUS");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateInterLockStatus(LLRFRecords::LLRF_CH4_INTERLOCK, args);
//}
//void EPICSLLRFInterface::update_CH5_INTERLOCK_STATUS(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH5_INTERLOCK_STATUS");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateInterLockStatus(LLRFRecords::LLRF_CH5_INTERLOCK, args);
//}
//void EPICSLLRFInterface::update_CH6_INTERLOCK_STATUS(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH6_INTERLOCK_STATUS");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateInterLockStatus(LLRFRecords::LLRF_CH6_INTERLOCK, args);
//}
//void EPICSLLRFInterface::update_CH7_INTERLOCK_STATUS(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH7_INTERLOCK_STATUS");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateInterLockStatus(LLRFRecords::LLRF_CH7_INTERLOCK, args);
//}
//void EPICSLLRFInterface::update_CH8_INTERLOCK_STATUS(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH8_INTERLOCK_STATUS");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateInterLockStatus(LLRFRecords::LLRF_CH8_INTERLOCK, args);
//}
//
//
//
//void EPICSLLRFInterface::update_CH1_INTERLOCK_ENABLE(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH1_INTERLOCK_ENABLE");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateInterLockEnable(LLRFRecords::LLRF_CH1_INTERLOCK, args);
//}
//void EPICSLLRFInterface::update_CH2_INTERLOCK_ENABLE(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH2_INTERLOCK_ENABLE");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateInterLockEnable(LLRFRecords::LLRF_CH2_INTERLOCK, args);
//}
//void EPICSLLRFInterface::update_CH3_INTERLOCK_ENABLE(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH3_INTERLOCK_ENABLE");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateInterLockEnable(LLRFRecords::LLRF_CH3_INTERLOCK, args);
//}
//void EPICSLLRFInterface::update_CH4_INTERLOCK_ENABLE(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH4_INTERLOCK_ENABLE");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateInterLockEnable(LLRFRecords::LLRF_CH4_INTERLOCK, args);
//}
//void EPICSLLRFInterface::update_CH5_INTERLOCK_ENABLE(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH5_INTERLOCK_ENABLE");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateInterLockEnable(LLRFRecords::LLRF_CH5_INTERLOCK, args);
//}
//void EPICSLLRFInterface::update_CH6_INTERLOCK_ENABLE(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH6_INTERLOCK_ENABLE");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateInterLockEnable(LLRFRecords::LLRF_CH6_INTERLOCK, args);
//}
//void EPICSLLRFInterface::update_CH7_INTERLOCK_ENABLE(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH7_INTERLOCK_ENABLE");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateInterLockEnable(LLRFRecords::LLRF_CH7_INTERLOCK, args);
//}
//void EPICSLLRFInterface::update_CH8_INTERLOCK_ENABLE(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH8_INTERLOCK_ENABLE");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateInterLockEnable(LLRFRecords::LLRF_CH8_INTERLOCK, args);
//}
//void EPICSLLRFInterface::update_CH1_INTERLOCK_U(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH1_INTERLOCK_U");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateInterLockU(LLRFRecords::LLRF_CH1_INTERLOCK, args);
//}
//void EPICSLLRFInterface::update_CH2_INTERLOCK_U(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH2_INTERLOCK_U");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateInterLockU(LLRFRecords::LLRF_CH2_INTERLOCK, args);
//}
//void EPICSLLRFInterface::update_CH3_INTERLOCK_U(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH3_INTERLOCK_U");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateInterLockU(LLRFRecords::LLRF_CH3_INTERLOCK, args);
//}
//void EPICSLLRFInterface::update_CH4_INTERLOCK_U(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH4_INTERLOCK_U");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateInterLockU(LLRFRecords::LLRF_CH4_INTERLOCK, args);
//}
//void EPICSLLRFInterface::update_CH5_INTERLOCK_U(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH5_INTERLOCK_U");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateInterLockU(LLRFRecords::LLRF_CH5_INTERLOCK, args);
//}
//void EPICSLLRFInterface::update_CH6_INTERLOCK_U(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH6_INTERLOCK_U");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateInterLockU(LLRFRecords::LLRF_CH6_INTERLOCK, args);
//}
//void EPICSLLRFInterface::update_CH7_INTERLOCK_U(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH7_INTERLOCK_U");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateInterLockU(LLRFRecords::LLRF_CH7_INTERLOCK, args);
//}
//void EPICSLLRFInterface::update_CH8_INTERLOCK_U(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH8_INTERLOCK_U");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateInterLockU(LLRFRecords::LLRF_CH8_INTERLOCK, args);
//}
//
//
//
//void EPICSLLRFInterface::update_CH1_INTERLOCK_P(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH1_INTERLOCK_P");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateInterLockP(LLRFRecords::LLRF_CH1_INTERLOCK, args);
//}
//void EPICSLLRFInterface::update_CH2_INTERLOCK_P(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH2_INTERLOCK_P");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateInterLockP(LLRFRecords::LLRF_CH2_INTERLOCK, args);
//}
//void EPICSLLRFInterface::update_CH3_INTERLOCK_P(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH3_INTERLOCK_P");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateInterLockP(LLRFRecords::LLRF_CH3_INTERLOCK, args);
//}
//void EPICSLLRFInterface::update_CH4_INTERLOCK_P(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH4_INTERLOCK_P");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateInterLockP(LLRFRecords::LLRF_CH4_INTERLOCK, args);
//}
//void EPICSLLRFInterface::update_CH5_INTERLOCK_P(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH5_INTERLOCK_P");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateInterLockP(LLRFRecords::LLRF_CH5_INTERLOCK, args);
//}
//void EPICSLLRFInterface::update_CH6_INTERLOCK_P(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH6_INTERLOCK_P");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateInterLockP(LLRFRecords::LLRF_CH6_INTERLOCK, args);
//}
//void EPICSLLRFInterface::update_CH7_INTERLOCK_P(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH7_INTERLOCK_P");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateInterLockP(LLRFRecords::LLRF_CH7_INTERLOCK, args);
//}
//void EPICSLLRFInterface::update_CH8_INTERLOCK_P(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH8_INTERLOCK_P");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateInterLockP(LLRFRecords::LLRF_CH8_INTERLOCK, args);
//}
//
//
//void EPICSLLRFInterface::update_CH1_INTERLOCK_PDBM(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH1_INTERLOCK_PDBM");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateInterLockPDBM(LLRFRecords::LLRF_CH1_INTERLOCK, args);
//}
//void EPICSLLRFInterface::update_CH2_INTERLOCK_PDBM(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH2_INTERLOCK_PDBM");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateInterLockPDBM(LLRFRecords::LLRF_CH2_INTERLOCK, args);
//}
//void EPICSLLRFInterface::update_CH3_INTERLOCK_PDBM(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH3_INTERLOCK_PDBM");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateInterLockPDBM(LLRFRecords::LLRF_CH3_INTERLOCK, args);
//}
//void EPICSLLRFInterface::update_CH4_INTERLOCK_PDBM(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH4_INTERLOCK_PDBM");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateInterLockPDBM(LLRFRecords::LLRF_CH4_INTERLOCK, args);
//}
//void EPICSLLRFInterface::update_CH5_INTERLOCK_PDBM(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH5_INTERLOCK_PDBM");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateInterLockPDBM(LLRFRecords::LLRF_CH5_INTERLOCK, args);
//}
//void EPICSLLRFInterface::update_CH6_INTERLOCK_PDBM(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH6_INTERLOCK_PDBM");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateInterLockPDBM(LLRFRecords::LLRF_CH6_INTERLOCK, args);
//}
//void EPICSLLRFInterface::update_CH7_INTERLOCK_PDBM(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH7_INTERLOCK_PDBM");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateInterLockPDBM(LLRFRecords::LLRF_CH7_INTERLOCK, args);
//}
//void EPICSLLRFInterface::update_CH8_INTERLOCK_PDBM(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH8_INTERLOCK_PDBM");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateInterLockPDBM(LLRFRecords::LLRF_CH8_INTERLOCK, args);
//}


//
//
//
//void EPICSLLRFInterface::update_CH1_PWR_REM_ACQM(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH1_PWR_REM_ACQM");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateACQM(GlobalConstants::CH1_PWR_REM_ACQM, args);
//}
//void EPICSLLRFInterface::update_CH2_PWR_REM_ACQM(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH2_PWR_REM_ACQM");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateACQM(GlobalConstants::CH2_PWR_REM_ACQM, args);
//}
//void EPICSLLRFInterface::update_CH3_PWR_REM_ACQM(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH3_PWR_REM_ACQM");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateACQM(GlobalConstants::CH3_PWR_REM_ACQM, args);
//}
//void EPICSLLRFInterface::update_CH4_PWR_REM_ACQM(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH4_PWR_REM_ACQM");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateACQM(GlobalConstants::CH4_PWR_REM_ACQM, args);
//}
//void EPICSLLRFInterface::update_CH5_PWR_REM_ACQM(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH5_PWR_REM_ACQM");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateACQM(GlobalConstants::CH5_PWR_REM_ACQM, args);
//}
//void EPICSLLRFInterface::update_CH6_PWR_REM_ACQM(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH6_PWR_REM_ACQM");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateACQM(GlobalConstants::CH6_PWR_REM_ACQM, args);
//}
//void EPICSLLRFInterface::update_CH7_PWR_REM_ACQM(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH7_PWR_REM_ACQM");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateACQM(GlobalConstants::CH7_PWR_REM_ACQM, args);
//}
//void EPICSLLRFInterface::update_CH8_PWR_REM_ACQM(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH8_PWR_REM_ACQM");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateACQM(GlobalConstants::CH8_PWR_REM_ACQM, args);
//}
//void EPICSLLRFInterface::update_CH1_PHASE_REM_ACQM(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH1_PHASE_REM_ACQM");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateACQM(GlobalConstants::CH1_PWR_REM_ACQM, args);
//}
//void EPICSLLRFInterface::update_CH2_PHASE_REM_ACQM(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH2_PHASE_REM_ACQM");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateACQM(GlobalConstants::CH2_PWR_REM_ACQM, args);
//}
//void EPICSLLRFInterface::update_CH3_PHASE_REM_ACQM(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH3_PHASE_REM_ACQM");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateACQM(GlobalConstants::CH3_PWR_REM_ACQM, args);
//}
//void EPICSLLRFInterface::update_CH4_PHASE_REM_ACQM(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH4_PHASE_REM_ACQM");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateACQM(GlobalConstants::CH4_PWR_REM_ACQM, args);
//}
//void EPICSLLRFInterface::update_CH5_PHASE_REM_ACQM(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH5_PHASE_REM_ACQM");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateACQM(GlobalConstants::CH5_PWR_REM_ACQM, args);
//}
//void EPICSLLRFInterface::update_CH6_PHASE_REM_ACQM(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH6_PHASE_REM_ACQM");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateACQM(GlobalConstants::CH6_PWR_REM_ACQM, args);
//}
//void EPICSLLRFInterface::update_CH7_PHASE_REM_ACQM(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH7_PHASE_REM_ACQM");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateACQM(GlobalConstants::CH7_PWR_REM_ACQM, args);
//}
//void EPICSLLRFInterface::update_CH8_PHASE_REM_ACQM(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH8_PHASE_REM_ACQM");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateACQM(GlobalConstants::CH8_PWR_REM_ACQM, args);
//}
//
//
//
//
//
//void EPICSLLRFInterface::update_CH1_PHASE_REM_SCAN(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH1_PHASE_REM_SCAN");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateSCAN(GlobalConstants::CH1_PHASE_REM_SCAN, args);
//}
//void EPICSLLRFInterface::update_CH1_PWR_REM_SCAN(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH1_PWR_REM_SCAN");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateSCAN(GlobalConstants::CH1_PWR_REM_SCAN, args);
//}
//void EPICSLLRFInterface::update_CH1_AMP_DER_SCAN(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH1_AMP_DER_SCAN");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateSCAN(GlobalConstants::CH1_AMP_DER_SCAN, args);
//}
//void EPICSLLRFInterface::update_CH1_PHASE_DER_SCAN(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH1_PHASE_DER_SCAN");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateSCAN(GlobalConstants::CH1_PHASE_DER_SCAN, args);
//}
//void EPICSLLRFInterface::update_CH1_PWR_LOC_SCAN(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH1_PWR_LOC_SCAN");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateSCAN(GlobalConstants::CH1_PWR_LOC_SCAN, args);
//}
//void EPICSLLRFInterface::update_CH2_PWR_REM_SCAN(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH2_PWR_REM_SCAN");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateSCAN(GlobalConstants::CH2_PWR_REM_SCAN, args);
//}
//void EPICSLLRFInterface::update_CH2_PHASE_REM_SCAN(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH2_PHASE_REM_SCAN");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateSCAN(GlobalConstants::CH2_PHASE_REM_SCAN, args);
//}
//void EPICSLLRFInterface::update_CH2_AMP_DER_SCAN(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH2_AMP_DER_SCAN");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateSCAN(GlobalConstants::CH2_AMP_DER_SCAN, args);
//}
//void EPICSLLRFInterface::update_CH2_PHASE_DER_SCAN(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH2_PHASE_DER_SCAN");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateSCAN(GlobalConstants::CH2_PHASE_DER_SCAN, args);
//}
//void EPICSLLRFInterface::update_CH2_PWR_LOC_SCAN(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH2_PWR_LOC_SCAN");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateSCAN(GlobalConstants::CH2_PWR_LOC_SCAN, args);
//}
//void EPICSLLRFInterface::update_CH3_PWR_REM_SCAN(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH3_PWR_REM_SCAN");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateSCAN(GlobalConstants::CH3_PWR_REM_SCAN, args);
//}
//void EPICSLLRFInterface::update_CH3_PHASE_REM_SCAN(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH3_PHASE_REM_SCAN");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateSCAN(GlobalConstants::CH3_PHASE_REM_SCAN, args);
//}
//void EPICSLLRFInterface::update_CH3_AMP_DER_SCAN(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH3_AMP_DER_SCAN");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateSCAN(GlobalConstants::CH3_AMP_DER_SCAN, args);
//}
//void EPICSLLRFInterface::update_CH3_PHASE_DER_SCAN(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH3_PHASE_DER_SCAN");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateSCAN(GlobalConstants::CH3_PHASE_DER_SCAN, args);
//}
//void EPICSLLRFInterface::update_CH3_PWR_LOC_SCAN(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH3_PWR_LOC_SCAN");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateSCAN(GlobalConstants::CH3_PWR_LOC_SCAN, args);
//}
//void EPICSLLRFInterface::update_CH4_PWR_REM_SCAN(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH4_PWR_REM_SCAN");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateSCAN(GlobalConstants::CH4_PWR_REM_SCAN, args);
//}
//void EPICSLLRFInterface::update_CH4_PHASE_REM_SCAN(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH4_PHASE_REM_SCAN");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateSCAN(GlobalConstants::CH4_PHASE_REM_SCAN, args);
//}
//void EPICSLLRFInterface::update_CH4_AMP_DER_SCAN(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH4_AMP_DER_SCAN");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateSCAN(GlobalConstants::CH4_AMP_DER_SCAN, args);
//}
//void EPICSLLRFInterface::update_CH4_PHASE_DER_SCAN(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH4_PHASE_DER_SCAN");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateSCAN(GlobalConstants::CH4_PHASE_DER_SCAN, args);
//}
//void EPICSLLRFInterface::update_CH4_PWR_LOC_SCAN(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH4_PWR_LOC_SCAN");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateSCAN(GlobalConstants::CH4_PWR_LOC_SCAN, args);
//}
//void EPICSLLRFInterface::update_CH5_PWR_REM_SCAN(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH5_PWR_REM_SCAN");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateSCAN(GlobalConstants::CH5_PWR_REM_SCAN, args);
//}
//void EPICSLLRFInterface::update_CH5_PHASE_REM_SCAN(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH5_PHASE_REM_SCAN");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateSCAN(GlobalConstants::CH5_PHASE_REM_SCAN, args);
//}
//void EPICSLLRFInterface::update_CH5_AMP_DER_SCAN(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH5_AMP_DER_SCAN");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateSCAN(GlobalConstants::CH5_AMP_DER_SCAN, args);
//}
//void EPICSLLRFInterface::update_CH5_PHASE_DER_SCAN(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH5_PHASE_DER_SCAN");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateSCAN(GlobalConstants::CH5_PHASE_DER_SCAN, args);
//}
//void EPICSLLRFInterface::update_CH5_PWR_LOC_SCAN(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH5_PWR_LOC_SCAN");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateSCAN(GlobalConstants::CH5_PWR_LOC_SCAN, args);
//}
//void EPICSLLRFInterface::update_CH6_PWR_REM_SCAN(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH6_PWR_REM_SCAN");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateSCAN(GlobalConstants::CH6_PWR_REM_SCAN, args);
//}
//void EPICSLLRFInterface::update_CH6_PHASE_REM_SCAN(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH6_PHASE_REM_SCAN");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateSCAN(GlobalConstants::CH6_PHASE_REM_SCAN, args);
//}
//void EPICSLLRFInterface::update_CH6_AMP_DER_SCAN(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH6_AMP_DER_SCAN");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateSCAN(GlobalConstants::CH6_AMP_DER_SCAN, args);
//}
//void EPICSLLRFInterface::update_CH6_PHASE_DER_SCAN(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH6_PHASE_DER_SCAN");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateSCAN(GlobalConstants::CH6_PHASE_DER_SCAN, args);
//}
//void EPICSLLRFInterface::update_CH6_PWR_LOC_SCAN(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH6_PWR_LOC_SCAN");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateSCAN(GlobalConstants::CH6_PWR_LOC_SCAN, args);
//}
//void EPICSLLRFInterface::update_CH7_PWR_REM_SCAN(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH7_PWR_REM_SCAN");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateSCAN(GlobalConstants::CH7_PWR_REM_SCAN, args);
//}
//void EPICSLLRFInterface::update_CH7_PHASE_REM_SCAN(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH7_PHASE_REM_SCAN");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateSCAN(GlobalConstants::CH7_PHASE_REM_SCAN, args);
//}
//void EPICSLLRFInterface::update_CH7_AMP_DER_SCAN(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH7_AMP_DER_SCAN");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateSCAN(GlobalConstants::CH7_AMP_DER_SCAN, args);
//}
//void EPICSLLRFInterface::update_CH7_PHASE_DER_SCAN(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH7_PHASE_DER_SCAN");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateSCAN(GlobalConstants::CH7_PHASE_DER_SCAN, args);
//}
//void EPICSLLRFInterface::update_CH7_PWR_LOC_SCAN(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH7_PWR_LOC_SCAN");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateSCAN(GlobalConstants::CH7_PWR_LOC_SCAN, args);
//}
//void EPICSLLRFInterface::update_CH8_PWR_REM_SCAN(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH8_PWR_REM_SCAN");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateSCAN(GlobalConstants::CH8_PWR_REM_SCAN, args);
//}
//void EPICSLLRFInterface::update_CH8_PHASE_REM_SCAN(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH8_PHASE_REM_SCAN");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateSCAN(GlobalConstants::CH8_PHASE_REM_SCAN, args);
//}
//void EPICSLLRFInterface::update_CH8_AMP_DER_SCAN(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH8_AMP_DER_SCAN");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateSCAN(GlobalConstants::CH8_AMP_DER_SCAN, args);
//}
//void EPICSLLRFInterface::update_CH8_PHASE_DER_SCAN(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH8_PHASE_DER_SCAN");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateSCAN(GlobalConstants::CH8_PHASE_DER_SCAN, args);
//}
//void EPICSLLRFInterface::update_CH8_PWR_LOC_SCAN(const struct event_handler_args args)
//{
//	messenger.printDebugMessage("update_CH8_PWR_LOC_SCAN");
//	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
//	recastLLRF->updateSCAN(GlobalConstants::CH8_PWR_LOC_SCAN, args);
//}
//
//




