#include "EPICSBPMInterface.h"

LoggingSystem EPICSBPMInterface::messenger;

EPICSBPMInterface::EPICSBPMInterface() : EPICSInterface()
{
//	this->messenger = LoggingSystem(false, false);
}
EPICSBPMInterface::~EPICSBPMInterface()
{
	messenger.printDebugMessage("EPICSBPMInterface Destructor Called");
}
void EPICSBPMInterface::retrieveupdateFunctionForRecord(pvStruct& pvStruct) const
{
	if (pvStruct.pvRecord == BPMRecords::X)
	{
		pvStruct.updateFunction = this->updateXPV;
	}
	else if (pvStruct.pvRecord == BPMRecords::Y)
	{
		pvStruct.updateFunction = this->updateYPV;
	}
	else if (pvStruct.pvRecord == BPMRecords::SA1)
	{
		pvStruct.updateFunction = this->updateSA1;
	}
	else if (pvStruct.pvRecord == BPMRecords::SA2)
	{
		pvStruct.updateFunction = this->updateSA2;
	}
	else if (pvStruct.pvRecord == BPMRecords::SD1)
	{
		pvStruct.updateFunction = this->updateSD1;
	}
	else if (pvStruct.pvRecord == BPMRecords::SD2)
	{
		pvStruct.updateFunction = this->updateSD2;
	}
	else if (pvStruct.pvRecord == BPMRecords::RA1)
	{
		pvStruct.updateFunction = this->updateRA1;
	}
	else if (pvStruct.pvRecord == BPMRecords::RA2)
	{
		pvStruct.updateFunction = this->updateRA2;
	}
	else if (pvStruct.pvRecord == BPMRecords::RD1)
	{
		pvStruct.updateFunction = this->updateRD1;
	}
	else if (pvStruct.pvRecord == BPMRecords::RD2)
	{
		pvStruct.updateFunction = this->updateRD2;
	}
	else if (pvStruct.pvRecord == BPMRecords::DATA)
	{
		pvStruct.updateFunction = this->updateData;
	}
	else if (pvStruct.pvRecord == BPMRecords::AWAK)
	{
		pvStruct.updateFunction = this->updateAWAK;
	}
	else if (pvStruct.pvRecord == BPMRecords::RDY)
	{
		pvStruct.updateFunction = this->updateRDY;
	}
	else
	{
		messenger.printDebugMessage("!!WARNING!! NO UPDATE FUNCTION FOUND FOR: " + pvStruct.pvRecord);
	}
}

void EPICSBPMInterface::updateXPV(const struct event_handler_args args)
{
	BPM* recastBPM = getHardwareFromArgs<BPM>(args);
	updateTimeStampDoublePair(args, recastBPM->xPV);
	messenger.printDebugMessage("XPV VALUE FOR: " + recastBPM->getHardwareName() + ": "
		+ std::to_string(recastBPM->xPV.second));
}

void EPICSBPMInterface::updateYPV(const struct event_handler_args args)
{
	BPM* recastBPM = getHardwareFromArgs<BPM>(args);
	updateTimeStampDoublePair(args, recastBPM->yPV);
	messenger.printDebugMessage("YPV VALUE FOR: " + recastBPM->getHardwareName() + ": "
		+ std::to_string(recastBPM->yPV.second));
}

void EPICSBPMInterface::updateData(const struct event_handler_args args)
{
	BPM* recastBPM = getHardwareFromArgs<BPM>(args);
	const struct dbr_time_double* tv = (const struct dbr_time_double*)(args.dbr);
	recastBPM->data.first = tv->stamp;
	recastBPM->setData(recastBPM->data.second);
	messenger.printDebugMessage("DATA PV VALUE FOR: " + recastBPM->getHardwareName());
	messenger.printDebugMessage(" CALLED UPDATE DATA ");

}

void EPICSBPMInterface::updateRA1(const struct event_handler_args args)
{
	BPM* recastBPM = getHardwareFromArgs<BPM>(args);
	updateTimeStampLongPair(args, recastBPM->ra1);
	messenger.printDebugMessage("RA1 VALUE FOR: " + recastBPM->getHardwareName() + ": "
		+ std::to_string(recastBPM->ra1.second));
}

void EPICSBPMInterface::updateRA2(const struct event_handler_args args)
{
	BPM* recastBPM = getHardwareFromArgs<BPM>(args);
	updateTimeStampLongPair(args, recastBPM->ra2);
	messenger.printDebugMessage("RA2 VALUE FOR: " + recastBPM->getHardwareName() + ": "
		+ std::to_string(recastBPM->ra2.second));
}

void EPICSBPMInterface::updateRD1(const struct event_handler_args args)
{
	BPM* recastBPM = getHardwareFromArgs<BPM>(args);
	updateTimeStampLongPair(args, recastBPM->rd1);
	messenger.printDebugMessage("RD1 VALUE FOR: " + recastBPM->getHardwareName() + ": "
		+ std::to_string(recastBPM->rd1.second));
}

void EPICSBPMInterface::updateRD2(const struct event_handler_args args)
{
	BPM* recastBPM = getHardwareFromArgs<BPM>(args);
	updateTimeStampLongPair(args, recastBPM->rd2);
	messenger.printDebugMessage("RD2 VALUE FOR: " + recastBPM->getHardwareName() + ": "
		+ std::to_string(recastBPM->rd1.second));
}

void EPICSBPMInterface::updateSA1(const struct event_handler_args args)
{
	BPM* recastBPM = getHardwareFromArgs<BPM>(args);
	updateTimeStampLongPair(args, recastBPM->sa1);
	messenger.printDebugMessage("SA1 VALUE FOR: " + recastBPM->getHardwareName() + ": "
		+ std::to_string(recastBPM->sa1.second));
}

void EPICSBPMInterface::updateSA2(const struct event_handler_args args)
{
	BPM* recastBPM = getHardwareFromArgs<BPM>(args);
	updateTimeStampLongPair(args, recastBPM->sa2);
	messenger.printDebugMessage("SA2 VALUE FOR: " + recastBPM->getHardwareName() + ": "
		+ std::to_string(recastBPM->sa2.second));
}

void EPICSBPMInterface::updateSD1(const struct event_handler_args args)
{
	BPM* recastBPM = getHardwareFromArgs<BPM>(args);
	updateTimeStampLongPair(args, recastBPM->sd1);
	messenger.printDebugMessage("SD1 VALUE FOR: " + recastBPM->getHardwareName() + ": "
		+ std::to_string(recastBPM->sd1.second));
}

void EPICSBPMInterface::updateSD2(const struct event_handler_args args)
{
	BPM* recastBPM = getHardwareFromArgs<BPM>(args);
	updateTimeStampLongPair(args, recastBPM->sd2);
	messenger.printDebugMessage("SD2 VALUE FOR: " + recastBPM->getHardwareName() + ": "
		+ std::to_string(recastBPM->sd2.second));
}

void EPICSBPMInterface::updateAWAK(const struct event_handler_args args)
{

	BPM* recastBPM = getHardwareFromArgs<BPM>(args);
	updateTimeStampLongPair(args, recastBPM->awak);
	messenger.printDebugMessage("AWAK VALUE FOR: " + recastBPM->getHardwareName() + ": "
		+ std::to_string(recastBPM->awak.second));
}

void EPICSBPMInterface::updateRDY(const struct event_handler_args args)
{
	BPM* recastBPM = getHardwareFromArgs<BPM>(args);
	updateTimeStampLongPair(args, recastBPM->rdy);
	messenger.printDebugMessage("RDY VALUE FOR: " + recastBPM->getHardwareName() + ": "
		+ std::to_string(recastBPM->rdy.second));
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