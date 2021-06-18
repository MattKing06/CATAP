#include <EPICSLinacPIDInterface.h>

LoggingSystem EPICSLinacPIDInterface::static_messenger;

EPICSLinacPIDInterface::EPICSLinacPIDInterface() : EPICSInterface()
{
	static_messenger = LoggingSystem(false, false);
}

EPICSLinacPIDInterface::~EPICSLinacPIDInterface()
{
	static_messenger.printDebugMessage("EPICSLinacPIDInterface Destructor Called");
}

void EPICSLinacPIDInterface::retrieveUpdateFunctionForRecord(pvStruct& pvStruct) const
{
	using namespace LinacPIDRecords;
	// TODO could be a const map and then a lookup ... might be neater?? 
	if (pvStruct.pvRecord == AVG_PHASE)
	{
		pvStruct.updateFunction = this->update_AVG_PHASE;
	}
	if (pvStruct.pvRecord == PROBE_PHASE_WEIGHT)
	{
		pvStruct.updateFunction = this->update_PROBE_PHASE_WEIGHT;
	}
	if (pvStruct.pvRecord == FORWARD_PHASE_WRAPPED)
	{
		pvStruct.updateFunction = this->update_FORWARD_PHASE_WRAPPED;
	}
	if (pvStruct.pvRecord == SCAN)
	{
		pvStruct.updateFunction = this->update_SCAN;
	}
	if (pvStruct.pvRecord == PID_OVAL)
	{
		pvStruct.updateFunction = this->update_PID_OVAL;
	}
	if (pvStruct.pvRecord == FORWARD_PHASE_WRAPPED)
	{
		pvStruct.updateFunction = this->update_FORWARD_PHASE_WRAPPED;
	}
	if (pvStruct.pvRecord == PROBE_PHASE_WEIGHT)
	{
		pvStruct.updateFunction = this->update_PROBE_PHASE_WEIGHT;
	}
}

void EPICSLinacPIDInterface::update_AVG_PHASE(const struct event_handler_args args)
{
	LinacPID* recastPID= static_cast<LinacPID*>(args.usr);
	updateTimeStampDoublePair(args, recastPID->phase);
	static_messenger.printDebugMessage(recastPID->hardwareName, " update_AVG_PHASE = ",
		recastPID->phase.second);
}
void EPICSLinacPIDInterface::update_FORWARD_PHASE_WEIGHT(const struct event_handler_args args)
{
	LinacPID* recastPID = static_cast<LinacPID*>(args.usr);
	updateTimeStampDoublePair(args, recastPID->forward_phase_weight);
	static_messenger.printDebugMessage(recastPID->hardwareName, " update_FORWARD_PHASE_WEIGHT = ",
		recastPID->phase.second);
}
void EPICSLinacPIDInterface::update_PROBE_PHASE_WEIGHT(const struct event_handler_args args)
{
	LinacPID* recastPID = static_cast<LinacPID*>(args.usr);
	updateTimeStampDoublePair(args, recastPID->probe_phase_weight);
	static_messenger.printDebugMessage(recastPID->hardwareName, " update_PROBE_PHASE_WEIGHT = ",
		recastPID->phase.second);
}
void EPICSLinacPIDInterface::update_SCAN(const struct event_handler_args args)
{
	LinacPID* recastPID = static_cast<LinacPID*>(args.usr);
	// SCAN Settings to be defined in the master lattice 
}
void EPICSLinacPIDInterface::update_PID_OVAL(const struct event_handler_args args)
{
	LinacPID* recastPID = static_cast<LinacPID*>(args.usr);
	updateTimeStampDoublePair(args, recastPID->oval);
	static_messenger.printDebugMessage(recastPID->hardwareName, " update_PID_OVAL = ",
		recastPID->phase.second);
}
void EPICSLinacPIDInterface::update_FORWARD_PHASE_WRAPPED(const struct event_handler_args args)
{
	LinacPID* recastPID = static_cast<LinacPID*>(args.usr);
	updateTimeStampDoublePair(args, recastPID->probe_phase_wrapped);
	static_messenger.printDebugMessage(recastPID->hardwareName, " update_FORWARD_PHASE_WRAPPED = ",
		recastPID->phase.second);
}
void EPICSLinacPIDInterface::update_PROBE_PHASE_WRAPPED(const struct event_handler_args args)
{
	LinacPID* recastPID = static_cast<LinacPID*>(args.usr);
	updateTimeStampDoublePair(args, recastPID->probe_phase_wrapped);
	static_messenger.printDebugMessage(recastPID->hardwareName, " update_PROBE_PHASE_WRAPPED = ",
		recastPID->phase.second);
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