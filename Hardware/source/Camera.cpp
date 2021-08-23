#include "Camera.h"
#include "Camera.h"
#include "GlobalFunctions.h"
#include "CameraPVRecords.h"
#include "PythonTypeConversions.h"
#include "boost/algorithm/string/split.hpp"
#include <algorithm>
#include <chrono> 
#include <time.h> 
#include <mutex>          // std::mutex
std::mutex mtx;           // mutex for critical section
#include "boost/algorithm/string/split.hpp"
#include <boost/make_shared.hpp>
#include <boost/range/combine.hpp>
#include "GlobalConstants.h"


Camera::Camera():
Hardware()
{
}

Camera::Camera(const std::map<std::string, std::string>& paramMap, STATE mode) :
	Hardware(paramMap, mode),
	//pointer_to_array_data(nullptr),
	epicsInterface(boost::make_shared<EPICSCameraInterface>(EPICSCameraInterface())), // calls copy constructor and destroys 
	pix2mmX_ratio(GlobalConstants::double_min),  // MAGIC STRING
	pix2mmY_ratio(GlobalConstants::double_min),  // MAGIC STRING
	max_shots_number(GlobalConstants::size_zero),  // MAGIC STRING

	pixel_to_mm(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),

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
	active_camera_limit(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
	active_camera_count(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
	led_state(std::make_pair(epicsTimeStamp(), STATE::UNKNOWN)),
	acquire_state(std::make_pair(epicsTimeStamp(), STATE::UNKNOWN)),
	analysis_state(std::make_pair(epicsTimeStamp(), STATE::UNKNOWN)),
	mask_x_center(std::make_pair(epicsTimeStamp(), GlobalConstants::long_min)),
	mask_y_center(std::make_pair(epicsTimeStamp(), GlobalConstants::long_min)),
	mask_x_radius(std::make_pair(epicsTimeStamp(), GlobalConstants::long_min)),
	mask_y_radius(std::make_pair(epicsTimeStamp(), GlobalConstants::long_min)),
	pixelResults(std::make_pair(epicsTimeStamp(), std::vector<double>{GlobalConstants::double_min, //X_POS
																	  GlobalConstants::double_min, //Y_POS
																	  GlobalConstants::double_min, //X_SIGMA
																	  GlobalConstants::double_min, //Y_SIGMA
																	  GlobalConstants::double_min  //COV
																	 })),
	mmResults(std::make_pair(epicsTimeStamp(), std::vector<double>{GlobalConstants::double_min, //X_POS
																	  GlobalConstants::double_min, //Y_POS
																	  GlobalConstants::double_min, //X_SIGMA
																	  GlobalConstants::double_min, //Y_SIGMA
																	  GlobalConstants::double_min  //COV
																	 })),

	master_lattice_pixel_to_mm(GlobalConstants::double_min),
	master_lattice_centre_x(GlobalConstants::long_min),
	master_lattice_centre_y(GlobalConstants::long_min),

	lastResultsUpdateTime(epicsTimeStamp()),
	lastResultsUpdateTime_mm_ana_results(epicsTimeStamp()),
	isResultUpdated(false),
	x_center_pixel(std::make_pair(epicsTimeStamp(), GlobalConstants::long_min)),
	y_center_pixel(std::make_pair(epicsTimeStamp(), GlobalConstants::long_min)),
	
	
	operating_centre_x(GlobalConstants::long_min),
	operating_centre_y(GlobalConstants::long_min),
	mechanical_centre_x(GlobalConstants::long_min),
	mechanical_centre_y(GlobalConstants::long_min),
	x_center_def(GlobalConstants::size_zero),
	y_center_def(GlobalConstants::size_zero),


	dbr_image_data(nullptr),
	dbr_roi_data(nullptr),

	image_data(std::make_pair(epicsTimeStamp(), std::vector<long>())),
	roi_data(std::make_pair(epicsTimeStamp(), std::vector<long>())),
	analysis_data(std::make_pair(epicsTimeStamp(), std::vector<double>())),
	step_size(std::make_pair(epicsTimeStamp(), GlobalConstants::long_min)),
	acquire_time(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
	acquire_period(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
	temperature(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
	array_rate(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
	use_npoint(std::make_pair(epicsTimeStamp(), STATE::UNKNOWN)),
	use_background(std::make_pair(epicsTimeStamp(), STATE::UNKNOWN)),
	cross_hair_overlay(std::make_pair(epicsTimeStamp(), STATE::UNKNOWN)),
	analysis_result_overlay(std::make_pair(epicsTimeStamp(), STATE::UNKNOWN)),
	analysis_mask_overlay(std::make_pair(epicsTimeStamp(), STATE::UNKNOWN)),
	black_level(std::make_pair(epicsTimeStamp(), GlobalConstants::long_min)),
	gain(std::make_pair(epicsTimeStamp(), GlobalConstants::long_min)),
	set_new_background(std::make_pair(epicsTimeStamp(), STATE::UNKNOWN)),
	cam_type(TYPE::UNKNOWN_TYPE),
	mask_and_roi_keywords({ "roi_x", "roi_y", "x_rad", "y_rad" }),  //MAGIC STRING TOD better place for these ??? 
	mask_keywords({ "mask_x", "mask_y", "mask_rad_x", "mask_rad_y" }),//MAGIC STRING 
	//roi_keywords({ "roi_x", "roi_y", "roi_rad_x", "roi_rad_y" }),     //MAGIC STRING
	image_data_has_not_malloced(true),
	image_data_has_not_vector_resized(true),
	roi_data_has_not_malloced(true),
	roi_data_has_not_vector_resized(true),
	array_data_num_pix_x(GlobalConstants::size_zero),
	array_data_num_pix_y(GlobalConstants::size_zero),
	array_data_pixel_count(GlobalConstants::size_zero),
	roi_total_pixel_count(GlobalConstants::size_zero),
	binary_num_pix_x(GlobalConstants::size_zero),
	binary_num_pix_y(GlobalConstants::size_zero),
	binary_data_pixel_count(GlobalConstants::size_zero),
	x_pix_scale_factor(GlobalConstants::size_zero),
	y_pix_scale_factor(GlobalConstants::size_zero),
	x_mask_def(GlobalConstants::size_zero),
	y_mask_def(GlobalConstants::size_zero),
	x_mask_rad_def(GlobalConstants::size_zero),
	y_mask_rad_def(GlobalConstants::size_zero),
	x_mask_rad_max(GlobalConstants::size_zero),
	y_mask_rad_max(GlobalConstants::size_zero),
	use_mask_rad_limits(false),
	sensor_max_temperature(GlobalConstants::double_min),
	sensor_min_temperature(GlobalConstants::double_min),
	average_pixel_value_for_beam(GlobalConstants::double_min),
	min_x_pixel_pos(GlobalConstants::double_min),
	max_x_pixel_pos(GlobalConstants::double_min),
	min_y_pixel_pos(GlobalConstants::double_min),
	max_y_pixel_pos(GlobalConstants::double_min),
	busy(false),
	has_led(false),
	last_capture_and_save_success(false),
	x_pix_rs(RunningStats() ),
	y_pix_rs(RunningStats() ),
	sigma_x_pix_rs(RunningStats() ),
	sigma_y_pix_rs(RunningStats() ),
	sigma_xy_pix_rs(RunningStats() ),
	x_mm_rs(RunningStats() ),
	y_mm_rs(RunningStats() ),
	sigma_x_mm_rs(RunningStats() ),
	sigma_y_mm_rs(RunningStats() ),
	sigma_xy_mm_rs(RunningStats() ),
	avg_intensity_rs(RunningStats() ),
	sum_intensity_rs(RunningStats() )
{
	messenger.printDebugMessage(hardwareName, " printing paramMap:" );
	for (auto&& item : paramMap)
	{
		messenger.printDebugMessage(item.first, " = ", item.second);
	}
	getMasterLatticeData(paramMap, mode);
	setPVStructs();
	//mask_and_roi_keywords_Py = to_py_list(mask_and_roi_keywords);
	//mask_keywords_Py = to_py_list(mask_keywords);
	//roi_keywords_Py = to_py_list(roi_keywords);
}

void Camera::getMasterLatticeData(const std::map<std::string, std::string>& paramMap, STATE mode)
{
	messenger.printDebugMessage("getMasterLatticeData");
	messenger.printDebugMessage(hardwareName, " find ARRAY_DATA_X_PIX_2_MM");
	if (GlobalFunctions::entryExists(paramMap, "ARRAY_DATA_X_PIX_2_MM"))
	{
		pix2mmX_ratio = std::stof(paramMap.find("ARRAY_DATA_X_PIX_2_MM")->second); 	
	}
	else
	{
		messenger.printDebugMessage(hardwareName, " !!WARNING!! could not find ARRAY_DATA_X_PIX_2_MM");
	}
	//-------------------------------------------------------------------------------------------------
	messenger.printDebugMessage(hardwareName, " find ARRAY_DATA_X_PIX_2_MM");
	if (GlobalFunctions::entryExists(paramMap, "ARRAY_DATA_X_PIX_2_MM"))
	{
		pix2mmY_ratio = std::stof(paramMap.find("ARRAY_DATA_Y_PIX_2_MM")->second);
	}
	else
	{
		messenger.printDebugMessage(hardwareName, " !!WARNING!! could not find ARRAY_DATA_Y_PIX_2_MM");
	}
	//-------------------------------------------------------------------------------------------------
	messenger.printDebugMessage(hardwareName, " find MAX_SHOTS_NUMBER");
	if (GlobalFunctions::entryExists(paramMap, "MAX_SHOTS_NUMBER"))
	{
		max_shots_number = (size_t)std::stof(paramMap.find("MAX_SHOTS_NUMBER")->second);
	}
	else
	{
		messenger.printDebugMessage(hardwareName, " !!WARNING!! could not find MAX_SHOTS_NUMBER");
	}
	//-------------------------------------------------------------------------------------------------
	messenger.printDebugMessage(hardwareName, " find name_alias");
	if (GlobalFunctions::entryExists(paramMap, "name_alias"))
	{
		boost::split(aliases, paramMap.find("name_alias")->second, [](char c) {return c == ','; });
		for (auto& name : aliases)
		{
			name.erase(std::remove_if(name.begin(), name.end(), isspace), name.end());
			messenger.printDebugMessage(hardwareName, " added aliase " + name);
		}
	}
	else{ messenger.printDebugMessage(hardwareName, " !!WARNING!!");}
	//-------------------------------------------------------------------------------------------------
	messenger.printDebugMessage(hardwareName, " find SCREEN_NAME");
	if (GlobalFunctions::entryExists(paramMap, "SCREEN_NAME"))
	{
		boost::split(screen_names, paramMap.find("SCREEN_NAME")->second, [](char c) {return c == ','; });
		for (auto& name : screen_names)
		{
			name.erase(std::remove_if(name.begin(), name.end(), isspace), name.end());
			messenger.printDebugMessage(hardwareName, " added screen_name " + name);
		}
	}
	else { messenger.printDebugMessage(hardwareName, " !!WARNING!!"); }
	//-------------------------------------------------------------------------------------------------
	messenger.printDebugMessage(hardwareName, " find CAM_TYPE");
	if (GlobalFunctions::entryExists(paramMap, "CAM_TYPE"))
	{
		if (GlobalFunctions::entryExists(GlobalConstants::stringToTypeMap, paramMap.at("CAM_TYPE")))
		{
			cam_type = GlobalConstants::stringToTypeMap.at(paramMap.at("CAM_TYPE"));
		}
	}
	else { messenger.printDebugMessage(hardwareName, " !!WARNING!!"); }
	//-------------------------------------------------------------------------------------------------
	messenger.printDebugMessage(hardwareName, " find HAS_LED");
	if (GlobalFunctions::entryExists(paramMap, "HAS_LED"))
	{
		if (std::string(paramMap.find("HAS_LED")->second) == GlobalConstants::TRUE_STR)
		{
			has_led = true;
		}
		if (std::string(paramMap.find("HAS_LED")->second) == GlobalConstants::FALSE_STR)
		{
			has_led = false;
		}
	}
	else { messenger.printDebugMessage(hardwareName, " !!WARNING!!"); }
	//-------------------------------------------------------------------------------------------------
	messenger.printDebugMessage(hardwareName, " find ARRAY_DATA_NUM_PIX_X");
	if (GlobalFunctions::entryExists(paramMap, "ARRAY_DATA_NUM_PIX_X"))
	{
		array_data_num_pix_x = (size_t)std::stof(paramMap.find("ARRAY_DATA_NUM_PIX_X")->second);
	}
	else
	{
		messenger.printDebugMessage(hardwareName, " !!WARNING!! could not find ARRAY_DATA_NUM_PIX_X");
	}

	messenger.printDebugMessage(hardwareName, " find ARRAY_DATA_NUM_PIX_Y");
	if (GlobalFunctions::entryExists(paramMap, "ARRAY_DATA_NUM_PIX_Y"))
	{
		array_data_num_pix_y = (size_t)std::stof(paramMap.find("ARRAY_DATA_NUM_PIX_Y")->second);
	}
	else
	{
		messenger.printDebugMessage(hardwareName, " !!WARNING!! could not find ARRAY_DATA_NUM_PIX_Y");
	}
	array_data_pixel_count = array_data_num_pix_y * array_data_num_pix_x;

	messenger.printDebugMessage(hardwareName, " array_data_pixel_count = ", array_data_pixel_count);
	//-------------------------------------------------------------------------------------------------
	messenger.printDebugMessage(hardwareName, " find BINARY_NUM_PIX_X");
	if (GlobalFunctions::entryExists(paramMap, "BINARY_NUM_PIX_X"))
	{
		binary_num_pix_x = (size_t)std::stof(paramMap.find("BINARY_NUM_PIX_X")->second);
	}
	else
	{
		messenger.printDebugMessage(hardwareName, " !!WARNING!! could not find BINARY_NUM_PIX_X");
	}
	messenger.printDebugMessage(hardwareName, " find BINARY_NUM_PIX_X");
	if (GlobalFunctions::entryExists(paramMap, "BINARY_NUM_PIX_X"))
	{
		binary_num_pix_y = (size_t)std::stof(paramMap.find("BINARY_NUM_PIX_Y")->second);
	}
	else
	{
		messenger.printDebugMessage(hardwareName, " !!WARNING!! could not find BINARY_NUM_PIX_Y");
	}
	binary_data_pixel_count = binary_num_pix_x * binary_num_pix_y;
	//-------------------------------------------------------------------------------------------------
	messenger.printDebugMessage(hardwareName, " find X_PIX_SCALE_FACTOR");
	if (GlobalFunctions::entryExists(paramMap, "X_PIX_SCALE_FACTOR"))
	{
		x_pix_scale_factor = (size_t)std::stof(paramMap.find("X_PIX_SCALE_FACTOR")->second);
	}
	else
	{
		messenger.printDebugMessage(hardwareName, " !!WARNING!! could not find Y_PIX_SCALE_FACTOR");
	}
	//-------------------------------------------------------------------------------------------------
	messenger.printDebugMessage(hardwareName, " find Y_PIX_SCALE_FACTOR");
	if (GlobalFunctions::entryExists(paramMap, "Y_PIX_SCALE_FACTOR"))
	{
		y_pix_scale_factor = (size_t)std::stof(paramMap.find("Y_PIX_SCALE_FACTOR")->second);
	}
	else
	{
		messenger.printDebugMessage(hardwareName, " !!WARNING!! could not find Y_PIX_SCALE_FACTOR");
	}
	//-------------------------------------------------------------------------------------------------
	messenger.printDebugMessage(hardwareName, " find AVG_PIXEL_VALUE_FOR_BEAM");
	if (GlobalFunctions::entryExists(paramMap, "AVG_PIXEL_VALUE_FOR_BEAM"))
	{
		average_pixel_value_for_beam = std::stod(paramMap.find("AVG_PIXEL_VALUE_FOR_BEAM")->second.data());
	}
	else
	{
		messenger.printDebugMessage(hardwareName, " !!WARNING!! could not find AVG_PIXEL_VALUE_FOR_BEAM");
	}
	//-------------------------------------------------------------------------------------------------
	messenger.printDebugMessage(hardwareName, " find SENSOR_MAX_TEMP");
	if (GlobalFunctions::entryExists(paramMap, "SENSOR_MAX_TEMP"))
	{
		sensor_max_temperature = std::stof(paramMap.find("SENSOR_MAX_TEMP")->second);
	}
	else
	{
		messenger.printDebugMessage(hardwareName, " !!WARNING!! could not find SENSOR_MAX_TEMP");
	}	
	//-------------------------------------------------------------------------------------------------
	messenger.printDebugMessage(hardwareName, " find SENSOR_MIN_TEMP");
	if (GlobalFunctions::entryExists(paramMap, "SENSOR_MIN_TEMP"))
	{
		sensor_min_temperature = std::stof(paramMap.find("SENSOR_MIN_TEMP")->second);
	}
	else
	{
		messenger.printDebugMessage(hardwareName, " !!WARNING!! could not find SENSOR_MIN_TEMP");
	}	
	//-------------------------------------------------------------------------------------------------
	messenger.printDebugMessage(hardwareName, " find MIN_X_PIXEL_POS");
	if (GlobalFunctions::entryExists(paramMap, "MIN_X_PIXEL_POS"))
	{
		min_x_pixel_pos = std::stof(paramMap.find("MIN_X_PIXEL_POS")->second);
	}
	else
	{
		messenger.printDebugMessage(hardwareName, " !!WARNING!! could not find MIN_X_PIXEL_POS");
	}	
	//-------------------------------------------------------------------------------------------------
	messenger.printDebugMessage(hardwareName, " find MAX_X_PIXEL_POS");
	if (GlobalFunctions::entryExists(paramMap, "MAX_X_PIXEL_POS"))
	{
		max_x_pixel_pos = std::stof(paramMap.find("MAX_X_PIXEL_POS")->second);
	}
	else
	{
	messenger.printDebugMessage(hardwareName, " !!WARNING!! could not find MAX_X_PIXEL_POS");
	}
	//-------------------------------------------------------------------------------------------------
	messenger.printDebugMessage(hardwareName, " find MIN_Y_PIXEL_POS");
	if (GlobalFunctions::entryExists(paramMap, "MIN_Y_PIXEL_POS"))
	{
		min_y_pixel_pos = std::stof(paramMap.find("MIN_Y_PIXEL_POS")->second);
	}
	else
	{
		messenger.printDebugMessage(hardwareName, " !!WARNING!! could not find MIN_Y_PIXEL_POS");
	}
	//-------------------------------------------------------------------------------------------------
	messenger.printDebugMessage(hardwareName, " find MAX_Y_PIXEL_POS");
	if (GlobalFunctions::entryExists(paramMap, "MAX_Y_PIXEL_POS"))
	{
		max_y_pixel_pos = std::stof(paramMap.find("MAX_Y_PIXEL_POS")->second);
	}
	else
	{
		messenger.printDebugMessage(hardwareName, " !!WARNING!! could not find MAX_Y_PIXEL_POS");
	}
	//-------------------------------------------------------------------------------------------------
	messenger.printDebugMessage(hardwareName, " find X_MASK_DEF");
	if (GlobalFunctions::entryExists(paramMap, "X_MASK_DEF"))
	{
		x_mask_def = (size_t)std::stof(paramMap.find("X_MASK_DEF")->second);
	}
	else
	{
		messenger.printDebugMessage(hardwareName, " !!WARNING!! could not find X_MASK_DEF");
	}	
	//-------------------------------------------------------------------------------------------------
	messenger.printDebugMessage(hardwareName, " find Y_MASK_DEF");
	if (GlobalFunctions::entryExists(paramMap, "Y_MASK_DEF"))
	{
		y_mask_def = (size_t)std::stof(paramMap.find("Y_MASK_DEF")->second);
	}
	else
	{
		messenger.printDebugMessage(hardwareName, " !!WARNING!! could not find Y_MASK_DEF");
	}	
	//-------------------------------------------------------------------------------------------------
	messenger.printDebugMessage(hardwareName, " find X_MASK_RAD_DEF");
	if (GlobalFunctions::entryExists(paramMap, "X_MASK_RAD_DEF"))
	{
		x_mask_rad_def = (size_t)std::stof(paramMap.find("X_MASK_RAD_DEF")->second);
	}
	else
	{
		messenger.printDebugMessage(hardwareName, " !!WARNING!! could not find X_MASK_RAD_DEF");
	}
	//-------------------------------------------------------------------------------------------------
	messenger.printDebugMessage(hardwareName, " find Y_MASK_RAD_DEF");
	if (GlobalFunctions::entryExists(paramMap, "Y_MASK_RAD_DEF"))
	{
		y_mask_rad_def = (size_t)std::stof(paramMap.find("Y_MASK_RAD_DEF")->second);
	}
	else
	{
		messenger.printDebugMessage(hardwareName, " !!WARNING!! could not find Y_MASK_RAD_DEF");
	}
	//-------------------------------------------------------------------------------------------------
	messenger.printDebugMessage(hardwareName, " find X_MASK_RAD_MAX");
	if (GlobalFunctions::entryExists(paramMap, "X_MASK_RAD_MAX"))
	{
		x_mask_rad_max = (size_t)std::stof(paramMap.find("X_MASK_RAD_MAX")->second);
	}
	else
	{
		messenger.printDebugMessage(hardwareName, " !!WARNING!! could not find X_MASK_RAD_MAX");
	}
	//-------------------------------------------------------------------------------------------------
	messenger.printDebugMessage(hardwareName, " find Y_MASK_RAD_MAX");
	if (GlobalFunctions::entryExists(paramMap, "Y_MASK_RAD_MAX"))
	{
		y_mask_rad_max = (size_t)std::stof(paramMap.find("Y_MASK_RAD_MAX")->second);
	}
	else
	{
		messenger.printDebugMessage(hardwareName, " !!WARNING!! could not find Y_MASK_RAD_MAX");
	}
	//-------------------------------------------------------------------------------------------------
	messenger.printDebugMessage(hardwareName, " find X_CENTER_DEF");
	if (GlobalFunctions::entryExists(paramMap, "X_CENTER_DEF"))
	{
		master_lattice_centre_x = (long)std::stoi(paramMap.find("X_CENTER_DEF")->second);
	}
	else
	{
		messenger.printDebugMessage(hardwareName, " !!WARNING!! could not find X_CENTER_DEF");
	}
	//-------------------------------------------------------------------------------------------------
	messenger.printDebugMessage(hardwareName, " find Y_CENTER_DEF");
	if (GlobalFunctions::entryExists(paramMap, "Y_CENTER_DEF"))
	{
		master_lattice_centre_y = (long)std::stoi(paramMap.find("Y_CENTER_DEF")->second);
	}
	else
	{
		messenger.printDebugMessage(hardwareName, " !!WARNING!! could not find Y_CENTER_DEF");
	}
	//-------------------------------------------------------------------------------------------------
	messenger.printDebugMessage(hardwareName, " find PIX_2_MM_RATIO_DEF");
	if (GlobalFunctions::entryExists(paramMap, "PIX_2_MM_RATIO_DEF"))
	{
		master_lattice_pixel_to_mm  = std::stod(paramMap.find("PIX_2_MM_RATIO_DEF")->second);
		messenger.printDebugMessage(hardwareName, " Found PIX_2_MM_RATIO_DEF, value = ", master_lattice_pixel_to_mm);

	}
	else
	{
		messenger.printDebugMessage(hardwareName, " !!WARNING!! could not find PIX_2_MM_RATIO_DEF");
	}
	// 
	// 
	// 
	//-------------------------------------------------------------------------------------------------
	messenger.printDebugMessage(hardwareName, " find USE_MASK_RAD_LIMITS");
	if (GlobalFunctions::entryExists(paramMap, "USE_MASK_RAD_LIMITS"))
	{
		if( std::string(paramMap.find("USE_MASK_RAD_LIMITS")->second) == GlobalConstants::TRUE_STR )
		{
			use_mask_rad_limits = true;
		}
		if( std::string(paramMap.find("USE_MASK_RAD_LIMITS")->second) == GlobalConstants::FALSE_STR )
		{
			use_mask_rad_limits = false;
		}
	}
	else
	{
		messenger.printDebugMessage(hardwareName, " !!WARNING!! could not find USE_MASK_RAD_LIMITS");
	}
	//-------------------------------------------------------------------------------------------------
	// This is really overworked, but does mean you can change names form the config file , if you want to, 
	// TODO in the future I would like more of the analysis results to go into this analysis_data array, also the analyis settings, etc  
	messenger.printDebugMessage(hardwareName, " find Y_CENTER_DEF");
	if (GlobalFunctions::entryExists(paramMap, "RESULTS_COUNT"))
	{
		analysis_data.second.resize( (size_t)std::stof(paramMap.find("RESULTS_COUNT")->second));
		analysis_data_names.resize( (size_t)std::stof(paramMap.find("RESULTS_COUNT")->second));
	}
	else
	{
		messenger.printDebugMessage(hardwareName, " !!WARNING!! could not find Y_CENTER_DEF");
	}

	if (GlobalFunctions::entryExists(paramMap, "X_POS"))
	{
		size_t pos = (size_t)std::stoi(paramMap.find("X_POS")->second);
		if (GlobalFunctions::entryExists(paramMap, "X_NAME"))
		{
			analysis_data_names[pos] = std::string(paramMap.find("X_NAME")->second);
		}
	}
	if (GlobalFunctions::entryExists(paramMap, "Y_POS"))
	{
		size_t pos = (size_t)std::stoi(paramMap.find("Y_POS")->second);
		if (GlobalFunctions::entryExists(paramMap, "Y_NAME"))
		{
			analysis_data_names[pos] = std::string(paramMap.find("Y_NAME")->second);
		}
	}
	if (GlobalFunctions::entryExists(paramMap, "X_SIGMA_POS"))
	{
		size_t pos = (size_t)std::stoi(paramMap.find("X_SIGMA_POS")->second);
		if (GlobalFunctions::entryExists(paramMap, "X_SIGMA_NAME"))
		{
			analysis_data_names[pos] = std::string(paramMap.find("X_SIGMA_NAME")->second);
		}
	}
	if (GlobalFunctions::entryExists(paramMap, "Y_SIGMA_POS"))
	{
		size_t pos = (size_t)std::stoi(paramMap.find("Y_SIGMA_POS")->second);
		if (GlobalFunctions::entryExists(paramMap, "Y_SIGMA_NAME"))
		{
			analysis_data_names[pos] = std::string(paramMap.find("Y_SIGMA_NAME")->second);
		}
	}
	if (GlobalFunctions::entryExists(paramMap, "COV_POS"))
	{
		size_t pos = (size_t)std::stoi(paramMap.find("COV_POS")->second);
		if (GlobalFunctions::entryExists(paramMap, "COV_NAME"))
		{
			analysis_data_names[pos] = std::string(paramMap.find("COV_NAME")->second);
		}
	}

	//-------------------------------------------------------------------------------------------------
	messenger.printDebugMessage(hardwareName, " find OPERATING_CENTER_X");
	if (GlobalFunctions::entryExists(paramMap, "OPERATING_CENTER_X"))
	{
		operating_centre_x = std::stod(paramMap.find("OPERATING_CENTER_X")->second);
		messenger.printDebugMessage(hardwareName, " Found OPERATING_CENTER_X, value = ", operating_centre_x);
	}
	else
	{
		messenger.printDebugMessage(hardwareName, " !!WARNING!! could not find RF_CENTER_X");
	}
	//-------------------------------------------------------------------------------------------------
	messenger.printDebugMessage(hardwareName, " find OPERATING_CENTER_Y");
	if (GlobalFunctions::entryExists(paramMap, "OPERATING_CENTER_Y"))
	{
		operating_centre_y = std::stod(paramMap.find("OPERATING_CENTER_Y")->second);
		messenger.printDebugMessage(hardwareName, " Found OPERATING_CENTER_Y, value = ", operating_centre_y);
	}
	else
	{
		messenger.printDebugMessage(hardwareName, " !!WARNING!! could not find RF_CENTER_Y");
	}

	messenger.printDebugMessage(hardwareName, " find MECHANICAL_CENTER_X");
	if (GlobalFunctions::entryExists(paramMap, "MECHANICAL_CENTER_X"))
	{
		mechanical_centre_x = std::stod(paramMap.find("MECHANICAL_CENTER_X")->second);
		messenger.printDebugMessage(hardwareName, " Found MECHANICAL_CENTER_X, value = ", mechanical_centre_x);

	}
	else
	{
		messenger.printDebugMessage(hardwareName, " !!WARNING!! could not find RF_CENTER_X");
	}
	//-------------------------------------------------------------------------------------------------
	messenger.printDebugMessage(hardwareName, " find MECHANICAL_CENTER_Y");
	if (GlobalFunctions::entryExists(paramMap, "MECHANICAL_CENTER_Y"))
	{
		mechanical_centre_y = std::stod(paramMap.find("MECHANICAL_CENTER_Y")->second);
		messenger.printDebugMessage(hardwareName, " Found MECHANICAL_CENTER_Y, value = ", mechanical_centre_y);

	}
	else
	{
		messenger.printDebugMessage(hardwareName, " !!WARNING!! could not find RF_CENTER_Y");
	}
	//if (GlobalFunctions::entryExists(paramMap, "CAM1_ARRAY_DATA_NUM_PIX_X"))
	//{
	//	size_t pos = (size_t)std::stoi(paramMap.find("CAM1_ARRAY_DATA_NUM_PIX_X")->second);
	//}
	//else
	//{
	//	messenger.printDebugMessage(hardwareName, " !!WARNING!! could not find CAM1_ARRAY_DATA_NUM_PIX_X");
	//}
}

Camera::Camera(const Camera& copyCamera):
Hardware(copyCamera),
mask_and_roi_keywords(copyCamera.mask_and_roi_keywords),
mask_keywords(copyCamera.mask_keywords),
roi_keywords(copyCamera.roi_keywords),
pixelResults(copyCamera.pixelResults),
x_pix_rs(copyCamera.x_pix_rs),
y_pix_rs(copyCamera.y_pix_rs),
sigma_x_pix_rs(copyCamera.sigma_x_pix_rs),
sigma_y_pix_rs(copyCamera.sigma_y_pix_rs),
sigma_xy_pix_rs(copyCamera.sigma_xy_pix_rs),
x_mm_rs(copyCamera.x_mm_rs),
y_mm_rs(copyCamera.y_mm_rs),
sigma_x_mm_rs(copyCamera.sigma_x_mm_rs),
sigma_y_mm_rs(copyCamera.sigma_y_mm_rs),
sigma_xy_mm_rs(copyCamera.sigma_xy_mm_rs),
avg_intensity_rs(copyCamera.avg_intensity_rs),
sum_intensity_rs(copyCamera.sum_intensity_rs),
image_data_has_not_malloced(copyCamera.image_data_has_not_malloced),
image_data_has_not_vector_resized(copyCamera.image_data_has_not_vector_resized),
roi_data_has_not_malloced(copyCamera.roi_data_has_not_malloced),
roi_data_has_not_vector_resized(copyCamera.roi_data_has_not_vector_resized),
image_data(copyCamera.image_data),
roi_data(copyCamera.roi_data),
analysis_data(copyCamera.analysis_data),
step_size(copyCamera.step_size),
acquire_time(copyCamera.acquire_time),
acquire_period(copyCamera.acquire_period),
temperature(copyCamera.temperature),
array_rate(copyCamera.array_rate),
use_npoint(copyCamera.use_npoint),
use_background(copyCamera.use_background),
cross_hair_overlay(copyCamera.cross_hair_overlay),
analysis_result_overlay(copyCamera.analysis_result_overlay),
analysis_mask_overlay(copyCamera.analysis_mask_overlay),
pixel_to_mm(copyCamera.pixel_to_mm),
black_level(copyCamera.black_level),
gain(copyCamera.gain),
set_new_background(copyCamera.set_new_background),
array_data_num_pix_x(copyCamera.array_data_num_pix_x),
array_data_num_pix_y(copyCamera.array_data_num_pix_y),
array_data_pixel_count(copyCamera.array_data_pixel_count),
roi_total_pixel_count(copyCamera.roi_total_pixel_count),
binary_num_pix_x(copyCamera.binary_num_pix_x),
binary_num_pix_y(copyCamera.binary_num_pix_y),
binary_data_pixel_count(copyCamera.binary_data_pixel_count),
x_pix_scale_factor(copyCamera.x_pix_scale_factor),
y_pix_scale_factor(copyCamera.y_pix_scale_factor),
x_mask_def(copyCamera.x_mask_def),
y_mask_def(copyCamera.y_mask_def),
x_mask_rad_def(copyCamera.x_mask_rad_def),
y_mask_rad_def(copyCamera.y_mask_rad_def),
x_mask_rad_max(copyCamera.x_mask_rad_max),
y_mask_rad_max(copyCamera.y_mask_rad_max),
use_mask_rad_limits(copyCamera.use_mask_rad_limits),
sensor_max_temperature(copyCamera.sensor_max_temperature),
sensor_min_temperature(copyCamera.sensor_min_temperature),
average_pixel_value_for_beam(copyCamera.average_pixel_value_for_beam),
min_x_pixel_pos(copyCamera.min_x_pixel_pos),
max_x_pixel_pos(copyCamera.max_x_pixel_pos),
min_y_pixel_pos(copyCamera.min_y_pixel_pos),
max_y_pixel_pos(copyCamera.max_y_pixel_pos),
busy(copyCamera.busy),
has_led(copyCamera.has_led),
last_capture_and_save_success(copyCamera.last_capture_and_save_success)
//mask_and_roi_keywords_Py(copyCamera.mask_and_roi_keywords_Py),
//mask_keywords_Py(copyCamera.mask_keywords_Py),
//roi_keywords_Py(copyCamera.roi_keywords_Py)
{
}
Camera::~Camera(){}
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
		}
		else
		{
			std::cout << "Can't find record = " << record << std::endl;
		}
	}
}
//
//                   ___  __  
//  |\ |  /\   |\/| |__  /__` 
//  | \| /~~\  |  | |___ .__/ 
// 
std::vector<std::string> Camera::getAliases()const { return aliases; }
boost::python::list Camera::getAliases_Py()const { return to_py_list<std::string>(getAliases()); }
std::string Camera::getScreen()const
{
	if (screen_names.size() > 0)
	{
		return screen_names[0];
	}
	return ENUM_TO_STRING(STATE::UNKNOWN_NAME);
}
std::vector<std::string> Camera::getScreenNames()const { return screen_names; }
boost::python::list Camera::getScreenNames_Py()const { return to_py_list<std::string>(getScreenNames()); }
// 
//  __         ___         ___  __                 
// |__) | \_/ |__  |        |  /  \     |\/|  |\/| 
// |    | / \ |___ |___     |  \__/     |  |  |  | 
//   
double Camera::getPixelToMM()const{	return pixel_to_mm.second;}
bool Camera::setPixelToMM(double value)const{return epicsInterface->putValue2<epicsFloat64>(pvStructs.at(CameraRecords::ANA_PixMM), (epicsFloat64)value);}
double Camera::pix2mm(double value)const{ return value / pixel_to_mm.second;}
double Camera::mm2pix(double value)const{ return value * pixel_to_mm.second;}
double Camera::pix2mmX(double value)const{ return value / pix2mmX_ratio;}
double Camera::pix2mmY(double value)const{ return value / pix2mmY_ratio;}
double Camera::mm2pixX(double value)const{ return value * pix2mmX_ratio;}
double Camera::mm2pixY(double value)const{ return value * pix2mmX_ratio;}
double Camera::getpix2mmX()const{ return pix2mmX_ratio;}
double Camera::getpix2mmY()const{ return pix2mmY_ratio;}
double Camera::setpix2mmX(double value){ return pix2mmX_ratio = value;}
double Camera::setpix2mmY(double value){ return pix2mmY_ratio = value;}
//    __   ___      ___  ___  __      __   __      __                      __      __   __      __   __   __  
//   /  ` |__  |\ |  |  |__  |__)    /  \ |__)    /  \ |\ | __  /\  \_/ | /__`    /  ` /  \ __ /  \ |__) |  \	
//   \__, |___ | \|  |  |___ |  \    \__/ |  \    \__/ | \|    /~~\ / \ | .__/    \__, \__/    \__/ |  \ |__/ 
//  
long Camera::getCentreXPixel()const{	return x_center_pixel.second;}
long Camera::getCentreYPixel()const{	return y_center_pixel.second;}
bool Camera::setCentreXPixel(long value)
{	
	return epicsInterface->putValue2<epicsInt32>(pvStructs.at(CameraRecords::ANA_CenterX), (epicsInt32)value);
}
bool Camera::setCentreYPixel(long value){	return epicsInterface->putValue2<epicsInt32>(pvStructs.at(CameraRecords::ANA_CenterY), (epicsInt32)value);}
bool Camera::setCentrePixels(long x, long y)
{
	if (setCentreXPixel(x))
	{
		return setCentreYPixel(y);
	}
	return false;
}
bool Camera::setMechanicalCentre(){	return setCentrePixels(mechanical_centre_x, mechanical_centre_y);}
bool Camera::setOperatingCenter(){	return setCentrePixels(operating_centre_x, operating_centre_y);}
long Camera::getOperatingCentreXPixel()const{	return operating_centre_x;}
long Camera::getOperatingCentreYPixel()const{	return operating_centre_y;}
long Camera::getMechanicalCentreXPixel()const{	return mechanical_centre_x;}
long Camera::getMechanicalCentreYPixel()const{	return mechanical_centre_y;}
//  			  __   ___ __    __  ___
//  |  |\/|  /\  / _` |__ /__` |  / |__
//  |  |  | /~~\ \__> |___.__/ | /_ |___
// 
long Camera::getPixelWidth()const{	return epics_pixel_width.second;}
long Camera::getPixelHeight()const{	return epics_pixel_height.second;}
size_t Camera::getArrayDataPixelCountX()const{	return array_data_num_pix_x;}
size_t Camera::getArrayDataPixelCountY()const{	return array_data_num_pix_y;}
size_t Camera::getBinaryDataPixelCountX()const{	return binary_num_pix_x;}
size_t Camera::getBinaryDataPixelCountY()const{	return binary_num_pix_y;}
//	 __   __                __        __  
//	/  ` |__) |  |    |    /  \  /\  |  \ 
//	\__, |    \__/    |___ \__/ /~~\ |__/ 
//	                                      
long Camera::getCPUTotal()const{	return cpu_total.second;}
long Camera::getCPUCropSubMask()const{	return cpu_crop_sub_mask.second;}
long Camera::getCPUNPoint()const{	return cpu_npoint.second;}
long Camera::getCPUDot()const{	return cpu_dot.second;}
//	              __   ___     __        ___  __                 __  
//	|  |\/|  /\  / _` |__     /  \ \  / |__  |__) |     /\  \ / /__` 
//	|  |  | /~~\ \__> |___    \__/  \/  |___ |  \ |___ /~~\  |  .__/ 
//
bool Camera::enableOverlayCross(){	return epicsInterface->putValue2<epicsUInt16>(pvStructs.at(CameraRecords::ANA_OVERLAY_1_CROSS), GlobalConstants::zero_ushort);}
bool Camera::disableOverlayCross(){	return epicsInterface->putValue2<epicsUInt16>(pvStructs.at(CameraRecords::ANA_OVERLAY_1_CROSS), GlobalConstants::one_ushort);}
STATE Camera::getOverlayCrossState()const{	return cross_hair_overlay.second;}
bool Camera::isOverlayCrossEnabled()const{	return cross_hair_overlay.second == STATE::ENABLED;}
bool Camera::isOverlayCrossDisabled()const{	return cross_hair_overlay.second == STATE::DISABLED;}
bool Camera::enableOverlayMask(){	return epicsInterface->putValue2<epicsUInt16>(pvStructs.at(CameraRecords::ANA_OVERLAY_3_MASK), GlobalConstants::one_ushort);}
bool Camera::disableOverlayMask(){	return epicsInterface->putValue2<epicsUInt16>(pvStructs.at(CameraRecords::ANA_OVERLAY_3_MASK), GlobalConstants::zero_ushort);}
STATE Camera::getOverlayMaskState()const{	return analysis_mask_overlay.second;}
bool Camera::isOverlayMaskEnabled()const{	return analysis_mask_overlay.second == STATE::ENABLED;}
bool Camera::isOverlayMaskDisabled()const{	return analysis_mask_overlay.second == STATE::DISABLED;}
bool Camera::enableOverlayResult(){	return epicsInterface->putValue2<epicsUInt16>(pvStructs.at(CameraRecords::ANA_OVERLAY_2_RESULT), GlobalConstants::one_ushort);}
bool Camera::disableOverlayResult(){	return epicsInterface->putValue2<epicsUInt16>(pvStructs.at(CameraRecords::ANA_OVERLAY_2_RESULT), GlobalConstants::zero_ushort);}
STATE Camera::getOverlayResultState()const{	return analysis_result_overlay.second;}
bool Camera::isOverlayResultEnabled()const{	return analysis_result_overlay.second == STATE::ENABLED;}
bool Camera::isOverlayResultDisabled()const{	return analysis_result_overlay.second == STATE::DISABLED;}
bool Camera::disableAllOverlay()
{
	bool m_dis = disableOverlayMask();
	bool c_dis = disableOverlayCross();
	bool r_dis = disableOverlayResult();
	if (m_dis)
	{
		if (c_dis)
		{
			if (r_dis)
			{
				return true;
			}
		}
	}
	return true;
}
//	              __   ___     __        ___  ___  ___  __  
//	|  |\/|  /\  / _` |__     |__) |  | |__  |__  |__  |__) 
//	|  |  | /~~\ \__> |___    |__) \__/ |    |    |___ |  \ 
//	
bool Camera::saveImageBuffer()
{
	if (busy)
	{
		return false;
	}
	if (makeANewDirectoryAndNameBuffer())
	{
		epicsUInt8 proc = 1;
		return epicsInterface->putValue2<epicsUInt8>(pvStructs.at(CameraRecords::HDFB_image_buffer_trigger), proc);
	}
	return false;
}
char Camera::getImageBufferTrigger()const{return image_buffer_trigger.second;}
std::string Camera::getImageBufferFilePath()const{return image_buffer_filepath.second;}
std::string Camera::getImageBufferFileName()const{return image_buffer_filename.second;}
long Camera::getImageBufferFileNumber()const{return image_buffer_filenumber.second;}
std::string Camera::getLastDirectoryandFileName()const{return getLastDirectory() + "/" + getLastFileName(); }// WARNING!! TODO unix / windows conventions :(( USE BOOST! 
std::string Camera::getLastDirectory()const{return save_filepath.second;}
std::string Camera::getLastFileName()const{return save_filename.second;}
std::string Camera::getLastImageBufferDirectoryandFileName() const{	return getLastImageBufferDirectory() + "/" + getLastImageBufferFileName();}// WARNING!! TODO unix / windows conventions :(( USE BOOST! 
std::string Camera::getLastImageBufferDirectory()const{	return image_buffer_filepath.second;}
std::string Camera::getLastImageBufferFileName()const{	return image_buffer_filename.second;}
bool Camera::makeANewDirectoryAndNameBuffer()///YUCK (make it look nice)
{
	messenger.printDebugMessage("makeANewDirectoryAndNameBuffer ");
	bool ans = false;
	messenger.printDebugMessage("char: ", sizeof(char));
	//std::string time_now = globalfunctions::time_iso_8601();
	// this sets up the directory structure, based on year, month date
	std::chrono::system_clock::time_point p = std::chrono::system_clock::now();
	std::time_t t = std::chrono::system_clock::to_time_t(p);
	// now we jump into some c because, meh 
	// use local timezone 
	tm local_tm = *localtime(&t);
	////struct tm local_tm;
	//localtime_s(&local_tm, &t);  windows version that got added c11 but not accepted by gcc !??!? 
	char newPath[256] = "/CameraImages/"; // case sensitive!!! TODO add to master lattice 
	std::string strpath = "/CameraImages/" +
		std::to_string(local_tm.tm_year + 1900) +
		"/" + std::to_string(local_tm.tm_mon + 1) +
		"/" + std::to_string(local_tm.tm_mday) + "/";
	strcpy(newPath, strpath.c_str());
	char newName[256] = "defaultname";
	std::string strName = hardwareName + "_" + GlobalFunctions::time_iso_8601() + "_ImageBuffer";
	strName = GlobalFunctions::replaceStrChar(strName, ":", '-');
	strcpy(newName, strName.c_str());
	messenger.printDebugMessage("File Directory would be: ", newPath);
	messenger.printDebugMessage("File name is: ", newName);
	std::stringstream s;
	for (auto&& t : newPath)
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
	if (ca_state(pvStructs.at(CameraRecords::HDFB_image_buffer_filename).CHID) == cs_conn)
	{
		int status = ca_array_put(DBR_CHAR, 256, pvStructs.at(CameraRecords::HDFB_image_buffer_filename).CHID, newName);
		MY_SEVCHK(status);
		status = ca_pend_io(CA_PEND_IO_TIMEOUT);
		MY_SEVCHK(status);
		if (status == ECA_NORMAL)
		{
			if (ca_state(pvStructs.at(CameraRecords::HDF_FilePath).CHID) == cs_conn)
			{
				int status = ca_array_put(DBR_CHAR, 256, pvStructs.at(CameraRecords::HDFB_image_buffer_filepath).CHID, newPath);
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
				messenger.printDebugMessage(hardwareName, " HDFB_image_buffer_filepath is not connected");
			}
		}
		else
		{
			messenger.printDebugMessage(hardwareName, " send file name failed status = ", status);
		}
	}
	else
	{
		messenger.printDebugMessage(hardwareName, " HDFB_image_buffer_filename is not connected");
	}
	return ans;
}
//	              __   ___     __        __  ___       __   ___               __      __             _ 
//	|  |\/|  /\  / _` |__     /  `  /\  |__)  |  |  | |__) |__      /\  |\ | |  \    /__`  /\  \  / |__  
//	|  |  | /~~\ \__> |___    \__, /~~\ |     |  \__/ |  \ |___    /~~\ | \| |__/    .__/ /~~\  \/  |___ 
//	 
bool Camera::makeANewDirectoryAndName(size_t numbOfShots)///YUCK (make it look nice)
{
	messenger.printDebugMessage("makeANewDirectoryAndName ");
	bool ans = false;
	messenger.printDebugMessage("char: ", sizeof(char));
	//std::string time_now = globalfunctions::time_iso_8601();

	// this sets up the directory structure, based on year, month date
	std::chrono::system_clock::time_point p = std::chrono::system_clock::now();
	std::time_t t = std::chrono::system_clock::to_time_t(p);

	// now we jump into some c because, meh 
	// use local timezone 
	tm local_tm = *localtime(&t);
	////struct tm local_tm;
	//localtime_s(&local_tm, &t);  windows version that got added c11 but not accepted by gcc !??!? 
	char newPath[256] = "/CameraImages/"; // case sensitive!!! TODO add to master lattice 
	std::string strpath = "/CameraImages/" +
		std::to_string(local_tm.tm_year + 1900) +
		"/" + std::to_string(local_tm.tm_mon + 1) +
		"/" + std::to_string(local_tm.tm_mday) + "/";

	strcpy(newPath, strpath.c_str());

	char newName[256] = "defaultname";
	std::string strName = hardwareName + "_" + GlobalFunctions::time_iso_8601() + "UTC_" + std::to_string(numbOfShots) + "_images";
	strName = GlobalFunctions::replaceStrChar(strName, ":", '-');
	strcpy(newName, strName.c_str());
	messenger.printDebugMessage("File Directory would be: ", newPath);
	messenger.printDebugMessage("File name is: ", newName);
	std::stringstream s;
	for (auto&& t : newPath)
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
bool Camera::didLastCaptureAndSaveSucceed()const{	return last_capture_and_save_success == true;}
bool Camera::captureAndSave(){	return captureAndSave(GlobalConstants::one_sizet);}
bool Camera::captureAndSave(size_t num_shots)
{
	messenger.printDebugMessage("**** captureAndSave ****\n", hardwareName);
	if (isNotAcquiring())
	{
		messenger.printDebugMessage(hardwareName," Is not acquiring so no images cna be captured adn saved");
		return false;
	}
	else
	{
		busy = true;
		messenger.printDebugMessage(hardwareName, " collectAndSave passed num_shots = ", num_shots);
		/*
			kill any finished threads
		*/
		killFinishedImageCollectThread();
		messenger.printDebugMessage("killFinishedImageCollectThreads fin");
		if (image_capture.thread == nullptr)
		{
			messenger.printDebugMessage("image_capture.thread is not busy");
			if (num_shots <= max_shots_number)
			{
				messenger.printDebugMessage("Requested number of shots ok, create new imageCollectStructs");

				image_capture.is_busy = true;
				image_capture.status = STATE::CAPTURING;
				image_capture.num_shots = num_shots;
				image_capture.cam = this;
				image_capture.thread
					= new std::thread(staticEntryImageCollectAndSave, std::ref(image_capture));
				//GlobalFunctions::pause_500();
				messenger.printDebugMessage("new imageCollectStruct created and running in new thread");
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
		messenger.printDebugMessage(hardwareName, " ImageCapture is busy, no images can be captured and saved at this time, try again later.");
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
	/*
		WHATEVER YOU DO TO THIS FUNCTION BE MINDFUL OF THE PAUSES!!! THEY COULD DO WITH A MORE ROBUST SOLUTION AT SOME POINT // TODO
	*/
	messenger.printDebugMessage(hardwareName, " imageCaptureAndSave called");
	bool timed_out = false;
	bool carry_on = true;
	if (num_shots > GlobalConstants::zero_sizet)
	{
		bool carry_on = setNumberOfShotsToCapture(num_shots);
	}
	if (carry_on)
	{
		messenger.printDebugMessage(hardwareName, " Set number of shots success");
		if (capture())
		{
			messenger.printDebugMessage("imageCollectAndSave is waiting for collection to finish");
			//GlobalFunctions::pause_500();
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
				messenger.printDebugMessage(hardwareName, " timed out during capture");
				image_capture.status = STATE::TIMEOUT;
				image_capture.is_busy = false;
			}
			else
			{
				messenger.printDebugMessage(hardwareName, " makeANewDirectoryAndName out during capture");
				if (makeANewDirectoryAndName(num_shots))
				{
					GlobalFunctions::pause_500();
					//GlobalFunctions::pause_500();
					messenger.printDebugMessage("imageCollectAndSave ", hardwareName, " is going to write collected data");
					if (write())
					{
						messenger.printDebugMessage("imageCollectAndSave ", hardwareName, " is waiting for writing to finish");
						while (isWriting())   //wait until saving is done...
						{
							GlobalFunctions::pause_50(); //MAGIC_NUMBER
						}
						messenger.printDebugMessage("imageCollectAndSave ", hardwareName, " writing has finished");
						// pause and wait for EPICS to UPDATE
						GlobalFunctions::pause_500();
						GlobalFunctions::pause_500();
						GlobalFunctions::pause_500();
						//check status of save/write
						if (write_state_check.second == STATE::WRITE_CHECK_OK)
						{
							last_capture_and_save_success = true;//this->message("Successful wrote image to disk.");
							image_capture.status = STATE::SUCCESS;
						}
						else
						{
							messenger.printDebugMessage("!!WRITE ERROR!!: camera = ", hardwareName, ", error message =  ", write_error_message.second);
							last_capture_and_save_success = false;
							image_capture.status = STATE::WRITE_CHECK_ERROR;
						}
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
	image_capture.is_busy = false;
	busy = false;
}
size_t Camera::getNumberOfShotsToCapture()const{return capture_count.second;}
bool Camera::setNumberOfShotsToCapture(size_t num)
{
	if (num <= max_shots_number)
	{
		return epicsInterface->putValue2<epicsInt32>(pvStructs.at(CameraRecords::HDF_NumCapture), (epicsInt32)num);
	}
	return false;
}
bool Camera::capture()
{
	bool ans = false;
	if (isAcquiring())
	{
		ans = epicsInterface->putValue2<epicsUInt16>(pvStructs.at(CameraRecords::HDF_Capture), GlobalConstants::one_ushort);
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
	// WHAT IS THIS DOING???? OLD stuff to converted to jpg automatically that has not been implmented 
	//setStartFileNumberJPG(startNumber);
	if (isNotCapturing())
	{
		ans = epicsInterface->putValue2<epicsUInt16>(pvStructs.at(CameraRecords::HDF_WriteFile), GlobalConstants::one_ushort);
		messenger.printDebugMessage("WriteFile set to 1 on camera = ", hardwareName);
	}
	else
	{
		messenger.printDebugMessage(hardwareName, " still collecting images when write function was called.");
	}
	return ans;
}
bool Camera::resetCaptureAndSaveError()
{
	bool set_capture = epicsInterface->putValue2<epicsUInt16>(pvStructs.at(CameraRecords::HDF_Capture), GlobalConstants::zero_ushort);
	bool set_write_file = epicsInterface->putValue2<epicsUInt16 >(pvStructs.at(CameraRecords::HDF_WriteFile), GlobalConstants::zero_ushort);
	bool set_write_stat = epicsInterface->putValue2<epicsUInt16>(pvStructs.at(CameraRecords::HDF_WriteStatus), GlobalConstants::zero_ushort);
	if (set_capture)
	{
		if (set_write_file)
		{
			if (set_write_stat)
			{
				return true;
			}
		}
	}
	return false;
}
bool Camera::isCapturing()const{ return getCaptureState() == STATE::CAPTURING; }
bool Camera::isNotCapturing()const { return getCaptureState() == STATE::NOT_CAPTURING; }
bool Camera::isCapturingOrSaving()const
{
	if (isCapturing())
	{
		return true;
	}
	if (isWriting())
	{
		return true;
	}
	return false;
}
bool Camera::isNotCapturingOrSaving()const{	return !isCapturingOrSaving();}
//	 __  ___       __  ___   /  __  ___  __   __           __   __          __          __  
//	/__`  |   /\  |__)  |   /  /__`  |  /  \ |__)     /\  /  ` /  \ |  | | |__) | |\ | / _` 
//	.__/  |  /~~\ |  \  |  /   .__/  |  \__/ |       /~~\ \__, \__X \__/ | |  \ | | \| \__> 
//
double Camera::getActiveCameraLimit()const{ return active_camera_limit.second;}
double Camera::getActiveCameraCount()const{ return active_camera_count.second;}
bool Camera::canStartCamera()const{	return getActiveCameraLimit() > getActiveCameraCount();}
void Camera::staticEntryWaitForCamStopAcquiring(CamStopWaiter& csw)
{
	//csw.cam->messenger.printDebugMessage(csw.cam->hardwareName + " timeout wait time = ", csw.wait_ms);
	auto start = std::chrono::high_resolution_clock::now();
	bool timed_out = false;
	// first wait fro isbusy to be false

	while (true)
	{
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start);
		/* check if time ran out */
		if (duration.count() > csw.wait_ms)
		{
			csw.cam->messenger.printDebugMessage(csw.cam->hardwareName + " has timed out waiting for isNotBusy, DT = ", duration.count());
			timed_out = true;
			break;
		}
		if (csw.cam->isNotBusy())
		{
			//csw.cam->messenger.printDebugMessage(csw.cam->hardwareName + " is NOT busy!");
			break;
		}
	}
	if (!timed_out)
	{
		start = std::chrono::high_resolution_clock::now();
		while (true)
		{
			auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start);
			/* check if time ran out */
			if (duration.count() > csw.wait_ms)
			{
				csw.cam->messenger.printDebugMessage(csw.cam->hardwareName + " has timed out waiting for StopAcquiring, DT = ", duration.count());
				timed_out = true;
				break;
			}
			if (csw.cam->isNotAcquiring())
			{
				csw.cam->messenger.printDebugMessage(csw.cam->hardwareName + " has Stopped Acquiring.");
				break;
			}
		}
	}
	if (timed_out)
	{
		csw.cam->messenger.printDebugMessage(csw.cam->hardwareName + " has timed out waiting for StopAcquiring");
		csw.result = STATE::TIMEOUT;
	}
	else
	{
		csw.cam->messenger.printDebugMessage(csw.cam->hardwareName + " has waited for  Stopped Acquiring.");
		csw.result = STATE::SUCCESS;

	}

}
bool Camera::stopAcquiringAndWait(size_t timeout = 3000)
{
	if (isNotAcquiring())
		return true;
	if (stopAcquiring())
	{	// TODO and tudy up 
		cam_stop_waiter_struct.cam = this;
		cam_stop_waiter_struct.wait_ms = timeout;
		cam_stop_waiter_struct.thread = new std::thread(staticEntryWaitForCamStopAcquiring, std::ref(cam_stop_waiter_struct));
		cam_stop_waiter_struct.thread->join();
		delete cam_stop_waiter_struct.thread;
		cam_stop_waiter_struct.thread = nullptr;
		if (cam_stop_waiter_struct.result == STATE::SUCCESS)
			return true;
	}
	return false;
}
bool Camera::startAcquiring()
{
	// we could put a canStartCamera check in here, but why bother???
	return epicsInterface->putValue2<epicsUInt16 >(pvStructs.at(CameraRecords::CAM_Start_Acquire), GlobalConstants::one_ushort);
}
bool Camera::stopAcquiring()
{
	return  epicsInterface->putValue2<epicsUInt16>(pvStructs.at(CameraRecords::CAM_Stop_Acquire), GlobalConstants::zero_ushort);
}
bool Camera::toggleAcquiring()
{
	std::cout << "toggleAcquiring" << std::endl;
	if (isAcquiring())
	{
		std::cout << "isAcquiring = true, stopAcquiring" << std::endl;
		return stopAcquiring();
	}
	std::cout << "isAnalysing = FALSE, startAcquiring" << std::endl;
	return startAcquiring();
}
bool Camera::isAcquiring()const{ return getAcquireState() == STATE::ACQUIRING; }
bool Camera::isNotAcquiring()const{ return getAcquireState() == STATE::NOT_ACQUIRING; }
STATE Camera::getAcquireState()const{ return acquire_state.second; }
double Camera::getAcquireTime()const{return acquire_time.second;}
double Camera::getAcquirePeriod()const{	return acquire_period.second;}
double Camera::getArrayRate()const{	return array_rate.second;}
//	              __   ___                             __     __  
//	|  |\/|  /\  / _` |__      /\  |\ |  /\  |    \ / /__` | /__` 
//	|  |  | /~~\ \__> |___    /~~\ | \| /~~\ |___  |  .__/ | .__/ 
// 
bool Camera::startAnalysing()
{
	bool  r = false;
	bool stopped = true;
	bool should_stop_and_restart = isAcquiring();
	if (should_stop_and_restart) { bool stopped = stopAcquiringAndWait(); }
	if(stopped)
	{ 
		r = epicsInterface->putValue2<epicsUInt16>(pvStructs.at(CameraRecords::ANA_EnableCallbacks), GlobalConstants::one_ushort);
		if (should_stop_and_restart) { startAcquiring(); }
	}
	return r;
}
bool Camera::stopAnalysing()
{
	bool  r = false;
	bool stopped = true;
	bool should_stop_and_restart = isAcquiring();
	if (should_stop_and_restart) { bool stopped = stopAcquiringAndWait(); }
	if (stopped)
	{
		r = epicsInterface->putValue2<epicsUInt16>(pvStructs.at(CameraRecords::ANA_EnableCallbacks), GlobalConstants::zero_ushort);
		if (should_stop_and_restart) { startAcquiring(); }
	}
	return r;
}
bool Camera::toggleAnalysing()
{
	std::cout << "toggleUseNPointScaling" << std::endl;
	if (isAnalysing())
	{
		std::cout << "isAnalysing = true, stopAnalysing" << std::endl;
		return stopAnalysing();
	}
	std::cout << "isAnalysing = FALSE, startAnalysing" << std::endl;
	return startAnalysing();
}
bool Camera::isAnalysing()const{ return getAnalysisState() == STATE::ANALYSING;}
bool Camera::isNotAnalysing()const{	return getAnalysisState() == STATE::NOT_ANALYSING;}
STATE Camera::getAnalysisState()const{ return analysis_state.second;}
bool Camera::isAnalysisUpdating(){	return isResultUpdated;}
//	                         __     __           __     __   ___     ___       __   __   __  
//	 /\  |\ |  /\  |    \ / /__` | /__`    |\ | /  \ | /__` |__     |__  |    /  \ /  \ |__) 
//	/~~\ | \| /~~\ |___  |  .__/ | .__/    | \| \__/ | .__/ |___    |    |___ \__/ \__/ |  \ 
//
STATE Camera::getUseFloorState()const{ return use_floor.second;}
bool Camera::isUsingFloor()const{ return use_floor.second == STATE::USING_FLOOR;}
bool Camera::isNotUsingFloor()const{ return use_floor.second == STATE::NOT_USING_FLOOR;}
bool Camera::epics_setUseFloor(){ return epicsInterface->putValue_flushio<epicsUInt16>(pvStructs.at(CameraRecords::ANA_UseFloor), GlobalConstants::one_ushort); }
bool Camera::epics_setDoNotUseFloor(){ return epicsInterface->putValue_flushio<epicsUInt16>(pvStructs.at(CameraRecords::ANA_UseFloor), GlobalConstants::zero_ushort); }
bool Camera::epics_setFloorLevel(long v) { return epicsInterface->putValue2<epicsInt32>(pvStructs.at(CameraRecords::ANA_FloorLevel), v); }
bool Camera::setUseFloor(){ return genericStopAcquiringApplySetting(&Camera::epics_setUseFloor);}
bool Camera::setDoNotUseFloor(){ return genericStopAcquiringApplySetting(&Camera::epics_setDoNotUseFloor);}
bool Camera::toggleUseFloor(){ return isUsingFloor()?setDoNotUseFloor():setUseFloor();}
bool Camera::setFloorLevel(long v)
{
	return genericStopAcquiringApplySetting2(&Camera::epics_setFloorLevel, v);

	//if (mode == STATE::PHYSICAL)
	//{
	//	bool should_stop_and_restart = isAcquiring();
	//	if (should_stop_and_restart) { stopAcquiringAndWait(); }
	//	bool r = epicsInterface->putValue2<epicsInt32>(pvStructs.at(CameraRecords::ANA_FloorLevel), v);
	//	if (should_stop_and_restart) { startAcquiring(); }
	//	return r;
	//}
	//return false;
}
//	                         __     __           __   __         ___     __   __                    __  
//	 /\  |\ | |     /\  \ / /__` | /__`    |\ | |__) /  \ | |\ |  |     /__` /  `  /\  |    | |\ | / _` 
//	/~~\ | \| |___ /~~\  |  .__/ | .__/    | \| |    \__/ | | \|  |     .__/ \__, /~~\ |___ | | \| \__> 
//	                                                                                                    
// 
bool Camera::setUseNPointScaling()
{
	bool  r = false;
	bool stopped = true;
	bool should_stop_and_restart = isAcquiring();
	if (should_stop_and_restart) { bool stopped = stopAcquiringAndWait(); }
	if (stopped)
	{
		std::cout << "setUseNPointScaling putValue2 1 " << std::endl;
		r = epicsInterface->putValue2<epicsUInt16>(pvStructs.at(CameraRecords::ANA_UseNPoint), GlobalConstants::one_ushort);
		if (should_stop_and_restart) { startAcquiring(); }
	}
	return r;
}
bool Camera::setDoNotUseNPointScaling()
{
	bool  r = false;
	bool stopped = true;
	bool should_stop_and_restart = isAcquiring();
	if (should_stop_and_restart) { bool stopped = stopAcquiringAndWait(); }
	if (stopped)
	{
		std::cout << "isUsingNsetDoNotUseNPointScalingPointScaling putValue2 0 " << std::endl;
		
		r = epicsInterface->putValue2<epicsUInt16>(pvStructs.at(CameraRecords::ANA_UseNPoint), GlobalConstants::zero_ushort);
		if (should_stop_and_restart) { startAcquiring(); }
	}
	return  r;
}
bool Camera::toggleUseNPointScaling()
{
	std::cout << "toggleUseNPointScaling" << std::endl;
	if (isUsingNPointScaling())
	{
		std::cout << "isUsingNPointScaling = true, setDoNotUseNPointScaling" << std::endl;
		return setDoNotUseNPointScaling();
	}
	std::cout << "isUsingNPointScaling = FALSE, setUseNPointScaling" << std::endl;
	return setUseNPointScaling();
}
STATE Camera::getNPointScalingState()const{	return use_npoint.second;}
bool Camera::isUsingNPointScaling()const{ return use_npoint.second == STATE::USING_NPOINT;}
bool Camera::isNotUsingNPointScaling()const{ return use_npoint.second == STATE::NOT_USING_NPOINT;}
long Camera::getNpointScalingStepSize()const{ return step_size.second;}
bool Camera::setNpointScalingStepSize(long val){ return epicsInterface->putValue2<epicsInt32>(pvStructs.at(CameraRecords::ANA_NPointStepSize), (epicsInt32)val);}
//                         __     __      __        __        __   __   __             __     
// /\  |\ | |     /\  \ / /__` | /__`    |__)  /\  /  ` |__/ / _` |__) /  \ |  | |\ | |  \    
///~~\ | \| |___ /~~\  |  .__/ | .__/    |__) /~~\ \__, |  \ \__> |  \ \__/ \__/ | \| |__/    
//                                                                                            
//
bool Camera::setNewBackgroundImage()
{
	return  epicsInterface->putValue2<epicsUInt16>(pvStructs.at(CameraRecords::ANA_NewBkgrnd), GlobalConstants::one_ushort);
}

bool Camera::epics_setUseBackgroundImage(){	return epicsInterface->putValue2<epicsUInt16>(pvStructs.at(CameraRecords::ANA_UseBkgrnd), GlobalConstants::one_ushort);}
bool Camera::epics_setDoNotUseBackgroundImage(){ return epicsInterface->putValue2<epicsUInt16>(pvStructs.at(CameraRecords::ANA_UseBkgrnd), GlobalConstants::zero_ushort);}


bool Camera::setUseBackgroundImage()
{

	bool  r = false;
	bool stopped = true;
	bool should_stop_and_restart = isAcquiring();
	if (should_stop_and_restart) { bool stopped = stopAcquiringAndWait(); }
	if (stopped)
	{
		std::cout << "setUseBackgroundImage putValue2 0 " << std::endl;
		r = epicsInterface->putValue2<epicsUInt16>(pvStructs.at(CameraRecords::ANA_UseBkgrnd), GlobalConstants::one_ushort);
		if (should_stop_and_restart) { startAcquiring(); }
	}
	return r;
}
bool Camera::setDoNotUseBackgroundImage()
{	
	bool  r = false;
	bool stopped = true;
	bool should_stop_and_restart = isAcquiring();
	if (should_stop_and_restart) { bool stopped = stopAcquiringAndWait(); }
	if (stopped)
	{
		std::cout << "setDoNotUseBackgroundImage putValue2 0 " << std::endl;
		r = epicsInterface->putValue2<epicsUInt16>(pvStructs.at(CameraRecords::ANA_UseBkgrnd), GlobalConstants::zero_ushort);
		if (should_stop_and_restart) { startAcquiring(); }
	}
	return r;
}
bool Camera::toggleUseBackgroundImage()
{
	std::cout << "toggleUseBackgroundImage" << std::endl;
	if (isUsingNPointScaling())
	{
		std::cout << "isUsingNPointScaling = true, setDoNotUseNPointScaling" << std::endl;
		return setDoNotUseNPointScaling();
	}
	std::cout << "isUsingNPointScaling = FALSE, setUseNPointScaling" << std::endl;
	return setUseNPointScaling();
}
bool Camera::isUsingBackgroundImage()const{	return use_background.second == STATE::USING_BACKGROUND;}
bool Camera::isNotUsingBackgroundImage()const{	return use_background.second == STATE::NOT_USING_BACKGROUND;}
STATE Camera::getUsingBackgroundImageState()const{	return use_background.second;}
//	                         __     __                 __       
//	 /\  |\ |  /\  |    \ / /__` | /__`     |\/|  /\  /__` |__/ 
//	/~~\ | \| /~~\ |___  |  .__/ | .__/     |  | /~~\ .__/ |  \ 
//
bool Camera::setMaskXCenter(long val){	return  epicsInterface->putValue2<epicsInt32>(pvStructs.at(CameraRecords::ANA_MaskXCenter), (epicsInt32)val);}
bool Camera::setMaskYCenter(long val){	return  epicsInterface->putValue2<epicsInt32>(pvStructs.at(CameraRecords::ANA_MaskYCenter), (epicsInt32)val);}
bool Camera::setMaskXRadius(long val){	return  epicsInterface->putValue2<epicsInt32>(pvStructs.at(CameraRecords::ANA_MaskXRad), (epicsInt32)val);}
bool Camera::setMaskYRadius(long val){	return  epicsInterface->putValue2<epicsInt32>(pvStructs.at(CameraRecords::ANA_MaskYRad), (epicsInt32)val);}
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
	std::cout << "!!ERROR!! wrong setMask keywords: ";
	for (auto&& item : settings)
	{
		std::cout << item.first << ", ";
	}
	std::cout << std::endl;
	std::cout << "Expecting: ";
	for (auto&& item : mask_keywords)
	{
		std::cout << item << ", ";
	}
	std::cout << std::endl;
	return false;
}
bool Camera::setMask_Py(boost::python::dict settings){	return setMask(to_std_map<std::string, long>(settings));}
long Camera::getMaskXCenter()const{	return mask_x_center.second;}
long Camera::getMaskYCenter()const{	return mask_y_center.second;}
long Camera::getMaskXRadius()const{	return mask_x_radius.second;}
long Camera::getMaskYRadius()const{	return mask_y_radius.second;}
std::map<std::string, long> Camera::getMask()const
{
	std::map<std::string, long> r;
	r["mask_x"] = getMaskXCenter(); // MAGIC STRING
	r["mask_y"] = getMaskYCenter(); // MAGIC STRING
	r["mask_rad_x"] = getMaskXRadius();// MAGIC STRING
	r["mask_rad_y"] = getMaskYRadius();// MAGIC STRING
	return r;
}
boost::python::dict Camera::getMask_Py()const{	return  to_py_dict<std::string, long>(getMask());}
//	 __   __    
//	|__) /  \ | 
//	|  \ \__/ | 
//
bool Camera::setROIMinX(long val){	return  epicsInterface->putValue2<epicsInt32>(pvStructs.at(CameraRecords::ROI1_MinX), (epicsInt32)val);}
bool Camera::setROIMinY(long val){	return  epicsInterface->putValue2<epicsInt32>(pvStructs.at(CameraRecords::ROI1_MinY), (epicsInt32)val);}
bool Camera::setROISizeX(long val){	return  epicsInterface->putValue2<epicsInt32>(pvStructs.at(CameraRecords::ROI1_SizeX), (epicsInt32)val);}
bool Camera::setROISizeY(long val){	return  epicsInterface->putValue2<epicsInt32>(pvStructs.at(CameraRecords::ROI1_SizeY), (epicsInt32)val);}
bool Camera::setROI(long x_max, long  y_max, long x_rad, long y_rad)
{
	if (setMaskAndROIxMax(x_max))
	{
		if (setMaskAndROIyMax(y_max))
		{
			if (setMaskAndROIxSize(x_rad))
			{
				if (setMaskAndROIySize(y_rad))
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
	if (GlobalFunctions::entriesExist(settings, mask_and_roi_keywords))
	{
		return setROI(settings.at("x_max"), settings.at("y_max"), settings.at("x_rad"), settings.at("y_rad"));
	}
	messenger.printDebugMessage("!!Failed!! setROI passed incorrect keywords, expecting, x_max, y_max, x_rad, y_rad");
	return false;
}
bool Camera::setROI_Py(boost::python::dict settings){	return setROI(to_std_map<std::string, long>(settings));}
long Camera::getROIMinX()const{	return roi_min_x.second;}
long Camera::getROIMinY()const{	return roi_min_y.second;}
long Camera::getROISizeX()const{	return roi_size_x.second;}
long Camera::getROISizeY()const{	return roi_size_y.second;}
std::map<std::string, long> Camera::getROI()const
{
	std::map<std::string, long> r;
	r["roi_min_x"] = getROIMinX(); // MAGIC STRING
	r["roi_min_y"] = getROIMinY(); // MAGIC STRING
	r["roi_x_size"] = getROISizeX();// MAGIC STRING
	r["roi_y_size"] = getROISizeY();// MAGIC STRING
	return r;
}
boost::python::dict Camera::getROI_Py()const
{
	return  to_py_dict<std::string, long>(getROI());
}
//	 __   __                  __                 __           __   __         __   __  
//	|__) /  \ |     /\  |\ | |  \     |\/|  /\  /__` |__/    /  ` /  \  |\/| |__) /  \ 
//	|  \ \__/ |    /~~\ | \| |__/     |  | /~~\ .__/ |  \    \__, \__/  |  | |__) \__/ 
//
long Camera::getMaskAndROIxMax()const{	return roi_and_mask_centre_x.second; }
long Camera::getMaskAndROIyMax()const{	return roi_and_mask_centre_y.second; }
long Camera::getMaskAndROIxSize()const{	return roi_and_mask_radius_x.second; }
long Camera::getMaskAndROIySize()const{	return roi_and_mask_radius_y.second; }
bool Camera::setMaskAndROIxMax(long val){	return  epicsInterface->putValue2<epicsFloat64>(pvStructs.at(CameraRecords::ROIandMask_SetX), (epicsFloat64)val);}
bool Camera::setMaskAndROIyMax(long val){	return  epicsInterface->putValue2<epicsFloat64>(pvStructs.at(CameraRecords::ROIandMask_SetY), (epicsFloat64)val);}
bool Camera::setMaskAndROIxSize(long val)
{
	return  epicsInterface->putValue2<epicsFloat64>(pvStructs.at(CameraRecords::ROIandMask_SetXrad), (epicsFloat64)val);
}
bool Camera::setMaskAndROIySize(long val)
{
	return  epicsInterface->putValue2<epicsFloat64>(pvStructs.at(CameraRecords::ROIandMask_SetYrad), (epicsFloat64)val);
}
bool Camera::setMaskandROI(long x_max, long  y_max, long x_rad, long y_rad)
{
	if (setMaskAndROIxMax(x_max))
	{
		if (setMaskAndROIyMax(y_max))
		{
			if (setMaskAndROIxSize(x_rad))
			{
				if (setMaskAndROIySize(y_rad))
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
		return setMaskandROI(settings.at("roi_x"), settings.at("roi_y"), settings.at("x_rad"), settings.at("y_rad"));
	}
	std::cout << "!!ERROR!! wrong setMaskandROI keywords: ";
	for (auto&& item : settings)
	{
		std::cout << item.first << ", ";
	}
	std::cout << std::endl;
	std::cout << "Expecting: ";
	for (auto&& item : mask_and_roi_keywords)
	{
		std::cout << item << ", ";
	}
	std::cout << std::endl;
	return false;
}
bool Camera::setMaskandROI_Py(boost::python::dict settings)
{
	return setMaskandROI(to_std_map<std::string, long>(settings));
}
std::map<std::string, long> Camera::getMaskandROI()const
{
	std::map<std::string, long> r;
	r["roi_min_x"] = getROIMinX(); // MAGIC STRING
	r["roi_min_y"] = getROIMinY(); // MAGIC STRING
	r["roi_x_size"] = getROISizeX();// MAGIC STRING
	r["roi_y_size"] = getROISizeY();// MAGIC STRING
	r["mask_center_x"] = getMaskXCenter(); // MAGIC STRING
	r["mask_center_y"] = getMaskYCenter(); // MAGIC STRING
	r["mask_x_size"] = getMaskXRadius();// MAGIC STRING
	r["mask_y_size"] = getMaskYRadius();// MAGIC STRING
	return r;
}
boost::python::dict Camera::getMaskandROI_Py()const
{
	return  to_py_dict<std::string, long>(getMaskandROI());
}
//	       ___   __   
//	|     |__   |  \  
//	|___ .|___ .|__/ .
//
bool Camera::hasLED()const
{
	return has_led;
}
bool Camera::setLEDOn()
{
	// ALSO HAVE IN SCREENS 
	if (has_led)
	{
		if (epicsInterface->putValue2<epicsUInt8>(pvStructs.at(CameraRecords::LED_On), (epicsUInt8)GlobalConstants::EPICS_ACTIVATE))
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(150));//MAGIC_NUMBER!
			return epicsInterface->putValue2<epicsUInt8>(pvStructs.at(CameraRecords::LED_On), (epicsUInt8)GlobalConstants::EPICS_SEND);
		}
		messenger.printDebugMessage(hardwareName, " Send setLEDOn EPICS_ACTIVATE failed ");
	}
	else
	{
		messenger.printDebugMessage(hardwareName, " Does not have a LED, setLEDOn failed ");
	}
	return false;
}
bool Camera::setLEDOff()
{
	// ALSO HAVE IN SCREENS
	if (epicsInterface->putValue2<epicsUInt8>(pvStructs.at(CameraRecords::LED_Off), (epicsUInt8)GlobalConstants::EPICS_ACTIVATE))
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(150));//MAGIC_NUMBER!
		return epicsInterface->putValue2<epicsUInt8>(pvStructs.at(CameraRecords::LED_Off), (epicsUInt8)GlobalConstants::EPICS_SEND);
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






//	 __                            __      __             __                ___       ___      
//	/ _`  /\  | |\ |     /\  |\ | |  \    |__) |     /\  /  ` |__/    |    |__  \  / |__  |    
//	\__> /~~\ | | \|    /~~\ | \| |__/    |__) |___ /~~\ \__, |  \    |___ |___  \/  |___ |___ 
//






boost::python::dict Camera::getAllRunningStats()const
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
	r["sigma_xy_mm"] = sigma_xy_mm_rs.getRunningStats();   // MAGIC STRING
	return r;
}











bool Camera::isBusy()const
{
	return busy == true;
}
bool Camera::isNotBusy()const
{
	return busy == false;
}






//size_t Camera::getNumXPixFromArrayData() const
//{
//	return array_data_num_pix_x;
//}
//size_t Camera::getNumYPixFromArrayData() const
//{
//	return array_data_num_pix_y;
//}
//size_t Camera::getFullNumXPix() const
//{
//	return binary_num_pix_x;
//}
//size_t Camera::getFullNumYPix() const
//{
//	return binary_num_pix_y;
//}



TYPE Camera::getCamType()const
{
	return cam_type;
}




std::map<std::string, double> Camera::getAnalysisResultsPixels()const
{
	std::map<std::string, double> r;
	std::string n;
	double v;
	for (auto it : boost::combine(analysis_data_names, analysis_data.second)) {
		;
		boost::tie(n, v) = it;
		r[n] = v;
	}
	return r;
}
boost::python::dict Camera::getAnalysisResultsPixels_Py()const
{
	return  to_py_dict<std::string, double>(getAnalysisResultsPixels());
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
size_t Camera::getXPixScaleFactor() const
{
	return x_pix_scale_factor;
}
double Camera::getYPix()const
{
	return y_pix.second;
}
size_t Camera::getYPixScaleFactor() const
{
	return y_pix_scale_factor;
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
		return  false;
	}
	//messenger.printDebugMessage(hardwareName, CameraRecords::ANA_X_RBV + " putvalue = ", value);
	epicsInterface->putValue(pvStructs.at(CameraRecords::ANA_X_RBV), value);
	epicsInterface->putValue(pvStructs.at(CameraRecords::ANA_XPix_RBV), mm2pixX(value));
	//x_pix = std::make_pair(x_mm.first, mm2pixX(value));
	//x_mm = std::make_pair(epicsTimeStamp(), value);
	return true;
}
bool Camera::setY(double value)
{
	if (mode == STATE::PHYSICAL)
	{
		return false;
	}
	//messenger.printDebugMessage(hardwareName, CameraRecords::ANA_Y_RBV + " putvalue = ", value);
	epicsInterface->putValue(pvStructs.at(CameraRecords::ANA_Y_RBV), value);
	epicsInterface->putValue(pvStructs.at(CameraRecords::ANA_YPix_RBV), mm2pixX(value));
	//y_mm = std::make_pair(epicsTimeStamp(), value);
	//y_pix = std::make_pair(y_mm.first, mm2pixY(value));
	return true;
}
bool Camera::setSigX(double value)
{
	if (mode == STATE::PHYSICAL)
	{
		return false;
	}
	//messenger.printDebugMessage(hardwareName, CameraRecords::ANA_SigmaX_RBV + " putvalue = ", value);
	epicsInterface->putValue(pvStructs.at(CameraRecords::ANA_SigmaX_RBV), value);
	epicsInterface->putValue(pvStructs.at(CameraRecords::ANA_SigmaXPix_RBV), mm2pixX(value));	
	//sigma_x_mm = std::make_pair(epicsTimeStamp(), value);
	//sigma_x_pix = std::make_pair(sigma_x_mm.first, mm2pixX(value));
	return true;
}
bool Camera::setSigY(double value)
{
	if (mode == STATE::PHYSICAL)
	{
		return false;
	}
	//messenger.printDebugMessage(hardwareName, CameraRecords::ANA_SigmaY_RBV + " putvalue = ", value);
	epicsInterface->putValue(pvStructs.at(CameraRecords::ANA_SigmaY_RBV), value);
	epicsInterface->putValue(pvStructs.at(CameraRecords::ANA_SigmaYPix_RBV), mm2pixX(value));
	//sigma_y_mm = std::make_pair(epicsTimeStamp(), value);
	//sigma_y_pix = std::make_pair(sigma_y_mm.first, mm2pixY(value));
	return true;
}
bool Camera::setSigXY(double value)
{
	if (mode == STATE::PHYSICAL)
	{
		return false;
	}
	//messenger.printDebugMessage(hardwareName, CameraRecords::ANA_CovXY_RBV + " putvalue = ", value);
	epicsInterface->putValue(pvStructs.at(CameraRecords::ANA_CovXY_RBV), value);
	epicsInterface->putValue(pvStructs.at(CameraRecords::ANA_CovXYPix_RBV), mm2pixX(value));
	//sigma_xy_mm = std::make_pair(epicsTimeStamp(), value);
	//sigma_xy_pix = std::make_pair(epicsTimeStamp(), mm2pixX(value));
	return true;
}

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
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
double Camera::getAveragePixelValueForBeam()const
{
	return average_pixel_value_for_beam;
}
bool Camera::setAveragePixelValueForBeam(const double& value)
{
	average_pixel_value_for_beam = value;
	return true;
}
bool Camera::hasBeam()const
{
	return getAvgIntensity() > getAveragePixelValueForBeam();
}
bool Camera::hasNoBeam()const
{
	return !hasBeam();
}




bool Camera::areAllRunningStatsFull()const
{
	if (x_pix_rs.Full())
	{
		if (y_pix_rs.Full())
		{
			if (sigma_x_pix_rs.Full())
			{
				if (sigma_y_pix_rs.Full())
				{
					if (sigma_xy_pix_rs.Full())
					{
						if (x_mm_rs.Full())
						{
							if (y_mm_rs.Full())
							{
								if (sigma_x_mm_rs.Full())
								{
									if (sigma_y_mm_rs.Full())
									{
										if (sigma_xy_mm_rs.Full())
										{
											if (avg_intensity_rs.Full())
											{
												if (sum_intensity_rs.Full())
												{
													return true;
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	return false;
}




RunningStats& Camera::getXPixRunningStats(){	return x_pix_rs;}
RunningStats& Camera::getYPixRunningStats(){	return y_pix_rs;}
RunningStats& Camera::getSigmaXPixRunningStats(){	return sigma_x_pix_rs;}
RunningStats& Camera::getSigmaYPixRunningStats(){	return sigma_y_pix_rs;}
RunningStats& Camera::getSigmaXYPixRunningStats(){	return sigma_xy_pix_rs;}
RunningStats& Camera::getXmmRunningStats(){	return x_mm_rs;}
RunningStats& Camera::getYmmRunningStats(){	return y_mm_rs;}
RunningStats& Camera::getSigmaXmmRunningStats(){	return sigma_x_mm_rs;}

RunningStats& Camera::getSigmaYmmRunningStats(){	return sigma_y_mm_rs;	}
RunningStats& Camera::getSigmaXYmmRunningStats(){	return sigma_xy_mm_rs;	}
RunningStats& Camera::getAvgIntensityRunningStats(){	return avg_intensity_rs;}
RunningStats& Camera::getSumIntensityRunningStats(){	return sum_intensity_rs;}
size_t Camera::getBufferSize()const{return running_stats_buffer_size;}
void Camera::setAllRunningStatBufferSizes(size_t v)
{
	x_pix_rs.setBufferSize(v);			y_pix_rs.setBufferSize(v);	sigma_x_pix_rs.setBufferSize(v);	sigma_y_pix_rs.setBufferSize(v);
	sigma_xy_pix_rs.setBufferSize(v);	x_mm_rs.setBufferSize(v);	y_mm_rs.setBufferSize(v);			sigma_x_mm_rs.setBufferSize(v);
	sigma_y_mm_rs.setBufferSize(v);		sigma_xy_mm_rs.setBufferSize(v);	running_stats_buffer_size = v;
}
void Camera::clearAllRunningStatBuffers()
{
	x_pix_rs.clearBuffer();	y_pix_rs.clearBuffer();	sigma_x_pix_rs.clearBuffer(); sigma_y_pix_rs.clearBuffer();	sigma_xy_pix_rs.clearBuffer();	
	x_mm_rs.clearBuffer();	y_mm_rs.clearBuffer();	sigma_x_mm_rs.clearBuffer();  sigma_y_mm_rs.clearBuffer();  sigma_xy_mm_rs.clearBuffer();
}
void Camera::clearAllRunningStats()
{
	x_pix_rs.Clear();	y_pix_rs.Clear();	sigma_x_pix_rs.Clear();	sigma_y_pix_rs.Clear();	sigma_xy_pix_rs.Clear();	x_mm_rs.Clear();
	y_mm_rs.Clear();	sigma_x_mm_rs.Clear();	sigma_y_mm_rs.Clear();	sigma_xy_mm_rs.Clear();	avg_intensity_rs.Clear();	sum_intensity_rs.Clear();
}
void Camera::setAllRunningStatSizes(size_t new_val)
{
	x_pix_rs.setMaxCount(new_val);	y_pix_rs.setMaxCount(new_val);	sigma_x_pix_rs.setMaxCount(new_val);	sigma_y_pix_rs.setMaxCount(new_val);	sigma_xy_pix_rs.setMaxCount(new_val);	x_mm_rs.setMaxCount(new_val);
	y_mm_rs.setMaxCount(new_val);	sigma_x_mm_rs.setMaxCount(new_val);	sigma_y_mm_rs.setMaxCount(new_val);	sigma_xy_mm_rs.setMaxCount(new_val);	avg_intensity_rs.setMaxCount(new_val);	sum_intensity_rs.setMaxCount(new_val);
}
size_t Camera::getRunningStatNumDataValues()const
{
	std::vector<size_t> NumDataValuesVector(GlobalConstants::twelve_sizet, GlobalConstants::sizet_max);
	NumDataValuesVector[0] = x_pix_rs.NumDataValues(); 			NumDataValuesVector[1] = y_pix_rs.NumDataValues();
	NumDataValuesVector[2] = sigma_x_pix_rs.NumDataValues();	NumDataValuesVector[3] = sigma_y_pix_rs.NumDataValues();
	NumDataValuesVector[4] = sigma_xy_pix_rs.NumDataValues();	NumDataValuesVector[5] = x_mm_rs.NumDataValues();
	NumDataValuesVector[6] = y_mm_rs.NumDataValues();			NumDataValuesVector[7] = sigma_x_mm_rs.NumDataValues();
	NumDataValuesVector[8] = sigma_y_mm_rs.NumDataValues();		NumDataValuesVector[9] = sigma_xy_mm_rs.NumDataValues();
	NumDataValuesVector[10] = avg_intensity_rs.NumDataValues();	NumDataValuesVector[11] = sum_intensity_rs.NumDataValues();
	return *std::min_element(NumDataValuesVector.begin(), NumDataValuesVector.end());;
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

std::vector<double> Camera::getPixelResults() const
{
	//std::cout << "X_POS: " << std::setprecision(15) << pixelResults.second.at(0)<< std::endl;
	return pixelResults.second;
}
boost::python::list Camera::getPixelResults_Py()
{
	//std::cout << "X_POS_PY: " << pixelResults.second.at(0) << std::endl;
	//std::vector<double> returnList = pixelResults.second;
	return to_py_list<double>(pixelResults.second);
}
std::vector<double> Camera::getMMResults() const
{
	return mmResults.second;
}
boost::python::list Camera::getMMResults_Py()
{
	return to_py_list<double>(pixelResults.second);
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
	messenger.printDebugMessage(hardwareName, " vector_resize to ", array_data_pixel_count, " Items");
	vec.resize(array_data_pixel_count);
	return vec.size() == array_data_pixel_count;
}
void Camera::malloc_imagedata()
{
	/*	allocate memory for dbr_image_data, array pointer 	*/
	messenger.printDebugMessage(hardwareName, " malloc_imagedata ", array_data_pixel_count, " DBR_TIME_LONG");
	unsigned nBytes = dbr_size_n(DBR_TIME_LONG, array_data_pixel_count);
	dbr_image_data = (struct dbr_time_long*)malloc(nBytes);
	messenger.printDebugMessage(hardwareName, " dbr_image_data pointer allocated ", nBytes, " BYTES ");
	image_data_has_not_malloced = false;
}
void Camera::malloc_roidata()
{
	/*		allocate memory for dbr_roi_data, array pointer  	*/
	messenger.printDebugMessage(hardwareName, " malloc_roidata ", array_data_pixel_count, " DBR_TIME_LONG");
	unsigned nBytes = dbr_size_n(DBR_TIME_LONG, array_data_pixel_count);
	dbr_roi_data = (struct dbr_time_long*)malloc(nBytes);
	messenger.printDebugMessage(hardwareName, " dbr_roi_data pointer allocated ", nBytes, " BYTES ");
	roi_data_has_not_malloced = false;
}
bool Camera::updateImageData()
{
	//std::lock_guard<std::mutex> lg(mtx);  // This now locked your mutex mtx.lock();
	//messenger.printDebugMessage("updateImageData");
	//if (image_data_has_not_malloced)
	//{
	//	messenger.printDebugMessage("image_data_has_not_malloced, calling malloc_imagedata, array_data_pixel_count = ", array_data_pixel_count);
	//	malloc_imagedata();
	//}
	//else
	//{
	//	messenger.printDebugMessage("image_data_has_malloce");
	//}
	if (image_data_has_not_vector_resized)
	{
		messenger.printDebugMessage("image_data_has_not_vector_resized = True");
		vector_resize(image_data.second);
		if (image_data.second.size() == array_data_pixel_count)
		{
			image_data_has_not_vector_resized = false;
		}
		else
		{
			image_data_has_not_vector_resized = true;
		}
	}
	if (!image_data_has_not_vector_resized)
	{
		//messenger.printDebugMessage("Getting new array data ");
		auto start = std::chrono::high_resolution_clock::now();
		//bool got_stamp = getArrayTimeStamp(dbr_image_data, pvStructs.at(CameraRecords::CAM2_ArrayData)
		//	, image_data.first);
		bool got_value = getArrayValue(image_data.second, pvStructs.at(CameraRecords::CAM2_ArrayData)
			, image_data.second.size());
		auto stop = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
		//messenger.printDebugMessage("updateImageData Time taken: ", duration.count(), " us");
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
		//auto start = std::chrono::high_resolution_clock::now();
		bool got_stamp = getArrayTimeStamp(dbr_image_data, pvStructs.at(CameraRecords::CAM2_ArrayData)
			, image_data.first);
		bool got_value = getArrayValue(image_data.second, pvStructs.at(CameraRecords::CAM2_ArrayData)
			, image_data.second.size());
		//auto stop = std::chrono::high_resolution_clock::now();
		//auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
		//messenger.printDebugMessage("updateImageDataWithTimeStamp Time taken: ", duration.count(), " us");
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
		vector_resize(roi_data.second);
		if (roi_data.second.size() == array_data_pixel_count)
		{
			roi_data_has_not_vector_resized = false;
		}
		else
		{
			roi_data_has_not_vector_resized = true;
		}
	}
	if (!roi_data_has_not_vector_resized)
	{
		auto start = std::chrono::high_resolution_clock::now();
		//bool got_value = getArrayValue(roi_data.second, pvStructs.at(CameraRecords::ROI1_ImageData_RBV)
		//	, roi_data.second.size());

		roi_total_pixel_count = roi_size_x.second * roi_size_y.second;

		//messenger.printDebugMessage("roi_total_pixel_count = ", roi_total_pixel_count);

		bool got_value = getArrayValue(roi_data.second, pvStructs.at(CameraRecords::ROI1_ImageData_RBV)
			, roi_total_pixel_count);

		auto stop = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
		//messenger.printDebugMessage("updateROIData Time taken: ", duration.count(), " us");
		return got_value;
	}
	return false;
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
		//messenger.printDebugMessage("updateROIDataWithTimeStamp Time taken: ", duration.count(), " us");
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
		EPICSInterface::sendToEPICSm("this is from getArrayTimeStamp");
		MY_SEVCHK(status);
		ts_to_update = dbr_struct->stamp;
		//std::cout << epicsInterface->getEPICSTime(ts_to_update) << std::endl;
		auto stop  = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
		//messenger.printDebugMessage("getArrayTimeStamp Time taken: ", duration.count(), " us");
		//std::cout << dbr_struct->value << std::endl;
		return true;
	}
	return false;
}
bool Camera::getArrayValue(std::vector<long>& data_vec, const pvStruct & pvs,size_t count)
{
//	return epicsInterface->get_camera_array(data_vec, pvs, count);
	//messenger.printDebugMessage("getArrayValue Function");
	auto start = std::chrono::high_resolution_clock::now();
	if (ca_state(pvs.CHID) == cs_conn)
	{
		int status = ca_array_get(DBR_LONG, count, pvs.CHID, &data_vec[0]);
		if (EPICSInterface::sendToEPICSm2("this is from getArrayValue"));
		{
			auto stop = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
			//messenger.printDebugMessage("getArrayValue Time taken: ", duration.count(), " us");
			return true;
		}
		//messenger.printDebugMessage("!!ERROR!!  ca_array_get, took  ", duration.count(), " us");	
	}
	return false;
}
//std::vector<double>* Camera::getImagedataByPointer()
//{
//	//mValues.clear();
//	//for (double d_ = 0.0; d_ < 4; d_ += 0.3)
//	//{
//	//	mValues.push_back(d_);
//	//}
//	return &image_data.second;
//}
//const std::vector<long>& Camera::getImageData_Ref()const
//{
//	//std::lock_guard<std::mutex> lg(mtx);  // This now locked your mutex mtx.lock();
//	return image_data.second;
//}
std::vector<long> Camera::getImageData()const
{
	//std::lock_guard<std::mutex> lg(mtx);  // This now locked your mutex mtx.lock();
	return image_data.second;
}
boost::python::list Camera::getImageData_Py()const
{
	return to_py_list<long>(image_data.second);
}
std::vector<long> Camera::getROIData()const
{
	return roi_data.second;
}
boost::python::list Camera::getROIData_Py()const
{
	return to_py_list<long>(GlobalFunctions::slice(getROIData(),GlobalConstants::zero_int, roi_total_pixel_count-1));
}
std::vector<long>& Camera::getImageDataConstRef()
{
	return image_data.second;
}
std::vector<long>& Camera::getROIDataConstRef()
{
	return roi_data.second;
}


//	 __                            __      __             __                ___       ___      
//	/ _`  /\  | |\ |     /\  |\ | |  \    |__) |     /\  /  ` |__/    |    |__  \  / |__  |    
//	\__> /~~\ | | \|    /~~\ | \| |__/    |__) |___ /~~\ \__, |  \    |___ |___  \/  |___ |___ 
//
bool Camera::setBlackLevel(long value)
{
	if (getCamType() == TYPE::VELA_CAMERA)
	{
		return epicsInterface->putValue2<epicsInt32>(pvStructs.at(CameraRecords::CAM_BlackLevel), (epicsInt32)value);
	}
	return false;
}
long Camera::getBlackLevel()const
{
	return black_level.second;
}
bool Camera::setGain(long value)
{
	if (getCamType() == TYPE::VELA_CAMERA)
	{
		return epicsInterface->putValue2<epicsInt32>(pvStructs.at(CameraRecords::CAM_Gain), (epicsInt32)value);
	}
	return false;
}
long Camera::getGain()const
{
	return gain.second;
}
//			 __             __   __        __  ___    
//			/__` |\ |  /\  |__) /__` |__| /  \  |     
//			.__/ | \| /~~\ |    .__/ |  | \__/  |     
//
HardwareSnapshot Camera::getSnapshot()
{
	messenger.printDebugMessage("CAM_Acquire_RBV:  ", ENUM_TO_STRING(acquire_state.second));
	currentSnapshot.update<STATE>(CameraRecords::CAM_Acquire_RBV, acquire_state.second);
	currentSnapshot.update<STATE>(CameraRecords::ANA_OVERLAY_1_CROSS_RBV, cross_hair_overlay.second);
	currentSnapshot.update<STATE>(CameraRecords::ANA_OVERLAY_2_RESULT, analysis_result_overlay.second);
	currentSnapshot.update<STATE>(CameraRecords::ANA_OVERLAY_3_MASK_RBV, analysis_mask_overlay.second);
	currentSnapshot.update<STATE>(CameraRecords::ANA_UseNPoint_RBV, use_npoint.second);
	currentSnapshot.update<STATE>(CameraRecords::ANA_EnableCallbacks_RBV, analysis_state.second);
	currentSnapshot.update<STATE>(CameraRecords::HDF_WriteFile_RBV, write_state.second);
	currentSnapshot.update<STATE>(CameraRecords::HDF_WriteStatus, write_state_check.second);
	currentSnapshot.update<STATE>(CameraRecords::LED_Sta, led_state.second);
	currentSnapshot.update<STATE>(CameraRecords::HDF_Capture_RBV, capture_state.second);
	currentSnapshot.update<STATE>(CameraRecords::ANA_UseFloor_RBV, use_floor.second);
	currentSnapshot.update<TYPE>("cam_type", cam_type);

	currentSnapshot.update(CameraRecords::ANA_FloorLevel_RBV, floor_level.second);


	currentSnapshot.update(CameraRecords::CAM_BlackLevel_RBV, black_level.second);
	currentSnapshot.update(CameraRecords::CAM_Gain_RBV, gain.second);
	
	currentSnapshot.update(CameraRecords::CAM_ArrayRate_RBV, array_rate.second);
	currentSnapshot.update(CameraRecords::CAM_AcquirePeriod_RBV, acquire_period.second);
	currentSnapshot.update(CameraRecords::CAM_AcquireTime_RBV, acquire_time.second);
	messenger.printDebugMessage("CAM_Active_Count:  ", active_camera_count.second);
	messenger.printDebugMessage("CAM_Active_Limit:  ", active_camera_limit.second);
	currentSnapshot.update<double>(CameraRecords::CAM_Active_Count, active_camera_count.second);
	currentSnapshot.update<double>(CameraRecords::CAM_Active_Limit, active_camera_limit.second);
	
	currentSnapshot.update(CameraRecords::ROI1_MinX_RBV, roi_min_x.second);
	currentSnapshot.update(CameraRecords::ROI1_MinY_RBV, roi_min_y.second);
	currentSnapshot.update(CameraRecords::ROI1_SizeX_RBV, roi_size_x.second);
	currentSnapshot.update(CameraRecords::ROI1_SizeY_RBV, roi_size_y.second);
	currentSnapshot.update(CameraRecords::ROIandMask_SetX, roi_and_mask_centre_x.second);
	currentSnapshot.update(CameraRecords::ROIandMask_SetY, roi_and_mask_centre_y.second);
	currentSnapshot.update(CameraRecords::ROIandMask_SetXrad, roi_and_mask_radius_x.second);
	currentSnapshot.update(CameraRecords::ROIandMask_SetYrad, roi_and_mask_radius_y.second);
	currentSnapshot.update(CameraRecords::ANA_MaskXCenter_RBV, mask_x_center.second);
	currentSnapshot.update(CameraRecords::ANA_MaskYCenter_RBV, mask_y_center.second);
	currentSnapshot.update(CameraRecords::ANA_MaskXRad_RBV, mask_x_radius.second);
	currentSnapshot.update(CameraRecords::ANA_MaskYRad_RBV, mask_y_radius.second);
	currentSnapshot.update(CameraRecords::ANA_FlooredPoints_RBV, floored_pts_count.second);
	currentSnapshot.update(CameraRecords::ANA_NPointStepSize_RBV, step_size.second);
	currentSnapshot.update(CameraRecords::ANA_UseBkgrnd_RBV, use_background.second);
	currentSnapshot.update(CameraRecords::ANA_FlooredPercent_RBV, floored_pts_percent.second);
	currentSnapshot.update(CameraRecords::CAM_Temperature_RBV, temperature.second);
	currentSnapshot.update(CameraRecords::ANA_CPU_RBV, cpu_total.second);
	currentSnapshot.update(CameraRecords::ANA_CPU_CropSubMask_RBV, cpu_crop_sub_mask.second);
	currentSnapshot.update(CameraRecords::ANA_CPU_Npoint_RBV, cpu_npoint.second);
	currentSnapshot.update(CameraRecords::ANA_CPU_Dot_RBV, cpu_dot.second);
	currentSnapshot.update(CameraRecords::HDF_WriteMessage, write_error_message.second);
	currentSnapshot.update(CameraRecords::HDF_FilePath_RBV, save_filepath.second);
	currentSnapshot.update(CameraRecords::HDF_FileName_RBV, save_filename.second);
	currentSnapshot.update(CameraRecords::HDF_FileNumber_RBV, save_filenumber.second);
	currentSnapshot.update(CameraRecords::HDFB_image_buffer_filepath_RBV, image_buffer_filepath.second);
	currentSnapshot.update(CameraRecords::HDFB_image_buffer_filename_RBV, image_buffer_filename.second);
	currentSnapshot.update(CameraRecords::HDFB_image_buffer_filenumber_RBV, image_buffer_filenumber.second);
	currentSnapshot.update(CameraRecords::HDF_NumCapture_RBV, capture_count.second);
	currentSnapshot.update(CameraRecords::HDF_FileNumber_RBV, save_filenumber.second);
	currentSnapshot.update(CameraRecords::ANA_PixW_RBV, epics_pixel_width.second);
	currentSnapshot.update(CameraRecords::ANA_PixH_RBV, epics_pixel_height.second);
	currentSnapshot.update(CameraRecords::ANA_CenterX_RBV, x_center_pixel.second);
	currentSnapshot.update(CameraRecords::ANA_CenterY_RBV, y_center_pixel.second);
	currentSnapshot.update(CameraRecords::ANA_PixMM_RBV, pixel_to_mm.second);
	currentSnapshot.update(CameraRecords::ANA_XPix_RBV, x_pix.second);
	currentSnapshot.update(CameraRecords::ANA_YPix_RBV, y_pix.second);
	currentSnapshot.update(CameraRecords::ANA_SigmaXPix_RBV, sigma_x_pix.second);
	currentSnapshot.update(CameraRecords::ANA_SigmaYPix_RBV, sigma_y_pix.second);
	currentSnapshot.update(CameraRecords::ANA_CovXYPix_RBV, sigma_xy_pix.second);
	currentSnapshot.update(CameraRecords::ANA_X_RBV, x_mm.second);
	currentSnapshot.update(CameraRecords::ANA_Y_RBV, y_mm.second);
	currentSnapshot.update(CameraRecords::ANA_SigmaX_RBV, sigma_x_mm.second);
	currentSnapshot.update(CameraRecords::ANA_SigmaY_RBV, sigma_y_mm.second);
	currentSnapshot.update(CameraRecords::ANA_CovXY_RBV, sigma_xy_mm.second);
	currentSnapshot.update(CameraRecords::ANA_AvgIntensity_RBV, avg_intensity.second);
	currentSnapshot.update(CameraRecords::ANA_Intensity_RBV, sum_intensity.second);
	currentSnapshot.update("busy", busy);
	currentSnapshot.update("max_shots_number", max_shots_number);
	currentSnapshot.update("last_capture_and_save_success", last_capture_and_save_success);
	currentSnapshot.update("aliases", GlobalFunctions::toString(aliases));
	currentSnapshot.update("screen_names", GlobalFunctions::toString(screen_names));
	currentSnapshot.update("average_pixel_value_for_beam", average_pixel_value_for_beam);
	currentSnapshot.update("array_data_num_pix_x", array_data_num_pix_x);
	currentSnapshot.update("array_data_num_pix_y", array_data_num_pix_y);
	currentSnapshot.update("array_data_pixel_count", array_data_pixel_count);
	currentSnapshot.update("binary_num_pix_x", binary_num_pix_x);
	currentSnapshot.update("binary_num_pix_y", binary_num_pix_y);
	currentSnapshot.update("binary_data_pixel_count", binary_data_pixel_count);
	currentSnapshot.update("roi_total_pixel_count", roi_total_pixel_count);
	currentSnapshot.update("operating_centre_x", operating_centre_x);
	currentSnapshot.update("operating_centre_y", operating_centre_y);
	currentSnapshot.update("mechanical_centre_x", mechanical_centre_x);
	currentSnapshot.update("mechanical_centre_y", mechanical_centre_y);
	currentSnapshot.update("pix2mmX_ratio", pix2mmX_ratio);
	currentSnapshot.update("pix2mmY_ratio", pix2mmY_ratio);
	currentSnapshot.update("roi_max_x", roi_max_x);
	currentSnapshot.update("roi_max_y", roi_max_y);
	currentSnapshot.update("x_pix_scale_factor", x_pix_scale_factor);
	currentSnapshot.update("y_pix_scale_factor", y_pix_scale_factor);
	currentSnapshot.update("x_mask_rad_max", x_mask_rad_max);
	currentSnapshot.update("y_mask_rad_max", y_mask_rad_max);
	currentSnapshot.update("use_mask_rad_limits", use_mask_rad_limits);
	currentSnapshot.update("sensor_max_temperature", sensor_max_temperature);
	currentSnapshot.update("sensor_min_temperature", sensor_min_temperature);
	currentSnapshot.update("average_pixel_value_for_beam", average_pixel_value_for_beam);
	currentSnapshot.update("min_x_pixel_pos", min_x_pixel_pos);
	currentSnapshot.update("max_x_pixel_pos", max_x_pixel_pos);
	currentSnapshot.update("min_y_pixel_pos", min_y_pixel_pos);
	currentSnapshot.update("max_y_pixel_pos", max_y_pixel_pos);
	currentSnapshot.update("mask_and_roi_keywords", GlobalFunctions::toString(mask_and_roi_keywords));
	currentSnapshot.update("mask_keywords", GlobalFunctions::toString(mask_keywords));
	currentSnapshot.update("roi_keywords", GlobalFunctions::toString(roi_keywords));

	return currentSnapshot;
}
boost::python::dict Camera::getSnapshot_Py()
{
	getSnapshot();
	return currentSnapshot.getSnapshot_Py();
}

std::map<std::string, double> Camera::getAnalayisData() const
{
	std::map < std::string, double> r;
	r["x_pix"] = getXPix();
	r["y_pix"] = getYPix();
	r["sigma_x_pix"] = getSigXPix();
	r["sigma_y_pix"] = getSigYPix();
	r["sigma_xy_pix"] = getSigXYPix();
	r["x_mm"] = getX();
	r["y_mm"] = getY();
	r["sigma_x_mm"] = getSigX();
	r["sigma_y_mm"] = getSigY();
	r["sigma_x_pix"] = getSigXPix();
	r["sigma_y_pix"] = getSigYPix();
	r["sigma_xy_pix"] = getSigXYPix();
	r["pix2mmX"] = getpix2mmX();
	r["pix2mmY"] = getpix2mmY();
	r["floor_state"] = getUseFloorState();
	r["floor_level"] = getFloorLevel();
	r["npoint_state"] = getNPointScalingState();
	r["step_size"] = getNpointScalingStepSize();
	r["using_background_state"] = getUsingBackgroundImageState();
	r["avg_intensity"] = getAvgIntensity();
	r["avg_intensity"] = getAvgIntensity();

	return r;
}
boost::python::dict Camera::getAnalayisData_Py() const
{
	//boost::python::dict r;
	//r["x_pix"] = getXPix();
	//r["y_pix"] = getYPix();
	//r["sigma_x_pix"] = getSigXPix();
	//r["sigma_y_pix"] = getSigYPix();
	//r["sigma_xy_pix"] = getSigXYPix();
	//r["x_mm"] = getX();
	//r["y_mm"] = getY();
	//r["sigma_x_mm"] = getSigX();
	//r["sigma_y_mm"] = getSigY();
	//r["sigma_x_pix"] = getSigXPix();
	//r["sigma_y_pix"] = getSigYPix();
	//r["sigma_xy_pix"] = getSigXYPix();
	//r["pix2mmX"] = getpix2mmX();
	//r["pix2mmY"] = getpix2mmY();
	//r["floor_state"] = getUseFloorState();
	//r["floor_level"] = getFloorLevel();
	//r["npoint_state"] = getNPointState();
	//r["npoint_step_size"] = getNpointStepSize();
	//r["using_background_state"] = getUsingBackgroundState();
	//r["sum_intensity"] = getSumIntensity();
	//r["avg_intensity"] = getAvgIntensity();

	return to_py_dict<std::string, double>(getAnalayisData());
}



bool Camera::genericStopAcquiringApplySetting(epics_caput_function_ptr f)
{
	/* is the camera currently acquiring and analyzing? */
	bool is_acquiring_at_start = isAcquiring();
	bool is_analyzing_at_start = isAnalysing();
	/* flag to store result of stop acquiring attempt */
	bool stopped_acquiring = !is_acquiring_at_start;
	//messenger.printDebugMessage(hardwareName, " is_acquiring_at_start = ", is_acquiring_at_start, ", is_analyzing_at_start = ", is_analyzing_at_start);
	if (is_acquiring_at_start)
	{
		//messenger.printDebugMessage("is_acquiring_at_start, therefore stopAcquiringAndWait");
		stopped_acquiring = stopAcquiringAndWait();
	}
	if (stopped_acquiring)
	{
		//messenger.printDebugMessage("stopAcquiringAndWait success, now calling passed member function ptr ");
		bool sent_use_floor = (*this.*f)(); // TODO use std::invoke ?? 
		//bool sent_use_floor = (*this.*f)();
		if (sent_use_floor)
		{
			if (is_acquiring_at_start)
			{
				//messenger.printDebugMessage("is_acquiring_at_start, startAcquiring");
				return startAcquiring();
			}
			return true;
		}
	}
	else
	{
		//messenger.printDebugMessage("!!Failed!! stopAcquiringAndWait");
	}
	return false;
}