#include <EPICSRFProtectionInterface.h>
#include <RFProtectionPVRecords.h>

LoggingSystem EPICSRFProtectionInterface::messenger;

EPICSRFProtectionInterface::EPICSRFProtectionInterface() : EPICSInterface()
{
	messenger = LoggingSystem(true, true);
}

EPICSRFProtectionInterface::EPICSRFProtectionInterface(const EPICSRFProtectionInterface& copyInterface)
{
	messenger.printDebugMessage("EPICSRFProtectionInterface Copy Constructor Called");
}

EPICSRFProtectionInterface::~EPICSRFProtectionInterface()
{
	messenger.printDebugMessage("EPICSRFProtectionInterface Destructor Called");
}

void EPICSRFProtectionInterface::retrieveUpdateFunctionForRecord(const pvStruct& pv)
{
	if (pv.pvRecord == RFProtectionRecords::Sta)
	{
		pv.updateFunction = updateProtectionStatus;
	}
	if (pv.pvRecord == RFProtectionRecords::Cmi)
	{
		pv.updateFunction = updateProtectionCmi;
	}
}

void EPICSRFProtectionInterface::updateProtectionStatus(const struct event_handler_args args)
{
	RFProtection* protection = getHardwareFromArgs<RFProtection>(args);

	std::pair<epicsTimeStamp, long > r = getTimeStampUShortPair(args);
	protection->status.first = r.first;
	switch (r.second)
	{
	case 0:  protection->status.second = STATE::BAD;  break;
	case 1:  protection->status.second = STATE::GOOD; break;
	}
	messenger.printMessage(protection->getHardwareName(), " new Sta = ", ENUM_TO_STRING(protection->status.second));
	
}

void EPICSRFProtectionInterface::updateProtectionCmi(const struct event_handler_args args)
{
	RFProtection* protection = getHardwareFromArgs<RFProtection>(args);
	updateTimeStampLongPair(args, protection->cmi);
	messenger.printMessage(protection->getHardwareName(), " new Cmi = ", protection->cmi.second);
	/*
		the bits in the cmi number code the state of the protection 	
	*/
	size_t counter = GlobalConstants::zero_sizet;
	for (auto bit = 0; bit < 8; ++bit)//MAGIC_NUMBER
	{
		//message(obj.name, " bit ", bit, " value = ",  (obj.cmi &( 1 << bit)) >> bit);
		if (std::find(protection->protKeyBits.begin(),
			protection->protKeyBits.end(), bit) != protection->protKeyBits.end())
		{
			protection->protKeyBitValues[counter] = (protection->cmi.second & (1 << bit)) >> bit;
			//message("obj.protKeyBitValues part ",
			//        counter, " = ",
			//        obj.protKeyBitValues[counter]);
			++counter;
		}
	}

	//if (allkeybitsaregood(obj))
	//{
	//	//message("Allkeybits for ", ENUM_TO_STRING(obj.protType), " ARE GOOD");
	//	//switch (obj.protType)
	//	//{
	//	//	// TODOD: This is completely wrong for the lINAC we need to test it
	//	//case rfProtStructs::RF_PROT_TYPE::TEST:
	//	//	currentMode = obj.protType;
	//	//	break;
	//	//default:
	//	//	currentMode = rfProtStructs::RF_PROT_TYPE::L01;
	//	//}
	//	//message("RF GUN Mode is ", ENUM_TO_STRING(currentMode));
	//}


}

bool EPICSRFProtectionInterface::setProtectionOn(const pvStruct& pv)
{
	return false;
}

bool EPICSRFProtectionInterface::setProtectionOff(const pvStruct& pv)
{
	return false;
}

bool EPICSRFProtectionInterface::resetProtection(const pvStruct& pv)
{
	return false;
}



bool EPICSRFProtectionInterface::sendCmmands(const std::vector<const chtype*>& CHTYPE, const std::vector<const chid*>& CHID)
{
	bool ret = false;
	for (size_t i = 0; i < CHTYPE.size(); ++i)
	{
		ca_put(*CHTYPE[i], *CHID[i], &GlobalConstants::EPICS_ACTIVATE);
	}
	int status = sendToEPICSReturnStatus();
	if (status == ECA_NORMAL)
	{
		for (size_t i = 0; i < CHTYPE.size(); ++i)
		{
			ca_put(*CHTYPE[i], *CHID[i], &GlobalConstants::EPICS_SEND);
		}
		int status = sendToEPICSReturnStatus();
		if (status == ECA_NORMAL)
		{
			ret = true;
		}
	}
	return ret;
}