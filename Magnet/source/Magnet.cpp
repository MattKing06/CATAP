#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <iostream>
#include <crtdbg.h>
#include "Magnet.h"
#include <map>
#include <vector>
#include "ConfigReader.h"
#include "boost\algorithm\string\split.hpp"



Magnet::Magnet()
{
	magnetParametersAndValuesMap = std::map<std::string, std::string>();
}

Magnet::Magnet(std::map<std::string, std::string> &paramsMap) : Hardware(paramsMap)
{
	//assuming known name
	fullPVName = paramsMap.find("name")->second;
	manufacturer = paramsMap.find("manufacturer")->second;
	serialNumber = std::stoi(paramsMap.find("serial_number")->second.data());
	magType = paramsMap.find("mag_type")->second;
	magRevType = paramsMap.find("mag_rev_type")->second;
	RI_tolerance = std::stof(paramsMap.find("RI_tolerance")->second);
	numberOfDegaussSteps = std::stoi(paramsMap.find("number_of_degauss_steps")->second);
	//convert list of degauss values from strings to floats
	std::vector<std::string> degaussValuesStrVec;
	boost::split(degaussValuesStrVec, paramsMap.find("degauss_values")->second, [](char c){return c == ','; });
	for (auto value : degaussValuesStrVec){ degaussValues.push_back(std::stof(value)); }
	degaussTolerance = std::stof(paramsMap.find("degauss_tolerance")->second);
	fullPSUName = paramsMap.find("PSU")->second;
	measurementDataLocation = paramsMap.find("measurement_data_location")->second;
	epicsInterface = new EPICSMagnetInterface();
	//_CrtDumpMemoryLeaks();
}

Magnet::~Magnet()
{
	delete this;
}
std::string Magnet::getFullPVName()
{
	return this->fullPVName;
}
std::vector<std::string> Magnet::getAliases()
{
	return this->aliases;
}
std::string Magnet::getManufacturer()
{
	return this->manufacturer;
}
int Magnet::getSerialNumber()
{
	return this->serialNumber;
}
std::string Magnet::getMagnetType()
{
	return this->magType;
}
std::string Magnet::getMagnetRevType()
{
	return this->magRevType;
}
double Magnet::getRITolerance()
{
	return this->RI_tolerance;
}
int Magnet::getNumberOfDegaussSteps()
{
	return this->numberOfDegaussSteps;
}
std::vector<double> Magnet::getDegaussValues()
{
	return this->degaussValues;
}
double Magnet::getDegaussTolerance()
{
	return this->degaussTolerance;
}
double Magnet::getMagneticLength()
{
	return this->magneticLength;
}
std::string Magnet::getFullPSUName()
{
	return this->fullPSUName;
}
std::string Magnet::getMeasurementDataLocation()
{
	return this->measurementDataLocation;
}
double Magnet::getCurrent()
{
	for (auto &pv = pvStructs.begin(); pv != pvStructs.end(); pv++)
	{
		if (pv->pvRecord == "READI")
		{
			double current = epicsInterface->getCurrent(pv->CHID);
			return current;
		}
	}
	return -1.0;
}