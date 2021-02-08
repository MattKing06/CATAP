#pragma once
#include <RFModulator.h>
#include <GlobalFunctions.h>


RFModulator::RFModulator()
{
}

RFModulator::RFModulator(const std::map<std::string, std::string>& paramMap, STATE mode) :
Hardware(paramMap, mode),
epicsInterface(boost::make_shared<EPICSRFModulatorInterface>(EPICSRFModulatorInterface()))
{
	messenger.printMessage("setMasterLatticeData data for: ", hardwareName);
	setMasterLatticeData();
	messenger.printMessage("constructing PV data for: ", hardwareName);
	setPVStructs();
}
RFModulator::RFModulator(const RFModulator& copyRFModulator)
{
}
RFModulator::~RFModulator(){}



void RFModulator::setMasterLatticeData()
{
	for (auto&& it : RFModulatorRecords::low_level_strings)
	{
		if (GlobalFunctions::entryExists(specificHardwareParameters, it))
		{
			low_level_strings[it];
		}
	}
	for (auto&& it : RFModulatorRecords::low_level_values)
	{
		if (GlobalFunctions::entryExists(specificHardwareParameters, it))
		{
			low_level_values[it];
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
	if (GlobalFunctions::entryExists(low_level_strings, key))
	{
		low_level_strings.at(key) = value;
	}
	messenger.printDebugMessage("!!ERROR!! ", hardwareName, " passed ",key, " , a key that does not exist");
}
void RFModulator::updateLowLevelDouble(const std::string& key, const std::pair < epicsTimeStamp, double>&  value)
{
	if (GlobalFunctions::entryExists(low_level_values, key))
	{
		low_level_values.at(key) = value;
	}
	messenger.printDebugMessage("!!ERROR!! ", hardwareName, " passed ", key, " , a key that does not exist");
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