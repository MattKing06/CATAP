#ifndef CAMERA_H_
#define CAMERA_H_

#include <vector>
#ifndef HARDWARE_H_
#include "Hardware.h"
#endif //HARDWARE_H_

#ifndef EPICS_CAMERA_INTERFACE_H_ // TODO why do we need an include gaurd here??? 
#include "EPICSCameraInterface.h"
#endif //EPICS_CAMERA_INTERFACE_H_
#include <boost/shared_ptr.hpp>
#include <vector>
#include "GlobalConstants.h"
#include "GlobalTypeEnums.h"

#include "CameraPVRecords.h"
#include <boost/make_shared.hpp>
#include <boost/python/dict.hpp>
#include <boost/python/list.hpp>
#include "RunningStats.h"
#include <thread>

class Camera;
class ImageCapture;
/*	Image collection and saving happens in a new thread,
	this struct is passed to the new thread function		*/
class ImageCapture
{
	public: 
		ImageCapture() :
			cam(nullptr), thread(nullptr), num_shots(0),
			is_busy(false),status(STATE::ERR){}
		/*
			https://stackoverflow.com/questions/4937180/a-base-class-pointer-can-point-to-a-derived-class-object-why-is-the-vice-versa
		*/
		Camera* cam;
		std::thread*   thread;
		size_t       num_shots;
		bool is_busy;
		STATE status;
};

class EPICSCameraInterface;
typedef boost::shared_ptr<EPICSCameraInterface> EPICSCameraInterface_sptr;
/* main camera object */
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
	// TODO these all need tidying up / renaming or deleting 
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
	/*! Get the pixels to mm in y (vertical) direction. (First set from the yaml config file)
	@param[out] double, value */
	double getpix2mmY()const;
	/*! Set the pixels to mm in x(horizontal) direction. 
	@param[in] double, new value */
	double setpix2mmX(double value);
	/*! Set the pixels to mm in y(vertical) direction.
	@param[in] double, new value */
	double setpix2mmY(double value);

	/*! Get the pixels to mm value (the one that is actaully used in EPICS).
	@param[out] double, value */
	double getPixelToMM()const;
	/*! Set the pixels to mm value (the one that is actaully used in EPICS).
	@param[in] double, value 
	@param[out] bool, if command got sent to EPICS (not if it was received). */
	bool setPixelToMM(double val)const;


	/*! Get the number of pixels in the x direction for the "standard" image array sent over the network
	@param[out] size_t, value*/
	size_t getArrayDataPixelCountX()const;
	/*! Get the number of pixels in the y direction for the "standard" image array sent over the network.
	@param[out] size_t, value*/
	size_t getArrayDataPixelCountY()const;

	/*! Get the number of pixels in the x direction for the full image array saved to disk.
	@param[out] size_t, value*/
	size_t getBinaryDataPixelCountX()const;
	/*!  Get the number of pixels in the x direction for the full image array saved to disk.
	@param[out] size_t, value*/
	size_t getBinaryDataPixelCountY()const;


	// THESE ARE JUST FOR ANLAYSIS RESULTS WHEN USING VIRTUAL CLARA 
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
	/*! Get the x pixel scale factor 
	@param[out] size_t, value*/
	size_t getXPixScaleFactor() const;
	/*! Get the y pixel scale factor
	@param[out] size_t, value*/
	size_t getYPixScaleFactor() const;
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
	/*!*/
	size_t getNumXPixFromArrayData() const;
	/*!*/
	size_t getNumYPixFromArrayData() const;
	/*!*/
	size_t getFullNumXPix() const;
	/*!*/
	size_t getFullNumYPix() const;

	/*! Clear all the values assoociated with the Running mean and variance stats.*/
	void clearAllRunningStats();
	/*! Set the size of all Running mean and variance stats.*/
	void setAllRunningStatSizes(size_t new_val);


	// TODO this is the IMAGE buffer, we (will) also have data buffers conatined in runningstats 
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

	/*! Get relevant analysis data in a map 
	@param[out] std::map<std::string, double> */
	std::map<std::string, double> getAnalayisData() const;
	boost::python::dict getAnalayisData_Py() const;


	/*! get Anlaysis results in pixels 
	@param[out] map<string, double>, values, keyed by names in master lattice */
	std::map<std::string, double> getAnalysisResultsPixels()const;
	/*! get Anlaysis results in pixels, Python version
	@param[out] map<string, double>, values, keyed by names in master lattice */
	boost::python::dict getAnalysisResultsPixels_Py()const;


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

	/*! Get the PixelResults from analysis. The values expected back are in the following order:
		[0] - X_POSITION
		[1] - Y_POSITION
		[2] - X_SIGMA
		[3] - Y_SIGMA
		[4] - COVARIANCE POSITION
		@param[out] vector<double> results : [x_pos, y_pos, x_sig, y_sig, cov]*/
	std::vector<double> getPixelResults() const;
	boost::python::list getPixelResults_Py();
	/*! Get the mm Results from analysis. The values expected back are in the following order:
	[0] - X_POSITION
	[1] - Y_POSITION
	[2] - X_SIGMA
	[3] - Y_SIGMA
	[4] - COVARIANCE 
	@param[out] vector<double> results : [x_pos, y_pos, x_sig, y_sig, cov]*/
	std::vector<double> getMMResults() const;
	boost::python::list getMMResults_Py() ;

	///*! Enable the Cross-Overlay on the decimated camera image array sent over the network. 
	//* Will be visiable to many camera image viewing apps, 
	//@param[out] bool, true if value got sent to epics (not if it was received)*/
	//bool enableOverlayCross();
	///*! Disable the Cross-Overlay on the decimated camera image array sent over the network. 	
	//* The overlay will be visable to many camera image viewing apps,
	//@param[out] bool, true if value got sent to epics (not if it was received)*/
	//bool disableOverlayCross();
	///*! Get the state of Cross-Overlay on the decimated camera image array sent over the network.
	//@param[out] STATE, current state of overla, ENABLED, DISBALED, */
	//STATE getOverlayCrossState()const;
	///*! Compare the current state of the cross overlay to ENABLED.
	//@param[out] bool, returns true if the overlay is ENABLED,  otherwise false.*/
	//bool isOverlayCrossEnabled()const;
	///*! Compare the current state of the cross overlay to DISABLED.
	//@param[out] bool, returns true if the overlay is DISABLED,  otherwise false.*/
	//bool isOverlayCrossDisabled()const;
	///*! Enable the Mask-Overlay on the decimated camera image array sent over the network.
	//* Will be visiable to many camera image viewing apps,
	//@param[out] bool, true if value got sent to epics (not if it was received)*/
	//bool enableOverlayMask();
	///*! Disable the Mask-Overlay on the decimated camera image array sent over the network.
	//* The overlay will be visable to many camera image viewing apps,
	//@param[out] bool, true if value got sent to epics (not if it was received)*/
	//bool disableOverlayMask();
	///*! Get the state of Cross-Overlay on the decimated camera image array sent over the network.
	//@param[out] STATE, current state of overla, ENABLED, DISBALED, */
	//STATE getOverlayMaskState()const;
	///*! Compre the current state of the Mask-Overlay to ENABLED.
	//@param[out] bool, returns true if the overlay is ENABLED,  otherwise false.*/
	//bool isOverlayMaskEnabled()const;
	///*! Compare the current state of the Mask-Overlay to DISABLED.
	//@param[out] bool, returns true if the overlay is DISABLED,  otherwise false.*/
	//bool isOverlayMaskDisabled()const;
	///*! Enable the Results-Overlay on the decimated camera image array sent over the network.
	//* Will be visiable to many camera image viewing apps,
	//@param[out] bool, true if value got sent to epics (not if it was received)*/
	//bool enableOverlayResult();
	///*! Disable the Results-Overlay on the decimated camera image array sent over the network.
	//* The overlay will be visable to many camera image viewing apps,
	//@param[out] bool, true if value got sent to epics (not if it was received)*/
	//bool disableOverlayResult();
	///*! Get the state of Results-Overlay on the decimated camera image array sent over the network.
	//@param[out] STATE, current state of overla, ENABLED, DISBALED, */
	//STATE getOverlayResultState()const;
	///*! Comapre the current state of the Results-Overlay to ENABLED.
	//@param[out] bool, returns true if the overlay is ENABLED,  otherwise false.*/
	//bool isOverlayResultEnabled()const;
	///*! Compare the current state of the Results-Overlay to DISABLED.
	//@param[out] bool, returns true if the overlay is DISABLED,  otherwise false.*/
	//bool isOverlayResultDisabled()const;


	
	
	
	
	/*! Check if the image analysis results are updating.
	@param[out] bool, ture if updating */
	bool isAnalysisUpdating();
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
	/*! Set the Region Of Interest minimum y pixel, !!WARNING!! This function does not move the analysis MASK and should only be used by experts. 
	@param[in] long, new val
	@param[out] bool, true if value got sent to epics (not if it was received)*/
	bool setROIMinY(long val);
	/*! Set the Region Of Interest size in x pixels, !!WARNING!! This function does not move the analysis MASK and should only be used by experts 
	@param[in] long, new val
	@param[out] bool, true if value got sent to epics (not if it was received)*/
	bool setROISizeX(long val);
	/*! Set the Region Of Interest size in y pixels, !!WARNING!! This function does not move the analysis MASK and should only be used by experts
	@param[in] long, new val 
	@param[out] bool, true if value got sent to epics (not if it was received)*/
	bool setROISizeY(long val);
	/*! Set the 4 Region Of Interest size in y pixels, !!WARNING!! This function does not move the analysis MASK and should only be used by experts
	@param[in] long, new x_pos value
	@param[in] long, new y_pos value 
	@param[in] long, new x_size value
	@param[in] long, new y_size value
	@param[out] bool, true if value got sent to epics (not if it was received)*/
	bool setROI(long x, long  y, long x_size, long y_size);
	/*! Set the Region Of Interest parameters, !!WARNING!! This function does not move the analysis MASK and should only be used by experts
	@param[in] map<std::string, long>, with new values (for keywords see roi_keywords)
	@param[out] bool, true if value got sent to epics (not if it was received)*/
	bool setROI(std::map<std::string, long> settings);
	/*! Set the Region Of Interest parameters, !!WARNING!! This function does not move the analysis MASK and should only be used by experts
	@param[in] long, dict with new values, (for keywords see roi_keywords)
	@param[out] bool, true if value got sent to epics (not if it was received)*/
	bool setROI_Py(boost::python::dict settings);
	/*! Get the Region Of Interest minimum x pixel.
	@param[out] long, value*/
	long getROIMinX()const;
	/*! Get the Region Of Interest minimum y (pixels).
	@param[out] long, value*/
	long getROIMinY()const;
	/*! Get the Region Of Interest x size (pixels).
	@param[out] long, value*/
	long getROISizeX()const;
	/*! Get the Region Of Interest y size (pixels).
	@param[out] long, value*/
	long getROISizeY()const;
	/*! Get the Region Of Interest values.
	@param[out] map<std::string, long>, with new values  */
	std::map<std::string, long> getROI()const;
	/*! Get the Region Of Interest values.
	@param[out] dict, with new values  */
	boost::python::dict getROI_Py()const;
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
	/*! Get the state of the Flag to set the next image as background
	@param[out] STATE, the STATE of teh set new background flag, YES or NO  */
	STATE getSetNewBackgroundState();
	/*! Flag to set the next image as background 
	@param[out] bool, true if using NPoint scaling*/
	bool setNewBackground(bool v);
	/*! Flag to set the next image as background
	@param[out] bool, true if using NPoint scaling*/
	bool getSetNewBackground(bool v);
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
	/*! get the Npoint scaling stepsize 
	@param[out] long, value */
	long getNpointStepSize()const;
	/*! set the Npoint scaling stepsize 
	@param[in] long, new stepsize
	@param[out] bool, value */
	bool setNpointStepSize(long val);
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
	/*! Get the average pixel value that determines if beam is present in image
	@param[out] double, average_pixel_value_for_beam*/
	double getAveragePixelValueForBeam()const;
	/*! Set the average pixel value that determines if beam is present in image
	@param[in] double, averagePixelValueForBeam
	@param[out] bool, isSetSucessfully*/
	bool setAveragePixelValueForBeam(const double& value);
	/*! Determines if beam is present in image data using AveragePixValue and AveragePixValueForBeam
	@param[out]: bool, true if AveragePixValue > AveragePixValueForBeam*/
	bool hasBeam()const;
	/*! Determines if beam is present in image data using AveragePixValue (from online analysis) and AveragePixValueForBeam (defined in Prime Lattice)
	@param[out]: bool, true if AveragePixValue < AveragePixValueForBeam*/
	bool hasNoBeam()const;
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
	/*! Set the mask x center position, setting the Mask and ROI together is preferred using setMaskAndROIxPos
	@param[in] long, value 
	@param[out] bool, if command got sent to EPICS (not if it was accepted)	*/
	bool setMaskXCenter(long val);
	/*! Set the mask y center position, setting the Mask and ROI together is preferred using setMaskAndROIyPos
	@param[in] long, value
	@param[out] bool, if command got sent to EPICS (not if it was accepted)	*/
	bool setMaskYCenter(long val);
	/*! Set the mask x radius, setting the Mask and ROI together is preferred using setMaskAndROIxSize
	@param[in] long, value
	@param[out] bool, if command got sent to EPICS (not if it was accepted)	*/
	bool setMaskXRadius(long val);
	/*! Set the mask y radius, setting the Mask and ROI together is preferred using setMaskAndROIySize
	@param[in] long, value
	@param[out] bool, if command got sent to EPICS (not if it was accepted)	*/
	bool setMaskYRadius(long val);
	/*! Set the mask, setting the Mask and ROI together is preferred using setMaskandROI
	@param[in] long, x_pos value
	@param[in] long, y_pos value
	@param[in] long, x_size value
	@param[in] long, y_size value
	@param[out] bool, if command got sent to EPICS (not if it was accepted)	*/
	bool setMask(long x_pos, long  y_pos, long x_size, long y_size);
	/*! Set the mask using map. Setting the Mask and ROI together is preferred using setMaskandROI
	@param[in] map<string, long>, values to set (keywords must be 'mask_keywords') 
	@param[out] long, value */
	bool setMask(std::map<std::string, long> settings);
	/*! Set the mask using python dict. Setting the Mask and ROI together is preferred using setMaskandROI_Py
	@param[in] dict, values to set (keywords must be 'mask_keywords')
	@param[out] long, value */
	bool setMask_Py(boost::python::dict settings);
	/*! Get the mask x center position.
	@param[in] long, value */
	long getMaskXCenter()const;
	/*! Get the mask y center position.
	@param[in] long, value */
	long getMaskYCenter()const;
	/*! Get the mask x radius.
	@param[in] long, value */
	long getMaskXRadius()const;
	/*! Get the mask y radius.
	@param[in] long, value */
	long getMaskYRadius()const;
	/*! Get the mask settings.
	@param[in] map<string, long>, value */
	std::map<std::string, long> getMask()const;
	/*! Get the mask settings (python version).
	@param[in] map<string, long>, value */
	boost::python::dict getMask_Py()const;
	/*! Set the mask and ROI x position,  
	@param[in] long, value (lower left hand pixel of ROI) 
	@param[out] bool, if command got sent to EPICS (not if it was accepted)	*/
	bool setMaskAndROIxMax(long val);
	/*! Set the mask and ROI y position,
	@param[in] long, value (left-most pixel of ROI)
	@param[out] bool, if command got sent to EPICS (not if it was accepted)	*/
	bool setMaskAndROIyMax(long val);
	/*! Set the mask and ROI x size,
	@param[in] long, value (width of ROI)
	@param[out] bool, if command got sent to EPICS (not if it was accepted)	*/
	bool setMaskAndROIxSize(long val);
	/*! Set the mask and ROI y size,
	@param[in] long, value (height of ROI)
	@param[out] bool, if command got sent to EPICS (not if it was accepted)	*/
	bool setMaskAndROIySize(long val);
	/*! Set the mask and ROI x position,
	@param[out] long, value	*/
	long getMaskAndROIxMax()const;
	/*! Set the mask and ROI y position,
	@param[out] long, value	*/
	long getMaskAndROIyMax()const;
	/*! Set the mask and ROI x size,
	@param[out] long, value	*/
	long getMaskAndROIxSize()const;
	/*! Set the mask and ROI y size,
	@param[out] long, value	*/
	long getMaskAndROIySize()const;
	/*! Set the mask and ROI	
	@param[in] long, x_pos value
	@param[in] long, y_pos value
	@param[in] long, x_size value
	@param[in] long, y_size value
	@param[out] bool, if command got sent to EPICS (not if it was accepted)	*/
	bool setMaskandROI(long x_pos, long  y_pos, long x_size, long y_size);
	/*! Set the mask and ROI using a map
	@param[in] map<string, long>, values to set (keywords must be 'mask_and_roi_keywords')
	@param[out] long, value */
	bool setMaskandROI(std::map<std::string, long> settings);
	/*! Set the mask using python dict. Setting the Mask and ROI together is preferred using setMaskAndROIxPos
	@param[in] dict, values to set (keywords must be 'mask_and_roi_keywords')
	@param[out] long, value */
	bool setMaskandROI_Py(boost::python::dict settings);
	/*! Get the mask and ROI settings
	@param[out] map<string, long>, values ) */
	std::map<std::string, long> getMaskandROI()const;
	/*! Get the mask and ROI settings (Python version).
	@param[out] dict, values ) */
	boost::python::dict getMaskandROI_Py()const;
	/*! Save the Camera IMage buffer to disk (typically the last 600 shots).
	@param[out] bool, if command got sent to EPICS (not if it was accepted)	*/
	bool saveImageBuffer();
	/*! Start image acquiring.
	@param[out] bool, if command got sent to EPICS (not if it was accepted)	*/
	bool startAcquiring();
	/*! Stop image acquiring.
	@param[out] bool, if command got sent to EPICS (not if it was accepted)	*/
	bool stopAcquiring();
	/*! Is camera acquire state == ACQUIRING.
	@param[out] bool, result*/
	bool isAcquiring()const;
	/*! Is camera acquire state == NOT_ACQUIRING.
	@param[out] bool, result*/
	bool isNotAcquiring() const;
	/*! Get image acquire state .
	@param[out] STATE, value from acquire_state*/
	STATE getAcquireState()const;
	/*! Start image analysis.
	@param[out] bool, if command got sent to EPICS (not if it was accepted)	*/
	bool startAnalysing();
	/*! Stop image analysis.
	@param[out] bool, if command got sent to EPICS (not if it was accepted)	*/
	bool stopAnalysing();
	/*! Is camera analysis state == ANALYSING.
	@param[out] bool, result*/
	bool isAnalysing()const;
	/*! Is camera analysis state == NOT_ANALYSING.
	@param[out] bool, result*/
	bool isNotAnalysing() const;
	/*! Get image analysis state .
	@param[out] STATE, value from analysis_state*/
	STATE getAnalysisState()const;
	/* Set the number of shots to capture when "collecting frames and writing to disc."
	@param[out] bool, if requested number is less than max_shots, and the value got sent to epics */
	bool setNumberOfShotsToCapture(size_t num);
	/* Get the number of shots that will be "collected and written to disk."
	@param[out] bool, if requested number is less than max_shots, and the value got sent to epics */
	size_t getNumberOfShotsToCapture()const;
	/*! Capture and save images to disc, using the currently set number of shots to capture.
	@param[out] bool, if command got sent to EPICS (not if it was accepted)	*/
	bool captureAndSave();
	/*! Capture and save images to disc.
	@param[in] size_t, num_images, number of images to capture and write to file
	@param[out] bool, if command got sent to EPICS (not if it was accepted)	*/
	bool captureAndSave(size_t num_images);
	/*! Get Capture state.
	@param[out] STATE, value from capture_state*/
	STATE getCaptureState()const;
	/*! Is camera capturing images, capture_state == CAPTURING.
	@param[out] bool*/
	bool isCapturing()const;
	/*! Is camera capturing images, capture_state == NOT_CAPTURING.
	@param[out] bool*/
	bool isNotCapturing()const;
	/*! Is camera writing data to file, write_state == WRITING.
	@param[out] bool*/
	bool isWriting()const;
	/*! Is camera writing data to file, write_state == NOT_WRITING.
	@param[out] bool*/
	bool isNotWriting()const;
	/*! Is camera capture_state == CAPTURING  OR is write_state == NOT_WRITING.
	@param[out] bool*/
	bool isCapturingOrSaving()const;
	/*! Is camera capture_state != CAPTURING  OR is write_state != NOT_WRITING.
	@param[out] bool*/
	bool isNotCapturingOrSaving()const;

	/*! Sometimes capturign and saving can fail and certian flags do nto get reset. 
	This function attempts to reset paramters to their nominal values.
	Use it if you feel to you have to try something, but really there should be no need to if the camera system is not mistreated 
	@param[out] bool, TRUE if commands got sent to EPICS */
	bool resetCaptureAndSaveError();

	bool areAllRunningStatsFull()const;

	// TODO	isBsuy has not been fully implmented yet  
	/*! Is the camera busy doing some collect, capture, save, write procedure, busy == true 
	* while busy attempts to write more data to disc will fail.
	@param[out] bool	*/
	bool isBusy();
	/*! Is the camera NOT busy doing some collect, capture, save, write procedure, busy == false
	* while busy attempts to write more data to disc will fail.
	@param[out] bool	*/
	bool isNotBusy();
	/*! Did the last capture and save procedure complete succesfully? 
	@param[out] bool	*/
	bool didLastCaptureAndSaveSucceed();
	/*! Does the camera / screen combination have an LED.
	@param[out] bool, value of has_led	*/
	bool hasLED()const;
	/*! Turn the LED on for this camera / screen.
	@param[out] bool, if command got sent to EPICS (not if it was accepted)	*/
	bool setLEDOn();
	/*! Turn the LED off for this camera / screen.
	@param[out] bool, if command got sent to EPICS (not if it was accepted)	*/
	bool setLEDOff();
	/*! Is LED on, led_state == ON.
	@param[out] bool*/
	bool isLEDOn()const;
	/*! Is LED off, led_state == OFF.
	@param[out] bool*/
	bool isLEDOff()const;
	/*! Get LED state.
	@param[out] STATE, value from led_state*/
	STATE getLEDState()const;
	/*! Get the latest image data (decimated array for passing accross network, not full image).
	@param[out] bool, did commands get sent to EPICS (should also mean data was updated)*/
	bool updateImageData();
	/*! Get the latest image data AND time stamp.
	It is possible in the current implementation that the time stamp is not synchronized with the data.
	(decimated array for passing accross network, not full image).
	@param[out] bool, did commands get sent to EPICS (should also mean data was updated)*/
	bool updateImageDataWithTimeStamp();
	/*! Get the latest ROI data (decimated array for passing accross network, not full image, !!ROI is new, not been tested!!). 
	@param[out] bool, did commands get sent to EPICS (should also mean data was updated)*/
	bool updateROIData();
	/*! Get the latest ROI data AND time stamp.
	It is possible in the current implementation that the time stamp is not synchronized with the data.
	(decimated array for passing accross network, not full image).
	@param[out] bool, did commands get sent to EPICS (should also mean data was updated)*/
	bool updateROIDataWithTimeStamp();
	/*! Get a copy of the current image data. Until an updateImage function is called this will be empty, 
	to reduce network load Camera data ARE NOT continuously monitored.
	@param[out] vector<long>, latest data */
	std::vector<long> getImageData()const;
	/*! Get a copy of the current image data (Python Version). Until an updateImage functiton is called this will be empty,
	to reduce network load Camera data arrays ARE NOT continuously monitored. 
	@param[out] list, latest data */
	boost::python::list getImageData_Py()const;
	/*! Get a copy of the current image data. Until an updateROI function is called this will be empty,
	to reduce network load Camera data arrays ARE NOT continuously monitored.
	@param[out] vector<long>, latest data */
	std::vector<long> getROIData()const;
	/*! Get a copy of the current image data (python version). Until an updateROI function is called this will be empty,
	to reduce network load Camera data arrays ARE NOT continuously monitored.
	@param[out] list, latest data */
	boost::python::list getROIData_Py()const;
	/*! Get a reference to the current image data. Gives access to image data without copying 
	@param[out] vector<long>&, reference to latest data, When exposed to python this function returns a std_vector_long  */
	std::vector<long>& getImageDataConstRef();
	/*! Get a reference to the current ROI data. Gives access to data without copying
	@param[out] vector<long>&, reference to latest data, When exposed to python this function returns a std_vector_long  */
	std::vector<long>& getROIDataConstRef();
	/*! Get the size of the running stats buffer, running_stats_buffer_size
	@param[out] size_t, value */
	size_t getBufferSize()const;
	/*! set the size of the running stats buffer, running_stats_buffer_size
	@param[in] size_t, value */
	void setAllRunningStatBufferSizes(size_t v);
	/*! clear all runing stats buffers */
	void clearAllRunningStatBuffers();
	/*! Get the current number of data values being used by the Running Stats.
* 	@param[out] size_t: number of data values.*/
	size_t getRunningStatNumDataValues()const;
	/*! Get the pixel to mm conversion factor, 
	@param[out] double, value */
	double getPix2mm()const;
	///*! Get the running stats for a particular analsys results (x position, or y position, etc.) 
	//@param[out] string, string of the TYPE of running stat to return  
	//@param[out] dict, value */
	//boost::python::dict getRunningStats(const std::string& type_str)const;
	///*! Get the running stats for a particular analsys results (x position, or y position, etc.)
	//@param[out] TYPE, TYPE of running stat to return
	//@param[out] dict, value */
	//boost::python::dict getRunningStats(TYPE type)const;
	/*! Get the running stats buffer,
	@param[out] dict, values */
	boost::python::dict getAllRunningStats()const;
	/*! Returns the running stats object for x_pix, not just the dict
	@param[out] RunningStats, rs*/
	RunningStats& getXPixRunningStats();
	/*! Returns the running stats object for y_pix, not just the dict
	@param[out] RunningStats, rs*/
	RunningStats& getYPixRunningStats();
	/*! Returns the running stats object for sigma_x_pix, not just the dict
	@param[out] RunningStats, rs*/
	RunningStats& getSigmaXPixRunningStats();
	/*! Returns the running stats object for sigma_y_pix, not just the dict
	@param[out] RunningStats, rs*/
	RunningStats& getSigmaYPixRunningStats();
	/*! Returns the running stats object for sigma_xy_pix, not just the dict
	@param[out] RunningStats, rs*/
	RunningStats& getSigmaXYPixRunningStats();
	/*! Returns the running stats object for x_mm, not just the dict
	@param[out] RunningStats, rs*/
	RunningStats& getXmmRunningStats();
	/*! Returns the running stats object for y_mm, not just the dict
	@param[out] RunningStats, rs*/
	RunningStats& getYmmRunningStats();
	/*! Returns the running stats object for sigma_x_mm, not just the dict
	@param[out] RunningStats, rs*/
	RunningStats& getSigmaXmmRunningStats();
	/*! Returns the running stats object for sigma_y_mm, not just the dict
	@param[out] RunningStats, rs*/
	RunningStats& getSigmaYmmRunningStats();
	/*! Returns the running stats object for sigma_xy_mm, not just the dict
	@param[out] RunningStats, rs*/
	RunningStats& getSigmaXYmmRunningStats();
	/*! Returns the running stats object for avg_intensity, not just the dict
	@param[out] RunningStats, rs*/
	RunningStats& getAvgIntensityRunningStats();
	/*! Returns the running stats object for sum_intensity, not just the dict
	@param[out] RunningStats, rs*/
	RunningStats& getSumIntensityRunningStats();
	/*! Set the black level (for vela camera types only),
	@param[in] long, values 
	@param[out] bool, values */
	bool setBlackLevel(long value);
	/*! Set the black level (for VELA camera types only),
	@param[out] bool, if command got sent to EPICS (not if it was accepted) */
	long getBlackLevel()const;
	/*! Set the black level (for VELA camera types only),
	@param[out] long, latest value */
	bool setGain(long value);
	/*! Get the gain (for VELA camera types only),
	@param[out] long, latest value */
	long getGain()const;



	/*! Enable the Cross-Overlay on the decimated camera image array sent over the network.
		* Will be visiable to many camera image viewing apps,
		@param[out] bool, true if value got sent to epics (not if it was received)*/
	bool enableOverlayCross();
	/*! Disable the Cross-Overlay on the decimated camera image array sent over the network.
	* The overlay will be visable to many camera image viewing apps,
	@param[out] bool, true if value got sent to epics (not if it was received)*/
	bool disableOverlayCross();
	/*! Get the state of Cross-Overlay on the decimated camera image array sent over the network.
	@param[out] STATE, current state of overla, ENABLED, DISBALED, */
	STATE getOverlayCrossState()const;
	/*! Compare the current state of the cross overlay to ENABLED.
	@param[out] bool, returns true if the overlay is ENABLED,  otherwise false.*/
	bool isOverlayCrossEnabled()const;
	/*! Compare the current state of the cross overlay to DISABLED.
	@param[out] bool, returns true if the overlay is DISABLED,  otherwise false.*/
	bool isOverlayCrossDisabled()const;
	/*! Enable the Mask-Overlay on the decimated camera image array sent over the network.
	* Will be visiable to many camera image viewing apps,
	@param[out] bool, true if value got sent to epics (not if it was received)*/
	bool enableOverlayMask();
	/*! Disable the Mask-Overlay on the decimated camera image array sent over the network.
	* The overlay will be visable to many camera image viewing apps,
	@param[out] bool, true if value got sent to epics (not if it was received)*/
	bool disableOverlayMask();
	/*! Get the state of Cross-Overlay on the decimated camera image array sent over the network.
	@param[out] STATE, current state of overla, ENABLED, DISBALED, */
	STATE getOverlayMaskState()const;
	/*! Compare the current state of the Mask-Overlay to ENABLED.
	@param[out] bool, returns true if the overlay is ENABLED,  otherwise false.*/
	bool isOverlayMaskEnabled()const;
	/*! Compare the current state of the Mask-Overlay to DISABLED.
	@param[out] bool, returns true if the overlay is DISABLED,  otherwise false.*/
	bool isOverlayMaskDisabled()const;
	/*! Enable the Results-Overlay on the decimated camera image array sent over the network.
	* Will be visiable to many camera image viewing apps,
	@param[out] bool, true if value got sent to epics (not if it was received)*/
	bool enableOverlayResult();
	/*! Disable the Results-Overlay on the decimated camera image array sent over the network.
	* The overlay will be visable to many camera image viewing apps,
	@param[out] bool, true if value got sent to epics (not if it was received)*/
	bool disableOverlayResult();
	/*! Get the state of Results-Overlay on the decimated camera image array sent over the network.
	@param[out] STATE, current state of overla, ENABLED, DISBALED, */
	STATE getOverlayResultState()const;
	/*! Comapre the current state of the Results-Overlay to ENABLED.
	@param[out] bool, returns true if the overlay is ENABLED,  otherwise false.*/
	bool isOverlayResultEnabled()const;
	/*! Compare the current state of the Results-Overlay to DISABLED.
	@param[out] bool, returns true if the overlay is DISABLED,  otherwise false.*/
	bool isOverlayResultDisabled()const;
	/*! Disbale all the image overlays.
	@param[out] bool, returns true if the overlay is DISABLED,  otherwise false.*/
	bool disableAllOverlay();

	/*! testing returning pointer to image array instead of copying 	*/
	//std::vector<double>* Camera::getImagedataPointer();
	


	/*! Get the X Pixel defined as the "horizontal" centre of the image-array, probably set from the Master Lattice.
	@param[out] long, value */
	long getCentreXPixel()const;
	/*! Get the Y Pixel defined as the "vertical" centre of the image-array, probably set from the Master Lattice.
	@param[out] long, value */
	bool getCentreYPixel()const;
	/*! Set the X Pixel to be defined as the "horizontal" centre of the image-array.
	@param[in] long, value to set
	@param[out] bool, true if value got sent to epics (not if it was received)*/
	bool setCentreXPixel(long value);
	/*! Set the Y Pixel to be defined as the "vertical" centre of the image-array.
	@param[in] long, value to set
	@param[out] bool, true if value got sent to epics (not if it was received)*/
	bool setCentreYPixel(long value);
	///*! Set the X and Y Pixels to be defined as the centre of the image-array.
	//@param[in] long, x value to set
	//@param[in] long, y value to set
	//@param[out] bool, true if value got sent to epics (not if it was received)*/
	//bool setCentrePixels(long x, long y);
	///*! Sets the analysis centre to be the main lattice values for the mechanical center (in pixels).
	//* Sets the analysis centre to be the main lattice values.
	//@param[out] bool, if the value got sent to epics.*/
	//bool setMechanicalCentre();
	///*! For the virtual Cathode the centre of the screen can be defined using the RF centre of the injector.
	//* Sets the analysis centre to be the main lattice values (in pixels).
	//@param[out] bool, if the value got sent to epics.*/
	//bool setRFCenter();
	///*! Get the RF center X Pixel, probably set from the Master Lattice.
	//@param[out] long, value */
	//long getRFCentreXPixel()const;
	///*! Get the RF center Y Pixel, probably set from the Master Lattice.
	//@param[out] long, value */
	//long getRFCentreYPixel()const;
	///*! Get the mechanical center X Pixel, probably set from the Master Lattice.
	//@param[out] long, value */
	//long getMechCentreXPixel()const;
	///*! Get the mechanical center Y Pixel, probably set from the Master Lattice.
	//@param[out] long, value */
	//long getMechCentreYPixel()const;


	/* Enable debug messages*/
	void debugMessagesOn();
	/* Disbale debug messages*/
	void debugMessagesOff();
	/* Enable messages*/
	void messagesOn();
	/* Disable messages*/
	void messagesOff();

	friend class EPICSCameraInterface;
	friend class CameraFactory;
protected:
	/* 
		In general we put data that comes back from EPICS into a timestamp, data pair. 
		This is because we expect to be able to use the time stamps to synchonize different data streams 
	*/
	/*! Cross-overlay status. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, STATE> cross_overlay;
	/*! Mask-overlay status. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, STATE> mask_overlay;
	/*! Results-overlay status. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, STATE> result_overlay;
	/*! Camera gain, for VELA_CAMERA type only. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, long > gain;
	/*! Camera black_level, for VELA_CAMERA type only. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, long > black_level;
	/*! Camera set new background STATE. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, STATE > set_new_background;
	/*! Camera set new background STATE. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, long > capture_count;
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
	/*! State of using NPoint scaling. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, STATE > use_npoint;
	/*! latest stepsize (Npoint scaling step size). Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, long > step_size;
	/*! State of background data  scaling. Value and epicstimestamp.	*/
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
	/*! Pixel Results from Analysis: [x_position, y_position, x_sigma_position, y_sigma_position, covariance_pos]*/
	std::pair<epicsTimeStamp, std::vector<double> > pixelResults;
	/*! mm Results from Analysis: [x_position, y_position, x_sigma_position, y_sigma_position, covariance_pos]*/
	std::pair<epicsTimeStamp, std::vector<double> > mmResults;
	/*! Stores the last time the ANA Pixel Results were updated, will be used to set isResultUpdated*/
	epicsTimeStamp lastResultsUpdateTime;
	/*! Stores the last time the ANA MM Results were updated, purely used fro pushign to the running stats */
	epicsTimeStamp lastResultsUpdateTime_mm_ana_results;
	/*! Flag for showing that the ANA Pixel Results are updating */
	bool isResultUpdated;
	/*! ROI image data. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, std::vector<long>> roi_imagedata;
	/*! An x pixel number to be the horizontal centre (probably set from the Master Lattice).	*/
	std::pair<epicsTimeStamp, double > x_center_pixel;
	/*! A y pixel number to be the vertical centre (probably set from the Master Lattice).	*/
	std::pair<epicsTimeStamp, double > y_center_pixel;
	/*! conversion factor for pixels to mm. Value and epicstimestamp. From Epics. */
	std::pair<epicsTimeStamp, double > pix_to_mm;


	// TODO get rid of these, or make better! 
	/*! conversion of pixels to mm */
	std::pair<epicsTimeStamp, double > pix2mm;
	double pix2mmX_ratio;
	/*! conversion factor for pixels to mm in the y direction. From Master Lattice. */
	double pix2mmY_ratio;


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
	/*! Array Image width in pixels. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, long> pixel_width;
	/*! Array Image height in pixels. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, long> pixel_height;
	/*! pixel to mm conversion. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, double> pixel_to_mm;
	/*! Number of images to capture. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, long> num_capture;
	/*! pointer to dbr_time_stuct, used to get timestmp for images*/
	struct dbr_time_long* dbr_image_data;
	/*! image_data vector to hold image data */
	std::pair<epicsTimeStamp, std::vector<long>> image_data;
	/*! pointer to dbr_time_stuct, used to get timestmp for roi*/
	struct dbr_time_long* dbr_roi_data;
	/*! roi_data vector to hold image data */
	std::pair<epicsTimeStamp, std::vector<long>> roi_data;


	/*! analysis data results, x, y , sigma_x, sigma_y, sigma_xy (can be re-defined in config file ) */
	std::pair<epicsTimeStamp, std::vector<double>> analysis_data;
	/*! names for each element in the pixel results array, defined in the master lattice config file  */
	std::vector<std::string> analysis_data_names;


	std::pair<epicsTimeStamp, STATE> cross_hair_overlay;
	std::pair<epicsTimeStamp, STATE> center_of_mass_overlay;
	std::pair<epicsTimeStamp, STATE> analysis_mask_overlay;

	/*! Keywords used for setting ROI and mask together, python version (OUR PREFFERRED INTERFACE!) */
	//boost::python::list mask_and_roi_keywords_Py;
	/*! Keywords used for setting mask, python version  (Not prefferred, use mask and ROI together) */
	//boost::python::list mask_keywords_Py;
	/* Number of shots to buffer for the analysis results*/  
	size_t running_stats_buffer_size;
	/* X position (pixels) running stats buffer*/
	RunningStats x_pix_rs;
	/* Y position (pixels) running stats buffer*/
	RunningStats y_pix_rs;
	/* X standard deviation (pixels) running stats buffer*/
	RunningStats sigma_x_pix_rs;
	/* Y standard deviation (pixels) running stats buffer*/
	RunningStats sigma_y_pix_rs;
	/* X-Y  standard deviatio(pixels) running stats buffer*/
	RunningStats sigma_xy_pix_rs;
	/* X position (mm) running stats buffer*/
	RunningStats x_mm_rs;
	/* Y position (mm) running stats buffer*/
	RunningStats y_mm_rs;
	/* X standard deviation (mm) running stats buffer*/
	RunningStats sigma_x_mm_rs;
	/* X standard deviation (mm) running stats buffer*/
	RunningStats sigma_y_mm_rs;
	/* X-Y standard deviation  (mm^2) running stats buffer*/
	RunningStats sigma_xy_mm_rs;
	/* avergae pixel intensity  running stats buffer*/
	RunningStats avg_intensity_rs;
	/* sum pixel running stats buffer*/
	RunningStats sum_intensity_rs;

	// protected so cam-factory can get these eaisly 
	double master_lattice_pixel_to_mm;
	long  master_lattice_centre_x;
	long  master_lattice_centre_y;

private:
	/*! Type of camera, different camera types have different funcionality, defined in Master Lattice  */
	TYPE cam_type;
	/* does this camera have an LED */
	bool has_led;
	/*! Number of pixels in 'horizontal axis for array data accross the network. From master lattice. */
	size_t array_data_num_pix_x;
	/*! Number of pixels in 'vertical' axis for array data accross the network. From master lattice. */
	size_t array_data_num_pix_y;
	/* total number of pixels in the image array data*/
	size_t array_data_pixel_count;
	/*! Number of pixels in 'horizontal axis for full image saved to disc. From master lattice. */
	size_t binary_num_pix_x;
	/*! Number of pixels in 'vertical axis for full image saved to disc. From master lattice. */
	size_t binary_num_pix_y;
	/* total number of pixels in the full binary image data*/
	size_t binary_data_pixel_count;
	/*! Number of pixels in the Region Of Interest data, (from ROI1:SizeX * ROI1:SizeY) */
	size_t roi_total_pixel_count;

	
	/*! Number of pixels in 'horizontal' axis for the CAM1 array data. */
	size_t cam1_num_pix_x;
	/*! Number of pixels in 'vertical' axis for the CAM1 array data. */
	size_t cam1_num_pix_y;
	/*! Number of pixels in 'horizontal' axis for the CAM2 array data. */
	size_t cam2_num_pix_x;
	/*! Number of pixels in 'vertical' axis for the CAM2 array data. */
	size_t cam2_num_pix_y;
	/*! Number of pixels in 'horizontal' axis for the CAM3 array data. */
	size_t cam3_num_pix_x;
	/*! Number of pixels in 'vertical' axis for the CAM3 array data. */
	size_t cam3_num_pix_y;

	size_t roi_max_x;
	size_t roi_max_y;


	/*! For the virtual Cathode we can have an RF centre (x pxiel) defined in main lattice */
	long rf_centre_x;
	/*! For the virtual Cathode we can have an RF centre (x pxiel) defined in main lattice */
	long rf_centre_y;
	/*! The horizontal mechanical centre of the image in pixels defined in main lattice */
	long mechanical_centre_x;
	/*! The vertical mechanical centre of the image in pixels defined in main lattice */
	long mechanical_centre_y;


	/* total number of pixels in the image array data*/

	/*! scaling from array_data num pixels to binary image data num pixels for 'horizontal' axis. */
	size_t x_pix_scale_factor;
	/*! scaling from array_data num pixels to binary image data num pixels for 'vertical' axis. */
	size_t y_pix_scale_factor;
	/*! default mask x values, might not be needed now we have mask AND roi */
	size_t x_mask_def;
	/*! default mask y values, might not be needed now we have mask AND roi */
	size_t y_mask_def;
	/*! default mask x rad values, migth not be needed now we have mask AND roi */
	size_t x_mask_rad_def;
	/*! default mask x rad values, might not be needed now we have mask AND roi */
	size_t y_mask_rad_def;
	/*! maximum allowed mask x radius */
	size_t x_mask_rad_max;
	/*! maximum allowed mask y radius */
	size_t y_mask_rad_max;
	/*! Flag to set if mask radius limits are enforced */
	bool use_mask_rad_limits;
	/*! 'Horizontal' pixel for nominal beam orbit */
	size_t x_center_def;
	/*! 'Vertical' pixel for nominal beam orbit */
	size_t y_center_def;
	/*! maximum temperature the camera sensor should get to */
	double sensor_max_temperature;
	/*! minimum temperature the camera sensor should get to  */
	double sensor_min_temperature;
	/*! empircally determined value used to estimate if there is beam in the image */
	double average_pixel_value_for_beam;
	/*! empircally determined value used to decide if the beam is close to the edge*/
	double min_x_pixel_pos;
	/*! empircally determined value used to decide if the beam is close to the edge*/
	double max_x_pixel_pos;
	/*! empircally determined value used to decide if the beam is close to the edge*/
	double min_y_pixel_pos;
	/*! empircally determined value used to decide if the beam is close to the edge*/
	double max_y_pixel_pos;
	/*! Maximum number of images that can be captured in a burst.*/
	size_t max_shots_number;
	/*! alternative names for the camera (usually shorter than the full PV root),
	defined in the Master Lattice*/
	std::vector<std::string> aliases;
	/*! Names of scnres that this camera views, defined in the Master Lattice*/
	std::vector<std::string> screen_names;
	/*! Funciton that captures and saves aimges to disc (run in new thread) */
	void imageCaptureAndSave(size_t num_shots);
	/*! Kills any completed image collect threads */
	void killFinishedImageCollectThread();
	/* static function that runs the capture and save fucntions */
	static void staticEntryImageCollectAndSave(ImageCapture& ic);

	/*! Parse the Master Lattice data for this camera */
	void getMasterLatticeData(const std::map<std::string, std::string>& paramMap, STATE mode);

	/* ImageCapture class to hold data for image capturing and savign thread */
	ImageCapture image_capture;
	/* Capture images 
	@param[out] bool, if the value got sent to epics */
	bool capture();
	/* Write images to disc
	@param[out] bool, if the value got sent to epics */
	bool write();
	/* Set the directory and file name for the next capture and save 
	@param[out] bool, if the value got sent to epics */
	bool makeANewDirectoryAndName(size_t numbOfShots);
	/* Set the directory and file name for the Camera Image Buffer dump.
	@param[out] bool, if the value got sent to epics */
	bool makeANewDirectoryAndNameBuffer();
	/* Flag set if the camera is "busy" busy means performing any part of the capture and save routine,  */
	bool busy;
	/* Flag set if the last capture and save routine manged to complete  */
	bool last_capture_and_save_success;

	EPICSCameraInterface_sptr epicsInterface;
	std::map<std::string, std::string> CameraParamMap;
	
	/*! Keywords used for setting ROI and mask together (OUR PREFFERRED INTERFACE!) */
	std::vector<std::string> mask_and_roi_keywords;
	/*! Keywords used for setting mask (Not prefferred, use mask and ROI together) */
	std::vector<std::string> mask_keywords;
	/*! Keywords used for setting ROI (Not preffeered, use mask and ROI together) */
	std::vector<std::string> roi_keywords;


	/*! Get the latest time stamp for an image / ROI  array 
	@param[in] struct dbr_time_long*, pointer to DBR_TIME_LONG struct 
	@param[in] const pvStruct&, pvStruct with channel information 
	@param[in] epicsTimeStamp&, value to update with timestamp 
	@param[out] bool, if command got sent to EPICS (not if it was accepted)	*/
	bool getArrayTimeStamp(struct dbr_time_long* dbr_struct, 
						  const pvStruct& pvs, epicsTimeStamp& ts_to_update);
	/*! Get the latest data an image / ROI  array
	@param[in] std::vector<long>&, vector to write new data to 
	@param[in] const pvStruct&, pvStruct with channel information
	@param[in] size_t, count of array elemnts to get (for ROI this changes depend on the size of the ROI  
	@param[out] bool, if command got sent to EPICS (not if it was accepted)	*/
	bool getArrayValue(std::vector<long>& data_vec, const pvStruct& pvs, size_t count);
	/*! Resize vector to array_data_pixel_count size, (to save memory only done on request) 
	@param[in] std::vector<long>&, vector to resize
	@param[out] bool, if resize succeeded */
	bool vector_resize(std::vector<long>& vec);
	/*! Allocate memory for image_data dbr_time_long pointer (to save memory only done on request) */
	void malloc_imagedata();
	/*! Allocate memory roi_data dbr_time_long pointer (to save memory only done on request) */
	void malloc_roidata();
	/*! Flag set if the dbr_image_data has had memory allocated for it. 
	To save application memory this ony happens when requesting image data accross the network */
	bool image_data_has_not_malloced;
	/*! Flag set if the image_data vector has had memory allocated for it.
	To save application memory this ony happens when requesting image data accross the network */
	bool image_data_has_not_vector_resized;
	/*! Flag set if the dbr_roi_data has had memory allocated for it.
	To save application memory this ony happens when requesting image data accross the network */
	bool roi_data_has_not_malloced;
	/*! Flag set if the roi_data vector has had memory allocated for it.
	To save application memory this ony happens when requesting image data accross the network */
	bool roi_data_has_not_vector_resized;


	// TODO issue 101, move to globalfunctions 
	bool areEqualEpicsTimeStamps(const epicsTimeStamp& t1, const epicsTimeStamp& t2)const 
	{
		if (t1.secPastEpoch == t2.secPastEpoch)
		{
			if (t1.nsec == t2.nsec)
			{
				return true;
			}
		}
		return false;
	}
	bool areNotEqualEpicsTimeStamps(const epicsTimeStamp& t1, const epicsTimeStamp& t2)const
	{
		return !areEqualEpicsTimeStamps(t1,t2);
	}

};


#endif //CAMERA_H_