#include "Hardware.h"
#include <boost/algorithm/string.hpp>
#include <boost/make_shared.hpp>
#include <vector>
#include "GlobalFunctions.h"
#include "GlobalTypeEnums.h"


Hardware::Hardware() :
machineArea(TYPE::UNKNOWN_AREA),
hardwareType(TYPE::UNKNOWN_AREA)
{
}

//Hardware::Hardware(const std::string& name):
//hardwareName(name) 
//{
//}

Hardware::Hardware(const std::map<std::string, std::string>& specificValueMap, STATE mode) :
mode(mode),
messenger(LoggingSystem(true, true)),
specificHardwareParameters(specificValueMap),
//machineArea(specificValueMap.find("machine_area")->second),
//hardwareType(specificValueMap.find("hardware_type")->second),
// this will NOT be set correctly if your machine_area type is not defined in GlobalTypeEnums AND in GlobalFunctions::stringToType
machineArea(GlobalFunctions::stringToType(specificValueMap.find("machine_area")->second)),
// this will NOT be set correctly if your hardware_type type is not defined in GlobalTypeEnums AND in GlobalFunctions::stringToType
hardwareType(GlobalFunctions::stringToType(specificValueMap.find("hardware_type")->second)),
hardwareName(specificValueMap.find("name")->second)
{
	//messenger.printDebugMessage("Constructing Hardware ", hardwareName);
	// equal_range returns a variable containing start (first) and end (second)
	// iterators for items in the multimap corresponding to pv records.


/* 	if (hardwareType.compare("Magnet") != 0)
	{
		//messenger.printDebugMessage("hardwareType.compare(Magnet) != 0  IS TRUE");
		std::string pvRecordsStr = specificHardwareParameters.find(hardwareName)->second.data();
		// iterate through the list of matches and set up a pvStruct to add to pvStructs.
		std::vector<std::string> pvRecordVec;

		// split a string by commas
		boost::algorithm::split(pvRecordVec, pvRecordsStr, [](char c) {return c == ','; });

		//messenger.printDebugMessage("Constructing PV information for ", hardwareName);
		for (auto record : pvRecordVec)
		{
			pvStruct pv = pvStruct();
			pv.fullPVName = hardwareName;
			pv.pvRecord = record;
			//chid, count, mask, chtype are left undefined for now.
			pvStructs[pv.pvRecord] = pv;
		}
	}
	else
	{
		//messenger.printDebugMessage("hardwareType.compare(Magnet) != 0  IS FALSE");
	} */
	//messenger.printDebugMessage(hardwareName, " Hardware Constructor Finished ");
}

Hardware::Hardware(const Hardware& copyHardware) :
	messenger(copyHardware.messenger), hardwareType(copyHardware.hardwareType),
	machineArea(copyHardware.machineArea), mode(copyHardware.mode)
{
	pvStructs.insert(copyHardware.pvStructs.begin(), copyHardware.pvStructs.end());
	specificHardwareParameters.insert(copyHardware.specificHardwareParameters.begin(), copyHardware.specificHardwareParameters.end());
}

std::string Hardware::getMachineAreaString() const
{
	return ENUM_TO_STRING(machineArea);
}
std::string Hardware::getHardwareTypeString() const
{
	return ENUM_TO_STRING(hardwareType);
}
TYPE Hardware::getMachineArea() const
{
	return machineArea;
}
TYPE Hardware::getHardwareType() const
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
