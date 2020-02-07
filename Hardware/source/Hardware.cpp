#include "Hardware.h"
#include <boost/algorithm/string.hpp>
#include <boost/make_shared.hpp>
#include <vector>
#include "GlobalFunctions.h"

Hardware::Hardware()
{
}

//Hardware::Hardware(const std::string& name):
//hardwareName(name) 
//{
//}

Hardware::Hardware(const std::map<std::string, std::string>& specificValueMap, STATE mode) :
mode(mode),
messenger(LoggingSystem(true, true)),

specificHardwareParameters(specificValueMap),
machineArea(specificValueMap.find("machine_area")->second),
hardwareType(specificValueMap.find("hardware_type")->second)
{
	// OLD METHOD	
	if (mode == STATE::VIRTUAL)
	{
		hardwareName = specificValueMap.find("virtual_name")->second.data();
	}
	else if (mode == STATE::PHYSICAL)
	{
		hardwareName = specificValueMap.find("name")->second.data();
	}
	else
	{
		mode = STATE::OFFLINE;
		hardwareName = specificValueMap.find("name")->second.data();
	}
	messenger.printDebugMessage("Constructing Hardware ", hardwareName);
	
	// equal_range returns a variable containing start (first) and end (second)
	// iterators for items in the multimap corresponding to pv records.
	if (hardwareType.compare("Magnet") != 0)
	{
		std::string pvRecordsStr = specificHardwareParameters.find(hardwareName)->second.data();
		// iterate through the list of matches and set up a pvStruct to add to pvStructs.
		std::vector<std::string> pvRecordVec;

		// split a string by commas
		boost::algorithm::split(pvRecordVec, pvRecordsStr, [](char c) {return c == ','; });

		messenger.printDebugMessage("Constructing PV information for ", hardwareName);
		for (auto record : pvRecordVec)
		{
			pvStruct pv = pvStruct();
			pv.fullPVName = hardwareName;
			pv.pvRecord = record;
			//chid, count, mask, chtype are left undefined for now.
			pvStructs[pv.pvRecord] = pv;
		}
	}

	
	
	//
	//
	//
	//
	//// equal_range returns a variable containing start (first) and end (second)
	//// iterators for items in the multimap corresponding to pv records.
	//std::string pvRecordsStr = specificHardwareParameters.find(hardwareName)->second.data();
	//// iterate through the list of matches and set up a pvStruct to add to pvStructs.
	//std::vector<std::string> pvRecordVec;

	//// split a string by commas
	//boost::algorithm::split(pvRecordVec, pvRecordsStr, [](char c) {return c == ','; });

	//messenger.printDebugMessage("Constructing PV information for ", hardwareName);
	//for (auto record : pvRecordVec)
	//{
	//	pvStruct pv = pvStruct();
	//	pv.fullPVName = hardwareName;
	//	pv.pvRecord = record;
	//	//chid, count, mask, chtype are left undefined for now.
	//	pvStructs[pv.pvRecord] = pv;
	//}

	//// NEW METHOD
	//// the full PV record goes in pv.fullPVRecord
	//if (GlobalFunctions::entryExists(specificHardwareParameters, "records"))
	//{
	//	//std::string pvRecordsStr = specificHardwareParameters.find("records")->second.data();
	//	//// iterate through the list of matches and set up a pvStruct to add to pvStructs.
	//	//std::vector<std::string> pvRecordVec;
	//	//// split a string by commas
	//	//boost::algorithm::split(pvRecordVec, pvRecordsStr, [](char c) {return c == ','; });

	//	//messenger.printDebugMessage("constructing PV information for ", hardwareName);
	//	//for (auto&& record : pvRecordVec)
	//	//{
	//	//	pvStruct pv = pvStruct();

	//	//	// This is from the OLD method, and i'm trying not to break it, by including 
	//	//	if (mode == STATE::VIRTUAL)
	//	//	{
	//	//		pv.fullPVName = specificValueMap.find("virtual_name")->second.data();
	//	//	}
	//	//	else if (mode == STATE::PHYSICAL)
	//	//	{
	//	//		pv.fullPVName = specificValueMap.find("name")->second.data();
	//	//	}


	//	//	pv.pvRecord = record;
	//	//	//chid, count, mask, chtype are left undefined for now.
	//	//	pvStructs[pv.pvRecord] = pv;
	//	//}


	//	// equal_range returns a variable cOntaining start (first) and end (second)
	//	// iterators for items in the multimap correspOnding to pv records.
	//	//std::string pvRecordsStr = specificHardwareParameters.find(hardwareName)->second.data();
	//	//// iterate through the list of matches and set up a pvStruct to add to pvStructs.
	//	//std::vector<std::string> pvRecordVec;

	//	//// split a string by commas
	//	//boost::algorithm::split(pvRecordVec, pvRecordsStr, [](char c) {return c == ','; });

	//	//messenger.printDebugMessage("constructing PV information for ", hardwareName);
	//	//for (auto record : pvRecordVec)
	//	//{
	//	//	pvStruct pv = pvStruct();
	//	//	pv.fullPVName = hardwareName;
	//	//	pv.pvRecord = record;
	//	//	//chid, count, mask, chtype are left undefined for now.
	//	//	pvStructs[pv.pvRecord] = pv;
	//	//}
	//	//messenger.printDebugMessage("Finished constructing: ", hardwareName);
	//}
}

Hardware::Hardware(const Hardware& copyHardware) :
messenger(copyHardware.messenger), hardwareType(copyHardware.hardwareType),
machineArea(copyHardware.machineArea), mode(copyHardware.mode)
{
	std::cout << "Hardware copy constructor called " << std::endl;
	pvStructs.insert(copyHardware.pvStructs.begin(), copyHardware.pvStructs.end());
	specificHardwareParameters.insert(copyHardware.specificHardwareParameters.begin(), copyHardware.specificHardwareParameters.end());
}

std::string Hardware::getMachineArea() const
{
	return machineArea;
}
std::string Hardware::getHardwareType() const
{
	return hardwareType;
}
std::string Hardware::getHardwareName() const
{
	return hardwareName;
}
std::map<std::string, pvStruct>& Hardware::getPVStructs()
{
	return pvStructs;
}
//std::map<int, pvStruct>& Hardware::getPVStructs2()
//{
//	return pvStructs2;
//}
std::map<std::string, std::string> Hardware::getSpecificHardwareParameters() const
{
	return specificHardwareParameters;
}
STATE Hardware::getMode() const
{
	return mode;
}


bool Hardware::operator==(Hardware rhs)
{
	auto LHSPVStructs = this->getPVStructs();
	auto RHSPVStructs = rhs.getPVStructs();
	if (LHSPVStructs.size() != RHSPVStructs.size())
	{
		return false;
	}
	else
	{
		// go through entries in the LHS map
		for (auto &value : LHSPVStructs)
		{
			//if our value in LHS != the value found for RHS[key]
			// we cannot have equal PV structs so we return false
			if (value.second != RHSPVStructs[value.first])
			{
				return false;
			}
		}
	}
	//otherwise, return true
	return true;
}

void Hardware::debugMessagesOn()
{
	messenger.debugMessagesOn();
	messenger.printDebugMessage(hardwareName, " - DEBUG On");
}

void Hardware::debugMessagesOff()
{
	messenger.printDebugMessage(hardwareName, "- DEBUG OFF");
	messenger.debugMessagesOff();
}

void Hardware::messagesOn()
{
	messenger.messagesOn();
	messenger.printMessage(hardwareName, " - MESSAGES On");
}

void Hardware::messagesOff()
{
	messenger.printMessage(hardwareName, " - MESSAGES OFF");
	messenger.messagesOff();
}

bool Hardware::isMessagingOn()
{
	return messenger.isMessagingOn();
}

bool Hardware::isDebugOn()
{
	return messenger.isDebugOn();
}

