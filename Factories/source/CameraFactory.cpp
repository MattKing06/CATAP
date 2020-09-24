#include "CameraFactory.h"
#include "CameraPVRecords.h"
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
	messenger.printDebugMessage("CameraFactory constructed");
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
	messenger.printDebugMessage("called Camera Factory setup ");
	if (hasBeenSetup)
	{
		messenger.printDebugMessage("setup Camera Factory: it has been setup");
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

	setupChannels();
	EPICSInterface::sendToEPICS();


	
	for (auto& item : camera_map)
	{
		messenger.printDebugMessage("setting up, " + item.first, item.second.getHardwareName());
		// update aliases for camera item in map
		updateAliasNameMap(item.second);
		
		//follow this through it seems to be empty! 
		std::map<std::string, pvStruct>& pvstruct = item.second.getPVStructs();


		


		for (auto& pv : pvstruct)
		{
			// sets the monitor state in the pvstruict to true or false
			if (ca_state(pv.second.CHID) == cs_conn)
			{
				setMonitorStatus(pv.second);
				item.second.epicsInterface->retrieveCHTYPE(pv.second);
				item.second.epicsInterface->retrieveCOUNT(pv.second);
				item.second.epicsInterface->retrieveupdateFunctionForRecord(pv.second);
				//// not sure how to set the mask from EPICS yet.
				pv.second.MASK = DBE_VALUE;
				messenger.printDebugMessage(pv.second.pvRecord, ": read = ", std::to_string(ca_read_access(pv.second.CHID)),
					"write = ", std::to_string(ca_write_access(pv.second.CHID)),
					"state = ", std::to_string(ca_state(pv.second.CHID)));
				if (pv.second.monitor)
				{
					messenger.printDebugMessage("createSubscription");
					item.second.epicsInterface->createSubscription(item.second, pv.second);
					EPICSInterface::sendToEPICS();
				}
				else
				{
					messenger.printDebugMessage("NO createSubscription");
				}
			}
			else
			{
				messenger.printMessage(item.first, " CANNOT CONNECT TO EPICS");
			}
		}
	}
	hasBeenSetup = true;
	return hasBeenSetup;
}

void CameraFactory::setMonitorStatus(pvStruct& pvStruct)
{
	using namespace CameraRecords;
	std::vector<std::string> cam_monitor_records{ HDF_WriteFile_RBV,
		HDF_WriteStatus,
		HDF_WriteMessage,
		HDF_NumCaptured_RBV,
		HDF_Capture_RBV,
		CAM_Acquire_RBV,
		HDF_NumCapture_RBV,
		MAGICK_NumCaptured_RBV,
		MAGICK_WriteFile_RBV,
		MAGICK_WriteStatus,
		MAGICK_WriteMessage,
		MAGICK_Capture_RBV,
		MAGICK_NumCapture_RBV,
		ANA_StepSize_RBV,
		ANA_EnableCallbacks_RBV,
		ANA_X_RBV,
		ANA_Y_RBV,
		ANA_SigmaX_RBV,
		ANA_SigmaY_RBV,
		ANA_CovXY_RBV,
		ANA_AvgIntensity_RBV,
		ANA_Intensity_RBV,
		ANA_XPix_RBV,
		ANA_YPix_RBV,
		ANA_SigmaXPix_RBV,
		ANA_SigmaYPix_RBV,
		ANA_CovXYPix_RBV,
		ANA_PixelResults_RBV,
		ANA_MaskXCenter_RBV,
		ANA_MaskYCenter_RBV,
		ANA_MaskXRad_RBV,
		ANA_MaskYRad_RBV,
		ANA_CenterX_RBV,
		ANA_CenterY_RBV,
		ANA_PixMM_RBV,
		CAM_AcquireTime_RBV,
		CAM_AcquirePeriod_RBV,
		CAM_ArrayRate_RBV,
		CAM_Temperature_RBV,
		ANA_UseNPoint };

	if (std::find(cam_monitor_records.begin(), cam_monitor_records.end(), pvStruct.pvRecord) != cam_monitor_records.end())
	{
		pvStruct.monitor = true;
	}
	else
	{
		pvStruct.monitor = false;
	}
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
		reader.parseNextYamlFile(camera_map);
	}
	messenger.printDebugMessage("CameraFactory has finished populating "
		"the Camera MAP, found ", camera_map.size(), " Camera  objects");
}
void CameraFactory::setupChannels()
{
	for (auto& cam : camera_map)
	{
		std::map<std::string, pvStruct>& pvStructs = cam.second.getPVStructs();
		for (auto& pv : pvStructs)
		{
			messenger.printMessage(cam.first,", retrieveCHID ", pv.first);
			cam.second.epicsInterface->retrieveCHID(pv.second);
		}
	}
}



double CameraFactory::pix2mmX(const std::string& name, double value)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).pix2mmX(value);
	}
	return GlobalConstants::double_min;
}
double CameraFactory::pix2mmY(const std::string& name, double value)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).pix2mmY(value);
	}
	return GlobalConstants::double_min;
}
double CameraFactory::mm2pixX(const std::string& name, double value)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).mm2pixX(value);
	}
	return GlobalConstants::double_min;
}
double CameraFactory::mm2pixY(const std::string& name, double value)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).mm2pixY(value);
	}
	return GlobalConstants::double_min;
}

double CameraFactory::getpix2mmX(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getpix2mmX();
	}
	return GlobalConstants::double_min;
}
double CameraFactory::getpix2mmY(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getpix2mmY();
	}
	return GlobalConstants::double_min;
}
double CameraFactory::setpix2mmX(const std::string& name, double value)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).setpix2mmX(value);
	}
	return GlobalConstants::double_min;
}
double CameraFactory::setpix2mmY(const std::string& name, double value)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).setpix2mmY(value);
	}
	return GlobalConstants::double_min;
}


double CameraFactory::getX(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getX();
	}
	return GlobalConstants::double_min;
}
double CameraFactory::getY(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getY();
	}
	return GlobalConstants::double_min;
}
double CameraFactory::getSigX(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getSigX();
	}
	return GlobalConstants::double_min;
}
double CameraFactory::getSigY(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getSigY();
	}
	return GlobalConstants::double_min;
}
double CameraFactory::getSigXY(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getSigXY();
	}
	return GlobalConstants::double_min;
}
double CameraFactory::getXPix(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getXPix();
	}
	return GlobalConstants::double_min;
}
double CameraFactory::getYPix(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getYPix();
	}
	return GlobalConstants::double_min;
}
double CameraFactory::getSigXPix(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getSigXPix();
	}
	return GlobalConstants::double_min;
}
double CameraFactory::getSigYPix(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getSigYPix();
	}
	return GlobalConstants::double_min;
}
double CameraFactory::getSigXYPix(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getSigXYPix();
	}
	return GlobalConstants::double_min;
}

bool CameraFactory::setX(const std::string& name, double value)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).setX(value);
	}
	return false;
}
bool CameraFactory::setY(const std::string& name, double value)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).setY(value);
	}
	return false;
}
bool CameraFactory::setSigX(const std::string& name, double value)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).setSigX(value);
	}
	return false;
}
bool CameraFactory::setSigY(const std::string& name, double value)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).setSigY(value);
	}
	return false;
}
bool CameraFactory::setSigXY(const std::string& name, double value)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).setSigXY(value);
	}
	return false;
}
//bool CameraFactory::setXPix(const std::string& name, double value)
//{
//	std::string full_name = getFullName(name);
//	if (GlobalFunctions::entryExists(camera_map, full_name))
//	{
//		return camera_map.at(full_name).setXPix(value);
//	}
//	return false;
//}
//bool CameraFactory::setYPix(const std::string& name, double value)
//{
//	std::string full_name = getFullName(name);
//	if (GlobalFunctions::entryExists(camera_map, full_name))
//	{
//		return camera_map.at(full_name).setYPix(value);
//	}
//	return false;
//}
//bool CameraFactory::setSigXPix(const std::string& name, double value)
//{
//	std::string full_name = getFullName(name);
//	if (GlobalFunctions::entryExists(camera_map, full_name))
//	{
//		return camera_map.at(full_name).setSigXPix(value);
//	}
//	return false;
//}
//bool CameraFactory::setSigYPix(const std::string& name, double value)
//{
//	std::string full_name = getFullName(name);
//	if (GlobalFunctions::entryExists(camera_map, full_name))
//	{
//		return camera_map.at(full_name).setSigYPix(value);
//	}
//	return false;
//}
//bool CameraFactory::setSigXYPix(const std::string& name, double value)
//{
//	std::string full_name = getFullName(name);
//	if (GlobalFunctions::entryExists(camera_map, full_name))
//	{
//		return camera_map.at(full_name).setSigXYPix(value);
//	}
//	return false;
//}

double CameraFactory::getSumIntensity(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getSumIntensity();
	}
	return GlobalConstants::double_min;
}
double CameraFactory::getAvgIntensity(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getAvgIntensity();
	}
	return GlobalConstants::double_min;
}
bool CameraFactory::setSumIntensity(const std::string& name, double value)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).setSumIntensity(value);
	}
	return false;
}
bool CameraFactory::setAvgIntensity(const std::string& name, double value)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).setAvgIntensity(value);
	}
	return false;
}

bool CameraFactory::stopAcquiring(const std::string& cam_name)
{
	std::string full_name = getFullName(cam_name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).stopAcquiring();
	}
	return false;
}

bool CameraFactory::stopAllAcquiringExceptVC()
{
	bool r = true;
	for (auto&& cam : camera_map)
	{
		if (GlobalFunctions::entryExists<std::string>(cam.second.getScreenNames(), "VIRTUAL_CATHODE")) // MAGIC STRING
		{
		}
		else
		{
			bool t = cam.second.stopAcquiring();
			if (!t)
			{
				r = false;
			}
		}
	}
	return r;
}

bool CameraFactory::stopAllAcquiring()
{
	bool r = true;
	for (auto&& cam : camera_map)
	{
		bool t = cam.second.stopAcquiring();
		if (!t)
		{
			r = false;
		}
	}
	return r;
}
bool CameraFactory::startAcquiring(const std::string& cam_name, bool stop_all )
{
	bool r = false;
	std::string full_name = getFullName(cam_name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		if (stop_all)
		{
			r = stopAllAcquiringExceptVC();
			if (r)
			{
				r = camera_map.at(full_name).startAcquiring();
			}
		}
		else
		{
			r = camera_map.at(full_name).startAcquiring();
		}
	}
	return r;
}






std::vector<std::string> CameraFactory::getCameraNames()
{
	std::vector<std::string> r;
	for (auto&& it : camera_map)
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



std::string CameraFactory::getScreen(const std::string cam_name) const
{
	std::string full_name = getFullName(cam_name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getScreen();
	}
	return "UNKNOWN";
}
std::vector<std::string> CameraFactory::getScreenNames(const std::string cam_name) const
{
	std::string full_name = getFullName(cam_name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getScreenNames();
	}
	return std::vector<std::string>{"UNKNOWN"};
}
boost::python::list CameraFactory::getScreenNames_Py(const std::string cam_name) const
{
	return to_py_list<std::string>(getScreenNames(cam_name));
}


std::vector<std::string> CameraFactory::getAliases(const std::string cam_name) const
{
	std::string full_name = getFullName(cam_name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getAliases();
	}
	return std::vector<std::string>{dummy_cam.getHardwareName()};
}
/*! get the name alises for this LLRF (python version)
	@param[ou	return to_py_list<std::string>(getAliases(cam_name));t] names, python list containing all the alias names */
boost::python::list CameraFactory::getAliases_Py(const std::string cam_name) const
{
	return to_py_list<std::string>(getAliases(cam_name));
}


Camera& CameraFactory::getCamera(const std::string& cam_name)
{
	std::string full_name = getFullName(cam_name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name);
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
		// Not necessarily an error, screen_name goes in the alias map too 
		//messenger.printMessage("!!ERROR!! ", full_name, " magnet name already exists! ");
	}
	else
	{
		messenger.printMessage("full_name ", full_name, " added to alias_name_map");
		alias_name_map[full_name] = full_name;
	}
	// now add in the aliases, for cameras we have name_alias and screen_name to include 
	std::vector<std::string> name_aliases = camera.getAliases();
	std::vector<std::string> screen_aliases = camera.getScreenNames();
	std::vector<std::string> aliases;
	aliases.reserve(name_aliases.size() + screen_aliases.size());
	aliases.insert(aliases.end(), name_aliases.begin(), name_aliases.end());
	aliases.insert(aliases.end(), screen_aliases.begin(), screen_aliases.end());
	
	const char* const delim = ", ";

	std::ostringstream imploded;
	std::copy(aliases.begin(), aliases.end(),std::ostream_iterator<std::string>(imploded, delim));

	messenger.printMessage("Aliases = ", imploded.str());


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
	size_t start_size = camera_map.size();
	// loop over each magnet
	for (auto it = camera_map.begin(); it != camera_map.end() /* not hoisted */; /* no increment */)
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
			it = camera_map.erase(it); //  m.erase(it++);    
		}
		else
		{
			++it;
		}
	}
	// a bit of cancer below ...
	// now check if there are two virtual_cathdoe cameras, if so remove thje VELA one (arbitrary decision, but meh)
	auto it_vela = camera_map.end();
	auto it_clara = camera_map.end();
	for (auto it = camera_map.begin(); it != camera_map.end() /* not hoisted */; /* no increment */)
	{
		if (it->second.getMachineArea() == TYPE::VELA_LASER)
		{
			//messenger.printDebugMessage("Found VELA Virtual Cathdoe Camera");
			it_vela = it;
		}
		if (it->second.getMachineArea() == TYPE::CLARA_LASER)
		{
			//messenger.printDebugMessage("Found CLARA Virtual Cathdoe Camera");
			it_clara = it;
		}
		++it;
	}
	if (it_vela != camera_map.end())
	{
		if (it_clara != camera_map.end())
		{
			camera_map.erase(it_vela);
			messenger.printDebugMessage("Still 2 Virtual_cathode Cameras, deleteing the VELA one");
		}
	}

	size_t end_size = camera_map.size();
	messenger.printDebugMessage("cutLHarwdareMapByMachineAreas camera_map.size() went from ", start_size, " to ", end_size);
}









void CameraFactory::debugMessagesOn()
{
	messenger.debugMessagesOn();
	messenger.printDebugMessage("CameraFactory Factory - DEBUG On");
	// reader.debugMessagesOn();
	for (auto& item : camera_map)
	{
		item.second.debugMessagesOn();
	}
}
void CameraFactory::debugMessagesOff()
{
	messenger.printDebugMessage("CameraFactory Factory - DEBUG OFF");
	messenger.debugMessagesOff();
	// reader.debugMessagesOff();
	for (auto& item : camera_map)
	{
		item.second.debugMessagesOff();
	}
}
void CameraFactory::messagesOn()
{
	messenger.messagesOn();
	messenger.printMessage("CameraFactory Factory - MESSAGES ON");
	// reader.messagesOn();
	for (auto& item : camera_map)
	{
		item.second.messagesOn();
	}
}
void CameraFactory::messagesOff()
{
	messenger.printMessage("CameraFactory Factory - MESSAGES OFF");
	messenger.messagesOff();
	// reader.messagesOff();
	for (auto& item : camera_map)
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