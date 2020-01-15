#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <iostream>
#include "Magnet.h"
#include <map>
#include <vector>
#include "boost/algorithm/string/split.hpp"
#include <boost/make_shared.hpp>

//double Magnet::current;

Magnet::Magnet()
{}

//Magnet::Magnet(std::string knownNameOfMagnet):
//	Hardware(knownNameOfMagnet)
//{
//}

Magnet::Magnet(const std::map<std::string, std::string> &paramsMap, STATE mode) :
	Hardware(paramsMap, mode),
	// Assumes all these find succeed ? 
manufacturer(paramsMap.find("manufacturer")->second),
serialNumber(std::stoi(paramsMap.find("serial_number")->second.data())),
magType(paramsMap.find("mag_type")->second),
magRevType(paramsMap.find("mag_rev_type")->second),
RI_tolerance(std::stof(paramsMap.find("RI_tolerance")->second)),
numberOfDegaussSteps(std::stoi(paramsMap.find("number_of_degauss_steps")->second)),
degaussTolerance(std::stof(paramsMap.find("degauss_tolerance")->second)),
fullPSUName(paramsMap.find("PSU")->second),
measurementDataLocation(paramsMap.find("measurement_data_location")->second),
magneticLength(std::stof(paramsMap.find("magnetic_length")->second)),
GETSETI(GlobalConstants::double_min),//this needs to be lower limits
psuState(STATE::ERR),
READI(GlobalConstants::double_min),
ilkState(STATE::ERR)
{
	//convert list of degauss values from strings to floats
	std::vector<std::string> degaussValuesStrVec;
	boost::split(degaussValuesStrVec, paramsMap.find("degauss_values")->second, [](char c){return c == ','; });
	for (auto value : degaussValuesStrVec){ degaussValues.push_back(std::stof(value)); }
	epicsInterface = boost::make_shared<EPICSMagnetInterface>(EPICSMagnetInterface());
	epicsInterface->ownerName = hardwareName;
}
Magnet::Magnet(const Magnet& copyMagnet) : Hardware(copyMagnet),
manufacturer(copyMagnet.manufacturer), serialNumber(copyMagnet.serialNumber),
magType(copyMagnet.magType), magRevType(copyMagnet.magRevType), RI_tolerance(copyMagnet.RI_tolerance),
numberOfDegaussSteps(copyMagnet.numberOfDegaussSteps), degaussValues(copyMagnet.degaussValues),
fullPSUName(copyMagnet.fullPSUName), measurementDataLocation(copyMagnet.measurementDataLocation),
magneticLength(copyMagnet.magneticLength), epicsInterface(copyMagnet.epicsInterface)
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



double Magnet::getSETI() const
{
	return GETSETI;
}


// THIS IS "SETTING" A SETI, WE NEVER READ SET WE USE GETSETI
// Python users call this function, then we decide what to do 
void Magnet::SETI(const double& value)
{
	switch (mode)
	{
		case STATE::PHYSICAL: 	
			setEPICSSETI(value);
			break;
		case STATE::VIRTUAL: 	
			setEPICSSETI(value);
			break;
		default: 
			offlineSETI(value);
	}
}

void Magnet::offlineSETI(const double& value)
{
	setREADI(value);
	updateGETSETI(value);
}


void Magnet::updateGETSETI(const double& value)
{
	//setREADI(value);
	GETSETI = value;
}
void Magnet::setEPICSSETI(const double &value)
{
	epicsInterface->setNewCurrent(value, pvStructs.at("SETI"));
	//
	//std::map<std::string, pvStruct>& pvData = getPVStructs();
	//for (auto &pv : pvData)
	//{
	//	if (pv.second.pvRecord == "SETI")
	//	{
	//		this->epicsInterface->setNewCurrent(value, pv.second);
	//	}
	//}
	//// subscription should sense current has changed and call 'updateCurrent' in MagnetEPICSInterface
	//return true;
}


void Magnet::setREADI(const double& value)
{
	/*
		This funciton is ONLY ever called to update the READI value (i.e from EPICS) 
	*/
	READI = value;
	messenger.printDebugMessage(hardwareName, " READI Value:", value);
	//return true;
}

double Magnet::getREADI() const
{
	return READI;
}





bool Magnet::setPSUState(const STATE& value)
{
	psuState = value;
	messenger.printDebugMessage(hardwareName, " PSU STATE:", value);
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
			this->epicsInterface->setNewPSUState(value, pv.second);
		}
	}
	return true;
}






bool Magnet::setILKState(const STATE& value)
{
	ilkState = value;
	messenger.printDebugMessage(hardwareName, " ILK State:", value);
	return true;
}
STATE Magnet::getILKState() const
{
	return ilkState;
}


