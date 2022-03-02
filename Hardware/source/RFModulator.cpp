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
    setGunModHexMap();
	messenger.printMessage("constructing PV data for: ", hardwareName);
	setPVStructs();
}
RFModulator::RFModulator(const RFModulator& copyRFModulator)
	: Hardware(copyRFModulator),
	  epicsInterface(copyRFModulator.epicsInterface),
	  low_level_values(copyRFModulator.low_level_values),
	  low_level_strings(copyRFModulator.low_level_strings)
	//:
	//good_gun_hex_codes(copyRFModulator.good_gun_hex_codes)
{
	good_gun_hex_codes = copyRFModulator.good_gun_hex_codes;
}
RFModulator::~RFModulator(){}

void RFModulator::attachToInitialContext()
{
    epicsInterface->attachTo_thisCaContext();
}

void RFModulator::detachFromInitialContext()
{
    epicsInterface->detachFrom_thisCaContext();
}



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
        messenger.printDebugMessage("MOD WARMUP TIME: ", wam_up_time_remaining.second);
		is_warm = false;
	}
}


std::map<std::string, double> RFModulator::getLowLevelNumericalData()const
{
	std::map < std::string, double> low_level_values_r;
	for (auto&& it : low_level_values)
	{
		low_level_values_r[it.first] = it.second.second;
	}
	return low_level_values_r;
}
boost::python::dict RFModulator::getLowLevelNumericalData_Py()const
{
	return to_py_dict<std::string, double>(getLowLevelNumericalData());
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
	for (auto&& iter: getLowLevelNumericalData())
	{
		newDictiOnary[iter.first] = iter.second;
	}
	for (auto&& iter : getLowLevelStringData())
	{
		newDictiOnary[iter.first] = iter.second;
	}
	return newDictiOnary;
}

bool RFModulator::isInHoldRFOn() const
{
    return holdRFOn.second == STATE::HOLD_RF_ON;
}

bool RFModulator::isInManualOperation() const
{
    return holdRFOn.second == STATE::MANUAL_OPERATION;
}

bool RFModulator::isInHoldRFOnCon() const
{
    return holdRFOn.second == STATE::HOLD_RF_ON_CON;
}

STATE RFModulator::getHoldRFOnState() const
{
    return holdRFOn.second;
}

void RFModulator::setHoldRFOnState(STATE holdRFOnState)
{
    epicsInterface->setHoldRFOnState(pvStructs.at(RFModulatorRecords::HOLD_RF_ON), holdRFOnState);
}

void RFModulator::setHoldRFOnToManualOperation()
{
    setHoldRFOnState(STATE::MANUAL_OPERATION);
}

void RFModulator::setHoldRFOnToHoldRFOnCon()
{
    setHoldRFOnState(STATE::HOLD_RF_ON_CON);
}

void RFModulator::setHoldRFOnToHoldRFOn()
{
    setHoldRFOnState(STATE::HOLD_RF_ON);
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
	if (getMainState() == STATE::RF_ON)
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

		std::pair<std::string, std::string> new_error{ time_stamp_str, value };
		gun_error_string_buffer.push_back(new_error);
		messenger.printDebugMessage("Gun ERROR MESSAGE = ", new_error.first, " ", new_error.second);
		updateErrorState();
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




void RFModulator::setGunModHexMap()
{
    gun_mod_hex_map = {
       {"0" , "Not connected****"},
       {"1000" , "OffInt***********"},
       {"2000" , "Off**************"},
       {"3000" , "OffReq***********"},
       {"4000" , "StandByInt*******"},
       {"5000" , "StandByReq*******"},
       {"6000" , "StandBy**********"},
       {"7000" , "HvOffReq*********"},
       {"8000" , "HvInt************"},
       {"9000" , "HvReq************"},
       {"A000" , "Hv***************"},
       {"B000" , "TrigOffReq*******"},
       {"C000" , "TrigReq**********"},
       {"D000" , "Trig*************"},
       {"1001" , "Sys\\HwCtr\\Klystron or Magnet temp int."},
       {"1002" , "Sys\\HwCtr\\StandBy 2 int."},
       {"1003" , "Sys\\Error."},
       {"1004" , "Sys\\IntCom\\Communication int."},
       {"1005" , "Sys\\IntCom\\Error"},
       {"1010" , "Pt\\Low oil level int."},
       {"1011" , "Pt\\Cool\\Flow & oil temp int."},
       {"1012" , "Pt\\FilPs\\Power fail int."},
       {"1013" , "Sys\\Fronp panel breaker int."},
       {"1020" , "RF\\HwCtr\\Vacuum 1 int."},
       {"1021" , "RF\\Duovac 1 volt int."},
       {"1022" , "RF\\Duovac 2 volt int."},
       {"1023" , "RF\\Duovac 3 volt int."},
       {"1024" , "RF\\Duovac 4 volt int."},
       {"1025" , "RF\\Duovac 5 volt int."},
       {"1026" , "RF\\JUMO Com. int."},
       {"1027" , "RF\\DUOVAC 1 Com. int."},
       {"1028" , "RF\\DUOVAC 2 Com. int."},
       {"1029" , "RF\\DUOVAC 3 Com. int."},
       {"1030" , "RF\\PLC Com. int."},
       {"1044" , "Sys\\Cool\\Solenoid Flow int."},
       {"1700" , "RF\\Plc\\Vacuum 1 int."},
       {"1701" , "RF\\Plc\\Filament int."},
       {"4001" , "Sys\\HwCtr\\Hv interlock 1."},
       {"4002" , "Sys\\HwCtr\\Hv interlock 2."},
       {"4003" , "Sys\\ExtCom\\Communication int."},
       {"4004" , "Sys\\IntCom\\Communication int."},
       {"4010" , "Pt\\High oil level int."},
       {"4011" , "Pt\\Cool\\HVPS Flow int."},
       {"4012" , "Pt\\Cool\\Tank temp int."},
       {"4013" , "Pt\\FilPs\\Curr int."},
       {"4014" , "Pt\\FilPs\\Volt int."},
       {"4015" , "Pt\\BiasPs\\Power fail int."},
       {"4016" , "Pt\\BiasPs\\Curr int."},
       {"4017" , "Pt\\BiasPs\\Volt int."},
       {"4018" , "Pt\\Cool\\Tank temp B int."},
       {"4019" , "Pt\\Cool\\Water in Temp int."},
       {"401A" , "Pt\\Cool\\water out Temp int."},
       {"401B" , "Pt\\Cool\\Tank ambience Temp int."},
       {"401C" , "Pt\\Cool\\Temp sensor failure int."},
       {"401D" , "Pt\\Cool\\HPSU Flow int."},
       {"401E" , "Pt\\Cool\\Tank Flow int."},
       {"401F" , "Pt\\Side door open int."},
       {"4020" , "Pt\\Cool\\Flow Circuit 5 int."},
       {"4021" , "Pt\\Cool\\Klystron Body Flow int."},
       {"4022" , "Pt\\Cool\\Klystron Collector Flow int."},
       {"4023" , "Pt\\Cool\\Flow Circuit 8 int."},
       {"4024" , "Pt\\Cool\\Flow Circuit 9 int."},
       {"4025" , "Pt\\Cool\\Flow Circuit 10 int."},
       {"4026" , "Pt\\Cool\\Flow Circuit 11 int."},
       {"4027" , "Pt\\Cool\\Flow Circuit 12 int."},
       {"4028" , "Pt\\Cool\\Flow Circuit 13 int."},
       {"4029" , "Pt\\Cool\\Flow Circuit 14 int."},
       {"402A" , "Pt\\Cool\\Flow Circuit 15 int."},
       {"402B" , "Pt\\Cool\\Flow Circuit 16 int."},
       {"4030" , "Rf\\Cool\\Rf Flow #1 int."},
       {"4031" , "Rf\\Cool\\Solenoid Flow int."},
       {"4032" , "Rf\\Cool\\Klystron Body Flow int."},
       {"4033" , "Rf\\Cool\\Collector Flow int."},
       {"4034" , "Rf\\Cool\\Water cubicle int."},
       {"4035" , "Rf\\Cool\\Temp 1 int."},
       {"4036" , "Rf\\MagPs1\\Curr int."},
       {"4037" , "Rf\\MagPs1\\Volt int."},
       {"4038" , "Rf\\WGuid\\Reflected Pow int."},
       {"4039" , "Rf\\WGuid\\Pressure int."},
       {"403A" , "Rf\\MagPs2\\Curr int."},
       {"403B" , "Rf\\MagPs2\\Volt int."},
       {"403C" , "Rf\\MagPs3\\Curr int."},
       {"403D" , "Rf\\MagPs3\\Volt int."},
       {"403E" , "Rf\\MagPs4\\Curr int."},
       {"403F" , "Rf\\MagPs4\\Volt int."},
       {"4040" , "Rf\\Cool\\Temp 2 int."},
       {"4041" , "Rf\\MagPs\\HwCurr int."},
       {"4042" , "Rf\\HwCtr\\Vacuum 2 int."},
       {"4043" , "Rf\\MagPs\\Comm int."},
       {"4050" , "Rf\\Unidentified int."},
       {"4060" , "HvPs\\Ps1\\Soft start int."},
       {"4061" , "HvPs\\Ps1\\IGBT int."},
       {"4062" , "HvPs\\Ps1\\Phase loss int."},
       {"4063" , "HvPs\\Ps1\\Transformer temp int."},
       {"4064" , "HvPs\\Ps1\\Rectifier temp int."},
       {"4065" , "HvPs\\Ps1\\IGBT temp int."},
       {"4066" , "HvPs\\Ps1\\Over volt int."},
       {"4067" , "HvPs\\Ps1\\Over curr int."},
       {"4068" , "HvPs\\Ps1\\volt int."},
       {"4070" , "HvPs\\Ps2\\Soft start int."},
       {"4071" , "HvPs\\Ps2\\IGBT int."},
       {"4072" , "HvPs\\Ps2\\Phase loss int."},
       {"4073" , "HvPs\\Ps2\\Transformer temp int."},
       {"4074" , "HvPs\\Ps2\\Rectifier temp int."},
       {"4075" , "HvPs\\Ps2\\IGBT temp int."},
       {"4076" , "HvPs\\Ps2\\Over volt int."},
       {"4077" , "HvPs\\Ps2\\Over curr int."},
       {"4078" , "HvPs\\Ps2\\volt int."},
       {"4080" , "HvPs\\Ps3\\Soft start int."},
       {"4081" , "HvPs\\Ps3\\IGBT int."},
       {"4082" , "HvPs\\Ps3\\Phase loss int."},
       {"4083" , "HvPs\\Ps3\\Transformer temp int."},
       {"4084" , "HvPs\\Ps3\\Rectifier temp int."},
       {"4085" , "HvPs\\Ps3\\IGBT temp int."},
       {"4086" , "HvPs\\Ps3\\Over volt int."},
       {"4087" , "HvPs\\Ps3\\Over curr int."},
       {"4088" , "HvPs\\Ps3\\volt int."},
       {"4090" , "HvPs\\Ps4\\Soft start int."},
       {"4091" , "HvPs\\Ps4\\IGBT int."},
       {"4092" , "HvPs\\Ps4\\Phase loss int."},
       {"4093" , "HvPs\\Ps4\\Transformer temp int."},
       {"4094" , "HvPs\\Ps4\\Rectifier temp int."},
       {"4095" , "HvPs\\Ps4\\IGBT temp int."},
       {"4096" , "HvPs\\Ps4\\Over volt int."},
       {"4097" , "HvPs\\Ps4\\Over curr int."},
       {"4098" , "HvPs\\Ps4\\volt int."},
       {"40A0" , "HvPs\\Ps5\\Soft start int."},
       {"40A1" , "HvPs\\Ps5\\IGBT int."},
       {"40A2" , "HvPs\\Ps5\\Phase loss int."},
       {"40A3" , "HvPs\\Ps5\\Transformer temp int."},
       {"40A4" , "HvPs\\Ps5\\Rectifier temp int."},
       {"40A5" , "HvPs\\Ps5\\IGBT temp int."},
       {"40A6" , "HvPs\\Ps5\\Over volt int."},
       {"40A7" , "HvPs\\Ps5\\Over curr int."},
       {"40A8" , "HvPs\\Ps5\\volt int."},
       {"40B0" , "HvPs\\Ps6\\Soft start int."},
       {"40B1" , "HvPs\\Ps6\\IGBT int."},
       {"40B2" , "HvPs\\Ps6\\Phase loss int."},
       {"40B3" , "HvPs\\Ps6\\Transformer temp int."},
       {"40B4" , "HvPs\\Ps6\\Rectifier temp int."},
       {"40B5" , "HvPs\\Ps6\\IGBT temp int."},
       {"40B6" , "HvPs\\Ps6\\Over volt int."},
       {"40B7" , "HvPs\\Ps6\\Over curr int."},
       {"40B8" , "HvPs\\Ps6\\volt int."},
       {"40C0" , "HvPs\\Ps7\\Soft start int."},
       {"40C1" , "HvPs\\Ps7\\IGBT int."},
       {"40C2" , "HvPs\\Ps7\\Phase loss int."},
       {"40C3" , "HvPs\\Ps7\\Transformer temp int."},
       {"40C4" , "HvPs\\Ps7\\Rectifier temp int."},
       {"40C5" , "HvPs\\Ps7\\IGBT temp int."},
       {"40C6" , "HvPs\\Ps7\\Over volt int."},
       {"40C7" , "HvPs\\Ps7\\Over curr int."},
       {"40C8" , "HvPs\\Ps7\\volt int."},
       {"40D0" , "HvPs\\Ps8\\Soft start int."},
       {"40D1" , "HvPs\\Ps8\\IGBT int."},
       {"40D2" , "HvPs\\Ps8\\Phase loss int."},
       {"40D3" , "HvPs\\Ps8\\Transformer temp int."},
       {"40D4" , "HvPs\\Ps8\\Rectifier temp int."},
       {"40D5" , "HvPs\\Ps8\\IGBT temp int."},
       {"40D6" , "HvPs\\Ps8\\Over volt int."},
       {"40D7" , "HvPs\\Ps8\\Over curr int."},
       {"40D8" , "HvPs\\Ps8\\volt int."},
       {"40E0" , "HvPs\\Ps9\\Soft start int."},
       {"40E1" , "HvPs\\Ps9\\IGBT int."},
       {"40E2" , "HvPs\\Ps9\\Phase loss int."},
       {"40E3" , "HvPs\\Ps9\\Transformer temp int."},
       {"40E4" , "HvPs\\Ps9\\Rectifier temp int."},
       {"40E5" , "HvPs\\Ps9\\IGBT temp int."},
       {"40E6" , "HvPs\\Ps9\\Over volt int."},
       {"40E7" , "HvPs\\Ps9\\Over curr int."},
       {"40E8" , "HvPs\\Ps9\\volt int."},
       {"40F0" , "HvPs\\Ps10\\Soft start int."},
       {"40F1" , "HvPs\\Ps10\\IGBT int."},
       {"40F2" , "HvPs\\Ps10\\Phase loss int."},
       {"40F3" , "HvPs\\Ps10\\Transformer temp int."},
       {"40F4" , "HvPs\\Ps10\\Rectifier temp int."},
       {"40F5" , "HvPs\\Ps10\\IGBT temp int."},
       {"40F6" , "HvPs\\Ps10\\Over volt int."},
       {"40F7" , "HvPs\\Ps10\\Over curr int."},
       {"40F8" , "HvPs\\Ps10\\volt int."},
       {"4100" , "HvPs\\Ps11\\Soft start int."},
       {"4101" , "HvPs\\Ps11\\IGBT int."},
       {"4102" , "HvPs\\Ps11\\Phase loss int."},
       {"4103" , "HvPs\\Ps11\\Transformer temp int."},
       {"4104" , "HvPs\\Ps11\\Rectifier temp int."},
       {"4105" , "HvPs\\Ps11\\IGBT temp int."},
       {"4106" , "HvPs\\Ps11\\Over volt int."},
       {"4107" , "HvPs\\Ps11\\Over curr int."},
       {"4108" , "HvPs\\Ps11\\volt int."},
       {"4110" , "HvPs\\Ps12\\Soft start int."},
       {"4111" , "HvPs\\Ps12\\IGBT int."},
       {"4112" , "HvPs\\Ps12\\Phase loss int."},
       {"4113" , "HvPs\\Ps12\\Transformer temp int."},
       {"4114" , "HvPs\\Ps12\\Rectifier temp int."},
       {"4115" , "HvPs\\Ps12\\IGBT temp int."},
       {"4116" , "HvPs\\Ps12\\Over volt int."},
       {"4117" , "HvPs\\Ps12\\Over curr int."},
       {"4118" , "HvPs\\Ps12\\volt int."},
       {"4130" , "Sw\\Section1\\Switch 1 curr int."},
       {"4131" , "Sw\\Section1\\Switch 2 curr int."},
       {"4132" , "Sw\\Section1\\Switch 3 curr int."},
       {"4133" , "Sw\\Section1\\Switch 4 curr int."},
       {"4134" , "Sw\\Section1\\Switch 5 curr int."},
       {"4135" , "Sw\\Section1\\Switch 6 curr int."},
       {"4136" , "Sw\\Section1\\Switch 7 curr int."},
       {"4137" , "Sw\\Section1\\Switch 8 curr int."},
       {"4138" , "Sw\\Section1\\Switch 9 curr int."},
       {"4139" , "Sw\\Section1\\Switch 10 curr int."},
       {"413A" , "Sw\\Section1\\Switch 11 curr int."},
       {"413B" , "Sw\\Section1\\Switch 12 curr int."},
       {"4140" , "Sw\\Section2\\Switch 1 curr int."},
       {"4141" , "Sw\\Section2\\Switch 2 curr int."},
       {"4142" , "Sw\\Section2\\Switch 3 curr int."},
       {"4143" , "Sw\\Section2\\Switch 4 curr int."},
       {"4144" , "Sw\\Section2\\Switch 5 curr int."},
       {"4145" , "Sw\\Section2\\Switch 6 curr int."},
       {"4146" , "Sw\\Section2\\Switch 7 curr int."},
       {"4147" , "Sw\\Section2\\Switch 8 curr int."},
       {"4148" , "Sw\\Section2\\Switch 9 curr int."},
       {"4149" , "Sw\\Section2\\Switch 10 curr int."},
       {"414A" , "Sw\\Section2\\Switch 11 curr int."},
       {"414B" , "Sw\\Section2\\Switch 12 curr int."},
       {"4150" , "Sw\\Section3\\Switch 1 curr int."},
       {"4151" , "Sw\\Section3\\Switch 2 curr int."},
       {"4152" , "Sw\\Section3\\Switch 3 curr int."},
       {"4153" , "Sw\\Section3\\Switch 4 curr int."},
       {"4154" , "Sw\\Section3\\Switch 5 curr int."},
       {"4155" , "Sw\\Section3\\Switch 6 curr int."},
       {"4156" , "Sw\\Section3\\Switch 7 curr int."},
       {"4157" , "Sw\\Section3\\Switch 8 curr int."},
       {"4158" , "Sw\\Section3\\Switch 9 curr int."},
       {"4159" , "Sw\\Section3\\Switch 10 curr int."},
       {"415A" , "Sw\\Section3\\Switch 11 curr int."},
       {"415B" , "Sw\\Section3\\Switch 12 curr int."},
       {"4160" , "Sw\\Section4\\Switch 1 curr int."},
       {"4161" , "Sw\\Section4\\Switch 2 curr int."},
       {"4162" , "Sw\\Section4\\Switch 3 curr int."},
       {"4163" , "Sw\\Section4\\Switch 4 curr int."},
       {"4164" , "Sw\\Section4\\Switch 5 curr int."},
       {"4165" , "Sw\\Section4\\Switch 6 curr int."},
       {"4166" , "Sw\\Section4\\Switch 7 curr int."},
       {"4167" , "Sw\\Section4\\Switch 8 curr int."},
       {"4168" , "Sw\\Section4\\Switch 9 curr int."},
       {"4169" , "Sw\\Section4\\Switch 10 curr int."},
       {"416A" , "Sw\\Section4\\Switch 11 curr int."},
       {"416B" , "Sw\\Section4\\Switch 12 curr int."},
       {"4170" , "Sw\\Section5\\Switch 1 curr int."},
       {"4171" , "Sw\\Section5\\Switch 2 curr int."},
       {"4172" , "Sw\\Section5\\Switch 3 curr int."},
       {"4173" , "Sw\\Section5\\Switch 4 curr int."},
       {"4174" , "Sw\\Section5\\Switch 5 curr int."},
       {"4175" , "Sw\\Section5\\Switch 6 curr int."},
       {"4176" , "Sw\\Section5\\Switch 7 curr int."},
       {"4177" , "Sw\\Section5\\Switch 8 curr int."},
       {"4178" , "Sw\\Section5\\Switch 9 curr int."},
       {"4179" , "Sw\\Section5\\Switch 10 curr int."},
       {"417A" , "Sw\\Section5\\Switch 11 curr int."},
       {"417B" , "Sw\\Section5\\Switch 12 curr int."},
       {"4180" , "Sw\\Section6\\Switch 1 curr int."},
       {"4181" , "Sw\\Section6\\Switch 2 curr int."},
       {"4182" , "Sw\\Section6\\Switch 3 curr int."},
       {"4183" , "Sw\\Section6\\Switch 4 curr int."},
       {"4184" , "Sw\\Section6\\Switch 5 curr int."},
       {"4185" , "Sw\\Section6\\Switch 6 curr int."},
       {"4186" , "Sw\\Section6\\Switch 7 curr int."},
       {"4187" , "Sw\\Section6\\Switch 8 curr int."},
       {"4188" , "Sw\\Section6\\Switch 9 curr int."},
       {"4189" , "Sw\\Section6\\Switch 10 curr int."},
       {"418A" , "Sw\\Section6\\Switch 11 curr int."},
       {"418B" , "Sw\\Section6\\Switch 12 curr int."},
       {"4190" , "Sw\\Section7\\Switch 1 curr int."},
       {"4191" , "Sw\\Section7\\Switch 2 curr int."},
       {"4192" , "Sw\\Section7\\Switch 3 curr int."},
       {"4193" , "Sw\\Section7\\Switch 4 curr int."},
       {"4194" , "Sw\\Section7\\Switch 5 curr int."},
       {"4195" , "Sw\\Section7\\Switch 6 curr int."},
       {"4196" , "Sw\\Section7\\Switch 7 curr int."},
       {"4197" , "Sw\\Section7\\Switch 8 curr int."},
       {"4198" , "Sw\\Section7\\Switch 9 curr int."},
       {"4199" , "Sw\\Section7\\Switch 10 curr int."},
       {"419A" , "Sw\\Section7\\Switch 11 curr int."},
       {"419B" , "Sw\\Section7\\Switch 12 curr int."},
       {"41A0" , "Sw\\Section8\\Switch 1 curr int."},
       {"41A1" , "Sw\\Section8\\Switch 2 curr int."},
       {"41A2" , "Sw\\Section8\\Switch 3 curr int."},
       {"41A3" , "Sw\\Section8\\Switch 4 curr int."},
       {"41A4" , "Sw\\Section8\\Switch 5 curr int."},
       {"41A5" , "Sw\\Section8\\Switch 6 curr int."},
       {"41A6" , "Sw\\Section8\\Switch 7 curr int."},
       {"41A7" , "Sw\\Section8\\Switch 8 curr int."},
       {"41A8" , "Sw\\Section8\\Switch 9 curr int."},
       {"41A9" , "Sw\\Section8\\Switch 10 curr int."},
       {"41AA" , "Sw\\Section8\\Switch 11 curr int."},
       {"41AB" , "Sw\\Section8\\Switch 12 curr int."},
       {"41B0" , "Sw\\Section9\\Switch 1 curr int."},
       {"41B1" , "Sw\\Section9\\Switch 2 curr int."},
       {"41B2" , "Sw\\Section9\\Switch 3 curr int."},
       {"41B3" , "Sw\\Section9\\Switch 4 curr int."},
       {"41B4" , "Sw\\Section9\\Switch 5 curr int."},
       {"41B5" , "Sw\\Section9\\Switch 6 curr int."},
       {"41B6" , "Sw\\Section9\\Switch 7 curr int."},
       {"41B7" , "Sw\\Section9\\Switch 8 curr int."},
       {"41B8" , "Sw\\Section9\\Switch 9 curr int."},
       {"41B9" , "Sw\\Section9\\Switch 10 curr int."},
       {"41BA" , "Sw\\Section9\\Switch 11 curr int."},
       {"41BB" , "Sw\\Section9\\Switch 12 curr int."},
       {"41C0" , "Sw\\Section10\\Switch 1 curr int."},
       {"41C1" , "Sw\\Section10\\Switch 2 curr int."},
       {"41C2" , "Sw\\Section10\\Switch 3 curr int."},
       {"41C3" , "Sw\\Section10\\Switch 4 curr int."},
       {"41C4" , "Sw\\Section10\\Switch 5 curr int."},
       {"41C5" , "Sw\\Section10\\Switch 6 curr int."},
       {"41C6" , "Sw\\Section10\\Switch 7 curr int."},
       {"41C7" , "Sw\\Section10\\Switch 8 curr int."},
       {"41C8" , "Sw\\Section10\\Switch 9 curr int."},
       {"41C9" , "Sw\\Section10\\Switch 10 curr int."},
       {"41CA" , "Sw\\Section10\\Switch 11 curr int."},
       {"41CB" , "Sw\\Section10\\Switch 12 curr int."},
       {"41D0" , "Sw\\Section11\\Switch 1 curr int."},
       {"41D1" , "Sw\\Section11\\Switch 2 curr int."},
       {"41D2" , "Sw\\Section11\\Switch 3 curr int."},
       {"41D3" , "Sw\\Section11\\Switch 4 curr int."},
       {"41D4" , "Sw\\Section11\\Switch 5 curr int."},
       {"41D5" , "Sw\\Section11\\Switch 6 curr int."},
       {"41D6" , "Sw\\Section11\\Switch 7 curr int."},
       {"41D7" , "Sw\\Section11\\Switch 8 curr int."},
       {"41D8" , "Sw\\Section11\\Switch 9 curr int."},
       {"41D9" , "Sw\\Section11\\Switch 10 curr int."},
       {"41DA" , "Sw\\Section11\\Switch 11 curr int."},
       {"41DB" , "Sw\\Section11\\Switch 12 curr int."},
       {"41E0" , "Sw\\Section12\\Switch 1 curr int."},
       {"41E1" , "Sw\\Section12\\Switch 2 curr int."},
       {"41E2" , "Sw\\Section12\\Switch 3 curr int."},
       {"41E3" , "Sw\\Section12\\Switch 4 curr int."},
       {"41E4" , "Sw\\Section12\\Switch 5 curr int."},
       {"41E5" , "Sw\\Section12\\Switch 6 curr int."},
       {"41E6" , "Sw\\Section12\\Switch 7 curr int."},
       {"41E7" , "Sw\\Section12\\Switch 8 curr int."},
       {"41E8" , "Sw\\Section12\\Switch 9 curr int."},
       {"41E9" , "Sw\\Section12\\Switch 10 curr int."},
       {"41EA" , "Sw\\Section12\\Switch 11 curr int."},
       {"41EB" , "Sw\\Section12\\Switch 12 curr int."},
       {"4230" , "Sw\\Section1\\Switch 1 temp int."},
       {"4231" , "Sw\\Section1\\Switch 2 temp int."},
       {"4232" , "Sw\\Section1\\Switch 3 temp int."},
       {"4233" , "Sw\\Section1\\Switch 4 temp int."},
       {"4234" , "Sw\\Section1\\Switch 5 temp int."},
       {"4235" , "Sw\\Section1\\Switch 6 temp int."},
       {"4236" , "Sw\\Section1\\Switch 7 temp int."},
       {"4237" , "Sw\\Section1\\Switch 8 temp int."},
       {"4238" , "Sw\\Section1\\Switch 9 temp int."},
       {"4239" , "Sw\\Section1\\Switch 10 temp int."},
       {"423A" , "Sw\\Section1\\Switch 11 temp int."},
       {"423B" , "Sw\\Section1\\Switch 12 temp int."},
       {"423C" , "Sw\\Section1\\Temp sens. fail int."},
       {"4240" , "Sw\\Section2\\Switch 1 temp int."},
       {"4241" , "Sw\\Section2\\Switch 2 temp int."},
       {"4242" , "Sw\\Section2\\Switch 3 temp int."},
       {"4243" , "Sw\\Section2\\Switch 4 temp int."},
       {"4244" , "Sw\\Section2\\Switch 5 temp int."},
       {"4245" , "Sw\\Section2\\Switch 6 temp int."},
       {"4246" , "Sw\\Section2\\Switch 7 temp int."},
       {"4247" , "Sw\\Section2\\Switch 8 temp int."},
       {"4248" , "Sw\\Section2\\Switch 9 temp int."},
       {"4249" , "Sw\\Section2\\Switch 10 temp int."},
       {"424A" , "Sw\\Section2\\Switch 11 temp int."},
       {"424B" , "Sw\\Section2\\Switch 12 temp int."},
       {"424C" , "Sw\\Section2\\Temp sens. fail int."},
       {"4250" , "Sw\\Section3\\Switch 1 temp int."},
       {"4251" , "Sw\\Section3\\Switch 2 temp int."},
       {"4252" , "Sw\\Section3\\Switch 3 temp int."},
       {"4253" , "Sw\\Section3\\Switch 4 temp int."},
       {"4254" , "Sw\\Section3\\Switch 5 temp int."},
       {"4255" , "Sw\\Section3\\Switch 6 temp int."},
       {"4256" , "Sw\\Section3\\Switch 7 temp int."},
       {"4257" , "Sw\\Section3\\Switch 8 temp int."},
       {"4258" , "Sw\\Section3\\Switch 9 temp int."},
       {"4259" , "Sw\\Section3\\Switch 10 temp int."},
       {"425A" , "Sw\\Section3\\Switch 11 temp int."},
       {"425B" , "Sw\\Section3\\Switch 12 temp int."},
       {"425C" , "Sw\\Section3\\Temp sens. fail int."},
       {"4260" , "Sw\\Section4\\Switch 1 temp int."},
       {"4261" , "Sw\\Section4\\Switch 2 temp int."},
       {"4262" , "Sw\\Section4\\Switch 3 temp int."},
       {"4263" , "Sw\\Section4\\Switch 4 temp int."},
       {"4264" , "Sw\\Section4\\Switch 5 temp int."},
       {"4265" , "Sw\\Section4\\Switch 6 temp int."},
       {"4266" , "Sw\\Section4\\Switch 7 temp int."},
       {"4267" , "Sw\\Section4\\Switch 8 temp int."},
       {"4268" , "Sw\\Section4\\Switch 9 temp int."},
       {"4269" , "Sw\\Section4\\Switch 10 temp int."},
       {"426A" , "Sw\\Section4\\Switch 11 temp int."},
       {"426B" , "Sw\\Section4\\Switch 12 temp int."},
       {"426C" , "Sw\\Section4\\Temp sens. fail int."},
       {"4270" , "Sw\\Section5\\Switch 1 temp int."},
       {"4271" , "Sw\\Section5\\Switch 2 temp int."},
       {"4272" , "Sw\\Section5\\Switch 3 temp int."},
       {"4273" , "Sw\\Section5\\Switch 4 temp int."},
       {"4274" , "Sw\\Section5\\Switch 5 temp int."},
       {"4275" , "Sw\\Section5\\Switch 6 temp int."},
       {"4276" , "Sw\\Section5\\Switch 7 temp int."},
       {"4277" , "Sw\\Section5\\Switch 8 temp int."},
       {"4278" , "Sw\\Section5\\Switch 9 temp int."},
       {"4279" , "Sw\\Section5\\Switch 10 temp int."},
       {"427A" , "Sw\\Section5\\Switch 11 temp int."},
       {"427B" , "Sw\\Section5\\Switch 12 temp int."},
       {"427C" , "Sw\\Section5\\Temp sens. fail int."},
       {"4280" , "Sw\\Section6\\Switch 1 temp int."},
       {"4281" , "Sw\\Section6\\Switch 2 temp int."},
       {"4282" , "Sw\\Section6\\Switch 3 temp int."},
       {"4283" , "Sw\\Section6\\Switch 4 temp int."},
       {"4284" , "Sw\\Section6\\Switch 5 temp int."},
       {"4285" , "Sw\\Section6\\Switch 6 temp int."},
       {"4286" , "Sw\\Section6\\Switch 7 temp int."},
       {"4287" , "Sw\\Section6\\Switch 8 temp int."},
       {"4288" , "Sw\\Section6\\Switch 9 temp int."},
       {"4289" , "Sw\\Section6\\Switch 10 temp int."},
       {"428A" , "Sw\\Section6\\Switch 11 temp int."},
       {"428B" , "Sw\\Section6\\Switch 12 temp int."},
       {"428C" , "Sw\\Section6\\Temp sens. fail int."},
       {"4290" , "Sw\\Section7\\Switch 1 temp int."},
       {"4291" , "Sw\\Section7\\Switch 2 temp int."},
       {"4292" , "Sw\\Section7\\Switch 3 temp int."},
       {"4293" , "Sw\\Section7\\Switch 4 temp int."},
       {"4294" , "Sw\\Section7\\Switch 5 temp int."},
       {"4295" , "Sw\\Section7\\Switch 6 temp int."},
       {"4296" , "Sw\\Section7\\Switch 7 temp int."},
       {"4297" , "Sw\\Section7\\Switch 8 temp int."},
       {"4298" , "Sw\\Section7\\Switch 9 temp int."},
       {"4299" , "Sw\\Section7\\Switch 10 temp int."},
       {"429A" , "Sw\\Section7\\Switch 11 temp int."},
       {"429B" , "Sw\\Section7\\Switch 12 temp int."},
       {"429C" , "Sw\\Section7\\Temp sens. fail int."},
       {"42A0" , "Sw\\Section8\\Switch 1 temp int."},
       {"42A1" , "Sw\\Section8\\Switch 2 temp int."},
       {"42A2" , "Sw\\Section8\\Switch 3 temp int."},
       {"42A3" , "Sw\\Section8\\Switch 4 temp int."},
       {"42A4" , "Sw\\Section8\\Switch 5 temp int."},
       {"42A5" , "Sw\\Section8\\Switch 6 temp int."},
       {"42A6" , "Sw\\Section8\\Switch 7 temp int."},
       {"42A7" , "Sw\\Section8\\Switch 8 temp int."},
       {"42A8" , "Sw\\Section8\\Switch 9 temp int."},
       {"42A9" , "Sw\\Section8\\Switch 10 temp int."},
       {"42AA" , "Sw\\Section8\\Switch 11 temp int."},
       {"42AB" , "Sw\\Section8\\Switch 12 temp int."},
       {"42AC" , "Sw\\Section8\\Temp sens. fail int."},
       {"42B0" , "Sw\\Section9\\Switch 1 temp int."},
       {"42B1" , "Sw\\Section9\\Switch 2 temp int."},
       {"42B2" , "Sw\\Section9\\Switch 3 temp int."},
       {"42B3" , "Sw\\Section9\\Switch 4 temp int."},
       {"42B4" , "Sw\\Section9\\Switch 5 temp int."},
       {"42B5" , "Sw\\Section9\\Switch 6 temp int."},
       {"42B6" , "Sw\\Section9\\Switch 7 temp int."},
       {"42B7" , "Sw\\Section9\\Switch 8 temp int."},
       {"42B8" , "Sw\\Section9\\Switch 9 temp int."},
       {"42B9" , "Sw\\Section9\\Switch 10 temp int."},
       {"42BA" , "Sw\\Section9\\Switch 11 temp int."},
       {"42BB" , "Sw\\Section9\\Switch 12 temp int."},
       {"42BC" , "Sw\\Section9\\Temp sens. fail int."},
       {"42C0" , "Sw\\Section10\\Switch 1 temp int."},
       {"42C1" , "Sw\\Section10\\Switch 2 temp int."},
       {"42C2" , "Sw\\Section10\\Switch 3 temp int."},
       {"42C3" , "Sw\\Section10\\Switch 4 temp int."},
       {"42C4" , "Sw\\Section10\\Switch 5 temp int."},
       {"42C5" , "Sw\\Section10\\Switch 6 temp int."},
       {"42C6" , "Sw\\Section10\\Switch 7 temp int."},
       {"42C7" , "Sw\\Section10\\Switch 8 temp int."},
       {"42C8" , "Sw\\Section10\\Switch 9 temp int."},
       {"42C9" , "Sw\\Section10\\Switch 10 temp int."},
       {"42CA" , "Sw\\Section10\\Switch 11 temp int."},
       {"42CB" , "Sw\\Section10\\Switch 12 temp int."},
       {"42CC" , "Sw\\Section10\\Temp sens.fail int."},
       {"42D0" , "Sw\\Section11\\Switch 1 temp int."},
       {"42D1" , "Sw\\Section11\\Switch 2 temp int."},
       {"42D2" , "Sw\\Section11\\Switch 3 temp int."},
       {"42D3" , "Sw\\Section11\\Switch 4 temp int."},
       {"42D4" , "Sw\\Section11\\Switch 5 temp int."},
       {"42D5" , "Sw\\Section11\\Switch 6 temp int."},
       {"42D6" , "Sw\\Section11\\Switch 7 temp int."},
       {"42D7" , "Sw\\Section11\\Switch 8 temp int."},
       {"42D8" , "Sw\\Section11\\Switch 9 temp int."},
       {"42D9" , "Sw\\Section11\\Switch 10 temp int."},
       {"42DA" , "Sw\\Section11\\Switch 11 temp int."},
       {"42DB" , "Sw\\Section11\\Switch 12 temp int."},
       {"42DC" , "Sw\\Section11\\Temp sens.fail int."},
       {"42E0" , "Sw\\Section12\\Switch 1 temp int."},
       {"42E1" , "Sw\\Section12\\Switch 2 temp int."},
       {"42E2" , "Sw\\Section12\\Switch 3 temp int."},
       {"42E3" , "Sw\\Section12\\Switch 4 temp int."},
       {"42E4" , "Sw\\Section12\\Switch 5 temp int."},
       {"42E5" , "Sw\\Section12\\Switch 6 temp int."},
       {"42E6" , "Sw\\Section12\\Switch 7 temp int."},
       {"42E7" , "Sw\\Section12\\Switch 8 temp int."},
       {"42E8" , "Sw\\Section12\\Switch 9 temp int."},
       {"42E9" , "Sw\\Section12\\Switch 10 temp int."},
       {"42EA" , "Sw\\Section12\\Switch 11 temp int."},
       {"42EB" , "Sw\\Section12\\Switch 12 temp int."},
       {"42EC" , "Sw\\Section12\\Temp sens.fail int."},
       {"4FFF" , "Unassigned interlock"},
       {"4708" , "Rf\\Cool\\RF Shut Off int."},
       {"470B" , "Rf\\PLC\\Ext1 int."},
       {"470C" , "Rf\\PLC\\Ext2 int."},
       {"470D" , "Rf\\PLC\\Ext3 int."},
       {"470E" , "Rf\\PLC\\Ext4 int."},
       {"470F" , "Rf\\PLC\\Ext5 int."},
       {"4710" , "Rf\\PLC\\Ext6 int."},
       {"4716" , "Rf\\PLC\\RF amplifier."},
       {"8001" , "Sys\\Trig\\High PRF int."},
       {"8002" , "Sys\\Trig\\Long trig int."},
       {"8003" , "Sys\\Trig\\Short trig int."},
       {"8004" , "Sys\\HwCtr\\Trig interlock 1."},
       {"8005" , "Sys\\HwCtr\\Trig interlock 2."},
       {"8010" , "Pt\\Diag\\CT arc int."},
       {"8011" , "Pt\\Diag\\CVD arc int."},
       {"8012" , "Pt\\Diag\\CT read int."},
       {"8013" , "Pt\\Diag\\CVD read int."},
       {"8014" , "Pt\\Diag\\Average power int."},
       {"8015" , "Pt\\Diag\\Long plswth read int."},
       {"8016" , "Pt\\Cool\\Temp 1 high int."},
       {"8017" , "Pt\\Cool\\Temp 1 low int."},
       {"8018" , "Pt\\Cool\\Temp 2 high int."},
       {"8019" , "Pt\\Cool\\Temp 2 low int."},
       {"8020" , "Pt\\Cool\\Temp 3 high int."},
       {"8021" , "Pt\\Cool\\Temp 3 low int."},
       {"8022" , "Pt\\Cool\\Temp 4 high int."},
       {"8023" , "Pt\\Cool\\Temp 4 low int."},
       {"8060" , "HvPs"},
       {"8130" , "Sw\\Section1\\Switch 1 curr int."},
       {"8131" , "Sw\\Section1\\Switch 2 curr int."},
       {"8132" , "Sw\\Section1\\Switch 3 curr int."},
       {"8133" , "Sw\\Section1\\Switch 4 curr int."},
       {"8134" , "Sw\\Section1\\Switch 5 curr int."},
       {"8135" , "Sw\\Section1\\Switch 6 curr int."},
       {"8136" , "Sw\\Section1\\Switch 7 curr int."},
       {"8137" , "Sw\\Section1\\Switch 8 curr int."},
       {"8138" , "Sw\\Section1\\Switch 9 curr int."},
       {"8139" , "Sw\\Section1\\Switch 10 curr int."},
       {"813A" , "Sw\\Section1\\Switch 11 curr int."},
       {"813B" , "Sw\\Section1\\Switch 12 curr int."},
       {"8140" , "Sw\\Section2\\Switch 1 curr int."},
       {"8141" , "Sw\\Section2\\Switch 2 curr int."},
       {"8142" , "Sw\\Section2\\Switch 3 curr int."},
       {"8143" , "Sw\\Section2\\Switch 4 curr int."},
       {"8144" , "Sw\\Section2\\Switch 5 curr int."},
       {"8145" , "Sw\\Section2\\Switch 6 curr int."},
       {"8146" , "Sw\\Section2\\Switch 7 curr int."},
       {"8147" , "Sw\\Section2\\Switch 8 curr int."},
       {"8148" , "Sw\\Section2\\Switch 9 curr int."},
       {"8149" , "Sw\\Section2\\Switch 10 curr int."},
       {"814A" , "Sw\\Section2\\Switch 11 curr int."},
       {"814B" , "Sw\\Section2\\Switch 12 curr int."},
       {"8150" , "Sw\\Section3\\Switch 1 curr int."},
       {"8151" , "Sw\\Section3\\Switch 2 curr int."},
       {"8152" , "Sw\\Section3\\Switch 3 curr int."},
       {"8153" , "Sw\\Section3\\Switch 4 curr int."},
       {"8154" , "Sw\\Section3\\Switch 5 curr int."},
       {"8155" , "Sw\\Section3\\Switch 6 curr int."},
       {"8156" , "Sw\\Section3\\Switch 7 curr int."},
       {"8157" , "Sw\\Section3\\Switch 8 curr int."},
       {"8158" , "Sw\\Section3\\Switch 9 curr int."},
       {"8159" , "Sw\\Section3\\Switch 10 curr int."},
       {"815A" , "Sw\\Section3\\Switch 11 curr int."},
       {"815B" , "Sw\\Section3\\Switch 12 curr int."},
       {"8160" , "Sw\\Section4\\Switch 1 curr int."},
       {"8161" , "Sw\\Section4\\Switch 2 curr int."},
       {"8162" , "Sw\\Section4\\Switch 3 curr int."},
       {"8163" , "Sw\\Section4\\Switch 4 curr int."},
       {"8164" , "Sw\\Section4\\Switch 5 curr int."},
       {"8165" , "Sw\\Section4\\Switch 6 curr int."},
       {"8166" , "Sw\\Section4\\Switch 7 curr int."},
       {"8167" , "Sw\\Section4\\Switch 8 curr int."},
       {"8168" , "Sw\\Section4\\Switch 9 curr int."},
       {"8169" , "Sw\\Section4\\Switch 10 curr int."},
       {"816A" , "Sw\\Section4\\Switch 11 curr int."},
       {"816B" , "Sw\\Section4\\Switch 12 curr int."},
       {"8170" , "Sw\\Section5\\Switch 1 curr int."},
       {"8171" , "Sw\\Section5\\Switch 2 curr int."},
       {"8172" , "Sw\\Section5\\Switch 3 curr int."},
       {"8173" , "Sw\\Section5\\Switch 4 curr int."},
       {"8174" , "Sw\\Section5\\Switch 5 curr int."},
       {"8175" , "Sw\\Section5\\Switch 6 curr int."},
       {"8176" , "Sw\\Section5\\Switch 7 curr int."},
       {"8177" , "Sw\\Section5\\Switch 8 curr int."},
       {"8178" , "Sw\\Section5\\Switch 9 curr int."},
       {"8179" , "Sw\\Section5\\Switch 10 curr int."},
       {"817A" , "Sw\\Section5\\Switch 11 curr int."},
       {"817B" , "Sw\\Section5\\Switch 12 curr int."},
       {"8180" , "Sw\\Section6\\Switch 1 curr int."},
       {"8181" , "Sw\\Section6\\Switch 2 curr int."},
       {"8182" , "Sw\\Section6\\Switch 3 curr int."},
       {"8183" , "Sw\\Section6\\Switch 4 curr int."},
       {"8184" , "Sw\\Section6\\Switch 5 curr int."},
       {"8185" , "Sw\\Section6\\Switch 6 curr int."},
       {"8186" , "Sw\\Section6\\Switch 7 curr int."},
       {"8187" , "Sw\\Section6\\Switch 8 curr int."},
       {"8188" , "Sw\\Section6\\Switch 9 curr int."},
       {"8189" , "Sw\\Section6\\Switch 10 curr int."},
       {"818A" , "Sw\\Section6\\Switch 11 curr int."},
       {"818B" , "Sw\\Section6\\Switch 12 curr int."},
       {"8190" , "Sw\\Section7\\Switch 1 curr int."},
       {"8191" , "Sw\\Section7\\Switch 2 curr int."},
       {"8192" , "Sw\\Section7\\Switch 3 curr int."},
       {"8193" , "Sw\\Section7\\Switch 4 curr int."},
       {"8194" , "Sw\\Section7\\Switch 5 curr int."},
       {"8195" , "Sw\\Section7\\Switch 6 curr int."},
       {"8196" , "Sw\\Section7\\Switch 7 curr int."},
       {"8197" , "Sw\\Section7\\Switch 8 curr int."},
       {"8198" , "Sw\\Section7\\Switch 9 curr int."},
       {"8199" , "Sw\\Section7\\Switch 10 curr int."},
       {"819A" , "Sw\\Section7\\Switch 11 curr int."},
       {"819B" , "Sw\\Section7\\Switch 12 curr int."},
       {"8230" , "Sw\\Section1\\Switch 1 temp int."},
       {"8231" , "Sw\\Section1\\Switch 2 temp int."},
       {"8232" , "Sw\\Section1\\Switch 3 temp int."},
       {"8233" , "Sw\\Section1\\Switch 4 temp int."},
       {"8234" , "Sw\\Section1\\Switch 5 temp int."},
       {"8235" , "Sw\\Section1\\Switch 6 temp int."},
       {"8236" , "Sw\\Section1\\Switch 7 temp int."},
       {"8237" , "Sw\\Section1\\Switch 8 temp int."},
       {"8238" , "Sw\\Section1\\Switch 9 temp int."},
       {"8239" , "Sw\\Section1\\Switch 10 temp int."},
       {"823A" , "Sw\\Section1\\Switch 11 temp int."},
       {"823B" , "Sw\\Section1\\Switch 12 temp int."},
       {"823C" , "Sw\\Section1\\Temp sens. fail int."},
       {"8240" , "Sw\\Section2\\Switch 1 temp int."},
       {"8241" , "Sw\\Section2\\Switch 2 temp int."},
       {"8242" , "Sw\\Section2\\Switch 3 temp int."},
       {"8243" , "Sw\\Section2\\Switch 4 temp int."},
       {"8244" , "Sw\\Section2\\Switch 5 temp int."},
       {"8245" , "Sw\\Section2\\Switch 6 temp int."},
       {"8246" , "Sw\\Section2\\Switch 7 temp int."},
       {"8247" , "Sw\\Section2\\Switch 8 temp int."},
       {"8248" , "Sw\\Section2\\Switch 9 temp int."},
       {"8249" , "Sw\\Section2\\Switch 10 temp int."},
       {"824A" , "Sw\\Section2\\Switch 11 temp int."},
       {"824B" , "Sw\\Section2\\Switch 12 temp int."},
       {"824C" , "Sw\\Section2\\Temp sens. fail int."},
       {"8250" , "Sw\\Section3\\Switch 1 temp int."},
       {"8251" , "Sw\\Section3\\Switch 2 temp int."},
       {"8252" , "Sw\\Section3\\Switch 3 temp int."},
       {"8253" , "Sw\\Section3\\Switch 4 temp int."},
       {"8254" , "Sw\\Section3\\Switch 5 temp int."},
       {"8255" , "Sw\\Section3\\Switch 6 temp int."},
       {"8256" , "Sw\\Section3\\Switch 7 temp int."},
       {"8257" , "Sw\\Section3\\Switch 8 temp int."},
       {"8258" , "Sw\\Section3\\Switch 9 temp int."},
       {"8259" , "Sw\\Section3\\Switch 10 temp int."},
       {"825A" , "Sw\\Section3\\Switch 11 temp int."},
       {"825B" , "Sw\\Section3\\Switch 12 temp int."},
       {"825C" , "Sw\\Section3\\Temp sens. fail int."},
       {"8260" , "Sw\\Section4\\Switch 1 temp int."},
       {"8261" , "Sw\\Section4\\Switch 2 temp int."},
       {"8262" , "Sw\\Section4\\Switch 3 temp int."},
       {"8263" , "Sw\\Section4\\Switch 4 temp int."},
       {"8264" , "Sw\\Section4\\Switch 5 temp int."},
       {"8265" , "Sw\\Section4\\Switch 6 temp int."},
       {"8266" , "Sw\\Section4\\Switch 7 temp int."},
       {"8267" , "Sw\\Section4\\Switch 8 temp int."},
       {"8268" , "Sw\\Section4\\Switch 9 temp int."},
       {"8269" , "Sw\\Section4\\Switch 10 temp int."},
       {"826A" , "Sw\\Section4\\Switch 11 temp int."},
       {"826B" , "Sw\\Section4\\Switch 12 temp int."},
       {"826C" , "Sw\\Section4\\Temp sens. fail int."},
       {"8270" , "Sw\\Section5\\Switch 1 temp int."},
       {"8271" , "Sw\\Section5\\Switch 2 temp int."},
       {"8272" , "Sw\\Section5\\Switch 3 temp int."},
       {"8273" , "Sw\\Section5\\Switch 4 temp int."},
       {"8274" , "Sw\\Section5\\Switch 5 temp int."},
       {"8275" , "Sw\\Section5\\Switch 6 temp int."},
       {"8276" , "Sw\\Section5\\Switch 7 temp int."},
       {"8277" , "Sw\\Section5\\Switch 8 temp int."},
       {"8278" , "Sw\\Section5\\Switch 9 temp int."},
       {"8279" , "Sw\\Section5\\Switch 10 temp int."},
       {"827A" , "Sw\\Section5\\Switch 11 temp int."},
       {"827B" , "Sw\\Section5\\Switch 12 temp int."},
       {"827C" , "Sw\\Section5\\Temp sens. fail int."},
       {"8280" , "Sw\\Section6\\Switch 1 temp int."},
       {"8281" , "Sw\\Section6\\Switch 2 temp int."},
       {"8282" , "Sw\\Section6\\Switch 3 temp int."},
       {"8283" , "Sw\\Section6\\Switch 4 temp int."},
       {"8284" , "Sw\\Section6\\Switch 5 temp int."},
       {"8285" , "Sw\\Section6\\Switch 6 temp int."},
       {"8286" , "Sw\\Section6\\Switch 7 temp int."},
       {"8287" , "Sw\\Section6\\Switch 8 temp int."},
       {"8288" , "Sw\\Section6\\Switch 9 temp int."},
       {"8289" , "Sw\\Section6\\Switch 10 temp int."},
       {"828A" , "Sw\\Section6\\Switch 11 temp int."},
       {"828B" , "Sw\\Section6\\Switch 12 temp int."},
       {"828C" , "Sw\\Section6\\Temp sens. fail int."},
       {"8290" , "Sw\\Section7\\Switch 1 temp int."},
       {"8291" , "Sw\\Section7\\Switch 2 temp int."},
       {"8292" , "Sw\\Section7\\Switch 3 temp int."},
       {"8293" , "Sw\\Section7\\Switch 4 temp int."},
       {"8294" , "Sw\\Section7\\Switch 5 temp int."},
       {"8295" , "Sw\\Section7\\Switch 6 temp int."},
       {"8296" , "Sw\\Section7\\Switch 7 temp int."},
       {"8297" , "Sw\\Section7\\Switch 8 temp int."},
       {"8298" , "Sw\\Section7\\Switch 9 temp int."},
       {"8299" , "Sw\\Section7\\Switch 10 temp int."},
       {"829A" , "Sw\\Section7\\Switch 11 temp int."},
       {"829B" , "Sw\\Section7\\Switch 12 temp int."},
       {"829C" , "Sw\\Section7\\Temp sens. fail int."},
       {"8301" , "Rf\\Reflected power int."},
       {"8304" , "Rf\\Klysteron Body power int."},
       {"8306" , "Rf\\Water flow 2 int."},
       {"8307" , "Rf\\Water flow 3 int."},
       {"8308" , "Rf\\Water flow 4 int."},
       {"8309" , "Rf\\Vacuum 2 int."},
       {"830A" , "Rf\\Temp. 1 int."},
       {"830B" , "Rf\\Temp. 2 int."},
       {"830C" , "Rf\\Magn. PS external int."},
       {"830D" , "Rf\\PLC in TERM int."},
       {"8401" , "Pt\\High oil level int."},
       {"8402" , "Pt\\Cool\\Tank temp A int."},
       {"8403" , "Pt\\Cool\\Tank."},
       {"8404" , "Pt\\Cool\\Water flow 1."},
       {"8405" , "Pt\\Cool\\Water flow 2."},
       {"8406" , "Pt\\Cool\\Water flow 3."},
       {"8407" , "Pt\\Cool\\Water flow 4."},
       {"8408" , "Sys\\HwCtr\\EM switch int."},
       {"8399" , "Unassigned interlock"},
       {"8700" , "Rf\\PLC\\MagPs1\\Curr int."},
       {"8701" , "Rf\\PLC\\MagPs1\\Volt int."},
       {"8702" , "Rf\\PLC\\MagPs2\\Curr int."},
       {"8703" , "Rf\\PLC\\MagPs2\\Volt int."},
       {"8704" , "Rf\\PLC\\MagPs3\\Curr int."},
       {"8705" , "Rf\\PLC\\MagPs3\\Volt int."},
       {"8706" , "Rf\\PLC\\MagPs4\\Curr int."},
       {"8707" , "Rf\\PLC\\MagPs4\\Volt int."},
       {"8708" , "Rf\\PLC\\MagPs5\\Curr int."},
       {"8709" , "Rf\\PLC\\MagPs5\\Volt int."},
       {"870A" , "Rf\\PLC\\MagPs6\\Curr int."},
       {"870B" , "Rf\\PLC\\MagPs6\\Volt int."},
       {"870C" , "Rf\\PLC\\Ionpump Recorder int."},
       {"870D" , "Rf\\PLC\\Aiw26 int."},
       {"870E" , "Rf\\PLC\\Aiw28 int."},
       {"870F" , "Rf\\PLC\\Aiw30 int."},
       {"8710" , "Rf\\PLC\\TC1 int."},
       {"8711" , "Rf\\PLC\\TC2 int."},
       {"8712" , "Rf\\PLC\\TC3 int."},
       {"8713" , "Rf\\PLC\\TC4 int."},
       {"8714" , "Rf\\PLC\\Vacuum 2 int."},
       {"8715" , "Rf\\PLC\\TempSwitch int."},
       {"8716" , "Rf\\PLC\\I1.0 int."},
       {"8717" , "Rf\\PLC\\I1.1 int."},
       {"8718" , "Rf\\PLC\\I1.2 int."},
       {"871A" , "Rf\\PLC\\I1.4 int."},
       {"8719" , "Rf\\PLC\\I1.3 int."},
       {"871B" , "Rf\\PLC\\I1.5 int."},
       {"8800" , "Rf\\PLC sum int."},
       {"2003" , "Incoming PRF too high!"},
       {"6003" , "Incoming PRF too high!"},
       {"A003" , "Incoming PRF too high!"},
       {"D003" , "Incoming PRF too high!"},
       {"2004" , "Trig inhibited!"},
       {"5001" , "Shit state!"},
       {"5002" , "Shit state!"},
       {"5003" , "Shit state!"},
       {"5004" , "Shit state!"},
       {"6004" , "Trig inhibited!"},
       {"A004" , "Trig inhibited!"},
       {"D004" , "Trig inhibited!"},
    };
}