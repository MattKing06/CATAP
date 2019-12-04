#include "EPICSMagnetInterface.h"

LoggingSystem EPICSMagnetInterface::messenger;

EPICSMagnetInterface::EPICSMagnetInterface() : EPICSInterface()
{
	this->messenger = LoggingSystem(false, false);
}
EPICSMagnetInterface::~EPICSMagnetInterface()
{
	messenger.debugMessagesOff();
	messenger.printDebugMessage("EPICSMagnetInterface Destructor Called");
}
void EPICSMagnetInterface::retrieveUpdateFunctionForRecord(pvStruct &pvStruct) const
{
	
	if (pvStruct.pvRecord == "GETSETI")
	{
		pvStruct.updateFunction = this->updateCurrent;
	}
	if (pvStruct.pvRecord == "RPOWER")
	{
		pvStruct.updateFunction = this->updatePSUState;
	}
	if (pvStruct.pvRecord == "READI")
	{
		pvStruct.updateFunction = this->updateREADI;
	}
	if (pvStruct.pvRecord == "RILK")
	{
		pvStruct.updateFunction = this->updateRILK;
	}
}

void EPICSMagnetInterface::updateCurrent(const struct event_handler_args args)
{
	messenger.debugMessagesOff();
	if (args.status != ECA_NORMAL)
	{
		messenger.messagesOn();
		messenger.printMessage("Something went wrong with update function!");
		messenger.messagesOff();
	}
	else if (args.type == DBR_DOUBLE)
	{
		MY_SEVCHK(args.status);
		Magnet* recastMagnet = static_cast<Magnet*>(args.usr);
		recastMagnet->setCurrent(*(double*)(args.dbr));
		messenger.printDebugMessage("GETSETI VALUE FOR " + recastMagnet->getHardwareName() + ": " + std::to_string(*(double*)(args.dbr)));
	}
	else if (args.type == DBR_TIME_DOUBLE)
	{
		MY_SEVCHK(args.status);
		Magnet* recastMagnet = static_cast<Magnet*>(args.usr);
		const struct dbr_time_double* pTD = (const struct dbr_time_double*)(args.dbr);
		recastMagnet->pvStructs.at("GETSETI").time = pTD->stamp;
		//recastMagnet->setCurrent(pTD->value);
		const double value = pTD->value;
		setValueByFunction<bool, double>(std::bind(&Magnet::setCurrent, recastMagnet, std::placeholders::_1), value);
		messenger.printDebugMessageWithEPICSTimestampString(getEPICSTime(recastMagnet->pvStructs.at("GETSETI").time),
			"GETSETI VALUE FOR: " + recastMagnet->getHardwareName() + ": "
			+ std::to_string(pTD->value));
	}
	messenger.printDebugMessage(" CALLED UPDATE CURRENT ");


}

void EPICSMagnetInterface::updatePSUState(const struct event_handler_args args)
{
	messenger.debugMessagesOff();
	if (args.status != ECA_NORMAL)
	{
		std::cerr << "Something whent wrong with update function" << std::endl;
	}
	else if (args.type == DBR_ENUM)
	{
		MY_SEVCHK(args.status);
		Magnet* recastMagnet = static_cast<Magnet*>(args.usr);
		int value = *(int*)(args.dbr);
		recastMagnet->setPSUState((STATE)(value));
		messenger.printDebugMessage("RPOWER VALUE FOR: " + recastMagnet->getHardwareName() + ": " + std::to_string(*(int*)(args.dbr)));
	}
	else if (args.type == DBR_TIME_ENUM)
	{
		messenger.printDebugMessage(" DBR TIME ENUM ");
		MY_SEVCHK(args.status);
		Magnet* recastMagnet = static_cast<Magnet*>(args.usr);
		const struct dbr_time_enum* pTD = (const struct dbr_time_enum*)(args.dbr);
		recastMagnet->pvStructs.at("RPOWER").time = pTD->stamp;
		recastMagnet->setPSUState((STATE)pTD->value);
		messenger.printDebugMessageWithEPICSTimestampString(getEPICSTime(recastMagnet->pvStructs.at("RPOWER").time),
			"RPOWER VALUE FOR: " + recastMagnet->getHardwareName() + ": "
			+ std::to_string(pTD->value));
	}
}

void EPICSMagnetInterface::updateREADI(const struct event_handler_args args)
{
	messenger.debugMessagesOff();
	if (args.status != ECA_NORMAL)
	{
		std::cerr << "Something whent wrong with update function" << std::endl;
	}
	else if (args.type == DBR_TIME_DOUBLE)
	{
		messenger.printDebugMessage(" DBR TIME DOUBLE ");
		MY_SEVCHK(args.status);
		Magnet* recastMagnet = static_cast<Magnet*>(args.usr);
		const struct dbr_time_double* pTD = (const struct dbr_time_double*)(args.dbr);
		recastMagnet->pvStructs.at("READI").time = pTD->stamp;
		recastMagnet->setRICurrent(pTD->value);
		messenger.printDebugMessageWithEPICSTimestampString(getEPICSTime(recastMagnet->pvStructs.at("READI").time),
															"READI VALUE FOR: " + recastMagnet->getHardwareName() + ": "
															+ std::to_string(pTD->value));
	}
	else if (args.type == DBR_DOUBLE)
	{
		messenger.printDebugMessage(" DBR DOUBLE ");
		MY_SEVCHK(args.status);
		Magnet* recastMagnet = static_cast<Magnet*>(args.usr);
		recastMagnet->setRICurrent(*(double*)args.dbr);
		messenger.printDebugMessage("READI VALUE FOR: " + recastMagnet->getHardwareName() + ": " + std::to_string(*(double*)args.dbr));
	}
	messenger.printDebugMessage(" CALLED UPDATE READI ");
}

void EPICSMagnetInterface::updateRILK(const struct event_handler_args args)
{
	messenger.debugMessagesOff();
	if (args.status != ECA_NORMAL)
	{
		std::cerr << "Something whent wrong with update function" << std::endl;
	}
	else if (args.type == DBR_ENUM)
	{
		MY_SEVCHK(args.status);
		Magnet* recastMagnet = static_cast<Magnet*>(args.usr);
		recastMagnet->setILKState(*(int*)(args.dbr));
		messenger.printDebugMessage("RILK VALUE FOR: " + recastMagnet->getHardwareName() + ": " + std::to_string(*(int*)(args.dbr)));
	}
	else if (args.type == DBR_TIME_ENUM)
	{
		messenger.printDebugMessage(" DBR TIME ENUM ");
		MY_SEVCHK(args.status);
		Magnet* recastMagnet = static_cast<Magnet*>(args.usr);
		const struct dbr_time_enum* pTD = (const struct dbr_time_enum*)(args.dbr);
		recastMagnet->pvStructs.at("RILK").time = pTD->stamp;
		recastMagnet->setILKState(pTD->value);
		messenger.printDebugMessageWithEPICSTimestampString(getEPICSTime(recastMagnet->pvStructs.at("RILK").time),
			"RILK VALUE FOR: " + recastMagnet->getHardwareName() + ": "
			+ std::to_string(pTD->value));
	}
}

void EPICSMagnetInterface::setNewCurrent(const double &value, const pvStruct &pv) const
{
	//we have checked that pvRecord is SETI before reaching here.
	putValue(pv, value);
	messenger.printDebugMessage("SENT CURRENT " + std::to_string(value) + " FOR " + pv.fullPVName + " TO EPICS ");
}

void EPICSMagnetInterface::setNewPSUState(const STATE& value, const pvStruct& pv) const
{
	putValue(pv, static_cast<int>(value));
	messenger.printDebugMessage("SENT POWER " + std::to_string(value) + " FOR " + pv.fullPVName + " TO EPICS");
}
