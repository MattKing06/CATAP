#include <RFProtection.h>
#include <RFProtectionPVRecords.h>

RFProtection::RFProtection()
{
}

RFProtection::RFProtection(const std::map<std::string, std::string>& paramMap, STATE mode) :
	Hardware(paramMap, mode),
	RFProtectionParamMap(paramMap),
	epicsInterface(boost::make_shared<EPICSRFProtectionInterface>(EPICSRFProtectionInterface())),
	protectionType(string_to_hardware_type_map.at(paramMap.at("prot_type")))
{
	messenger.printMessage("constructing PV data for: ", hardwareName);
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

TYPE RFProtection::getProtectionType() const
{
	return protectionType;
}

std::string RFProtection::getProtectionTypeAsStr() const
{
	return ENUM_TO_STRING(protectionType);
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
