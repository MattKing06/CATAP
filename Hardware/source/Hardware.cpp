#include "Hardware.h"

Hardware::Hardware()
{
	machineArea = "UNKNOWN";
	hardwareType = "UNKNOWN";
	specificHardwareParameters = std::multimap<std::string, std::string>();
}

Hardware::Hardware(std::multimap<std::string, std::string> specificValueMap)
{
	logger = new LoggingSystem(true, true);
	//std::string YAMLConfigDirectory = "C:\\Users\\ujo48515\\Documents\\YAMLParserTestFiles";
	std::string currentHardwareName = specificValueMap.find("name")->second.data();
	logger->printDebugMessage(std::string("Constructing Hardware" + currentHardwareName));

	//ConfigReader configReader(currentHardwareName);
	specificHardwareParameters = specificValueMap;
	machineArea = specificValueMap.find("machine_area")->second;
	hardwareType = specificValueMap.find("hardware_type")->second;
	
	// equal_range returns a variable containing start (first) and end (second)
	// iterators for items in the multimap corresponding to pv records.
	auto pvRecords = specificHardwareParameters.equal_range(currentHardwareName);
	// iterate through the list of matches and set up a pvStruct to add to pvStructs.
	logger->printDebugMessage(std::string("Constructing PV information for " + currentHardwareName));
	for (auto record = pvRecords.first; record != pvRecords.second; record++)
	{
		pvStruct pv;
		pv.fullPVName = record->first.data();
		pv.pvRecord = record->second.data();
		//chid, count, mask, chtype are left undefined for now.
		pvStructs.push_back(pv);
	}
	logger->printDebugMessage(std::string("Finished constructing: " + currentHardwareName));
}
std::string Hardware::getMachineArea()
{
	return this->machineArea;
}
std::string Hardware::getHardwareType()
{
	return this->hardwareType;
}
std::vector<pvStruct> Hardware::getPVStructs()
{
	return this->pvStructs;
}
std::multimap<std::string, std::string> Hardware::getSpecificHardwareParameters()
{
	return this->specificHardwareParameters;
}