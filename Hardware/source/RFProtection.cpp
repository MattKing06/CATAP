#include <RFProtection.h>
#include <RFProtectionPVRecords.h>

RFProtection::RFProtection()
{
}

RFProtection::RFProtection(const std::map<std::string, std::string>& paramMap, STATE mode) :
	Hardware(paramMap, mode),
	RFProtectionParamMap(paramMap),
	epicsInterface(boost::make_shared<EPICSRFProtectionInterface>(EPICSRFProtectionInterface()))
{
	std::cout << "constructing PV data for" << std::endl;
	setPVStructs();
}

RFProtection::RFProtection(const RFProtection& copyRFProtection)
{
}

RFProtection::~RFProtection()
{
}

void RFProtection::setPVStructs()
{
	for (auto& record : RFProtectionRecords::rfProtectionRecordsList)
	{
		pvStructs[record] = pvStruct();
		pvStructs[record].pvRecord = record;
		std::cout << " RECORD: " << record << std::endl;
		std::string PV = specificHardwareParameters.find(record)->second;
		switch (mode)
		{
		case(STATE::VIRTUAL):
			pvStructs[record].fullPVName = "VM-" + PV;
			break;
		case(STATE::PHYSICAL):
			pvStructs[record].fullPVName = PV;
			break;
		default:
			pvStructs[record].fullPVName = PV;
			break;
		}
	}
}

void RFProtection::debugMessagesOn()
{
	messenger.debugMessagesOn();
	messenger.printDebugMessage(hardwareName, " - DEBUG ON");
	epicsInterface->debugMessagesOn();
}

void RFProtection::debugMessagesOff()
{
	messenger.debugMessagesOff();
	messenger.printDebugMessage(hardwareName, " - DEBUG OFF");
	epicsInterface->debugMessagesOff();
}

void RFProtection::messagesOn()
{
	messenger.messagesOn();
	messenger.printMessage(hardwareName, " - MESSAGES ON");
	epicsInterface->messagesOn();
}

void RFProtection::messagesOff()
{
	messenger.printMessage(hardwareName, " - MESSAGES OFF");
	messenger.messagesOff();
	epicsInterface->messagesOff();
}
