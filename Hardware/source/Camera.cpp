#include "Camera.h"
#include "GlobalFunctions.h"
#include "CameraPVRecords.h"
#include "PythonTypeConversions.h"
#include "boost/algorithm/string/split.hpp"
#include <algorithm>
#include <chrono> 
#include <mutex>          // std::mutex
std::mutex mtx;           // mutex for critical section
Camera::Camera():
Hardware()
{
}

Camera::Camera(const std::map<std::string, std::string>& paramMap, STATE mode) :
	Hardware(paramMap, mode),
	//pointer_to_array_data(nullptr),
	epicsInterface(boost::make_shared<EPICSCameraInterface>(EPICSCameraInterface())), // calls copy constructor and destroys 
	pix2mmX_ratio(std::stof(paramMap.find("ARRAY_DATA_X_PIX_2_MM")->second)),  // MAGIC STRING
	pix2mmY_ratio(std::stof(paramMap.find("ARRAY_DATA_Y_PIX_2_MM")->second)),  // MAGIC STRING
	max_shots_number((size_t)std::stoi(paramMap.find("MAX_SHOTS_NUMBER")->second)),  // MAGIC STRING
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
	led_state(std::make_pair(epicsTimeStamp(), STATE::UNKNOWN)),
	acquire_state(std::make_pair(epicsTimeStamp(), STATE::UNKNOWN)),
	analysis_state(std::make_pair(epicsTimeStamp(), STATE::UNKNOWN)),
	mask_x_center(std::make_pair(epicsTimeStamp(), GlobalConstants::long_min)),
	mask_y_center(std::make_pair(epicsTimeStamp(), GlobalConstants::long_min)),
	mask_x_radius(std::make_pair(epicsTimeStamp(), GlobalConstants::long_min)),
	mask_y_radius(std::make_pair(epicsTimeStamp(), GlobalConstants::long_min)),
	x_center(std::make_pair(epicsTimeStamp(), GlobalConstants::long_min)),
	y_center(std::make_pair(epicsTimeStamp(), GlobalConstants::long_min)),
	step_size(std::make_pair(epicsTimeStamp(), GlobalConstants::long_min)),
	acquire_time(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
	acquire_period(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
	temperature(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
	array_rate(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
	use_npoint(std::make_pair(epicsTimeStamp(), STATE::UNKNOWN)),
	use_background(std::make_pair(epicsTimeStamp(), STATE::UNKNOWN)),
	pixel_to_mm(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
	cam_type(TYPE::UNKNOWN_TYPE),
	mask_and_roi_keywords({ "x_pos", "y_pos", "x_size", "x_size" }),  //MAGIC STRING
	mask_keywords({ "mask_x", "mask_y", "mask_rad_x", "mask_rad_y" }),//MAGIC STRING 
	roi_keywords({ "roi_x", "roi_y", "roi_rad_x", "roi_rad_y" }),     //MAGIC STRING
	//mask_and_roi_keywords_Py(to_py_list(mask_and_roi_keywords)),
	//mask_keywords_Py(to_py_list(mask_keywords)),
	//roi_keywords_Py(to_py_list(roi_keywords))
	image_data_has_not_malloced(true),
	image_data_has_not_vector_resized(true),
	roi_data_has_not_malloced(true),
	roi_data_has_not_vector_resized(true)
{

	for (auto&& item : paramMap)
	{
		messenger.printDebugMessage(item.first, " = ", item.second);
	}
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
	else
	{
		messenger.printDebugMessage(hardwareName, " !!ERROR!! config file CAM_TYPE = ", paramMap.at("CAM_TYPE"));
	}
	//mask_and_roi_keywords = std::vector<std::string>{ "x_pos", "y_pos", "x_size", "x_size" };  //MAGIC STRING
	//mask_keywords = std::vector<std::string>{ "mask_x", "mask_y", "mask_rad_x", "mask_rad_y" };//MAGIC STRING 
	//roi_keywords = std::vector<std::string>{ "roi_x", "roi_y", "roi_rad_x", "roi_rad_y" };     //MAGIC STRING
	mask_and_roi_keywords_Py = to_py_list(mask_and_roi_keywords);
	mask_keywords_Py = to_py_list(mask_keywords);
	roi_keywords_Py = to_py_list(roi_keywords);

	// set the number of pixels in the (decminated) camera image 
	int num_pix_x = std::stoi(paramMap.find("ARRAY_DATA_NUM_PIX_X")->second);
	int num_pix_y = std::stoi(paramMap.find("ARRAY_DATA_NUM_PIX_Y")->second);
	num_pixels = num_pix_x * num_pix_y;
}

Camera::Camera(const Camera& copyCamera):
Hardware(copyCamera),
mask_and_roi_keywords(copyCamera.mask_and_roi_keywords),
mask_keywords(copyCamera.mask_keywords),
roi_keywords(copyCamera.roi_keywords)
//mask_and_roi_keywords_Py(copyCamera.mask_and_roi_keywords_Py),
//mask_keywords_Py(copyCamera.mask_keywords_Py),
//roi_keywords_Py(copyCamera.roi_keywords_Py)
{
}
Camera::~Camera()
{
}
void Camera::setPVStructs()
{
	for (auto&& record : CameraRecords::cameraRecordList)
	{
		
		if (GlobalFunctions::entryExists(specificHardwareParameters, record))
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
		else
		{
			std::cout << "Can't find record = "<< record << std::endl;
		}

	}

}
TYPE Camera::getCamType()const
{
	return cam_type;
}
double Camera::pix2mmX(double value)const
{
	return value / pix2mmX_ratio;
}
double Camera::pix2mmY(double value)const
{
	return value / pix2mmY_ratio;
}
double Camera::mm2pixX(double value)const
{
	return value * pix2mmX_ratio;
}
double Camera::mm2pixY(double value)const
{
	return value * pix2mmX_ratio;
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
long Camera::getStepSize()const
{
	return step_size.second;
}
bool Camera::setStepSize(long val)
{
	return epicsInterface->putValue2<long>(pvStructs.at(CameraRecords::ANA_StepSize), val);
}
bool Camera::setX(double value)
{
	if(mode == STATE::PHYSICAL)
	{
		return  false;
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
bool Camera::setBufferTrigger()
{
	if (mode == STATE::PHYSICAL)
	{
		char v = 1;
		return epicsInterface->putValue2<char>(pvStructs.at(CameraRecords::HDFB_Buffer_Trigger), v);
	}
	return false;
}
//
//bool Camera::setROIminX(long v)
//{
//	if (mode == STATE::PHYSICAL)
//	{
//		return epicsInterface->putValue2<long>(pvStructs.at(CameraRecords::ROI1_MinX), v);
//	}
//	return false;
//}
//bool Camera::setROIminY(long v)
//{
//	if (mode == STATE::PHYSICAL)
//	{
//		return epicsInterface->putValue2<long>(pvStructs.at(CameraRecords::ROI1_MinY), v);
//	}
//	return false;
//}
//bool Camera::setROIsizeX(long v)
//{
//	if (mode == STATE::PHYSICAL)
//	{
//		return epicsInterface->putValue2<long>(pvStructs.at(CameraRecords::ROI1_SizeX), v);
//	}
//	return false;
//}
//bool Camera::setROIsizeY(long v)
//{
//	if (mode == STATE::PHYSICAL)
//	{
//		return epicsInterface->putValue2<long>(pvStructs.at(CameraRecords::ROI1_SizeY), v);
//	}
//	return false;
//}
bool Camera::setLEDOn()
{
	// ALSO HAVE IN SCREENS 
	if (epicsInterface->putValue2(pvStructs.at(CameraRecords::LED_On), GlobalConstants::EPICS_ACTIVATE))
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(150));//MAGIC_NUMBER!
		return epicsInterface->putValue2(pvStructs.at(CameraRecords::LED_On), GlobalConstants::EPICS_SEND);
	}
	messenger.printDebugMessage("Send setLEDOn EPICS_ACTIVATE failed ");
	return false;
}
bool Camera::setLEDOff()
{
	// ALSO HAVE IN SCREENS
	if(epicsInterface->putValue2(pvStructs.at(CameraRecords::LED_Off), GlobalConstants::EPICS_ACTIVATE))
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(150));//MAGIC_NUMBER!
		return epicsInterface->putValue2(pvStructs.at(CameraRecords::LED_Off), GlobalConstants::EPICS_SEND);
	}
	messenger.printDebugMessage("Send LED_Off EPICS_ACTIVATE failed ");
	return false;
}
bool Camera::isLEDOn()const
{
	return led_state.second == STATE::ON;
}
bool Camera::isLEDOff()const
{
	return led_state.second == STATE::OFF;
}
STATE Camera::getLEDState()const
{
	return led_state.second;
}
//--------------------------------------------------------------------------------------------------
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
bool Camera::setMaskXCenter(long val)
{
	messenger.printMessage("setMaskXCenter ", val);
	return  epicsInterface->putValue2<dbr_long_t>(pvStructs.at(CameraRecords::ANA_MaskXCenter), (dbr_long_t)val);
}
bool Camera::setMaskYCenter(long val)
{
	return  epicsInterface->putValue2<dbr_long_t>(pvStructs.at(CameraRecords::ANA_MaskYCenter), (dbr_long_t)val);
}
bool Camera::setMaskXRadius(long val)
{
	return  epicsInterface->putValue2<dbr_long_t>(pvStructs.at(CameraRecords::ANA_MaskXRad), (dbr_long_t)val);
}
bool Camera::setMaskYRadius(long val)
{
	return  epicsInterface->putValue2<dbr_long_t>(pvStructs.at(CameraRecords::ANA_MaskYRad), (dbr_long_t)val);
}
bool Camera::setMask(long mask_x, long  mask_y, long mask_rad_x, long mask_rad_y)
{
	if (setMaskXCenter(mask_x))
	{
		if (setMaskYCenter(mask_y))
		{
			if (setMaskXRadius(mask_rad_x))
			{
				if (setMaskYRadius(mask_rad_y))
				{
					return true;
				}
			}
		}
	}
	return false;
}
bool Camera::setMask(std::map<std::string, long> settings)
{
	if (GlobalFunctions::entriesExist(settings, mask_keywords))
	{
		return setMask(settings.at("mask_x"), settings.at("mask_y"), settings.at("mask_rad_x"), settings.at("mask_rad_y"));
	}
	else
	{
		messenger.printMessage("setMask passed map with incorrect keywords");
	}
	return false;
}
bool Camera::setMask_Py(boost::python::dict settings)
{
	return setMask(to_std_map<std::string, long>(settings));

}
bool Camera::setROIMinX(long val)
{
	return  epicsInterface->putValue2<dbr_long_t>(pvStructs.at(CameraRecords::ROI1_MinX), (dbr_long_t)val);
}
bool Camera::setROIMinY(long val)
{
	return  epicsInterface->putValue2<dbr_long_t>(pvStructs.at(CameraRecords::ROI1_MinY), (dbr_long_t)val);
}
bool Camera::setROISizeX(long val)
{
	return  epicsInterface->putValue2<dbr_long_t>(pvStructs.at(CameraRecords::ROI1_SizeX), (dbr_long_t)val);
}
bool Camera::setROISizeY(long val)
{
	return  epicsInterface->putValue2<dbr_long_t>(pvStructs.at(CameraRecords::ROI1_SizeY), (dbr_long_t)val);
}
bool Camera::setROI(long roi_x, long  roi_y, long roi_rad_x, long roi_rad_y)
{
	if (setMaskXCenter(roi_x))
	{
		if (setMaskYCenter(roi_y))
		{
			if (setMaskXRadius(roi_rad_x))
			{
				if (setMaskYRadius(roi_rad_y))
				{
					return true;
				}
			}
		}
	}
	return false;
}
bool Camera::setROI(std::map<std::string, long> settings)
{
	if (GlobalFunctions::entriesExist(settings, mask_keywords))
	{
		return setROI(settings.at("roi_x"), settings.at("roi_y"), settings.at("roi_rad_x"), settings.at("roi_rad_y"));
	}
	return false;
}
bool Camera::setROI_Py(boost::python::dict settings)
{
	return setROI(to_std_map<std::string, long>(settings));

}
long Camera::getMaskAndROIxPos()const
{
	return roi_and_mask_centre_x.second; // NEEDS CHECKING
}
long Camera::getMaskAndROIyPos()const
{
	return roi_and_mask_centre_y.second; // NEEDS CHECKING
}
long Camera::getMaskAndROIxSize()const
{
	return roi_and_mask_radius_x.second; // NEEDS CHECKING 
}
long Camera::getMaskAndROIySize()const
{
	return roi_and_mask_radius_y.second; // NEEDS CHECKING
}
bool Camera::setMaskAndROIxPos(long val)
{
	return  epicsInterface->putValue2<double>(pvStructs.at(CameraRecords::ROIandMask_SetX), (double)val);
}
bool Camera::setMaskAndROIyPos(long val)
{
	return  epicsInterface->putValue2<double>(pvStructs.at(CameraRecords::ROIandMask_SetY), (double)val);
}
bool Camera::setMaskAndROIxSize(long val)
{
	return  epicsInterface->putValue2<double>(pvStructs.at(CameraRecords::ROIandMask_SetXrad), (double)val);
}
bool Camera::setMaskAndROIySize(long val)
{
	return  epicsInterface->putValue2<double>(pvStructs.at(CameraRecords::ROIandMask_SetYrad), (double)val);
}
bool Camera::setMaskandROI(long x_pos, long  y_pos, long x_size, long y_size)
{
	if(setMaskAndROIxPos(x_pos))
	{
		if (setMaskAndROIyPos(y_pos))
		{
			if (setMaskAndROIxSize(x_size))
			{
				if (setMaskAndROIySize(y_size))
				{
					return true;
				}
			}
		}
	}
	return false;
}
bool Camera::setMaskandROI(std::map<std::string, long> settings)
{
	if (GlobalFunctions::entriesExist(settings, mask_and_roi_keywords))
	{
		return setMaskandROI(settings.at("x_pos"), settings.at("y_pos"), settings.at("x_size"), settings.at("y_size"));
	}
	return false;
}
bool Camera::setMaskandROI_Py(boost::python::dict settings)
{
	return setMaskandROI(to_std_map<std::string, long>(settings));
}
bool Camera::useNPoint(bool v)
{
	unsigned short comm = v ? GlobalConstants::one_ushort : GlobalConstants::zero_ushort;
	return  epicsInterface->putValue2<unsigned short >(pvStructs.at(CameraRecords::ANA_UseNPoint), comm);
}
STATE Camera::getNPointState()const
{
	return use_npoint.second;
}
bool Camera::isUsingNPoint()const
{
	return use_npoint.second == STATE::USING_NPOINT;
}
bool Camera::isNotUsingNPoint()const
{
	return use_npoint.second == STATE::NOT_USING_NPOINT;
}
bool Camera::useBackground(bool v)
{
	unsigned short comm = v ? GlobalConstants::one_ushort : GlobalConstants::zero_ushort;
	messenger.printDebugMessage(hardwareName, " useBackground, ", comm);
	return  epicsInterface->putValue2<unsigned short >(pvStructs.at(CameraRecords::ANA_UseBkgrnd), comm);
}
bool Camera::isUsingBackground()const
{
	return use_background.second == STATE::USING_BACKGROUND;
}
bool Camera::isNotUsingBackground()const
{
	return use_background.second == STATE::NOT_USING_BACKGROUND;
}
STATE Camera::getUsingBackgroundState()const
{
	return use_background.second;
}
double Camera::getPix2mm()const
{
	return pixel_to_mm.second;
}
long Camera::getMaskXCenter()const
{
	return mask_x_center.second;
}
long Camera::getMaskYCenter()const
{
	return mask_y_center.second;
}
long Camera::getMaskXRadius()const
{
	return mask_x_radius.second;
}
long Camera::getMaskYRadius()const
{
	return mask_y_radius.second;
}
std::map<std::string, long> Camera::getMask()
{
	std::map<std::string, long> r;
	r["mask_x"] = getMaskXCenter(); // MAGIC STRING
	r["mask_y"] = getMaskYCenter(); // MAGIC STRING
	r["mask_rad_x"] = getMaskXRadius();// MAGIC STRING
	r["mask_rad_x"] = getMaskYRadius();// MAGIC STRING
	return r;
}
boost::python::dict Camera::getMask_Py()
{
	return  to_py_dict<std::string, long>(getMask());
}
long Camera::getROIMinX()const
{
	return roi_min_x.second;
}
long Camera::getROIMinY()const
{
	return roi_min_y.second;
}
long Camera::getROISizeX()const
{
	return roi_size_x.second;
}
long Camera::getROISizeY()const
{
	return roi_size_y.second;
}
std::map<std::string, long> Camera::getROI()
{
	std::map<std::string, long> r;
	r["x_pos"] = getROIMinX(); // MAGIC STRING
	r["y_pos"] = getROIMinY(); // MAGIC STRING
	r["x_size"] = getROISizeX();// MAGIC STRING
	r["y_size"] = getROISizeY();// MAGIC STRING
	return r;
}
boost::python::dict Camera::getROI_Py()
{
	return  to_py_dict<std::string, long>(getMaskandROI());
}
std::map<std::string, long> Camera::getMaskandROI()const
{
	std::map<std::string, long> r;
	r["x_pos"] = getROIMinX(); // MAGIC STRING
	r["y_pos"] = getROIMinY(); // MAGIC STRING
	r["x_size"] = getROISizeX();// MAGIC STRING
	r["y_size"] = getROISizeY();// MAGIC STRING
	return r;
}
boost::python::dict Camera::getMaskandROI_Py()const
{
	return  to_py_dict<std::string, long>(getMaskandROI());
}
boost::python::dict Camera::getRunningStats()const
{
	boost::python::dict r;
	r["x_pix"] = x_pix_rs.getRunningStats();  				// MAGIC STRING
	r["y_pix"] = y_pix_rs.getRunningStats();  				// MAGIC STRING
	r["sigma_x_pix"] = sigma_x_pix_rs.getRunningStats();    // MAGIC STRING
	r["sigma_y_pix"] = sigma_y_pix_rs.getRunningStats();    // MAGIC STRING
	r["sigma_xy_pix"] = sigma_xy_pix_rs.getRunningStats();  // MAGIC STRING
	r["x_mm"] = x_mm_rs.getRunningStats();				    // MAGIC STRING
	r["y_mm"] = y_mm_rs.getRunningStats();				    // MAGIC STRING
	r["sigma_x_mm"] = sigma_x_mm_rs.getRunningStats();	    // MAGIC STRING
	r["sigma_y_mm"] = sigma_y_mm_rs.getRunningStats();	    // MAGIC STRING
	r["sigma_xy_mm"] =  sigma_xy_mm_rs.getRunningStats();   // MAGIC STRING
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
	return acquire_state.second;
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
	return getAnalysisState() == STATE::ANALYSING;
}
bool Camera::isNotAnalysing() const
{
	return getAnalysisState() == STATE::NOT_ANALYSING;
}
STATE Camera::getAnalysisState( )const
{
	return analysis_state.second;
}
//
//  __   __             ___  __  ___               __      __             ___                  __   ___
// /  ` /  \ |    |    |__  /  `  |      /\  |\ | |  \    /__`  /\  \  / |__     |  |\/|  /\  / _` |__
// \__, \__/ |___ |___ |___ \__,  |     /~~\ | \| |__/    .__/ /~~\  \/  |___    |  |  | /~~\ \__> |___
//
//
//-------------------------------------------------------------------------------------------------------
//
//
bool Camera::captureAndSave(size_t num_shots)
{
	if (isNotAcquiring())
	{
		return false;
	}
	else
	{
		messenger.printDebugMessage(hardwareName, " collectAndSave passed num_shots = ", num_shots);
		/*
			kill any finished threads
		*/
		killFinishedImageCollectThread();
		messenger.printDebugMessage("killFinishedImageCollectThreads fin");
		messenger.printDebugMessage(" exists in imageCollectStructs");
		if (image_capture.thread == nullptr)
		{
			messenger.printDebugMessage(" is not busy");
			if (num_shots <= max_shots_number)
			{
				messenger.printDebugMessage("Requested number of shots ok, create new imageCollectStructs");

				image_capture.is_busy= true;
				image_capture.status = STATE::CAPTURING;
				image_capture.num_shots = num_shots;
				image_capture.cam = this;
				image_capture.thread
					= new std::thread(staticEntryImageCollectAndSave, std::ref(image_capture));
				GlobalFunctions::pause_500();
				messenger.printDebugMessage("new imageCollectStruct created and running");
				return true;
			}
			else
			{
				messenger.printDebugMessage("!!ERROR!! Requested number of camera images too large. Please set to be less than or equal to ",
					max_shots_number);
			}
		}
		else
		{
			messenger.printDebugMessage(hardwareName, " imageCollectStructs is busy ");
		}
	}
	return false;
}
void Camera::killFinishedImageCollectThread()
{
	messenger.printDebugMessage("killFinishedImageCollectThreads");
	if (image_capture.is_busy)
	{
		messenger.printDebugMessage(hardwareName, " ImageCapture is busy");
	}
	else
	{
		messenger.printDebugMessage(hardwareName, " ImageCapture is NOT busy");
		if (image_capture.thread)
		{
			messenger.printDebugMessage(hardwareName, " thread is not nullptr, can delete");
			/*
				join before deleting...
				http://stackoverflow.com/questions/25397874/deleting-stdthread-pointer-raises-exception-libcabi-dylib-terminating
			*/
			image_capture.thread->join();
			delete image_capture.thread;
			image_capture.thread = nullptr;
		}
	}
}
void Camera::staticEntryImageCollectAndSave(ImageCapture& ic)
{
	ic.cam->messenger.printDebugMessage("staticEntryImageCollectAndSave running");
	ic.cam->epicsInterface->attachTo_thisCaContext();
	ic.cam->imageCaptureAndSave(ic.num_shots);
	ic.cam->epicsInterface->detachFrom_thisCaContext();
	ic.cam->messenger.printDebugMessage("staticEntryImageCollectAndSave complete");
}
void Camera::imageCaptureAndSave(size_t num_shots)
{
	messenger.printDebugMessage(hardwareName, " imageCaptureAndSave called");
	bool timed_out = false;
	if (setNumberOfShotsToCapture(num_shots))
	{
		messenger.printDebugMessage(hardwareName, " Set number of shots success");
		if (capture())
		{
			messenger.printDebugMessage("imageCollectAndSave is waiting for collection to finish");
			GlobalFunctions::pause_500();
			// add a time out here
			time_t wait_time = num_shots * 10 + 5; // MAGIC numbers 
			time_t time_start = GlobalFunctions::timeNow();
			while (isCapturing())   //wait until collecting is done...
			{
				GlobalFunctions::pause_50(); //MAGIC_NUMBER
				messenger.printDebugMessage(hardwareName, " isCapturing is TRUE");
				/* check if time ran out */
				if (GlobalFunctions::timeNow() - time_start > wait_time)
				{
					timed_out = true;
				}
			}
			if (timed_out)
			{
				messenger.printDebugMessage(hardwareName," timed out during capture");
				image_capture.status = STATE::TIMEOUT;
				image_capture.is_busy = false;
			}
			else
			{
				if (makeANewDirectoryAndName(num_shots))
				{
					GlobalFunctions::pause_500();
					GlobalFunctions::pause_500();
					messenger.printDebugMessage("imageCollectAndSave ", hardwareName, " is going to write collected data");
					if (write())
					{
						//message("imageCollectAndSave ", hardwareName, " is waiting for writing to finish");
						while (isWriting())   //wait until saving is done...
						{
							GlobalFunctions::pause_50(); //MAGIC_NUMBER
						}
						messenger.printDebugMessage("imageCollectAndSave ", hardwareName, " writing has finished");
						// pause and wait for EPICS to UPDATE
						GlobalFunctions::pause_500();
						GlobalFunctions::pause_500();
						///check status of save/write
						if (write_state_check.second == STATE::WRITE_CHECK_OK)
						{
							last_capture_and_save_success = true;//this->message("Successful wrote image to disk.");
							image_capture.status = STATE::SUCCESS;
						}
						else
						{
							messenger.printDebugMessage("!!SAVE ERROR!!: camera = ", hardwareName, ", error message =  ", write_error_message.second);
							last_capture_and_save_success = false;
							image_capture.status = STATE::WRITE_CHECK_ERROR;
						}
						busy = false;
						image_capture.is_busy = false;
					}
					else
					{
						messenger.printDebugMessage(hardwareName, " write returned false.");
					}
				}
				else
				{
					messenger.printDebugMessage(hardwareName, " makeANewDirectoryAndName returned false.");
				}
			}

		}
	}
	else
	{
		messenger.printDebugMessage(hardwareName, " setNumberOfShotsToCapture returned false."); 
	}

}
//-------------------------------------------------------------------------------------------------------
bool Camera::setNumberOfShotsToCapture(size_t num)
{
	if (num <= max_shots_number)
	{
		return epicsInterface->putValue2<int>(pvStructs.at(CameraRecords::HDF_Capture), (int)num);
	}
	return false;
}
bool Camera::capture()
{
	bool ans = false;
	if (isAcquiring())
	{
		ans = epicsInterface->putValue2<unsigned short>(pvStructs.at(CameraRecords::HDF_Capture), GlobalConstants::one_ushort);
		messenger.printDebugMessage("Capture set to 1 on camera ", hardwareName);
	}
	else
	{
		messenger.printDebugMessage(hardwareName, " is not acquiring");
	}
	return ans;
}
bool Camera::write()
{
	bool ans = false;
	int startNumber(1);// MAGIC_NUMBER should this be a one or a zero?
	// WHAT IS THIS DOING????
	//setStartFileNumberJPG(startNumber);
	if (isNotCapturing())
	{
		ans = epicsInterface->putValue2<unsigned short>(pvStructs.at(CameraRecords::HDF_WriteFile), GlobalConstants::one_ushort);
		messenger.printDebugMessage("WriteFile set to 1 on camera = ", hardwareName);
	}
	else
	{
		messenger.printDebugMessage(hardwareName, " still collecting images when write function was called.");
	}
	return ans;
}
//-------------------------------------------------------------------------------------------------------
bool Camera::makeANewDirectoryAndName(size_t numbOfShots)///YUCK (make it look nice)
{
	bool ans = false;
	
	messenger.printDebugMessage("char: ", sizeof(char));
	messenger.printDebugMessage("char: ", sizeof(char));
	messenger.printDebugMessage("char: ", sizeof(char));
	

	//std::string time_now = GlobalFunctions::time_iso_8601();
	
	// this sets up the directory structure, based on year, month date
	std::chrono::system_clock::time_point p = std::chrono::system_clock::now();
	std::time_t t = std::chrono::system_clock::to_time_t(p);
	tm local_tm = *localtime(&t);
	//struct tm local_tm;
	localtime_s(&local_tm, &t);
	char newPath[256] = "/CameraImages/";
	std::string strPath = "/CameraImages/" +
		std::to_string(local_tm.tm_year + 1900) +
		"/" + std::to_string(local_tm.tm_mon + 1) +
		"/" + std::to_string(local_tm.tm_mday) + "/";
	strcpy(newPath, strPath.c_str());

	char newName[256] = "defaultname";
	std::string strName = hardwareName + "_" + GlobalFunctions::time_iso_8601() + "_" + std::to_string(numbOfShots) + "_images";
	strName = GlobalFunctions::replaceStrChar(strName, ":", '-');
	strcpy(newName, strName.c_str()); 
	messenger.printDebugMessage("File Directory would be: ",newPath);
	messenger.printDebugMessage("File name is: ", newName);

	std::stringstream s;
	for (auto&& t: newPath)
	{
		s << static_cast<unsigned>(t);
		s << " ";
	}
	messenger.printDebugMessage("t = ", s.str());
	s.clear();
	for (auto&& t : newPath)
	{
		s << static_cast<signed>(t);
		s << " ";
	}
	messenger.printDebugMessage("t = ", s.str());

	//auto  name_pvs = &pvStructs.at(CameraRecords::HDF_FileName);
	//auto  path_pvs = &pvStructs.at(CameraRecords::HDF_FilePath);

	if (ca_state(pvStructs.at(CameraRecords::HDF_FileName).CHID) == cs_conn)
	{
		int status = ca_array_put(DBR_CHAR, 256, pvStructs.at(CameraRecords::HDF_FileName).CHID, newName);
		MY_SEVCHK(status);
		status = ca_pend_io(CA_PEND_IO_TIMEOUT);
		MY_SEVCHK(status);

		if (status == ECA_NORMAL)
		{

			if (ca_state(pvStructs.at(CameraRecords::HDF_FilePath).CHID) == cs_conn)
			{
				int status = ca_array_put(DBR_CHAR, 256, pvStructs.at(CameraRecords::HDF_FilePath).CHID, newPath);
				MY_SEVCHK(status);
				status = ca_pend_io(CA_PEND_IO_TIMEOUT);
				MY_SEVCHK(status);
				if (status == ECA_NORMAL)
				{
					return true;
				}
			}
			else
			{
				messenger.printDebugMessage(hardwareName, " HDF_FilePath is not connected");
			}
		}
		else
		{
			messenger.printDebugMessage(hardwareName, " send file name failed status = ", status);
		}


	}
	else
	{ 
		messenger.printDebugMessage(hardwareName, " HDF_FileName is not connected");
	}



	//ans = epicsInterface->putArrayValue<char*>(pvStructs.at(CameraRecords::HDF_FileName), newName);
	//if (ans)
	//{
	//	ans = epicsInterface->putArrayValue<char*>(pvStructs.at(CameraRecords::HDF_FilePath), newPath);
	//	if (ans)
	//	{
	//		requested_directory = newPath;
	//		requested_filename = newName;
	//	}
	//	else
	//	{
	//		messenger.printDebugMessage(hardwareName, " Failed to send new filepath");
	//	}
	//}
	//else
	//{
	//	messenger.printDebugMessage(hardwareName, " Failed to send new filename");
	//}


	//}
	//else
	//{
	//	messenger.printDebugMessage(hardwareName," Failed to send new filename");
	//}

	//putArrayValue(camera.pvComStructs.at(CAM_PV_TYPE::CAM_FILE_NAME).CHTYPE,
	//	camera.pvComStructs.at(CAM_PV_TYPE::CAM_FILE_NAME).COUNT,
	//	camera.pvComStructs.at(CAM_PV_TYPE::CAM_FILE_NAME).CHID,
	//	&newName);
	//ca_array_put(camera.pvComStructs.at(CAM_PV_TYPE::CAM_FILE_PATH).CHTYPE,
	//	camera.pvComStructs.at(CAM_PV_TYPE::CAM_FILE_PATH).COUNT,
	//	camera.pvComStructs.at(CAM_PV_TYPE::CAM_FILE_PATH).CHID,
	//	&newPath);

	//int status = sendToEpics("ca_put", "", "Timeout trying to send new file path state.");
	//if (status == ECA_NORMAL)
	//{
	//	ans = true;
	//	camera.daq.latestDirectory = newPath;
	//	camera.daq.latestFilename = newName;

	//	allCamData.at(camera.name).daq.latestDirectory = newPath;
	//	allCamData.at(camera.name).daq.latestFilename = newName;

	//message("New latestDirectory set to ", latest_directory, " on ", camera.name, " camera.");
	//message("New latestFilename set to  ", latest_filename, " on ", camera.name, " camera.");
	//}

	//return true;
	return ans;
}
bool Camera::isWriting()const
{
	return write_state.second == STATE::WRITING;
}
bool Camera::isNotWriting()const
{
	return write_state.second == STATE::NOT_WRITING;
}
STATE Camera::getCaptureState()const
{
	return capture_state.second;
}
//---------------------------------------------------------------------------------
bool Camera::isCapturing()const
{
	if (getCaptureState() == STATE::CAPTURING)
	{
		messenger.printDebugMessage("isCapturing ", hardwareName, " is TRUE");
	}
	else
	{
		messenger.printDebugMessage("isCapturing ", hardwareName, " is FALSE");
	}
	return getCaptureState() == STATE::CAPTURING;
}
bool Camera::isNotCapturing()const
{
	if (getCaptureState() == STATE::NOT_CAPTURING)
	{
		messenger.printDebugMessage("isNotCapturing ", hardwareName, " is TRUE");
	}
	else
	{
		messenger.printDebugMessage("isNotCapturing ", hardwareName, " is FALSE");
	}
	return getCaptureState() == STATE::NOT_CAPTURING;
}
bool Camera::isCapturingOrSaving()const
{
	if(isCapturing())
	{
		return true;
	}
	if (isWriting())
	{
		return true;
	}
	return false;
}
std::string Camera::getLastDirectoryandFileName() const
{
	return getLastDirectory() + "/" + getLastFileName(); // WARNING!! TODO unix / windows conventions :(( 
}
std::string Camera::getLastDirectory()const
{
	return save_filepath.second;
}
std::string Camera::getLastFileName()const
{
	return save_filename.second;
}
std::string Camera::getLastBufferDirectoryandFileName() const
{
	return getLastBufferDirectory() + "/" + getLastBufferFileName();
}
std::string Camera::getLastBufferDirectory()const
{
	return buffer_filepath.second;
}
std::string Camera::getLastBufferFileName()const
{
	return buffer_filename.second;
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
char Camera::getBufferTrigger()const
{
	return buffer_trigger.second;
}
std::string Camera::getBufferFilePath()const
{
	return buffer_filepath.second;
}
std::string Camera::getBufferFileName()const
{
	return buffer_filename.second;
}
long Camera::getBufferFileNumber()const
{
	return buffer_filenumber.second;
}
//long Camera::getBufferROIminX()const
//{
//	return roi_min_x.second;
//}
//long Camera::getBufferROIminY()const
//{
//	return roi_min_y.second;
//}
//long Camera::getBufferROIsizeX()const
//{
//	return roi_size_x.second;
//}
//long Camera::getBufferROIsizeY()const
//{
//	return roi_size_y.second;
//}
STATE Camera::getUseFloorState()const
{
	return use_floor.second;
}
bool Camera::isUsingFloor()const
{
	return use_floor.second == STATE::USING_FLOOR;
}
bool Camera::isNotUsingFloor()const
{
	return use_floor.second == STATE::NOT_USING_FLOOR;
}
bool Camera::setUseFloor()
{
	if (mode == STATE::PHYSICAL)
	{
		//messenger.printDebugMessage("Send ANA_UseFloor 1");
		if (GlobalFunctions::entryExists(pvStructs, CameraRecords::ANA_UseFloor))
		{
			//messenger.printDebugMessage("PV FOUND");
			return epicsInterface->putValue2<unsigned short>(pvStructs.at(CameraRecords::ANA_UseFloor), GlobalConstants::one_ushort);
		}
		//messenger.printDebugMessage("ERROR ANA_UseFloor PV not found");
		//return epicsInterface->putValue2<unsigned short>(pvStructs.at(CameraRecords::ANA_UseFloor), GlobalConstants::one_ushort);
	}
	return false;
}
bool Camera::setDoNotUseFloor()
{
	if (mode == STATE::PHYSICAL)
	{
		messenger.printDebugMessage("Send ANA_UseFloor 0");
		if (GlobalFunctions::entryExists(pvStructs, CameraRecords::ANA_UseFloor))
		{
			messenger.printDebugMessage("PV FOUND");
			return epicsInterface->putValue2<unsigned short>(pvStructs.at(CameraRecords::ANA_UseFloor), GlobalConstants::zero_ushort);
		}
		messenger.printDebugMessage("ERROR ANA_UseFloor PV not found");

	}
	return false;
}
bool Camera::setFloorLevel(long v)
{
	if (mode == STATE::PHYSICAL)
	{
		return epicsInterface->putValue2<long>(pvStructs.at(CameraRecords::ANA_FloorLevel), v);
	}
	return false;
}
double Camera::getAcquireTime()const
{
	return acquire_time.second;
}
double Camera::getAcquirePeriod()const
{
	return acquire_period.second;
}
double Camera::getArrayRate()const
{
	return array_rate.second;
}
double Camera::getTemperature()const
{
	return temperature.second;
}
long Camera::getFloorLevel()const
{
	return floor_level.second;
}
long Camera::getFlooredPtsCount()const
{
	return floored_pts_count.second;
}
double Camera::getFlooredPtsPercent()const
{
	return floored_pts_percent.second;
}
long Camera::getCPUTotal()const
{
	return cpu_total.second;
}
long Camera::getCPUCropSubMask()const
{
	return cpu_crop_sub_mask.second;
}
long Camera::getCPUNPoint()const
{
	return cpu_npoint.second;
}
long Camera::getCPUDot()const
{
	return cpu_dot.second;
}
long Camera::getPixelWidth()const
{
	return pixel_width.second;
}
long Camera::getPixelHeight()const
{
	return pixel_height.second;
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
/*
		Getting image data:

		What I want to achieve: 

		Get camera image data without any extra copying 
		Have the data in a container that can (easily) be seen in python without copying
		Get time stamped data 

		It seems out that acheiving all 3 at once is not obvious 
		Proposed solution (which is not perfect and has flaws) 

		Function to get time stamp (fast as possible)
		Function to update data into a vector 
		Vector exposed to python for access without copying 
		Function to get data will copy data into a python list 
*/
/* memory shenanigans so its only used for large image arrays when requested */ 
bool Camera::vector_resize(std::vector<long>& vec)
{
	vec.resize(num_pixels);
	return vec.size() == num_pixels;
}
void Camera::malloc_imagedata()
{
	/*
		allocate memory for pointer_to_array_data, array pointer
	*/
	unsigned nBytes = dbr_size_n(DBR_TIME_LONG, num_pixels);
	//std::cout << sizeof(*dbr_image_data) << std::endl;
	dbr_image_data = (struct dbr_time_long*)malloc(nBytes);
	//std::cout << sizeof(*dbr_image_data) << std::endl;
	messenger.printDebugMessage(hardwareName, " dbr_image_data pointer allocated ", nBytes, " BYTES ");
	image_data_has_not_malloced = false;
}
void Camera::malloc_roidata()
{
	/*
		allocate memory for pointer_to_array_data, array pointer
	*/
	unsigned nBytes = dbr_size_n(DBR_TIME_LONG, num_pixels);
	//std::cout << sizeof(*dbr_roi_data) << std::endl;
	dbr_roi_data = (struct dbr_time_long*)malloc(nBytes);
	//std::cout << sizeof(*dbr_roi_data) << std::endl;
	messenger.printDebugMessage(hardwareName, " dbr_roi_data pointer allocated ", nBytes, " BYTES ");
	roi_data_has_not_malloced = false;
}
bool Camera::updateImageData()
{
	if (image_data_has_not_vector_resized)
	{
		messenger.printDebugMessage("vector_resize for image_data ");
		image_data_has_not_vector_resized = vector_resize(image_data.second);
	}
	if (!image_data_has_not_vector_resized)
	{
		auto start = std::chrono::high_resolution_clock::now();
		//bool got_stamp = getArrayTimeStamp(dbr_image_data, pvStructs.at(CameraRecords::CAM2_ArrayData)
		//	, image_data.first);
		bool got_value = getArrayValue(image_data.second, pvStructs.at(CameraRecords::CAM2_ArrayData)
			, image_data.second.size());
		auto stop = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
		messenger.printDebugMessage("updateImageData Time taken: ", duration.count(), " us");
		return got_value;
	}
	else {
		messenger.printDebugMessage("!!ERROR!! image_data_has_not_vector_resized = ", image_data_has_not_vector_resized);
	}
	return false;
}
bool Camera::updateImageDataWithTimeStamp()
{
	if (image_data_has_not_malloced)
	{
		messenger.printDebugMessage("calling malloc_imagedata");
		malloc_imagedata();
	}
	if (image_data_has_not_vector_resized)
	{
		messenger.printDebugMessage("vector_resize for image_data ");
		image_data_has_not_vector_resized = !vector_resize(image_data.second);
	}
	if (!image_data_has_not_vector_resized && !image_data_has_not_malloced)
	{
		auto start = std::chrono::high_resolution_clock::now();
		bool got_stamp = getArrayTimeStamp(dbr_image_data, pvStructs.at(CameraRecords::CAM2_ArrayData)
			, image_data.first);
		bool got_value = getArrayValue(image_data.second, pvStructs.at(CameraRecords::CAM2_ArrayData)
			, image_data.second.size());
		auto stop = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
		messenger.printDebugMessage("updateImageDataWithTimeStamp Time taken: ", duration.count(), " us");
		return got_stamp && got_value;
	}
	else {
		messenger.printDebugMessage("!!ERROR!! image_data_has_not_vector_resized = ", image_data_has_not_vector_resized);
		messenger.printDebugMessage("!!ERROR!! image_data_has_not_malloced = ", image_data_has_not_malloced);
	}

	return false;
}
bool Camera::updateROIData()
{
	if (roi_data_has_not_vector_resized)
	{
		messenger.printDebugMessage("vector_resize for roi_data ");
		roi_data_has_not_vector_resized = !vector_resize(roi_data.second);
	}
	auto start = std::chrono::high_resolution_clock::now();
	//bool got_stamp = getArrayTimeStamp(dbr_image_data, pvStructs.at(CameraRecords::CAM2_ArrayData)
	//	, image_data.first);
	bool got_value = getArrayValue(roi_data.second, pvStructs.at(CameraRecords::ROI1_ImageData_RBV)
		, roi_data.second.size());
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
	messenger.printDebugMessage("updateROIData Time taken: ", duration.count(), " us");
	return got_value;
}
bool Camera::updateROIDataWithTimeStamp()
{
	if (roi_data_has_not_malloced)
	{
		messenger.printDebugMessage("calling malloc_roidata");
		malloc_roidata();
	}
	if (roi_data_has_not_vector_resized)
	{
		messenger.printDebugMessage("vector_resize for roi_data ");
		roi_data_has_not_vector_resized = !vector_resize(roi_data.second);
	}
	if (!roi_data_has_not_vector_resized && !roi_data_has_not_malloced)
	{
		auto start = std::chrono::high_resolution_clock::now();
		bool got_stamp = getArrayTimeStamp(dbr_roi_data, pvStructs.at(CameraRecords::ROI1_ImageData_RBV)
			, roi_data.first);
		long roi_num_pixels = getROIMinY() * getROIMinX();
		bool got_value = getArrayValue(roi_data.second, pvStructs.at(CameraRecords::ROI1_ImageData_RBV)
			, roi_num_pixels);
		auto stop = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
		messenger.printDebugMessage("updateROIDataWithTimeStamp Time taken: ", duration.count(), " us");
		return got_stamp && got_value;
	}
	return false;
}
bool Camera::getArrayTimeStamp(struct dbr_time_long* dbr_struct, const pvStruct& pvs,
	epicsTimeStamp& ts_to_update
	)
{
	auto start = std::chrono::high_resolution_clock::now();
	if (ca_state(pvs.CHID) == cs_conn)
	{
		int status = ca_array_get(DBR_TIME_LONG,1,pvs.CHID,dbr_struct);
		EPICSInterface::sendToEPICS();
		MY_SEVCHK(status);
		ts_to_update = dbr_struct->stamp;
		//std::cout << epicsInterface->getEPICSTime(ts_to_update) << std::endl;
		auto stop  = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
		messenger.printDebugMessage("getArrayTimeStamp Time taken: ", duration.count(), " us");
		std::cout << dbr_struct->value << std::endl;
		return true;
	}
	return false;
}




bool Camera::getArrayValue(std::vector<long>& data_vec, const pvStruct & pvs,size_t count)
{
	auto start = std::chrono::high_resolution_clock::now();
	if (ca_state(pvs.CHID) == cs_conn)
	{
		int status = ca_array_get(DBR_LONG, count, pvs.CHID, &data_vec[0]);
		EPICSInterface::sendToEPICS();
		MY_SEVCHK(status);
		auto stop = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
		messenger.printDebugMessage("getArrayValue Time taken: ", duration.count(), " us");
		return true;
	}
	return false;
}





std::vector<long> Camera::getImageData()
{
	return image_data.second;
}
boost::python::list Camera::getImageData_Py()
{
	return to_py_list<long>(getImageData());
}
std::vector<long> Camera::getROIData()
{
	return roi_data.second;
}
boost::python::list Camera::getROIData_Py()
{
	return to_py_list<long>(getROIData());
}

std::vector<long>& Camera::getImageDataConstRef()
{
	return image_data.second;
}
std::vector<long>& Camera::getROIDataConstRef()
{
	return roi_data.second;
}






