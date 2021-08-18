#include "CameraFactory.h"
#include "CameraPVRecords.h"
#include "GlobalStateEnums.h"
#include "GlobalConstants.h"
#include "GlobalFunctions.h"
#include "PythonTypeConversions.h"
#include "SnapshotFileManager.h"
#include <algorithm>
#include <exception>

CameraFactory::CameraFactory()
{
}

CameraFactory::CameraFactory(STATE mode) :
	mode(mode),
	hasBeenSetup(false),
	reader(ConfigReader("Camera", mode)),
	messenger(LoggingSystem(true, true)),
	machineAreas(std::vector<TYPE>{TYPE::UNKNOWN_AREA}),
	dummy_cam(Camera())
{
	messenger.printDebugMessage("CameraFactory constructed");
}
CameraFactory::CameraFactory(const CameraFactory& copyFactory) :
	hasBeenSetup(copyFactory.hasBeenSetup),
	mode(copyFactory.mode),
	messenger(copyFactory.messenger),
	reader(copyFactory.reader),
	machineAreas(copyFactory.machineAreas)
{
}
CameraFactory::~CameraFactory()
{
	messenger.printDebugMessage("CameraFactory Destructor Called");
	if (hasBeenSetup)
	{
		for (auto& camera : camera_map)
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

bool CameraFactory::setup(){	return setup(GlobalConstants::nominal);}
bool CameraFactory::setup(const std::string& version){	return setup(version, TYPE::ALL_VELA_CLARA);}
bool CameraFactory::setup(TYPE machineArea){	return setup(GlobalConstants::nominal, machineArea);}
bool CameraFactory::setup(const std::string& version, TYPE machineArea){	return setup(GlobalConstants::nominal, std::vector<TYPE>{machineArea});}
bool CameraFactory::setup(const std::vector<TYPE>& machineAreas){	return setup(GlobalConstants::nominal, machineAreas);}
bool CameraFactory::setup(const boost::python::list& machineAreas){	return setup(GlobalConstants::nominal, to_std_vector<TYPE>(machineAreas));}
bool CameraFactory::setup(const std::string& version, const boost::python::list& machineAreas){	return setup(version, to_std_vector<TYPE>(machineAreas));}
bool CameraFactory::setup(const std::string& version, const std::vector<TYPE>& machineAreas_IN)
{
	messenger.printDebugMessage("setup Camera Factory with vector of machine areas");
	machineAreas = machineAreas_IN;
	if (hasBeenSetup)
	{
		messenger.printDebugMessage("setup Camera Factory: it has been setup");
		return true;
	}
	if (mode == STATE::VIRTUAL)
	{
		messenger.printDebugMessage("VIRTUAL SETUP: TRUE");
	}
	populateCameraMap();
	if (reader.yamlFilenamesAndParsedStatusMap.empty())
	{
		hasBeenSetup = false;
		return hasBeenSetup;
	}
	for (auto& item : camera_map)
	{
		std::string name(item.second.getHardwareName());
		messenger.printDebugMessage("calling updateAliasNameMap for , ", item.first, ", ", name);
		// update aliases for camera item in map
		updateAliasNameMap(item.second);
	}
	messenger.printDebugMessage("setup by areas calling cutLHarwdareMapByMachineAreas");
	cutLHarwdareMapByMachineAreas();
	// TODOD the below needs to be a seperate fun ction as i have copied and pasted this into the otehr setup by names function !
	messenger.printDebugMessage("setup by areas calling setupChannels");
	setupChannels();
	messenger.printDebugMessage("setup by areas calling sendToEPICS");
	//EPICSInterface::sendToEPICS();
	EPICSInterface::sendToEPICSm("CameraFactory");
	for (auto& item : camera_map)
	{
		std::string name(item.second.getHardwareName());
		messenger.printDebugMessage("setting up, ", item.first, ", ", name);
		// update aliases for camera item in map
		//updateAliasNameMap(item.second);

		//follow this through it seems to be empty! 
		std::map<std::string, pvStruct>& pvstruct = item.second.getPVStructs();

		for (auto&& pv : pvstruct)
		{
			messenger.printDebugMessage("PVRecord = ", pv.second.pvRecord);
			// sets the monitor state in the pvstruict to true or false
			if (ca_state(pv.second.CHID) == cs_conn)
			{
				setMonitorStatus(pv.second);
				item.second.epicsInterface->retrieveCHTYPE(pv.second);
				item.second.epicsInterface->retrieveCOUNT(pv.second);
				item.second.epicsInterface->retrieveupdateFunctionForRecord(pv.second);
				//// not sure how to set the mask from EPICS yet.
				pv.second.MASK = DBE_VALUE;
				//messenger.printDebugMessage(item.second.getHardwareName()),
				messenger.printDebugMessage(pv.second.pvRecord, " r, w, s = ", std::to_string(ca_read_access(pv.second.CHID)),
					std::to_string(ca_write_access(pv.second.CHID)), std::to_string(ca_state(pv.second.CHID)));
				if (pv.second.monitor)
				{
					//messenger.printDebugMessage("createSubscription");
					item.second.epicsInterface->createSubscription(item.second, pv.second);
				}
				else
				{
					messenger.printDebugMessage("NO createSubscription");
				}
			}
			else
			{
				messenger.printMessage(item.first, ", ", pv.second.pvRecord, " CANNOT CONNECT TO EPICS");
			}
		}
		EPICSInterface::sendToEPICSm("CameraFactory connect PVs");
	}
	messenger.printDebugMessage("Finished Setting up EPICS channels, caput default values ");
	try
	{
		caputMasterLatticeParametersAfterSetup();
	}
	catch(const std::out_of_range& error)
	{
		std::cout << "ERROR" << std::endl;
	}
		
	hasBeenSetup = true;
	return hasBeenSetup;
}
bool CameraFactory::setup(const std::string& version, const std::vector<std::string>& names)
{
	messenger.printDebugMessage("setup Camera Factory with vector of camera names");
	if (hasBeenSetup)
	{
		messenger.printDebugMessage("setup Camera Factory: it has been setup");
		return true;
	}
	if (mode == STATE::VIRTUAL)
	{
		messenger.printDebugMessage("VIRTUAL SETUP: TRUE");
	}
	messenger.printDebugMessage("setup_names calling populateCameraMap");	
	populateCameraMap();
	messenger.printDebugMessage("setup_names calling populateCameraMap finished");
	if (reader.yamlFilenamesAndParsedStatusMap.empty())
	{
		hasBeenSetup = false;
		return hasBeenSetup;
	}
	for (auto& item : camera_map)
	{
		std::string name(item.second.getHardwareName());
		messenger.printDebugMessage("setting up, ", item.first, ", ", name);
		// update aliases for camera item in map
		updateAliasNameMap(item.second);
	}
	messenger.printDebugMessage("setup by names calling cutLHarwdareMapByNames");
	cutLHarwdareMapByNames(names);
	messenger.printDebugMessage("setup by names calling setupChannels");
	setupChannels();
	messenger.printDebugMessage("setup by names calling sendToEPICS");
	EPICSInterface::sendToEPICS();
	messenger.printDebugMessage("setup by names doign detailed setup and connections");
	for (auto& item : camera_map)
	{
		std::string name(item.second.getHardwareName());
		messenger.printDebugMessage("setting up, ", item.first, ", ", name);
		// update aliases for camera item in map
		// updateAliasNameMap(item.second);

		//follow this through it seems to be empty! 
		std::map<std::string, pvStruct>& pvstruct = item.second.getPVStructs();
		messenger.printDebugMessage("Start Setting up EPICS channels ");
		for (auto&& pv : pvstruct)
		{
			messenger.printDebugMessage("Check CHID ");
			// sets the monitor state in the pvstruict to true or false
			if (ca_state(pv.second.CHID) == cs_conn)
			{
				messenger.printDebugMessage("CHID connected");
				setMonitorStatus(pv.second);
				messenger.printDebugMessage("CHID connected");

				item.second.epicsInterface->retrieveCHTYPE(pv.second);
				item.second.epicsInterface->retrieveCOUNT(pv.second);
				item.second.epicsInterface->retrieveupdateFunctionForRecord(pv.second);
				//// not sure how to set the mask from EPICS yet.
				pv.second.MASK = DBE_VALUE;
				//messenger.printDebugMessage(item.second.getHardwareName()),
				messenger.printDebugMessage(pv.second.pvRecord, " r, w, s = ", std::to_string(ca_read_access(pv.second.CHID)),
					std::to_string(ca_write_access(pv.second.CHID)), std::to_string(ca_state(pv.second.CHID)));
				if (pv.second.monitor)
				{
					//messenger.printDebugMessage("createSubscription");
					item.second.epicsInterface->createSubscription(item.second, pv.second);
				}
				else
				{
					messenger.printDebugMessage("NO createSubscription");
				}
			}
			else
			{
				messenger.printMessage(item.first, ", ", pv.second.pvRecord, " CANNOT CONNECT TO EPICS");
			}
		}
		messenger.printDebugMessage("Finished Setting up EPICS channels ");
		EPICSInterface::sendToEPICS();
	}
	messenger.printDebugMessage("Finished Setting up EPICS channels, caput default values ");
	caputMasterLatticeParametersAfterSetup();
	hasBeenSetup = true;
	return hasBeenSetup;
}
void CameraFactory::caputMasterLatticeParametersAfterSetup()
{
	messenger.printMessage("caputMasterLatticeParametersAfterSetup");
	for (auto& cam: camera_map)
	{
		messenger.printMessage(cam.first, " setCentreXPixel to ", cam.second.master_lattice_centre_x);
		cam.second.setCentreXPixel(cam.second.master_lattice_centre_x);
		messenger.printMessage(cam.first, " getCentreYPixel to ", cam.second.master_lattice_centre_y);
		cam.second.setCentreYPixel(cam.second.master_lattice_centre_y);
		messenger.printMessage(cam.first, " setPixelToMM to ", cam.second.master_lattice_pixel_to_mm);
		cam.second.setPixelToMM(cam.second.master_lattice_pixel_to_mm);

	}
}
void CameraFactory::setMonitorStatus(pvStruct& pvStruct)
{
	messenger.printMessage("setMonitorStatus checking ", pvStruct.pvRecord);
	if (std::find(CameraRecords::cameraMonitorRecordsList.begin(), CameraRecords::cameraMonitorRecordsList.end(), pvStruct.pvRecord) != CameraRecords::cameraMonitorRecordsList.end())
	{
		pvStruct.monitor = true;
		messenger.printMessage("setMonitorStatus ", pvStruct.pvRecord, ", status = true");
	}
	else
	{
		pvStruct.monitor = false;
		messenger.printMessage("setMonitorStatus ", pvStruct.pvRecord, ", status = false ");
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
//
//                   ___  __  
//  |\ |  /\   |\/| |__  /__` 
//  | \| /~~\  |  | |___ .__/ 
//
std::vector<std::string> CameraFactory::getAliases(const std::string& name) const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getAliases();
	}
	return std::vector<std::string>{name + " DOES NOT EXIST"};
}
boost::python::list CameraFactory::getAliases_Py(const std::string& name) const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getAliases_Py();
	}
	return to_py_list<std::string>(std::vector<std::string>{name + " DOES NOT EXIST"});
}
std::vector<std::string> CameraFactory::getScreenNames(const std::string& name) const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getScreenNames();
	}
	return std::vector<std::string>{name + " DOES NOT EXIST"};
}
boost::python::list CameraFactory::getScreenNames_Py(const std::string& name) const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getScreenNames_Py();
	}
	return to_py_list<std::string>(std::vector<std::string>{name + " DOES NOT EXIST"});
}
std::string CameraFactory::getScreen(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getScreen();
	}
	return name + " DOES NOT EXIST";
}
TYPE CameraFactory::getCamType(const std::string & name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getCamType();
	}
	return TYPE::UNKNOWN_TYPE;
}
// 
//  __         ___         ___  __                 
// |__) | \_/ |__  |        |  /  \     |\/|  |\/| 
// |    | / \ |___ |___     |  \__/     |  |  |  | 
//  
double CameraFactory::getPixelToMM(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getPixelToMM();
	}
	return GlobalConstants::double_min;
}
bool CameraFactory::setPixelToMM(const std::string& name, double value)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).setPixelToMM(value);
	}
	return false;
}
double CameraFactory::pix2mm(const std::string& name, double value)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).pix2mm(value);
	}
	return GlobalConstants::double_min;
}
double CameraFactory::mm2pix(const std::string& name, double value)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).mm2pix(value);
	}
	return GlobalConstants::double_min;
}
double CameraFactory::pix2mmX(const std::string& name, double value)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getpix2mmX();
	}
	return GlobalConstants::double_min;
}
double CameraFactory::pix2mmY(const std::string& name, double value)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getpix2mmX();
	}
	return GlobalConstants::double_min;
}
double CameraFactory::mm2pixX(const std::string& name, double value)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getpix2mmX();
	}
	return GlobalConstants::double_min;
}
double CameraFactory::mm2pixY(const std::string& name, double value)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getpix2mmX();
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
		return camera_map.at(full_name).getpix2mmX();
	}
	return GlobalConstants::double_min;
}
double CameraFactory::setpix2mmY(const std::string& name, double value)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getpix2mmX();
	}
	return GlobalConstants::double_min;
}
//    __   ___      ___  ___  __      __   __      __                      __      __   __      __   __   __  
//   /  ` |__  |\ |  |  |__  |__)    /  \ |__)    /  \ |\ | __  /\  \_/ | /__`    /  ` /  \ __ /  \ |__) |  \	
//   \__, |___ | \|  |  |___ |  \    \__/ |  \    \__/ | \|    /~~\ / \ | .__/    \__, \__/    \__/ |  \ |__/ 
//  
long CameraFactory::getCentreXPixel(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getCentreXPixel();
	}
	return GlobalConstants::long_min;
}
long CameraFactory::getCentreYPixel(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getCentreYPixel();
	}
	return GlobalConstants::long_min;
}
bool CameraFactory::setCentreXPixel(const std::string& name, long value)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).setCentreXPixel(value);
	}
	return false;
}
bool CameraFactory::setCentreYPixel(const std::string& name, long value)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).setCentreYPixel(value);
	}
	return false;
}
bool CameraFactory::setCentrePixels(const std::string& name, long x, long y)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).setCentrePixels(x,y);
	}
	return false;
}
bool CameraFactory::setMechanicalCentre(const std::string& name)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).setMechanicalCentre();
	}
	return false;
}
bool CameraFactory::setOperatingCenter(const std::string& name)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).setOperatingCenter();
	}
	return GlobalConstants::long_min;
}
long CameraFactory::getOperatingCentreXPixel(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getOperatingCentreXPixel();
	}
	return GlobalConstants::long_min;
}
long CameraFactory::getOperatingCentreYPixel(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getOperatingCentreYPixel();
	}
	return GlobalConstants::long_min;
}
long CameraFactory::getMechanicalCentreXPixel(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getMechanicalCentreXPixel();
	}
	return GlobalConstants::long_min;
}
long CameraFactory::getMechanicalCentreYPixel(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getMechanicalCentreYPixel();
	}
	return GlobalConstants::long_min;
}
//  
//  			  __   ___ __    __  ___
//  |  |\/|  /\  / _` |__ /__` |  / |__
//  |  |  | /~~\ \__> |___.__/ | /_ |___
// 
long CameraFactory::getPixelWidth(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getPixelWidth();
	}
	return GlobalConstants::long_min;
}
long CameraFactory::getPixelHeight(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getPixelHeight();
	}
	return GlobalConstants::long_min;
}
size_t CameraFactory::getArrayDataPixelCountX(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getArrayDataPixelCountX();
	}
	return GlobalConstants::size_zero;
}
size_t CameraFactory::getArrayDataPixelCountY(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getArrayDataPixelCountY();
	}
	return GlobalConstants::size_zero;
}
size_t CameraFactory::getBinaryDataPixelCountX(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getBinaryDataPixelCountX();
	}
	return GlobalConstants::size_zero;
}
size_t CameraFactory::getBinaryDataPixelCountY(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getBinaryDataPixelCountY();
	}
	return GlobalConstants::size_zero;
}
//	 __   __                __        __  
//	/  ` |__) |  |    |    /  \  /\  |  \ 
//	\__, |    \__/    |___ \__/ /~~\ |__/ 
//	
long CameraFactory::getCPUTotal(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getCPUTotal();
	}
	return GlobalConstants::long_min;
}
long CameraFactory::getCPUCropSubMask(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getCPUCropSubMask();
	}
	return GlobalConstants::long_min;
}
long CameraFactory::getCPUNPoint(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getCPUNPoint();
	}
	return GlobalConstants::long_min;
}
long CameraFactory::getCPUDot(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getCPUDot();
	}
	return GlobalConstants::long_min;
}
//	              __   ___     __        ___  ___  ___  __  
//	|  |\/|  /\  / _` |__     |__) |  | |__  |__  |__  |__) 
//	|  |  | /~~\ \__> |___    |__) \__/ |    |    |___ |  \ 
//
char CameraFactory::getImageBufferTrigger(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getImageBufferTrigger();
	}
	return GlobalConstants::char_min;
}
std::string CameraFactory::getImageBufferFilePath(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getImageBufferFilePath();
	}
	return std::string(name + " DOES NOT EXIST");
}
std::string CameraFactory::getImageBufferFileName(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getImageBufferFileName();
	}
	return std::string(name + " DOES NOT EXIST");
}
long CameraFactory::getImageBufferFileNumber(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getImageBufferFileNumber();
	}
	return GlobalConstants::long_min;
}
std::string CameraFactory::getLastImageBufferDirectoryandFileName(const std::string& name) const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getLastImageBufferDirectoryandFileName();
	}
	return std::string(name + "DOES NOT EXIST");
}
std::string CameraFactory::getLastImageBufferDirectory(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getLastImageBufferDirectory();
	}
	return std::string(name + "DOES NOT EXIST");
}
std::string CameraFactory::getLastImageBufferFileName(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getLastImageBufferFileName();
	}
	return std::string(name + "DOES NOT EXIST");
}
//	              __   ___     __        __  ___       __   ___               __      __             _ 
//	|  |\/|  /\  / _` |__     /  `  /\  |__)  |  |  | |__) |__      /\  |\ | |  \    /__`  /\  \  / |__  
//	|  |  | /~~\ \__> |___    \__, /~~\ |     |  \__/ |  \ |___    /~~\ | \| |__/    .__/ /~~\  \/  |___ 
//	
bool CameraFactory::didLastCaptureAndSaveSucceed(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).didLastCaptureAndSaveSucceed();
	}
	return false;
}
bool CameraFactory::setNumberOfShotsToCapture(const std::string& name, size_t num)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).setNumberOfShotsToCapture(num);
	}
	return false;
}
size_t CameraFactory::getNumberOfShotsToCapture(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getNumberOfShotsToCapture();
	}
	return GlobalConstants::zero_sizet;
}
bool CameraFactory::captureAndSave(const std::string& name)
{
	return captureAndSave(name, GlobalConstants::zero_sizet);
}
bool CameraFactory::captureAndSave(const std::string& name, size_t num_images)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).captureAndSave(num_images);
	}
	return false;
}
STATE CameraFactory::getCaptureState(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getCaptureState();
	}
	return STATE::ERR;
}
bool CameraFactory::isCapturing(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).isCapturing();
	}
	return false;
}
bool CameraFactory::isNotCapturing(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).isNotCapturing();
	}
	return false;
}
bool CameraFactory::isWriting(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).isWriting();
	}
	return false;
}
bool CameraFactory::isNotWriting(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).isNotWriting();
	}
	return false;
}
bool CameraFactory::isCapturingOrSaving(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).isCapturingOrSaving();
	}
	return false;
}




bool CameraFactory::isBusy(const std::string& name)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).isBusy();
	}
	return false;
}
bool CameraFactory::isNotBusy(const std::string& name)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).isNotBusy();
	}
	return false;
}





bool CameraFactory::setX(const std::string& name, double value)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getpix2mmX();
	}
	return false;
}
bool CameraFactory::setY(const std::string& name, double value)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getpix2mmX();
	}
	return false;
}
bool CameraFactory::setSigX(const std::string& name, double value)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getpix2mmX();
	}
	return false;
}
bool CameraFactory::setSigY(const std::string& name, double value)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getpix2mmX();
	}
	return false;
}
bool CameraFactory::setSigXY(const std::string& name, double value)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getpix2mmX();
	}
	return false;
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

bool CameraFactory::setBlackLevel(const std::string& name, long value)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).setBlackLevel(value);
	}
	return GlobalConstants::long_min;
}

bool CameraFactory::areAllRunningStatsFull(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).areAllRunningStatsFull();
	}
	return false;
}

long CameraFactory::getBlackLevel(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getBlackLevel();
	}
	return GlobalConstants::long_min;
}

bool CameraFactory::setGain(const std::string& name, long value)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).setGain(value);
	}
	return GlobalConstants::long_min;
}

long CameraFactory::getGain(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getGain();
	}
	return GlobalConstants::long_min;
}





double CameraFactory::getAcquireTime(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getAcquireTime();
	}
	return GlobalConstants::double_min;
}
double CameraFactory::getAcquirePeriod(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getAcquirePeriod();
	}
	return GlobalConstants::double_min;
}
double CameraFactory::getArrayRate(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getArrayRate();
	}
	return GlobalConstants::double_min;
}
double CameraFactory::getTemperature(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getTemperature();
	}
	return GlobalConstants::double_min;
}
bool CameraFactory::setROIMinX(const std::string& name, long val)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).setROIMinX(val);
	}
	return false;
}
bool CameraFactory::setROIMinY(const std::string& name, long val)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).setROIMinY(val);
	}
	return false;
}
bool CameraFactory::setROISizeX(const std::string& name, long val)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).setROISizeX(val);
	}
	return false;
}
bool CameraFactory::setROISizeY(const std::string& name, long val)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).setROISizeY(val);
	}
	return false;
}
bool CameraFactory::setROI(const std::string& name, long x, long  y, long x_size, long y_size)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).setROI(x, y, x_size, y_size);
	}
	return false;
}
bool CameraFactory::setROI(const std::string& name, std::map<std::string, long> settings)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).setROI(settings);
	}
	return false;
}
bool CameraFactory::setROI_Py(const std::string& name, boost::python::dict settings)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).setROI_Py(settings);
	}
	return false;
}
long CameraFactory::getROIMinX(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getROIMinX();
	}
	return GlobalConstants::long_min;
}
long CameraFactory::getROIMinY(const std::string& name )const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getROIMinY();
	}
	return GlobalConstants::long_min;
}
long CameraFactory::getROISizeX(const std::string& name )const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getROISizeX();
	}
	return GlobalConstants::long_min;
}
long CameraFactory::getROISizeY(const std::string& name )const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getROISizeY();
	}
	return GlobalConstants::long_min;
}
std::map<std::string, double> CameraFactory::getAnalysisResultsPixels(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getAnalysisResultsPixels();
	}
	std::map<std::string, double> r;
	r["ERROR"] = GlobalConstants::double_min; // MAGIC STRING
	return r;
}
boost::python::dict CameraFactory::getAnalysisResultsPixels_Py(const std::string& name)const
{
	return to_py_dict<std::string, double>(getAnalysisResultsPixels(name));
}

std::map<std::string, long> CameraFactory::getROI(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getROI();
	}
	std::map<std::string, long> r;
	r["x_pos"] = GlobalConstants::long_min; // MAGIC STRING
	r["y_pos"] = GlobalConstants::long_min; // MAGIC STRING
	r["x_size"] = GlobalConstants::long_min;// MAGIC STRING
	r["y_size"] = GlobalConstants::long_min;// MAGIC STRING
	return r;
}
boost::python::dict CameraFactory::getROI_Py(const std::string& name )const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getROI_Py();
	}
	std::map<std::string, long> r;
	r["x_pos"] = GlobalConstants::long_min; // MAGIC STRING
	r["y_pos"] = GlobalConstants::long_min; // MAGIC STRING
	r["x_size"] = GlobalConstants::long_min;// MAGIC STRING
	r["y_size"] = GlobalConstants::long_min;// MAGIC STRING
	return to_py_dict<std::string, long>(r);
}
bool CameraFactory::setUseFloor(const std::string& name )
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).setUseFloor();
	}
	return false;
}
bool CameraFactory::setDoNotUseFloor(const std::string& name )
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).setDoNotUseFloor();
	}
	return false;
}
bool CameraFactory::toggleUseFloor(const std::string& name)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).toggleUseFloor();
	}
	return false;
}
bool CameraFactory::setFloorLevel(const std::string& name, long value)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).setFloorLevel(value);
	}
	return false;
}
STATE CameraFactory::getUseFloorState(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getUseFloorState();
	}
	return STATE::ERR;
}
bool CameraFactory::isUsingFloor(const std::string& name )const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).isUsingFloor();
	}
	return false;
}
bool CameraFactory::isNotUsingFloor(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).isNotUsingFloor();
	}
	return false;
}
long CameraFactory::getFloorLevel(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getFloorLevel();
	}
	return GlobalConstants::long_min;
}
long CameraFactory::getFlooredPtsCount(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getFlooredPtsCount();
	}
	return GlobalConstants::long_min;
}
double CameraFactory::getFlooredPtsPercent(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getFlooredPtsPercent();
	}
	return GlobalConstants::double_min;
}



bool CameraFactory::setUseNPointScaling(const std::string& name)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).setUseNPointScaling();
	}
	return false;
}
bool CameraFactory::setDoNotUseNPointScaling(const std::string& name)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).setDoNotUseNPointScaling();
	}
	return false;
}
bool CameraFactory::toggleUseNpointScaling(const std::string& name)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).toggleUseNpointScaling();
	}
	return false;
}
STATE CameraFactory::getNPointScalingState(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getNPointScalingState();
	}
	return STATE::UNKNOWN_NAME;
}
bool CameraFactory::isUsingNPointScaling(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).isUsingNPointScaling();
	}
	return false;
}
bool CameraFactory::isNotUsingNPointScaling(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).isNotUsingNPointScaling();
	}
	return false;
}
bool CameraFactory::setNpointScalingStepSize(const std::string& name, long v)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).setNpointScalingStepSize(v);
	}
	return false;
}
long CameraFactory::getNpointScalingStepSize(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getNpointScalingStepSize();
	}
	return GlobalConstants::long_min;
}


//                          __     __      __        __        __   __   __             __     
//  /\  |\ | |     /\  \ / /__` | /__`    |__)  /\  /  ` |__/ / _` |__) /  \ |  | |\ | |  \    
// /~~\ | \| |___ /~~\  |  .__/ | .__/    |__) /~~\ \__, |  \ \__> |  \ \__/ \__/ | \| |__/    
// 
STATE CameraFactory::getUsingBackgroundImageState(const std::string& name)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getUsingBackgroundImageState();
	}
	return STATE::UNKNOWN_NAME;
}
bool CameraFactory::setNewBackgroundImage(const std::string& name)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).setNewBackgroundImage();
	}
	return false;
}
bool CameraFactory::setUseBackgroundImage(const std::string& name)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).setUseBackgroundImage();
	}
	return false;
}
bool CameraFactory::setDoNotUseBackgroundImage(const std::string& name)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).setDoNotUseBackgroundImage();
	}
	return false;
}
bool CameraFactory::toggleUseBackgroundImage(const std::string& name)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).toggleUseBackgroundImage();
	}
	return false;
}
bool CameraFactory::isUsingBackgroundImage(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).isUsingBackgroundImage();
	}
	return false;
}
bool CameraFactory::isNotUsingBackgroundImage(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).isNotUsingBackgroundImage();
	}
	return false;
}







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
bool CameraFactory::setSumIntensity(const std::string& name,double value)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).setSumIntensity(value);
	}
	return false;
}
bool CameraFactory::setAvgIntensity(const std::string& name,double value)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).setAvgIntensity(value);
	}
	return false;
}
std::string CameraFactory::getLastDirectoryandFileName(const std::string& name) const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getLastDirectoryandFileName();
	}
	return std::string(name + "DOES NOT EXIST");
}
std::string CameraFactory::getLastDirectory(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getLastDirectory();
	}
	return std::string(name + "DOES NOT EXIST");
}
std::string CameraFactory::getLastFileName(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getLastFileName();
	}
	return std::string(name + "DOES NOT EXIST");
}

bool CameraFactory::setMaskXCenter(const std::string& name, long val)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).setMaskXCenter(val);
	}
	return false;
}
bool CameraFactory::setMaskYCenter(const std::string& name, long val)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).setMaskYCenter(val);
	}
	return false;
}
bool CameraFactory::setMaskXRadius(const std::string& name, long val)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).setMaskXRadius(val);
	}
	return false;
}
bool CameraFactory::setMaskYRadius(const std::string& name, long val)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).setMaskYRadius(val);
	}
	return GlobalConstants::double_min;
}
bool CameraFactory::setMask(const std::string& name, long x_pos, long  y_pos, long x_size, long y_size)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).setMask(x_pos, y_pos, x_size, y_size);
	}
	return GlobalConstants::double_min;
}
bool CameraFactory::setMask(const std::string& name, std::map<std::string, long> settings)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).setMask(settings);
	}
	return GlobalConstants::double_min;
}
bool CameraFactory::setMask_Py(const std::string& name, boost::python::dict settings)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).setMask_Py(settings);
	}
	return GlobalConstants::double_min;
}
long CameraFactory::getMaskXCenter(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getMaskXCenter();
	}
	return GlobalConstants::long_min;
}
long CameraFactory::getMaskYCenter(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getMaskYCenter();
	}
	return GlobalConstants::long_min;
}
long CameraFactory::getMaskXRadius(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getMaskXRadius();
	}
	return GlobalConstants::long_min;
}
long CameraFactory::getMaskYRadius(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getMaskYRadius();
	}
	return GlobalConstants::long_min;
}
std::map<std::string, long> CameraFactory::getMask(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getMask();
	}
	std::map<std::string, long> r;
	r["mask_x"] = GlobalConstants::long_min; // MAGIC STRING
	r["mask_y"] = GlobalConstants::long_min; // MAGIC STRING
	r["mask_rad_x"] = GlobalConstants::long_min;// MAGIC STRING
	r["mask_rad_x"] = GlobalConstants::long_min;// MAGIC STRING
	return r;
}
boost::python::dict CameraFactory::getMask_Py(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getMask_Py();
	}
	return to_py_dict<std::string, long>(CameraFactory::getMask(name));
}
bool CameraFactory::setMaskAndROIxMax(const std::string& name, long val)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).setMaskAndROIxMax(val);
	}
	return GlobalConstants::double_min;
}
bool CameraFactory::setMaskAndROIyMax(const std::string& name, long val)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).setMaskAndROIyMax(val);
	}
	return GlobalConstants::double_min;
}
bool CameraFactory::setMaskAndROIxSize(const std::string& name, long val)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getpix2mmX();
	}
	return GlobalConstants::double_min;
}
bool CameraFactory::setMaskAndROIySize(const std::string& name, long val)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).setMaskAndROIySize(val);
	}
	return GlobalConstants::double_min;
}
long CameraFactory::getMaskAndROIxMax(const std::string& name )const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getMaskAndROIxMax();
	}
	return GlobalConstants::long_min;
}
long CameraFactory::getMaskAndROIyMax(const std::string& name )const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getMaskAndROIyMax();
	}
	return GlobalConstants::long_min;
}
long CameraFactory::getMaskAndROIxSize(const std::string& name )const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getMaskAndROIxSize();
	}
	return GlobalConstants::long_min;
}
long CameraFactory::getMaskAndROIySize(const std::string& name )const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getMaskAndROIySize();
	}
	return GlobalConstants::long_min;
}
bool CameraFactory::setMaskandROI(const std::string& name, long x_pos, long  y_pos, long x_size, long y_size)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).setMaskandROI(x_pos, y_pos, x_size, y_size);
	}
	return false;
}
bool CameraFactory::setMaskandROI(const std::string& name, std::map<std::string, long> settings)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).setMaskandROI(settings);
	}
	return false;
}
bool CameraFactory::setMaskandROI_Py(const std::string& name, boost::python::dict settings)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).setMaskandROI_Py(settings);
	}
	return false;
}
std::map<std::string, long> CameraFactory::getMaskandROI(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getMaskandROI();
	}
	std::map<std::string, long> r;
	r["x_pos"] = GlobalConstants::double_min; // MAGIC STRING
	r["y_pos"] = GlobalConstants::double_min; // MAGIC STRING
	r["x_size"] = GlobalConstants::double_min;// MAGIC STRING
	r["y_size"] = GlobalConstants::double_min;// MAGIC STRING
	return r;
}
boost::python::dict CameraFactory::getMaskandROI_Py(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getMaskandROI_Py();
	}
	return to_py_dict<std::string, long>(getMaskandROI(name));
}


double CameraFactory::getActiveCameraLimit(const std::string& name) const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getActiveCameraLimit();
	}
	return GlobalConstants::double_min;
}
double CameraFactory::getActiveCameraCount(const std::string& name) const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getActiveCameraCount();
	}
	return GlobalConstants::double_min;
}
bool CameraFactory::canStartCamera(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getMaskandROI_Py();
	}
	return to_py_dict<std::string, long>(getMaskandROI(name));
}
bool CameraFactory::startAcquiring(const std::string& name)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).startAcquiring();
	}
	return false;
}
bool CameraFactory::stopAcquiring(const std::string& name)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).stopAcquiring();
	}
	return false;
}
bool CameraFactory::stopAcquiringAndWait(const std::string& name, size_t timeout = 3000)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).stopAcquiringAndWait(timeout);
	}
	return false;
}

bool CameraFactory::isAcquiring(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).isAcquiring();
	}
	return false;
}
bool CameraFactory::isNotAcquiring(const std::string& name) const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).isNotAcquiring();
	}
	return false;
}
STATE CameraFactory::getAcquireState(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getAcquireState();
	}
	return STATE::ERR;
}
bool CameraFactory::startAnalysing(const std::string& name)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).startAnalysing();
	}
	return false;
}
bool CameraFactory::stopAnalysing(const std::string& name)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).stopAnalysing();
	}
	return false;
}
bool CameraFactory::isAnalysing(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).isAnalysing();
	}
	return false;
}
bool CameraFactory::isNotAnalysing(const std::string& name) const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).isNotAnalysing();
	}
	return false;
}
STATE CameraFactory::getAnalysisState(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getAnalysisState();
	}
	return STATE::ERR;
}




bool CameraFactory::hasLED(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).hasLED();
	}
	return false;
}
bool CameraFactory::setLEDOn(const std::string& name)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).setLEDOn();
	}
	return false;
}
bool CameraFactory::setLEDOff(const std::string& name)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).setLEDOff();
	}
	return false;
}
bool CameraFactory::isLEDOn(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).isLEDOn();
	}
	return false;
}
bool CameraFactory::isLEDOff(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).isLEDOff();
	}
	return false;
}
STATE CameraFactory::getLEDState(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getLEDState();
	}
	return STATE::ERR;
}
bool CameraFactory::updateImageData(const std::string&name)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).updateImageData();
	}
	return false;
}
bool CameraFactory::updateImageDataWithTimeStamp(const std::string& name)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).updateImageDataWithTimeStamp();
	}
	return false;
}
bool CameraFactory::updateROIData(const std::string& name)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).updateROIData();
	}
	return false;
}
bool CameraFactory::updateROIDataWithTimeStamp(const std::string& name)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).updateROIDataWithTimeStamp();
	}
	return false;
}
std::vector<long> CameraFactory::getImageData(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getImageData();
	}
	return std::vector<long>{GlobalConstants::long_min};
}
boost::python::list CameraFactory::getImageData_Py(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getImageData_Py();
	}
	return to_py_list<long>(std::vector<long>{GlobalConstants::long_min});
}
std::vector<long> CameraFactory::getROIData(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getROIData();
	}
	return std::vector<long>{GlobalConstants::long_min};
}
boost::python::list CameraFactory::getROIData_Py(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getROIData_Py();
	}
	return to_py_list<long>(std::vector<long>{GlobalConstants::long_min});
}
std::vector<long>& CameraFactory::getImageDataConstRef(const std::string& name)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getImageDataConstRef();
	}
	return array_data_dummy;
}
std::vector<long>& CameraFactory::getROIDataConstRef(const std::string& name)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getROIDataConstRef();
	}
	return array_data_dummy;
}
size_t CameraFactory::getBufferSize(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getBufferSize();
	}
	return GlobalConstants::zero_sizet;
}
void CameraFactory::setBufferSize(const std::string& name, size_t v)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).setAllRunningStatBufferSizes(v);
	}
}
void CameraFactory::clearBuffers(const std::string& name)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).clearAllRunningStatBuffers();
	}
}
void CameraFactory::setRunningStatSize(const std::string& name, size_t size)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		camera_map.at(full_name).setAllRunningStatSizes(size);
	}
}
void CameraFactory::clearRunningStats(const std::string& name)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		camera_map.at(full_name).clearAllRunningStats();
	}
}



boost::python::dict CameraFactory::getAllRunningStats(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getAllRunningStats();
	}
	return boost::python::dict();
}
size_t CameraFactory::getRunningStatNumDataValues(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getRunningStatNumDataValues();
	}
	return GlobalConstants::zero_sizet;
}



bool CameraFactory::enableOverlayMask(const std::string& name)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).enableOverlayMask();
	}
	return false;
}
bool CameraFactory::enableOverlayCross(const std::string& name)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).enableOverlayCross();
	}
	return false;
}
bool CameraFactory::enableOverlayResult(const std::string& name)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).enableOverlayResult();
	}
	return false;
}
bool CameraFactory::disableOverlayCross(const std::string& name)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).disableOverlayCross();
	}
	return false;
}
bool CameraFactory::disableOverlayMask(const std::string& name)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).disableOverlayMask();
	}
	return false;
}
bool CameraFactory::disableOverlayResult(const std::string& name)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).disableOverlayResult();
	}
	return false;
}
bool CameraFactory::disableAllOverlay(const std::string& name)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).disableAllOverlay();
	}
	return false;
}
bool CameraFactory::disableAllOverlayForAllCameras()
{
	for (auto& cam : camera_map)
	{
		cam.second.disableAllOverlay();
	}
	return false;
}





STATE CameraFactory::getOverlayMaskState(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getOverlayMaskState();
	}
	return STATE::UNKNOWN_STATE;
}
STATE CameraFactory::getOverlayCrossState(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getOverlayCrossState();
	}
	return STATE::UNKNOWN_STATE;
}
STATE CameraFactory::getOverlayResultState(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getOverlayResultState();
	}
	return STATE::UNKNOWN_STATE;
}
std::map<std::string, STATE> CameraFactory::getAllOverlayStates()const
{
	std::map<std::string, STATE> r;
	return r;
}
boost::python::dict CameraFactory::getAllOverlayStates_Py()const
{
	boost::python::dict r;
	return r;
}












/*

double CameraFactory::pix2mmX(const std::string& name, double value)const
{
	std::string full_name = getFullName(name);
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
*/
//double CameraFactory::getX(const std::string& name)const
//{
//	std::string full_name = getFullName(name);
//	if (GlobalFunctions::entryExists(camera_map, full_name))
//	{
//		return camera_map.at(full_name).getX();
//	}
//	return GlobalConstants::double_min;
//}
//double CameraFactory::getY(const std::string& name)const
//{
//	std::string full_name = getFullName(name);
//	if (GlobalFunctions::entryExists(camera_map, full_name))
//	{
//		return camera_map.at(full_name).getY();
//	}
//	return GlobalConstants::double_min;
//}
//double CameraFactory::getSigX(const std::string& name)const
//{
//	std::string full_name = getFullName(name);
//	if (GlobalFunctions::entryExists(camera_map, full_name))
//	{
//		return camera_map.at(full_name).getSigX();
//	}
//	return GlobalConstants::double_min;
//}
//double CameraFactory::getSigY(const std::string& name)const
//{
//	std::string full_name = getFullName(name);
//	if (GlobalFunctions::entryExists(camera_map, full_name))
//	{
//		return camera_map.at(full_name).getSigY();
//	}
//	return GlobalConstants::double_min;
//}
//double CameraFactory::getSigXY(const std::string& name)const
//{
//	std::string full_name = getFullName(name);
//	if (GlobalFunctions::entryExists(camera_map, full_name))
//	{
//		return camera_map.at(full_name).getSigXY();
//	}
//	return GlobalConstants::double_min;
//}
//double CameraFactory::getXPix(const std::string& name)const
//{
//	std::string full_name = getFullName(name);
//	if (GlobalFunctions::entryExists(camera_map, full_name))
//	{
//		return camera_map.at(full_name).getXPix();
//	}
//	return GlobalConstants::double_min;
//}
//double CameraFactory::getYPix(const std::string& name)const
//{
//	std::string full_name = getFullName(name);
//	if (GlobalFunctions::entryExists(camera_map, full_name))
//	{
//		return camera_map.at(full_name).getYPix();
//	}
//	return GlobalConstants::double_min;
//}
//double CameraFactory::getSigXPix(const std::string& name)const
//{
//	std::string full_name = getFullName(name);
//	if (GlobalFunctions::entryExists(camera_map, full_name))
//	{
//		return camera_map.at(full_name).getSigXPix();
//	}
//	return GlobalConstants::double_min;
//}
//double CameraFactory::getSigYPix(const std::string& name)const
//{
//	std::string full_name = getFullName(name);
//	if (GlobalFunctions::entryExists(camera_map, full_name))
//	{
//		return camera_map.at(full_name).getSigYPix();
//	}
//	return GlobalConstants::double_min;
//}
//double CameraFactory::getSigXYPix(const std::string& name)const
//{
//	std::string full_name = getFullName(name);
//	if (GlobalFunctions::entryExists(camera_map, full_name))
//	{
//		return camera_map.at(full_name).getSigXYPix();
//	}
//	return GlobalConstants::double_min;
//}
//
//bool CameraFactory::setX(const std::string& name, double value)
//{
//	std::string full_name = getFullName(name);
//	if (GlobalFunctions::entryExists(camera_map, full_name))
//	{
//		return camera_map.at(full_name).setX(value);
//	}
//	return false;
//}
//bool CameraFactory::setY(const std::string& name, double value)
//{
//	std::string full_name = getFullName(name);
//	if (GlobalFunctions::entryExists(camera_map, full_name))
//	{
//		return camera_map.at(full_name).setY(value);
//	}
//	return false;
//}
//bool CameraFactory::setSigX(const std::string& name, double value)
//{
//	std::string full_name = getFullName(name);
//	if (GlobalFunctions::entryExists(camera_map, full_name))
//	{
//		return camera_map.at(full_name).setSigX(value);
//	}
//	return false;
//}
//bool CameraFactory::setSigY(const std::string& name, double value)
//{
//	std::string full_name = getFullName(name);
//	if (GlobalFunctions::entryExists(camera_map, full_name))
//	{
//		return camera_map.at(full_name).setSigY(value);
//	}
//	return false;
//}
//bool CameraFactory::setSigXY(const std::string& name, double value)
//{
//	std::string full_name = getFullName(name);
//	if (GlobalFunctions::entryExists(camera_map, full_name))
//	{
//		return camera_map.at(full_name).setSigXY(value);
//	}
//	return false;
//}
//
//
//long CameraFactory::getStepSize(const std::string& name)const
//{
//	std::string full_name = getFullName(name);
//	if (GlobalFunctions::entryExists(camera_map, full_name))
//	{
//		return camera_map.at(full_name).getStepSize();
//	}
//	return false;
//}
//bool CameraFactory::setStepSize(const std::string& name, long val)
//{
//	std::string full_name = getFullName(name);
//	if (GlobalFunctions::entryExists(camera_map, full_name))
//	{
//		return camera_map.at(full_name).setStepSize(val);
//	}
//	return false;
//}
//
//boost::python::dict CameraFactory::getRunningStats(const std::string& name)const
//{
//	std::string full_name = getFullName(name);
//	if (GlobalFunctions::entryExists(camera_map, full_name))
//	{
//		return camera_map.at(full_name).getRunningStats();
//	}
//	boost::python::dict r;
//	return r;
//}
//
//boost::python::dict CameraFactory::getAllRunningStats()const
//{
//	boost::python::dict r;
//	for (auto&& cam : camera_map)
//	{
//		r[cam.first] = cam.second.getRunningStats();
//	}
//	return r;
//}
//
//
//
//
////bool CameraFactory::setXPix(const std::string& name, double value)
////{
////	std::string full_name = getFullName(name);
////	if (GlobalFunctions::entryExists(camera_map, full_name))
////	{
////		return camera_map.at(full_name).setXPix(value);
////	}
////	return false;
////}
////bool CameraFactory::setYPix(const std::string& name, double value)
////{
////	std::string full_name = getFullName(name);
////	if (GlobalFunctions::entryExists(camera_map, full_name))
////	{
////		return camera_map.at(full_name).setYPix(value);
////	}
////	return false;
////}
////bool CameraFactory::setSigXPix(const std::string& name, double value)
////{
////	std::string full_name = getFullName(name);
////	if (GlobalFunctions::entryExists(camera_map, full_name))
////	{
////		return camera_map.at(full_name).setSigXPix(value);
////	}
////	return false;
////}
////bool CameraFactory::setSigYPix(const std::string& name, double value)
////{
////	std::string full_name = getFullName(name);
////	if (GlobalFunctions::entryExists(camera_map, full_name))
////	{
////		return camera_map.at(full_name).setSigYPix(value);
////	}
////	return false;
////}
////bool CameraFactory::setSigXYPix(const std::string& name, double value)
////{
////	std::string full_name = getFullName(name);
////	if (GlobalFunctions::entryExists(camera_map, full_name))
////	{
////		return camera_map.at(full_name).setSigXYPix(value);
////	}
////	return false;
////}
//
//double CameraFactory::getSumIntensity(const std::string& name)const
//{
//	std::string full_name = getFullName(name);
//	if (GlobalFunctions::entryExists(camera_map, full_name))
//	{
//		return camera_map.at(full_name).getSumIntensity();
//	}
//	return GlobalConstants::double_min;
//}
//double CameraFactory::getAvgIntensity(const std::string& name)const
//{
//	std::string full_name = getFullName(name);
//	if (GlobalFunctions::entryExists(camera_map, full_name))
//	{
//		return camera_map.at(full_name).getAvgIntensity();
//	}
//	return GlobalConstants::double_min;
//}
//bool CameraFactory::setSumIntensity(const std::string& name, double value)
//{
//	std::string full_name = getFullName(name);
//	if (GlobalFunctions::entryExists(camera_map, full_name))
//	{
//		return camera_map.at(full_name).setSumIntensity(value);
//	}
//	return false;
//}
//bool CameraFactory::setAvgIntensity(const std::string& name, double value)
//{
//	std::string full_name = getFullName(name);
//	if (GlobalFunctions::entryExists(camera_map, full_name))
//	{
//		return camera_map.at(full_name).setAvgIntensity(value);
//	}
//	return false;
//}
//
//bool CameraFactory::stopAcquiring(const std::string& cam_name)
//{
//	std::string full_name = getFullName(cam_name);
//	if (GlobalFunctions::entryExists(camera_map, full_name))
//	{
//		return camera_map.at(full_name).stopAcquiring();
//	}
//	return false;
//}
//bool CameraFactory::stopAllAcquiringExceptVC()
//{
//	bool r = true;
//	for (auto&& cam : camera_map)
//	{
//		if (GlobalFunctions::entryExists<std::string>(cam.second.getScreenNames(), "VIRTUAL_CATHODE")) // MAGIC STRING
//		{
//		}
//		else
//		{
//			bool t = cam.second.stopAcquiring();
//			if (!t)
//			{
//				r = false;
//			}
//		}
//	}
//	return r;
//}
//bool CameraFactory::stopAllAcquiring()
//{
//	bool r = true;
//	for (auto&& cam : camera_map)
//	{
//		bool t = cam.second.stopAcquiring();
//		if (!t)
//		{
//			r = false;
//		}
//	}
//	return r;
//}
//bool CameraFactory::startAcquiring(const std::string& cam_name, bool stop_all )
//{
//	bool r = false;
//	std::string full_name = getFullName(cam_name);
//	if (GlobalFunctions::entryExists(camera_map, full_name))
//	{
//		if (stop_all)
//		{
//			r = stopAllAcquiringExceptVC();
//			if (r)
//			{
//				r = camera_map.at(full_name).startAcquiring();
//			}
//		}
//		else
//		{
//			r = camera_map.at(full_name).startAcquiring();
//		}
//	}
//	return r;
//}
//STATE CameraFactory::getAcquireState(const std::string& name)const
//{
//	std::string full_name = getFullName(name);
//	if (GlobalFunctions::entryExists(camera_map, full_name))
//	{
//		return camera_map.at(full_name).getAcquireState();
//	}
//	return STATE::UNKNOWN_NAME;
//}
//std::map<std::string, STATE> CameraFactory::getAllAcquireStates()const
//{
//	std::map<std::string, STATE> r;
//	for(auto&& cam : camera_map)
//	{
//		r[cam.first] = cam.second.getAcquireState();
//	}
//	return r;
//}
//boost::python::dict CameraFactory::getAllAcquireStates_Py()const
//{
//	return to_py_dict<std::string, STATE>(getAllAcquireStates());
//}
//
//bool CameraFactory::isAcquiring(const std::string& name)const
//{
//	std::string full_name = getFullName(name);
//	if (GlobalFunctions::entryExists(camera_map, full_name))
//	{
//		return camera_map.at(full_name).isAcquiring();
//	}
//	return false;
//}
//bool CameraFactory::isNotAcquiring(const std::string& name) const
//{
//	std::string full_name = getFullName(name);
//	if (GlobalFunctions::entryExists(camera_map, full_name))
//	{
//		return camera_map.at(full_name).isNotAcquiring();
//	}
//	return true; //Hmmm what to do about no-existent existential quantification? relly need ternary state, meh meh meh 
//}
//
//
//long CameraFactory::getMaskXCenter(const std::string& name)const
//{
//	std::string full_name = getFullName(name);
//	if (GlobalFunctions::entryExists(camera_map, full_name))
//	{
//		return camera_map.at(full_name).getMaskXCenter();
//	}
//	return GlobalConstants::ushort_min;
//}
//long CameraFactory::getMaskYCenter(const std::string& name)const
//{
//	std::string full_name = getFullName(name);
//	if (GlobalFunctions::entryExists(camera_map, full_name))
//	{
//		return camera_map.at(full_name).getMaskYCenter();
//	}
//	return GlobalConstants::ushort_min;
//}
//long CameraFactory::getMaskXRadius(const std::string& name)const
//{
//	std::string full_name = getFullName(name);
//	if (GlobalFunctions::entryExists(camera_map, full_name))
//	{
//		return camera_map.at(full_name).getMaskXRadius();
//	}
//	return GlobalConstants::ushort_min;
//}
//long CameraFactory::getMaskYRadius(const std::string& name)const
//{
//	std::string full_name = getFullName(name);
//	if (GlobalFunctions::entryExists(camera_map, full_name))
//	{
//		return camera_map.at(full_name).getMaskYRadius();
//	}
//	return GlobalConstants::ushort_min;
//}
//
//long CameraFactory::setMaskXCenter(const std::string& name, long val)
//{
//	std::string full_name = getFullName(name);
//	if (GlobalFunctions::entryExists(camera_map, full_name))
//	{
//		return camera_map.at(full_name).setMaskXCenter(val);
//	}
//	return GlobalConstants::ushort_min;
//}
//long CameraFactory::setMaskYCenter(const std::string& name, long val)
//{
//	std::string full_name = getFullName(name);
//	if (GlobalFunctions::entryExists(camera_map, full_name))
//	{
//		return camera_map.at(full_name).setMaskYCenter(val);
//	}
//	return GlobalConstants::ushort_min;
//}
//long CameraFactory::setMaskXRadius(const std::string& name, long val)
//{
//	std::string full_name = getFullName(name);
//	if (GlobalFunctions::entryExists(camera_map, full_name))
//	{
//		return camera_map.at(full_name).setMaskXRadius(val);
//	}
//	return GlobalConstants::ushort_min;
//}
//long CameraFactory::setMaskYRadius(const std::string& name, long val)
//{
//	std::string full_name = getFullName(name);
//	if (GlobalFunctions::entryExists(camera_map, full_name))
//	{
//		return camera_map.at(full_name).setMaskYRadius(val);
//	}
//	return GlobalConstants::ushort_min;
//}
//bool CameraFactory::startAnalysing(const std::string& name)
//{
//	std::string full_name = getFullName(name);
//	if (GlobalFunctions::entryExists(camera_map, full_name))
//	{
//		return camera_map.at(full_name).startAnalysing();
//	}
//	return false;
//}
//bool CameraFactory::stopAnalysing(const std::string& name)
//{
//	std::string full_name = getFullName(name);
//	if (GlobalFunctions::entryExists(camera_map, full_name))
//	{
//		return camera_map.at(full_name).stopAnalysing();
//	}
//	return false;
//}
//bool CameraFactory::isAnalysing(const std::string& name)const
//{
//	std::string full_name = getFullName(name);
//	if (GlobalFunctions::entryExists(camera_map, full_name))
//	{
//		return camera_map.at(full_name).isAnalysing();
//	}
//	return false;
//}
//bool CameraFactory::isNotAnalysing(const std::string& name) const
//{
//	std::string full_name = getFullName(name);
//	if (GlobalFunctions::entryExists(camera_map, full_name))
//	{
//		return camera_map.at(full_name).isNotAnalysing();
//	}
//	return true;
//}
//STATE CameraFactory::getAnalysisState(const std::string& name)const
//{
//	std::string full_name = getFullName(name);
//	if (GlobalFunctions::entryExists(camera_map, full_name))
//	{
//		return camera_map.at(full_name).getAnalysisState();
//	}
//	return STATE::UNKNOWN_NAME;
//}
//std::map<std::string, STATE> CameraFactory::getAllAnalysisState()const
//{
//	std::map<std::string, STATE> r;
//	for (auto&& cam : camera_map)
//	{
//		r[cam.first] = cam.second.getAnalysisState();
//	}
//	return r;
//}
//boost::python::dict CameraFactory::getAllAnalysisState_Py()const
//{
//	return to_py_dict<std::string, STATE>(getAllAnalysisState());
//}
//
//
//bool CameraFactory::useNPoint(const std::string& name, bool v)
//{
//	std::string full_name = getFullName(name);
//	if (GlobalFunctions::entryExists(camera_map, full_name))
//	{
//		return camera_map.at(full_name).useNPoint(v);
//	}
//	return false;
//}
//bool CameraFactory::isUsingNPoint(const std::string& name)const
//{
//	std::string full_name = getFullName(name);
//	if (GlobalFunctions::entryExists(camera_map, full_name))
//	{
//		return camera_map.at(full_name).isUsingNPoint();
//	}
//	return false;
//}
//STATE CameraFactory::getNPointState(const std::string& name)const
//{
//	std::string full_name = getFullName(name);
//	if (GlobalFunctions::entryExists(camera_map, full_name))
//	{
//		return camera_map.at(full_name).getNPointState();
//	}
//	return STATE::UNKNOWN_NAME;
//}
//
//bool CameraFactory::useBackground(const std::string& name, bool v)
//{
//	std::string full_name = getFullName(name);
//	if (GlobalFunctions::entryExists(camera_map, full_name))
//	{
//		return camera_map.at(full_name).useBackground(v);
//	}
//	return false;
//}
//bool CameraFactory::isUsingBackground(const std::string& name)const
//{
//	std::string full_name = getFullName(name);
//	if (GlobalFunctions::entryExists(camera_map, full_name))
//	{
//		return camera_map.at(full_name).isUsingBackground();
//	}
//	return false;
//}
//STATE CameraFactory::getUsingBackgroundState(const std::string& name)const
//{
//	std::string full_name = getFullName(name);
//	if (GlobalFunctions::entryExists(camera_map, full_name))
//	{
//		return camera_map.at(full_name).getUsingBackgroundState();
//	}
//	return STATE::UNKNOWN_NAME;
//}
//
//
//
//
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


std::vector<std::string> CameraFactory::getNameAliases(const std::string& cam_name) const
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
boost::python::list CameraFactory::getNameAliases_Py(const std::string& cam_name) const
{
	return to_py_list<std::string>(getNameAliases(cam_name));
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
	std::stringstream ss;
	ss << "Cutting to areas in ";
	for(auto&& area : machineAreas)
	{
		ss << ENUM_TO_STRING(area);
		ss << ", ";
	}
	messenger.printDebugMessage(ss.str());
		
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

	//GlobalFunctions::pause_2000();

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

void CameraFactory::cutLHarwdareMapByNames(const std::vector<std::string>& names)
{
	std::stringstream ss;
	ss << "Cutting to name(s): ";
	for (auto&& name : names)
	{
		ss << name;
		ss << ", ";
	}
	messenger.printDebugMessage(ss.str());

	size_t start_size = camera_map.size();
	// loop over each magnet
	for (auto it = camera_map.begin(); it != camera_map.end() /* not hoisted */; /* no increment */)
	{
		// flag for if we should erase this entry, default to true 
		bool should_erase = true;
		// now we loop over every area in machineAreas and checl against isInMachineArea
		//messenger.printDebugMessage(it->first, " is in area = ", ENUM_TO_STRING(it->second.getMachineArea()));
		for (auto&& name : names)
		{
			std::string full_name = getFullName(name);



			if (it->first == full_name)
			{
				should_erase = false;
				break;
			}
			else{ /* .. */ }
		} // if should_erase is still true, erase object from  magnetMap
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
	messenger.printDebugMessage("cutLHarwdareMapByNames camera_map.size() went from ", start_size, " to ", end_size);
	GlobalFunctions::pause_2000();
}





//----------------------------------------------------------------------------------------------------------------
//			 __             __   __        __  ___    
//			/__` |\ |  /\  |__) /__` |__| /  \  |     
//			.__/ | \| /~~\ |    .__/ |  | \__/  |     
//


STATE CameraFactory::saveSnapshot(const std::string& comments)
{
	return saveSnapshot(SnapshotFileManager::defaultMachineSnapshotLocation, GlobalFunctions::getTimeAndDateString(), comments);
}
STATE CameraFactory::saveSnapshot(const std::string& filepath, const std::string& filename, const std::string& comments)
{
	messenger.printDebugMessage("saveSnapshot, call getSnapshot");
	hardwareSnapshotMap = getSnapshot();
	messenger.printDebugMessage("getSnapshot, writeSnapshotToYAML, ", "\n", filepath, "\n", filename, "\n", comments);
	bool written = SnapshotFileManager::writeSnapshotToYAML(filepath, filename, hardwareSnapshotMapToYAMLNode(hardwareSnapshotMap), mode, comments);
	if (written)
	{
		return STATE::SUCCESS;
	}
	return STATE::FAIL;
}
STATE CameraFactory::saveSnapshot_Pydict(const boost::python::dict& snapshot_dict, const std::string& comments)
{
	messenger.printDebugMessage("saveSnapshot_Pydict");
	return saveSnapshot_Pyfile(SnapshotFileManager::defaultMachineSnapshotLocation, GlobalFunctions::getTimeAndDateString(), snapshot_dict, comments);
}
STATE CameraFactory::saveSnapshot_Pyfile(const std::string& filepath, const std::string& filename, const boost::python::dict& snapshot_dict, const std::string& comments)
{
	messenger.printDebugMessage("saveSnapshot_Pyfile, convert dict to hssm ");
	auto hssm = pyDictToHardwareSnapshotMap(snapshot_dict);
	messenger.printDebugMessage("hssm size = ", hssm.size());
	auto yn = hardwareSnapshotMapToYAMLNode(hssm);
	messenger.printDebugMessage("yn size = ", yn.size());
	bool written = SnapshotFileManager::writeSnapshotToYAML(filepath, filename, yn, mode, comments);
	if (written)
	{
		return STATE::SUCCESS;
	}
	return STATE::FAIL;
}

STATE CameraFactory::loadSnapshot(const std::string filepath, const std::string& filename) // read into hardwareSnapshotMap
{
	messenger.printDebugMessage("loadSnapshot");
	YAML::Node file_node = SnapshotFileManager::readSnapshotFile(filepath, filename);
	messenger.printDebugMessage("loadSnapshot get a file_node with size() ", file_node.size());
	hardwareSnapshotMap = yamlNodeToHardwareSnapshotMap(file_node);
	return STATE::SUCCESS; // TODO do we need some error checking 
}
STATE CameraFactory::loadSnapshot_Py(const boost::python::dict& snapshot_dict) // put d into hardwareSnapshotMap
{
	hardwareSnapshotMap = pyDictToHardwareSnapshotMap(snapshot_dict);
	return STATE::SUCCESS;
}
std::map<std::string, HardwareSnapshot> CameraFactory::getSnapshot() // c++ version 
{
	std::map<std::string, HardwareSnapshot> snapshot_map;
	for (auto& item : camera_map)
	{
		messenger.printDebugMessage("getSnapshot, found ", item.first);
		snapshot_map[item.first] = item.second.getSnapshot();
	}
	messenger.printDebugMessage("returning snapshot_map with size =  ", snapshot_map.size());
	return snapshot_map;
}
boost::python::dict CameraFactory::getSnapshot_Py() // return current state as py dict 
{
	messenger.printDebugMessage("getSnapshot_Py, calling getSnapshot ");
	// FIRST GET A snaphot that is current (i.e. live!)! 
	std::map<std::string, HardwareSnapshot> current_snapshot = getSnapshot();
	boost::python::dict r; 
	messenger.printDebugMessage("loop over current_snapshot (.size() =", current_snapshot.size());
	for (auto&& item : current_snapshot)
	{
		// THIS IS NOT UPDATEING ANY DATA IN THE HARDWARE SNAPSHOT _ ITS CONVERTING WHAT ALREADY EXIST "
		messenger.printDebugMessage(item.first, " get snap data");
		r[item.first] = item.second.getSnapshot_Py();
	}
	boost::python::dict r2;
	r2[ENUM_TO_STRING(TYPE::MAGNET)] = r;
	return r2;
}
boost::python::dict CameraFactory::getSnapshotFromFile_Py(const std::string& location, const std::string& filename) // return file contents as py dict 
{
	messenger.printDebugMessage("getSnapshotFromFile_Py loadSnapshot");
	loadSnapshot(location, filename);
	boost::python::dict r;
	messenger.printDebugMessage("loop over hardwareSnapshotMap, with size = ", hardwareSnapshotMap.size());
	for (auto&& item : hardwareSnapshotMap)
	{
		messenger.printDebugMessage(item.first, " get snap data");
		// THIS IS NOT UPDTAEING ANY DATA IN TH EHARDWRAE SNAPSHOT _ ITS CONVERTING WAHT ALREADY EXIST "
		r[item.first] = item.second.getSnapshot_Py();
	}
	boost::python::dict r2;
	r2[ENUM_TO_STRING(TYPE::MAGNET)] = r;
	return r2;
}


STATE CameraFactory::applySnaphot(const boost::python::dict& snapshot_dict, TYPE type)
{
	hardwareSnapshotMap = pyDictToHardwareSnapshotMap(snapshot_dict);
	return applyhardwareSnapshotMap(hardwareSnapshotMap, type);
}
STATE CameraFactory::applySnaphot(const std::string& filepath, const std::string& filename, TYPE type)
{
	loadSnapshot(filepath, filename); // this sets the hardwareSnapshotMap member variables 
	return applyhardwareSnapshotMap(hardwareSnapshotMap, type);
}
STATE CameraFactory::applyLoadedSnaphost(TYPE type)
{
	return applyhardwareSnapshotMap(hardwareSnapshotMap, type);
}
STATE CameraFactory::applyhardwareSnapshotMap(const std::map<std::string, HardwareSnapshot>& hardwaresnapshot_map, TYPE type)
{
	for (auto&& snap : hardwaresnapshot_map)
	{
		std::string fullName = getFullName(snap.first);
		if (GlobalFunctions::entryExists(camera_map, fullName))
		{
			for (auto&& state_items : snap.second.state)
			{
				if (state_items.first == CameraRecords::CAM_BlackLevel_RBV)
				{
					camera_map.at(fullName).setBlackLevel(snap.second.get<long>(state_items.first));
				}
				else if (state_items.first == CameraRecords::CAM_Gain_RBV) 
				{
					camera_map.at(fullName).setGain(snap.second.get<long>(state_items.first));
				}
				else if (state_items.first == CameraRecords::ROI1_MinX_RBV) 
				{
					camera_map.at(fullName).setROIMinX(snap.second.get<long>(state_items.first));
				}
				else if (state_items.first == CameraRecords::ROI1_MinY_RBV) 
				{
					camera_map.at(fullName).setROIMinY(snap.second.get<long>(state_items.first));
				}
				else if (state_items.first == CameraRecords::ROI1_SizeX_RBV) 
				{
					camera_map.at(fullName).setROISizeX(snap.second.get<long>(state_items.first));
				}
				else if (state_items.first == CameraRecords::ROI1_SizeY_RBV) 
				{
					camera_map.at(fullName).setROISizeY(snap.second.get<long>(state_items.first));
				}
				//else if (state_items.first == CameraRecords::ROIandMask_SetX) 
				//{
				//	camera_map.at(fullName).setGain(snap.second.get<long>(state_items.first));
				//}
				//else if (state_items.first == CameraRecords::ROIandMask_SetY) 
				//{
				//	camera_map.at(fullName).setGain(snap.second.get<long>(state_items.first));
				//}
				//else if (state_items.first == CameraRecords::ROIandMask_SetXrad) 
				//{
				//	camera_map.at(fullName).setGain(snap.second.get<long>(state_items.first));
				//}
				//else if (state_items.first == CameraRecords::ROIandMask_SetYrad) 
				//{
				//	camera_map.at(fullName).setGain(snap.second.get<long>(state_items.first));
				//}
				else if (state_items.first == CameraRecords::ANA_MaskXCenter_RBV) 
				{
					camera_map.at(fullName).setMaskXCenter(snap.second.get<long>(state_items.first));
				}
				else if (state_items.first == CameraRecords::ANA_MaskYCenter_RBV) 
				{
					camera_map.at(fullName).setMaskYCenter(snap.second.get<long>(state_items.first));
				}
				else if (state_items.first == CameraRecords::ANA_MaskXRad_RBV)
				{
					camera_map.at(fullName).setMaskXRadius(snap.second.get<long>(state_items.first));
				}
				else if (state_items.first == CameraRecords::ANA_MaskYRad_RBV)
				{
					camera_map.at(fullName).setMaskYRadius(snap.second.get<long>(state_items.first));
				}
				else if (state_items.first == CameraRecords::ANA_UseFloor_RBV)
				{
					if (snap.second.get<STATE>(state_items.first) == STATE::USING_FLOOR)
					{
						camera_map.at(fullName).setUseFloor();
					}
					else
					{
						camera_map.at(fullName).setDoNotUseFloor();
					}
				}
				else if (state_items.first == CameraRecords::ANA_FloorLevel_RBV)
				{
					camera_map.at(fullName).setFloorLevel(snap.second.get<long>(state_items.first));
				}
				else if (state_items.first == CameraRecords::ANA_UseNPoint_RBV)
				{
					if (snap.second.get<STATE>(state_items.first) == STATE::USING_NPOINT)
					{
						camera_map.at(fullName).setUseNPointScaling();
					}
					else
					{
						camera_map.at(fullName).setDoNotUseNPointScaling();
					}
				}
				else if (state_items.first == CameraRecords::ANA_NPointStepSize_RBV)
				{
					camera_map.at(fullName).setNpointScalingStepSize(snap.second.get<long>(state_items.first));
				}
				// can't set background as backgroudn data can't be saved ... 
				//else if (state_items.first == CameraRecords::ANA_UseBkgrnd_RBV)
				//{
				//	camera_map.at(fullName).setMaskYRadius(snap.second.get<long>(state_items.first));
				//}
				else if (state_items.first == CameraRecords::ANA_OVERLAY_1_CROSS_RBV)
				{
					if (snap.second.get<STATE>(state_items.first) == STATE::ENABLED)
					{
						camera_map.at(fullName).enableOverlayCross();
					}
					else
					{
						camera_map.at(fullName).disableOverlayCross();
					}
				}
				else if (state_items.first == CameraRecords::ANA_OVERLAY_2_RESULT)
				{
					if (snap.second.get<STATE>(state_items.first) == STATE::ENABLED)
					{
						camera_map.at(fullName).enableOverlayResult();
					}
					else
					{
						camera_map.at(fullName).disableOverlayResult();
					}
				}
				else if (state_items.first == CameraRecords::ANA_OVERLAY_3_MASK_RBV)
				{
					if (snap.second.get<STATE>(state_items.first) == STATE::ENABLED)
					{
						camera_map.at(fullName).enableOverlayMask();
					}
					else
					{
						camera_map.at(fullName).disableOverlayMask();
					}
				}
//				else if (state_items.first == CameraRecords::HDF_Capture_RBV) {}
				else if (state_items.first == CameraRecords::LED_Sta)
				{
					if (snap.second.get<STATE>(state_items.first) == STATE::ON)
					{
						camera_map.at(fullName).setLEDOn();
					}
					else
					{
						camera_map.at(fullName).setLEDOff();
					}
				}
				//else if (state_items.first == CameraRecords::HDF_NumCapture_RBV) 
				else if (state_items.first == CameraRecords::ANA_CenterX_RBV) 
				{
					camera_map.at(fullName).setCentreXPixel(snap.second.get<long>(state_items.first));
				}
				else if (state_items.first == CameraRecords::ANA_CenterY_RBV)
				{
					camera_map.at(fullName).setCentreYPixel(snap.second.get<long>(state_items.first));
				}
				//else if (state_items.first == CameraRecords::ANA_XPix_RBV)
				//{
				//camera_map.at(fullName).setCentreYPixel(snap.second.get<long>(state_items.first));
				//}
				//else if (state_items.first == CameraRecords::ANA_YPix_RBV) {}
				//else if (state_items.first == CameraRecords::ANA_SigmaXPix_RBV) {}
				//else if (state_items.first == CameraRecords::ANA_SigmaYPix_RBV) {}
				//else if (state_items.first == CameraRecords::ANA_CovXYPix_RBV) {}
				else if (state_items.first == CameraRecords::ANA_X_RBV)
				{
					camera_map.at(fullName).setX(snap.second.get<double>(state_items.first));
				}
				else if (state_items.first == CameraRecords::ANA_Y_RBV)
				{
					camera_map.at(fullName).setY(snap.second.get<double>(state_items.first));
				}
				else if (state_items.first == CameraRecords::ANA_SigmaX_RBV)
				{
						camera_map.at(fullName).setSigX(snap.second.get<double>(state_items.first));
				}
				else if (state_items.first == CameraRecords::ANA_SigmaY_RBV)
				{
					camera_map.at(fullName).setSigY(snap.second.get<double>(state_items.first));
				}
				else if (state_items.first == CameraRecords::ANA_CovXY_RBV)
				{
					camera_map.at(fullName).setSigXY(snap.second.get<double>(state_items.first));
				}
				else if (state_items.first == CameraRecords::ANA_AvgIntensity_RBV)
				{
					camera_map.at(fullName).setAvgIntensity(snap.second.get<double>(state_items.first));
				}
				else if (state_items.first == CameraRecords::ANA_Intensity_RBV)
				{
					camera_map.at(fullName).setSumIntensity(snap.second.get<double>(state_items.first));
				}
				//else if (state_items.first == "busy")
				//{
				//}
				//else if (state_items.first == "max_shots_number") {}
				//else if (state_items.first == "last_capture_and_save_success") {}
				//else if (state_items.first == "aliases") {}
				//else if (state_items.first == "screen_names") {}
				//else if (state_items.first == "average_pixel_value_for_beam") {}
				//else if (state_items.first == "array_data_num_pix_x") {}
				//else if (state_items.first == "array_data_num_pix_y") {}
				//else if (state_items.first == "array_data_pixel_count") {}
				//else if (state_items.first == "binary_num_pix_x") {}
				//else if (state_items.first == "binary_num_pix_y") {}
				//else if (state_items.first == "binary_data_pixel_count") {}
				//else if (state_items.first == "roi_total_pixel_count") {}
				//else if (state_items.first == "operating_centre_x") {}
				//else if (state_items.first == "operating_centre_y") {}
				//else if (state_items.first == "mechanical_centre_x") {}
				//else if (state_items.first == "mechanical_centre_y") {}
				//else if (state_items.first == "pix2mmX_ratio") {}
				//else if (state_items.first == "pix2mmY_ratio") {}
				//else if (state_items.first == "cam_type") {}
				//else if (state_items.first == "roi_max_x") {}
				//else if (state_items.first == "roi_max_y") {}
				//else if (state_items.first == "x_pix_scale_factor") {}
				//else if (state_items.first == "y_pix_scale_factor") {}
				//else if (state_items.first == "x_mask_rad_max") {}
				//else if (state_items.first == "y_mask_rad_max") {}
				//else if (state_items.first == "use_mask_rad_limits") {}
				//else if (state_items.first == "sensor_max_temperature") {}
				//else if (state_items.first == "sensor_max_temperature") {}
				//else if (state_items.first == "sensor_min_temperature") {}
				//else if (state_items.first == "average_pixel_value_for_beam") {}
				//else if (state_items.first == "min_x_pixel_pos") {}
				//else if (state_items.first == "max_x_pixel_pos") {}
				//else if (state_items.first == "min_y_pixel_pos") {}
				//else if (state_items.first == "min_y_pixel_pos") {}
				//else if (state_items.first == "mask_and_roi_keywords") {}
				//else if (state_items.first == "mask_keywords") {}
				//else if (state_items.first == "roi_keywords") {}

				else if (state_items.first == CameraRecords::ANA_EnableCallbacks_RBV) 
				{
					if(STATE::ANALYSING)

					if (snap.second.get<STATE>(state_items.first) == STATE::ANALYSING)
					{
						camera_map.at(fullName).startAnalysing();
					}
					else
					{
						camera_map.at(fullName).stopAnalysing();
					}
				}
				else if (state_items.first == CameraRecords::CAM_Acquire_RBV)
				{
					if (snap.second.get<STATE>(state_items.first) == STATE::ANALYSING)
					{
						camera_map.at(fullName).startAnalysing();
					}
					else
					{
						camera_map.at(fullName).stopAnalysing();
					}
				}
			}
		}
	}
	return STATE::SUCCESS;
}

// CONVERTERS between boost:python::dict yaml::node and hssm (other conversion happen else where, e.g. hssm to pydict / yaml::node HardwareSnapshot.h
std::map<std::string, HardwareSnapshot> CameraFactory::yamlNodeToHardwareSnapshotMap(const YAML::Node& input_node)
{
	// This function returns a map of <OBJECT NAME: snap parameters> 
	// We fill it from the yaml node data 
	// map of < hardwarename, HardwareSnapshot > 
	// each HardwareSnapshot will be passed to its correpsonding hardware object in another function  
	//
	// Without reading more of the manual, it seem that you can access "nodes within the main node" 
	//  so auto& lower_level_node = snapshot_data[ENUM_TO_STRING(TYPE::MAGNET)] APPEARS NOT TO WORK!!! 
	// (BUT IT MIGHT IF YOU DO IT CORRECTLY) 
	// 
	// TODOD once thsi i sup and runnign maybe write a something better than a long if else tree 
	//
	std::map<std::string, HardwareSnapshot> return_map;
	messenger.printMessage("yamlNodeToHardwareSnapshotMap");
	messenger.printMessage("loop over input_node ");
	for (auto& it : input_node["CAMERA"])
	{
		std::string object_name = getFullName(it.first.as<std::string>());
		std::cout << "(objectname) key = " << object_name << std::endl;
		std::map<std::string, std::string >  value = it.second.as<std::map<std::string, std::string >>();

		//return_map[object_name] = HardwareSnapshot();
		for (auto&& map_it : value)
		{
			std::string record = map_it.first;
			if (record == CameraRecords::CAM_BlackLevel_RBV)
			{
				long new_val = std::stol(map_it.second);
				return_map[object_name].update<long>(record, new_val);
			}
			else if (record == CameraRecords::CAM_Gain_RBV)
			{
				long new_val = std::stol(map_it.second);
				return_map[object_name].update<long>(record, new_val);
			}
			else if (record == CameraRecords::ROI1_MinX_RBV)
			{
				long new_val = std::stol(map_it.second);
				return_map[object_name].update<long>(record, new_val);
			}
			else if (record == CameraRecords::ROI1_MinY_RBV)
			{
				long new_val = std::stol(map_it.second);
				return_map[object_name].update<long>(record, new_val);
			}
			else if (record == CameraRecords::ROI1_SizeX_RBV)
			{
				long new_val = std::stol(map_it.second);
				return_map[object_name].update<long>(record, new_val);
			}
			else if (record == CameraRecords::ROI1_SizeY_RBV)
			{
				long new_val = std::stol(map_it.second);
				return_map[object_name].update<long>(record, new_val);
			}
			else if (record == CameraRecords::ROIandMask_SetX) 
			{
				long new_val = std::stol(map_it.second);
				return_map[object_name].update<long>(record, new_val);
			}
			else if (record == CameraRecords::ROIandMask_SetY) 
			{
				long new_val = std::stol(map_it.second);
				return_map[object_name].update<long>(record, new_val);
			}
			else if (record == CameraRecords::ROIandMask_SetXrad) 
			{
				long new_val = std::stol(map_it.second);
				return_map[object_name].update<long>(record, new_val);
			}
			else if (record == CameraRecords::ROIandMask_SetYrad) 
			{
				long new_val = std::stol(map_it.second);
				return_map[object_name].update<long>(record, new_val);
			}
			else if (record == CameraRecords::ANA_MaskXCenter_RBV)
			{
				long new_val = std::stol(map_it.second);
				return_map[object_name].update<long>(record, new_val);
			}
			else if (record == CameraRecords::ANA_MaskYCenter_RBV)
			{
				long new_val = std::stol(map_it.second);
				return_map[object_name].update<long>(record, new_val);
			}
			else if (record == CameraRecords::ANA_MaskXRad_RBV)
			{
				long new_val = std::stol(map_it.second);
				return_map[object_name].update<long>(record, new_val);
			}
			else if (record == CameraRecords::ANA_MaskYRad_RBV)
			{
				long new_val = std::stol(map_it.second);
				return_map[object_name].update<long>(record, new_val);
			}
			else if (record == CameraRecords::ANA_UseFloor_RBV)
			{
				STATE new_val = GlobalFunctions::stringToState(map_it.second);
				return_map[object_name].update<STATE>(record, new_val);
			}
			else if (record == CameraRecords::ANA_FloorLevel_RBV)
			{
				long new_val = std::stol(map_it.second);
				return_map[object_name].update<long>(record, new_val);
			}
			else if (record == CameraRecords::ANA_UseNPoint_RBV)
			{
				STATE new_val = GlobalFunctions::stringToState(map_it.second);
				return_map[object_name].update<STATE>(record, new_val);
			}
			else if (record == CameraRecords::ANA_NPointStepSize_RBV)
			{
				long new_val = std::stol(map_it.second);
				return_map[object_name].update<long>(record, new_val);
			}
			// can't set background as backgroudn data can't be saved ... 
			else if (record == CameraRecords::ANA_UseBkgrnd_RBV)
			{
				STATE new_val = GlobalFunctions::stringToState(map_it.second);
				return_map[object_name].update<STATE>(record, new_val);
			}
			else if (record == CameraRecords::ANA_OVERLAY_1_CROSS_RBV)
			{
				STATE new_val = GlobalFunctions::stringToState(map_it.second);
				return_map[object_name].update<STATE>(record, new_val);
			}
			else if (record == CameraRecords::ANA_OVERLAY_2_RESULT)
			{
				STATE new_val = GlobalFunctions::stringToState(map_it.second);
				return_map[object_name].update<STATE>(record, new_val);
			}
			else if (record == CameraRecords::ANA_OVERLAY_3_MASK_RBV)
			{
				STATE new_val = GlobalFunctions::stringToState(map_it.second);
				return_map[object_name].update<STATE>(record, new_val);
			}
			else if (record == CameraRecords::HDF_Capture_RBV) 
			{
				STATE new_val = GlobalFunctions::stringToState(map_it.second);
				return_map[object_name].update<STATE>(record, new_val);
			}
			else if (record == CameraRecords::LED_Sta)
			{
				STATE new_val = GlobalFunctions::stringToState(map_it.second);
				return_map[object_name].update<STATE>(record, new_val);
			}
			else if (record == CameraRecords::HDF_NumCapture_RBV)
			{
				long new_val = std::stol(map_it.second);
				return_map[object_name].update<long>(record, new_val);
			}
			else if (record == CameraRecords::ANA_CenterX_RBV)
			{
				long new_val = std::stol(map_it.second);
				return_map[object_name].update<long>(record, new_val);
			}
			else if (record == CameraRecords::ANA_CenterY_RBV)
			{
				long new_val = std::stol(map_it.second);
				return_map[object_name].update<long>(record, new_val);
			}
			else if (record == CameraRecords::ANA_XPix_RBV)
			{
				double new_val = std::stod(map_it.second);
				return_map[object_name].update<double>(record, new_val);
			}
			else if (record == CameraRecords::ANA_YPix_RBV)
			{
				double new_val = std::stod(map_it.second);
				return_map[object_name].update<double>(record, new_val);
			}
			else if (record == CameraRecords::ANA_SigmaXPix_RBV)
			{
				double new_val = std::stod(map_it.second);
				return_map[object_name].update<double>(record, new_val);
			}
			else if (record == CameraRecords::ANA_SigmaYPix_RBV)
			{
				double new_val = std::stod(map_it.second);
				return_map[object_name].update<double>(record, new_val);
			}
			else if (record == CameraRecords::ANA_CovXYPix_RBV)
			{
				double new_val = std::stod(map_it.second);
				return_map[object_name].update<double>(record, new_val);
			}
			else if (record == CameraRecords::ANA_X_RBV)
			{
				double new_val = std::stod(map_it.second);
				return_map[object_name].update<double>(record, new_val);
			}
			else if (record == CameraRecords::ANA_Y_RBV)
			{
				double new_val = std::stod(map_it.second);
				return_map[object_name].update<double>(record, new_val);
			}
			else if (record == CameraRecords::ANA_SigmaX_RBV)
			{
				double new_val = std::stod(map_it.second);
				return_map[object_name].update<double>(record, new_val);
			}
			else if (record == CameraRecords::ANA_SigmaY_RBV)
			{
				double new_val = std::stod(map_it.second);
				return_map[object_name].update<double>(record, new_val);
			}
			else if (record == CameraRecords::ANA_CovXY_RBV)
			{
				double new_val = std::stod(map_it.second);
				return_map[object_name].update<double>(record, new_val);
			}
			else if (record == CameraRecords::ANA_AvgIntensity_RBV)
			{
				double new_val = std::stod(map_it.second);
				return_map[object_name].update<double>(record, new_val);
			}
			else if (record == CameraRecords::ANA_Intensity_RBV)
			{
				double new_val = std::stod(map_it.second);
				return_map[object_name].update<double>(record, new_val);
			}
			else if (record == "busy")
			{
				bool new_val = (bool)std::stoi(map_it.second);
				return_map[object_name].update<bool>(record, new_val);
			}
			else if (record == "max_shots_number")
			{
				size_t new_val = (size_t)std::stoi(map_it.second);
				return_map[object_name].update<size_t>(record, new_val);
			}
			else if (record == "last_capture_and_save_success")
			{
				bool new_val = (bool)std::stoi(map_it.second);
				return_map[object_name].update<bool>(record, new_val);
			}
			else if (record == "aliases")
			{
				return_map[object_name].update<std::string>(record, map_it.second);
			}
			else if (record == "screen_names")
			{
				return_map[object_name].update<std::string>(record, map_it.second);
			}
			else if (record == "average_pixel_value_for_beam")
			{
				double new_val = std::stod(map_it.second);
				return_map[object_name].update<double>(record, new_val);
			}
			else if (record == "array_data_num_pix_x")
			{
				size_t new_val = (size_t)std::stoi(map_it.second);
				return_map[object_name].update<size_t>(record, new_val);
			}
			else if (record == "array_data_num_pix_y")
			{
				size_t new_val = (size_t)std::stoi(map_it.second);
				return_map[object_name].update<size_t>(record, new_val);
			}
			else if (record == "array_data_pixel_count")
			{
				size_t new_val = (size_t)std::stoi(map_it.second);
				return_map[object_name].update<size_t>(record, new_val);
			}
			else if (record == "binary_num_pix_x") 
			{
				size_t new_val = (size_t)std::stoi(map_it.second);
				return_map[object_name].update<size_t>(record, new_val);
			}
			else if (record == "binary_num_pix_y")
			{
				size_t new_val = (size_t)std::stoi(map_it.second);
				return_map[object_name].update<size_t>(record, new_val);
			}
			else if (record == "binary_data_pixel_count")
			{
				size_t new_val = (size_t)std::stoi(map_it.second);
				return_map[object_name].update<size_t>(record, new_val);
			}
			else if (record == "roi_total_pixel_count")
			{
				size_t new_val = (size_t)std::stoi(map_it.second);
				return_map[object_name].update<size_t>(record, new_val);
			}
			else if (record == "operating_centre_x")
			{
				long new_val = std::stol(map_it.second);
				return_map[object_name].update<long>(record, new_val);
			}
			else if (record == "operating_centre_y")
			{
				long new_val = std::stol(map_it.second);
				return_map[object_name].update<long>(record, new_val);
			}
			else if (record == "mechanical_centre_x")
			{
				long new_val = std::stol(map_it.second);
				return_map[object_name].update<long>(record, new_val);
			}
			else if (record == "mechanical_centre_y")
			{
				long new_val = std::stol(map_it.second);
				return_map[object_name].update<long>(record, new_val);
			}
			else if (record == "pix2mmX_ratio")
			{
				double new_val = std::stod(map_it.second);
				return_map[object_name].update<double>(record, new_val);
			}
			else if (record == "pix2mmY_ratio")
			{
				double new_val = std::stod(map_it.second);
				return_map[object_name].update<double>(record, new_val);
			}
			else if (record == "cam_type")
			{
				TYPE new_val = GlobalFunctions::stringToType(map_it.second);
				return_map[object_name].update<TYPE>(record, new_val);
			}
			else if (record == "roi_max_x")
			{
				size_t new_val = (size_t)std::stoi(map_it.second);
				return_map[object_name].update<size_t>(record, new_val);
			}
			else if (record == "roi_max_y")
			{
				size_t new_val = (size_t)std::stoi(map_it.second);
				return_map[object_name].update<size_t>(record, new_val);
			}
			else if (record == "x_pix_scale_factor")
			{
				size_t new_val = (size_t)std::stoi(map_it.second);
				return_map[object_name].update<size_t>(record, new_val);
			}
			else if (record == "y_pix_scale_factor")
			{
				size_t new_val = (size_t)std::stoi(map_it.second);
				return_map[object_name].update<size_t>(record, new_val);
			}
			else if (record == "x_mask_rad_max")
			{
				size_t new_val = (size_t)std::stoi(map_it.second);
				return_map[object_name].update<size_t>(record, new_val);
			}
			else if (record == "y_mask_rad_max") 
			{
				size_t new_val = (size_t)std::stoi(map_it.second);
				return_map[object_name].update<size_t>(record, new_val);
			}
			else if (record == "use_mask_rad_limits")
			{
				bool new_val = (bool)std::stoi(map_it.second);
				return_map[object_name].update<bool>(record, new_val);
			}
			else if (record == "sensor_max_temperature")
			{
				double new_val = std::stod(map_it.second);
				return_map[object_name].update<double>(record, new_val);
			}			
			else if (record == "sensor_min_temperature")
			{
				double new_val = std::stod(map_it.second);
				return_map[object_name].update<double>(record, new_val);
			}
			else if (record == "average_pixel_value_for_beam")
			{
				double new_val = std::stod(map_it.second);
				return_map[object_name].update<double>(record, new_val);
			}
			else if (record == "min_x_pixel_pos")
			{
				double new_val = std::stod(map_it.second);
				return_map[object_name].update<double>(record, new_val);
			}
			else if (record == "max_x_pixel_pos")
			{
				double new_val = std::stod(map_it.second);
				return_map[object_name].update<double>(record, new_val);
			}
			else if (record == "min_y_pixel_pos")
			{
				double new_val = std::stod(map_it.second);
				return_map[object_name].update<double>(record, new_val);
			}
			else if (record == "min_y_pixel_pos")
			{
				double new_val = std::stod(map_it.second);
				return_map[object_name].update<double>(record, new_val);
			}
			else if (record == "mask_and_roi_keywords")
			{
				return_map[object_name].update<std::string>(record, map_it.second);
			}
			else if (record == "mask_keywords")
			{
				return_map[object_name].update<std::string>(record, map_it.second);
			}
			else if (record == "roi_keywords")
			{
				return_map[object_name].update<std::string>(record, map_it.second);
			}
			else if (record == CameraRecords::ANA_EnableCallbacks_RBV)
			{
				STATE new_val = GlobalFunctions::stringToState(map_it.second);
				return_map[object_name].update<STATE>(record, new_val);
			}
			else if (record == CameraRecords::CAM_Acquire_RBV)
			{
				STATE new_val = GlobalFunctions::stringToState(map_it.second);
				return_map[object_name].update<STATE>(record, new_val);
			}
		}
	}
	//// loop over each node (map) in the YAML.NODE
	//messenger.printMessage("loop over Magnet snapshot data");
	//std::cout << "yamlNodeToHardwareSnapshotMap COMPLETE" << std::endl;
	return return_map;
}
std::map<std::string, HardwareSnapshot> CameraFactory::pyDictToHardwareSnapshotMap(const boost::python::dict& input_dict)
{
	// This function returns a map of <OBJECT NAME: HardwareSnapshot > 
	// it is created from a python dictionary that ius assuemd to contain a 3 deep nested dict stcuture
	// IT COULD LOOK Something like :
//{'MAGNET': 
//{'CLA-C2V-MAG-DIP-01': 
//	{'INT_STR': 0.0, 'INT_STR_MM': 0.0, 'K_ANG': 0.0, 'K_DIP_P': 0.0, 'K_MRAD': 0.0, 'K_SET_P': 0.0, 'READI': 5.67, 
//		'RILK': _HardwareFactory.STATE.OK, 'RPOWER': _HardwareFactory.STATE.ON}, 
//	'CLA-C2V-MAG-DIP-02': 
//	{'INT_STR': 0.0, 'INT_STR_MM': 0.0, 'K_ANG': 0.0, 'K_DIP_P': 0.0, 'K_MRAD': 0.0, 'K_SET_P': 0.0, 'READI': 2.09, 
//		'RILK': _HardwareFactory.STATE.OK, 'RPOWER': _HardwareFactory.STATE.ON},
// 'CLA-C2V-MAG-HCOR-01': 
//	{...
//	}
//	...
	std::map<std::string, HardwareSnapshot> return_map;
	messenger.printMessage("pyDictToHardwareSnapshotMap");
	messenger.printMessage("loop over input_node ");
	// TO SOLVE THIS PROPBLEM WE HAVE HAD TO FIURE OUT HOW TO ITERATE OVER NESTED boost::python::dict, 
	// HOW TO ITERATE OVER A BOOST::PYTHON::DICT ???? (after 7 year sof trying ... :( )
	// https://stackoverflow.com/questions/58096040/how-iterate-key-value-boostpythondict
	// 
	auto input_dict_items = input_dict.items();
	for (ssize_t i = 0; i < len(input_dict_items); ++i)
	{
		//messenger.printMessage("loop over input_node ");
		boost::python::object key = input_dict_items[i][0];  // SHOULD BE ''CAMERA'
		boost::python::dict objects = boost::python::extract<boost::python::dict>(input_dict_items[i][1]); // SHOULD BE dict of {magnames: dict of mag snapshot data }
		std::string key_str = boost::python::extract<std::string>(key);
		//messenger.printMessage(key_str, " == MAGNET   ... ??? ");
		auto objects_list = objects.items();
		for (size_t i = 0; i < len(objects_list); ++i)
		{
			boost::python::object name = objects_list[i][0];  // SHOULD BE ''CAMERA'
			boost::python::dict magnet_snapshotdata = boost::python::extract<boost::python::dict>(objects_list[i][1]); // SHOULD BE dict of {magnames: dict of mag snapshot data }
			auto magnet_snapshotdata_items_list = magnet_snapshotdata.items();

			std::string object_name = boost::python::extract<std::string>(name);
			//messenger.printMessage(object_name, " == object_name   ... ??? ");

			for (ssize_t i = 0; i < len(magnet_snapshotdata_items_list); ++i)
			{
				std::string record = boost::python::extract<std::string>(magnet_snapshotdata_items_list[i][0]);   // SHOULD BE ''MAGNET'
				boost::python::object record_value = magnet_snapshotdata_items_list[i][1]; // SHOULD BE dict of {magnames: dict of mag snapshot data }
				//messenger.printMessage("Next record is ", record);

				if (record == CameraRecords::CAM_BlackLevel_RBV)
				{
					long new_val = boost::python::extract<long>(record_value);
					return_map[object_name].update<long>(record, new_val);
				}
				else if (record == CameraRecords::CAM_Gain_RBV)
				{
					long new_val = boost::python::extract<long>(record_value);
					return_map[object_name].update<long>(record, new_val);
				}
				else if (record == CameraRecords::ROI1_MinX_RBV)
				{
					long new_val = boost::python::extract<long>(record_value);
					return_map[object_name].update<long>(record, new_val);
				}
				else if (record == CameraRecords::ROI1_MinY_RBV)
				{
					long new_val = boost::python::extract<long>(record_value);
					return_map[object_name].update<long>(record, new_val);
				}
				else if (record == CameraRecords::ROI1_SizeX_RBV)
				{
					long new_val = boost::python::extract<long>(record_value);
					return_map[object_name].update<long>(record, new_val);
				}
				else if (record == CameraRecords::ROI1_SizeY_RBV)
				{
					long new_val = boost::python::extract<long>(record_value);
					return_map[object_name].update<long>(record, new_val);
				}
				else if (record == CameraRecords::ROIandMask_SetX)
				{
					long new_val = boost::python::extract<long>(record_value);
					return_map[object_name].update<long>(record, new_val);
				}
				else if (record == CameraRecords::ROIandMask_SetY)
				{
					long new_val = boost::python::extract<long>(record_value);
					return_map[object_name].update<long>(record, new_val);
				}
				else if (record == CameraRecords::ROIandMask_SetXrad)
				{
					long new_val = boost::python::extract<long>(record_value);
					return_map[object_name].update<long>(record, new_val);
				}
				else if (record == CameraRecords::ROIandMask_SetYrad)
				{
					long new_val = boost::python::extract<long>(record_value);
					return_map[object_name].update<long>(record, new_val);
				}
				else if (record == CameraRecords::ANA_MaskXCenter_RBV)
				{
					long new_val = boost::python::extract<long>(record_value);
					return_map[object_name].update<long>(record, new_val);
				}
				else if (record == CameraRecords::ANA_MaskYCenter_RBV)
				{
					long new_val = boost::python::extract<long>(record_value);
					return_map[object_name].update<long>(record, new_val);
				}
				else if (record == CameraRecords::ANA_MaskXRad_RBV)
				{
					long new_val = boost::python::extract<long>(record_value);
					return_map[object_name].update<long>(record, new_val);
				}
				else if (record == CameraRecords::ANA_MaskYRad_RBV)
				{
					long new_val = boost::python::extract<long>(record_value);
					return_map[object_name].update<long>(record, new_val);
				}
				else if (record == CameraRecords::ANA_UseFloor_RBV)
				{
					STATE new_val = boost::python::extract<STATE>(record_value);
					return_map[object_name].update<STATE>(record, new_val);
				}
				else if (record == CameraRecords::ANA_FloorLevel_RBV)
				{
					long new_val = boost::python::extract<long>(record_value);
					return_map[object_name].update<long>(record, new_val);
				}
				else if (record == CameraRecords::ANA_UseNPoint_RBV)
				{
					STATE new_val = boost::python::extract<STATE>(record_value);
					return_map[object_name].update<STATE>(record, new_val);
				}
				else if (record == CameraRecords::ANA_NPointStepSize_RBV)
				{
					long new_val = boost::python::extract<long>(record_value);
					return_map[object_name].update<long>(record, new_val);
				}
				// can't set background as backgroudn data can't be saved ... 
				else if (record == CameraRecords::ANA_UseBkgrnd_RBV)
				{
					STATE new_val = boost::python::extract<STATE>(record_value);
					return_map[object_name].update<STATE>(record, new_val);
				}
				else if (record == CameraRecords::ANA_OVERLAY_1_CROSS_RBV)
				{
					STATE new_val = boost::python::extract<STATE>(record_value);
					return_map[object_name].update<STATE>(record, new_val);
				}
				else if (record == CameraRecords::ANA_OVERLAY_2_RESULT)
				{
				STATE new_val = boost::python::extract<STATE>(record_value);
				return_map[object_name].update<STATE>(record, new_val);
				}
				else if (record == CameraRecords::ANA_OVERLAY_3_MASK_RBV)
				{
				STATE new_val = boost::python::extract<STATE>(record_value);
				return_map[object_name].update<STATE>(record, new_val);
				}
				else if (record == CameraRecords::HDF_Capture_RBV)
				{
				STATE new_val = boost::python::extract<STATE>(record_value);
				return_map[object_name].update<STATE>(record, new_val);
				}
				else if (record == CameraRecords::LED_Sta)
				{
				STATE new_val = boost::python::extract<STATE>(record_value);
				return_map[object_name].update<STATE>(record, new_val);
				}
				else if (record == CameraRecords::HDF_NumCapture_RBV)
				{
				long new_val = boost::python::extract<long>(record_value);
				return_map[object_name].update<long>(record, new_val);
				}
				else if (record == CameraRecords::ANA_CenterX_RBV)
				{
				long new_val = boost::python::extract<long>(record_value);
				return_map[object_name].update<long>(record, new_val);
				}
				else if (record == CameraRecords::ANA_CenterY_RBV)
				{
				long new_val = boost::python::extract<long>(record_value);
				return_map[object_name].update<long>(record, new_val);
				}
				else if (record == CameraRecords::ANA_XPix_RBV)
				{
				long new_val = boost::python::extract<long>(record_value);
				return_map[object_name].update<long>(record, new_val);
				}
				else if (record == CameraRecords::ANA_YPix_RBV)
				{
				double new_val = boost::python::extract<double>(record_value);
				return_map[object_name].update<double>(record, new_val);
				}
				else if (record == CameraRecords::ANA_SigmaXPix_RBV)
				{
				double new_val = boost::python::extract<double>(record_value);
				return_map[object_name].update<double>(record, new_val);
				}
				else if (record == CameraRecords::ANA_SigmaYPix_RBV)
				{
				double new_val = boost::python::extract<double>(record_value);
				return_map[object_name].update<double>(record, new_val);
				}
				else if (record == CameraRecords::ANA_CovXYPix_RBV)
				{
				double new_val = boost::python::extract<double>(record_value);
				return_map[object_name].update<double>(record, new_val);
				}
				else if (record == CameraRecords::ANA_X_RBV)
				{
				double new_val = boost::python::extract<double>(record_value);
				return_map[object_name].update<double>(record, new_val);
				}
				else if (record == CameraRecords::ANA_Y_RBV)
				{
				double new_val = boost::python::extract<double>(record_value);
				return_map[object_name].update<double>(record, new_val);
				}
				else if (record == CameraRecords::ANA_SigmaX_RBV)
				{
				double new_val = boost::python::extract<double>(record_value);
				return_map[object_name].update<double>(record, new_val);
				}
				else if (record == CameraRecords::ANA_SigmaY_RBV)
				{
				double new_val = boost::python::extract<double>(record_value);
				return_map[object_name].update<double>(record, new_val);
				}
				else if (record == CameraRecords::ANA_CovXY_RBV)
				{
				double new_val = boost::python::extract<double>(record_value);
				return_map[object_name].update<double>(record, new_val);
				}
				else if (record == CameraRecords::ANA_AvgIntensity_RBV)
				{
				double new_val = boost::python::extract<double>(record_value);
				return_map[object_name].update<double>(record, new_val);
				}
				else if (record == CameraRecords::ANA_Intensity_RBV)
				{
				double new_val = boost::python::extract<double>(record_value);
				return_map[object_name].update<double>(record, new_val);
				}
				else if (record == "busy")
				{
					bool new_val = boost::python::extract<double>(record_value);
					return_map[object_name].update<bool>(record, new_val);
				}
				else if (record == "max_shots_number")
				{
					size_t new_val = boost::python::extract<size_t>(record_value);
					return_map[object_name].update<size_t>(record, new_val);
				}
				else if (record == "last_capture_and_save_success")
				{
					bool new_val = boost::python::extract<double>(record_value);
					return_map[object_name].update<bool>(record, new_val);
				}
				else if (record == "aliases")
				{
					std::string new_val = boost::python::extract<std::string>(record_value);
					return_map[object_name].update<std::string>(record, new_val);
				}
				else if (record == "screen_names")
				{
					std::string new_val = boost::python::extract<std::string>(record_value);
					return_map[object_name].update<std::string>(record, new_val);
				}
				else if (record == "average_pixel_value_for_beam")
				{
					double new_val = boost::python::extract<double>(record_value);
					return_map[object_name].update<double>(record, new_val);
				}
				else if (record == "array_data_num_pix_x")
				{
					size_t new_val = boost::python::extract<size_t>(record_value);
					return_map[object_name].update<size_t>(record, new_val);
				}
				else if (record == "array_data_num_pix_y")
				{
					size_t new_val = boost::python::extract<size_t>(record_value);
					return_map[object_name].update<size_t>(record, new_val);
				}
				else if (record == "array_data_pixel_count")
				{
					size_t new_val = boost::python::extract<size_t>(record_value);
					return_map[object_name].update<size_t>(record, new_val);
				}
				else if (record == "binary_num_pix_x")
				{
					size_t new_val = boost::python::extract<size_t>(record_value);
					return_map[object_name].update<size_t>(record, new_val);
				}
				else if (record == "binary_num_pix_y")
				{
					size_t new_val = boost::python::extract<size_t>(record_value);
					return_map[object_name].update<size_t>(record, new_val);
				}
				else if (record == "binary_data_pixel_count")
				{
					size_t new_val = boost::python::extract<size_t>(record_value);
					return_map[object_name].update<size_t>(record, new_val);
				}
				else if (record == "roi_total_pixel_count")
				{
					size_t new_val = boost::python::extract<size_t>(record_value);
					return_map[object_name].update<size_t>(record, new_val);
				}
				else if (record == "operating_centre_x")
				{
				long new_val = boost::python::extract<long>(record_value);
				return_map[object_name].update<long>(record, new_val);
				}
				else if (record == "operating_centre_y")
				{
				long new_val = boost::python::extract<long>(record_value);
				return_map[object_name].update<long>(record, new_val);
				}
				else if (record == "mechanical_centre_x")
				{
				long new_val = boost::python::extract<long>(record_value);
				return_map[object_name].update<long>(record, new_val);
				}
				else if (record == "mechanical_centre_y")
				{
				long new_val = boost::python::extract<long>(record_value);
				return_map[object_name].update<long>(record, new_val);
				}
				else if (record == "pix2mmX_ratio")
				{
					double new_val = boost::python::extract<double>(record_value);
					return_map[object_name].update<double>(record, new_val);
				}
				else if (record == "pix2mmY_ratio")
				{
					double new_val = boost::python::extract<double>(record_value);
					return_map[object_name].update<double>(record, new_val);
				}
				else if (record == "cam_type")
				{
					TYPE new_val = boost::python::extract<TYPE>(record_value);
					return_map[object_name].update<TYPE>(record, new_val);
				}
				else if (record == "roi_max_x")
				{
					size_t new_val = boost::python::extract<size_t>(record_value);
					return_map[object_name].update<size_t>(record, new_val);
				}
				else if (record == "roi_max_y")
				{
					size_t new_val = boost::python::extract<size_t>(record_value);
					return_map[object_name].update<size_t>(record, new_val);
				}
				else if (record == "x_pix_scale_factor")
				{
					size_t new_val = boost::python::extract<size_t>(record_value);
					return_map[object_name].update<size_t>(record, new_val);
				}
				else if (record == "y_pix_scale_factor")
				{
					size_t new_val = boost::python::extract<size_t>(record_value);
					return_map[object_name].update<size_t>(record, new_val);
				}
				else if (record == "x_mask_rad_max")
				{
					size_t new_val = boost::python::extract<size_t>(record_value);
					return_map[object_name].update<size_t>(record, new_val);
				}
				else if (record == "y_mask_rad_max")
				{
					size_t new_val = boost::python::extract<size_t>(record_value);
					return_map[object_name].update<size_t>(record, new_val);
				}
				else if (record == "use_mask_rad_limits")
				{
					bool new_val = boost::python::extract<double>(record_value);
					return_map[object_name].update<bool>(record, new_val);
				}
				else if (record == "sensor_max_temperature")
				{
					double new_val = boost::python::extract<double>(record_value);
					return_map[object_name].update<double>(record, new_val);
				}
				else if (record == "sensor_min_temperature")
				{
					double new_val = boost::python::extract<double>(record_value);
					return_map[object_name].update<double>(record, new_val);
				}
				else if (record == "average_pixel_value_for_beam")
				{
					double new_val = boost::python::extract<double>(record_value);
					return_map[object_name].update<double>(record, new_val);
				}
				else if (record == "min_x_pixel_pos")
				{
					double new_val = boost::python::extract<double>(record_value);
					return_map[object_name].update<double>(record, new_val);
				}
				else if (record == "max_x_pixel_pos")
				{
					double new_val = boost::python::extract<double>(record_value);
					return_map[object_name].update<double>(record, new_val);
				}
				else if (record == "min_y_pixel_pos")
				{
					double new_val = boost::python::extract<double>(record_value);
					return_map[object_name].update<double>(record, new_val);
				}
				else if (record == "min_y_pixel_pos")
				{
					double new_val = boost::python::extract<double>(record_value);
					return_map[object_name].update<double>(record, new_val);
				}
				else if (record == "mask_and_roi_keywords")
				{
					std::string new_val = boost::python::extract<std::string>(record_value);
					return_map[object_name].update<std::string>(record, new_val);
				}
				else if (record == "mask_keywords")
				{
					std::string new_val = boost::python::extract<std::string>(record_value);
					return_map[object_name].update<std::string>(record, new_val);
				}
				else if (record == "roi_keywords")
				{
					std::string new_val = boost::python::extract<std::string>(record_value);
					return_map[object_name].update<std::string>(record, new_val);
				}
				else if (record == CameraRecords::ANA_EnableCallbacks_RBV)
				{
					STATE new_val = boost::python::extract<STATE>(record_value);
					return_map[object_name].update<STATE>(record, new_val);
				}
				else if (record == CameraRecords::CAM_Acquire_RBV)
				{
					STATE new_val = boost::python::extract<STATE>(record_value);
					return_map[object_name].update<STATE>(record, new_val);
				}
			}
		}
	}
	//// loop over each node (map) in the YAML.NODE
	//messenger.printMessage("loop over Magnet snapshot data");
	//std::cout << "yamlNodeToHardwareSnapshotMap COMPLETE" << std::endl;
	return return_map;
}
YAML::Node CameraFactory::hardwareSnapshotMapToYAMLNode(const std::map<std::string, HardwareSnapshot>& hardwaresnapshot_map)
{
	YAML::Node return_node;
	for (auto& item : hardwaresnapshot_map)
	{
		return_node[ENUM_TO_STRING(TYPE::CAMERA_TYPE)][item.first] = item.second.getYAMLNode();
	}
	return return_node;
}
STATE CameraFactory::checkLastAppliedSnapshot(TYPE type_to_check)
{
	STATE return_state = STATE::SUCCESS;
	//std::map<std::string, HardwareSnapshot> current_snapshot = getSnapshot();

	//std::vector<std::string> wrong_seti;
	//std::vector<std::string> wrong_psu;
	//std::vector<std::string> wrong_name;

	//set_wrong_seti.clear();
	//set_wrong_psu.clear();
	//set_wrong_name.clear();

	//for (auto&& item : hardwareSnapshotMap)
	//{
	//	std::string fullName = getFullName(item.first);

	//	// only apply to magnets that match magnet_type
	//	bool correct_magnet_type = false;
	//	switch (mag_type_to_check)
	//	{
	//	case TYPE::CORRECTOR:
	//		correct_magnet_type = isACor(fullName);
	//		break;
	//	case TYPE::VERTICAL_CORRECTOR:
	//		correct_magnet_type = isAVCor(fullName);
	//		break;
	//	case TYPE::HORIZONTAL_CORRECTOR: correct_magnet_type =
	//		isAHCor(fullName);
	//		break;
	//	case TYPE::QUADRUPOLE:
	//		correct_magnet_type = isAQuad(fullName);
	//		break;
	//	case TYPE::MAGNET:
	//		correct_magnet_type = true;
	//		break;
	//	default:
	//		correct_magnet_type = false;
	//	}

	//	if (correct_magnet_type)
	//	{

	//		if (GlobalFunctions::entryExists(current_snapshot, fullName))
	//		{
	//			auto& hss_ref = current_snapshot.at(fullName);

	//			for (auto&& state_items : item.second.state)
	//			{
	//				if (state_items.first == "GETSETI")
	//				{
	//					if (GlobalFunctions::entryExists(hss_ref.state, state_items.first))
	//					{
	//						double val_now = hss_ref.get<double>("GETSETI");
	//						double val_ref = item.second.get<double>("GETSETI");
	//						double tol = magnetMap.at(fullName).READI_tolerance;
	//						bool compare = GlobalFunctions::areSame(val_now, val_ref, tol);
	//						if (!compare)
	//						{
	//							return_state = STATE::FAIL;
	//							std::pair<double, double> new_seti_pair;
	//							new_seti_pair.first = val_ref;
	//							new_seti_pair.second = val_now;
	//							set_wrong_seti[std::string(fullName)] = new_seti_pair;
	//							messenger.printDebugMessage(fullName, " has wrong GETSETI, (now, ref) ", val_now, " !+ ", val_ref, "  tol set_wrong_seti.size() = ", set_wrong_psu.size());
	//						}
	//					}
	//				}
	//				else if (state_items.first == "RPOWER")
	//				{
	//					if (GlobalFunctions::entryExists(hss_ref.state, state_items.first))
	//					{
	//						bool comp = hss_ref.get<STATE>("RPOWER") == item.second.get<STATE>("RPOWER");
	//						if (!comp)
	//						{
	//							return_state = STATE::FAIL;
	//							std::pair<STATE, STATE> new_psu_pair;
	//							new_psu_pair.first = hss_ref.get<STATE>("RPOWER");
	//							new_psu_pair.second = item.second.get<STATE>("RPOWER");
	//							set_wrong_psu[std::string(fullName)] = new_psu_pair;
	//							messenger.printDebugMessage(fullName, " has wrong RPOWER, set_wrong_psu.size() = ", set_wrong_psu.size());
	//						}
	//					}
	//				}
	//			}
	//		}
	//		else
	//		{
	//			return_state = STATE::FAIL;
	//			set_wrong_name.push_back(item.first);
	//			messenger.printDebugMessage(item.first, " has wrong NAME ");
	//		}
	//	}
	//}
	return  STATE::UNKNOWN;
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
