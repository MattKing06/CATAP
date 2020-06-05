#include <CameraFactory.h>
#include "GlobalStateEnums.h"
#include "GlobalConstants.h"
#include "GlobalFunctions.h"
#include "PythonTypeConversions.h"

CameraFactory::CameraFactory()
{
}

CameraFactory::CameraFactory(STATE mode) :
mode(mode),
hasBeenSetup(false),
reader(ConfigReader("Camera", mode)),
messenger(LoggingSystem(true, true)),
machineAreas(std::vector<TYPE>{TYPE::ALL_VELA_CLARA}),
dummy_cam(Camera())
{
	messenger.printDebugMessage("LLRFFactory constructed");
}

CameraFactory::CameraFactory(const CameraFactory& copyFactory) :
hasBeenSetup(copyFactory.hasBeenSetup),
mode(copyFactory.mode),
messenger(copyFactory.messenger),
reader(copyFactory.reader),
machineAreas(machineAreas)
{
}

CameraFactory::~CameraFactory()
{
}



bool CameraFactory::setup()
{
	return setup(GlobalConstants::nominal, machineAreas);
}
bool CameraFactory::setup(const std::string& version)
{
	return setup(version, machineAreas);
}
bool CameraFactory::setup(TYPE machineArea)
{
	return setup(GlobalConstants::nominal, machineAreas);
}
bool CameraFactory::setup(const std::string& version, TYPE machineArea)
{
	return setup(GlobalConstants::nominal, std::vector<TYPE>{machineArea});
}
bool CameraFactory::setup(const std::vector<TYPE>& machineAreas)
{
	return setup(GlobalConstants::nominal, machineAreas);
}
bool CameraFactory::setup(const boost::python::list& machineAreas)
{
	return setup(GlobalConstants::nominal, to_std_vector<TYPE>(machineAreas));
}
bool CameraFactory::setup(const std::string& version, const boost::python::list& machineAreas)
{
	return setup(version, to_std_vector<TYPE>(machineAreas));
}
bool CameraFactory::setup(const std::string& version, const std::vector<TYPE>& machineAreas_IN)
{
	machineAreas = machineAreas_IN;
	// we CANNOT HAVE HRRG_GUN AND LRG_GUN, default to LRRG_GUN
	if (GlobalFunctions::entryExists(machineAreas, TYPE::HRRG_GUN))
	{
		if (GlobalFunctions::entryExists(machineAreas, TYPE::LRRG_GUN))
		{
			machineAreas.erase(std::remove(machineAreas.begin(), machineAreas.end(), TYPE::HRRG_GUN), machineAreas.end());
		}
	}

	messenger.printDebugMessage("called LLRF Factory  setup ");
	if (hasBeenSetup)
	{
		messenger.printDebugMessage("setup LLRF Factory : it has been setup");
		return true;
	}
	if (mode == STATE::VIRTUAL)
	{
		messenger.printDebugMessage("VIRTUAL SETUP: TRUE");
	}
	// epics valve interface has been initialized in valve constructor
	// but we have a lot of PV information to retrieve from EPICS first
	// so we will cycle through the PV structs, and set up their values.
	populateCameraMap();
	if (reader.yamlFilenamesAndParsedStatusMap.empty())
	{
		hasBeenSetup = false;
		return hasBeenSetup;
	}
	cutLHarwdareMapByMachineAreas();

	//setupChannels();
	//EPICSInterface::sendToEPICS();


	for (auto& item : CameraMap)
	{
		// update aliases for valve in map
		updateAliasNameMap(item.second);
		std::map<std::string, pvStruct>& pvstruct = item.second.getPVStructs();
		//	for (auto& pv : pvstruct)
		//	{
		//		// sets the monitor state in the pvstruict to true or false
		//		if (ca_state(pv.second.CHID) == cs_conn)
		//		{
		//			retrieveMonitorStatus(pv.second);
		//			valve.second.epicsInterface->retrieveCHTYPE(pv.second);
		//			valve.second.epicsInterface->retrieveCOUNT(pv.second);
		//			valve.second.epicsInterface->retrieveupdateFunctionForRecord(pv.second);
		//			//// not sure how to set the mask from EPICS yet.
		//			pv.second.MASK = DBE_VALUE;
		//			messenger.printDebugMessage(pv.second.pvRecord, ": read", std::to_string(ca_read_access(pv.second.CHID)),
		//				"write", std::to_string(ca_write_access(pv.second.CHID)),
		//				"state", std::to_string(ca_state(pv.second.CHID)));
		//			if (pv.second.monitor)
		//			{
		//				valve.second.epicsInterface->createSubscription(valve.second, pv.second);
		//				EPICSInterface::sendToEPICS();
		//			}
		//		}
		//		else
		//		{
		//			messenger.printMessage(valve.first, " CANNOT CONNECT TO EPICS");
		//		}
		//	}
	}
	hasBeenSetup = true;
	return hasBeenSetup;
}


void CameraFactory::populateCameraMap()
{
	messenger.printDebugMessage("CameraFactory is populating the Camera object map");
	if (!reader.hasMoreFilesToParse())
	{
		throw std::runtime_error("Did not receive camera configuration parameters from ConfigReader, "
			"please contact support");
	}
	while (reader.hasMoreFilesToParse())
	{
		messenger.printDebugMessage("CameraFactory calling parseNextYamlFile");
		reader.parseNextYamlFile(CameraMap);
	}
	messenger.printDebugMessage("CameraFactory has finished populating "
		"the LLRF MAP, found ", CameraMap.size(), " LLRF objects");
}



std::vector<std::string> CameraFactory::getCameraNames()
{
	std::vector<std::string> r;
	for (auto&& it : CameraMap)
	{
		r.push_back(it.first);
	}
	return r;
}
boost::python::list CameraFactory::getCameraNames_Py()
{
	return to_py_list<std::string>(getCameraNames());
}
std::string CameraFactory::getFullName(const std::string& name_to_check) const
{
	//std::cout << "getFullName looking for " << name_to_check << std::endl;
	if (GlobalFunctions::entryExists(alias_name_map, name_to_check))
	{
		//std::cout << name_to_check << " found " << std::endl;
		return alias_name_map.at(name_to_check);
	}
	//std::cout << name_to_check << " NOT found " << std::endl;
	return dummy_cam.getHardwareName();
}



std::vector<std::string> CameraFactory::getAliases(const std::string cam_name) const
{
	std::string full_name = getFullName(cam_name);
	if (GlobalFunctions::entryExists(CameraMap, full_name))
	{
		return CameraMap.at(full_name).getAliases();
	}
	return std::vector<std::string>{dummy_cam.getHardwareName()};
}
/*! get the name alises for this LLRF (python version)
	@param[out] names, python list containing all the alias names */
boost::python::list CameraFactory::getAliases_Py(const std::string cam_name) const
{
	return to_py_list<std::string>(getAliases(cam_name));
}


Camera& CameraFactory::getCamera(const std::string& cam_name)
{
	std::string full_name = getFullName(cam_name);
	if (GlobalFunctions::entryExists(CameraMap, full_name))
	{
		return CameraMap.at(full_name);
	}
	return dummy_cam;
}


void CameraFactory::updateAliasNameMap(const Camera& camera)
{
	// first add in the magnet full name
	std::string full_name = camera.getHardwareName();
	//messenger.printMessage("updateAliasNameMap ", full_name);
	if (GlobalFunctions::entryExists(alias_name_map, full_name))
	{
		//messenger.printMessage("!!ERROR!! ", full_name, " magnet name already exists! ");
	}
	else
	{
		messenger.printMessage("full_name ", full_name, " added to alias_name_map");
		alias_name_map[full_name] = full_name;
	}
	// now add in the aliases
	std::vector<std::string> aliases = camera.getAliases();
	for (auto&& next_alias : aliases)
	{
		if (GlobalFunctions::entryExists(alias_name_map, next_alias))
		{
			messenger.printMessage("!!ERROR!! ", camera.getHardwareName(), " alias = ", next_alias, " already exists");
		}
		else
		{
			alias_name_map[next_alias] = full_name;
			messenger.printMessage("Added alias " + next_alias + " for " + full_name);
		}
	}
}

void CameraFactory::cutLHarwdareMapByMachineAreas()
{
	size_t start_size = CameraMap.size();
	// loop over each magnet
	for (auto it = CameraMap.begin(); it != CameraMap.end() /* not hoisted */; /* no increment */)
	{
		// flag for if we should erase this entry, default to true 
		bool should_erase = true;
		// now we loop over every area in machineAreas and checl against isInMachineArea

		messenger.printDebugMessage(it->first, " is in area = ", ENUM_TO_STRING(it->second.getMachineArea()));

		for (auto&& machineArea : machineAreas)
		{
			// if this returns true then we should keep the LLRF and can break out the for loop 
			if (GlobalFunctions::isInMachineArea(machineArea, it->second.getMachineArea()))
			{
				should_erase = false;
				break;
			}
			else
			{

			}
		}
		// if should_erase is still true, erase object from  magnetMap
		if (should_erase)
		{
			messenger.printDebugMessage("Camera Factory erasing " + it->second.getHardwareName());
			it = CameraMap.erase(it); //  m.erase(it++);    
		}
		else
		{
			++it;
		}
	}
	size_t end_size = CameraMap.size();
	messenger.printDebugMessage("cutLLRFMapByMachineAreas CameraMap.size() went from ", start_size, " to ", end_size);
}






















void CameraFactory::debugMessagesOn()
{
	messenger.debugMessagesOn();
	messenger.printDebugMessage("CameraFactory Factory - DEBUG On");
	// reader.debugMessagesOn();
	for (auto& item : CameraMap)
	{
		item.second.debugMessagesOn();
	}
}
void CameraFactory::debugMessagesOff()
{
	messenger.printDebugMessage("CameraFactory Factory - DEBUG OFF");
	messenger.debugMessagesOff();
	// reader.debugMessagesOff();
	for (auto& item : CameraMap)
	{
		item.second.debugMessagesOff();
	}
}
void CameraFactory::messagesOn()
{
	messenger.messagesOn();
	messenger.printMessage("CameraFactory Factory - MESSAGES ON");
	// reader.messagesOn();
	for (auto& item : CameraMap)
	{
		item.second.messagesOn();
	}
}
void CameraFactory::messagesOff()
{
	messenger.printMessage("CameraFactory Factory - MESSAGES OFF");
	messenger.messagesOff();
	// reader.messagesOff();
	for (auto& item : CameraMap)
	{
		item.second.messagesOff();
	}
}
bool CameraFactory::isDebugOn()
{
	return messenger.isDebugOn();
}
bool CameraFactory::isMessagingOn()
{
	return messenger.isMessagingOn();
}