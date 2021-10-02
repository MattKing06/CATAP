#include "Hardware.h"
#include <boost/algorithm/string.hpp>
#include <boost/make_shared.hpp>
#include <vector>
#include "GlobalFunctions.h"
#include "GlobalTypeEnums.h"
#include <PythonTypeConversions.h>


Hardware::Hardware() :
	machine_area(TYPE::UNKNOWN_TYPE),
	hardware_type(TYPE::UNKNOWN_TYPE),
	machine_area_str(ENUM_TO_STRING(TYPE::UNKNOWN_TYPE)),
	hardware_type_str(ENUM_TO_STRING(TYPE::UNKNOWN_TYPE)),
	currentSnapshot(HardwareSnapshot())
{
	messenger.printDebugMessage("Constructing Hardware ", hardwareName, " complete" );
}

Hardware::Hardware(STATE mode) :
	machine_area(TYPE::UNKNOWN_TYPE),
	hardware_type(TYPE::UNKNOWN_TYPE),
	machine_area_str(ENUM_TO_STRING(TYPE::UNKNOWN_TYPE)),
	hardware_type_str(ENUM_TO_STRING(TYPE::UNKNOWN_TYPE)),
	currentSnapshot(HardwareSnapshot()),
	mode(mode)
{
	//messenger.printDebugMessage("Constructing Hardware (with no config)");
	std::cout << "Constructing Hardware with no config" << std::endl;
}

Hardware::Hardware(const std::map<std::string, std::string>& specificValueMap, STATE mode) :
mode(mode),
messenger(LoggingSystem(true, true)),
specificHardwareParameters(specificValueMap),
// TODO exceptions?? should be more robust, 
// TODO we DONT need the string verions machine_area  and hardware_type they are ENUMS
// TODO is hardware type and machine area in all hardware files ?? 
machine_area_str(specificValueMap.at("machine_area")),
hardwareName(specificValueMap.at("name")),
hardware_type_str(specificValueMap.at("hardware_type")),
// this will NOT be set correctly if your machine_area type is not defined in GlobalTypeEnums AND in GlobalFunctions::stringToType
machine_area(GlobalConstants::stringToTypeMap.at(specificValueMap.at("machine_area"))),
hardware_type(GlobalConstants::stringToTypeMap.at(specificValueMap.at("hardware_type"))),
onlineProperties(std::map<std::string, std::string>()),
offlineProperties(std::map<std::string, std::string>()),
currentSnapshot(HardwareSnapshot()),
pvStructs(std::map<std::string, pvStruct>())
{
	messenger.printDebugMessage("Constructing Hardware ", hardwareName);
}

Hardware::Hardware(const Hardware& copyHardware) :
	messenger(copyHardware.messenger), 
	hardware_type(copyHardware.hardware_type),
	machine_area(copyHardware.machine_area), 
	mode(copyHardware.mode), 
	hardwareName(copyHardware.hardwareName),
	hardware_type_str(copyHardware.hardware_type_str),
	machine_area_str(copyHardware.machine_area_str),
	onlineProperties(copyHardware.onlineProperties),
	offlineProperties(copyHardware.offlineProperties),
	currentSnapshot(copyHardware.currentSnapshot)
{
	pvStructs.insert(copyHardware.pvStructs.begin(), copyHardware.pvStructs.end());
	specificHardwareParameters.insert(copyHardware.specificHardwareParameters.begin(), copyHardware.specificHardwareParameters.end());

	messenger.printDebugMessage(" specificHardwareParameters " );
	for (auto&& it : specificHardwareParameters)
	{
		messenger.printDebugMessage(it.first, " = ", it.second );
	}

}

std::string Hardware::getMachineAreaStr() const
{
	return machine_area_str;
}

TYPE Hardware::getMachineArea() const
{
	return machine_area;
}
std::string Hardware::getHardwareTypeStr() const
{
	return hardware_type_str;
}
TYPE Hardware::getHardwareType() const
{
	return hardware_type;
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
std::map<std::string, std::string> Hardware::getOnlineProperties() const
{
	return onlineProperties;
}

boost::python::dict Hardware::getOnlineProperties_Py()
{
	return to_py_dict(onlineProperties);
}


void Hardware::setOnlineProperties(const std::map<std::string, std::string>& properties)
{
	onlineProperties = properties;
}

HardwareSnapshot Hardware::getSnapshot()
{
	std::cout << "BASE CLASS CALLED" << std::endl;
	return this->currentSnapshot;
}
boost::python::dict Hardware::getSnapshot_Py() 
{
	std::cout << "BASE CLASS CALLED" << std::endl;
	return this->currentSnapshot.getSnapshot_Py();
}
std::map<std::string, std::string> Hardware::getOfflineProperties() const
{
	return offlineProperties;
}

boost::python::dict Hardware::getOfflineProperties_Py()
{
	return to_py_dict(offlineProperties);
}

void Hardware::setOfflineProperties(const std::map<std::string, std::string>& properties)
{
	offlineProperties = properties;
}

STATE Hardware::getMode() const
{
	return mode;
}



bool Hardware::isVirtual()const
{
	return getMode() == STATE::VIRTUAL;
}
bool Hardware::isPhysical()const
{
	return getMode() == STATE::PHYSICAL;
}
bool Hardware::isOffline()const
{
	return getMode() == STATE::OFFLINE;
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
