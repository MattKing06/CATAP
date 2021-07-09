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
	//resetToZero(true),
	degaussTolerance(GlobalConstants::zero_point_one_double),
	wait_time(GlobalConstants::TIMET_45)
{}

Magnet::Magnet()
{}

Magnet::Magnet(const std::map<std::string, std::string>& paramsMap, STATE mode) :
	Hardware(paramsMap, mode),
	epicsInterface(boost::make_shared<EPICSMagnetInterface>(EPICSMagnetInterface())), // calls copy constructor and destroys 
	// Assumes all these find succeed ? 
	manufacturer(paramsMap.find("manufacturer")->second),
	serial_number(paramsMap.find("serial_number")->second.data()),
	//mag_type(paramsMap.find("mag_type")->second),
	RI_tolerance(std::stof(paramsMap.find("ri_tolerance")->second)),
	numberOfDegaussSteps(std::stoi(paramsMap.find("num_degauss_steps")->second)),
	degaussTolerance(std::stof(paramsMap.find("degauss_tolerance")->second)),
	magnetic_length(std::stof(paramsMap.find("magnetic_length")->second)),
	min_i(std::stof(paramsMap.find("min_i")->second)),
	max_i(std::stof(paramsMap.find("max_i")->second)),
	position(std::stof(paramsMap.find("position")->second)),
	GETSETI(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
	READI(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
	psu_state(std::make_pair(epicsTimeStamp(), STATE::UNKNOWN_STATE)),
	ilk_state(std::make_pair(epicsTimeStamp(), STATE::UNKNOWN_STATE)),
	is_degaussing(false)
{
	messenger.printDebugMessage("Magnet Constructor");
	for (auto&& item : paramsMap)
	{
		std::cout << item.first << " = " << item.second << std::endl;
	}

	epicsInterface->ownerName = hardwareName;
	setPVStructs();
	//convert list of degauss values from strings to floats
	std::vector<std::string> degaussValuesStrVec, field_integral_coefficients_STR;
	messenger.debugMessagesOn();
	boost::split(degaussValuesStrVec, paramsMap.find("degauss_values")->second, [](char c) {return c == ','; });
	boost::split(field_integral_coefficients_STR, paramsMap.find("field_integral_coefficients")->second, [](char c) {return c == ','; });
	for (auto value : degaussValuesStrVec)
	{
		degaussValues.push_back(std::stof(value));
	}
	for (auto value : field_integral_coefficients_STR)
	{
		field_integral_coefficients.push_back(std::stof(value));
	}
	//convert list of field integral coefficients from strings to floats
	boost::split(field_integral_coefficients_STR, paramsMap.find("field_integral_coefficients")->second, [](char c) {return c == ','; });
	for (auto value : field_integral_coefficients_STR)
	{
		field_integral_coefficients.push_back(std::stof(value));
	}

	// set the magnet_type
	// TODO THIS HAS NOW BEEN REPLACED BY A GLOBALCONSTANT FOR ALL HARDWARE TYPES
	if(GlobalFunctions::entryExists(paramsMap, "mag_type"))
	{
		if (GlobalFunctions::entryExists(GlobalConstants::stringToTypeMap, paramsMap.at("mag_type")))
		{
			mag_type = GlobalConstants::stringToTypeMap.at(paramsMap.at("mag_type"));
		}
		else
		{
			std::cout << "!!ERROR!! " << paramsMap.at("mag_type") << " is na unrecognized magnet type" << std::endl;
			mag_type = TYPE::UNKNOWN_TYPE;
		}

	}
	// convert value for YAML key "name_aliases", to vector of strings and set equal to memebr variable aliases
	// TODO name_alias should be in harwdare constructor?? 
	boost::split(aliases, paramsMap.find("name_alias")->second, [](char c) {return c == ','; });
	// convert yaml mag_type strings to CATAP.TYPE enum
	// epicsInterface = boost::make_shared<EPICSMagnetInterface>(EPICSMagnetInterface());
}
Magnet::Magnet(const Magnet& copyMagnet) :
	Hardware(copyMagnet),
	manufacturer(copyMagnet.manufacturer),
	serial_number(copyMagnet.serial_number),
	mag_type(copyMagnet.mag_type),
	magRevType(copyMagnet.magRevType),
	READI_tolerance(copyMagnet.READI_tolerance),
	numberOfDegaussSteps(copyMagnet.numberOfDegaussSteps),
	degaussValues(copyMagnet.degaussValues),
	fullPSUName(copyMagnet.fullPSUName),
	measurementDataLocation(copyMagnet.measurementDataLocation),
	aliases(copyMagnet.aliases),
	RI_tolerance(copyMagnet.RI_tolerance),
	magnetic_length(copyMagnet.magnetic_length),
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
	//std::string PV = specificHardwareParameters.find(record)->second.data();
	for (auto& record : MagnetRecords::magnetRecordList)
	{
		if (GlobalFunctions::entryExists(specificHardwareParameters, record))
		{
			std::string PV = specificHardwareParameters.find(record)->second.data();
			std::cout << "FOUND PV  = " + PV << std::endl;
			pvStructs[record] = pvStruct();
			pvStructs[record].pvRecord = record;
			// TODO NO ERROR CHECKING! (we assum config file is good??? 
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
				std::cout << "Physical Magnet PV " + pvStructs[record].fullPVName << std::endl;
			}
			//pv.pvRecord = record;
			//chid, count, mask, chtype are left undefined for now.
			//pvStructs[pv.pvRecord] = pv;
		}
	}
}



HardwareSnapshot Magnet::getSnapshot()
{
	currentSnapshot.update(MagnetRecords::GETSETI , GETSETI.second);
	currentSnapshot.update(MagnetRecords::READI, READI.second);
	currentSnapshot.update(MagnetRecords::RILK, ilk_state.second);
	currentSnapshot.update(MagnetRecords::RPOWER, psu_state.second);
	currentSnapshot.update(MagnetRecords::K_DIP_P, K_DIP_P.second);
	currentSnapshot.update(MagnetRecords::INT_STR_MM, INT_STR_MM.second);
	currentSnapshot.update(MagnetRecords::INT_STR, INT_STR.second);
	currentSnapshot.update(MagnetRecords::K_SET_P, K_SET_P.second);
	currentSnapshot.update(MagnetRecords::K_ANG, K_ANG.second);
	currentSnapshot.update(MagnetRecords::K_MRAD, K_MRAD.second);
	currentSnapshot.update(MagnetRecords::K_VAL, K_VAL.second);
	return currentSnapshot;
}
boost::python::dict Magnet::getSnapshot_Py()
{
	getSnapshot();
	return currentSnapshot.getSnapshot_Py();
}


bool Magnet::matchesSnapshot()const
{
//	if (isREADIequalValue(ms.readi, READI_tolerance))
//	{
//		if (ms.seti == getSETI())
//		{
//			if (ms.psu_state == getPSUState())
//			{
//				if (ms.psu_state == getIlkState())
//				{
//					return true;
//				}
//			}
//		}
//	}
	return false;
}

double Magnet::getMinI()const
{
	return min_i;
}
double Magnet::getMaxI()const
{
	return max_i;
}

std::string Magnet::getManufacturer() const
{
	return manufacturer;
}
std::string Magnet::getSerialNumber() const
{
	return serial_number;
}
TYPE Magnet::getMagType() const
{
	return mag_type;
}
double Magnet::getPosition() const
{
	return position;
}
TYPE Magnet::getMagnetType() const
{
	return mag_type;
}
std::string Magnet::getMagnetRevType() const
{
	return magRevType;
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

std::vector<double> Magnet::getFieldIntegralCoefficients() const
{
	return field_integral_coefficients;
}

boost::python::list Magnet::getFieldIntegralCoefficients_Py() const
{
	return to_py_list(getFieldIntegralCoefficients());
}


double Magnet::getMagneticLength() const
{
	return magnetic_length;
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

bool Magnet::degauss(double set_value_after_degauss)
{
	return 	degauss(degaussValues, set_value_after_degauss);
}

bool Magnet::degauss(const boost::python::list& custum_degauss_values, double set_value_after_degauss)
{
	return degauss(to_std_vector<double>(custum_degauss_values), set_value_after_degauss);
}
bool Magnet::degauss(const bool reset_to_zero)
{
	if (reset_to_zero)
	{
		return 	degauss(degaussValues, GlobalConstants::zero_double);
	}
	else
	{
		return 	degauss(degaussValues, GETSETI.second);
	}
}

bool Magnet::degauss(const std::vector<double>& degauss_values, double set_value_after_degauss)
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
		degausser.degauss_values = degauss_values;
		degausser.current_step = GlobalConstants::zero_sizet;
		//degausser.resetToZero = reset_to_zero;
		degausser.set_value_after_degauss = set_value_after_degauss;
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
	for (auto&& next_value : ds.magnet->degaussValues)
	{
		seti_success = ds.magnet->SETIZero();
		if (ds.magnet->waitForMagnetToSettle(GlobalConstants::zero_double, ds.degaussTolerance, ds.wait_time))
		{
			seti_success = ds.magnet->SETI(next_value); // expposed to PYTHON
			if (ds.magnet->waitForMagnetToSettle(next_value, ds.degaussTolerance, ds.wait_time))
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
	ds.magnet->SETI(ds.set_value_after_degauss);
	ds.magnet->last_degauss_success = degauss_success;
	ds.magnet->is_degaussing = false;
}
bool Magnet::isREADIequalValue(const double value) const
{
	return GlobalFunctions::areSame(value, READI_tolerance);
}
bool Magnet::isREADIequalValue(const double value, const double tolerance) const
{
	return GlobalFunctions::areSame(value, READI.second, tolerance);
}
bool Magnet::waitForMagnetToSettle(const double value, const double tolerance, const time_t waitTime) const
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
		if (isREADIequalValue(value, tolerance))
		{
			// Complete , we are where we want to be
			shouldBreak = true;
		}
		else if (settingZero) // We are supposed to be setting zero....
		{
			if (has_READI_settled)
			{
				messenger.printDebugMessage(hardwareName + " is setting 0.0 and READI_new == READI_old. SETTLED = True ");
				shouldBreak = true;
			}
		}
		else // we are not setting zero...
		{
			/*
				If we are not setting zero, and going through a polarity change then sometimes the magnet stays
				at zero for some time whilst the "polarity flipping happens" we have to check for this
			*/
			if (has_READI_settled && is_READI_zero)
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
		if (GlobalFunctions::timeNow() - timeStart > waitTime)
		{
			messenger.printDebugMessage("!!WARNING!! " + hardwareName + " has timed out waiting for READI to reach ",
				value, " with tolerance =  ", tolerance, ", READI = ", READI.second);
			timed_out = true;
			break;
		}
		if (shouldBreak)
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
	switch (mode)
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
STATE Magnet::getIlkState() const
{
	return ilk_state.second;
}
STATE Magnet::getPSUState()const
{
	// get PSU STATE
	return psu_state.second;
}

double Magnet::getKDipP() const
{
	return K_DIP_P.second;
}
double Magnet::getIntStr_mm() const
{
	return INT_STR_MM.second;
}
double Magnet::getIntStr() const
{
	return INT_STR.second;
}

double Magnet::getKSetP() const
{
	return K_SET_P.second;
}
double Magnet::getKAng() const
{
	return K_ANG.second;
}
double Magnet::getKmrad() const
{
	return K_MRAD.second;
}
double Magnet::getKVal() const
{
	return K_VAL.second;
}

bool Magnet::setKSetP(const double value)
{
	switch (mode)
	{
	case STATE::PHYSICAL:
		return epicsInterface->setNewKSetP(value, pvStructs.at(MagnetRecords::K_SET_P));
		break;
	case STATE::VIRTUAL:
		return epicsInterface->setNewKSetP(value, pvStructs.at(MagnetRecords::K_SET_P));
		break;
	default:
		offlineSETI(value);
	}
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
	case STATE::VIRTUAL:
		return 	epicsInterface->setNewPSUState(value, pvStructs.at(MagnetRecords::SPOWER));
	case STATE::OFFLINE:
		return offlineSetPSUState(value);
	}
	return false;
}
bool Magnet::offlineSetPSUState(const STATE value)
{
	epicsTimeGetCurrent(&psu_state.first);
	switch (value)
	{
	case STATE::ON:
		psu_state.second = value;
		break;
	case STATE::OFF:
		psu_state.second = value;
		break;
	default:
		psu_state.second = STATE::ERR;
	}
	return true;
}

bool Magnet::offlineSetIlkState(const STATE value)
{
	if (mode == STATE::OFFLINE)
	{
		epicsTimeGetCurrent(&ilk_state.first);
		switch (value)
		{
		case STATE::GOOD:
			ilk_state.second = value;
			break;
		case STATE::BAD:
			ilk_state.second = value;
			break;
		default:
			ilk_state.second = STATE::ERR;
		}
		return true;
	}
	return false;
}


bool Magnet::resetILK() const
{
	// TODO assumes this exists! 
	return epicsInterface->resetILK(pvStructs.at(MagnetRecords::ILK_RESET));
}

//std::map<std::string, std::string> Magnet::getState()
//{
//	magnetState currentState = getMagnetState();
//	std::map<std::string, std::string> stateMap = to_std_map<std::string, std::string>(currentState.state_dict);
//	return stateMap;
//}
//
//boost::python::dict Magnet::getState_Py()
//{
//	magnetState currentState = getMagnetState();
//	return currentState.state_dict;
//}

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



