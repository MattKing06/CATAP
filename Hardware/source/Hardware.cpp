#include "Hardware.h"
#include <boost/algorithm/string.hpp>
#include <boost/make_shared.hpp>
#include <vector>

Hardware::Hardware()
{
}

Hardware::Hardware(const std::map<std::string, std::string>& specificValueMap, bool isVirtual = false) :
isVirtual(isVirtual),
//messenger(LoggingSystem(false,false)),
specificHardwareParameters(specificValueMap),
machineArea(specificValueMap.find("machine_area")->second),
hardwareType(specificValueMap.find("hardware_type")->second)
{
	if (this->isVirtual)
	{
		hardwareName = specificValueMap.find("virtual_name")->second.data();
	}
	else
	{
		hardwareName = specificValueMap.find("name")->second.data();
	}
	printDebugMessage( "Constructing Hardware ", hardwareName);
	
	// equal_range returns a variable containing start (first) and end (second)
	// iterators for items in the multimap corresponding to pv records.
	std::string pvRecordsStr = specificHardwareParameters.find(hardwareName)->second.data();
	// iterate through the list of matches and set up a pvStruct to add to pvStructs.
	std::vector<std::string> pvRecordVec;
	boost::algorithm::split(pvRecordVec, pvRecordsStr, [](char c){return c == ','; });
	printDebugMessage("Constructing PV information for ", hardwareName);
	for (auto record : pvRecordVec)
	{
		pvStruct pv = pvStruct();
		pv.fullPVName = hardwareName;
		pv.pvRecord = record;
		//chid, count, mask, chtype are left undefined for now.
		pvStructs[pv.pvRecord] = pv;
	}
	printDebugMessage("Finished constructing: ", hardwareName);
}

Hardware::Hardware(const Hardware& copyHardware) :
hardwareType(copyHardware.hardwareType),
machineArea(copyHardware.machineArea), isVirtual(copyHardware.isVirtual)
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
std::map<std::string, std::string> Hardware::getSpecificHardwareParameters() const
{
	return specificHardwareParameters;
}

//void Hardware::debugMessagesOn()
//{
//	debugMessagesOn();
//	printDebugMessage(hardwareName, " - DEBUG ON");
//}
//
//void Hardware::debugMessagesOff()
//{
//	printDebugMessage(hardwareName, "- DEBUG OFF");
//	debugMessagesOff();
//}
//
//void Hardware::messagesOn()
//{
//	messagesOn();
//	printMessage(hardwareName, " - MESSAGES ON");
//}
//
//void Hardware::messagesOff()
//{
//	printMessage(hardwareName, " - MESSAGES OFF");
//	messagesOff();
//}
//
//bool Hardware::isMessagingOn()
//{
//	return isMessagingOn();
//}
//
//bool Hardware::isDebugOn()
//{
//	return isDebugOn();
//}

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
		for (auto &value : LHSPVStructs)
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
