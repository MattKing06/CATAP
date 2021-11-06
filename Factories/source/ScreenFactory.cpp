#include "ScreenFactory.h"
#include <boost/filesystem.hpp>
#include <map>
#include <iostream>
#include <utility>
#include "GlobalFunctions.h"
#include "GlobalConstants.h"
#include <PythonTypeConversions.h>
#ifndef __CINT__
#include <cadef.h>
#endif
#include "yaml-cpp/emitter.h"

ScreenFactory::ScreenFactory() : ScreenFactory(STATE::OFFLINE)
{
	std::cout << "ScreenFactory DEFAULT constRUCTOR called " << std::endl;
}
ScreenFactory::ScreenFactory(STATE mode):
ScreenFactory(mode, MASTER_LATTICE_FILE_LOCATION)
{

}
ScreenFactory::ScreenFactory(STATE mode, const std::string& primeLatticeLocation) :
mode(mode),
hasBeenSetup(false),
reader(ConfigReader("Screen", mode, primeLatticeLocation)),
messenger(LoggingSystem(true, true))
{
	messenger.printDebugMessage("Screen Factory constructed");
}

ScreenFactory::ScreenFactory(const ScreenFactory& copyScreenFactory)
	: hasBeenSetup(copyScreenFactory.hasBeenSetup),
	mode(copyScreenFactory.mode),
	messenger(copyScreenFactory.messenger),
	reader(copyScreenFactory.reader)
{
	messenger.printDebugMessage("ScreenFactory Copy cOntructor");
	screenMap.insert(copyScreenFactory.screenMap.begin(), copyScreenFactory.screenMap.end());
}
ScreenFactory::~ScreenFactory()
{
	messenger.printDebugMessage("ScreenFactory Destructor Called");
	if (hasBeenSetup)
	{
		for (auto& screen : screenMap)
		{
			auto pvStructsList = screen.second.getPVStructs();
			for (auto& pvStruct : pvStructsList)
			{
				if (pvStruct.second.monitor)
				{
					screen.second.epicsInterface->removeSubscription(pvStruct.second);
					ca_flush_io();
				}
				screen.second.epicsInterface->removeChannel(pvStruct.second);
				ca_pend_io(CA_PEND_IO_TIMEOUT);
			}
		}
	}
}
void ScreenFactory::populateScreenMap()
{
	messenger.printDebugMessage("ScreenFactory is populating the screen map");
	if (!reader.hasMoreFilesToParse())
	{
		throw std::runtime_error("Did not receive cOnfiguratiOn parameters from ConfigReader, please contact support");
	}
	while (reader.hasMoreFilesToParse())
	{
		messenger.printDebugMessage("Screen Factory calling parseNextYamlFile");
		reader.parseNextYamlFile(screenMap);
	}
	messenger.printDebugMessage("ScreenFactory has finished populating the screen map");
}
void ScreenFactory::retrievemonitorStatus(pvStruct& pvStruct)
{
	if (pvStruct.pvRecord == ScreenRecords::H_SDEV ||
		pvStruct.pvRecord == ScreenRecords::H_TRIGGER ||
		pvStruct.pvRecord == ScreenRecords::H_EX ||
		pvStruct.pvRecord == ScreenRecords::H_TGTPOS ||
		pvStruct.pvRecord == ScreenRecords::H_JOG ||
		pvStruct.pvRecord == ScreenRecords::H_JDIFF ||
		pvStruct.pvRecord == ScreenRecords::H_MOVING ||
		pvStruct.pvRecord == ScreenRecords::H_READY ||
		pvStruct.pvRecord == ScreenRecords::H_GETDEV ||
		pvStruct.pvRecord == ScreenRecords::H_DEVSTATE ||
		pvStruct.pvRecord == ScreenRecords::H_MAXPOS ||
		pvStruct.pvRecord == ScreenRecords::H_DEVCENT ||
		pvStruct.pvRecord == ScreenRecords::H_ACTPOS ||
		pvStruct.pvRecord == ScreenRecords::H_EN ||
		pvStruct.pvRecord == ScreenRecords::V_SDEV ||
		pvStruct.pvRecord == ScreenRecords::V_TRIGGER ||
		pvStruct.pvRecord == ScreenRecords::V_EX ||
		pvStruct.pvRecord == ScreenRecords::V_TGTPOS ||
		pvStruct.pvRecord == ScreenRecords::V_JOG ||
		pvStruct.pvRecord == ScreenRecords::V_JDIFF ||
		pvStruct.pvRecord == ScreenRecords::V_MOVING ||
		pvStruct.pvRecord == ScreenRecords::V_READY ||
		pvStruct.pvRecord == ScreenRecords::V_GETDEV ||
		pvStruct.pvRecord == ScreenRecords::V_DEVSTATE ||
		pvStruct.pvRecord == ScreenRecords::V_MAXPOS ||
		pvStruct.pvRecord == ScreenRecords::V_DEVCENT ||
		pvStruct.pvRecord == ScreenRecords::V_ACTPOS ||
		pvStruct.pvRecord == ScreenRecords::V_EN ||
		pvStruct.pvRecord == ScreenRecords::SDEV ||
		pvStruct.pvRecord == ScreenRecords::TRIGGER ||
		pvStruct.pvRecord == ScreenRecords::EX ||
		pvStruct.pvRecord == ScreenRecords::TGTPOS ||
		pvStruct.pvRecord == ScreenRecords::JOG ||
		pvStruct.pvRecord == ScreenRecords::JDIFF ||
		pvStruct.pvRecord == ScreenRecords::MOVING ||
		pvStruct.pvRecord == ScreenRecords::READY ||
		pvStruct.pvRecord == ScreenRecords::GETDEV ||
		pvStruct.pvRecord == ScreenRecords::DEVSTATE ||
		pvStruct.pvRecord == ScreenRecords::MAXPOS ||
		pvStruct.pvRecord == ScreenRecords::DEVCENT ||
		pvStruct.pvRecord == ScreenRecords::ACTPOS ||
		pvStruct.pvRecord == ScreenRecords::EN
		)
	{
		pvStruct.monitor = true;
		std::cout << "retrievemonitorStatus get TRUE" << std::endl;
		messenger.printDebugMessage("retrievemonitorStatus get TRUE");
	}
	else
	{
		pvStruct.monitor = false;
		messenger.printDebugMessage("retrievemonitorStatus get FALSE");
		std::cout << "retrievemonitorStatus get FALSE" << std::endl;
	}

}
void ScreenFactory::setupChannels()
{
	for (auto& screen : screenMap)
	{
		std::map<std::string, pvStruct>& pvStructs = screen.second.getPVStructs();

		for (auto& pv : pvStructs)
		{
			screen.second.epicsInterface->retrieveCHID(pv.second);
		}
	}
}
bool ScreenFactory::setup(const std::string& VERSION)
{
	if (hasBeenSetup)
	{
		return true;
	}
	if (mode == STATE::VIRTUAL)
	{
		messenger.printDebugMessage("VIRTUAL SETUP: TRUE");
	}
	//// epics magnet interface has been initialized in Screen constructor
	//// but we have a lot of PV informatiOn to retrieve from EPICS first
	//// so we will cycle through the PV structs, and set up their values.
	populateScreenMap();

	setupChannels();
	EPICSInterface::sendToEPICS();
	for (auto& screen : screenMap)
	{
		std::map<std::string, pvStruct>& screenPVStructs = screen.second.getPVStructs();

		std::string name(screen.second.getHardwareName());
		messenger.printDebugMessage("setting up, ", screen.first, ", ", name);

		updateAliasNameMap(screen.second);

		for (auto& pv : screenPVStructs)
		{
			std::string pvAndRecordName = pv.second.fullPVName + ":" + pv.first;
			messenger.printDebugMessage("next PV  =", pvAndRecordName);
			if (ca_state(pv.second.CHID) == cs_conn)
			{
				retrievemonitorStatus(pv.second);
				screen.second.epicsInterface->retrieveCHTYPE(pv.second);
				screen.second.epicsInterface->retrieveCOUNT(pv.second);
				screen.second.epicsInterface->retrieveupdateFunctionForRecord(pv.second);
				// not sure how to set the mask from EPICS yet.
				pv.second.MASK = DBE_VALUE;
				messenger.printDebugMessage(pv.second.pvRecord, ": read=", std::to_string(ca_read_access(pv.second.CHID)),
					", write=", std::to_string(ca_write_access(pv.second.CHID)),
					", state=", std::to_string(ca_state(pv.second.CHID)));
				if (pv.second.monitor)
				{
					screen.second.epicsInterface->createSubscription(screen.second, pv.second);
				}
				else
				{
					messenger.printDebugMessage("NO createSubscription");
				}
			}
			else
			{
				messenger.printMessage(screen.first, " CANNOT CONNECT TO EPICS");
				hasBeenSetup = false;
				return hasBeenSetup;
			}
		}
	}
	EPICSInterface::sendToEPICS();
	hasBeenSetup = true;
	return hasBeenSetup;
	std::cout << "end" << std::endl;
}
void ScreenFactory::updateAliasNameMap(const Screen& screen)
{
	// first add in the magnet full name
	std::string full_name = screen.getHardwareName();
	messenger.printMessage("updateAliasNameMap ", full_name);
	if (GlobalFunctions::entryExists(alias_name_map, full_name))
	{
		// Not necessarily an error, screen_name goes in the alias map too 
		//messenger.printMessage("!!ERROR!! ", full_name, " magnet name already exists! ");
	}
	else
	{
		messenger.printMessage("full_name ", full_name, " added to alias_name_map");
		alias_name_map[full_name] = full_name;
	}
	// now add in the aliases, for cameras we have name_alias and screen_name to include 
	std::vector<std::string> name_aliases = screen.getAliases();
	for (const auto& next_alias : name_aliases)
	{
		messenger.printMessage(full_name, " next_alias =  ", next_alias);
		if (GlobalFunctions::entryExists(alias_name_map, next_alias))
		{
			messenger.printMessage("!!ERROR!! ", screen.getHardwareName(), " alias = ", next_alias, " already exists");
		}
		else
		{
			alias_name_map[next_alias] = full_name;
			messenger.printMessage("Added alias " + next_alias + " for " + full_name);
		}
	}
}


/*
///
///   __           __        ___     __   ___ ___ ___  ___  __   __
///  /__` |  |\/| |__) |    |__     / _` |__   |   |  |__  |__) /__`
///  .__/ |  |  | |    |___ |___    \__> |___  |   |  |___ |  \ .__/
///
///  simple getters for the main parameters
*/

std::string ScreenFactory::getFullName(const std::string& name_to_check) const
{
	//std::cout << "getFullName looking for " << name_to_check << std::endl;
	if (GlobalFunctions::entryExists(alias_name_map, name_to_check))
	{
		//std::cout << name_to_check << " found as " << alias_name_map.at(name_to_check) << std::endl;
		return alias_name_map.at(name_to_check);
	}
	//std::cout << name_to_check << " NOT found " << std::endl;
	return dummy_screen.getHardwareName();
}

//std::map<std::string, Screen> ScreenFactory::getScreens(std::vector<std::string> screenNames)
//{
//	std::map<std::string, Screen> selectedScreens;
//	for (auto& screenName : screenNames)
//	{
//		selectedScreens[screenName] = screenMap.find(screenName)->second;
//	}
//	return selectedScreens;
//}

Screen& ScreenFactory::getScreen(const std::string& fullScreenName)
{
	std::string full_name = getFullName(fullScreenName);
	if (GlobalFunctions::entryExists(screenMap, full_name))
	{
		return screenMap.at(full_name);
	}
	return dummy_screen;
}

std::map<std::string, STATE> ScreenFactory::getScreenStates(std::vector<std::string> names)const
{
	std::map<std::string, STATE> states;
	for (auto it : names)
	{
		states[it] = getScreenState(it);
	}
	return states;
}

std::map<std::string, STATE> ScreenFactory::getScreenSetStates(std::vector<std::string> names)const
{
	std::map<std::string, STATE> states;
	for (auto it : names)
	{
		states[it] = getScreenSetState(it);
	}
	return states;
}

//std::map<std::string, Screen> ScreenFactory::getAllScreens()
//{
//	if (!hasBeenSetup)
//	{
//		this->setup("nominal");
//	}
//	else
//	{
//		messenger.printDebugMessage("ScreenS HAVE ALREADY BEEN constRUCTED.");
//	}
//	return screenMap;
//}

std::map<std::string, STATE> ScreenFactory::getAllScreenStates()const
{
	std::vector<std::string> names = getAllScreenNames();
	std::map<std::string, STATE> states = getScreenStates(names);
	return states;
}

std::map<std::string, STATE> ScreenFactory::getAllScreenSetStates()const
{
	std::vector<std::string> names = getAllScreenNames();
	std::map<std::string, STATE> states = getScreenSetStates(names);
	return states;
}

std::vector<std::string> ScreenFactory::getAllScreenNames()const
{
	std::vector<std::string> names;
	for (auto it : screenMap)
	{
		names.push_back(it.first);
	}
	return names;
}

std::vector<std::string> ScreenFactory::getAliases(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(screenMap, full_name))
	{
		return screenMap.at(full_name).getAliases();
	}
	std::vector<std::string> r{ "UNKNOWN_NAME" };
	return r;
}
boost::python::list ScreenFactory::getAliases_Py(const std::string& name)const
{
	return to_py_list<std::string>(getAliases(name));
}
std::string ScreenFactory::getCameraName(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(screenMap, full_name))
	{
		return screenMap.at(full_name).getCameraName();
	}
}
std::map<std::string, std::string> ScreenFactory::getCameraNames(const std::vector<std::string> names)const
{
	std::map<std::string, std::string> screenAndCameraNames;
	for (auto&& name : names)
	{
		screenAndCameraNames[name] = getCameraName(name);
	}
	return screenAndCameraNames;
}
std::map<std::string, std::string> ScreenFactory::getAllCameraNames()const
{
	std::map<std::string, std::string> screenAndCameraNames;
	std::vector<std::string> names = getAllScreenNames();
	for (auto&& name : names)
	{
		screenAndCameraNames[name] = getCameraName(name);
	}
	return screenAndCameraNames;
}
boost::python::dict ScreenFactory::getCameraNames_Py(const boost::python::list names)const
{
	std::vector<std::string> py_names = to_std_vector<std::string>(names);
	return to_py_dict(getCameraNames(py_names));
}
boost::python::dict ScreenFactory::getAllCameraNames_Py()const
{
	return to_py_dict(getAllCameraNames());
}
boost::python::list ScreenFactory::getAllScreenNames_Py()const
{
	std::vector<std::string> screens;
	screens = getAllScreenNames();
	boost::python::list newPyList = to_py_list(screens);
	return newPyList;
}
boost::python::dict ScreenFactory::getAllScreenStates_Py()const
{
	std::map<std::string, STATE> screens;
	screens = getAllScreenStates();
	boost::python::dict newPyDict = to_py_dict(screens);
	return newPyDict;
}
boost::python::dict ScreenFactory::getAllScreenSetStates_Py()const
{
	std::map<std::string, STATE> screens;
	screens = getAllScreenSetStates();
	boost::python::dict newPyDict = to_py_dict(screens);
	return newPyDict;
}
boost::python::dict ScreenFactory::getScreenStates_Py(boost::python::list names)const
{
	std::map<std::string, STATE> screens;
	screens = getScreenStates(to_std_vector<std::string>(names));
	boost::python::dict newPyDict = to_py_dict(screens);
	return newPyDict;
}
boost::python::dict ScreenFactory::getScreenSetStates_Py(boost::python::list names)const
{
	std::map<std::string, STATE> screens;
	screens = getScreenSetStates(to_std_vector<std::string>(names));
	boost::python::dict newPyDict = to_py_dict(screens);
	return newPyDict;
}
std::string ScreenFactory::getScreenName(const std::string& name)const
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call ScreenFactory.setup(VERSION)");
	}
	else
	{
		return screenMap.find(name)->second.getScreenName();
	}
	return "0";
}





bool ScreenFactory::isHOut(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(screenMap, full_name))
	{
		return screenMap.at(full_name).isHOut();
	}
	return false;
}
bool ScreenFactory::isVOut(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(screenMap, full_name))
	{
		return screenMap.at(full_name).isVOut();
	}
	return false;
}
bool ScreenFactory::isHIn(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(screenMap, full_name))
	{
		return screenMap.at(full_name).isHIn();
	}
	return false;
}

bool ScreenFactory::isVIn(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(screenMap, full_name))
	{
		return screenMap.at(full_name).isVIn();
	}
	return false;
}

bool ScreenFactory::is_HandV_OUT(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(screenMap, full_name))
	{
		return screenMap.at(full_name).is_HandV_OUT();
	}
	return false;
}

bool ScreenFactory::isScreenIn(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(screenMap, full_name))
	{
		return screenMap.at(full_name).isScreenIn();
	}
	return false;
}

bool ScreenFactory::isHMoving(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(screenMap, full_name))
	{
		return screenMap.at(full_name).isHMoving();
	}
	return false;
}

bool ScreenFactory::isVMoving(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(screenMap, full_name))
	{
		return screenMap.at(full_name).isVMoving();
	}
	return false;
}

bool ScreenFactory::isPMoving(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(screenMap, full_name))
	{
		return screenMap.at(full_name).isPMoving();
	}
	return false;
}

bool ScreenFactory::isMoving(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(screenMap, full_name))
	{
		return screenMap.at(full_name).isMoving();
	}
	return false;
}

bool ScreenFactory::isClearForBeam(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(screenMap, full_name))
	{
		return screenMap.at(full_name).isClearForBeam();
	}
	return false;
}

bool ScreenFactory::isMover(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(screenMap, full_name))
	{
		return screenMap.at(full_name).isMover();
	}
	return false;
}

bool ScreenFactory::isVMover(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(screenMap, full_name))
	{
		return screenMap.at(full_name).isVMover();
	}
	return false;
}

bool ScreenFactory::isHVMover(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(screenMap, full_name))
	{
		return screenMap.at(full_name).isHVMover();
	}
	return false;
}

bool ScreenFactory::isPneumatic(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(screenMap, full_name))
	{
		return screenMap.at(full_name).isPneumatic();
	}
	return false;
}
bool ScreenFactory::isVELAPneumatic(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(screenMap, full_name))
	{
		return screenMap.at(full_name).isVELAPneumatic();
	}
	return false;
}

bool ScreenFactory::isVELAHVMover(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(screenMap, full_name))
	{
		return screenMap.at(full_name).isVELAHVMover();
	}
	return false;
}

bool ScreenFactory::isVELAVMover(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(screenMap, full_name))
	{
		return screenMap.at(full_name).isVELAVMover();
	}
	return false;
}

bool ScreenFactory::isCLARAHVMover(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(screenMap, full_name))
	{
		return screenMap.at(full_name).isCLARAHVMover();
	}
	return false;
}
bool ScreenFactory::isCLARAVMover(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(screenMap, full_name))
	{
		return screenMap.at(full_name).isCLARAVMover();
	}
	return false;
}

STATE ScreenFactory::getScreenState(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(screenMap, full_name))
	{
		return screenMap.at(full_name).getScreenState();
	}
	return STATE::UNKNOWN_NAME;
}

STATE ScreenFactory::getScreenSetState(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(screenMap, full_name))
	{
		return screenMap.at(full_name).getScreenState();
	}
	return STATE::UNKNOWN_NAME;
}

std::pair< STATE, TYPE > ScreenFactory::getScreenStatePair(const std::string& name, TYPE type)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(screenMap, full_name))
	{
		return screenMap.at(full_name).getScreenStatePair(type);
	}
	return std::make_pair(STATE::UNKNOWN_NAME, TYPE::UNKNOWN_SCREEN_TYPE);
}

std::pair< STATE, TYPE > ScreenFactory::getScreenSetStatePair(const std::string& name, TYPE type)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(screenMap, full_name))
	{
		return screenMap.at(full_name).getScreenSetStatePair(type);
	}
	return std::make_pair(STATE::UNKNOWN_NAME, TYPE::UNKNOWN_SCREEN_TYPE);
}

TYPE ScreenFactory::getScreenType(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(screenMap, full_name))
	{
		return screenMap.at(full_name).getScreenType();
	}
	return TYPE::UNKNOWN_SCREEN_TYPE;
}
bool ScreenFactory::isBusy(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(screenMap, full_name))
	{
		return screenMap.at(full_name).isBusy();
	}
	return false;
}







std::vector< STATE > ScreenFactory::getAvailableDevices(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(screenMap, full_name))
	{
		return screenMap.at(full_name).getAvailableDevices();
	}
	return std::vector < STATE>(STATE::UNKNOWN_NAME);
}


boost::python::list ScreenFactory::getAvailableDevices_Py(const std::string& name)const
{
	return to_py_list<STATE>(getAvailableDevices(name));
}



bool ScreenFactory::isScreenInState(const std::string& name, STATE sta)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(screenMap, full_name))
	{
		return screenMap.at(full_name).isScreenInState(sta);
	}
	return false;
}

bool ScreenFactory::isYAGNotIn(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(screenMap, full_name))
	{
		return screenMap.at(full_name).isYAGNotIn();
	}
	return false;
}

bool ScreenFactory::isYAGIn(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(screenMap, full_name))
	{
		return screenMap.at(full_name).isYAGIn();
	}
	return false;
}

bool ScreenFactory::isRFCageIn(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(screenMap, full_name))
	{
		return screenMap.at(full_name).isRFCageIn();
	}
	return false;
}

bool ScreenFactory::isHElement(const std::string& name, STATE e)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(screenMap, full_name))
	{
		return screenMap.at(full_name).isHElement(e);
	}
	return false;
}

bool ScreenFactory::isVElement(const std::string& name, STATE e)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(screenMap, full_name))
	{
		return screenMap.at(full_name).isVElement(e);
	}
	return false;
}

bool ScreenFactory::isPElement(const std::string& name, STATE e)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(screenMap, full_name))
	{
		return screenMap.at(full_name).isPElement(e);
	}
	return false;
}

bool ScreenFactory::isHEnabled(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(screenMap, full_name))
	{
		return screenMap.at(full_name).isHEnabled();
	}
	return false;
}

bool ScreenFactory::isVEnabled(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(screenMap, full_name))
	{
		return screenMap.at(full_name).isVEnabled();
	}
	return false;
}

double ScreenFactory::getACTPOS(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(screenMap, full_name))
	{
		return screenMap.at(full_name).getACTPOS();
	}
	return GlobalConstants::double_min;
}

double ScreenFactory::getJDiff(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(screenMap, full_name))
	{
		return screenMap.at(full_name).getJDiff();
	}
	return GlobalConstants::double_min;
}


double ScreenFactory::getPosition(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(screenMap, full_name))
	{
		return screenMap.at(full_name).getJDiff();
	}
	return GlobalConstants::double_min;
}

double ScreenFactory::get_H_ACTPOS(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(screenMap, full_name))
	{
		return screenMap.at(full_name).getJDiff();
	}
	return GlobalConstants::double_min;
}

double ScreenFactory::get_V_ACTPOS(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(screenMap, full_name))
	{
		return screenMap.at(full_name).get_V_ACTPOS();
	}
	return GlobalConstants::double_min;
}


bool ScreenFactory::moveAllScreensOut()
{
	bool r = true;
	for (const auto& screen : getAllScreenNames())
	{
		std::cout << "MOVE SCREEN OUT " << screen << std::endl;
		bool f = moveScreenOut(screen);
		if (f)
		{

		}
		else
		{
			r = false;
		}
	}
	return r;

}
bool ScreenFactory::removeAllYAG()
{
	return moveAllScreensOut();
}

bool ScreenFactory::moveScreenTo(const std::string& name, STATE state)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(screenMap, full_name))
	{
		return screenMap.at(full_name).moveScreenTo(state);
	}
	return false;
}
bool ScreenFactory::insertYAG(const std::string& name)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(screenMap, full_name))
	{
		return screenMap.at(full_name).insertYAG();
	}
	return false;
}
bool ScreenFactory::removeYAG(const std::string& name)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(screenMap, full_name))
	{
		return screenMap.at(full_name).removeYAG();
	}
	return false;
}
bool ScreenFactory::moveScreenOut(const std::string& name){	return removeYAG(name);}
bool ScreenFactory::moveScreenIn(const std::string& name){	return insertYAG(name);}


void ScreenFactory::makeReadEqualSet(const std::string& name)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(screenMap, full_name))
	{
		screenMap.at(full_name).makeReadEqualSet();
	}
}
void ScreenFactory::makeSetEqualRead(const std::string& name)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(screenMap, full_name))
	{
		screenMap.at(full_name).makeSetEqualRead();
	}
}
void ScreenFactory::resetPosition(const std::string& name)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(screenMap, full_name))
	{
		screenMap.at(full_name).resetPosition();
	}
}
void ScreenFactory::jogScreen(const std::string& name, const double jog)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(screenMap, full_name))
	{
		screenMap.at(full_name).jogScreen(jog);
	}
}
void ScreenFactory::setPosition(const std::string& name, const double setPos, TYPE type)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(screenMap, full_name))
	{
		screenMap.at(full_name).setPosition(setPos, type);
	}
}


//void ScreenFactory::monitorForNShots(const std::string& name, const size_t& value)
//{
//	if (!hasBeenSetup)
//	{
//		messenger.printDebugMessage("Please call ScreenFactory.setup(VERSION)");
//	}
//	else
//	{
//		screenMap.find(name)->second.monitorForNShots(value);
//	}
//}
//
//double ScreenFactory::getX(const std::string& name)
//{
//	if (!hasBeenSetup)
//	{
//		messenger.printDebugMessage("Please call ScreenFactory.setup(VERSION)");
//	}
//	else
//	{
//		return screenMap.find(name)->second.getX();
//	}
//	return std::numeric_limits<double>::min();;
//}
//
//double ScreenFactory::getXFromPV(const std::string& name)
//{
//	if (!hasBeenSetup)
//	{
//		messenger.printDebugMessage("Please call ScreenFactory.setup(VERSION)");
//	}
//	else
//	{
//		return screenMap.find(name)->second.getXFromPV();
//	}
//	return std::numeric_limits<double>::min();;
//}
//
//double ScreenFactory::getY(const std::string& name)
//{
//	if (!hasBeenSetup)
//	{
//		messenger.printDebugMessage("Please call ScreenFactory.setup(VERSION)");
//	}
//	else
//	{
//		return screenMap.find(name)->second.getY();
//	}
//	return std::numeric_limits<double>::min();;
//}
//
//double ScreenFactory::getYFromPV(const std::string& name)
//{
//	if (!hasBeenSetup)
//	{
//		messenger.printDebugMessage("Please call ScreenFactory.setup(VERSION)");
//	}
//	else
//	{
//		return screenMap.find(name)->second.getYFromPV();
//	}
//	return std::numeric_limits<double>::min();;
//}
//
//STATE ScreenFactory::getStatus(const std::string& name)
//{
//	if (!hasBeenSetup)
//	{
//		messenger.printDebugMessage("Please call ScreenFactory.setup(VERSION)");
//	}
//	else
//	{
//		return screenMap.find(name)->second.getStatus();
//	}
//	return STATE::UNKNOWN;
//}
//
//boost::circular_buffer< STATE > ScreenFactory::getStatusBuffer(const std::string& name)
//{
//	if (!hasBeenSetup)
//	{
//		messenger.printDebugMessage("Please call ScreenFactory.setup(VERSION)");
//	}
//	else
//	{
//		return screenMap.find(name)->second.getStatusBuffer();
//	}
//	boost::circular_buffer< STATE> statebuffer;
//	statebuffer.push_back(STATE::UNKNOWN);
//	return statebuffer;
//}
//
//std::vector< STATE > ScreenFactory::getStatusVector(const std::string& name)
//{
//	if (!hasBeenSetup)
//	{
//		messenger.printDebugMessage("Please call ScreenFactory.setup(VERSION)");
//	}
//	else
//	{
//		return screenMap.find(name)->second.getStatusVector();
//	}
//	std::vector< STATE> statevector;
//	statevector.push_back(STATE::UNKNOWN);
//	return statevector;
//}
//
//std::vector< double > ScreenFactory::getData(const std::string& name)
//{
//	if (!hasBeenSetup)
//	{
//		messenger.printDebugMessage("Please call ScreenFactory.setup(VERSION)");
//	}
//	else
//	{
//		return screenMap.find(name)->second.getData();
//	}
//	std::vector<double> vector2(9, std::numeric_limits<double>::min());
//	return vector2;
//}
//
//std::pair<double, double> ScreenFactory::getXYPosition(const std::string& name)
//{
//	if (!hasBeenSetup)
//	{
//		messenger.printDebugMessage("Please call ScreenFactory.setup(VERSION)");
//	}
//	else
//	{
//		double x = screenMap.find(name)->second.getYFromPV();
//		double y = screenMap.find(name)->second.getYFromPV();
//		return std::make_pair(x, y);
//	}
//	return std::make_pair(std::numeric_limits<double>::min(), std::numeric_limits<double>::min());
//}
//
//double ScreenFactory::getQ(const std::string& name)
//{
//	if (!hasBeenSetup)
//	{
//		messenger.printDebugMessage("Please call ScreenFactory.setup(VERSION)");
//	}
//	else
//	{
//		return screenMap.find(name)->second.getQ();
//	}
//	return std::numeric_limits<double>::min();;
//}
//
//double ScreenFactory::getPosition(const std::string& name)
//{
//	if (!hasBeenSetup)
//	{
//		messenger.printDebugMessage("Please call ScreenFactory.setup(VERSION)");
//	}
//	else
//	{
//		return screenMap.find(name)->second.getPosition();
//	}
//	return std::numeric_limits<double>::min();;
//}
//
//double ScreenFactory::getResolution(const std::string& name)
//{
//	if (!hasBeenSetup)
//	{
//		messenger.printDebugMessage("Please call ScreenFactory.setup(VERSION)");
//	}
//	else
//	{
//		return screenMap.find(name)->second.getResolution();
//	}
//	return std::numeric_limits<double>::min();;
//}
//
//std::vector< double > ScreenFactory::getXPVVector(const std::string& name)
//{
//	if (!hasBeenSetup)
//	{
//		messenger.printDebugMessage("Please call ScreenFactory.setup(VERSION)");
//	}
//	else
//	{
//		return screenMap.find(name)->second.getXPVVector();
//	}
//	std::vector<double> vector2(9, std::numeric_limits<double>::min());
//	return vector2;
//}
//
//std::vector< double > ScreenFactory::getYPVVector(const std::string& name)
//{
//	if (!hasBeenSetup)
//	{
//		messenger.printDebugMessage("Please call ScreenFactory.setup(VERSION)");
//	}
//	else
//	{
//		return screenMap.find(name)->second.getYPVVector();
//	}
//	std::vector<double> vector2(9, std::numeric_limits<double>::min());
//	return vector2;
//}
//
//std::vector< double > ScreenFactory::getQVector(const std::string& name)
//{
//	if (!hasBeenSetup)
//	{
//		messenger.printDebugMessage("Please call ScreenFactory.setup(VERSION)");
//	}
//	else
//	{
//		return screenMap.find(name)->second.getQVector();
//	}
//	std::vector<double> vector2(9, std::numeric_limits<double>::min());
//	return vector2;
//}
//
//std::vector< std::vector< double > > ScreenFactory::getDataVector(const std::string& name)
//{
//	if (!hasBeenSetup)
//	{
//		messenger.printDebugMessage("Please call ScreenFactory.setup(VERSION)");
//	}
//	else
//	{
//		return screenMap.find(name)->second.getDataVector();
//	}
//	std::vector< double > vector2(9, std::numeric_limits<double>::min());
//	std::vector< std::vector< double > > vector3(9, vector2);
//	return vector3;
//}
//
//boost::circular_buffer< double > ScreenFactory::getXPVBuffer(const std::string& name)
//{
//	if (!hasBeenSetup)
//	{
//		messenger.printDebugMessage("Please call ScreenFactory.setup(VERSION)");
//	}
//	else
//	{
//		return screenMap.find(name)->second.getXPVBuffer();
//	}
//	boost::circular_buffer<double> vector2(9, std::numeric_limits<double>::min());
//	return vector2;
//}
//
//boost::circular_buffer< double > ScreenFactory::getYPVBuffer(const std::string& name)
//{
//	if (!hasBeenSetup)
//	{
//		messenger.printDebugMessage("Please call ScreenFactory.setup(VERSION)");
//	}
//	else
//	{
//		return screenMap.find(name)->second.getYPVBuffer();
//	}
//	boost::circular_buffer<double> vector2(9, std::numeric_limits<double>::min());
//	return vector2;
//}
//
//boost::circular_buffer< double > ScreenFactory::getQBuffer(const std::string& name)
//{
//	if (!hasBeenSetup)
//	{
//		messenger.printDebugMessage("Please call ScreenFactory.setup(VERSION)");
//	}
//	else
//	{
//		return screenMap.find(name)->second.getQBuffer();
//	}
//	boost::circular_buffer<double> vector2(9, std::numeric_limits<double>::min());
//	return vector2;
//}
//
//boost::circular_buffer< std::vector< double > > ScreenFactory::getDataBuffer(const std::string& name)
//{
//	if (!hasBeenSetup)
//	{
//		messenger.printDebugMessage("Please call ScreenFactory.setup(VERSION)");
//	}
//	else
//	{
//		return screenMap.find(name)->second.getDataBuffer();
//	}
//	std::vector< double > vector2(9, std::numeric_limits<double>::min());
//	boost::circular_buffer< std::vector< double > > vector3(9, vector2);
//	return vector3;
//}
//
//bool ScreenFactory::ismonitoringXPV(const std::string& name)
//{
//	if (!hasBeenSetup)
//	{
//		messenger.printDebugMessage("Please call ScreenFactory.setup(VERSION)");
//	}
//	else
//	{
//		return screenMap.find(name)->second.ismonitoringXPV();
//	}
//	return false;
//}
//
//bool ScreenFactory::ismonitoring(const std::string& name)
//{
//	if (!hasBeenSetup)
//	{
//		messenger.printDebugMessage("Please call ScreenFactory.setup(VERSION)");
//	}
//	else
//	{
//		return screenMap.find(name)->second.ismonitoring();
//	}
//	return false;
//}
//
//bool ScreenFactory::ismonitoringYPV(const std::string& name)
//{
//	if (!hasBeenSetup)
//	{
//		messenger.printDebugMessage("Please call ScreenFactory.setup(VERSION)");
//	}
//	else
//	{
//		return screenMap.find(name)->second.ismonitoringYPV();
//	}
//	return false;
//}
//
//std::map<std::string, double> ScreenFactory::getQs(const std::vector<std::string>& names)
//{
//	std::map<std::string, double> qmap;
//	for (auto name : names)
//	{
//		double q = screenMap.find(name)->second.getQ();
//		qmap[name] = q;
//	}
//	return qmap;
//}
//
//boost::python::list ScreenFactory::getData_Py(const std::string& screenName)
//{
//	std::vector< double > data;
//	data = getData(screenName);
//	boost::python::list newPyList = to_py_list(data);
//	return newPyList;
//}

void ScreenFactory::debugMessagesOn()
{
	messenger.debugMessagesOn();
	messenger.printDebugMessage("Screen-FAC - DEBUG On");
	reader.debugMessagesOn();
	for (auto& screen : screenMap)
	{
		screen.second.debugMessagesOn();
	}
}
void ScreenFactory::debugMessagesOff()
{
	messenger.printDebugMessage("Screen-FAC - DEBUG OFF");
	messenger.debugMessagesOff();
	reader.debugMessagesOff();
	for (auto& screen : screenMap)
	{
		screen.second.debugMessagesOff();
	}
}
void ScreenFactory::messagesOn()
{
	messenger.messagesOn();
	messenger.printMessage("Screen-FAC - MESSAGES On");
	reader.messagesOn();
	for (auto& screen : screenMap)
	{
		screen.second.messagesOn();
	}
}
void ScreenFactory::messagesOff()
{
	messenger.printMessage("Screen-FAC - MESSAGES OFF");
	messenger.messagesOff();
	reader.messagesOff();
	for (auto& screen : screenMap)
	{
		screen.second.messagesOff();
	}
}
bool ScreenFactory::isDebugOn()const
{
	return messenger.isDebugOn();
}
bool ScreenFactory::isMessagingOn()const
{
	return messenger.isMessagingOn();
}





// DONT USE!!!!!!!!!!!!!!!!!!!
bool ScreenFactory::setScreenSetState(const std::string& name, STATE state)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(screenMap, full_name))
	{
		return screenMap.at(full_name).setScreenSetState(state);
	}
	return false;
}
bool ScreenFactory::setScreenSDEV(const std::string& name, STATE& state)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(screenMap, full_name))
	{
		return screenMap.at(full_name).setScreenSDEV(state);
	}
	return false;
}
bool ScreenFactory::setScreenTrigger(const std::string& name, const int& value)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(screenMap, full_name))
	{
		return screenMap.at(full_name).setScreenTrigger(value);
	}
	return false;

}
bool ScreenFactory::setScreenTriggerWDir(const std::string& name, const int& value, TYPE& type)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(screenMap, full_name))
	{
		return screenMap.at(full_name).setScreenTriggerWDir(value, type);
	}
	return false;
}
bool ScreenFactory::setEX(const std::string& name, const int& value, TYPE type)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(screenMap, full_name))
	{
		return screenMap.at(full_name).setEX(value, type);
	}
	return false;
}
bool ScreenFactory::setEN(const std::string& name, const int& value, TYPE direction)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(screenMap, full_name))
	{
		return screenMap.at(full_name).setEN(value, direction);
	}
	return false;
}
bool ScreenFactory::setTGTPOS(const std::string& name, const double& value, TYPE direction)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(screenMap, full_name))
	{
		return screenMap.at(full_name).setTGTPOS(value, direction);
	}
	return false;
}


