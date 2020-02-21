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