#include "CameraFactory.h"
#include "CameraPVRecords.h"
#include "GlobalStateEnums.h"
#include "GlobalConstants.h"
#include "GlobalFunctions.h"
#include "PythonTypeConversions.h"
#include <algorithm>

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
}

bool CameraFactory::setup()
{
	return setup(GlobalConstants::nominal);
}
bool CameraFactory::setup(const std::string& version)
{
	return setup(version, TYPE::ALL_VELA_CLARA);
}
bool CameraFactory::setup(TYPE machineArea)
{
	return setup(GlobalConstants::nominal, machineArea);
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
	cutLHarwdareMapByMachineAreas();
	setupChannels();
	EPICSInterface::sendToEPICS();

	for (auto& item : camera_map)
	{
		std::string name(item.second.getHardwareName());
		messenger.printDebugMessage("setting up, ", item.first, ", ", name);
		updateAliasNameMap(item.second);
		std::map<std::string, pvStruct>& pvstruct = item.second.getPVStructs();

		for (auto&& pv : pvstruct)
		{
			if (ca_state(pv.second.CHID) == cs_conn)
			{
				// sets the monitor state in the pvstruict to true or false
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
		EPICSInterface::sendToEPICS();
	}
	hasBeenSetup = true;
	return hasBeenSetup;
}
bool CameraFactory::setup(const std::string& version, const std::vector<std::string>& names)
{
	//machineAreas = machineAreas_IN;
	//messenger.printDebugMessage("called Camera Factory setup  ");
	//std::stringstream ss;
	//ss << "machineAreas = ";
	//for (auto&& area : machineAreas)
	//{
	//	ss << ENUM_TO_STRING(area);
	//	ss << ", ";
	//}
	//messenger.printDebugMessage(ss.str());
	//GlobalFunctions::pause_2000();

	if (hasBeenSetup)
	{
		messenger.printDebugMessage("setup Camera Factory: it has been setup");
		return true;
	}
	if (mode == STATE::VIRTUAL)
	{
		messenger.printDebugMessage("VIRTUAL SETUP: TRUE");
	}
	messenger.printDebugMessage("Calling  populateCameraMap");
	populateCameraMap();
	messenger.printDebugMessage("populateCameraMap finished");
	if (reader.yamlFilenamesAndParsedStatusMap.empty())
	{
		hasBeenSetup = false;
		return hasBeenSetup;
	}
	messenger.printDebugMessage("Calling cutLHarwdareMapByNames");
	cutLHarwdareMapByNames(names);
	messenger.printDebugMessage("cutLHarwdareMapByNames Finished");
	setupChannels();
	EPICSInterface::sendToEPICS();
	for (auto& item : camera_map)
	{
		std::string name(item.second.getHardwareName());
		messenger.printDebugMessage("setting up, ", item.first, ", ", name);
		// update aliases for camera item in map
		updateAliasNameMap(item.second);

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
	messenger.printDebugMessage("Finished Setting up EPICS channels ");
	hasBeenSetup = true;
	return hasBeenSetup;
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
char CameraFactory::getBufferTrigger(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getBufferTrigger();
	}
	return GlobalConstants::char_min;
}
std::string CameraFactory::getBufferFilePath(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getBufferFilePath();
	}
	return std::string(name + " DOES NOT EXIST");
}
std::string CameraFactory::getBufferFileName(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getBufferFileName();
	}
	return std::string(name + " DOES NOT EXIST");
}
long CameraFactory::getBufferFileNumber(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getBufferFileNumber();
	}
	return GlobalConstants::long_min;
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
bool CameraFactory::setBufferTrigger(const std::string& name )
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).setBufferTrigger();
	}
	return false;
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
bool CameraFactory::useNPoint(const std::string& name,bool v)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).useNPoint(v);
	}
	return false;
}
STATE CameraFactory::getNPointState(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getNPointState();
	}
	return STATE::ERR;
}
bool CameraFactory::isUsingNPoint(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).isUsingNPoint();
	}
	return false;
}
bool CameraFactory::isNotUsingNPoint(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).isNotUsingNPoint();
	}
	return false;
}
bool CameraFactory::useBackground(const std::string& name,bool v)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).useBackground(v);
	}
	return false;
}
STATE CameraFactory::getUsingBackgroundState(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getUsingBackgroundState();
	}
	return STATE::ERR;
}
bool CameraFactory::isUsingBackground(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).isUsingBackground();
	}
	return false;
}
bool CameraFactory::isNotUsingBackground(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).isNotUsingBackground();
	}
	return false;
}
long CameraFactory::getStepSize(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getStepSize();
	}
	return GlobalConstants::long_min;
}
bool CameraFactory::setStepSize(const std::string& name, long val)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).setStepSize(val);
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
std::string CameraFactory::getLastBufferDirectoryandFileName(const std::string& name) const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getLastBufferDirectoryandFileName();
	}
	return std::string(name + "DOES NOT EXIST");
}
std::string CameraFactory::getLastBufferDirectory(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getLastBufferDirectory();
	}
	return std::string(name + "DOES NOT EXIST");
}
std::string CameraFactory::getLastBufferFileName(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getLastBufferFileName();
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
bool CameraFactory::setMaskAndROIxPos(const std::string& name, long val)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).setMaskAndROIxPos(val);
	}
	return GlobalConstants::double_min;
}
bool CameraFactory::setMaskAndROIyPos(const std::string& name, long val)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).setMaskAndROIyPos(val);
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
long CameraFactory::getMaskAndROIxPos(const std::string& name )const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getMaskAndROIxPos();
	}
	return GlobalConstants::long_min;
}
long CameraFactory::getMaskAndROIyPos(const std::string& name )const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getMaskAndROIyPos();
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
bool CameraFactory::didLastCaptureAndSaveSucceed(const std::string& name)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).didLastCaptureAndSaveSucceed();
	}
	return false;
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
		return camera_map.at(full_name).setBufferSize(v);
	}
}
void CameraFactory::clearBuffers(const std::string& name)
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).clearBuffers();
	}
}
double CameraFactory::getPix2mm(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getPix2mm();
	}
	return GlobalConstants::double_min;
}
boost::python::dict CameraFactory::getRunningStats(const std::string& name)const
{
	std::string full_name = getFullName(name);
	if (GlobalFunctions::entryExists(camera_map, full_name))
	{
		return camera_map.at(full_name).getRunningStats();
	}
	return boost::python::dict();
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

//
//
//
//std::string CameraFactory::getScreen(const std::string& cam_name) const
//{
//	std::string full_name = getFullName(cam_name);
//	if (GlobalFunctions::entryExists(camera_map, full_name))
//	{
//		return camera_map.at(full_name).getScreen();
//	}
//	return "UNKNOWN";
//}
//std::vector<std::string> CameraFactory::getScreenNames(const std::string& cam_name) const
//{
//	std::string full_name = getFullName(cam_name);
//	if (GlobalFunctions::entryExists(camera_map, full_name))
//	{
//		return camera_map.at(full_name).getScreenNames();
//	}
//	return std::vector<std::string>{"UNKNOWN"};
//}
//boost::python::list CameraFactory::getScreenNames_Py(const std::string& cam_name) const
//{
//	return to_py_list<std::string>(getScreenNames(cam_name));
//}
//
//
//std::vector<std::string> CameraFactory::getNameAliases(const std::string& cam_name) const
//{
//	std::string full_name = getFullName(cam_name);
//	if (GlobalFunctions::entryExists(camera_map, full_name))
//	{
//		return camera_map.at(full_name).getAliases();
//	}
//	return std::vector<std::string>{dummy_cam.getHardwareName()};
//}
///*! get the name alises for this LLRF (python version)
//	@param[ou	return to_py_list<std::string>(getAliases(cam_name));t] names, python list containing all the alias names */
//boost::python::list CameraFactory::getNameAliases_Py(const std::string& cam_name) const
//{
//	return to_py_list<std::string>(getNameAliases(cam_name));
//}

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
			if (it->first == name )
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
