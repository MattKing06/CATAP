#include "EPICSLLRFInterface.h"
#include "LLRFPVRecords.h"
#include "GlobalFunctions.h"


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

void EPICSLLRFInterface::update_KEEP_ALIVE(const struct event_handler_args args)
{

}
void EPICSLLRFInterface::update_AMP_FF(const struct event_handler_args args)
{

}
void EPICSLLRFInterface::update_TRIG_SOURCE(const struct event_handler_args args)
{}
void EPICSLLRFInterface::update_AMP_SP(const struct event_handler_args args)
{}
void EPICSLLRFInterface::update_PHI_FF(const struct event_handler_args args)
{}
void EPICSLLRFInterface::update_PHI_SP(const struct event_handler_args args)
{}
void EPICSLLRFInterface::update_RF_OUTPUT(const struct event_handler_args args)
{}
void EPICSLLRFInterface::update_FF_PHASE_LOCK_STATE(const struct event_handler_args args)
{}
void EPICSLLRFInterface::update_FF_AMP_LOCK_STATE(const struct event_handler_args args)
{}
void EPICSLLRFInterface::update_TIME_VECTOR(const struct event_handler_args args)
{}
void EPICSLLRFInterface::update_PULSE_OFFSET(const struct event_handler_args args)
{}
void EPICSLLRFInterface::update_PULSE_LENGTH(const struct event_handler_args args)
{}
void EPICSLLRFInterface::update_INTERLOCK(const struct event_handler_args args)
{}
void EPICSLLRFInterface::update_PULSE_SHAPE(const struct event_handler_args args)
{}
void EPICSLLRFInterface::update_PULSE_SHAPE_APPLY(const struct event_handler_args args)
{}
void EPICSLLRFInterface::update_CH1_INTERLOCK_STATUS(const struct event_handler_args args)
{}
void EPICSLLRFInterface::update_CH1_INTERLOCK_ENABLE(const struct event_handler_args args)
{}
void EPICSLLRFInterface::update_CH1_INTERLOCK_U(const struct event_handler_args args)
{}
void EPICSLLRFInterface::update_CH1_INTERLOCK_P(const struct event_handler_args args)
{}
void EPICSLLRFInterface::update_CH1_INTERLOCK_PDBM(const struct event_handler_args args)
{}
void EPICSLLRFInterface::update_CH2_INTERLOCK_STATUS(const struct event_handler_args args)
{}
void EPICSLLRFInterface::update_CH2_INTERLOCK_ENABLE(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH2_INTERLOCK_U(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH2_INTERLOCK_P(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH2_INTERLOCK_PDBM(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH3_INTERLOCK_STATUS(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH3_INTERLOCK_ENABLE(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH3_INTERLOCK_U(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH3_INTERLOCK_P(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH3_INTERLOCK_PDBM(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH4_INTERLOCK_STATUS(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH4_INTERLOCK_ENABLE(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH4_INTERLOCK_U(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH4_INTERLOCK_P(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH4_INTERLOCK_PDBM(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH5_INTERLOCK_STATUS(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH5_INTERLOCK_ENABLE(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH5_INTERLOCK_U(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH5_INTERLOCK_P(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH5_INTERLOCK_PDBM(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH6_INTERLOCK_STATUS(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH6_INTERLOCK_ENABLE(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH6_INTERLOCK_U(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH6_INTERLOCK_P(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH6_INTERLOCK_PDBM(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH7_INTERLOCK_STATUS(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH7_INTERLOCK_ENABLE(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH7_INTERLOCK_U(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH7_INTERLOCK_P(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH7_INTERLOCK_PDBM(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH8_INTERLOCK_STATUS(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH8_INTERLOCK_ENABLE(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH8_INTERLOCK_U(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH8_INTERLOCK_P(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH8_INTERLOCK_PDBM(const struct event_handler_args args){}
void EPICSLLRFInterface::update_LLRF_TRACES(const struct event_handler_args args){}
void EPICSLLRFInterface::update_LLRF_TRACES_SCAN(const struct event_handler_args args){}
void EPICSLLRFInterface::update_LLRF_TRACES_ACQM(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH1_PWR_REM_SCAN(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH1_PWR_REM(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH1_PWR_REM_ACQM(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH1_PHASE_REM_SCAN(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH1_PHASE_REM_ACQM(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH1_PHASE_REM(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH1_AMP_DER_SCAN(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH1_PHASE_DER_SCAN(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH1_PWR_LOC_SCAN(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH2_PWR_REM_SCAN(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH2_PWR_REM(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH2_PWR_REM_ACQM(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH2_PHASE_REM_SCAN(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH2_PHASE_REM_ACQM(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH2_PHASE_REM(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH2_AMP_DER_SCAN(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH2_PHASE_DER_SCAN(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH2_PWR_LOC_SCAN(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH3_PWR_REM_SCAN(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH3_PWR_REM(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH3_PWR_REM_ACQM(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH3_PHASE_REM_SCAN(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH3_PHASE_REM_ACQM(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH3_PHASE_REM(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH3_AMP_DER_SCAN(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH3_PHASE_DER_SCAN(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH3_PWR_LOC_SCAN(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH4_PWR_REM_SCAN(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH4_PWR_REM(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH4_PWR_REM_ACQM(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH4_PHASE_REM_SCAN(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH4_PHASE_REM_ACQM(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH4_PHASE_REM(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH4_AMP_DER_SCAN(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH4_PHASE_DER_SCAN(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH4_PWR_LOC_SCAN(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH5_PWR_REM_SCAN(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH5_PWR_REM(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH5_PWR_REM_ACQM(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH5_PHASE_REM_SCAN(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH5_PHASE_REM_ACQM(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH5_PHASE_REM(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH5_AMP_DER_SCAN(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH5_PHASE_DER_SCAN(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH5_PWR_LOC_SCAN(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH6_PWR_REM_SCAN(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH6_PWR_REM(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH6_PWR_REM_ACQM(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH6_PHASE_REM_SCAN(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH6_PHASE_REM_ACQM(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH6_PHASE_REM(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH6_AMP_DER_SCAN(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH6_PHASE_DER_SCAN(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH6_PWR_LOC_SCAN(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH7_PWR_REM_SCAN(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH7_PWR_REM(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH7_PWR_REM_ACQM(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH7_PHASE_REM_SCAN(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH7_PHASE_REM_ACQM(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH7_PHASE_REM(const struct event_handler_args args){}
void EPICSLLRFInterface::update_CH7_AMP_DER_SCAN(const struct event_handler_args args)
{}
void EPICSLLRFInterface::update_CH7_PHASE_DER_SCAN(const struct event_handler_args args)
{}
void EPICSLLRFInterface::update_CH7_PWR_LOC_SCAN(const struct event_handler_args args)
{}
void EPICSLLRFInterface::update_CH8_PWR_REM_SCAN(const struct event_handler_args args)
{}
void EPICSLLRFInterface::update_CH8_PWR_REM(const struct event_handler_args args)
{}
void EPICSLLRFInterface::update_CH8_PWR_REM_ACQM(const struct event_handler_args args)
{}
void EPICSLLRFInterface::update_CH8_PHASE_REM_SCAN(const struct event_handler_args args)
{}
void EPICSLLRFInterface::update_CH8_PHASE_REM_ACQM(const struct event_handler_args args)
{}
void EPICSLLRFInterface::update_CH8_PHASE_REM(const struct event_handler_args args)
{}
void EPICSLLRFInterface::update_CH8_AMP_DER_SCAN(const struct event_handler_args args)
{}
void EPICSLLRFInterface::update_CH8_PHASE_DER_SCAN(const struct event_handler_args args)
{}
void EPICSLLRFInterface::update_CH8_PWR_LOC_SCAN(const struct event_handler_args args)
{}
