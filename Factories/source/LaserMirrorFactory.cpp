#include <LaserMirrorFactory.h>

LaserMirrorFactory::LaserMirrorFactory()
{
}

LaserMirrorFactory::LaserMirrorFactory(STATE mode) :
	mode(mode),
	hasBeenSetup(false),
	reader(ConfigReader("LaserMirror", mode)),
	messenger(LoggingSystem(true, true))
{
}

LaserMirrorFactory::LaserMirrorFactory(const LaserMirrorFactory& copyFactory) :
	hasBeenSetup(copyFactory.hasBeenSetup),
	mode(copyFactory.mode),
	messenger(copyFactory.messenger),
	reader(copyFactory.reader)
{
	laserMirrorMap.insert(copyFactory.laserMirrorMap.begin(), copyFactory.laserMirrorMap.end());
}

LaserMirrorFactory::~LaserMirrorFactory()
{
	messenger.printDebugMessage("LaserHWPFactory Destructor Called");
	if (hasBeenSetup)
	{
		for (auto& mirror : laserMirrorMap)
		{
			auto pvStructsList = mirror.second.getPVStructs();
			for (auto& pvStruct : pvStructsList)
			{
				if (pvStruct.second.monitor)
				{
					if (pvStruct.second.EVID)
					{
						mirror.second.epicsInterface->removeSubscription(pvStruct.second);
						ca_flush_io();
					}
				}
				mirror.second.epicsInterface->removeChannel(pvStruct.second);
				ca_pend_io(CA_PEND_IO_TIMEOUT);
			}
		}
	}
}

void LaserMirrorFactory::populateLaserMirrorMap()
{
	if (!reader.hasMoreFilesToParse())
	{
		throw std::runtime_error("Did not receive configuration parameters from ConfigReader, please contact support");
	}
	while (reader.hasMoreFilesToParse())
	{
		reader.parseNextYamlFile(laserMirrorMap);
	}
}

void LaserMirrorFactory::retrieveMonitorStatus(pvStruct& pvStruct)
{
	if (pvStruct.pvRecord == LaserMirrorRecords::H_POS ||
		pvStruct.pvRecord == LaserMirrorRecords::V_POS)
	{
		pvStruct.monitor = true;
	}
	else
	{
		pvStruct.monitor = false;
	}
}


void LaserMirrorFactory::setupChannels()
{
	for (auto& mirror : laserMirrorMap)
	{
		std::map<std::string, pvStruct>& pvStructs = mirror.second.getPVStructs();
		for (auto& pv : pvStructs)
		{
			mirror.second.epicsInterface->retrieveCHID(pv.second);
		}
	}
}

LaserMirror& LaserMirrorFactory::getLaserMirror(const std::string& name)
{
	if (GlobalFunctions::entryExists(laserMirrorMap, name))
	{
		return laserMirrorMap.at(name);
	}
}

bool LaserMirrorFactory::setup(const std::string& version)
{
	if (hasBeenSetup)
	{
		return true;
	}
	populateLaserMirrorMap();
	setupChannels();
	EPICSInterface::sendToEPICS();
	for (auto& mirror : laserMirrorMap)
	{
		auto mirrorPVStructs = mirror.second.getPVStructs();
		for (auto& pv : mirrorPVStructs)
		{
			if (ca_state(pv.second.CHID) == cs_conn)
			{
				retrieveMonitorStatus(pv.second);
				mirror.second.epicsInterface->retrieveCHTYPE(pv.second);
				mirror.second.epicsInterface->retrieveCOUNT(pv.second);
				mirror.second.epicsInterface->retrieveUpdateFunctionForRecord(pv.second);
				pv.second.MASK = DBE_VALUE;
				if (pv.second.monitor)
				{
					mirror.second.epicsInterface->createSubscription(mirror.second, pv.second);
				}
				EPICSInterface::sendToEPICS();
			}
			else
			{
				messenger.printMessage(pv.second.fullPVName, " CANNOT CONNECT TO EPICS.");
				hasBeenSetup = false;
				return hasBeenSetup;
			}
		}
	}
	hasBeenSetup = true;
	return hasBeenSetup;
}


void LaserMirrorFactory::debugMessagesOn()
{
	messenger.debugMessagesOn();
	messenger.printDebugMessage("Laser Mirror FACTORY - DEBUG ON");
	// reader.debugMessagesOn();
	for (auto& laserMirror : laserMirrorMap)
	{
		laserMirror.second.debugMessagesOn();
	}
}

void LaserMirrorFactory::debugMessagesOff()
{
	messenger.printDebugMessage("Laser Mirror FACTORY - DEBUG OFF");
	messenger.debugMessagesOff();
	// reader.debugMessagesOff();
	for (auto& laserMirror : laserMirrorMap)
	{
		laserMirror.second.debugMessagesOff();
	}
}

void LaserMirrorFactory::messagesOn()
{
	messenger.messagesOn();
	messenger.printMessage("Laser Mirror FACTORY - MESSAGES ON");
	// reader.messagesOn();
	for (auto& laserMirror : laserMirrorMap)
	{
		laserMirror.second.messagesOn();
	}
}

void LaserMirrorFactory::messagesOff()
{
	messenger.printMessage("Laser Mirror FACTORY - MESSAGES OFF");
	messenger.messagesOff();
	// reader.messagesOff();
	for (auto& laserMirror : laserMirrorMap)
	{
		laserMirror.second.messagesOff();
	}
}

bool LaserMirrorFactory::isDebugOn()
{
	return messenger.isDebugOn();
}

bool LaserMirrorFactory::isMessagingOn()
{
	return messenger.isMessagingOn();
}

