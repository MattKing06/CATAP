#include <RFProtection.h>
#include <RFProtectionPVRecords.h>
#include "boost/algorithm/string/split.hpp"
// CATAP includes
#include "PythonTypeConversions.h"
#include "GlobalConstants.h"
#include "GlobalFunctions.h"

RFProtection::RFProtection()
{
}

RFProtection::RFProtection(const std::map<std::string, std::string>& paramMap, STATE mode) :
	Hardware(paramMap, mode),
	epicsInterface(boost::make_shared<EPICSRFProtectionInterface>(EPICSRFProtectionInterface()))
{
	messenger.printMessage("setMasterLatticeData data for: ", hardwareName);
	setMasterLatticeData();
	messenger.printMessage("constructing PV data for: ", hardwareName);
	setPVStructs();
}
RFProtection::RFProtection(const RFProtection& copyRFProtection)
{
}

RFProtection::~RFProtection()
{
}


void RFProtection::setMasterLatticeData()
{
	messenger.printMessage("setMasterLatticeData ", hardwareName);
	//protectionType(string_to_hardware_type_map.at(RFProtectionParamMap.at("prot_type")))

	boost::split(aliases, specificHardwareParameters.find("name_alias")->second, [](char c) {return c == ','; });


	if (GlobalFunctions::entryExists(GlobalConstants::stringToTypeMap, specificHardwareParameters.at("prot_type")))
	{
		protectionType = GlobalConstants::stringToTypeMap.at(specificHardwareParameters.at("prot_type"));
		messenger.printMessage("protectionType = ", ENUM_TO_STRING(protectionType));

	}
	else
	{
		messenger.printMessage("!!ERROR!! Can't find protectionType = ", specificHardwareParameters.at("prot_type"));
	}

	if (GlobalFunctions::entryExists(GlobalConstants::stringToTypeMap, specificHardwareParameters.at("machine_area")))
	{
		machine_area = GlobalConstants::stringToTypeMap.at(specificHardwareParameters.at("machine_area"));
		messenger.printMessage("machine_area = ", ENUM_TO_STRING(machine_area));

	}
	else
	{
		messenger.printMessage("!!ERROR!! Can't find machine_area = ", specificHardwareParameters.at("machine_area"));
	}

	if (GlobalFunctions::entryExists(GlobalConstants::stringToTypeMap, specificHardwareParameters.at("hardware_type")))
	{
		machine_area = GlobalConstants::stringToTypeMap.at(specificHardwareParameters.at("hardware_type"));
		messenger.printMessage("machine_area = ", ENUM_TO_STRING(machine_area));

	}
	else
	{
		messenger.printMessage("!!ERROR!! Can't find hardware_type = ", specificHardwareParameters.at("hardware_type"));
	}


}




long RFProtection::getCmi()const
{
	return cmi.second;
}
STATE RFProtection::getStatus()const
{
	return status.second;
}
std::vector<int>  RFProtection::getKeyBits()const
{
	return protKeyBits;
}
std::vector<bool> RFProtection::getKeyBitValues() const
{
	return protKeyBitValues;
}

bool RFProtection::isGood() const
{
	return status.second == STATE::GOOD;
}
bool RFProtection::isNotGood()const
{
	return status.second != STATE::GOOD;
}

bool RFProtection::isBad() const
{
	return status.second == STATE::BAD;
}

bool RFProtection::reset() const
{
	return false;
}
bool RFProtection::enable() const
{
	return false;
}
bool RFProtection::disable() const
{
	return false;
}

std::vector<std::string> RFProtection::getAliases() const
{
	return aliases;
}
boost::python::list RFProtection::getAliases_Py() const
{
	return to_py_list<std::string>(getAliases());
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
