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
	/*! get the screen name the camera is attached to (defined in master lattice)
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

	/*! Get the pixels to mm in x(horizontal) direction. (First set from the yaml config file) 
	@param[out] double, value */
	double getpix2mmX()const;
	/*! Get the pixels to mm in y (horizontal) direction. (First set from the yaml config file)
	@param[out] double, value */
	double getpix2mmY()const;
	/*! Set the pixels to mm in x(horizontal) direction. 
	@param[in] double, new value */
	double setpix2mmX(double value);
	/*! Set the pixels to mm in y(horizontal) direction.
	@param[in] double, new value */
	double setpix2mmY(double value);


	/*! Keywords used for setting ROI and mask together, python version (OUR PREFFERRED INTERFACE!) */
	boost::python::list mask_and_roi_keywords_Py;
	/*! Keywords used for setting mask, python version  (Not prefferred, use mask and ROI together) */
	boost::python::list mask_keywords_Py;
	/*! Keywords used for setting ROI , python version (Not preffeered, use mask and ROI together) */
	boost::python::list roi_keywords_Py;


	// THESE ARE JUST FOR ANLSYIS RESULTS WHEN USING THE VIRTUAL CLARA 
	/*! Set the x position in mm from the online analysis (only available VIRTUAL mode).
	@param[in] double, value 
	@param[out] bool, true if value set correctly, false could mean you are trying to set a value for PHYSICAL CLARA   */
	bool setX(double value);
	/*! Set the y position in mm for the online analysis (only available VIRTUAL mode).
	@param[in] double, value
	@param[out] bool, true if value set correctly, false could mean you are trying to set a value for PHYSICAL CLARA   */
	bool setY(double value);
	/*! Set the x width in mm for the online analysis (only available VIRTUAL mode).
	@param[in] double, value
	@param[out] bool, true if value set correctly, false could mean you are trying to set a value for PHYSICAL CLARA   */
	bool setSigX(double value);
	/*! Set the y width mm for the online analysis (only available VIRTUAL mode).
	@param[in] double, value
	@param[out] bool, true if value set correctly, false could mean you are trying to set a value for PHYSICAL CLARA   */
	bool setSigY(double value);
	/*! Set the x-y correlation in mm for the online analysis (only available VIRTUAL mode).
	@param[in] double, value
	@param[out] bool, true if value set correctly, false could mean you are trying to set a value for PHYSICAL CLARA   */
	bool setSigXY(double value);
	/*! Get the x position in mm from the online analysis.
	@param[out] double, value */
	double getX()const;
	/*! Get the y position in mm from the online analysis.
	@param[out] double, value */
	double getY()const;
	/*! Get the x width in mm from the online analysis.
	@param[out] double, value */
	double getSigX()const;
	/*! Get the y width in mm from the online analysis.
	@param[out] double, value */
	double getSigY()const;
	/*! Get the x-y coviariance in mm from the online analysis.
	@param[out] double, value */
	double getSigXY()const;
	/*! Get the x position in pixels from the online analysis.
	@param[out] double, value */
	double getXPix()const;
	/*! Get the y position in pixels from the online analysis.
	@param[out] double, value */
	double getYPix()const;
	/*! Get the x width in pixels from the online analysis.
	@param[out] double, value */
	double getSigXPix()const;
	/*! Get the y width in pixels from the online analysis.
	@param[out] double, value */
	double getSigYPix()const;
	/*! Get the x-y coviariance in pixels from the online analysis.
	@param[out] double, value */
	double getSigXYPix()const;


	/*! Get the buffer trigger value.
	@param[out] double, value */
	char getBufferTrigger()const;
	/*! Get the buffer dump file-path.
	@param[out] double, value */
	std::string getBufferFilePath()const;
	/*! Get the buffer dump file-name.
	@param[out] double, value */
	std::string getBufferFileName()const;
	/*! Get the buffer dump file-number.
	@param[out] double, value */
	long getBufferFileNumber()const;

	// TODO sort this out and neaten it
	///*! Get the buffer dump file-path.
	//@param[out] long, value */
	//long getBufferROIminX()const;
	///*! Get the buffer dump file-path.
	//@param[out] long, value */
	//long getBufferROIminY()const;
	///*! Get the buffer dump file-path.
	//@param[out] long, value */
	//long getBufferROIsizeX()const;
	///*! Get the buffer dump file-path.
	//@param[out] long, value */
	//long getBufferROIsizeY()const;

	/*! Get the total CPU usage of the camera.
	@param[out] long, value */
	long getCPUTotal()const;
	/*! Get the CPU usage of the CropSubMask procedure.
	@param[out] long, value */
	long getCPUCropSubMask()const;
	/*! Get the CPU usage of the NPoint scaling procedure.
	@param[out] long, value */
	long getCPUNPoint()const;
	/*! Get the CPU usage of the dot product procedure.
	@param[out] long, value */
	long getCPUDot()const;
	/*! Get the Number of pixels in the width of the (full) image.
	@param[out] long, value */
	long getPixelWidth()const;
	/*! Get the Number of pixels in the height of the (full) image.
	@param[out] long, value */
	long getPixelHeight()const;

	/*! Get the Acquire Time for the camera (shutter open time, units??).
	@param[out] double, value */
	double getAcquireTime()const;
	/*! Get the Acquire Period for the camera (shutter open time, units??).
	@param[out] double, value */
	double getAcquirePeriod()const;
	/*! Get the Array Rate for the camera (repetition rate, Hz).
	@param[out] double, value */
	double getArrayRate()const;
	/*! Get the Sensor Temperature for the camera (degrees Celsius).
	@param[out] double, value */
	double getTemperature()const;


	/*! Set the buffer trigger to dump the camera image buffer to disc.
	@param[out] bool, true if value got sent to epics (not if it was received)*/
	bool setBufferTrigger();

	/*! Set the Region Of Interest minimum x pixel.
	@param[in] long, new val
	@param[out] bool, true if value got sent to epics (not if it was received)*/
	bool setROIMinX(long val);
	/*! Set the Region Of Interest minimum y pixel.
	@param[in] long, new val
	@param[out] bool, true if value got sent to epics (not if it was received)*/
	bool setROIMinY(long val);
	/*! Set the Region Of Interest size in x pixels.
	@param[in] long, new val
	@param[out] bool, true if value got sent to epics (not if it was received)*/
	bool setROISizeX(long val);
	/*! Set the Region Of Interest size in y pixels.
	@param[in] long, new val 
	@param[out] bool, true if value got sent to epics (not if it was received)*/
	bool setROISizeY(long val);
	/*! Set the 4 Region Of Interest size in y pixels.
	@param[in] long, new x_pos value
	@param[in] long, new y_pos value 
	@param[in] long, new x_size value
	@param[in] long, new y_size value
	@param[out] bool, true if value got sent to epics (not if it was received)*/
	bool setROI(long x, long  y, long x_size, long y_size);
	/*! Set the Region Of Interest parameters.
	@param[in] map<std::string, long>, with new values (for keywords see roi_keywords)
	@param[out] bool, true if value got sent to epics (not if it was received)*/
	bool setROI(std::map<std::string, long> settings);
	/*! Set the Region Of Interest parameters. 
	@param[in] long, dict with new values, (for keywords see roi_keywords)
	@param[out] bool, true if value got sent to epics (not if it was received)*/
	bool setROI_Py(boost::python::dict settings);
	/*! Get the Region Of Interest minimum x pixel.
	@param[out] long, value*/
	long getROIMinX()const;
	/*! Get the Region Of Interest minimum y (pixels).
	@param[out] long, value*/
	long getROIMinY()const;
	/*! Get the Region Of Interest minimum x (pixels).
	@param[out] long, value*/
	long getROISizeX()const;
	/*! Get the Region Of Interest minimum y (pixels).
	@param[out] long, value*/
	long getROISizeY()const;
	/*! Get the Region Of Interest values.
	@param[out] map<std::string, long>, with new values  */
	std::map<std::string, long> getROI();
	/*! Get the Region Of Interest values.
	@param[out] dict, with new values  */
	boost::python::dict getROI_Py();

	// FLOOR Set get etc ... 
	/*! Set the using a Floor during image analysis to true .
	@param[out] bool, true if value got sent to epics (not if it was received)*/
	bool setUseFloor();
	/*! Set the using a Floor during image analysis to false.
	@param[out] bool, true if value got sent to epics (not if it was received)*/
	bool setDoNotUseFloor();
	/*! Set the Floor level to use during image analysis (value below floor get set to zero).
	@param[in] long, value to use 
	@param[out] bool, true if value got sent to epics (not if it was received)*/
	bool setFloorLevel(long v);
	/*! Get the state of use_floor.
	@param[in] long, value to use
	@param[out] bool, true if value got sent to epics (not if it was received)*/
	STATE getUseFloorState()const;
	/*! Return true if use_floor == STATE::USING_FLOOR, otherwise false 
	@param[out] bool, value*/
	bool isUsingFloor()const;
	/*! Return true if use_floor == STATE::NOT_USING_FLOOR, otherwise false
	@param[out] bool, value*/
	bool isNotUsingFloor()const;
	
	/*! get the level of the analysis floor 
	@param[out] long, value*/
	long getFloorLevel()const;
	/*! get the number of pixels that have been floored 
	@param[out] long, value*/
	long getFlooredPtsCount()const;
	/*! get the percentage of pixels that have been floored
	@param[out] long, value*/
	double getFlooredPtsPercent()const;


	/*! set use Npoint scaling or not 
	@param[in] bool, true for use NPoint, False for do not use NPoint 
	@param[out] bool, true if value set correctly, false could mean you are trying to set a value for PHYSICAL CLARA   */
	bool useNPoint(bool v);
	/*! get the use Npoint scaling state
	@param[out] STATE, */
	STATE getNPointState()const;
	/*! check if analysis is using NPoint scaling
	@param[out] bool, true if using NPoint scaling*/
	bool isUsingNPoint()const;
	/*! check if analysis is Not using NPoint scaling
	@param[out] bool, true if using NPoint scaling*/
	bool isNotUsingNPoint()const;

	/*! set use the background image during the analysis  procedure 
	@param[in] bool, true to use the background, False to not use the background 
	@param[out] bool, true if value set correctly, false could mean you are trying to set a value for PHYSICAL CLARA   */
	bool useBackground(bool v);
	/*! get the use background state
	@param[out] STATE, */
	STATE getUsingBackgroundState()const;
	/*! check if analysis is using a background data
	@param[out] bool, true if using background image during analysis */
	bool isUsingBackground()const;
	/*! check if analysis is Not using a background data
	@param[out] bool, true if NOT  using background image during analysis*/
	bool isNotUsingBackground()const;

	/*! get the latest pixel sum for the image
	@param[out] long, value */
	long getStepSize()const;
	/*! set the Npoint scaling stepsize 
	@param[in] long, new stepsize
	@param[out] bool, value */
	bool setStepSize(long val);

	/*! get the latest pixel sum for the image 
	@param[out] double, value */
	double getSumIntensity()const;

	/*! get the latest pixel average for the image
	@param[out] double, value */
	double getAvgIntensity()const;

	/*! Set the latest pixel sum for the image (Only available when NOT in PHYSICAL mode)
	@param[in] double, value 
	@param[out] bool, value */
	bool setSumIntensity(double value);
	/*! Set the latest pixel average for the image (Only available when NOT in PHYSICAL mode)
	@param[in] double, value 
	@param[out] bool, value */
	bool setAvgIntensity(double value);

	/*! Get the last directory / filename that HDF5 data was saved to.
	@param[out] string, value */
	std::string getLastDirectoryandFileName() const;
	/*! Get the last directory that HDF5 data was saved to.
	@param[out] string, value */
	std::string getLastDirectory()const;
	/*! Get the last filename that HDF5 data was saved to.
	@param[out] string, value */
	std::string getLastFileName()const;

	/*! Get the last directory / filename that the HDF5 buffer data was saved to.
	@param[out] string, value */
	std::string getLastBufferDirectoryandFileName() const;
	/*! Get the last directory that the HDF5 buffer data was saved to.
	@param[out] string, value */
	std::string getLastBufferDirectory()const;
	/*! Get the last filename that the HDF5 buffer data was saved to.
	@param[out] string, value */
	std::string getLastBufferFileName()const;


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

	bool setMaskAndROIxPos(long val);
	bool setMaskAndROIyPos(long val);
	bool setMaskAndROIxSize(long val);
	bool setMaskAndROIySize(long val);

	long getMaskAndROIxPos()const;
	long getMaskAndROIyPos()const;
	long getMaskAndROIxSize()const;
	long getMaskAndROIySize()const;

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

	bool isCapturingOrSaving()const;

	bool setLEDOn();
	bool setLEDOff();
	bool isLEDOn()const;
	bool isLEDOff()const;
	STATE getLEDState()const;


	bool updateImageData();
	bool updateImageDataWithTimeStamp();
	bool updateROIData();
	bool updateROIDataWithTimeStamp();

	std::vector<long> getImageData();
	boost::python::list getImageData_Py();
	std::vector<long> getROIData();
	boost::python::list getROIData_Py();


	std::vector<long>& getImageDataConstRef();
	std::vector<long>& getROIDataConstRef();

	






	/* after debugging move this to private */
	bool makeANewDirectoryAndName(size_t numbOfShots);

	size_t getBufferSize()const;

	double getPix2mm()const;
		
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
		
	size_t num_pixels;

	//std::pair<epicsTimeStamp, boost::python::list> test_list;
	
	
	/*! background image data used in analysis. Value and epicstimestamp.	*/
	//std::pair<epicsTimeStamp, std::vector<long>> background_image_data;

	/*! Region of Interes image data. Value and epicstimestamp.	*/
	//std::pair<epicsTimeStamp, std::vector<long>> roi_array_data;
	//std::pair<epicsTimeStamp, boost::python::list> roi_array_data_py;


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
	std::pair<epicsTimeStamp, STATE > use_npoint;
	/*! latest stepsize (Npoint scaling step size). Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, long > step_size;

	/*! latest Average Intensity (mean of pixel values). Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, STATE > use_background;


	/*! latest Sum Intensity (sum of pixel values). Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, double > sum_intensity; // just intensity in pv name
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
	std::pair<epicsTimeStamp, STATE> led_state;
	/*! Acquire status. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, STATE> acquire_state;
	/*! Analysis status. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, STATE> analysis_state;
	/*! Capture status (is the Cmaera "busy" capturing images). Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, STATE> capture_state;

	/*! Write status (is the camera writing or not writing data to disc). Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, STATE> write_state;

	/*! Write status check (did the last write operation succesfully complete). Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, STATE> write_state_check;
	/*! Write status error message (did the last write operation succesfully complete). Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, std::string> write_error_message;
	
	/*! Latest directory to write image data to disc. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, std::string> save_filepath;
	/*! Latest filename to write image data to disc. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, std::string> save_filename;
	/*! Latest filenumber when write image data to disc. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, long> save_filenumber;

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

	/*! Full Image height in pixels. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, double> pixel_to_mm;

	/*! Number of images to capture. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, long> num_capture;


	struct dbr_time_long* dbr_image_data;
	struct dbr_time_long* dbr_roi_data;

	std::pair<epicsTimeStamp, boost::python::list> image_data_py;
	std::pair<epicsTimeStamp, boost::python::list> roi_array_data_py;

	/*! Number of images to capture. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, std::vector<long>> image_data;
	std::pair<epicsTimeStamp, std::vector<long>> roi_data;


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


	/*! pointer to an epics dbr_time_long. This is *needed* as a class variable so that 
	ca_array_get has a pointer to pass back image array data to*/
	//dbr_time_long* pointer_to_array_data;

	static void updateROIArrayData(struct  event_handler_args args);

	bool caArrayGetImageDtata(const std::string& record, unsigned count,
		struct dbr_time_long* array_struct);
	//bool updateArrayData(std::pair<epicsTimeStamp, std::vector<long>>& pair_to_update, const event_handler_args& args);
	bool updateArrayData(std::pair<epicsTimeStamp, boost::python::list>& pair_to_update, const event_handler_args& args);
	//void updateArrayDataTEST(struct event_handler_args args);
	//void(Camera::*updateArrayDataTEST_pointer)(struct event_handler_args);


	bool getArrayTimeStamp(struct dbr_time_long* dbr_struct, 
						  const pvStruct& pvs, epicsTimeStamp& ts_to_update);
	bool getArrayValue(std::vector<long>& data_vec, const pvStruct& pvs, size_t count);

	// some flags and functions to resevere memory when it is needed (not automatically!) 
	bool vector_resize(std::vector<long>& vec);
	//bool malloc_reserve(struct dbr_time_long& vec);
	void malloc_imagedata();
	void malloc_roidata();

	bool image_data_has_not_malloced;
	bool image_data_has_not_vector_resized;
	bool roi_data_has_not_malloced;
	bool roi_data_has_not_vector_resized;

};


#endif //RF_HEARTBEAT_H_