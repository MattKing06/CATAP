#include "Hardware.h"
#include <boost/algorithm/string.hpp>
#include <boost/make_shared.hpp>
#include <vector>
#include "GlobalFunctions.h"

Hardware::Hardware()
{
}

//Hardware::Hardware(const std::string& name):
//hardwareName(name) 
//{
//}

Hardware::Hardware(const std::map<std::string, std::string>& specificValueMap, STATE mode) :
mode(mode),
messenger(LoggingSystem(false, false)),
specificHardwareParameters(specificValueMap),
machineArea(specificValueMap.find("machine_area")->second),
hardwareType(specificValueMap.find("hardware_type")->second)
{
	switch (mode)
	{
	case STATE::VIRTUAL: hardwareName = "VM-" + specificValueMap.find("name")->second; break;
	case STATE::PHYSICAL: hardwareName = specificValueMap.find("name")->second; break;
	default:
		hardwareName = specificValueMap.find("name")->second; break;
	}
	messenger.printDebugMessage("Constructing Hardware ", hardwareName);
}

Hardware::Hardware(const Hardware& copyHardware) :
	messenger(copyHardware.messenger), hardwareType(copyHardware.hardwareType),
	machineArea(copyHardware.machineArea), mode(copyHardware.mode)
{
	pvStructs.insert(copyHardware.pvStructs.begin(), copyHardware.pvStructs.end());
	specificHardwareParameters.insert(copyHardware.specificHardwareParameters.begin(), copyHardware.specificHardwareParameters.end());
}

std::string Hardware::getMachineArea() const
{
	return machineArea;
}
std::string Hardware::getHardwareType() const
{
	return hardwareType;
}
std::string Hardware::getHardwareName() const
{
	return hardwareName;
}
std::map<std::string, pvStruct>& Hardware::getPVStructs()
{
	return pvStructs;
}
//std::map<int, pvStruct>& Hardware::getPVStructs2()
//{
//	return pvStructs2;
//}
std::map<std::string, std::string> Hardware::getSpecificHardwareParameters() const
{
	return specificHardwareParameters;
}
STATE Hardware::getMode() const
{
	return mode;
}


bool Hardware::operator==(Hardware rhs)
{
	auto LHSPVStructs = this->getPVStructs();
	auto RHSPVStructs = rhs.getPVStructs();
	if (LHSPVStructs.size() != RHSPVStructs.size())
	{
		return false;
	}
	else
	{
		// go through entries in the LHS map
		for (auto& value : LHSPVStructs)
		{
			//if our value in LHS != the value found for RHS[key]
			// we cannot have equal PV structs so we return false
			if (value.second != RHSPVStructs[value.first])
			{
				return false;
			}
		}
	}
	//otherwise, return true
	return true;
}

void Hardware::debugMessagesOn()
{
	messenger.debugMessagesOn();
	messenger.printDebugMessage(hardwareName, " - DEBUG On");
}

void Hardware::debugMessagesOff()
{
	messenger.printDebugMessage(hardwareName, "- DEBUG OFF");
	messenger.debugMessagesOff();
}

void Hardware::messagesOn()
{
	messenger.messagesOn();
	messenger.printMessage(hardwareName, " - MESSAGES On");
}

void Hardware::messagesOff()
{
	messenger.printMessage(hardwareName, " - MESSAGES OFF");
	messenger.messagesOff();
}

bool Hardware::isMessagingOn()
{
	return messenger.isMessagingOn();
}

bool Hardware::isDebugOn()
{
	return messenger.isDebugOn();
}
