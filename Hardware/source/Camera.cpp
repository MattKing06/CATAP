#include "Camera.h"
#include "GlobalFunctions.h"
#include "CameraPVRecords.h"
#include "PythonTypeConversions.h"
#include "boost/algorithm/string/split.hpp"
#include <algorithm>

Camera::Camera():
Hardware()
{
}

Camera::Camera(const std::map<std::string, std::string>& paramMap, STATE mode) :
Hardware(paramMap, mode),
epicsInterface(boost::make_shared<EPICSCameraInterface>(EPICSCameraInterface())), // calls copy constructor and destroys 
pix2mmX_ratio(std::stof(paramMap.find("ARRAY_DATA_X_PIX_2_MM")->second)),  // MAGIC TSRING
pix2mmY_ratio(std::stof(paramMap.find("ARRAY_DATA_Y_PIX_2_MM")->second)),  // MAGIC TSRING
x_pix(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
y_pix(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
sigma_x_pix(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
sigma_y_pix(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
sigma_xy_pix(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
x_mm(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
y_mm(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
sigma_x_mm(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
sigma_y_mm(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
sigma_xy_mm(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
led_status(std::make_pair(epicsTimeStamp(), STATE::UNKNOWN)),
acquire_status(std::make_pair(epicsTimeStamp(), STATE::UNKNOWN)),
analysis_status(std::make_pair(epicsTimeStamp(), STATE::UNKNOWN)),
mask_x_center(std::make_pair(epicsTimeStamp(), GlobalConstants::ushort_min)),
mask_y_center(std::make_pair(epicsTimeStamp(), GlobalConstants::ushort_min)),
mask_x_radius(std::make_pair(epicsTimeStamp(), GlobalConstants::ushort_min)),
mask_y_radius(std::make_pair(epicsTimeStamp(), GlobalConstants::ushort_min)),
x_center(std::make_pair(epicsTimeStamp(), GlobalConstants::ushort_min)),
y_center(std::make_pair(epicsTimeStamp(), GlobalConstants::ushort_min)),
acquire_time(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
acquire_period(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
temperature(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
array_rate(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
cam_type(TYPE::UNKNOWN_TYPE)
{

	setPVStructs();

	// TODO name_alias should be in harwdare constructor?? 
	boost::split(aliases, paramMap.find("name_alias")->second, [](char c) {return c == ','; });
	boost::split(screen_names, paramMap.find("SCREEN_NAME")->second, [](char c) {return c == ','; });
	// REMOVE SPACES FROM THE NAME
	for (auto& name : screen_names)
	{
		name.erase(std::remove_if(name.begin(), name.end(), isspace), name.end());
		messenger.printDebugMessage(hardwareName, " added screen_name " + name);
	}
	for (auto& name : aliases)
	{
		name.erase(std::remove_if(name.begin(), name.end(), isspace), name.end());
		messenger.printDebugMessage(hardwareName, " added aliase " + name);
	}

	// add cmaera type 
	if (GlobalFunctions::entryExists(GlobalConstants::stringToTypeMap, paramMap.at("CAM_TYPE")))
	{
		cam_type = GlobalConstants::stringToTypeMap.at(paramMap.at("CAM_TYPE"));
	}


}

Camera::Camera(const Camera& copyCamera):
Hardware(copyCamera)
{
}

Camera::~Camera()
{
}

void Camera::setPVStructs()
{
	for (auto&& record : CameraRecords::cameraRecordList)
	{
		pvStructs[record] = pvStruct();
		pvStructs[record].pvRecord = record;

		// TODO NO ERROR CHECKING! (we assum config file is good??? 
		std::string PV = specificHardwareParameters.find(record)->second.data();
		// iterate through the list of matches and set up a pvStruct to add to pvStructs.
		//messenger.printDebugMessage("Constructing PV information for ", record);

		/*TODO
		  This should be put into some general function: generateVirtualPV(PV) or something...
		  Unless virtual PVs are to be included in the YAML files, they can be dealt with on
		  The config reader level if that is the case.
		  DJS maybe they should, how certian can we be all virtual PVs will get a VM- prefix???
		  */
		if (mode == STATE::VIRTUAL)
		{
			pvStructs[record].fullPVName = "VM-" + PV;
			std::cout << "Virtual Camera PV " + pvStructs[record].fullPVName << std::endl;
		}
		else
		{
			pvStructs[record].fullPVName = PV;
			std::cout << "Physical Camera PV " + pvStructs[record].fullPVName << std::endl;
		}
		//pv.pvRecord = record;
		//chid, count, mask, chtype are left undefined for now.
		//pvStructs[pv.pvRecord] = pv;
	}

}



TYPE Camera::getCamType()const
{
	return cam_type;
}



double Camera::pix2mmX(double value)const
{
	return value * pix2mmX_ratio;
}
double Camera::pix2mmY(double value)const
{
	return value * pix2mmY_ratio;
}

double Camera::mm2pixX(double value)const
{
	return value / pix2mmX_ratio;
}

double Camera::mm2pixY(double value)const
{
	return value / pix2mmX_ratio;
}


double Camera::getpix2mmX()const
{
	return pix2mmX_ratio;
}
double Camera::getpix2mmY()const
{
	return pix2mmY_ratio;
}
double Camera::setpix2mmX(double value)
{
	return pix2mmX_ratio = value;
}
double Camera::setpix2mmY(double value)
{
	return pix2mmY_ratio = value;
}

double Camera::getX()const
{
	return x_mm.second;
}
double Camera::getY()const
{
	return y_mm.second;
}
double Camera::getSigX()const
{
	return sigma_x_mm.second;
}
double Camera::getSigY()const
{
	return sigma_y_mm.second;
}
double Camera::getSigXY()const
{
	return sigma_xy_mm.second;
}

double Camera::getXPix()const
{
	return x_pix.second;
}
double Camera::getYPix()const
{
	return y_pix.second;
}
double Camera::getSigXPix()const
{
	return sigma_x_pix.second;
}
double Camera::getSigYPix()const
{
	return sigma_y_pix.second;
}
double Camera::getSigXYPix()const
{
	return sigma_xy_pix.second;
}

double Camera::getSumIntensity()const
{
	return sum_intensity.second;
}
double Camera::getAvgIntensity()const
{
	return avg_intensity.second;
}



bool Camera::setX(double value)
{
	if(mode == STATE::PHYSICAL)
	{
		return false;
	}
	x_mm = std::make_pair(epicsTimeStamp(), value);
	x_pix = std::make_pair(x_mm.first, mm2pixX(value));
	return true;
}
bool Camera::setY(double value)
{
	if (mode == STATE::PHYSICAL)
	{
		return false;
	}
	y_mm = std::make_pair(epicsTimeStamp(), value);
	y_pix = std::make_pair(y_mm.first, mm2pixY(value));
	return true;
}
bool Camera::setSigX(double value)
{
	if (mode == STATE::PHYSICAL)
	{
		return false;
	}
	sigma_x_mm = std::make_pair(epicsTimeStamp(), value);
	sigma_x_pix = std::make_pair(sigma_x_mm.first, mm2pixX(value));
	return true;
}
bool Camera::setSigY(double value)
{
	if (mode == STATE::PHYSICAL)
	{
		return false;
	}
	sigma_y_mm = std::make_pair(epicsTimeStamp(), value);
	sigma_y_pix = std::make_pair(sigma_y_mm.first, mm2pixY(value));
	return true;
}
bool Camera::setSigXY(double value)
{
	if (mode == STATE::PHYSICAL)
	{
		return false;
	}
	sigma_xy_mm = std::make_pair(epicsTimeStamp(), value);
	sigma_xy_pix = std::make_pair(epicsTimeStamp(), mm2pixX(value));
	return true;
}

//bool Camera::setXPix(double value)
//{
//	if (mode == STATE::PHYSICAL)
//	{
//		return false;
//	}
//	x_pix = std::make_pair(epicsTimeStamp(), value);
//	return true;
//}
//bool Camera::setYPix(double value)
//{
//	if (mode == STATE::PHYSICAL)
//	{
//		return false;
//	}
//	y_pix = std::make_pair(epicsTimeStamp(), value);
//	return true;
//}
//bool Camera::setSigXPix(double value)
//{
//	if (mode == STATE::PHYSICAL)
//	{
//		return false;
//	}
//	sigma_x_pix = std::make_pair(epicsTimeStamp(), value);
//	return true;
//}
//bool Camera::setSigYPix(double value)
//{
//	if (mode == STATE::PHYSICAL)
//	{
//		return false;
//	}
//	sigma_y_pix = std::make_pair(epicsTimeStamp(), value);
//	return true;
//}
//bool Camera::setSigXYPix(double value)
//{
//	if (mode == STATE::PHYSICAL)
//	{
//		return false;
//	}
//	sigma_xy_pix = std::make_pair(epicsTimeStamp(), value);
//	return true;
//}

bool Camera::setSumIntensity(double value)
{
	if (mode == STATE::PHYSICAL)
	{
		return false;
	}
	sum_intensity = std::make_pair(epicsTimeStamp(), value);
	return true;
}
bool Camera::setAvgIntensity(double value)
{
	if (mode == STATE::PHYSICAL)
	{
		return false;
	}
	avg_intensity = std::make_pair(epicsTimeStamp(), value);
	return true;
}


unsigned short Camera::getMaskXCenter()const
{
	return mask_x_center.second;
}
unsigned short Camera::getMaskYCenter()const
{
	return mask_y_center.second;
}
unsigned short Camera::getMaskXRadius()const
{
	return mask_x_radius.second;
}
unsigned short Camera::getMaskYRadius()const
{
	return mask_y_radius.second;
}

unsigned short Camera::setMaskXCenter(unsigned short val)
{
	return  epicsInterface->putValue2<unsigned short>(pvStructs.at(CameraRecords::ANA_MaskXCenter), val);
}
unsigned short Camera::setMaskYCenter(unsigned short val)
{
	return  epicsInterface->putValue2<unsigned short>(pvStructs.at(CameraRecords::ANA_MaskYCenter), val);
}
unsigned short Camera::setMaskXRadius(unsigned short val)
{
	return  epicsInterface->putValue2<unsigned short>(pvStructs.at(CameraRecords::ANA_MaskXRad), val);
}
unsigned short Camera::setMaskYRadius(unsigned short val)
{
	return  epicsInterface->putValue2<unsigned short>(pvStructs.at(CameraRecords::ANA_MaskYRad), val);
}



boost::python::dict Camera::getRunningStats()
{
	boost::python::dict r;
	r["x_pix"] = x_pix_rs.getRunningStats();
	r["y_pix"] = y_pix_rs.getRunningStats();
	r["sigma_x_pix"] = sigma_x_pix_rs.getRunningStats();
	r["sigma_y_pix"] = sigma_y_pix_rs.getRunningStats();
	r["sigma_xy_pix"] = sigma_xy_pix_rs.getRunningStats();
	r["x_mm"] = x_mm_rs.getRunningStats();
	r["y_mm"] = y_mm_rs.getRunningStats();
	r["sigma_x_mm"] = sigma_x_mm_rs.getRunningStats();
	r["sigma_y_mm"] = sigma_y_mm_rs.getRunningStats();
	r["sigma_xy_mm"] =  sigma_xy_mm_rs.getRunningStats();
	return r;
}

size_t Camera::getBufferSize()const
{
	return running_stats_buffer_size;
}

void Camera::setBufferSize(size_t v)
{
	x_pix_rs.setBufferSize(v);
	y_pix_rs.setBufferSize(v);
	sigma_x_pix_rs.setBufferSize(v);
	sigma_y_pix_rs.setBufferSize(v);
	sigma_xy_pix_rs.setBufferSize(v);
	x_mm_rs.setBufferSize(v);
	y_mm_rs.setBufferSize(v);
	sigma_x_mm_rs.setBufferSize(v);
	sigma_y_mm_rs.setBufferSize(v);
	sigma_xy_mm_rs.setBufferSize(v);
	running_stats_buffer_size = v;
}
void Camera::clearBuffers()
{
	x_pix_rs.clearBuffer();
	y_pix_rs.clearBuffer();
	sigma_x_pix_rs.clearBuffer();
	sigma_y_pix_rs.clearBuffer();
	sigma_xy_pix_rs.clearBuffer();
	x_mm_rs.clearBuffer();
	y_mm_rs.clearBuffer();
	sigma_x_mm_rs.clearBuffer();
	sigma_y_mm_rs.clearBuffer();
	sigma_xy_mm_rs.clearBuffer();
}



bool Camera::startAcquiring()
{
	return  epicsInterface->putValue2<unsigned short >(pvStructs.at(CameraRecords::CAM_Acquire), GlobalConstants::one_ushort);
}
bool Camera::stopAcquiring()
{
	return  epicsInterface->putValue2<unsigned short >(pvStructs.at(CameraRecords::CAM_Acquire), GlobalConstants::zero_ushort);
}
bool Camera::isAcquiring()const
{
	return getAcquireState() == STATE::ACQUIRING;
}
bool Camera::isNotAcquiring() const
{
	return getAcquireState() == STATE::NOT_ACQUIRING;
}
STATE Camera::getAcquireState()const
{
	return acquire_status.second;
}


bool Camera::startAnalysing()
{
	return  epicsInterface->putValue2<unsigned short >(pvStructs.at(CameraRecords::ANA_EnableCallbacks), GlobalConstants::one_ushort);
}
bool Camera::stopAnalysing()
{
	return  epicsInterface->putValue2<unsigned short >(pvStructs.at(CameraRecords::ANA_EnableCallbacks), GlobalConstants::zero_ushort);
}
bool Camera::isAnalysing()const
{
	return getAcquireState() == STATE::ANALYSING;
}
bool Camera::isNotAnalysing() const
{
	return getAcquireState() == STATE::NOT_ANALYSING;
}
STATE Camera::getAnalysisState( )const
{
	return analysis_status.second;
}






std::vector<std::string> Camera::getAliases() const
{
	return aliases;
}
boost::python::list Camera::getAliases_Py() const
{
	return to_py_list<std::string>(getAliases());
}

std::string Camera::getScreen()const
{
	if (screen_names.size() > 0)
	{
		return screen_names[0];
	}
	return "UNKNOWN";
}

std::vector<std::string> Camera::getScreenNames() const
{
	return screen_names;
}
boost::python::list Camera::getScreenNames_Py() const
{
	return to_py_list<std::string>(getScreenNames());
}

void Camera::debugMessagesOn()
{
	messenger.debugMessagesOn();
	messenger.printDebugMessage(hardwareName, " - DEBUG ON");
	epicsInterface->debugMessagesOn();
}

void Camera::debugMessagesOff()
{
	messenger.debugMessagesOff();
	messenger.printDebugMessage(hardwareName, " - DEBUG OFF");
	epicsInterface->debugMessagesOff();
}

void Camera::messagesOn()
{
	messenger.messagesOn();
	messenger.printMessage(hardwareName, " - MESSAGES ON");
	epicsInterface->messagesOn();
}

void Camera::messagesOff()
{
	messenger.printMessage(hardwareName, " - MESSAGES OFF");
	messenger.messagesOff();
	epicsInterface->messagesOff();
}

