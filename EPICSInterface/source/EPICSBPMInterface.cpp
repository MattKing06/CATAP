#include "EPICSBPMInterface.h"

LoggingSystem EPICSBPMInterface::messenger;

EPICSBPMInterface::EPICSBPMInterface() : EPICSInterface()
{
//	this->messenger = LoggingSystem(false, false);
}
EPICSBPMInterface::~EPICSBPMInterface()
{
	LoggingSystem::printDebugMessage("EPICSBPMInterface Destructor Called");
}
void EPICSBPMInterface::retrieveUpdateFunctionForRecord(pvStruct& pvStruct) const
{

	if (pvStruct.pvRecord == "X")
	{
		pvStruct.updateFunction = this->updateXPV;
	}
	if (pvStruct.pvRecord == "Y")
	{
		pvStruct.updateFunction = this->updateYPV;
	}
	if (pvStruct.pvRecord == "SA1")
	{
		pvStruct.updateFunction = this->updateSA1;
	}
	if (pvStruct.pvRecord == "SA2")
	{
		pvStruct.updateFunction = this->updateSA2;
	}
	if (pvStruct.pvRecord == "SD1")
	{
		pvStruct.updateFunction = this->updateSD1;
	}
	if (pvStruct.pvRecord == "SD2")
	{
		pvStruct.updateFunction = this->updateSD2;
	}
	if (pvStruct.pvRecord == "RA1")
	{
		pvStruct.updateFunction = this->updateRA1;
	}
	if (pvStruct.pvRecord == "RA2")
	{
		pvStruct.updateFunction = this->updateRA2;
	}
	if (pvStruct.pvRecord == "RD1")
	{
		pvStruct.updateFunction = this->updateRD1;
	}
	if (pvStruct.pvRecord == "RD2")
	{
		pvStruct.updateFunction = this->updateRD2;
	}
	if (pvStruct.pvRecord == "DATA:B2V.VALA")
	{
		pvStruct.updateFunction = this->updateData;
	}
	if (pvStruct.pvRecord == "AWAK")
	{
		pvStruct.updateFunction = this->updateAWAK;
	}
	if (pvStruct.pvRecord == "RDY")
	{
		pvStruct.updateFunction = this->updateRDY;
	}
}

void EPICSBPMInterface::updateXPV(const struct event_handler_args args)
{
	BPM* recastBPM = getHardwareFromArgs<BPM>(args);
	setPVTimeStampFromArgs(recastBPM->pvStructs.at("X"), args);
	double value = returnValueFromArgsAsDouble(args);
	recastBPM->setXPV(value);
	LoggingSystem::printDebugMessage("X PV VALUE FOR: " + recastBPM->getHardwareName() + ": "
								+ std::to_string(value));
	LoggingSystem::printDebugMessage(" CALLED UPDATE X ");
}

void EPICSBPMInterface::updateYPV(const struct event_handler_args args)
{
	BPM* recastBPM = getHardwareFromArgs<BPM>(args);
	setPVTimeStampFromArgs(recastBPM->pvStructs.at("Y"), args);
	double value = returnValueFromArgsAsDouble(args);
	recastBPM->setYPV(value);
	LoggingSystem::printDebugMessage("Y PV VALUE FOR: " + recastBPM->getHardwareName() + ": "
								+ std::to_string(value));
	LoggingSystem::printDebugMessage(" CALLED UPDATE Y ");
}

void EPICSBPMInterface::updateData(const struct event_handler_args args)
{
	BPM* recastBPM = getHardwareFromArgs<BPM>(args);
	setPVTimeStampFromArgs(recastBPM->pvStructs.at("DATA"), args);
	setPVTimeStampFromArgs(recastBPM->pvStructs.at("Q"), args);
	std::vector< double > value = returnValueFromArgsAsDoubleVector(args);
	recastBPM->setData(value);
	recastBPM->setQ(value);
	LoggingSystem::printDebugMessage("DATA PV VALUE FOR: " + recastBPM->getHardwareName());
	LoggingSystem::printDebugMessage(" CALLED UPDATE DATA ");
}

void EPICSBPMInterface::updateRA1(const struct event_handler_args args)
{
	BPM* recastBPM = getHardwareFromArgs<BPM>(args);
	setPVTimeStampFromArgs(recastBPM->pvStructs.at("RA1"), args);
	long value = returnValueFromArgsAsLong(args);
	recastBPM->setRA1(value);
	LoggingSystem::printDebugMessage(" CALLED UPDATE RA1 ");
}

void EPICSBPMInterface::updateRA2(const struct event_handler_args args)
{
	BPM* recastBPM = getHardwareFromArgs<BPM>(args);
	setPVTimeStampFromArgs(recastBPM->pvStructs.at("RA2"), args);
	long value = returnValueFromArgsAsLong(args);
	recastBPM->setRA2(value);
	LoggingSystem::printDebugMessage(" CALLED UPDATE RA2 ");
}

void EPICSBPMInterface::updateRD1(const struct event_handler_args args)
{
	BPM* recastBPM = getHardwareFromArgs<BPM>(args);
	setPVTimeStampFromArgs(recastBPM->pvStructs.at("RD1"), args);
	long value = returnValueFromArgsAsLong(args);
	recastBPM->setRD1(value);
	LoggingSystem::printDebugMessage(" CALLED UPDATE RD1 ");
}

void EPICSBPMInterface::updateRD2(const struct event_handler_args args)
{
	BPM* recastBPM = getHardwareFromArgs<BPM>(args);
	setPVTimeStampFromArgs(recastBPM->pvStructs.at("RD2"), args);
	long value = returnValueFromArgsAsLong(args);
	recastBPM->setRD2(value);
	LoggingSystem::printDebugMessage(" CALLED UPDATE RD2 ");
}

void EPICSBPMInterface::updateSA1(const struct event_handler_args args)
{
	BPM* recastBPM = getHardwareFromArgs<BPM>(args);
	setPVTimeStampFromArgs(recastBPM->pvStructs.at("SA1"), args);
	long value = returnValueFromArgsAsLong(args);
	recastBPM->setSA1(value);
	LoggingSystem::printDebugMessage(" CALLED UPDATE SA1 ");
}

void EPICSBPMInterface::updateSA2(const struct event_handler_args args)
{
	BPM* recastBPM = getHardwareFromArgs<BPM>(args);
	setPVTimeStampFromArgs(recastBPM->pvStructs.at("SA2"), args);
	long value = returnValueFromArgsAsLong(args);
	recastBPM->setSA2(value);
	LoggingSystem::printDebugMessage(" CALLED UPDATE SA2 ");
}

void EPICSBPMInterface::updateSD1(const struct event_handler_args args)
{
	BPM* recastBPM = getHardwareFromArgs<BPM>(args);
	setPVTimeStampFromArgs(recastBPM->pvStructs.at("SD1"), args);
	long value = returnValueFromArgsAsLong(args);
	recastBPM->setSD1(value);
	LoggingSystem::printDebugMessage(" CALLED UPDATE SD1 ");
}

void EPICSBPMInterface::updateSD2(const struct event_handler_args args)
{
	BPM* recastBPM = getHardwareFromArgs<BPM>(args);
	setPVTimeStampFromArgs(recastBPM->pvStructs.at("SD2"), args);
	long value = returnValueFromArgsAsLong(args);
	recastBPM->setSD2(value);
	LoggingSystem::printDebugMessage(" CALLED UPDATE SD2 ");
}

void EPICSBPMInterface::updateAWAK(const struct event_handler_args args)
{

	BPM* recastBPM = getHardwareFromArgs<BPM>(args);
	setPVTimeStampFromArgs(recastBPM->pvStructs.at("AWAK"), args);
	long value = returnValueFromArgsAsLong(args);
	recastBPM->setAWAK(value);
	recastBPM->setAWAKTStamp(std::stod(getEPICSTime(recastBPM->pvStructs.at("AWAK").time)));
	LoggingSystem::printDebugMessage(" CALLED UPDATE AWAK ");
}

void EPICSBPMInterface::updateRDY(const struct event_handler_args args)
{
	BPM* recastBPM = getHardwareFromArgs<BPM>(args);
	setPVTimeStampFromArgs(recastBPM->pvStructs.at("RDY"), args);
	long value = returnValueFromArgsAsLong(args);
	recastBPM->setRDY(value);
	recastBPM->setRDYTStamp(std::stod(getEPICSTime(recastBPM->pvStructs.at("RDY").time)));
	LoggingSystem::printDebugMessage(" CALLED UPDATE RDY ");
}

void EPICSBPMInterface::setSA1(const long& value, const pvStruct& pv)
{
	putValue(pv, value);
}

void EPICSBPMInterface::setSA2(const long& value, const pvStruct& pv)
{
	putValue(pv, value);
}

void EPICSBPMInterface::setSD1(const long& value, const pvStruct& pv)
{
	putValue(pv, value);
}

void EPICSBPMInterface::setSD2(const long& value, const pvStruct& pv)
{
	putValue(pv, value);
}