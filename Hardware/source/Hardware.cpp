#include "Hardware.h"

Hardware::Hardware()
{
	logger = new LoggingSystem(true, true);
	logger->printDebugMessage(std::string("Constructing Hardware"));
	std::string YAMLConfigDirectory = "C:\\Users\\ujo48515\\Documents\\YAMLParserTestFiles";
	std::string currentHardwareName = "CLA-C2V-MAG-HCOR-01";
	ConfigReader configReader(currentHardwareName);
	specificHardwareParameters = configReader.parseYamlFile();
	machineArea = specificHardwareParameters.find("machine_area")->second;
	hardwareType = specificHardwareParameters.find("hardware_type")->second;
	// equal_range returns a variable containing start (first) and end (second)
	// iterators for items in the multimap corresponding to pv records.
	auto pvRecords = specificHardwareParameters.equal_range(currentHardwareName);
	// iterate through the list of matches and set up a pvStruct to add to pvStructs.
	for (auto record = pvRecords.first; record != pvRecords.second; record++)
	{
		pvStruct pv;
		pv.fullPVName = record->first;
		pv.pvRecord = record->second;
		//chid, count, mask, chtype are left undefined for now.
		pvStructs.push_back(pv);
	}
}
