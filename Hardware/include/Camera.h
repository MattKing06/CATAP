#ifndef CAMERA_H_
#define CAMERA_H_

#include <vector>
#include <Hardware.h>
#include <EPICSCameraInterface.h>
#include <GlobalConstants.h>
#include <GlobalStateEnums.h>
#include <boost/make_shared.hpp>
#include <boost/python/dict.hpp>
#include <boost/python/list.hpp>
#include "RunningStats.h"
#include <thread>



class Camera;
class ImageCapture;
/*
	Image collection and saving happens in a new thread,
	this struct is passed to the new thread function
*/
class ImageCapture
{
	public: 
		ImageCapture() :
			//camInterface(nullptr),
			cam(nullptr),
			thread(nullptr),
			num_shots(0),
			is_busy(false),
			status(STATE::ERR)
		{}
		/*
			https://stackoverflow.com/questions/4937180/a-base-class-pointer-can-point-to-a-derived-class-object-why-is-the-vice-versa
		*/
		//cameraBase* camInterface;
		Camera* cam;
		std::thread*   thread;
		size_t       num_shots;
		bool is_busy;
		STATE status;
};

class EPICSCameraInterface;

typedef boost::shared_ptr<EPICSCameraInterface> EPICSCameraInterface_sptr;

class Camera : public Hardware
{
public:
	Camera();
	Camera(const std::map<std::string, std::string>& paramMap, STATE mode);
	Camera(const Camera& copyCamera);
	~Camera();
	void setPVStructs();

	/*! get the name alises for this Camera
	@param[out] names, vector containing  all the alias names */
	std::vector<std::string> getAliases() const;
	/*! get the name alises for this Camera (python version)
		@param[out] names, python list containing all the alias names */
	boost::python::list getAliases_Py() const;
	/*! get the screen name (and aliases) the camera is attached to 
	@param[out] names, vector containing  all the screen names (and their aliases) */
	std::vector<std::string> getScreenNames() const;
	/*! get the screen name (and aliases) the camera is attached to (python version)
	@param[out] names, python list containing all the screen names (and their aliases) */
	boost::python::list getScreenNames_Py() const;
	/*! get the screen name the camera is attached to 
	@param[out] name, */
	std::string getScreen()const;

	/*! get the type of the camera (e.g. vela_camera, clara_camera 
	@param[out] type */
	TYPE getCamType()const;

	/*! convert a horizontal length in pixels to mm for this camera 
	@param[in] length in pixels 
	@param[out] length in mm*/
	double pix2mmX(double value)const;
	/*! convert a vertical length in pixels to mm for this camera
	@param[in] length in pixels
	@param[out] length in mm*/
	double pix2mmY(double value)const;
	/*! convert a horizontal length in mm to pixels for this camera
	@param[in] length in pixels
	@param[out] length in mm*/
	double mm2pixX(double value)const;
	/*! convert a vertical length in mm to pixels for this camera
	@param[in] length in pixels
	@param[out] length in mm*/
	double mm2pixY(double value)const;

	double getpix2mmX()const;
	double getpix2mmY()const;
	double setpix2mmX(double value);
	double setpix2mmY(double value);


	/*! Keywords used for setting ROI and mask together, python version (OUR PREFFERRED INTERFACE!) */
	boost::python::list mask_and_roi_keywords_Py;
	/*! Keywords used for setting mask, python version  (Not prefferred, use mask and ROI together) */
	boost::python::list mask_keywords_Py;
	/*! Keywords used for setting ROI , python version (Not preffeered, use mask and ROI together) */
	boost::python::list roi_keywords_Py;



	double getX()const;
	double getY()const;
	double getSigX()const;
	double getSigY()const;
	double getSigXY()const;
	double getXPix()const;
	double getYPix()const;
	double getSigXPix()const;
	double getSigYPix()const;
	double getSigXYPix()const;


	char getBufferTrigger()const;
	std::string getBufferFilePath()const;
	std::string getBufferFileName()const;
	long getBufferFileNumber()const;
	long getBufferROIminX()const;
	long getBufferROIminY()const;
	long getBufferROIsizeX()const;
	long getBufferROIsizeY()const;
	STATE getUseFloor()const;
	bool isUsingFloor()const;
	bool isNotUsingFloor()const;
	long getFLoorLevel()const;
	long getFLooredPtsCount()const;
	long getFLooredPtsPercent()const;
	long getCPUTotal()const;
	long getCPUCropSubMask()const;
	long getCPUNPoint()const;
	long getCPUDot()const;
	long getPixelWidth()const;
	long getPixelHeight()const;

	// THESE ARE JUST FOR ANLSYIS RESULTS WHEN USING THE VIRTUAL CLARA 
	bool setX(double value);
	bool setY(double value);
	bool setSigX(double value);
	bool setSigY(double value);
	bool setSigXY(double value);


	bool setBufferTrigger();
	bool setBufferROIminX(long v);
	bool setBufferROIminY(long v);
	bool setBufferROIsizeX(long v);
	bool setBufferROIsizeY(long v);
	bool setUseFloor();
	bool setDoNotUseFloor();
	bool setFLoorLevel(long v);









	double getSumIntensity()const;
	double getAvgIntensity()const;
	bool setSumIntensity(double value);
	bool setAvgIntensity(double value);

	void debugMessagesOn();
	void debugMessagesOff();
	void messagesOn();
	void messagesOff();

	// MASK and ROI 
	bool setMaskXCenter(long val);
	bool setMaskYCenter(long val);
	bool setMaskXRadius(long val);
	bool setMaskYRadius(long val);
	bool setMask(long x_pos, long  y_pos, long x_size, long y_size);
	bool setMask(std::map<std::string, long> settings);
	bool setMask_Py(boost::python::dict settings);
	long getMaskXCenter()const;
	long getMaskYCenter()const;
	long getMaskXRadius()const;
	long getMaskYRadius()const;
	std::map<std::string, long> getMask();
	boost::python::dict getMask_Py();



	bool setROIMinX(long val);
	bool setROIMinY(long val);
	bool setROISizeX(long val);
	bool setROISizeY(long val);
	bool setROI(long x_pos, long  y_pos, long x_size, long y_size);
	bool setROI(std::map<std::string, long> settings);
	bool setROI_Py(boost::python::dict settings);
	long getROIMinX()const;
	long getROIMinY()const;
	long getROISizeX()const;
	std::map<std::string, long> getROI();
	boost::python::dict getROI_Py();


	bool setMaskAndROIxPos(long val);
	bool setMaskAndROIyPos(long val);
	bool setMaskAndROIxSize(long val);
	bool setMaskAndROIySize(long val);

		
	bool setMaskandROI(long x_pos, long  y_pos, long x_size, long y_size);
	bool setMaskandROI(std::map<std::string, long> settings);
	bool setMaskandROI_Py(boost::python::dict settings);




	std::map<std::string, long> getMaskandROI()const;
	boost::python::dict getMaskandROI_Py()const;


	bool startAcquiring();
	bool stopAcquiring();
	bool isAcquiring()const;
	bool isNotAcquiring() const;
	STATE getAcquireState()const;

	bool startAnalysing();
	bool stopAnalysing();
	bool isAnalysing()const;
	bool isNotAnalysing() const;
	STATE getAnalysisState()const;

	bool captureAndSave(size_t num_images);
		
	STATE getCaptureState()const;
	bool isCapturing()const;
	bool isNotCapturing()const;

	bool isWriting()const;
	bool isNotWriting()const;

	/* after debugging move this to private */
	bool makeANewDirectoryAndName(size_t numbOfShots);

	size_t getBufferSize()const;

	
	void setBufferSize(size_t v);
	void clearBuffers();

	boost::python::dict getRunningStats()const;

	friend class EPICSCameraInterface;
	friend class CameraFactory;
protected:

	/* In general put data that comes beack from EPCIS into a timestamp, data pair */
	//std::pair<epicsTimeStamp, double > some_data;

	
	/*! latest horizontal position (expected value) in pixels. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, double > x_pix;
	/*! latest vertical position (expected value) in pixels. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, double > y_pix;
	/*! latest horizontal rms (width) in pixels. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, double > sigma_x_pix;
	/*! latest vertical rms (width) in pixels. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, double > sigma_y_pix;
	/*! latest tilt rms (width) in pixels. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, double > sigma_xy_pix;
	/*! latest horizontal position (expected value) in pixels. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, double > x_mm;
	/*! latest vertical position (expected value) in pixels. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, double > y_mm;
	/*! latest horizontal rms (width) in pixels. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, double > sigma_x_mm;
	/*! latest vertical rms (width) in pixels. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, double > sigma_y_mm;
	/*! latest tilt rms (width) in pixels. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, double > sigma_xy_mm;

	 

	size_t running_stats_buffer_size;

	RunningStats x_pix_rs;
	RunningStats y_pix_rs;
	RunningStats sigma_x_pix_rs;
	RunningStats sigma_y_pix_rs;
	RunningStats sigma_xy_pix_rs;
	RunningStats x_mm_rs;
	RunningStats y_mm_rs;
	RunningStats sigma_x_mm_rs;
	RunningStats sigma_y_mm_rs;
	RunningStats sigma_xy_mm_rs;



	/*! latest Sum Intensity (sum of pixel values). Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, double > sum_intensity;
	/*! latest Average Intensity (mean of pixel values). Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, double > avg_intensity;

	/*! analaysis mask center x position (pixels). Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, long> mask_x_center;
	/*! analaysis mask center y position (pixels). Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, long> mask_y_center;
	/*! analaysis mask center x radius (pixels). Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, long> mask_x_radius;
	/*! analaysis mask center y radius (pixels). Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, long> mask_y_radius;
	/*! Rectangular ROI X corner, ROI used to extract masked data. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, long> roi_min_x;
	/*! Rectangular ROI Y corner, ROI used to extract masked data. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, long> roi_min_y;
	/*! Rectangular ROI X size, ROI used to extract masked data. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, long> roi_size_x;
	/*! Rectangular ROI Y size, ROI used to extract masked data. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, long> roi_size_y;
	/*! ROI image data. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, std::vector<long>> roi_imagedata;

	/*! analaysis center x NOT SURE WHAT THIS IS YET. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, double > x_center;
	/*! analaysis center y NOT SURE WHAT THIS IS YET. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, double > y_center;
	/*! conversion factor fro pixels to mm. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, double > pix_to_mm;

	/*! Camera acquire time. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, double > acquire_time;
	/*! Camera acquire period. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, double > acquire_period;
	/*! Camera array rate. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, double > array_rate;
	/*! Camera temperature. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, double > temperature;
	/*! LED status. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, STATE> led_status;
	/*! LED status. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, STATE> acquire_status;
	/*! Analysis status. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, STATE> analysis_status;
	/*! Capture status (is the Cmaera "busy" capturing images). Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, STATE> capture_state;

	/*! Write status (is the camera writing or not writing data to disc). Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, STATE> write_state;

	/*! Write status check (did the last write operation succesfully complete). Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, STATE> write_state_check;

	/*! Write status error message (did the last write operation succesfully complete). Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, std::string> write_error_message;

	
	/*! Latest directory to write image data to. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, std::string> save_filepath;

	/*! Latest filename to write image data to. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, std::string> save_filename;
		
	/*! Camera circular buffer trigger PROC. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, char> buffer_trigger;
	/*! Latest filepath to write camera buffer data to. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, std::string> buffer_filepath;
	/*! Latest filename to write camera buffer data to. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, std::string> buffer_filename;
	/*! Latest file number for camera buffer data to. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, long> buffer_filenumber;

	/*! Rectangular ROI AND Mask X centre. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, double> roi_and_mask_centre_x; // MAY NOT NEED AS MONITORS
	/*! Rectangular ROI AND Mask Y centre. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, double> roi_and_mask_centre_y; // MAY NOT NEED AS MONITORS
	/*! Rectangular ROI AND Mask X radius. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, double> roi_and_mask_radius_x;
	/*! Rectangular ROI AND Mask Y radius. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, double> roi_and_mask_radius_y;
	/*! Image analysis is floor being used or not. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, STATE> use_floor;
	/*! Image analysis  Floor count level. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, long> floor_level;
	/*! Number of points rejected by floor. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, long> floored_pts_count;
	/*! Percentage of points rejected by floor. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, double> floored_pts_percent;
	/*! Total CPU usage (units??). Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, long> cpu_total;
	/*! CPU use by crop, subtract and mask (units??). Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, long> cpu_crop_sub_mask;
	/*! CPU use by NPoint analysis (units??). Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, long> cpu_npoint;
	/*! CPU use by dot product (units??). Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, long> cpu_dot;
	/*! Full Image width in pixels. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, long> pixel_width;
	/*! Full Image height in pixels. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, long> pixel_height;


private:

	TYPE cam_type;

	/*! alternative names for the magnet (usually shorter thna the full PV root),
	defined in the master lattice yaml file	*/
	std::vector<std::string> aliases;
	std::vector<std::string> screen_names;


	void imageCaptureAndSave(size_t num_shots);
	void killFinishedImageCollectThread();

	static void staticEntryImageCollectAndSave(ImageCapture& ic);

	double pix2mmX_ratio;
	double pix2mmY_ratio;

	size_t max_shots_number;

	ImageCapture image_capture;

	std::string requested_directory;
	std::string requested_filename;


	// collect and save stuff 
	//void captureAndSaveStaticEntry(size_t num_images);
	bool setNumberOfShotsToCapture(size_t num);
	bool capture();
	bool write();
	
	bool busy;
	bool last_capture_and_save_success;

	EPICSCameraInterface_sptr epicsInterface;
	std::map<std::string, std::string> CameraParamMap;

	
	/*! Keywords used for setting ROI and mask together (OUR PREFFERRED INTERFACE!) */
	std::vector<std::string> mask_and_roi_keywords;
	/*! Keywords used for setting mask (Not prefferred, use mask and ROI together) */
	std::vector<std::string> mask_keywords;
	/*! Keywords used for setting ROI (Not preffeered, use mask and ROI together) */
	std::vector<std::string> roi_keywords;



};


#endif //RF_HEARTBEAT_H_