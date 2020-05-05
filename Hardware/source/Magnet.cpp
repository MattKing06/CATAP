#define _CRTDBG_MAP_ALLOC // TODO: WHAT IS THIS??? 
#include <stdlib.h>
#include <iostream>
#include "Magnet.h"
#include <map>
#include <vector>
#include <functional>
// boost
#include "boost/algorithm/string/split.hpp"
#include <boost/make_shared.hpp>
// CATAP includes
#include "PythonTypeConversions.h"
#include "GlobalConstants.h"
#include "GlobalFunctions.h"

// map to convert yaml file strings to magnet TYPE enums
//const std::map<std::string, TYPE> Magnet::magnet_string_to_type_map = Magnet::create_map();


Degauss::Degauss() :
magnet(nullptr), 
degauss_thread(nullptr),
current_step(GlobalConstants::zero_sizet),
resetToZero(true),
degaussTolerance(GlobalConstants::zero_point_one_double),
wait_time(GlobalConstants::TIMET_45)
{}

Magnet::Magnet()
{}
Magnet::Magnet(const std::map<std::string, std::string> &paramsMap, STATE mode) :
Hardware(paramsMap, mode),
epicsInterface(boost::make_shared<EPICSMagnetInterface>(EPICSMagnetInterface())), // calls copy constructor and destroys 
// Assumes all these find succeed ? 
manufacturer(paramsMap.find("manufacturer")->second),
serialNumber(paramsMap.find("serial_number")->second.data()),
//magType(paramsMap.find("mag_type")->second),
RI_tolerance(std::stof(paramsMap.find("ri_tolerance")->second)),
numberOfDegaussSteps(std::stoi(paramsMap.find("num_degauss_steps")->second)),
degaussTolerance(std::stof(paramsMap.find("degauss_tolerance")->second)),
magneticLength(std::stof(paramsMap.find("magnetic_length")->second)),
min_i(std::stof(paramsMap.find("min_i")->second)),
max_i(std::stof(paramsMap.find("max_i")->second)),
GETSETI( std::make_pair(epicsTimeStamp(), GlobalConstants::double_min) ),
READI( std::make_pair(epicsTimeStamp(), GlobalConstants::double_min) ),
psuState( std::make_pair(epicsTimeStamp(), STATE::ERR) ),
ilkState(std::make_pair(epicsTimeStamp(), STATE::ERR) ),
is_degaussing(false)
{
	messenger.printDebugMessage("Magnet Constructor");
	epicsInterface->ownerName = hardwareName;
	setPVStructs();
	//convert list of degauss values from strings to floats
	std::vector<std::string> degaussValuesStrVec;
	messenger.debugMessagesOn();
	boost::split(degaussValuesStrVec, paramsMap.find("degauss_values")->second, [](char c){return c == ','; });
	for (auto value : degaussValuesStrVec) 
	{ 
		degaussValues.push_back(std::stof(value)); 
	}
	// set the magnet_type
	if(GlobalFunctions::entryExists(paramsMap, "mag_type"))
	{
		if(GlobalFunctions::entryExists(GlobalConstants::stringToTypeMap, paramsMap.at("mag_type")))
		{
			magType = GlobalConstants::stringToTypeMap.at(paramsMap.at("mag_type"));
		}
		else
		{
			magType = TYPE::UNKNOWN_TYPE;
		}

	}
	// convert value for YAML key "name_aliases", to vector of strings and set equal to memebr variable aliases
	boost::split(aliases, paramsMap.find("name_alias")->second, [](char c) {return c == ','; });
	// convert yaml mag_type strings to CATAP.TYPE enum
	// epicsInterface = boost::make_shared<EPICSMagnetInterface>(EPICSMagnetInterface());
}
Magnet::Magnet(const Magnet& copyMagnet) : Hardware(copyMagnet),
manufacturer(copyMagnet.manufacturer), 
serialNumber(copyMagnet.serialNumber),
magType(copyMagnet.magType),
magRevType(copyMagnet.magRevType), 
READI_tolerance(copyMagnet.READI_tolerance),
numberOfDegaussSteps(copyMagnet.numberOfDegaussSteps), 
degaussValues(copyMagnet.degaussValues),
fullPSUName(copyMagnet.fullPSUName), 
measurementDataLocation(copyMagnet.measurementDataLocation),
aliases(copyMagnet.aliases),
RI_tolerance(copyMagnet.RI_tolerance),
magneticLength(copyMagnet.magneticLength), 
epicsInterface(copyMagnet.epicsInterface)
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


void Magnet::setPVStructs()
{
	for (auto&& record : MagnetRecords::magnetRecordList)
	{
		pvStructs[record] = pvStruct();
		pvStructs[record].pvRecord = record;
		
		// TODO NO ERROR CHECKING! (we assum config file is good??? 
		std::string PV = specificHardwareParameters.find(record)->second.data();
		// iterate through the list of matches and set up a pvStruct to add to pvStructs.
		//messenger.printDebugMessage("Constructing PV information for ", record);

		/*TODO
		  This should be put into some general function: generateVirtualPV(PV) or something...
		  Unless virtual PVs are to be included in the YAML files, they can be dealt with on
		  The config reader level if that is the case.
		  DJS maybe they should, how certian can we be all virtual PVs will get a VM- prefix??? 
		  */
		if (mode == STATE::VIRTUAL)
		{
			pvStructs[record].fullPVName = "VM-" + PV;
			std::cout << "Virtual Magnet PV " + pvStructs[record].fullPVName << std::endl;
		}
		else
		{
			pvStructs[record].fullPVName = PV;
			std::cout << "Magnet PV " + pvStructs[record].fullPVName << std::endl;
		}
		//pv.pvRecord = record;
		//chid, count, mask, chtype are left undefined for now.
		//pvStructs[pv.pvRecord] = pv;
	}

}

magnetState Magnet::getMagnetState()const
{
	magnetState r;
	r.readi = getREADI();
	r.seti = getSETI();
	r.ilkState = getILKState();
	r.psuState = getPSUState();
	r.name = getHardwareName();
	return r;
}

bool Magnet::setMagnetState(const magnetState& ms)
{
	bool seti_sent = SETI(ms.seti);
	GlobalFunctions::pause_50;
	bool psu_sent = setPSUState(ms.psuState);
	if (seti_sent && psu_sent)
	{
		return true;
	}
	return false;
}

bool Magnet::isInState(const magnetState& ms)const
{
	if (isREADIequalValue(ms.readi, READI_tolerance))
	{
		if (ms.seti == getSETI())
		{
			if (ms.psuState == getPSUState())
			{
				if (ms.psuState == getILKState())
				{
					return true;
				}
			}
		}
	}
	return false;
}

bool Magnet::isInSETIandPSUState(const magnetState& ms)const
{
	if (ms.seti == getSETI())
	{
		if (ms.psuState == getPSUState())
		{
			return true;
		}
	}
	return false;
}

double Magnet::getMinI()const
{
	return this->min_i;
}
double Magnet::getMaxI()const
{
	return this->max_i;
}

std::string Magnet::getManufacturer() const
{
	return this->manufacturer;
}
std::string Magnet::getSerialNumber() const
{
	return this->serialNumber;
}
TYPE Magnet::getMagnetType() const
{
	return magType;
}
std::string Magnet::getMagnetRevType() const
{
	return this->magRevType;
}
double Magnet::getREADITolerance() const
{
	return READI_tolerance;
}
double Magnet::setREADITolerance(const double value)
{
	READI_tolerance = value;
	return READI_tolerance;
}
size_t Magnet::getNumberOfDegaussSteps() const
{
	return numberOfDegaussSteps;
}

std::vector<double> Magnet::getDegaussValues() const
{
	return degaussValues;
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
	return degaussTolerance;
}
double Magnet::setDegaussTolerance(const double value)
{
	degaussTolerance = value;
	return degaussTolerance;
}


double Magnet::getMagneticLength() const
{
	return magneticLength;
}
std::string Magnet::getFullPSUName() const
{
	return fullPSUName;
}
std::string Magnet::getMeasurementDataLocation() const
{
	return measurementDataLocation;
}

bool Magnet::isDegaussing()const
{
	return is_degaussing;
}

bool Magnet::degauss(const bool reset_to_zero)
{
	if (is_degaussing)
	{

	}
	else
	{
		#ifdef _WIN32
			//std::cout << hardwareName << " is NOT DEGAUSSING!! " << std::endl;
			if (degausser.degauss_thread)
			{
				//std::cout << hardwareName << " Kill degauss thread " << std::endl;
				degausser.degauss_thread->join();
				delete degausser.degauss_thread;
				degausser.degauss_thread = nullptr;
			}
			is_degaussing = true;
			degausser.degauss_values = degaussValues;
			degausser.current_step = GlobalConstants::zero_sizet;
			degausser.resetToZero = reset_to_zero;
			degausser.degaussTolerance = degaussTolerance;
			degausser.magnet = this;
			//degausser.wait_time = this; set on constructor for now 
			//std::cout << hardwareName << " START NEW degauss thread " << std::endl;
			degausser.degauss_thread = new std::thread(staticEntryDeGauss, std::ref(degausser));
			return true;
		#endif //WIN32
	}
	return false;
}
void Magnet::staticEntryDeGauss(const Degauss& ds)
{
	ds.magnet->epicsInterface->attachTo_thisCaContext();
	// Degauss procedures:
	// set zero, wait, set next value,
	// if at any stage waitForMagnetToSettle returns false, we have failed and exit
	bool degauss_success = true;
	bool seti_success;
	ds.magnet->current_degauss_step = GlobalConstants::zero_sizet;
	for(auto&& next_value: ds.magnet->degaussValues)
	{
		seti_success = ds.magnet->SETIZero(); 
		if(ds.magnet->waitForMagnetToSettle(GlobalConstants::zero_double, ds.degaussTolerance, ds.wait_time))
		{
			seti_success = ds.magnet->SETI(next_value ); // expposed to PYTHON
			if (ds.magnet->waitForMagnetToSettle(next_value, ds.degaussTolerance, ds.wait_time) )
			{
			}
			else
			{
				ds.magnet->messenger.printDebugMessage(ds.magnet->hardwareName + " ERROR degaussing magnet did not settle");
				degauss_success = false;
				break;
			}
		}
		ds.magnet->current_degauss_step += GlobalConstants::one_sizet;
	}
	if(ds.resetToZero)
	{
		ds.magnet->SETIZero();
	}

	ds.magnet->last_degauss_success = degauss_success;
	ds.magnet->is_degaussing = false;
}


bool Magnet::isREADIequalValue(const double value, const double tolerance ) const
{
	return GlobalFunctions::areSame(value, READI.second, tolerance);
}


bool Magnet::waitForMagnetToSettle(const double value,	const double tolerance,	const time_t waitTime) const
{
	/*
		This is surprisingly complicated.
		As there are NO guarantees that RI will get close to SI,
		we have to check if RI is changing.
		Also, we have to check if RI is supposed to be zero,
		because sometimes we hang around at zero while flipping
		before setting the SI value we actually want
		I've done the conditional flags very verbose
		just to make sure i follow what I'm doing
	*/
	bool timeOut = false;

	double oldRIValue = GlobalConstants::double_min;
	/*
		To test if we are supposed to be setting zero.
		have a vector of bool indicating if they actually ARE being set to zero
	*/
	bool settingZero = value == GlobalConstants::zero_double;
	// we also need to monitor time, and timeout if things take too long
	time_t timeStart = GlobalFunctions::timeNow();
	bool timed_out = false;
	while (true)
	{
		// current READI value
		double currentRIValue = READI.second;
		// if the old value is the same as the current value, lets assume READI has settled
		bool has_READI_settled = GlobalFunctions::areSame(oldRIValue, currentRIValue, tolerance);
		// is the READI value zero?
		bool is_READI_zero = GlobalFunctions::areSame(GlobalConstants::zero_double, currentRIValue, tolerance);
		// set SHOULD break to true
		bool shouldBreak = false;
		messenger.printDebugMessage("currentRIValue = ", currentRIValue, " oldRIValue = ", oldRIValue, " has_READI_settled = ", has_READI_settled, " is_READI_zero ", is_READI_zero);
		// The acid test! is if READI is equal value
		if(isREADIequalValue(value, tolerance) )
		{
			// Complete , we are where we want to be
			shouldBreak = true;
		}
		else if (settingZero) // We are supposed to be setting zero....
		{
			if(has_READI_settled)
			{
				messenger.printDebugMessage(hardwareName +  " is setting 0.0 and READI_new == READI_old. SETTLED = True ");
				shouldBreak = true;
			}
		}
		else // we are not setting zero...
		{
			/*
				If we are not setting zero, and going through a polarity change then sometimes the magnet stays 
				at zero for some time whilst the "polarity flipping happens" we have to check for this 
			*/
			if(has_READI_settled && is_READI_zero )
			{
				/*
					READI is not changing and it's at zero, but zero isn't the final READI value we expect.
					so don't breka out of this loop
				*/
			}
			else if (has_READI_settled && !is_READI_zero)
			{
				/* we are NOT setting zero, READI has settled and READI has not settled to zero, this is success */
				//debugMessage(mags[i],
				//	" RI_new != 0.0&& RI_new == RI_old RI. SETTLED = True ",
				//	currentRIValues[i], ", ", oldRIValues[i],
				//	", ", tolerances[i]);
				messenger.printDebugMessage(hardwareName + " Not setting zero and READI has settled NOT at zero. I think we have settled correctly.");
				shouldBreak = true;
			}
		}
		/* check if time ran out */
		if( GlobalFunctions::timeNow() - timeStart > waitTime)
		{
			messenger.printDebugMessage("!!WARNING!! " + hardwareName + " has timed out waiting for READI to reach ", 
				value, " with tolerance =  ", tolerance, ", READI = ", READI.second);
			timed_out = true;
			break;
		}
		if(shouldBreak)
		{
			messenger.printDebugMessage(hardwareName + " setting READI = ", value, " with tolerance =  ", 
				tolerance, ", has probably settled, READI = ", READI.second);
			break;
		}
		/* save current values to see check if settled */
		oldRIValue = currentRIValue;
		/*  really 2000 milliseconds while we wait for RI to update>>>> ?
			YES!!
		*/
		GlobalFunctions::pause_2000();
	} /// while

	return !timed_out;
	
}




// THIS IS "SETTING" A SETI, WE NEVER READ SET WE USE GETSETI
// PYTHON users call this function, then we decide what to do 
bool Magnet::SETI(const double value)
{
	switch(mode)
	{
		case STATE::PHYSICAL: 	
			return epicsInterface->setNewCurrent(value, pvStructs.at(MagnetRecords::SETI));
			break;
		case STATE::VIRTUAL: 	
			return epicsInterface->setNewCurrent(value, pvStructs.at(MagnetRecords::SETI));
			break;
		default: 
			offlineSETI(value);
	}
	return true;
}
bool Magnet::SETIZero() // expposed to PYTHON
{
	return SETI(GlobalConstants::zero_double);
}


void Magnet::offlineSETI(const double& value)
{
	epicsTimeGetCurrent(&GETSETI.first);
	READI.first = GETSETI.first;
	READI.second = value;
	GETSETI.second = value;
}




double Magnet::getSETI() const
{
	return GETSETI.second;
}

double Magnet::getREADI() const
{
	return READI.second;
}
STATE Magnet::getILKState() const
{
	return ilkState.second;
}
STATE Magnet::getPSUState()const
{
	// get PSU STATE
	return psuState.second;
}


// apply new state, 
bool Magnet::switchOn()
{
	//messenger.printDebugMessage("switchOn() " + hardwareName);
	return 	setPSUState(STATE::ON);
}
bool Magnet::switchOff()
{
	return 	setPSUState(STATE::OFF);
}
bool Magnet::setPSUState(const STATE value)
{
	switch (mode)
	{
	case STATE::PHYSICAL:
		messenger.printDebugMessage(ENUM_TO_STRING(value), " PSU " + hardwareName);
		return 	epicsInterface->setNewPSUState(value, pvStructs.at(MagnetRecords::SPOWER));
		break;
	case STATE::VIRTUAL:
		return 	epicsInterface->setNewPSUState(value, pvStructs.at(MagnetRecords::SPOWER));
		break;
	case STATE::OFFLINE:
		return offlineSetPSUState(value);
	}
	return false;
}
bool Magnet::offlineSetPSUState(const STATE value)
{
	epicsTimeGetCurrent(&psuState.first);
	switch(value)
	{
		case STATE::ON:		
			psuState.second = value;		
			break;
		case STATE::OFF:	
			psuState.second = value;
			break;
		default: 
			psuState.second = STATE::ERR;
	}
	return true;
}

bool Magnet::offlineSetILKState(const STATE value)
{
	if( mode == STATE::OFFLINE)
	{
		epicsTimeGetCurrent(&ilkState.first);
		switch (value)
		{
			case STATE::GOOD:
				ilkState.second = value;
				break;
			case STATE::BAD:
				ilkState.second = value;
				break;
			default:
				psuState.second = STATE::ERR;
		}
		return true;
	}
	return false;
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



