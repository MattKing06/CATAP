#include <IMGFactory.h>
#include <GlobalFunctions.h>
#include <PythonTypeConversions.h>
#include <boost/filesystem.hpp>

IMGFactory::IMGFactory():
	IMGFactory(STATE::OFFLINE)
{
	std::cout << "IMGFactory DEFAULT constRUCTOR called " << std::endl;
}

IMGFactory::IMGFactory(STATE mode):
	mode(mode),
	hasBeenSetup(false),
	reader(ConfigReader("IMG", mode))
{
	messenger = LoggingSystem(true, true);
	//hasBeenSetup = false;
	messenger.printDebugMessage("IMG Factory constructed");
}

IMGFactory::IMGFactory(const IMGFactory& copyIMGFactory)
	: hasBeenSetup(copyIMGFactory.hasBeenSetup),
	mode(copyIMGFactory.mode),
	messenger(copyIMGFactory.messenger),
	reader(copyIMGFactory.reader), dummyIMG(copyIMGFactory.dummyIMG)
{
	messenger.printDebugMessage("IMG copy constructor called");
	IMGMap.insert(copyIMGFactory.IMGMap.begin(), copyIMGFactory.IMGMap.end());
}

IMGFactory::~IMGFactory()
{
	messenger.printDebugMessage("IMGFactory Destructor Called");
	if (hasBeenSetup)
	{
		for (auto& img : IMGMap)
		{
			auto pvStructsList = img.second.getPVStructs();
			for (auto& pvStruct : pvStructsList)
			{
				if (pvStruct.second.monitor)
				{
					if (pvStruct.second.EVID)
					{
						img.second.epicsInterface->removeSubscription(pvStruct.second);
						ca_flush_io();
					}
				}
				if (pvStruct.second.CHID)
				{
					img.second.epicsInterface->removeChannel(pvStruct.second);
					ca_pend_io(CA_PEND_IO_TIMEOUT);
				}
			}
		}
	}
}
void IMGFactory::populateIMGMap()
{
	messenger.printDebugMessage("IMGFactory is populating the IMG map");
	if (!reader.hasMoreFilesToParse())
	{
		throw std::runtime_error("Did not receive Configuration parameters from ConfigReader, please contact support");
	}
	while (reader.hasMoreFilesToParse())
	{

		messenger.printDebugMessage("IMG Factory calling parseNextYamlFile");
		reader.parseNextYamlFile(IMGMap);
	}
	messenger.printDebugMessage("IMGFactory has finished populating the IMG map");
}
bool IMGFactory::setup(const std::string& version)
{
	messenger.printDebugMessage("called IMG Factory setup ");
	if (hasBeenSetup)
	{
		messenger.printDebugMessage("setup IMG Factory : it has been setup");
		return true;
	}
	if (mode == STATE::VIRTUAL)
	{
		messenger.printDebugMessage("VIRTUAL SETUP: TRUE");
	}
	//// epics IMG interface has been initialized in IMG constructor
	//// but we have a lot of PV information to retrieve from EPICS first
	//// so we will cycle through the PV structs, and set up their values.

	std::cout << "populateIMGMap()" << std::endl;
	populateIMGMap();
	std::cout << "populateIMGMap() fin" << std::endl;;
	if (reader.yamlFilenamesAndParsedStatusMap.empty())
	{
		hasBeenSetup = false;
		return hasBeenSetup;
	}

	setupChannels();
	EPICSInterface::sendToEPICS();
	/*
		LOOP OVER ALL IMG AGAIN TO SET MORE EPICS INFO.
	*/
	for (auto& img : IMGMap)
	{
		/*
			NOW CHANNELS HAVE BEEN SENT TO EPICS, SET UP EVERYTHING ELSE
		*/
		updateAliasNameMap(img.second);
		std::map<std::string, pvStruct>& imgPVStructs = img.second.getPVStructs();
		for (auto& pv : imgPVStructs)
		{
			
			if (ca_state(pv.second.CHID) == cs_conn)
			{
				std::cout << "cs_conn, getting some values " << std::endl;
				retrieveMonitorStatus(pv.second);
				img.second.epicsInterface->retrieveCHTYPE(pv.second);
				img.second.epicsInterface->retrieveCOUNT(pv.second);
				img.second.epicsInterface->retrieveupdateFunctionForRecord(pv.second);
				//// not sure how to set the mask from EPICS yet.
				pv.second.MASK = DBE_VALUE;
				messenger.printDebugMessage(pv.second.pvRecord, ": read", std::to_string(ca_read_access(pv.second.CHID)),
					"write", std::to_string(ca_write_access(pv.second.CHID)),
					"state", std::to_string(ca_state(pv.second.CHID)));
				if (pv.second.monitor)
				{
					img.second.epicsInterface->createSubscription(img.second, pv.second);
				}
				//// update the alias_name_map so we can use fullNames or Aliases
				EPICSInterface::sendToEPICS();
			}
			else
			{
				//std::cout << img.first << " CANNOT CONNECT TO EPICS" << std::endl;
				messenger.printMessage(img.first, " CANNOT CONNECT TO EPICS");
				//hasBeenSetup = false;
				//return hasBeenSetup;
			}
		}
	}

	hasBeenSetup = true;
	std::cout << "hasBeenSetup = true " << std::endl;
	return hasBeenSetup;
}
void IMGFactory::setupChannels()
{
	for (auto& img : IMGMap)
	{
		std::map<std::string, pvStruct>& pvStructs = img.second.getPVStructs();

		for (auto& pv : pvStructs)
		{
			img.second.epicsInterface->retrieveCHID(pv.second);
		}
	}
}
void IMGFactory::retrieveMonitorStatus(pvStruct& pvStruct)
{
	if (pvStruct.pvRecord == IMGRecords::Sta ||
		pvStruct.pvRecord == IMGRecords::Pressure)
	{
		pvStruct.monitor = true;
	}
}
IMG& IMGFactory::getIMG(const std::string& fullIMGName)
{
	std::string fullName = getFullName(fullIMGName);
	if (GlobalFunctions::entryExists(IMGMap, fullName))
	{
		return IMGMap.at(fullName);
	}
	return dummyIMG;
}
std::string IMGFactory::getFullName(const std::string& nameToCheck) const
{	
	messenger.printMessage(dummyIMG.getHardwareName());
	if (GlobalFunctions::entryExists(alias_name_map, nameToCheck))
	{
		return alias_name_map.at(nameToCheck);
	}
	return dummyIMG.getHardwareName();
}
void IMGFactory::updateAliasNameMap(const IMG& img)
{
	// first add in the IMG full name
	std::string full_name = img.getHardwareName();
	if (GlobalFunctions::entryExists(alias_name_map, full_name))
	{
		messenger.printMessage("!!ERROR!! ", full_name, " IMG name already exists! ");
	}
	else
	{
		messenger.printMessage("full_name ", full_name, " added to alias_name_map");
		alias_name_map[full_name] = full_name;
	}

	// now add in the aliases
	std::vector<std::string> aliases = img.getAliases();
	for (auto&& next_alias : aliases)
	{
		if (GlobalFunctions::entryExists(alias_name_map, next_alias))
		{
			messenger.printMessage("!!ERROR!! ", img.getHardwareName(), " alias = ", next_alias, " already exists");
		}
		else
		{
			alias_name_map[next_alias] = full_name;
			messenger.printMessage("Added alias " + next_alias + " for " + full_name);
		}
	}
}
std::vector<std::string> IMGFactory::getAllIMGNames() const
{
	std::vector<std::string> returnNames;
	for (auto&& item : IMGMap)
	{
		returnNames.push_back(item.first);
	}
	return returnNames;
}

boost::python::list IMGFactory::getAllIMGNames_Py() const
{
	return to_py_list(getAllIMGNames());
}


double IMGFactory::getIMGPressure(const std::string& name) const
{
	std::string fullName = getFullName(name);
	return IMGMap.at(fullName).getIMGPressure();
}

std::map<std::string, double> IMGFactory::getIMGPressures(const std::vector<std::string>& names) const
{
	std::map<std::string, double> return_map;
	for (auto&& name : names)
	{
		return_map[name] = getIMGPressure(name);
	}
	return return_map;
}
boost::python::dict IMGFactory::getIMGPressures_Py(const boost::python::list& names) const
{
	std::vector<std::string> namesVector = to_std_vector<std::string>(names);
	return to_py_dict(getIMGPressures(namesVector));
}
std::map<std::string, double> IMGFactory::getAllIMGPressure() const
{
	return getIMGPressures(getAllIMGNames());
}
boost::python::dict IMGFactory::getAllIMGPressure_Py() const
{
	return to_py_dict<std::string, double>(getAllIMGPressure());
}
void IMGFactory::debugMessagesOn()
{
	messenger.debugMessagesOn();
	messenger.printDebugMessage("IMG-FAC - DEBUG On");
	reader.debugMessagesOn();
	for (auto& img : IMGMap)
	{
		img.second.debugMessagesOn();
	}
}
void IMGFactory::debugMessagesOff()
{
	messenger.printDebugMessage("IMG-FAC - DEBUG OFF");
	messenger.debugMessagesOff();
	reader.debugMessagesOff();
	for (auto& img : IMGMap)
	{
		img.second.debugMessagesOff();
	}
}
void IMGFactory::messagesOn()
{
	messenger.messagesOn();
	messenger.printMessage("IMG-FAC - MESSAGES On");
	reader.messagesOn();
	for (auto& img : IMGMap)
	{
		img.second.messagesOn();
	}
}
void IMGFactory::messagesOff()
{
	messenger.printMessage("IMG-FAC - MESSAGES OFF");
	messenger.messagesOff();
	reader.messagesOff();
	for (auto& img : IMGMap)
	{
		img.second.messagesOff();
	}
}
bool IMGFactory::isDebugOn()
{
	return messenger.isDebugOn();
}
bool IMGFactory::isMessagingOn()
{
	return messenger.isMessagingOn();
}
