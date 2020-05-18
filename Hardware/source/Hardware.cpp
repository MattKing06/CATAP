#include "Hardware.h"
#include <boost/algorithm/string.hpp>
#include <boost/make_shared.hpp>
#include <vector>
#include "GlobalFunctions.h"
#include "GlobalTypeEnums.h"


// map to convert yaml file strings to magnet TYPE enums
const std::map<std::string, TYPE> Hardware::string_to_hardware_type_map = Hardware::create_map();


Hardware::Hardware()
{
}

Hardware::Hardware(const std::map<std::string, std::string>& specificValueMap, STATE mode) :
mode(mode),
messenger(LoggingSystem(true, true)),
specificHardwareParameters(specificValueMap),
// TODO exceptions?? shoudl be more robut, 
// TODO we DONT need teh string verions machine_area  and hardware_type they are ENUMS
// TODO is hardware type and machine area in al hardware files ?? 
machine_area_str(specificValueMap.find("machine_area")->second),
machine_area(GlobalConstants::stringToTypeMap.at(specificValueMap.find("machine_area")->second)),
hardware_type_str(specificValueMap.find("hardware_type")->second),
//hardwareType_e(GlobalConstants::stringToTypeMap.at(specificValueMap.find("hardware_type")->second)),
hardwareName(specificValueMap.find("name")->second)
{
	messenger.printDebugMessage("Constructing Hardware ", hardwareName);
	// equal_range returns a variable containing start (first) and end (second)
	// iterators for items in the multimap corresponding to pv records.
	if (GlobalFunctions::entryExists(specificValueMap, "hardware_type"))
	{
		if (GlobalFunctions::entryExists(string_to_hardware_type_map, "Magnet"))
		{
			hardware_type = string_to_hardware_type_map.at("Magnet");
		}
	}

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
	messenger(copyHardware.messenger), 
	hardware_type(copyHardware.hardware_type),
	machine_area(copyHardware.machine_area), 
	mode(copyHardware.mode), 
	hardwareName(copyHardware.hardwareName),
	hardware_type_str(copyHardware.hardware_type_str),
	machine_area_str(copyHardware.machine_area_str)
{
	pvStructs.insert(copyHardware.pvStructs.begin(), copyHardware.pvStructs.end());
	specificHardwareParameters.insert(copyHardware.specificHardwareParameters.begin(), copyHardware.specificHardwareParameters.end());
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
