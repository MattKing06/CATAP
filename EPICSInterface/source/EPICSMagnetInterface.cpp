#include "EPICSMagnetInterface.h"

LoggingSystem EPICSMagnetInterface::messenger;

EPICSMagnetInterface::EPICSMagnetInterface() : EPICSInterface()
{
	this->messenger = LoggingSystem(true, true);
}
EPICSMagnetInterface::~EPICSMagnetInterface()
{
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
	if (args.status != ECA_NORMAL)
	{
		messenger.printMessage( "Something went wrong with update function!");
	}
	else if (args.type == DBR_DOUBLE)
	{
		MY_SEVCHK(args.status);
		Magnet* recastMagnet = static_cast<Magnet*>(args.usr);
		recastMagnet->setCurrent(*(double*)(args.dbr));
	}
	else if (args.type == DBR_TIME_DOUBLE)
	{
		MY_SEVCHK(args.status);
		Magnet* recastMagnet = static_cast<Magnet*>(args.usr);
		const struct dbr_time_double* pTD = (const struct dbr_time_double*)(args.dbr);
		recastMagnet->pvStructs.at("GETSETI").time = pTD->stamp;
		recastMagnet->setCurrent(pTD->value);
	}
}

void EPICSMagnetInterface::updatePSUState(const struct event_handler_args args)
{
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
	}
	else if (args.type == DBR_TIME_ENUM)
	{
		MY_SEVCHK(args.status);
		Magnet* recastMagnet = static_cast<Magnet*>(args.usr);
		const struct dbr_time_enum* pTD = (const struct dbr_time_enum*)(args.dbr);
		recastMagnet->pvStructs.at("RPOWER").time = pTD->stamp;
		recastMagnet->setPSUState((STATE)pTD->value);
	}
}

void EPICSMagnetInterface::updateREADI(const struct event_handler_args args)
{
	if (args.status != ECA_NORMAL)
	{
		std::cerr << "Something whent wrong with update function" << std::endl;
	}
	else if (args.type == DBR_TIME_DOUBLE)
	{
		MY_SEVCHK(args.status);
		Magnet* recastMagnet = static_cast<Magnet*>(args.usr);
		const struct dbr_time_double* pTD = (const struct dbr_time_double*)(args.dbr);
		recastMagnet->pvStructs.at("READI").time = pTD->stamp;
		recastMagnet->setRICurrent(pTD->value);
	}
	else if (args.type == DBR_DOUBLE)
	{
		MY_SEVCHK(args.status);
		Magnet* recastMagnet = static_cast<Magnet*>(args.usr);
		recastMagnet->setRICurrent(*(double*)args.dbr);
	}
}

void EPICSMagnetInterface::updateRILK(const struct event_handler_args args)
{
	if (args.status != ECA_NORMAL)
	{
		std::cerr << "Something whent wrong with update function" << std::endl;
	}
	else if (args.type == DBR_ENUM)
	{
		MY_SEVCHK(args.status);
		Magnet* recastMagnet = static_cast<Magnet*>(args.usr);
		recastMagnet->setILKState(*(int*)(args.dbr));
	}
	else if (args.type == DBR_TIME_ENUM)
	{
		MY_SEVCHK(args.status);
		Magnet* recastMagnet = static_cast<Magnet*>(args.usr);
		const struct dbr_time_enum* pTD = (const struct dbr_time_enum*)(args.dbr);
		recastMagnet->pvStructs.at("RILK").time = pTD->stamp;
		recastMagnet->setILKState(pTD->value);
	}
}

void EPICSMagnetInterface::setNewCurrent(const double &value, const pvStruct &pv) const
{
	//we have checked that pvRecord is SETI before reaching here.
	putValue(pv, value);
}

void EPICSMagnetInterface::setNewPSUState(const STATE& value, const pvStruct& pv) const
{
	putValue(pv, static_cast<int>(value));
}
