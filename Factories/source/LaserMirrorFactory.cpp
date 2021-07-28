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
	EPICSInterface::sendToEPICS();
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
	if (reader.yamlFilenamesAndParsedStatusMap.empty())
	{
		hasBeenSetup = false;
		return hasBeenSetup;
	}
	setupChannels();
	EPICSInterface::sendToEPICS();
	for (auto&& mirror : laserMirrorMap)
	{
		std::map<std::string, pvStruct>& mirrorPVStructs = mirror.second.getPVStructs();
		for (auto&& pv : mirrorPVStructs)
		{
			if (ca_state(pv.second.CHID) == cs_conn)
			{
				std::cout << " CONNECTED " << std::endl;
				retrieveMonitorStatus(pv.second);
				mirror.second.epicsInterface->retrieveCHTYPE(pv.second);
				mirror.second.epicsInterface->retrieveCOUNT(pv.second);
				mirror.second.epicsInterface->retrieveUpdateFunctionForRecord(pv.second);
				std::cout << "RECORD: " << pv.second.fullPVName << "/// TYPE: " << pv.second.CHTYPE << std::endl;
				pv.second.MASK = DBE_VALUE;
				if (pv.second.monitor)
				{
					std::cout << " MONITORING: " << pv.first << std::endl;
					mirror.second.epicsInterface->createSubscription(mirror.second, pv.second);
				}
			}
			else
			{
				messenger.printMessage(pv.second.fullPVName, " CANNOT CONNECT TO EPICS.");
				//hasBeenSetup = false;
				//return hasBeenSetup;
			}
		}
		EPICSInterface::sendToEPICS();
	}
	hasBeenSetup = true;
	return hasBeenSetup;
}


void LaserMirrorFactory::setHStep(const std::string& mirrorName, const double& value)
{
	if (GlobalFunctions::entryExists(laserMirrorMap, mirrorName))
	{
		laserMirrorMap.at(mirrorName).setHStep(value);
	}
	else
	{
		messenger.printMessage(mirrorName, " does not exist. Not setting H-Step.");
	}
}

void LaserMirrorFactory::setVStep(const std::string& mirrorName, const double& value)
{
	if (GlobalFunctions::entryExists(laserMirrorMap, mirrorName))
	{
		laserMirrorMap.at(mirrorName).setVStep(value);
	}
	else
	{
		messenger.printMessage(mirrorName, " does not exist. Not setting V-Step.");
	}
}

double LaserMirrorFactory::getHStep(const std::string& mirrorName)
{
	if (GlobalFunctions::entryExists(laserMirrorMap, mirrorName))
	{
		return laserMirrorMap.at(mirrorName).getHStep();
	}
	else
	{
		messenger.printMessage(mirrorName, " does not exist. Not getting H-Step.");
		return GlobalConstants::double_min;
	}
}

double LaserMirrorFactory::getVStep(const std::string& mirrorName)
{
	if (GlobalFunctions::entryExists(laserMirrorMap, mirrorName))
	{
		return laserMirrorMap.at(mirrorName).getVStep();
	}
	else
	{
		messenger.printMessage(mirrorName, " does not exist. Not getting V-Step.");
		return GlobalConstants::double_min;
	}
}

//bool LaserMirrorFactory::moveHorizontally(const std::string& mirrorName)
//{
//	if (GlobalFunctions::entryExists(laserMirrorMap, mirrorName))
//	{
//		return laserMirrorMap.at(mirrorName).moveHorizontally();
//	}
//	else
//	{
//		messenger.printMessage(mirrorName, " does not exist. Not Moving.");
//		return false;
//	}
//}
//
//bool LaserMirrorFactory::moveVertically(const std::string& mirrorName)
//{
//	if (GlobalFunctions::entryExists(laserMirrorMap, mirrorName))
//	{
//		return laserMirrorMap.at(mirrorName).moveVertically();
//	}
//	else
//	{
//		messenger.printMessage(mirrorName, " does not exist. Not Moving.");
//		return false;
//	}
//}

bool LaserMirrorFactory::moveLeft(const std::string& mirrorName)
{
	if (GlobalFunctions::entryExists(laserMirrorMap, mirrorName))
	{
		return laserMirrorMap.at(mirrorName).moveLeft();
	}
	else
	{
		messenger.printMessage(mirrorName, " does not exist. Not Moving.");
		return false;
	}
}

bool LaserMirrorFactory::moveRight(const std::string& mirrorName)
{
	if (GlobalFunctions::entryExists(laserMirrorMap, mirrorName))
	{
		return laserMirrorMap.at(mirrorName).moveRight();
	}
	else
	{
		messenger.printMessage(mirrorName, " does not exist. Not Moving.");
		return false;
	}
}

bool LaserMirrorFactory::moveUp(const std::string& mirrorName)
{
	if (GlobalFunctions::entryExists(laserMirrorMap, mirrorName))
	{
		return laserMirrorMap.at(mirrorName).moveUp();
	}
	else
	{
		messenger.printMessage(mirrorName, " does not exist. Not Moving.");
		return false;
	}
}

bool LaserMirrorFactory::moveDown(const std::string& mirrorName)
{
	if (GlobalFunctions::entryExists(laserMirrorMap, mirrorName))
	{
		return laserMirrorMap.at(mirrorName).moveDown();
	}
	else
	{
		messenger.printMessage(mirrorName, " does not exist. Not Moving.");
		return false;
	}
}

double LaserMirrorFactory::getCurrentHorizontalPosition(const std::string& mirrorName)
{
	if (GlobalFunctions::entryExists(laserMirrorMap, mirrorName))
	{
		return laserMirrorMap.at(mirrorName).getCurrentHorizontalPosition();
	}
	else
	{
		messenger.printMessage(mirrorName, " does not exist. Not getting horizontal position.");
		return GlobalConstants::double_min;
	}
}

double LaserMirrorFactory::getCurrentVerticalPosition(const std::string& mirrorName)
{
	if (GlobalFunctions::entryExists(laserMirrorMap, mirrorName))
	{
		return laserMirrorMap.at(mirrorName).getCurrentVerticalPosition();
	}
	else
	{
		messenger.printMessage(mirrorName, " does not exist. Not getting vertical position.");
		return GlobalConstants::double_min;
	}
}

bool LaserMirrorFactory::updatePositions(const std::string& mirrorName)
{
	if (GlobalFunctions::entryExists(laserMirrorMap, mirrorName))
	{
		return laserMirrorMap.at(mirrorName).updatePositions();
	}
	else
	{
		messenger.printMessage(mirrorName, " does not exist. Not updating position.");
		return GlobalConstants::double_min;
	}
}

double LaserMirrorFactory::getMaximumStepSize(const std::string& mirrorName)
{
	if (GlobalFunctions::entryExists(laserMirrorMap, mirrorName))
	{
		return laserMirrorMap.at(mirrorName).getMaximumStepSize();
	}
	else
	{
		messenger.printMessage(mirrorName, " does not exist. Not getting max step size.");
		return GlobalConstants::double_min;
	}
}

double LaserMirrorFactory::getLeftSense(const std::string& mirrorName)
{
	if (GlobalFunctions::entryExists(laserMirrorMap, mirrorName))
	{
		return laserMirrorMap.at(mirrorName).getLeftSense();
	}
	else
	{
		messenger.printMessage(mirrorName, " does not exist. Not getting left sense.");
		return GlobalConstants::double_min;
	}
}

double LaserMirrorFactory::getRightSense(const std::string& mirrorName)
{
	if (GlobalFunctions::entryExists(laserMirrorMap, mirrorName))
	{
		return laserMirrorMap.at(mirrorName).getRightSense();
	}
	else
	{
		messenger.printMessage(mirrorName, " does not exist. Not getting right sense.");
		return GlobalConstants::double_min;
	}
}

double LaserMirrorFactory::getUpSense(const std::string& mirrorName)
{
	if (GlobalFunctions::entryExists(laserMirrorMap, mirrorName))
	{
		return laserMirrorMap.at(mirrorName).getUpSense();
	}
	else
	{
		messenger.printMessage(mirrorName, " does not exist. Not getting up sense.");
		return GlobalConstants::double_min;
	}
}

double LaserMirrorFactory::getDownSense(const std::string& mirrorName)
{
	if (GlobalFunctions::entryExists(laserMirrorMap, mirrorName))
	{
		return laserMirrorMap.at(mirrorName).getDownSense();
	}
	else
	{
		messenger.printMessage(mirrorName, " does not exist. Not getting down sense.");
		return GlobalConstants::double_min;
	}
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

