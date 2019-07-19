#include "Hardware.h"
#include <boost/algorithm/string.hpp>

Hardware::Hardware()
{
	machineArea = "UNKNOWN";
	hardwareType = "UNKNOWN";
	specificHardwareParameters = std::map<std::string, std::string>();
}

Hardware::Hardware(std::map<std::string, std::string> specificValueMap, bool isVirtual=false)
{
	logger = LoggingSystem(false, false);
	std::string currentHardwareName;
	//std::string YAMLConfigDirectory = "C:\\Users\\ujo48515\\Documents\\YAMLParserTestFiles";
	this->isVirtual = isVirtual;
	
	if (this->isVirtual)
	{
		currentHardwareName = specificValueMap.find("virtual_name")->second.data();
	}
	else
	{
		currentHardwareName = specificValueMap.find("name")->second.data();
	}

	logger.printDebugMessage(std::string("Constructing Hardware" + currentHardwareName));

	//ConfigReader configReader(currentHardwareName);
	specificHardwareParameters = specificValueMap;
	machineArea = specificValueMap.find("machine_area")->second;
	hardwareType = specificValueMap.find("hardware_type")->second;
	
	// equal_range returns a variable containing start (first) and end (second)
	// iterators for items in the multimap corresponding to pv records.
	std::string pvRecordsStr = specificHardwareParameters.find(currentHardwareName)->second.data();
	// iterate through the list of matches and set up a pvStruct to add to pvStructs.
	std::vector<std::string> pvRecordVec;
	boost::algorithm::split(pvRecordVec, pvRecordsStr, [](char c){return c == ','; });
	logger.printDebugMessage(std::string("Constructing PV information for " + currentHardwareName));
	for (auto record : pvRecordVec)
	{
		pvStruct* pv = new pvStruct;
		pv->fullPVName = currentHardwareName;
		pv->pvRecord = record;
		//chid, count, mask, chtype are left undefined for now.
		pvStructs.push_back(pv);
	}
	logger.printDebugMessage(std::string("Finished constructing: " + currentHardwareName));
}
std::string Hardware::getMachineArea()
{
	return this->machineArea;
}
std::string Hardware::getHardwareType()
{
	return this->hardwareType;
}
std::vector<pvStruct*> Hardware::getPVStructs()
{
	return this->pvStructs;
}
std::map<std::string, std::string> Hardware::getSpecificHardwareParameters()
{
	return this->specificHardwareParameters;
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
		for (size_t i = 0; i < LHSPVStructs.size(); i++)
		{
			if (LHSPVStructs.at(i)->fullPVName != RHSPVStructs.at(i)->fullPVName)
			{
				return false;
			}
		}
	}
	return true;
}
