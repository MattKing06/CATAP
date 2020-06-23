#include "Camera.h"
#include "GlobalFunctions.h"
#include "PythonTypeConversions.h"
#include "boost/algorithm/string/split.hpp"
#include <algorithm>
#include "boost/algorithm/string/split.hpp"
#include <boost/make_shared.hpp>
#include "GlobalConstants.h"


Camera::Camera():
Hardware()
{
}

Camera::Camera(const std::map<std::string, std::string>& paramMap, STATE mode) :
Hardware(paramMap, mode),
epicsInterface(boost::make_shared<EPICSCameraInterface>(EPICSCameraInterface())), // calls copy constructor and destroys 
pix2mmX_ratio(std::stof(paramMap.find("ARRAY_DATA_X_PIX_2_MM")->second)),
pix2mmY_ratio(std::stof(paramMap.find("ARRAY_DATA_Y_PIX_2_MM")->second)),
x_pix(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
y_pix(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
sigma_x_pix(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
sigma_y_pix(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
sigma_xy_pix(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
x_mm(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
y_mm(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
sigma_x_mm(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
sigma_y_mm(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
sigma_xy_mm(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min))
{
	messenger.printDebugMessage("Camera Constructor hardwareName = ", hardwareName);
	//epicsInterface->ownerName = hardwareName;
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

	// TODO causes crash 
	messenger.printDebugMessage("epicsInterface set ownerName ");
	epicsInterface->ownerName = hardwareName;

	setPVStructs();

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



		// TODO NO ERROR CHECKING! (we assume config file is good??? 
		std::string PV = specificHardwareParameters.find(record)->second.data();
		// iterate through the list of matches and set up a pvStruct to add to pvStructs.
		messenger.printDebugMessage("Constructing PV information for ", record);

		/*TODO
		  This should be put into some general function: generateVirtualPV(PV) or something...
		  Unless virtual PVs are to be included in the YAML files, they can be dealt with on
		  The config reader level if that is the case.
		  DJS maybe they should, how certian can we be all virtual PVs will get a VM- prefix???
		  */
		if (mode == STATE::VIRTUAL)
		{
			pvStructs[record].fullPVName = "VM-" + PV;
			messenger.printDebugMessage(record, " record, Virtual Camera PV " + pvStructs[record].fullPVName);
		}
		else
		{
			pvStructs[record].fullPVName = PV;
			messenger.printDebugMessage(record, " record, Physical Camera PV " + pvStructs[record].fullPVName);
		}
		//pv.pvRecord = record;
		//chid, count, mask, chtype are left undefined for now.
		//pvStructs[pv.pvRecord] = pv;
	}

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

