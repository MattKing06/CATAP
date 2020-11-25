#include "LaserHWPFactory.h"
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

LaserHWPFactory::LaserHWPFactory() : LaserHWPFactory(STATE::OFFLINE)
{
	std::cout << "LaserHWPFactory DEFAULT constRUCTOR called " << std::endl;
}
LaserHWPFactory::LaserHWPFactory(STATE mode):
	mode(mode),
	hasBeenSetup(false),
	reader(ConfigReader("LaserHWP", mode))
{
	messenger = LoggingSystem(true, true);
	//hasBeenSetup = false;
//	messenger = LoggingSystem(false, false);
	messenger.printDebugMessage("Laser HWP Factory constructed");
}
LaserHWPFactory::LaserHWPFactory(const LaserHWPFactory& copyLaserHWPFactory)
	: hasBeenSetup(copyLaserHWPFactory.hasBeenSetup),
	mode(copyLaserHWPFactory.mode),
	messenger(copyLaserHWPFactory.messenger),
	reader(copyLaserHWPFactory.reader)
{
	messenger.printDebugMessage("LaserHWPFactory Copy cOntructor");
	laserHWPMap.insert(copyLaserHWPFactory.laserHWPMap.begin(), copyLaserHWPFactory.laserHWPMap.end());
}

LaserHWPFactory::~LaserHWPFactory()
{
	messenger.printDebugMessage("LaserHWPFactory Destructor Called");
	if (hasBeenSetup)
	{
		for (auto& laser : laserHWPMap)
		{
			auto pvStructsList = laser.second.getPVStructs();
			for (auto& pvStruct : pvStructsList)
			{
				if (pvStruct.second.monitor)
				{
					laser.second.epicsInterface->removeSubscription(pvStruct.second);
					ca_flush_io();
				}
				laser.second.epicsInterface->removeChannel(pvStruct.second);
				ca_pend_io(CA_PEND_IO_TIMEOUT);
			}
		}
	}
}

void LaserHWPFactory::populateLaserHWPMap()
{
	messenger.printDebugMessage("LaserHWPFactory is populating the laser HWP map");
	if (!reader.hasMoreFilesToParse())
	{
		throw std::runtime_error("Did not receive cOnfiguratiOn parameters from ConfigReader, please contact support");
	}
	while (reader.hasMoreFilesToParse())
	{
		messenger.printDebugMessage("Laser HWP Factory calling parseNextYamlFile");
		reader.parseNextYamlFile(laserHWPMap);
	}
	messenger.printDebugMessage("LaserHWPFactory has finished populating the laser map");
}

void LaserHWPFactory::retrievemonitorStatus(pvStruct& pvStruct)
{
	if (
		pvStruct.pvRecord == LaserHWPRecords::MABS ||
		pvStruct.pvRecord == LaserHWPRecords::RPOS
		)
	{
		pvStruct.monitor = true;
	}
	else
	{
		pvStruct.monitor = false;
	}
}

void LaserHWPFactory::setupChannels()
{
	for (auto& laser : laserHWPMap)
	{
		std::map<std::string, pvStruct>& pvStructs = laser.second.getPVStructs();
		for (auto& pv : pvStructs)
		{
			laser.second.epicsInterface->retrieveCHID(pv.second);
		}
	}
}

bool LaserHWPFactory::setup(const std::string& VERSION)
{
	if (hasBeenSetup)
	{
		return true;
	}
	if (mode == STATE::VIRTUAL)
	{
		messenger.printDebugMessage("VIRTUAL SETUP: TRUE");
	}
	//// epics magnet interface has been initialized in laser constructor
	//// but we have a lot of PV informatiOn to retrieve from EPICS first
	//// so we will cycle through the PV structs, and set up their values.
	populateLaserHWPMap();

	setupChannels();
	EPICSInterface::sendToEPICS();
	for (auto& laser : laserHWPMap)
	{
		std::map<std::string, pvStruct>& laserPVStructs = laser.second.getPVStructs();
		for (auto& pv : laserPVStructs)
		{
			std::string pvAndRecordName = pv.second.fullPVName + ":" + pv.first;
			if (ca_state(pv.second.CHID) == cs_conn)
			{
				retrievemonitorStatus(pv.second);
				laser.second.epicsInterface->retrieveCHTYPE(pv.second);
				laser.second.epicsInterface->retrieveCOUNT(pv.second);
				laser.second.epicsInterface->retrieveupdateFunctionForRecord(pv.second);
				// not sure how to set the mask from EPICS yet.
				pv.second.MASK = DBE_VALUE;
				messenger.printDebugMessage(pv.second.pvRecord, ": read", std::to_string(ca_read_access(pv.second.CHID)),
					"write", std::to_string(ca_write_access(pv.second.CHID)),
					"state", std::to_string(ca_state(pv.second.CHID)));
				if (pv.second.monitor)
				{
					laser.second.epicsInterface->createSubscription(laser.second, pv.second);
				}
				EPICSInterface::sendToEPICS();
			}
			else
			{
				messenger.printMessage(laser.first, " CANNOT CONNECT TO EPICS");
				hasBeenSetup = false;
				return hasBeenSetup;
			}
		}
	}
	hasBeenSetup = true;
	return hasBeenSetup;
	std::cout << "end" << std::endl;
}

std::map<std::string, LaserHWP> LaserHWPFactory::getLaserHWPs(std::vector<std::string> laserNames)
{
	std::map<std::string, LaserHWP> selectedLasers;
	for (auto& laserName : laserNames)
	{
		selectedLasers[laserName] = laserHWPMap.find(laserName)->second;
	}
	return selectedLasers;
}

LaserHWP& LaserHWPFactory::getLaserHWP(const std::string& fullLaserName)
{
	return laserHWPMap.find(fullLaserName)->second;
}

std::map<std::string, LaserHWP> LaserHWPFactory::getAllLaserHWPs()
{
	if (!hasBeenSetup)
	{
		this->setup("nominal");
	}
	else
	{
		messenger.printDebugMessage("LASER HWPS HAVE ALREADY BEEN constRUCTED.");
	}
	return laserHWPMap;
}

std::string LaserHWPFactory::getLaserHWPName(const std::string& name)
{
	if (!hasBeenSetup)
	{
		messenger.printDebugMessage("Please call LaserHWPFactory.setup(VERSION)");
	}
	else
	{
		return laserHWPMap.find(name)->second.getLaserHWPName();
	}
	return "0";
}

bool LaserHWPFactory::setHWP(const std::string& name, double value)
{
	return laserHWPMap.find(name)->second.setHWP(value);
}

double LaserHWPFactory::getHWPSet(const std::string& name) const
{
	return laserHWPMap.find(name)->second.getHWPSet();
}

double LaserHWPFactory::getHWPRead(const std::string& name) const
{
	return laserHWPMap.find(name)->second.getHWPRead();
}

void LaserHWPFactory::debugMessagesOn()
{
	messenger.debugMessagesOn();
	messenger.printDebugMessage("LASER-HWP-FAC - DEBUG On");
	reader.debugMessagesOn();
	for (auto& laser : laserHWPMap)
	{
		laser.second.debugMessagesOn();
	}
}

void LaserHWPFactory::debugMessagesOff()
{
	messenger.printDebugMessage("LASER-HWP-FAC - DEBUG OFF");
	messenger.debugMessagesOff();
	reader.debugMessagesOff();
	for (auto& laser : laserHWPMap)
	{
		laser.second.debugMessagesOff();
	}
}

void LaserHWPFactory::messagesOn()
{
	messenger.messagesOn();
	messenger.printMessage("LASER-HWP-FAC - MESSAGES On");
	reader.messagesOn();
	for (auto& laser : laserHWPMap)
	{
		laser.second.messagesOn();
	}
}

void LaserHWPFactory::messagesOff()
{
	messenger.printMessage("LASER-HWP-FAC - MESSAGES OFF");
	messenger.messagesOff();
	reader.messagesOff();
	for (auto& laser : laserHWPMap)
	{
		laser.second.messagesOff();
	}
}

bool LaserHWPFactory::isDebugOn()
{
	return messenger.isDebugOn();
}

bool LaserHWPFactory::isMessagingOn()
{
	return messenger.isMessagingOn();
}