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
	mode(mode),
	hasBeenSetup(false),
	reader(ConfigReader("Screen", mode))
{
	messenger = LoggingSystem(true, true);
	//hasBeenSetup = false;
//	messenger = LoggingSystem(false, false);
	messenger.printDebugMessage("Screen Factory constructed");
	//mode = mode;
	//reader = ConfigReader("Screen", mode);
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
	if (pvStruct.pvRecord == ScreenRecords::HSDEV ||
		pvStruct.pvRecord == ScreenRecords::HTRIGGER ||
		pvStruct.pvRecord == ScreenRecords::HEX ||
		pvStruct.pvRecord == ScreenRecords::HTGTPOS ||
		pvStruct.pvRecord == ScreenRecords::HJOG ||
		pvStruct.pvRecord == ScreenRecords::HJDIFF ||
		pvStruct.pvRecord == ScreenRecords::HMOVING ||
		pvStruct.pvRecord == ScreenRecords::HREADY ||
		pvStruct.pvRecord == ScreenRecords::HGETDEV ||
		pvStruct.pvRecord == ScreenRecords::HDEVSTATE ||
		pvStruct.pvRecord == ScreenRecords::HMAXPOS ||
		pvStruct.pvRecord == ScreenRecords::HDEVCENT ||
		pvStruct.pvRecord == ScreenRecords::HACTPOS ||
		pvStruct.pvRecord == ScreenRecords::HEN ||
		pvStruct.pvRecord == ScreenRecords::VSDEV ||
		pvStruct.pvRecord == ScreenRecords::VTRIGGER ||
		pvStruct.pvRecord == ScreenRecords::VEX ||
		pvStruct.pvRecord == ScreenRecords::VTGTPOS ||
		pvStruct.pvRecord == ScreenRecords::VJOG ||
		pvStruct.pvRecord == ScreenRecords::VJDIFF ||
		pvStruct.pvRecord == ScreenRecords::VMOVING ||
		pvStruct.pvRecord == ScreenRecords::VREADY ||
		pvStruct.pvRecord == ScreenRecords::VGETDEV ||
		pvStruct.pvRecord == ScreenRecords::VDEVSTATE ||
		pvStruct.pvRecord == ScreenRecords::VMAXPOS ||
		pvStruct.pvRecord == ScreenRecords::VDEVCENT ||
		pvStruct.pvRecord == ScreenRecords::VACTPOS ||
		pvStruct.pvRecord == ScreenRecords::VEN ||
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
	}
	else
	{
		pvStruct.monitor = false;
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
		for (auto& pv : screenPVStructs)
		{
			std::string pvAndRecordName = pv.second.fullPVName + ":" + pv.first;
			if (ca_state(pv.second.CHID) == cs_conn)
			{
				retrievemonitorStatus(pv.second);
				screen.second.epicsInterface->retrieveCHTYPE(pv.second);
				screen.second.epicsInterface->retrieveCOUNT(pv.second);
				screen.second.epicsInterface->retrieveupdateFunctionForRecord(pv.second);
				// not sure how to set the mask from EPICS yet.
				pv.second.MASK = DBE_VALUE;
				messenger.printDebugMessage(pv.second.pvRecord, ": read", std::to_string(ca_read_access(pv.second.CHID)),
					"write", std::to_string(ca_write_access(pv.second.CHID)),
					"state", std::to_string(ca_state(pv.second.CHID)));
				if (pv.second.monitor)
				{
					screen.second.epicsInterface->createSubscription(screen.second, pv.second);
				}
				EPICSInterface::sendToEPICS();
			}
			else
			{
				messenger.printMessage(screen.first, " CANNOT CONNECT TO EPICS");
				hasBeenSetup = false;
				return hasBeenSetup;
			}
		}
	}
	hasBeenSetup = true;
	return hasBeenSetup;
	std::cout << "end" << std::endl;
}

std::map<std::string, Screen> ScreenFactory::getScreens(std::vector<std::string> screenNames)
{
	std::map<std::string, Screen> selectedScreens;
	for (auto& screenName : screenNames)
	{
		selectedScreens[screenName] = screenMap.find(screenName)->second;
	}
	return selectedScreens;
}

Screen& ScreenFactory::getScreen(const std::string& fullScreenName)
{
	return screenMap.find(fullScreenName)->second;
}

std::map<std::string, STATE> ScreenFactory::getScreenStates(std::vector<std::string> names)
{
	std::map<std::string, STATE> states;
	for (auto it : names)
	{
		states[it] = getScreenState(it);
	}
	return states;
}

std::map<std::string, STATE> ScreenFactory::getScreenSetStates(std::vector<std::string> names)
{
	std::map<std::string, STATE> states;
	for (auto it : names)
	{
		states[it] = getScreenSetState(it);
	}
	return states;
}

std::map<std::string, Screen> ScreenFactory::getAllScreens()
{
	if (!hasBeenSetup)
	{
		this->setup("nominal");
	}
	else
	{
		messenger.printDebugMessage("ScreenS HAVE ALREADY BEEN constRUCTED.");
	}
	return screenMap;
}

std::map<std::string, STATE> ScreenFactory::getAllScreenStates()
{
	std::vector<std::string> names = getAllScreenNames();
	std::map<std::string, STATE> states = getScreenStates(names);
	return states;
}

std::map<std::string, STATE> ScreenFactory::getAllScreenSetStates()
{
	std::vector<std::string> names = getAllScreenNames();
	std::map<std::string, STATE> states = getScreenSetStates(names);
	return states;
}

std::vector<std::string> ScreenFactory::getAllScreenNames()
{
	std::vector<std::string> names;
	for (auto it : screenMap)
	{
		names.push_back(it.first);
	}
	return names;
}

std::string ScreenFactory::getScreenName(const std::string& name)
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

bool ScreenFactory::isHOut(const std::string& name)
{
	return screenMap.find(name)->second.isHOut();

}

bool ScreenFactory::isVOut(const std::string& name)
{
	return screenMap.find(name)->second.isVOut();
}

bool ScreenFactory::isHIn(const std::string& name)
{
	return screenMap.find(name)->second.isHIn();
}

bool ScreenFactory::isVIn(const std::string& name)
{
	return screenMap.find(name)->second.isVIn();
}

bool ScreenFactory::is_HandV_OUT(const std::string& name)
{
	return screenMap.find(name)->second.is_HandV_OUT();
}

bool ScreenFactory::isScreenIn(const std::string& name)
{
	return screenMap.find(name)->second.isScreenIn();
}

bool ScreenFactory::isHMoving(const std::string& name)
{
	return screenMap.find(name)->second.isHMoving();
}

bool ScreenFactory::isVMoving(const std::string& name)
{
	return screenMap.find(name)->second.isVMoving();
}

bool ScreenFactory::isPMoving(const std::string& name)
{
	return screenMap.find(name)->second.isPMoving();
}

bool ScreenFactory::isMoving(const std::string& name)
{
	return screenMap.find(name)->second.isMoving();
}

bool ScreenFactory::isClearForBeam(const std::string& name)
{
	return screenMap.find(name)->second.isClearForBeam();
}

bool ScreenFactory::isMover(const std::string& name)
{
	return screenMap.find(name)->second.isMover();
}

bool ScreenFactory::isVMover(const std::string& name)
{
	return screenMap.find(name)->second.isVMover();
}

bool ScreenFactory::isHVMover(const std::string& name)
{
	return screenMap.find(name)->second.isHVMover();
}

bool ScreenFactory::isPneumatic(const std::string& name)
{
	return screenMap.find(name)->second.isPneumatic();
}

bool ScreenFactory::isVELAPneumatic(const std::string& name)
{
	return screenMap.find(name)->second.isVELAPneumatic();
}

bool ScreenFactory::isVELAHVMover(const std::string& name)
{
	return screenMap.find(name)->second.isVELAHVMover();
}

bool ScreenFactory::isCLARAHVMover(const std::string& name)
{
	return screenMap.find(name)->second.isCLARAHVMover();
}
bool ScreenFactory::isCLARAVMover(const std::string& name)
{
	return screenMap.find(name)->second.isCLARAVMover();
}

STATE ScreenFactory::getScreenState(const std::string& name)
{
	return screenMap.find(name)->second.getScreenState();
}

STATE ScreenFactory::getScreenSetState(const std::string& name)
{
	return screenMap.find(name)->second.getScreenSetState();
}

std::pair< STATE, TYPE > ScreenFactory::getScreenStatePair(const std::string& name, TYPE type)
{
	return screenMap.find(name)->second.getScreenStatePair(type);
}

std::pair< STATE, TYPE > ScreenFactory::getScreenSetStatePair(const std::string& name, TYPE type)
{
	return screenMap.find(name)->second.getScreenSetStatePair(type);
}

TYPE ScreenFactory::getScreenType(const std::string& name)
{
	return screenMap.find(name)->second.getScreenType();
}

std::vector< STATE > ScreenFactory::getAvailableDevices(const std::string& name)
{
	return screenMap.find(name)->second.getAvailableDevices();
}

bool ScreenFactory::isScreenInState(const std::string& name, STATE sta)
{
	return screenMap.find(name)->second.isScreenInState(sta);
}

bool ScreenFactory::isYAGIn(const std::string& name)
{
	return screenMap.find(name)->second.isYAGIn();
}

bool ScreenFactory::isRFCageIn(const std::string& name)
{
	return screenMap.find(name)->second.isRFCageIn();
}

bool ScreenFactory::isHElement(const std::string& name, STATE e)
{
	return screenMap.find(name)->second.isHElement(e);
}

bool ScreenFactory::isVElement(const std::string& name, STATE e)
{
	return screenMap.find(name)->second.isVElement(e);
}

bool ScreenFactory::isPElement(const std::string& name, STATE e)
{
	return screenMap.find(name)->second.isPElement(e);
}

bool ScreenFactory::isHEnabled(const std::string& name)
{
	return screenMap.find(name)->second.isHEnabled();
}

bool ScreenFactory::isVEnabled(const std::string& name)
{
	return screenMap.find(name)->second.isVEnabled();
}

double ScreenFactory::getACTPOS(const std::string& name)
{
	return screenMap.find(name)->second.getACTPOS();
}

double ScreenFactory::getJDiff(const std::string& name)
{
	return screenMap.find(name)->second.getJDiff();
}

//double ScreenFactory::getDevicePosition(const std::string& name, STATE state)
//{
//	return screenMap.find(name)->second.getDevicePosition(state);
//}

double ScreenFactory::getPosition(const std::string& name)
{
	return screenMap.find(name)->second.getPosition();
}

double ScreenFactory::get_H_ACTPOS(const std::string& name)
{
	return screenMap.find(name)->second.get_H_ACTPOS();
}

double ScreenFactory::get_V_ACTPOS(const std::string& name)
{
	return screenMap.find(name)->second.get_V_ACTPOS();
}

void ScreenFactory::moveScreenTo(const std::string& name, STATE& state)
{
	screenMap.find(name)->second.moveScreenTo(state);
}

void ScreenFactory::insertYAG(const std::string& name)
{
	screenMap.find(name)->second.insertYAG();
}

void ScreenFactory::makeReadEqualSet(const std::string& name)
{
	screenMap.find(name)->second.makeReadEqualSet();
}

void ScreenFactory::makeSetEqualRead(const std::string& name)
{
	screenMap.find(name)->second.makeSetEqualRead();
}

void ScreenFactory::moveScreenOut(const std::string& name)
{
	screenMap.find(name)->second.moveScreenOut();
}

void ScreenFactory::resetPosition(const std::string& name)
{
	screenMap.find(name)->second.resetPosition();
}

void ScreenFactory::jogScreen(const std::string& name, const double jog)
{
	screenMap.find(name)->second.jogScreen(jog);
}

void ScreenFactory::setPosition(const std::string& name, const double setPos, TYPE type)
{
	screenMap.find(name)->second.setPosition(setPos, type);
}

bool ScreenFactory::setScreenSetState(const std::string& name, STATE state)
{
	return screenMap.find(name)->second.setScreenSetState(state);
}

bool ScreenFactory::setScreenSDEV(const std::string& name, STATE& state)
{
	return screenMap.find(name)->second.setScreenSDEV(state);
}

bool ScreenFactory::setScreenTrigger(const std::string& name, const int& value)
{
	return screenMap.find(name)->second.setScreenTrigger(value);
}

bool ScreenFactory::setScreenTriggerWDir(const std::string& name, const int& value, TYPE& type)
{
	return screenMap.find(name)->second.setScreenTriggerWDir(value, type);
}

bool ScreenFactory::setEX(const std::string& name, const int& value, TYPE type)
{
	return screenMap.find(name)->second.setEX(value, type);
}

bool ScreenFactory::setEN(const std::string& name, const int& value, TYPE direction)
{
	return screenMap.find(name)->second.setEN(value, direction);
}

bool ScreenFactory::setTGTPOS(const std::string& name, const double& value, TYPE direction)
{
	return screenMap.find(name)->second.setTGTPOS(value, direction);
}

boost::python::list ScreenFactory::getAllScreenNames_Py()
{
	std::vector<std::string> screens;
	screens = getAllScreenNames();
	boost::python::list newPyList = to_py_list(screens);
	return newPyList;
}

boost::python::dict ScreenFactory::getAllScreenStates_Py()
{
	std::map<std::string, STATE> screens;
	screens = getAllScreenStates();
	boost::python::dict newPyDict = to_py_dict(screens);
	return newPyDict;
}

boost::python::dict ScreenFactory::getAllScreenSetStates_Py()
{
	std::map<std::string, STATE> screens;
	screens = getAllScreenSetStates();
	boost::python::dict newPyDict = to_py_dict(screens);
	return newPyDict;
}

boost::python::dict ScreenFactory::getAllScreens_Py()
{
	std::map<std::string, Screen> screens;
	screens = getAllScreens();
	boost::python::dict newPyDict = to_py_dict(screens);
	return newPyDict;
}

boost::python::dict ScreenFactory::getScreenStates_Py(boost::python::list names)
{
	std::map<std::string, STATE> screens;
	screens = getScreenStates(to_std_vector<std::string>(names));
	boost::python::dict newPyDict = to_py_dict(screens);
	return newPyDict;
}

boost::python::dict ScreenFactory::getScreenSetStates_Py(boost::python::list names)
{
	std::map<std::string, STATE> screens;
	screens = getScreenSetStates(to_std_vector<std::string>(names));
	boost::python::dict newPyDict = to_py_dict(screens);
	return newPyDict;
}

boost::python::dict ScreenFactory::getScreens_Py(boost::python::list names)
{
	std::map<std::string, Screen> screens;
	screens = getScreens(to_std_vector<std::string>(names));
	boost::python::dict newPyDict = to_py_dict(screens);
	return newPyDict;
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
bool ScreenFactory::isDebugOn()
{
	return messenger.isDebugOn();
}
bool ScreenFactory::isMessagingOn()
{
	return messenger.isMessagingOn();
}