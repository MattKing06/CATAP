#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <iostream>
#include "Magnet.h"
#include <map>
#include <vector>
#include "ConfigReader.h"
#include "boost/algorithm/string/split.hpp"
#include <boost/make_shared.hpp>

Magnet::Magnet()
{}

Magnet::Magnet(const std::map<std::string, std::string> &paramsMap, bool isVirtual) :
Hardware(paramsMap, isVirtual),
manufacturer(paramsMap.find("manufacturer")->second),
serialNumber(std::stoi(paramsMap.find("serial_number")->second.data())),
magType(paramsMap.find("mag_type")->second),
magRevType(paramsMap.find("mag_rev_type")->second),
RI_tolerance(std::stof(paramsMap.find("RI_tolerance")->second)),
numberOfDegaussSteps(std::stoi(paramsMap.find("number_of_degauss_steps")->second)),
degaussTolerance(std::stof(paramsMap.find("degauss_tolerance")->second)),
fullPSUName(paramsMap.find("PSU")->second),
measurementDataLocation(paramsMap.find("measurement_data_location")->second),
magneticLength(std::stof(paramsMap.find("magnetic_length")->second))
{
	//convert list of degauss values from strings to floats
	std::vector<std::string> degaussValuesStrVec;
	boost::split(degaussValuesStrVec, paramsMap.find("degauss_values")->second, [](char c){return c == ','; });
	for (auto value : degaussValuesStrVec){ degaussValues.push_back(std::stof(value)); }
	//BAD NEW: needs to be changed to shared_ptr/unique_ptr
	epicsInterface = boost::make_shared<EPICSMagnetInterface>(EPICSMagnetInterface());
}
Magnet::Magnet(const Magnet& copyMagnet) : Hardware(copyMagnet),
manufacturer(copyMagnet.manufacturer), serialNumber(copyMagnet.serialNumber),
magType(copyMagnet.magType), magRevType(copyMagnet.magRevType), RI_tolerance(copyMagnet.RI_tolerance),
numberOfDegaussSteps(copyMagnet.numberOfDegaussSteps), degaussValues(copyMagnet.degaussValues),
fullPSUName(copyMagnet.fullPSUName), measurementDataLocation(copyMagnet.measurementDataLocation),
epicsInterface(copyMagnet.epicsInterface), magneticLength(copyMagnet.magneticLength)
{
}
std::vector<std::string> Magnet::getAliases() const
{
	return this->aliases;
}
std::string Magnet::getManufacturer() const
{
	return this->manufacturer;
}
int Magnet::getSerialNumber() const
{
	return this->serialNumber;
}
std::string Magnet::getMagnetType() const
{
	return this->magType;
}
std::string Magnet::getMagnetRevType() const
{
	return this->magRevType;
}
double Magnet::getRITolerance() const
{
	return this->RI_tolerance;
}
int Magnet::getNumberOfDegaussSteps() const
{
	return this->numberOfDegaussSteps;
}
std::vector<double> Magnet::getDegaussValues() const
{
	return this->degaussValues;
}
double Magnet::getDegaussTolerance() const
{
	return this->degaussTolerance;
}
double Magnet::getMagneticLength() const
{
	return this->magneticLength;
}
std::string Magnet::getFullPSUName() const
{
	return this->fullPSUName;
}
std::string Magnet::getMeasurementDataLocation() const
{
	return this->measurementDataLocation;
}
double Magnet::getCurrent() const
{
	return this->current;
}
bool Magnet::setEPICSCurrent(const double &value)
{
	std::map<std::string, pvStruct>& pvData = getPVStructs();
	for (auto &pv : pvData)
	{
		if (pv.second.pvRecord == "SETI")
		{
			messenger.printDebugMessage("SETTING TO VALUE: " + std::to_string(value) + " for [" + pv.second.fullPVName + "]");
			this->epicsInterface->setNewCurrent(value, pv.second);
		}
	}
	// subscription should sense current has changed and call 'updateCurrent' in MagnetEPICSInterface
	return true;
}

bool Magnet::setPSUState(const STATE& value)
{
	psuState = value;
	return true;
}

STATE Magnet::getPSUState() const
{
	return psuState;
}

bool Magnet::setEPICSPSUState(const STATE& value)
{
	std::map<std::string, pvStruct>& pvData = getPVStructs();
	for (auto &pv : pvData)
	{
		if (pv.second.pvRecord == "SPOWER")
		{
			messenger.printDebugMessage("SETTING TO POWER: " + std::to_string(value) + " for [" + pv.second.fullPVName + "]");
			this->epicsInterface->setNewPSUState(value, pv.second);
		}
	}
	return true;
}

bool Magnet::setRICurrent(const double& value)
{
	RICurrent = value;
	return true;
}

double Magnet::getRICurrent() const
{
	return RICurrent;
}

bool Magnet::setILKState(const int& value)
{
	ilkState = value;
	return true;
}

int Magnet::getILKState() const
{
	return ilkState;
}

bool Magnet::setCurrent(const double &value)
{
	this->current = value;
	return true;
}