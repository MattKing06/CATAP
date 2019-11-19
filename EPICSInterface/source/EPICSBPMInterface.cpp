#include "EPICSBPMInterface.h"

#define MY_SEVCHK(status)		\
{								\
	if (status != ECA_NORMAL)	\
		{						\
		SEVCHK(status, NULL);	\
		exit(status);			\
		}						\
}								\

LoggingSystem EPICSBPMInterface::messenger;

EPICSBPMInterface::EPICSBPMInterface() : EPICSInterface()
{
	this->messenger = LoggingSystem(false, false);
}
EPICSBPMInterface::~EPICSBPMInterface()
{
	messenger.debugMessagesOff();
	messenger.printDebugMessage("EPICSBPMInterface Destructor Called");
}
void EPICSBPMInterface::retrieveUpdateFunctionForRecord(pvStruct &pvStruct) const
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
	//if (pvStruct.pvRecord == "DATA:B2V.VALA")
	//{
	//	pvStruct.updateFunction = this->updateData;
	//}
	//if (pvStruct.pvRecord == "AWAK")
	//{
	//	pvStruct.updateFunction = this->updateAWAK;
	//}
	//if (pvStruct.pvRecord == "RDY")
	//{
	//	pvStruct.updateFunction = this->updateRDY;
	//}
}

void EPICSBPMInterface::updateXPV(const struct event_handler_args args)
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
		BPM* recastBPM = static_cast<BPM*>(args.usr);
		recastBPM->setXPV(*(double*)(args.dbr));
		recastBPM->setXPVBuffer(*(double*)(args.dbr));
		//messenger.printDebugMessage("GETSETI VALUE FOR " + recastMagnet->getHardwareName() + ": " + std::to_string(*(double*)(args.dbr)));
	}
	else if (args.type == DBR_TIME_DOUBLE)
	{
		MY_SEVCHK(args.status);
		BPM* recastBPM = static_cast<BPM*>(args.usr);
		const struct dbr_time_double* pTD = (const struct dbr_time_double*)(args.dbr);
		recastBPM->pvStructs.at("X").time = pTD->stamp;
		recastBPM->setXPV(pTD->value);
		recastBPM->setXPVBuffer(pTD->value);
		messenger.printDebugMessageWithEPICSTimestampString(getEPICSTime(recastBPM->pvStructs.at("X").time),
			"X PV VALUE FOR: " + recastBPM->getHardwareName() + ": "
			+ std::to_string(pTD->value));
	}
	messenger.printDebugMessage(" CALLED UPDATE X ");
}

void EPICSBPMInterface::updateYPV(const struct event_handler_args args)
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
		BPM* recastBPM = static_cast<BPM*>(args.usr);
		recastBPM->setYPV(*(double*)(args.dbr));
		recastBPM->setYPVBuffer(*(double*)(args.dbr));
		//messenger.printDebugMessage("GETSETI VALUE FOR " + recastMagnet->getHardwareName() + ": " + std::to_string(*(double*)(args.dbr)));
	}
	else if (args.type == DBR_TIME_DOUBLE)
	{
		MY_SEVCHK(args.status);
		BPM* recastBPM = static_cast<BPM*>(args.usr);
		const struct dbr_time_double* pTD = (const struct dbr_time_double*)(args.dbr);
		recastBPM->pvStructs.at("Y").time = pTD->stamp;
		recastBPM->setYPV(pTD->value);
		recastBPM->setYPVBuffer(pTD->value);
		messenger.printDebugMessageWithEPICSTimestampString(getEPICSTime(recastBPM->pvStructs.at("Y").time),
			"Y PV VALUE FOR: " + recastBPM->getHardwareName() + ": "
			+ std::to_string(pTD->value));
	}
	messenger.printDebugMessage(" CALLED UPDATE Y ");
}

void EPICSBPMInterface::updateRA1(const struct event_handler_args args)
{
	messenger.debugMessagesOff();
	if (args.status != ECA_NORMAL)
	{
		messenger.messagesOn();
		messenger.printMessage("Something went wrong with update function!");
		messenger.messagesOff();
	}
	else if (args.type == DBR_LONG)
	{
		MY_SEVCHK(args.status);
		BPM* recastBPM = static_cast<BPM*>(args.usr);
		recastBPM->setRA1(*(long*)(args.dbr));
		//messenger.printDebugMessage("GETSETI VALUE FOR " + recastMagnet->getHardwareName() + ": " + std::to_string(*(double*)(args.dbr)));
	}
	else if (args.type == DBR_TIME_LONG)
	{
		MY_SEVCHK(args.status);
		BPM* recastBPM = static_cast<BPM*>(args.usr);
		const struct dbr_time_long* pTD = (const struct dbr_time_long*)(args.dbr);
		recastBPM->pvStructs.at("RA1").time = pTD->stamp;
		recastBPM->setRA1(pTD->value);
	}
	messenger.printDebugMessage(" CALLED UPDATE RA1 ");
}

void EPICSBPMInterface::updateRA2(const struct event_handler_args args)
{
	messenger.debugMessagesOff();
	if (args.status != ECA_NORMAL)
	{
		messenger.messagesOn();
		messenger.printMessage("Something went wrong with update function!");
		messenger.messagesOff();
	}
	else if (args.type == DBR_LONG)
	{
		MY_SEVCHK(args.status);
		BPM* recastBPM = static_cast<BPM*>(args.usr);
		recastBPM->setRA2(*(long*)(args.dbr));
		//messenger.printDebugMessage("GETSETI VALUE FOR " + recastMagnet->getHardwareName() + ": " + std::to_string(*(double*)(args.dbr)));
	}
	else if (args.type == DBR_TIME_LONG)
	{
		MY_SEVCHK(args.status);
		BPM* recastBPM = static_cast<BPM*>(args.usr);
		const struct dbr_time_long* pTD = (const struct dbr_time_long*)(args.dbr);
		recastBPM->pvStructs.at("RA2").time = pTD->stamp;
		recastBPM->setRA2(pTD->value);
	}
	messenger.printDebugMessage(" CALLED UPDATE RA1 ");
}

void EPICSBPMInterface::updateRD1(const struct event_handler_args args)
{
	messenger.debugMessagesOff();
	if (args.status != ECA_NORMAL)
	{
		messenger.messagesOn();
		messenger.printMessage("Something went wrong with update function!");
		messenger.messagesOff();
	}
	else if (args.type == DBR_LONG)
	{
		MY_SEVCHK(args.status);
		BPM* recastBPM = static_cast<BPM*>(args.usr);
		recastBPM->setRD1(*(long*)(args.dbr));
		//messenger.printDebugMessage("GETSETI VALUE FOR " + recastMagnet->getHardwareName() + ": " + std::to_string(*(double*)(args.dbr)));
	}
	else if (args.type == DBR_TIME_LONG)
	{
		MY_SEVCHK(args.status);
		BPM* recastBPM = static_cast<BPM*>(args.usr);
		const struct dbr_time_long* pTD = (const struct dbr_time_long*)(args.dbr);
		recastBPM->pvStructs.at("RD1").time = pTD->stamp;
		recastBPM->setRD1(pTD->value);
	}
	messenger.printDebugMessage(" CALLED UPDATE RA1 ");
}

void EPICSBPMInterface::updateRD2(const struct event_handler_args args)
{
	messenger.debugMessagesOff();
	if (args.status != ECA_NORMAL)
	{
		messenger.messagesOn();
		messenger.printMessage("Something went wrong with update function!");
		messenger.messagesOff();
	}
	else if (args.type == DBR_LONG)
	{
		MY_SEVCHK(args.status);
		BPM* recastBPM = static_cast<BPM*>(args.usr);
		recastBPM->setRD2(*(long*)(args.dbr));
		//messenger.printDebugMessage("GETSETI VALUE FOR " + recastMagnet->getHardwareName() + ": " + std::to_string(*(double*)(args.dbr)));
	}
	else if (args.type == DBR_TIME_LONG)
	{
		MY_SEVCHK(args.status);
		BPM* recastBPM = static_cast<BPM*>(args.usr);
		const struct dbr_time_long* pTD = (const struct dbr_time_long*)(args.dbr);
		recastBPM->pvStructs.at("RD2").time = pTD->stamp;
		recastBPM->setRD2(pTD->value);
	}
	messenger.printDebugMessage(" CALLED UPDATE RA1 ");
}

void EPICSBPMInterface::updateSA1(const struct event_handler_args args)
{
	messenger.debugMessagesOff();
	if (args.status != ECA_NORMAL)
	{
		messenger.messagesOn();
		messenger.printMessage("Something went wrong with update function!");
		messenger.messagesOff();
	}
	else if (args.type == DBR_LONG)
	{
		MY_SEVCHK(args.status);
		BPM* recastBPM = static_cast<BPM*>(args.usr);
		recastBPM->setSA1(*(long*)(args.dbr));
		//messenger.printDebugMessage("GETSETI VALUE FOR " + recastMagnet->getHardwareName() + ": " + std::to_string(*(double*)(args.dbr)));
	}
	else if (args.type == DBR_TIME_LONG)
	{
		MY_SEVCHK(args.status);
		BPM* recastBPM = static_cast<BPM*>(args.usr);
		const struct dbr_time_long* pTD = (const struct dbr_time_long*)(args.dbr);
		recastBPM->pvStructs.at("SA1").time = pTD->stamp;
		recastBPM->setSA1(pTD->value);
	}
	messenger.printDebugMessage(" CALLED UPDATE RA1 ");
}

void EPICSBPMInterface::updateSA2(const struct event_handler_args args)
{
	messenger.debugMessagesOff();
	if (args.status != ECA_NORMAL)
	{
		messenger.messagesOn();
		messenger.printMessage("Something went wrong with update function!");
		messenger.messagesOff();
	}
	else if (args.type == DBR_LONG)
	{
		MY_SEVCHK(args.status);
		BPM* recastBPM = static_cast<BPM*>(args.usr);
		recastBPM->setSA2(*(long*)(args.dbr));
		//messenger.printDebugMessage("GETSETI VALUE FOR " + recastMagnet->getHardwareName() + ": " + std::to_string(*(double*)(args.dbr)));
	}
	else if (args.type == DBR_TIME_LONG)
	{
		MY_SEVCHK(args.status);
		BPM* recastBPM = static_cast<BPM*>(args.usr);
		const struct dbr_time_long* pTD = (const struct dbr_time_long*)(args.dbr);
		recastBPM->pvStructs.at("SA2").time = pTD->stamp;
		recastBPM->setSA2(pTD->value);
	}
	messenger.printDebugMessage(" CALLED UPDATE RA1 ");
}

void EPICSBPMInterface::updateSD1(const struct event_handler_args args)
{
	messenger.debugMessagesOff();
	if (args.status != ECA_NORMAL)
	{
		messenger.messagesOn();
		messenger.printMessage("Something went wrong with update function!");
		messenger.messagesOff();
	}
	else if (args.type == DBR_LONG)
	{
		MY_SEVCHK(args.status);
		BPM* recastBPM = static_cast<BPM*>(args.usr);
		recastBPM->setSD1(*(long*)(args.dbr));
		//messenger.printDebugMessage("GETSETI VALUE FOR " + recastMagnet->getHardwareName() + ": " + std::to_string(*(double*)(args.dbr)));
	}
	else if (args.type == DBR_TIME_LONG)
	{
		MY_SEVCHK(args.status);
		BPM* recastBPM = static_cast<BPM*>(args.usr);
		const struct dbr_time_long* pTD = (const struct dbr_time_long*)(args.dbr);
		recastBPM->pvStructs.at("SD1").time = pTD->stamp;
		recastBPM->setSD1(pTD->value);
	}
	messenger.printDebugMessage(" CALLED UPDATE RA1 ");
}

void EPICSBPMInterface::updateSD2(const struct event_handler_args args)
{
	messenger.debugMessagesOff();
	if (args.status != ECA_NORMAL)
	{
		messenger.messagesOn();
		messenger.printMessage("Something went wrong with update function!");
		messenger.messagesOff();
	}
	else if (args.type == DBR_LONG)
	{
		MY_SEVCHK(args.status);
		BPM* recastBPM = static_cast<BPM*>(args.usr);
		recastBPM->setSD2(*(long*)(args.dbr));
		//messenger.printDebugMessage("GETSETI VALUE FOR " + recastMagnet->getHardwareName() + ": " + std::to_string(*(double*)(args.dbr)));
	}
	else if (args.type == DBR_TIME_LONG)
	{
		MY_SEVCHK(args.status);
		BPM* recastBPM = static_cast<BPM*>(args.usr);
		const struct dbr_time_long* pTD = (const struct dbr_time_long*)(args.dbr);
		recastBPM->pvStructs.at("RA1").time = pTD->stamp;
		recastBPM->setSD2(pTD->value);
	}
	messenger.printDebugMessage(" CALLED UPDATE RA1 ");
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

template<typename T>
void EPICSBPMInterface::putValue(const pvStruct& pvStruct, const T& value) const
{
	if (ca_state(pvStruct.CHID) == cs_conn)
	{
		int status = ca_put(pvStruct.CHTYPE, pvStruct.CHID, &value);
		MY_SEVCHK(status);
		status = ca_pend_io(CA_PEND_IO_TIMEOUT);
		MY_SEVCHK(status);
	}
	else
	{
		messenger.messagesOn();
		messenger.printMessage("NOT CONNECTED TO EPICS");
		messenger.messagesOff();
	}
}