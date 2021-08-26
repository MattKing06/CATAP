#include <LightingFactory.h>
#include <LightingPVRecords.h>
#include <GlobalConstants.h>
#include "PythonTypeConversions.h"


LightingFactory::LightingFactory() {}


LightingFactory::LightingFactory(STATE mode):
	mode(mode),
hasBeenSetup(false),
reader(ConfigReader("Lighting", mode)),
messenger(LoggingSystem(true, true)),
dummy_light(Lighting())
{
}
LightingFactory::LightingFactory(STATE mode, const std::string& primeLatticeLocation) :
reader(ConfigReader("Lighting", mode, primeLatticeLocation))
{
}
LightingFactory::LightingFactory(const LightingFactory& copyFactory) :
	hasBeenSetup(copyFactory.hasBeenSetup),
	mode(copyFactory.mode),
	messenger(copyFactory.messenger),
	reader(copyFactory.reader)
{
}
LightingFactory::~LightingFactory()
{
	messenger.printDebugMessage("CameraFactory Destructor Called");
	if (hasBeenSetup)
	{
		for (auto& camera : lightingMap)
		{
			auto pvStructsList = camera.second.getPVStructs();
			for (auto& pvStruct : pvStructsList)
			{
				if (pvStruct.second.monitor)
				{
					if (pvStruct.second.EVID)
					{
						camera.second.epicsInterface->removeSubscription(pvStruct.second);
						ca_flush_io();
					}
				}
				camera.second.epicsInterface->removeChannel(pvStruct.second);
				ca_pend_io(CA_PEND_IO_TIMEOUT);
			}
		}
	}
}


bool LightingFactory::setup(std::string version)
{
	//LightingFactory::machineAreas = machineAreas;
	if (hasBeenSetup)
	{
		messenger.printDebugMessage("setup Magnet Factory : it has been setup");
		return true;
	}
	//std::cout << "populateMagnetMap()" << std::endl;
	messenger.printDebugMessage("LightingFactory is populating the lightingMap");
	if (!reader.hasMoreFilesToParse())
	{
		throw std::runtime_error("Did not receive configuration parameters from ConfigReader, please contact support");
	}
	while (reader.hasMoreFilesToParse())
	{
		messenger.printDebugMessage("LightingFactory calling parseNextYamlFile");
		reader.parseNextYamlFile(lightingMap);
	}
	messenger.printDebugMessage("LightingFactory has finished populating the magnet map, found ", lightingMap.size(), " Lighting objects");
	//std::cout << "populateMagnetMap() fin" << std::endl;;
	if (reader.yamlFilenamesAndParsedStatusMap.empty())
	{
		hasBeenSetup = false;
		return hasBeenSetup;
	}
	// 
	//convertConfigStringsToGlobalTypeEnums();
	setupChannels();
	EPICSInterface::sendToEPICS();
	/*
		LOOP OVER ALL MAGNETS AGAIN TO SET MORE EPICS INFO.
	*/
	for (auto& light : lightingMap)
	{
		std::cout << std::endl;
		messenger.printMessage(light.second.getHardwareName(), " Subscriptions");
		// NO ALIAS FOR THIS HARDWARE
		//updateAliasNameMap(magnet.second);
		std::map<std::string, pvStruct>& lightPVStructs = light.second.getPVStructs();
		for (auto& pv : lightPVStructs)
		{
			messenger.printMessage("Set up " + pv.first);
			if (ca_state(pv.second.CHID) == cs_conn)
			{
				//messenger.printMessage("Connected!, getting some channel data (COUNT, CHTYPE, ... )");
				setMonitorStatus(pv.second);
				light.second.epicsInterface->retrieveCHTYPE(pv.second);
				light.second.epicsInterface->retrieveCOUNT(pv.second);
				light.second.epicsInterface->retrieveupdateFunctionForRecord(pv.second);
				//// not sure how to set the mask from EPICS yet.
				pv.second.MASK = DBE_VALUE;
				messenger.printDebugMessage(pv.second.pvRecord, ": can_read = ", std::to_string(ca_read_access(pv.second.CHID)),
					", can_write = ", std::to_string(ca_write_access(pv.second.CHID)),
					", state = ", std::to_string(ca_state(pv.second.CHID)));
				if (pv.second.monitor)
				{
					light.second.epicsInterface->createSubscription(light.second, pv.second);
				}
			}
			else
			{
				messenger.printMessage(light.first, ":", pv.second.pvRecord, " CANNOT CONNECT TO EPICS");
				//hasBeenSetup = false;
				//return hasBeenSetup;
			}
		}
	}
	//int status = EPICSInterface::caFlushIO("ca_create_subscription");
	int status = EPICSInterface::sendToEPICSReturnStatus();
	//switch (status)
	//{
	//case ECA_NORMAL:
	//	messenger.printMessage("success");
	//	break;
	//case ECA_TIMEOUT:
	//	messenger.printMessage("timeout");
	//	break;
	//case ECA_BADTYPE:
	//	messenger.printMessage("Invalid DBR_XXXX type");
	//	break;
	//case ECA_BADCHID:
	//	messenger.printMessage("Corrupted CHID");
	//	break;
	//case ECA_BADCOUNT:
	//	messenger.printMessage("Requested count larger than native element count");
	//	break;
	//case ECA_GETFAIL:
	//	messenger.printMessage("A local database get failed");
	//	break;
	//case ECA_NORDACCESS:
	//	messenger.printMessage("Read access denied");
	//	break;
	//case ECA_DISCONN:
	//	messenger.printMessage("Unable to allocate memory");
	//	break;
	//default:
	//	messenger.printMessage("!!! Unexpected error while searching: ", ca_message(status));
	//}

	hasBeenSetup = true;
	//std::cout << "hasBeenSetup = true " << std::endl;
	return hasBeenSetup;


}
void LightingFactory::setupChannels()
{
	for (auto& light : lightingMap)
	{
		std::map<std::string, pvStruct>& pvStructs = light.second.getPVStructs();
		for (auto& pv : pvStructs)
		{
			// this is connecting to a CHID
			messenger.printMessage("Connect ", pv.first);
			light.second.epicsInterface->retrieveCHID(pv.second);
		}
	}
	EPICSInterface::sendToEPICS();		
}
void LightingFactory::setMonitorStatus(pvStruct& pvStruct)
{
	if (GlobalFunctions::entryExists(LightingRecords::lightMonitorRecordList, pvStruct.pvRecord))
	{
		pvStruct.monitor = true;
		messenger.printDebugMessage("monitor = true");
	}
	else
	{
		pvStruct.monitor = false;
		messenger.printDebugMessage("monitor = false");
	}
}


Lighting& LightingFactory::getLighting(const std::string& name)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(lightingMap, full_name))
	{
		return lightingMap.at(full_name);
	}
	return dummy_light;
}

std::string LightingFactory::getFullName(const std::string& name)const
{
	return name;
}


std::map<std::string, STATE> LightingFactory::getLEDState(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(lightingMap, full_name))
	{
		return lightingMap.at(full_name).getLEDState();
	}
	std::map<std::string, STATE> r;
	r[name] = STATE::UNKNOWN_NAME;
	return r;
}
boost::python::dict LightingFactory::getLEDState_Py(const std::string& name)const
{
	return to_py_dict<std::string, STATE>(getLEDState(name));
}
std::map<std::string, STATE> LightingFactory::getLightingState(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(lightingMap, full_name))
	{
		return lightingMap.at(full_name).getLightingState();
	}
	std::map<std::string, STATE> r;
	r[name] = STATE::UNKNOWN_NAME;
	return r;
}
boost::python::dict LightingFactory::getLightingState_Py(const std::string& name)const
{
	return to_py_dict<std::string, STATE>(getLightingState(name));
}
bool LightingFactory::allLEDOn(const std::string& name)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(lightingMap, full_name))
	{
		return lightingMap.at(full_name).allLEDOn();
	}
	return false;
}
bool LightingFactory::allLEDOff(const std::string& name)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(lightingMap, full_name))
	{
		return lightingMap.at(full_name).allLEDOn();
	}
	return false;
}
STATE LightingFactory::getClaraLEDState(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(lightingMap, full_name))
	{
		return lightingMap.at(full_name).getClaraLEDState();
	}
	return STATE::UNKNOWN_NAME;;
}
bool LightingFactory::setClaraLEDOn(const std::string& name)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(lightingMap, full_name))
	{
		return lightingMap.at(full_name).setClaraLEDOn();
	}
	return false;
}
bool LightingFactory::setClaraLEDOff(const std::string& name)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(lightingMap, full_name))
	{
		return lightingMap.at(full_name).setClaraLEDOff();
	}
	return false;
}
bool LightingFactory::isClaraLEDOn(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(lightingMap, full_name))
	{
		return lightingMap.at(full_name).isClaraLEDOn();
	}
	return false;
}
bool LightingFactory::isClaraLEDOff(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(lightingMap, full_name))
	{
		return lightingMap.at(full_name).isClaraLEDOff();
	}
	return false;
}
bool LightingFactory::toggleClaraLED(const std::string& name)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(lightingMap, full_name))
	{
		return lightingMap.at(full_name).toggleClaraLED();
	}
	return false;
}
STATE LightingFactory::getVelaLEDState(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(lightingMap, full_name))
	{
		return lightingMap.at(full_name).getVelaLEDState();
	}
	return STATE::UNKNOWN_NAME;
}
bool LightingFactory::setVelaLEDOn(const std::string& name)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(lightingMap, full_name))
	{
		return lightingMap.at(full_name).setVelaLEDOn();
	}
	return false;
}
bool LightingFactory::setVelaLEDOff(const std::string& name)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(lightingMap, full_name))
	{
		return lightingMap.at(full_name).setVelaLEDOff();
	}
	return false;
}
bool LightingFactory::isVelaLEDOn(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(lightingMap, full_name))
	{
		return lightingMap.at(full_name).isVelaLEDOn();
	}
	return false;
}
bool LightingFactory::isVelaLEDOff(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(lightingMap, full_name))
	{
		return lightingMap.at(full_name).isVelaLEDOff();
	}
	return false;
}
bool LightingFactory::toggleVelaLED(const std::string& name)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(lightingMap, full_name))
	{
		return lightingMap.at(full_name).toggleVelaLED();
	}
	return false;
}




void LightingFactory::debugMessagesOn()
{
	messenger.debugMessagesOn();
	messenger.printDebugMessage("Lighting Factory - DEBUG On");
	// reader.debugMessagesOn();
	for (auto& lighting : lightingMap)
	{
		lighting.second.debugMessagesOn();
	}
}
void LightingFactory::debugMessagesOff()
{
	messenger.printDebugMessage("Lighting Factory - DEBUG OFF");
	messenger.debugMessagesOff();
	// reader.debugMessagesOff();
	for (auto& lighting : lightingMap)
	{
		lighting.second.debugMessagesOff();
	}
}
void LightingFactory::messagesOn()
{
	messenger.messagesOn();
	messenger.printMessage("Lighting Factory - MESSAGES ON");
	// reader.messagesOn();
	for (auto& lighting : lightingMap)
	{
		lighting.second.messagesOn();
	}
}
void LightingFactory::messagesOff()
{
	messenger.printMessage("Lighting Factory - MESSAGES OFF");
	messenger.messagesOff();
	// reader.messagesOff();
	for (auto& lighting : lightingMap)
	{
		lighting.second.messagesOff();
	}
}
bool LightingFactory::isDebugOn()
{
	return messenger.isDebugOn();
}
bool LightingFactory::isMessagingOn()
{
	return messenger.isMessagingOn();
}