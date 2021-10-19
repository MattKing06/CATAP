#include <EPICSLinacPIDInterface.h>
#include <GlobalFunctions.h>

LoggingSystem EPICSLinacPIDInterface::static_messenger;

EPICSLinacPIDInterface::EPICSLinacPIDInterface() : EPICSInterface()
{
	static_messenger = LoggingSystem(true, true);
}

EPICSLinacPIDInterface::~EPICSLinacPIDInterface()
{
	static_messenger.printDebugMessage("EPICSLinacPIDInterface Destructor Called");
}

void EPICSLinacPIDInterface::retrieveUpdateFunctionForRecord(pvStruct& pvStruct) const
{
	using namespace LinacPIDRecords;
	static_messenger.printDebugMessage("retrieveUpdateFunctionForRecord passed ", pvStruct.pvRecord);
	// TODO could be a const map and then a lookup ... might be neater?? 
	if (pvStruct.pvRecord == AVG_PHASE)
	{
		static_messenger.printDebugMessage("retrieveUpdateFunctionForRecord, updateFunction =  update_AVG_PHASE");
		pvStruct.updateFunction = this->update_AVG_PHASE;
	}
	if (pvStruct.pvRecord == PROBE_PHASE_WEIGHT)
	{
		static_messenger.printDebugMessage("retrieveUpdateFunctionForRecord, updateFunction =  update_PROBE_PHASE_WEIGHT");
		pvStruct.updateFunction = this->update_PROBE_PHASE_WEIGHT;
	}
	if (pvStruct.pvRecord == FORWARD_PHASE_WRAPPED)
	{
		static_messenger.printDebugMessage("retrieveUpdateFunctionForRecord, updateFunction =  update_FORWARD_PHASE_WRAPPED");
		pvStruct.updateFunction = this->update_FORWARD_PHASE_WRAPPED;
	}
	if (pvStruct.pvRecord == SCAN)
	{
		static_messenger.printDebugMessage("retrieveUpdateFunctionForRecord, updateFunction =  update_SCAN");
		pvStruct.updateFunction = this->update_SCAN;
	}
	if (pvStruct.pvRecord == PID_OVAL)
	{
		static_messenger.printDebugMessage("retrieveUpdateFunctionForRecord, updateFunction =  update_PID_OVAL");
		pvStruct.updateFunction = this->update_PID_OVAL;
	}
	if (pvStruct.pvRecord == FORWARD_PHASE_WRAPPED)
	{
		static_messenger.printDebugMessage("retrieveUpdateFunctionForRecord, updateFunction = update_FORWARD_PHASE_WRAPPED");
		pvStruct.updateFunction = this->update_FORWARD_PHASE_WRAPPED;
	}
	if (pvStruct.pvRecord == PROBE_PHASE_WEIGHT)
	{
		static_messenger.printDebugMessage("retrieveUpdateFunctionForRecord, updateFunction =  update_PROBE_PHASE_WEIGHT");
		pvStruct.updateFunction = this->update_PROBE_PHASE_WEIGHT;
	}
}

void EPICSLinacPIDInterface::update_AVG_PHASE(const struct event_handler_args args)
{
	LinacPID* recastPID= static_cast<LinacPID*>(args.usr);
	updateTimeStampDoublePair(args, recastPID->phase);
	//static_messenger.printDebugMessage(recastPID->hardwareName, " update_AVG_PHASE = ",recastPID->phase.second);
}
void EPICSLinacPIDInterface::update_FORWARD_PHASE_WEIGHT(const struct event_handler_args args)
{
	LinacPID* recastPID = static_cast<LinacPID*>(args.usr);
	updateTimeStampDoublePair(args, recastPID->forward_phase_weight);
	//static_messenger.printDebugMessage(recastPID->hardwareName, " update_FORWARD_PHASE_WEIGHT = ",recastPID->forward_phase_weight.second);
}
void EPICSLinacPIDInterface::update_PROBE_PHASE_WEIGHT(const struct event_handler_args args)
{
	LinacPID* recastPID = static_cast<LinacPID*>(args.usr);
	updateTimeStampDoublePair(args, recastPID->probe_phase_weight);
	//static_messenger.printDebugMessage(recastPID->hardwareName, " update_PROBE_PHASE_WEIGHT = ",recastPID->probe_phase_weight.second);
}
void EPICSLinacPIDInterface::update_SCAN(const struct event_handler_args args)
{
	LinacPID* recastPID = static_cast<LinacPID*>(args.usr);
	updateTimeStampUShortPair(args, recastPID->scan);
	// from this number update the scan_enum
	recastPID->scan_enum = GlobalFunctions::numberToLLRFSCAN(recastPID->scan.second);
	// udpate the "enable state" by comapring it to master lattice values 
	if (recastPID->scan.second == recastPID->ml_enable_number)
	{
		recastPID->enable_state = STATE::ENABLED;
	}
	else if(recastPID->scan.second == recastPID->ml_disable_number)
	{
		recastPID->enable_state = STATE::DISABLED;
	}
	else
	{
		recastPID->enable_state = STATE::UNKNOWN_SET_STATE;
	}
	// 
	std::cout << "recastPID->ml_enable_number = " << recastPID->ml_enable_number  << std::endl;
	static_messenger.printDebugMessage(recastPID->hardwareName, " update_SCAN (n, enum, enable_state) = ", 
		recastPID->scan.second, ", ", ENUM_TO_STRING(recastPID->scan_enum), ", ", ENUM_TO_STRING(recastPID->enable_state));

}
void EPICSLinacPIDInterface::update_PID_OVAL(const struct event_handler_args args)
{
	LinacPID* recastPID = static_cast<LinacPID*>(args.usr);
	updateTimeStampDoublePair(args, recastPID->oval);
	//static_messenger.printDebugMessage(recastPID->hardwareName, " update_PID_OVAL = ",recastPID->oval.second);
}
void EPICSLinacPIDInterface::update_FORWARD_PHASE_WRAPPED(const struct event_handler_args args)
{
	LinacPID* recastPID = static_cast<LinacPID*>(args.usr);
	updateTimeStampDoublePair(args, recastPID->forward_phase_wrapped);
	//static_messenger.printDebugMessage(recastPID->hardwareName, " update_FORWARD_PHASE_WRAPPED = ",recastPID->forward_phase_wrapped.second);
}
void EPICSLinacPIDInterface::update_PROBE_PHASE_WRAPPED(const struct event_handler_args args)
{
	LinacPID* recastPID = static_cast<LinacPID*>(args.usr);
	updateTimeStampDoublePair(args, recastPID->probe_phase_wrapped);
	//static_messenger.printDebugMessage(recastPID->hardwareName, " update_PROBE_PHASE_WRAPPED = ",recastPID->probe_phase_wrapped.second);
}

void EPICSLinacPIDInterface::debugMessagesOn()
{
	this->static_messenger.debugMessagesOn();
	this->static_messenger.printDebugMessage(ownerName, " EPICS Interface - DEBUG On");
}

void EPICSLinacPIDInterface::debugMessagesOff()
{
	this->static_messenger.printDebugMessage(ownerName, " EPICS Interface - DEBUG OFF");
	this->static_messenger.debugMessagesOff();
}

void EPICSLinacPIDInterface::messagesOn()
{
	this->static_messenger.messagesOn();
	this->static_messenger.printMessage(ownerName, " EPICS Interface - MESSAGES On");
}

void EPICSLinacPIDInterface::messagesOff()
{
	this->static_messenger.printMessage(ownerName, " EPICS Interface - MESSAGES OFF");
	this->static_messenger.messagesOff();
}

bool EPICSLinacPIDInterface::isMessagingOn()
{
	return this->static_messenger.isMessagingOn();
}

bool EPICSLinacPIDInterface::isDebugOn()
{
	return this->static_messenger.isDebugOn();
}