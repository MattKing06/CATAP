#include "EPICSLLRFInterface.h"
#include "LLRFPVRecords.h"
#include "GlobalFunctions.h"
#include "GlobalConstants.h"
#include "GlobalStateEnums.h"
#include <mutex>          // std::mutex
std::mutex llrf_mtx_1;           // mutex for critical section

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
		messenger.printDebugMessage("!!WARNING!! NO UPDATE FUNCTION FOUND FOR: " + pvStruct.pvRecord);
		pvStruct.updateFunction = nullptr;
	}
}

void EPICSLLRFInterface::update_AMP_FF(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	updateTimeStampDoublePair(args, recastLLRF->amp_ff);
	messenger.printDebugMessage("EPICS update_AMP_FF FOR: " + recastLLRF->getHardwareName(), " ", recastLLRF->amp_ff.second);
}
void EPICSLLRFInterface::update_TRIG_SOURCE(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	const struct dbr_time_enum* tv = (const struct dbr_time_enum*)(args.dbr);
	recastLLRF->trig_source.first = tv->stamp;
	switch (tv->value)
	{
	case 0:		recastLLRF->trig_source.second = STATE::OFF; break;
	case 1:		recastLLRF->trig_source.second = STATE::EXTERNAL; break;
	case 2:		recastLLRF->trig_source.second = STATE::INTERNAL; break;
	}
	messenger.printDebugMessage("EPICS update_TRIG_SOURCE FOR: " + recastLLRF->getHardwareName(), 
		" ", ENUM_TO_STRING(recastLLRF->trig_source.second));
}

void EPICSLLRFInterface::update_PULSE_SHAPE(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	// mutex from updateLLRFValue()
	const dbr_double_t* p_data = (const dbr_double_t*)args.dbr;
	std::copy(p_data, p_data + recastLLRF->pulse_shape.second.size(), recastLLRF->pulse_shape.second.begin());
}

void EPICSLLRFInterface::update_AMP_SP(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	updateTimeStampDoublePair(args, recastLLRF->amp_sp);
	messenger.printDebugMessage("EPICS update_AMP_SP FOR: " + recastLLRF->getHardwareName(), 
		" ", recastLLRF->amp_sp.second);
}
void EPICSLLRFInterface::update_MAX_AMP_SP(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	updateTimeStampDoublePair(args, recastLLRF->max_amp_sp);
	messenger.printDebugMessage("EPICS update_MAX_AMP_SP FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->max_amp_sp.second);
}
void EPICSLLRFInterface::update_PHI_FF(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	updateTimeStampDoublePair(args, recastLLRF->phi_ff);
	messenger.printDebugMessage("EPICS update_PHI_FF FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->phi_ff.second);
}
void EPICSLLRFInterface::update_PHI_SP(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	updateTimeStampDoublePair(args, recastLLRF->phi_sp);
	messenger.printDebugMessage("EPICS update_PHI_SP FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->phi_sp.second);
}
void EPICSLLRFInterface::update_RF_OUTPUT(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	updateTimeStampBoolPair(args, recastLLRF->rf_output);
	messenger.printDebugMessage("EPICS update_RF_OUTPUT FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->rf_output.second);
}
void EPICSLLRFInterface::update_FF_PHASE_LOCK_STATE(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	updateTimeStampBoolPair(args, recastLLRF->ff_phase_lock);
	messenger.printDebugMessage("EPICS update_FF_PHASE_LOCK_STATE FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->ff_phase_lock.second);
}
void EPICSLLRFInterface::update_FF_AMP_LOCK_STATE(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	updateTimeStampBoolPair(args, recastLLRF->ff_amp_lock);
	messenger.printDebugMessage("EPICS update_FF_AMP_LOCK_STATE FOR: " + recastLLRF->getHardwareName(),
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
	messenger.printDebugMessage("EPICS update_PULSE_OFFSET FOR: " + recastLLRF->getHardwareName(), " ", 
		recastLLRF->pulse_offset.second);
}

void EPICSLLRFInterface::update_LLRF_PULSE_DURATION(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	updateTimeStampDoublePair(args, recastLLRF->llrf_pulse_duration);
	messenger.printDebugMessage("EPICS update_LLRF_PULSE_DURATION FOR: " + recastLLRF->getHardwareName(), " = ", 
		recastLLRF->llrf_pulse_duration.second);
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
	messenger.printDebugMessage("EPICS update_INTERLOCK for " + recastLLRF->getHardwareName(), " = ", 
		ENUM_TO_STRING(recastLLRF->interlock_state.second));
}

void EPICSLLRFInterface::update_LLRF_TRACES(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_LLRF_TRACES");
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	messenger.printDebugMessage("splitOneTraceValues", args.dbr);
	recastLLRF->splitOneTraceValues((const struct dbr_time_double*)args.dbr);
}
void EPICSLLRFInterface::update_LLRF_TRACES_ACQM(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	update_trace_ACQM(args, recastLLRF->all_trace_acqm.at(LLRFRecords::ONE_TRACE_ACQM));
	messenger.printDebugMessage("update_LLRF_TRACES_ACQM FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_acqm.at(LLRFRecords::ONE_TRACE_ACQM).second);
}
void EPICSLLRFInterface::update_LLRF_TRACES_SCAN(const struct event_handler_args args)
{
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	update_trace_SCAN(args, recastLLRF->all_trace_scan.at(LLRFRecords::ONE_TRACE_SCAN));
	messenger.printDebugMessage("update_LLRF_TRACES_SCAN FOR: " + recastLLRF->getHardwareName(),
		" ", recastLLRF->all_trace_scan.at(LLRFRecords::ONE_TRACE_SCAN).second);
}

void EPICSLLRFInterface::update_CH1_INTERLOCK_STATUS(const struct event_handler_args args)
{
	updateInterLockStatus(LLRFRecords::LLRF_CH1_INTERLOCK, LLRFRecords::CH1, args);
}
void EPICSLLRFInterface::update_CH2_INTERLOCK_STATUS(const struct event_handler_args args)
{
	updateInterLockStatus(LLRFRecords::LLRF_CH2_INTERLOCK, LLRFRecords::CH2, args);
}
void EPICSLLRFInterface::update_CH3_INTERLOCK_STATUS(const struct event_handler_args args)
{
	updateInterLockStatus(LLRFRecords::LLRF_CH3_INTERLOCK, LLRFRecords::CH3, args);
}
void EPICSLLRFInterface::update_CH4_INTERLOCK_STATUS(const struct event_handler_args args)
{
	updateInterLockStatus(LLRFRecords::LLRF_CH4_INTERLOCK, LLRFRecords::CH4, args);
}
void EPICSLLRFInterface::update_CH5_INTERLOCK_STATUS(const struct event_handler_args args)
{
	updateInterLockStatus(LLRFRecords::LLRF_CH5_INTERLOCK, LLRFRecords::CH5, args);
}
void EPICSLLRFInterface::update_CH6_INTERLOCK_STATUS(const struct event_handler_args args)
{
	updateInterLockStatus(LLRFRecords::LLRF_CH6_INTERLOCK, LLRFRecords::CH6, args);
}
void EPICSLLRFInterface::update_CH7_INTERLOCK_STATUS(const struct event_handler_args args)
{
	updateInterLockStatus(LLRFRecords::LLRF_CH7_INTERLOCK, LLRFRecords::CH7, args);
}
void EPICSLLRFInterface::update_CH8_INTERLOCK_STATUS(const struct event_handler_args args)
{
	updateInterLockStatus(LLRFRecords::LLRF_CH8_INTERLOCK, LLRFRecords::CH8, args);
}

void EPICSLLRFInterface::updateInterLockStatus(const std::string& ch, const std::string& CH, const struct event_handler_args& args)
{

	//messenger.printDebugMessage("updateInterLockStatus");
	// TODO maybe all the sorting out the names could be put into 1 function to make it a bit clearner, 
	// but ... THIS works, so only change when eveyrthing else is working 
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	const struct dbr_time_enum* tv = (const struct dbr_time_enum*)(args.dbr);
	if (GlobalFunctions::entryExists(recastLLRF->all_trace_interlocks, ch))
	{
		updateTimeStampBoolPair(args,recastLLRF->all_trace_interlocks.at(ch).status);
		//messenger.printDebugMessage("updateInterLock STATUS: ", ch, " :: = :: ", recastLLRF->all_trace_interlocks.at(ch).status.second);
		//recastLLRF->all_trace_interlocks.at(ch).status = (bool)tv->value;
		std::string trace;
		if (recastLLRF->getTraceFromChannelData(CH, trace))
		{
			//messenger.printDebugMessage(CH, " has a useful name of ", trace);
			trace.append("_POWER");
			//messenger.printDebugMessage("NOW Looking for ", trace);
			if(GlobalFunctions::entryExists(recastLLRF->trace_data_map, trace))
			{
				recastLLRF->all_trace_interlocks.at(ch).name = trace;
				recastLLRF->trace_data_map.at(trace).interlock_state = recastLLRF->all_trace_interlocks.at(ch).status;
				//messenger.printDebugMessage(trace, " interlock_state = ", recastLLRF->trace_data_map.at(trace).interlock_state.second);
			}
			else
			{
				//messenger.printDebugMessage(trace, " NOT in trace_data_map");
			}
		}
		else
		{
			//messenger.printDebugMessage(CH, " getTraceFromChannelData returned false ");
		}
	}
	else
	{
		//messenger.printDebugMessage(ch, " NOT in all_trace_interlocks");
	}
}

void EPICSLLRFInterface::update_CH1_INTERLOCK_PDBM(const struct event_handler_args args)
{
	updateInterLockPDBM(LLRFRecords::LLRF_CH1_INTERLOCK, LLRFRecords::CH1, args);
}
void EPICSLLRFInterface::update_CH2_INTERLOCK_PDBM(const struct event_handler_args args)
{
	updateInterLockPDBM(LLRFRecords::LLRF_CH2_INTERLOCK, LLRFRecords::CH2, args);
}
void EPICSLLRFInterface::update_CH3_INTERLOCK_PDBM(const struct event_handler_args args)
{
	updateInterLockPDBM(LLRFRecords::LLRF_CH3_INTERLOCK, LLRFRecords::CH3, args);
}
void EPICSLLRFInterface::update_CH4_INTERLOCK_PDBM(const struct event_handler_args args)
{
	updateInterLockPDBM(LLRFRecords::LLRF_CH4_INTERLOCK, LLRFRecords::CH4, args);
}
void EPICSLLRFInterface::update_CH5_INTERLOCK_PDBM(const struct event_handler_args args)
{
	updateInterLockPDBM(LLRFRecords::LLRF_CH5_INTERLOCK, LLRFRecords::CH5, args);
}
void EPICSLLRFInterface::update_CH6_INTERLOCK_PDBM(const struct event_handler_args args)
{
	updateInterLockPDBM(LLRFRecords::LLRF_CH6_INTERLOCK, LLRFRecords::CH6, args);
}
void EPICSLLRFInterface::update_CH7_INTERLOCK_PDBM(const struct event_handler_args args)
{
	updateInterLockPDBM(LLRFRecords::LLRF_CH7_INTERLOCK, LLRFRecords::CH7, args);
}
void EPICSLLRFInterface::update_CH8_INTERLOCK_PDBM(const struct event_handler_args args)
{
	updateInterLockPDBM(LLRFRecords::LLRF_CH8_INTERLOCK, LLRFRecords::CH8, args);
}
void EPICSLLRFInterface::updateInterLockPDBM(const std::string& ch, const std::string& CH, const struct event_handler_args& args)
{

	//// TODO maybe all the sorting out the names could be put into 1 function to make it a bit clearner, 
	//// but ... THIS works, so only change when eveyrthing else is working 
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	const struct dbr_time_enum* tv = (const struct dbr_time_enum*)(args.dbr);
	if (GlobalFunctions::entryExists(recastLLRF->all_trace_interlocks, ch))
	{
		updateTimeStampDoublePair(args, recastLLRF->all_trace_interlocks.at(ch).pdbm_level);
		messenger.printDebugMessage("updateInterLock PDBM: ", ch, " :: = :: ", recastLLRF->all_trace_interlocks.at(ch).pdbm_level.second);
		//recastLLRF->all_trace_interlocks.at(ch).status = (bool)tv->value;
		std::string trace;
		if (recastLLRF->getTraceFromChannelData(CH, trace))
		{
			//messenger.printDebugMessage(CH, " has a useful name of ", trace);
			trace.append("_POWER");
			//messenger.printDebugMessage("NOW Looking for ", trace);
			if (GlobalFunctions::entryExists(recastLLRF->trace_data_map, trace))
			{
				recastLLRF->all_trace_interlocks.at(ch).name = trace;
				recastLLRF->trace_data_map.at(trace).pdbm_level = recastLLRF->all_trace_interlocks.at(ch).pdbm_level;
				//messenger.printDebugMessage(trace, " interlock_state = ", recastLLRF->trace_data_map.at(trace).interlock_state.second);
			}
			else
			{
				//messenger.printDebugMessage(trace, " NOT in trace_data_map");
			}
		}
		else
		{
			//messenger.printDebugMessage(CH, " getTraceFromChannelData returned false ");
		}
	}
	else
	{
		//messenger.printDebugMessage(ch, " NOT in all_trace_interlocks");
	}
}


void EPICSLLRFInterface::update_CH1_INTERLOCK_P(const struct event_handler_args args)
{
	updateInterLockP(LLRFRecords::LLRF_CH1_INTERLOCK, LLRFRecords::CH1, args);
}
void EPICSLLRFInterface::update_CH2_INTERLOCK_P(const struct event_handler_args args)
{
	updateInterLockP(LLRFRecords::LLRF_CH2_INTERLOCK, LLRFRecords::CH2, args);
}
void EPICSLLRFInterface::update_CH3_INTERLOCK_P(const struct event_handler_args args)
{
	updateInterLockP(LLRFRecords::LLRF_CH3_INTERLOCK, LLRFRecords::CH3, args);
}
void EPICSLLRFInterface::update_CH4_INTERLOCK_P(const struct event_handler_args args)
{
	updateInterLockP(LLRFRecords::LLRF_CH4_INTERLOCK, LLRFRecords::CH4, args);
}
void EPICSLLRFInterface::update_CH5_INTERLOCK_P(const struct event_handler_args args)
{
	updateInterLockP(LLRFRecords::LLRF_CH5_INTERLOCK, LLRFRecords::CH5, args);
}
void EPICSLLRFInterface::update_CH6_INTERLOCK_P(const struct event_handler_args args)
{
	updateInterLockP(LLRFRecords::LLRF_CH6_INTERLOCK, LLRFRecords::CH6, args);
}
void EPICSLLRFInterface::update_CH7_INTERLOCK_P(const struct event_handler_args args)
{
	updateInterLockP(LLRFRecords::LLRF_CH7_INTERLOCK, LLRFRecords::CH7, args);
}
void EPICSLLRFInterface::update_CH8_INTERLOCK_P(const struct event_handler_args args)
{
	updateInterLockP(LLRFRecords::LLRF_CH8_INTERLOCK, LLRFRecords::CH8, args);
}
void EPICSLLRFInterface::updateInterLockP(const std::string& ch, const std::string& CH, const struct event_handler_args& args)
{
	////messenger.printDebugMessage("updateInterLockStatus");

	//// TODO maybe all the sorting out the names could be put into 1 function to make it a bit clearner, 
	//// but ... THIS works, so only change when eveyrthing else is working 
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	const struct dbr_time_enum* tv = (const struct dbr_time_enum*)(args.dbr);
	if (GlobalFunctions::entryExists(recastLLRF->all_trace_interlocks, ch))
	{
		updateTimeStampDoublePair(args, recastLLRF->all_trace_interlocks.at(ch).p_level);
		messenger.printDebugMessage("updateInterLock P: ", ch, " :: = :: ", recastLLRF->all_trace_interlocks.at(ch).p_level.second);
		//recastLLRF->all_trace_interlocks.at(ch).status = (bool)tv->value;
		std::string trace;
		if (recastLLRF->getTraceFromChannelData(CH, trace))
		{
			//messenger.printDebugMessage(CH, " has a useful name of ", trace);
			trace.append("_POWER");
			//messenger.printDebugMessage("NOW Looking for ", trace);
			if (GlobalFunctions::entryExists(recastLLRF->trace_data_map, trace))
			{
				recastLLRF->all_trace_interlocks.at(ch).name = trace;
				recastLLRF->trace_data_map.at(trace).p_level = recastLLRF->all_trace_interlocks.at(ch).p_level;
				//messenger.printDebugMessage(trace, " interlock_state = ", recastLLRF->trace_data_map.at(trace).interlock_state.second);
			}
			else
			{
				//messenger.printDebugMessage(trace, " NOT in trace_data_map");
			}
		}
		else
		{
			//messenger.printDebugMessage(CH, " getTraceFromChannelData returned false ");
		}
	}
	else
	{
		//messenger.printDebugMessage(ch, " NOT in all_trace_interlocks");
	}
}


void EPICSLLRFInterface::update_CH1_INTERLOCK_U(const struct event_handler_args args)
{
	updateInterLockU(LLRFRecords::LLRF_CH1_INTERLOCK, LLRFRecords::CH1, args);
}
void EPICSLLRFInterface::update_CH2_INTERLOCK_U(const struct event_handler_args args)
{
	updateInterLockU(LLRFRecords::LLRF_CH2_INTERLOCK, LLRFRecords::CH2, args);
}
void EPICSLLRFInterface::update_CH3_INTERLOCK_U(const struct event_handler_args args)
{
	updateInterLockU(LLRFRecords::LLRF_CH3_INTERLOCK, LLRFRecords::CH3, args);
}
void EPICSLLRFInterface::update_CH4_INTERLOCK_U(const struct event_handler_args args)
{
	updateInterLockU(LLRFRecords::LLRF_CH4_INTERLOCK, LLRFRecords::CH4, args);
}
void EPICSLLRFInterface::update_CH5_INTERLOCK_U(const struct event_handler_args args)
{
	updateInterLockU(LLRFRecords::LLRF_CH5_INTERLOCK, LLRFRecords::CH5, args);
}
void EPICSLLRFInterface::update_CH6_INTERLOCK_U(const struct event_handler_args args)
{
	updateInterLockU(LLRFRecords::LLRF_CH6_INTERLOCK, LLRFRecords::CH6, args);
}
void EPICSLLRFInterface::update_CH7_INTERLOCK_U(const struct event_handler_args args)
{
	updateInterLockU(LLRFRecords::LLRF_CH7_INTERLOCK, LLRFRecords::CH7, args);
}
void EPICSLLRFInterface::update_CH8_INTERLOCK_U(const struct event_handler_args args)
{
	updateInterLockU(LLRFRecords::LLRF_CH8_INTERLOCK, LLRFRecords::CH8, args);
}
void EPICSLLRFInterface::updateInterLockU(const std::string& ch, const std::string& CH, const struct event_handler_args& args)
{
	//messenger.printDebugMessage("updateInterLockStatus");
	// TODO maybe all the sorting out the names could be put into 1 function to make it a bit clearner, 
	// but ... THIS works, so only change when eveyrthing else is working 
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	const struct dbr_time_enum* tv = (const struct dbr_time_enum*)(args.dbr);
	if (GlobalFunctions::entryExists(recastLLRF->all_trace_interlocks, ch))
	{
		updateTimeStampDoublePair(args, recastLLRF->all_trace_interlocks.at(ch).u_level);
		//recastLLRF->all_trace_interlocks.at(ch).status = (bool)tv->value;
		messenger.printDebugMessage("updateInterLock U: ", ch, " :: = :: ", recastLLRF->all_trace_interlocks.at(ch).u_level.second);
		std::string trace;
		if (recastLLRF->getTraceFromChannelData(CH, trace))
		{
			//messenger.printDebugMessage(CH, " has a useful name of ", trace);
			trace.append("_POWER");
			//messenger.printDebugMessage("NOW Looking for ", trace);
			if (GlobalFunctions::entryExists(recastLLRF->trace_data_map, trace))
			{
				recastLLRF->all_trace_interlocks.at(ch).name = trace;
				recastLLRF->trace_data_map.at(trace).u_level = recastLLRF->all_trace_interlocks.at(ch).u_level;
				//messenger.printDebugMessage(trace, " interlock_state = ", recastLLRF->trace_data_map.at(trace).interlock_state.second);
			}
			else
			{
				//messenger.printDebugMessage(trace, " NOT in trace_data_map");
			}
		}
		else
		{
			//messenger.printDebugMessage(CH, " getTraceFromChannelData returned false ");
		}
	}
	else
	{
		//messenger.printDebugMessage(ch, " NOT in all_trace_interlocks");
	}
}





void EPICSLLRFInterface::update_CH1_INTERLOCK_ENABLE(const struct event_handler_args args)
{
	updateInterLockENABLE(LLRFRecords::LLRF_CH1_INTERLOCK, LLRFRecords::CH1, args);
}
void EPICSLLRFInterface::update_CH2_INTERLOCK_ENABLE(const struct event_handler_args args)
{
	updateInterLockENABLE(LLRFRecords::LLRF_CH2_INTERLOCK, LLRFRecords::CH2, args);
}
void EPICSLLRFInterface::update_CH3_INTERLOCK_ENABLE(const struct event_handler_args args)
{
	updateInterLockENABLE(LLRFRecords::LLRF_CH3_INTERLOCK, LLRFRecords::CH3, args);
}
void EPICSLLRFInterface::update_CH4_INTERLOCK_ENABLE(const struct event_handler_args args)
{
	updateInterLockENABLE(LLRFRecords::LLRF_CH4_INTERLOCK, LLRFRecords::CH4, args);
}
void EPICSLLRFInterface::update_CH5_INTERLOCK_ENABLE(const struct event_handler_args args)
{
	updateInterLockENABLE(LLRFRecords::LLRF_CH5_INTERLOCK, LLRFRecords::CH5, args);
}
void EPICSLLRFInterface::update_CH6_INTERLOCK_ENABLE(const struct event_handler_args args)
{
	updateInterLockENABLE(LLRFRecords::LLRF_CH6_INTERLOCK, LLRFRecords::CH6, args);
}
void EPICSLLRFInterface::update_CH7_INTERLOCK_ENABLE(const struct event_handler_args args)
{
	updateInterLockENABLE(LLRFRecords::LLRF_CH7_INTERLOCK, LLRFRecords::CH7, args);
}
void EPICSLLRFInterface::update_CH8_INTERLOCK_ENABLE(const struct event_handler_args args)
{
	updateInterLockENABLE(LLRFRecords::LLRF_CH8_INTERLOCK, LLRFRecords::CH8, args);
}
void EPICSLLRFInterface::updateInterLockENABLE(const std::string& ch, const std::string& CH, const struct event_handler_args& args)
{
	//// TODO maybe all the sorting out the names could be put into 1 function to make it a bit clearner, 
	//// but ... THIS works, so only change when eveyrthing else is working 
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	const struct dbr_time_enum* tv = (const struct dbr_time_enum*)(args.dbr);
	if (GlobalFunctions::entryExists(recastLLRF->all_trace_interlocks, ch))
	{
		updateTimeStampBoolPair(args, recastLLRF->all_trace_interlocks.at(ch).enable);
		messenger.printDebugMessage("updateInterLock ENABLE: ", ch, " :: = :: ", recastLLRF->all_trace_interlocks.at(ch).enable.second);
		//recastLLRF->all_trace_interlocks.at(ch).status = (bool)tv->value;
		std::string trace;
		if (recastLLRF->getTraceFromChannelData(CH, trace))
		{
			//messenger.printDebugMessage(CH, " has a useful name of ", trace);
			trace.append("_POWER");
			//messenger.printDebugMessage("NOW Looking for ", trace);
			if (GlobalFunctions::entryExists(recastLLRF->trace_data_map, trace))
			{
				recastLLRF->all_trace_interlocks.at(ch).name = trace;
				recastLLRF->trace_data_map.at(trace).enable = recastLLRF->all_trace_interlocks.at(ch).enable;
				//messenger.printDebugMessage(trace, " interlock_state = ", recastLLRF->trace_data_map.at(trace).interlock_state.second);
			}
			else
			{
				//messenger.printDebugMessage(trace, " NOT in trace_data_map");
			}
		}
		else
		{
			//messenger.printDebugMessage(CH, " getTraceFromChannelData returned false ");
		}
	}
	else
	{
		//messenger.printDebugMessage(ch, " NOT in all_trace_interlocks");
	}
}


void EPICSLLRFInterface::updateChannnelSCAN(const std::string& ch, const std::string& CH, const struct event_handler_args& args)
{
	std::lock_guard<std::mutex> lg(llrf_mtx_1);  // This now locked your mutex mtx.lock();
	 messenger.printDebugMessage("updateChannnelSCAN");
	 //TODO maybe all the sorting out the names could be put into 1 function to make it a bit clearner, 
	 //but ... THIS works, so only change when eveyrthing else is working 
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	/*First update all_trace_scan, this conatins all the trace SCAN */
	if (GlobalFunctions::entryExists(recastLLRF->all_trace_scan, ch))
	{
		std::pair<epicsTimeStamp, STATE> t = recastLLRF->all_trace_scan.at(ch);
		update_trace_SCAN(args, recastLLRF->all_trace_scan.at(ch));
		messenger.printDebugMessage("updateChannnelSCAN FOR: " + recastLLRF->getHardwareName(),
			" ", ch, ", to ", ENUM_TO_STRING(recastLLRF->all_trace_scan.at(ch).second));
	}
	else
	{
		messenger.printMessage(ch, " NOT FOUND IN ALL_TRACE_SCAN FOR: ", recastLLRF->getHardwareName());
	}


	// NOW WE FIND OUT IF THE PASSED SCAN IS IN ALL TRACE DATA 
	std::string trace;
	if (recastLLRF->getTraceFromChannelData(CH, trace))
	{
		messenger.printDebugMessage(CH, " has a useful name of ", trace);

		if (GlobalFunctions::stringIsSubString(ch, "PWR_REM") )
		{
			messenger.printDebugMessage(ch, " is a power remote trace for ", trace);
			std::string powerTraceName = trace+"_POWER";
			//trace.append("_POWER");
			if (GlobalFunctions::entryExists(recastLLRF->trace_data_map, powerTraceName))
			{
				recastLLRF->trace_data_map.at(powerTraceName).scan = recastLLRF->all_trace_scan.at(ch);
			}

		}
		else if (GlobalFunctions::stringIsSubString(ch, "PHASE_REM"))
		{
			std::string phaseTraceName = trace+"_PHASE";
			messenger.printDebugMessage(ch, " is a phase remote trace for ", trace);
			//trace.append("_PHASE");
			if (GlobalFunctions::entryExists(recastLLRF->trace_data_map, phaseTraceName))
			{
				recastLLRF->trace_data_map.at(phaseTraceName).scan = recastLLRF->all_trace_scan.at(ch);
			}

		}
		else
		{
			messenger.printDebugMessage(ch, " is for a trace not in trace_data_map (", trace,")");
		}

		if(GlobalFunctions::entryExists(recastLLRF->trace_data_map, trace))
		{
			recastLLRF->trace_data_map.at(trace).scan = recastLLRF->all_trace_scan.at(ch);
			messenger.printDebugMessage(trace, " data added to trace_data_map, ", 
				ENUM_TO_STRING(recastLLRF->trace_data_map.at(trace).scan.second));
		}
		else
		{
			messenger.printDebugMessage(trace, " NOT FOUND in trace_data_map");
		}
	}
	else
	{
		messenger.printDebugMessage(CH, " getTraceFromChannelData returned false ");
	}
}
void EPICSLLRFInterface::update_trace_SCAN(const struct event_handler_args& args, std::pair<epicsTimeStamp, STATE >& scan)
{
	messenger.printDebugMessage("update_trace_SCAN");
	std::pair<epicsTimeStamp, unsigned short> pairToUpdate = getTimeStampUShortPair(args);
	scan.first = pairToUpdate.first;

	messenger.printDebugMessage("update_trace_SCAN, pairToUpdate.second = ", pairToUpdate.second);
	// TODO this lookup is done lots, so use version from GlobalConstants / Global Functions  
	switch (pairToUpdate.second)
	{
	case 0:			scan.second = STATE::PASSIVE; break;
	case 1:			scan.second = STATE::EVENT; break;
	case 2:			scan.second = STATE::IO_INTR; break;
	case 3:			scan.second = STATE::TEN; break;
	case 4:			scan.second = STATE::FIVE; break;
	case 5:			scan.second = STATE::TWO; break;
	case 6:			scan.second = STATE::ONE; break;
	case 7:			scan.second = STATE::ZERO_POINT_FIVE; break;
	case 8:			scan.second = STATE::ZERO_POINT_TWO; break;
	case 9:			scan.second = STATE::ZERO_POINT_ONE; break;
	case 10:		scan.second = STATE::ZERO_POINT_ZERO_FIVE; break;
	default:		scan.second = STATE::UNKNOWN;
	}
	messenger.printDebugMessage("update_trace_SCAN to ", ENUM_TO_STRING(scan.second));
}
void EPICSLLRFInterface::update_CH1_PHASE_REM_SCAN(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH1_PHASE_REM_SCAN");
	updateChannnelSCAN(LLRFRecords::CH1_PHASE_REM, LLRFRecords::CH1, args);
}
void EPICSLLRFInterface::update_CH1_PWR_REM_SCAN(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH1_PWR_REM_SCAN");
	updateChannnelSCAN(LLRFRecords::CH1_PWR_REM, LLRFRecords::CH1, args);
}
void EPICSLLRFInterface::update_CH1_AMP_DER_SCAN(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH1_AMP_DER_SCAN");
	updateChannnelSCAN(LLRFRecords::CH1_AMP_DER, LLRFRecords::CH1, args);
}
void EPICSLLRFInterface::update_CH1_PHASE_DER_SCAN(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH1_PHASE_DER_SCAN");
	updateChannnelSCAN(LLRFRecords::CH1_PHASE_DER, LLRFRecords::CH1, args);
}
void EPICSLLRFInterface::update_CH1_PWR_LOC_SCAN(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH1_PWR_LOC_SCAN");
	updateChannnelSCAN(LLRFRecords::CH1_PWR_LOC, LLRFRecords::CH1, args);
}
void EPICSLLRFInterface::update_CH2_PWR_REM_SCAN(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH2_PWR_REM_SCAN");
	updateChannnelSCAN(LLRFRecords::CH2_PWR_REM, LLRFRecords::CH2, args);
}
void EPICSLLRFInterface::update_CH2_PHASE_REM_SCAN(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH2_PHASE_REM_SCAN");
	updateChannnelSCAN(LLRFRecords::CH2_PHASE_REM, LLRFRecords::CH2, args);
}
void EPICSLLRFInterface::update_CH2_AMP_DER_SCAN(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH2_AMP_DER_SCAN");
	updateChannnelSCAN(LLRFRecords::CH2_AMP_DER, LLRFRecords::CH2, args);
}
void EPICSLLRFInterface::update_CH2_PHASE_DER_SCAN(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH2_PHASE_DER_SCAN");
	updateChannnelSCAN(LLRFRecords::CH2_PHASE_DER, LLRFRecords::CH2, args);
}
void EPICSLLRFInterface::update_CH2_PWR_LOC_SCAN(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH2_PWR_LOC_SCAN");
	updateChannnelSCAN(LLRFRecords::CH2_PWR_LOC, LLRFRecords::CH2, args);
}
void EPICSLLRFInterface::update_CH3_PWR_REM_SCAN(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH3_PWR_REM_SCAN");
	updateChannnelSCAN(LLRFRecords::CH3_PWR_REM, LLRFRecords::CH3, args);
}
void EPICSLLRFInterface::update_CH3_PHASE_REM_SCAN(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH3_PHASE_REM_SCAN");
	updateChannnelSCAN(LLRFRecords::CH3_PHASE_REM, LLRFRecords::CH3, args);
}
void EPICSLLRFInterface::update_CH3_AMP_DER_SCAN(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH3_AMP_DER_SCAN");
	updateChannnelSCAN(LLRFRecords::CH3_AMP_DER, LLRFRecords::CH3, args);
}
void EPICSLLRFInterface::update_CH3_PHASE_DER_SCAN(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH3_PHASE_DER_SCAN");
	updateChannnelSCAN(LLRFRecords::CH3_PHASE_DER, LLRFRecords::CH3, args);
}
void EPICSLLRFInterface::update_CH3_PWR_LOC_SCAN(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH3_PWR_LOC_SCAN");
	updateChannnelSCAN(LLRFRecords::CH3_PWR_LOC, LLRFRecords::CH3, args);
}
void EPICSLLRFInterface::update_CH4_PWR_REM_SCAN(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH4_PWR_REM_SCAN");
	updateChannnelSCAN(LLRFRecords::CH4_PWR_REM, LLRFRecords::CH4, args);
}
void EPICSLLRFInterface::update_CH4_PHASE_REM_SCAN(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH4_PHASE_REM_SCAN");
	updateChannnelSCAN(LLRFRecords::CH4_PHASE_REM, LLRFRecords::CH4, args);
}
void EPICSLLRFInterface::update_CH4_AMP_DER_SCAN(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH4_AMP_DER_SCAN");
	updateChannnelSCAN(LLRFRecords::CH4_AMP_DER, LLRFRecords::CH4, args);
}
void EPICSLLRFInterface::update_CH4_PHASE_DER_SCAN(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH4_PHASE_DER_SCAN");
	updateChannnelSCAN(LLRFRecords::CH4_PHASE_DER, LLRFRecords::CH4, args);
}
void EPICSLLRFInterface::update_CH4_PWR_LOC_SCAN(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH4_PWR_LOC_SCAN");
	updateChannnelSCAN(LLRFRecords::CH4_PWR_LOC, LLRFRecords::CH4, args);
}
void EPICSLLRFInterface::update_CH5_PWR_REM_SCAN(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH5_PWR_REM_SCAN");
	updateChannnelSCAN(LLRFRecords::CH5_PWR_REM, LLRFRecords::CH5, args);
}
void EPICSLLRFInterface::update_CH5_PHASE_REM_SCAN(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH5_PHASE_REM_SCAN");
	updateChannnelSCAN(LLRFRecords::CH5_PHASE_REM, LLRFRecords::CH5, args);
}
void EPICSLLRFInterface::update_CH5_AMP_DER_SCAN(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH5_AMP_DER_SCAN");
	updateChannnelSCAN(LLRFRecords::CH5_AMP_DER, LLRFRecords::CH5, args);
}
void EPICSLLRFInterface::update_CH5_PHASE_DER_SCAN(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH5_PHASE_DER_SCAN");
	updateChannnelSCAN(LLRFRecords::CH5_PHASE_DER, LLRFRecords::CH5, args);
}
void EPICSLLRFInterface::update_CH5_PWR_LOC_SCAN(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH5_PWR_LOC_SCAN");
	updateChannnelSCAN(LLRFRecords::CH5_PWR_LOC, LLRFRecords::CH5, args);
}
void EPICSLLRFInterface::update_CH6_PWR_REM_SCAN(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH6_PWR_REM_SCAN");
	updateChannnelSCAN(LLRFRecords::CH6_PWR_REM, LLRFRecords::CH6, args);
}
void EPICSLLRFInterface::update_CH6_PHASE_REM_SCAN(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH6_PHASE_REM_SCAN");
	updateChannnelSCAN(LLRFRecords::CH6_PHASE_REM, LLRFRecords::CH6, args);
}
void EPICSLLRFInterface::update_CH6_AMP_DER_SCAN(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH6_AMP_DER_SCAN");
	updateChannnelSCAN(LLRFRecords::CH6_AMP_DER, LLRFRecords::CH6, args);
}
void EPICSLLRFInterface::update_CH6_PHASE_DER_SCAN(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH6_PHASE_DER_SCAN");
	updateChannnelSCAN(LLRFRecords::CH6_PHASE_DER, LLRFRecords::CH6, args);
}
void EPICSLLRFInterface::update_CH6_PWR_LOC_SCAN(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH6_PWR_LOC_SCAN");
	updateChannnelSCAN(LLRFRecords::CH6_PWR_LOC, LLRFRecords::CH6, args);
}
void EPICSLLRFInterface::update_CH7_PWR_REM_SCAN(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH7_PWR_REM_SCAN");
	updateChannnelSCAN(LLRFRecords::CH7_PWR_REM, LLRFRecords::CH7, args);
}
void EPICSLLRFInterface::update_CH7_PHASE_REM_SCAN(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH7_PHASE_REM_SCAN");
	updateChannnelSCAN(LLRFRecords::CH7_PHASE_REM, LLRFRecords::CH7, args);
}
void EPICSLLRFInterface::update_CH7_AMP_DER_SCAN(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH7_AMP_DER_SCAN");
	updateChannnelSCAN(LLRFRecords::CH7_AMP_DER, LLRFRecords::CH7, args);
}
void EPICSLLRFInterface::update_CH7_PHASE_DER_SCAN(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH7_PHASE_DER_SCAN");
	updateChannnelSCAN(LLRFRecords::CH7_PHASE_DER, LLRFRecords::CH7, args);
}
void EPICSLLRFInterface::update_CH7_PWR_LOC_SCAN(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH7_PWR_LOC_SCAN");
	updateChannnelSCAN(LLRFRecords::CH7_PWR_LOC, LLRFRecords::CH7, args);
}
void EPICSLLRFInterface::update_CH8_PWR_REM_SCAN(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH8_PWR_REM_SCAN");
	updateChannnelSCAN(LLRFRecords::CH8_PWR_REM, LLRFRecords::CH8, args);
}
void EPICSLLRFInterface::update_CH8_PHASE_REM_SCAN(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH8_PHASE_REM_SCAN");
	updateChannnelSCAN(LLRFRecords::CH8_PHASE_REM, LLRFRecords::CH8, args);
}
void EPICSLLRFInterface::update_CH8_AMP_DER_SCAN(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH8_AMP_DER_SCAN");
	updateChannnelSCAN(LLRFRecords::CH8_AMP_DER, LLRFRecords::CH8, args);
}
void EPICSLLRFInterface::update_CH8_PHASE_DER_SCAN(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH8_PHASE_DER_SCAN");
	updateChannnelSCAN(LLRFRecords::CH8_PHASE_DER, LLRFRecords::CH8, args);
}
void EPICSLLRFInterface::update_CH8_PWR_LOC_SCAN(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH8_PWR_LOC_SCAN");
	updateChannnelSCAN(LLRFRecords::CH8_PWR_LOC, LLRFRecords::CH8, args);
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
	default:acqm.second = STATE::UNKNOWN_ACQM;
	}
}
void EPICSLLRFInterface::updateChannnelACQM(const std::string& ch, const std::string& CH, const struct event_handler_args& args)
{
	std::lock_guard<std::mutex> lg(llrf_mtx_1);  // This now locked your mutex mtx.lock();
	messenger.printDebugMessage("updateInterLockStatus");
	// TODO maybe all the sorting out the names could be put into 1 function to make it a bit clearner, 
	// and improved in other ways 
	// but ... THIS works, so only change when eveyrthing else is working 
	LLRF* recastLLRF = static_cast<LLRF*>(args.usr);
	update_trace_ACQM(args, recastLLRF->all_trace_acqm.at(ch));
	messenger.printDebugMessage("updateChannnelACQM FOR: " + recastLLRF->getHardwareName(),
		" ", ENUM_TO_STRING(recastLLRF->all_trace_acqm.at(ch).second));

	// NOW WE FIND OUT I FTHE PASSED SCAN IN IN ALL TRACE DATA 
	std::string trace;
	if (recastLLRF->getTraceFromChannelData(CH, trace))
	{
		messenger.printDebugMessage(CH, " has a useful name of ", trace);
		if (GlobalFunctions::stringIsSubString(ch, "PWR_REM"))
		{
			messenger.printDebugMessage(ch, " is a power remote trace for ", trace);
			std::string powerTraceName = trace+"_POWER";
			if (GlobalFunctions::entryExists(recastLLRF->trace_data_map, powerTraceName))
			{
				
				recastLLRF->trace_data_map.at(powerTraceName).scan = recastLLRF->all_trace_scan.at(ch);
			}
			else
			{
				messenger.printMessage(trace.append(powerTraceName), " does not exist in trace_data_map.");
			}

		}
		else if (GlobalFunctions::stringIsSubString(ch, "PHASE_REM"))
		{
			messenger.printDebugMessage(ch, " is a phase remote trace for ", trace);
			std::string phaseTraceName = trace+"_PHASE";
			if (GlobalFunctions::entryExists(recastLLRF->trace_data_map, phaseTraceName))
			{
				recastLLRF->trace_data_map.at(phaseTraceName).scan = recastLLRF->all_trace_scan.at(ch);
			}
			else
			{
				messenger.printMessage(phaseTraceName, " does not exist in trace_data_map.");
			}
		}
		if (GlobalFunctions::entryExists(recastLLRF->all_trace_acqm, trace))
		{
			if (GlobalFunctions::entryExists(recastLLRF->trace_data_map, trace))
			{
				recastLLRF->trace_data_map.at(trace).scan = recastLLRF->all_trace_acqm.at(ch);
				messenger.printDebugMessage(trace, " all_trace_acqm = ", recastLLRF->trace_data_map.at(trace).acqm.second);
			}
			else
			{
				messenger.printMessage(trace.append("_PHASE"), " does not exist in trace_data_map.");
			}
		}
		else
		{
			messenger.printDebugMessage(trace, " NOT in trace_data_map");
		}

	}
	else
	{
		messenger.printDebugMessage(CH, " getTraceFromChannelData returned false ");
	}
}

void EPICSLLRFInterface::update_CH1_PHASE_REM_ACQM(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH1_PHASE_REM_ACQM");
	updateChannnelACQM(LLRFRecords::CH1_PHASE_REM, LLRFRecords::CH1, args);
}
void EPICSLLRFInterface::update_CH1_PWR_REM_ACQM(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH1_PWR_REM_ACQM");
	updateChannnelACQM(LLRFRecords::CH1_PWR_REM, LLRFRecords::CH1, args);
}
void EPICSLLRFInterface::update_CH1_AMP_DER_ACQM(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH1_AMP_DER_ACQM");
	updateChannnelACQM(LLRFRecords::CH1_AMP_DER, LLRFRecords::CH1, args);
}
void EPICSLLRFInterface::update_CH1_PHASE_DER_ACQM(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH1_PHASE_DER_ACQM");
	updateChannnelACQM(LLRFRecords::CH1_PHASE_DER, LLRFRecords::CH1, args);
}
void EPICSLLRFInterface::update_CH1_PWR_LOC_ACQM(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH1_PWR_LOC_ACQM");
	updateChannnelACQM(LLRFRecords::CH1_PWR_LOC, LLRFRecords::CH1, args);
}
void EPICSLLRFInterface::update_CH2_PWR_REM_ACQM(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH2_PWR_REM_ACQM");
	updateChannnelACQM(LLRFRecords::CH2_PWR_REM, LLRFRecords::CH2, args);
}
void EPICSLLRFInterface::update_CH2_PHASE_REM_ACQM(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH2_PHASE_REM_ACQM");
	updateChannnelACQM(LLRFRecords::CH2_PHASE_REM, LLRFRecords::CH2, args);
}
void EPICSLLRFInterface::update_CH2_AMP_DER_ACQM(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH2_AMP_DER_ACQM");
	updateChannnelACQM(LLRFRecords::CH2_AMP_DER, LLRFRecords::CH2, args);
}
void EPICSLLRFInterface::update_CH2_PHASE_DER_ACQM(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH2_PHASE_DER_ACQM");
	updateChannnelACQM(LLRFRecords::CH2_PHASE_DER, LLRFRecords::CH2, args);
}
void EPICSLLRFInterface::update_CH2_PWR_LOC_ACQM(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH2_PWR_LOC_ACQM");
	updateChannnelACQM(LLRFRecords::CH2_PWR_LOC, LLRFRecords::CH2, args);
}
void EPICSLLRFInterface::update_CH3_PWR_REM_ACQM(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH3_PWR_REM_ACQM");
	updateChannnelACQM(LLRFRecords::CH3_PWR_REM, LLRFRecords::CH3, args);
}
void EPICSLLRFInterface::update_CH3_PHASE_REM_ACQM(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH3_PHASE_REM_ACQM");
	updateChannnelACQM(LLRFRecords::CH3_PHASE_REM, LLRFRecords::CH3, args);
}
void EPICSLLRFInterface::update_CH3_AMP_DER_ACQM(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH3_AMP_DER_ACQM");
	updateChannnelACQM(LLRFRecords::CH3_AMP_DER, LLRFRecords::CH3, args);
}
void EPICSLLRFInterface::update_CH3_PHASE_DER_ACQM(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH3_PHASE_DER_ACQM");
	updateChannnelACQM(LLRFRecords::CH3_PHASE_DER, LLRFRecords::CH3, args);
}
void EPICSLLRFInterface::update_CH3_PWR_LOC_ACQM(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH3_PWR_LOC_ACQM");
	updateChannnelACQM(LLRFRecords::CH3_PWR_LOC, LLRFRecords::CH3, args);
}
void EPICSLLRFInterface::update_CH4_PWR_REM_ACQM(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH4_PWR_REM_ACQM");
	updateChannnelACQM(LLRFRecords::CH4_PWR_REM, LLRFRecords::CH4, args);
}
void EPICSLLRFInterface::update_CH4_PHASE_REM_ACQM(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH4_PHASE_REM_ACQM");
	updateChannnelACQM(LLRFRecords::CH4_PHASE_REM, LLRFRecords::CH4, args);
}
void EPICSLLRFInterface::update_CH4_AMP_DER_ACQM(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH4_AMP_DER_ACQM");
	updateChannnelACQM(LLRFRecords::CH4_AMP_DER, LLRFRecords::CH4, args);
}
void EPICSLLRFInterface::update_CH4_PHASE_DER_ACQM(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH4_PHASE_DER_ACQM");
	updateChannnelACQM(LLRFRecords::CH4_PHASE_DER, LLRFRecords::CH4, args);
}
void EPICSLLRFInterface::update_CH4_PWR_LOC_ACQM(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH4_PWR_LOC_ACQM");
	updateChannnelACQM(LLRFRecords::CH4_PWR_LOC, LLRFRecords::CH4, args);
}
void EPICSLLRFInterface::update_CH5_PWR_REM_ACQM(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH5_PWR_REM_ACQM");
	updateChannnelACQM(LLRFRecords::CH5_PWR_REM, LLRFRecords::CH5, args);
}
void EPICSLLRFInterface::update_CH5_PHASE_REM_ACQM(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH5_PHASE_REM_ACQM");
	updateChannnelACQM(LLRFRecords::CH5_PHASE_REM, LLRFRecords::CH5, args);
}
void EPICSLLRFInterface::update_CH5_AMP_DER_ACQM(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH5_AMP_DER_ACQM");
	updateChannnelACQM(LLRFRecords::CH5_AMP_DER, LLRFRecords::CH5, args);
}
void EPICSLLRFInterface::update_CH5_PHASE_DER_ACQM(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH5_PHASE_DER_ACQM");
	updateChannnelACQM(LLRFRecords::CH5_PHASE_DER, LLRFRecords::CH5, args);
}
void EPICSLLRFInterface::update_CH5_PWR_LOC_ACQM(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH5_PWR_LOC_ACQM");
	updateChannnelACQM(LLRFRecords::CH5_PWR_LOC, LLRFRecords::CH5, args);
}
void EPICSLLRFInterface::update_CH6_PWR_REM_ACQM(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH6_PWR_REM_ACQM");
	updateChannnelACQM(LLRFRecords::CH6_PWR_REM, LLRFRecords::CH5, args);
}
void EPICSLLRFInterface::update_CH6_PHASE_REM_ACQM(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH6_PHASE_REM_ACQM");
	updateChannnelACQM(LLRFRecords::CH6_PHASE_REM, LLRFRecords::CH5, args);
}
void EPICSLLRFInterface::update_CH6_AMP_DER_ACQM(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH6_AMP_DER_ACQM");
	updateChannnelACQM(LLRFRecords::CH6_AMP_DER, LLRFRecords::CH6, args);
}
void EPICSLLRFInterface::update_CH6_PHASE_DER_ACQM(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH6_PHASE_DER_ACQM");
	updateChannnelACQM(LLRFRecords::CH6_PHASE_DER, LLRFRecords::CH6, args);
}
void EPICSLLRFInterface::update_CH6_PWR_LOC_ACQM(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH6_PWR_LOC_ACQM");
	updateChannnelACQM(LLRFRecords::CH6_PWR_LOC, LLRFRecords::CH6, args);
}
void EPICSLLRFInterface::update_CH7_PWR_REM_ACQM(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH7_PWR_REM_ACQM");
	updateChannnelACQM(LLRFRecords::CH7_PWR_REM, LLRFRecords::CH7, args);
}
void EPICSLLRFInterface::update_CH7_PHASE_REM_ACQM(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH7_PHASE_REM_ACQM");
	updateChannnelACQM(LLRFRecords::CH7_PHASE_REM, LLRFRecords::CH7, args);
}
void EPICSLLRFInterface::update_CH7_AMP_DER_ACQM(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH7_AMP_DER_ACQM");
	updateChannnelACQM(LLRFRecords::CH7_AMP_DER, LLRFRecords::CH7, args);
}
void EPICSLLRFInterface::update_CH7_PHASE_DER_ACQM(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH7_PHASE_DER_ACQM");
	updateChannnelACQM(LLRFRecords::CH7_PHASE_DER, LLRFRecords::CH7, args);
}
void EPICSLLRFInterface::update_CH7_PWR_LOC_ACQM(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH7_PWR_LOC_ACQM");
	updateChannnelACQM(LLRFRecords::CH7_PWR_LOC, LLRFRecords::CH7, args);
}
void EPICSLLRFInterface::update_CH8_PWR_REM_ACQM(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH8_PWR_REM_ACQM");
	updateChannnelACQM(LLRFRecords::CH8_PWR_REM, LLRFRecords::CH8, args);
}
void EPICSLLRFInterface::update_CH8_PHASE_REM_ACQM(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH8_PHASE_REM_ACQM");
	updateChannnelACQM(LLRFRecords::CH8_PHASE_REM, LLRFRecords::CH8, args);
}
void EPICSLLRFInterface::update_CH8_AMP_DER_ACQM(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH8_AMP_DER_ACQM");
	updateChannnelACQM(LLRFRecords::CH8_AMP_DER, LLRFRecords::CH8, args);
}
void EPICSLLRFInterface::update_CH8_PHASE_DER_ACQM(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH8_PHASE_DER_ACQM");
	updateChannnelACQM(LLRFRecords::CH8_PHASE_DER, LLRFRecords::CH8, args);
}
void EPICSLLRFInterface::update_CH8_PWR_LOC_ACQM(const struct event_handler_args args)
{
	messenger.printDebugMessage("update_CH8_PWR_LOC_ACQM");
	updateChannnelACQM(LLRFRecords::CH8_PWR_LOC, LLRFRecords::CH8, args);
}






