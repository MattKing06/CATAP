#include "Magnet.h"
#include <vector>
#include "ConfigReader.h"

#include "boost\algorithm\string\split.hpp"

Magnet::Magnet(std::string knownNameOfMagnet) : Hardware()
{
	//assuming known name
	std::multimap<std::string, std::string> magnetParametersAndValuesMap = specificHardwareParameters;
	hardwareType = magnetParametersAndValuesMap.find("hardware_type")->second;
	fullPVName = magnetParametersAndValuesMap.find("name")->second;
	manufacturer = magnetParametersAndValuesMap.find("manufacturer")->second;
	serialNumber = std::atoi(magnetParametersAndValuesMap.find("serial_number")->second.c_str());
	magType = magnetParametersAndValuesMap.find("mag_type")->second;
	magRevType = magnetParametersAndValuesMap.find("mag_rev_type")->second;
	RI_tolerance = std::atof(magnetParametersAndValuesMap.find("RI_tolerance")->second.c_str());
	numberOfDegaussSteps = std::atoi(magnetParametersAndValuesMap.find("number_of_degauss_streps")->second.c_str());
	//convert list of degauss values from strings to floats
	std::vector<std::string> degaussValuesStrVec;
	boost::split(degaussValuesStrVec, magnetParametersAndValuesMap.find("degauss_values")->second, [](char c){return c == ','; });
	for (auto value : degaussValuesStrVec){ degaussValues.push_back(std::atof(value.c_str())); }
	degaussTolerance = std::atof(magnetParametersAndValuesMap.find("degauss_tolerance")->second.c_str());
	fullPSUName = magnetParametersAndValuesMap.find("PSU")->second;
	measurementDataLocation = magnetParametersAndValuesMap.find("measurement_data_location")->second;
}