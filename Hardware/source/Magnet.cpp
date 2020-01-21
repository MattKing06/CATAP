#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <iostream>
#include "Magnet.h"
#include <map>
#include <vector>
#include "boost/algorithm/string/split.hpp"
#include <boost/make_shared.hpp>

#include "PythonTypeConversions.h"
#include "GlobalConstants.h"

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
	messenger = LoggingSystem(false, false);
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
boost::python::list Magnet::getAliases_Py() const
{
	return to_py_list<std::string>(getAliases());
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
double setRITolerance(const double value);


int Magnet::getNumberOfDegaussSteps() const
{
	return this->numberOfDegaussSteps;
}


std::vector<double> Magnet::getDegaussValues() const
{
	return this->degaussValues;
}
boost::python::list Magnet::getDegaussValues_Py() const
{
	return to_py_list<double>(getDegaussValues());
}
std::vector<double> Magnet::setDegaussValues(const std::vector<double>& values)
{
	degaussValues = values;
	numberOfDegaussSteps = (int)degaussValues.size();
	return degaussValues;
}
boost::python::list Magnet::setDegaussValues_Py(const boost::python::list& values)
{
	return to_py_list(setDegaussValues(to_std_vector<double>(values)));
}





double Magnet::getDegaussTolerance() const
{
	return this->degaussTolerance;
}
double Magnet::setDegaussTolerance(const double value)
{
	degaussTolerance = value;
	return degaussTolerance;
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
// PYTHON users call this function, then we decide what to do 
void Magnet::SETI(const double value)
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
void Magnet::SETIZero() // expposed to PYTHON
{
	SETI(GlobalConstants::zero_double);
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




bool Magnet::switchOn()const
{
	return 	epicsInterface->setNewPSUState(STATE::ON, pvStructs.at("SPOWER"));
}
bool Magnet::switchOFF()const
{
	return 	epicsInterface->setNewPSUState(STATE::OFF, pvStructs.at("SPOWER"));
}
STATE Magnet::getPSUState() const
{
	return psuState;
}
bool Magnet::setPSUState(const STATE value)const
{
	return 	epicsInterface->setNewPSUState(value, pvStructs.at("SPOWER"));
}


//void Magnet::setPSUState(const int & value)
//{
//	switch( value)
//	{
//	case GlobalConstants::zero_int: psuState = STATE:: OFF; break;
//	case GlobalConstants::one_int: psuState = STATE:: ON; break;
//	default: psuState = STATE::ERR;
//	}
//	messenger.printDebugMessage(hardwareName, " PSU STATE = ", ENUM_TO_STRING(psuState));
//}



//bool Magnet::setEPICSPSUState(const STATE value) const
//{
//	epicsInterface->setNewPSUState(value, pvStructs.at("SPOWER"))
//	//std::map<std::string, pvStruct>& pvData = getPVStructs();
//	//for (auto &pv : pvData)
//	//{
//	//	if (pv.second.pvRecord == "SPOWER")
//	//	{
//	//		this->epicsInterface->setNewPSUState(value, pv.second);
//	//	}
//	//}
//	return true;
//}






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

void Magnet::debugMessagesOff()
{
	messenger.printDebugMessage(hardwareName, " - DEBUG OFF");
	messenger.debugMessagesOff();
	epicsInterface->debugMessagesOff();
}

void Magnet::debugMessagesOn()
{
	messenger.debugMessagesOn();
	messenger.printDebugMessage(hardwareName, " - DEBUG ON");
	epicsInterface->debugMessagesOn();
}

void Magnet::messagesOff()
{
	messenger.printMessage(hardwareName, " - MESSAGES OFF");
	messenger.messagesOff();
	epicsInterface->messagesOff();
}

void Magnet::messagesOn()
{
	messenger.messagesOn();
	messenger.printMessage(hardwareName, " - MESSAGES ON");
	epicsInterface->messagesOn();
}



