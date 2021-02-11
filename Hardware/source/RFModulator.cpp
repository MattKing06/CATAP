#pragma once
#include <RFModulator.h>
#include <GlobalFunctions.h>
#include "boost/algorithm/string/split.hpp"

RFModulator::RFModulator()
{
}

RFModulator::RFModulator(const std::map<std::string, std::string>& paramMap, STATE mode) :
Hardware(paramMap, mode),
is_warm(false),
epicsInterface(boost::make_shared<EPICSRFModulatorInterface>(EPICSRFModulatorInterface()))
{
	messenger.printMessage("setMasterLatticeData data for: ", hardwareName);
	setMasterLatticeData();
	messenger.printMessage("constructing PV data for: ", hardwareName);
	setPVStructs();
}
RFModulator::RFModulator(const RFModulator& copyRFModulator)
	//:
	//good_gun_hex_codes(copyRFModulator.good_gun_hex_codes)
{
	good_gun_hex_codes = copyRFModulator.good_gun_hex_codes;
}
RFModulator::~RFModulator(){}



void RFModulator::setMasterLatticeData()
{
	std::pair< epicsTimeStamp, std::string> temp;
	temp.first  = epicsTimeStamp();
	temp.second = GlobalConstants::UNKNOWN;
	for (auto&& it : RFModulatorRecords::low_level_strings)
	{
		if (GlobalFunctions::entryExists(specificHardwareParameters, it))
		{
			low_level_strings[it] = temp;

			messenger.printDebugMessage(hardwareName, " added \"", it, "\" to low_level_strings");
		}
	}
	std::pair< epicsTimeStamp, double> temp2;
	temp2.first = epicsTimeStamp();
	temp2.second = GlobalConstants::double_min;
	for (auto&& it : RFModulatorRecords::low_level_values)
	{
		if (GlobalFunctions::entryExists(specificHardwareParameters, it))
		{
			low_level_values[it] = temp2;
			messenger.printDebugMessage(hardwareName, " added \"", it, "\" to low_level_values");

		}
	}
	if (GlobalFunctions::entryExists(specificHardwareParameters, "name_alias"))
	{
		// TODOD white space trimiming probably required here for robustness 
		boost::split(aliases, specificHardwareParameters.find("name_alias")->second, [](char c) {return c == ','; });
		for (auto&& next_alias : aliases)
		{
			messenger.printMessage("Found name alias = \"", next_alias, "\"");
		}

	}
}
void RFModulator::setPVStructs()
{
	for (auto&& record : RFModulatorRecords::rf_mod_records_list)
	{
		if (GlobalFunctions::entryExists(specificHardwareParameters, record))
		{
			messenger.printDebugMessage(hardwareName, " found ", record);
			pvStructs[record] = pvStruct();
			pvStructs[record].pvRecord = record;
			std::string PV = specificHardwareParameters.find(record)->second;
			switch (mode)
			{
			case(STATE::VIRTUAL):
				pvStructs[record].fullPVName = "VM-" + PV;
				break;
			case(STATE::PHYSICAL):
				pvStructs[record].fullPVName = PV;
				break;
			default:
				pvStructs[record].fullPVName = PV;
				break;
			}
		}

	}
}
// Name alies 
std::vector<std::string> RFModulator::getAliases() const
{
	return aliases;
}
boost::python::list RFModulator::getAliases_Py() const
{
	return to_py_list<std::string>(getAliases());
}

void RFModulator::updateLowLevelString(const std::string& key, const std::pair < epicsTimeStamp, std::string>& value)
{
	//messenger.printDebugMessage("updateLowLevelString");
	//messenger.printDebugMessage(hardwareName, " low_level_strings ");
	//for (auto&& it : low_level_strings)
	//{
	//	messenger.printDebugMessage(it.first);
	//}
	//
	//low_level_strings.at(key) = value;

	if (GlobalFunctions::entryExists(low_level_strings, key))
	{
		low_level_strings.at(key) = value;
		//messenger.printMessage("updateLowLevelString ", hardwareName, " ", key, " = ", value.second);
		//messenger.printMessage("string length  = ", value.second.length());
	}
	else
	{
		messenger.printDebugMessage("!!ERROR!! updateLowLevelString ", hardwareName, " passed: \"", key, "\" a key that does not exist");
	}
}
void RFModulator::updateLowLevelDouble(const std::string& key, const std::pair < epicsTimeStamp, double>&  value)
{
	if (GlobalFunctions::entryExists(low_level_values, key))
	{
		low_level_values.at(key) = value;
		//messenger.printMessage("updateLowLevelDouble ", hardwareName, " ", key, " = ", value.second);
	}
	else
	{
		messenger.printDebugMessage("!!ERROR!! updateLowLevelDouble ", hardwareName, " passed: \"", key, "\" a key that does not exist");
	}
}
void RFModulator::updateRemainingWarmUpTime(const std::pair < epicsTimeStamp, long>& value)
{
	// todo need to find out what the linac is like for this 
	wam_up_time_remaining = value;
	if (wam_up_time_remaining.second == GlobalConstants::zero_long)
	{
		//if the mode is standby or better??
		is_warm = true;
		//message("Gun Modulator Safely Warmed Up:");
	}
	else
	{
		is_warm = false;
	}
}


std::map<std::string, double> RFModulator::getLowLevelNumercialData()const
{
	std::map < std::string, double> low_level_values_r;
	for (auto&& it : low_level_values)
	{
		low_level_values_r[it.first] = it.second.second;
	}
	return low_level_values_r;
}
boost::python::dict RFModulator::getLowLevelNumercialData_Py()const
{
	return to_py_dict<std::string, double>(getLowLevelNumercialData());
}
std::map<std::string, std::string> RFModulator::getLowLevelStringData()const
{
	std::map < std::string, std::string> low_level_strings_r;
	for (auto&& it : low_level_strings)
	{
		low_level_strings_r[it.first] = it.second.second;
	}
	return low_level_strings_r;
}
boost::python::dict RFModulator::getLowLevelStringData_Py()const
{
	return to_py_dict<std::string, std::string>(getLowLevelStringData());
}
boost::python::dict RFModulator::getLowLevelData()const
{
	//typename std::map<key, value>::iterator iter;
	boost::python::dict newDictiOnary;
	for (auto&& iter: getLowLevelNumercialData())
	{
		newDictiOnary[iter.first] = iter.second;
	}
	for (auto&& iter : getLowLevelStringData())
	{
		newDictiOnary[iter.first] = iter.second;
	}
	return newDictiOnary;
}

STATE RFModulator::getErrorState() const
{
	return error_state;
}
bool RFModulator::isErrorStateGood() const
{
	return error_state == STATE::GOOD;
}
STATE RFModulator::getStateSetRead() const
{
	return main_state_set_read.second;
}
bool RFModulator::isWarm()const
{
	return is_warm;
}
bool RFModulator::isNotWarm()const
{
	return !is_warm;
}
STATE  RFModulator::getMainState()const
{
	return main_state.second;
}
bool RFModulator::isInRFOn() const
{
	if (getMainState() == STATE::ON)
	{
		return true;
	}
	return false;
}
bool RFModulator::isInHVOn() const
{
	if (getMainState() == STATE::HV_ON)
	{
		return true;
	}
	return false;
}
bool RFModulator::isInStandby() const
{
	if (getMainState() == STATE::STANDBY)
	{
		return true;
	}
	return false;
}
bool RFModulator::isInOff() const
{
	if (getMainState() == STATE::OFF)
	{
		return true;
	}
	return false;
}


void RFModulator::updateGunErrorString(const std::string& time_stamp_str, const std::string& value)
{
	messenger.printDebugMessage("updateGunErrorString, value = ", value);

	if (GlobalFunctions::entryExists(gun_mod_hex_map, value))
	{
		std::pair<std::string, std::string> new_error{ time_stamp_str, gun_mod_hex_map.at(value) };
		gun_error_string_buffer.push_back(new_error);
		messenger.printDebugMessage("Gun ERROR MESSAGE = ", new_error.first, " ", new_error.second);
	}
	else
	{
		messenger.printDebugMessage("!!ERROR!! unexpecetd Gun Mod Error-code:[", value, "]");
	}
	//messenger.printDebugMessage("Gun ERROR MESSAGE = ", gun_error_string_buffer.back().first," ", gun_error_string_buffer.back().second);
}

void RFModulator::updateErrorState()
{
	/* based on the hexcode */
	if (GlobalFunctions::entryExists(good_gun_hex_codes, gun_error_string_buffer.back().second))
	{
		error_state = STATE::GOOD;
	}
	else
	{
		if (GlobalFunctions::entryExists(gun_mod_hex_map, gun_error_string_buffer.back().second))
		{
			error_state = STATE::BAD;
		}
		else
		{
			error_state = STATE::UNKNOWN;
		}
	}
}
void RFModulator::debugMessagesOn()
{
	messenger.debugMessagesOn();
	messenger.printDebugMessage(hardwareName, " - DEBUG ON");
	epicsInterface->debugMessagesOn();
}

void RFModulator::debugMessagesOff()
{
	messenger.debugMessagesOff();
	messenger.printDebugMessage(hardwareName, " - DEBUG OFF");
	epicsInterface->debugMessagesOff();
}

void RFModulator::messagesOn()
{
	messenger.messagesOn();
	messenger.printMessage(hardwareName, " - MESSAGES ON");
	epicsInterface->messagesOn();
}

void RFModulator::messagesOff()
{
	messenger.printMessage(hardwareName, " - MESSAGES OFF");
	messenger.messagesOff();
	epicsInterface->messagesOff();
}