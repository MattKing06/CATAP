#include "Hardware.h"
#include <boost/algorithm/string.hpp>
#include <vector>

Hardware::Hardware()
{
	std::cout << "DEFAULT CONSTRUCTOR CALLED " << std::endl;
}

Hardware::Hardware(const std::map<std::string, std::string>& specificValueMap, bool isVirtual = false) :
isVirtual(isVirtual),
messenger(LoggingSystem(false,false)),
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
	messenger.printDebugMessage(std::string("Constructing Hardware" + hardwareName));
	
	// equal_range returns a variable containing start (first) and end (second)
	// iterators for items in the multimap corresponding to pv records.
	std::string pvRecordsStr = specificHardwareParameters.find(hardwareName)->second.data();
	// iterate through the list of matches and set up a pvStruct to add to pvStructs.
	std::vector<std::string> pvRecordVec;
	boost::algorithm::split(pvRecordVec, pvRecordsStr, [](char c){return c == ','; });
	messenger.printDebugMessage(std::string("Constructing PV information for " + hardwareName));
	for (auto record : pvRecordVec)
	{
		pvStruct& pv = pvStruct();
		pv.fullPVName = hardwareName;
		pv.pvRecord = record;
		//chid, count, mask, chtype are left undefined for now.
		pvStructs[pv.pvRecord] = pv;
	}
	messenger.printDebugMessage(std::string("Finished constructing: " + hardwareName));
}

Hardware::Hardware(const Hardware& copyHardware) :
messenger(copyHardware.messenger), hardwareType(copyHardware.hardwareType),
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
