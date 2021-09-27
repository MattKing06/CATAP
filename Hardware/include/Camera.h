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
#include "HardwareSnapshot.h"
#include "CameraPVRecords.h"
#include <boost/make_shared.hpp>
#include <boost/python/dict.hpp>
#include <boost/python/list.hpp>
#include "RunningStats.h"
#include <thread>
#include "HardwareSnapshot.h"


class Camera;
class ImageCapture;
/*	Image collection and saving happens in a new thread, this struct is passed to the new thread function		*/
class ImageCapture
{
	public: 
		ImageCapture() :
			cam(nullptr), thread(nullptr), num_shots(0),
			is_busy(false),status(STATE::ERR){}
		/*! pointer to a cam object fro access */
		Camera* cam;
		/*! Thread image capture occurs in */
		std::thread*   thread;
		/*! Number of Images  */
		size_t       num_shots;
		/*! Flag to indicate if iage colelctign and saving is still in progress */
		bool is_busy;
		/*! A Status flag  */
		STATE status;
};
/*!	Wait for camera to stop acquiring, this struct is passed to the new thread function		*/
class CamStopWaiter
{public:CamStopWaiter() :cam(nullptr), thread(nullptr), wait_ms(5000),result(STATE::UNKNOWN){}Camera* cam; std::thread* thread; size_t wait_ms; STATE result;};
class EPICSCameraInterface;
typedef boost::shared_ptr<EPICSCameraInterface> EPICSCameraInterface_sptr;
/*! main camera object */
class Camera : public Hardware
{
public:
	Camera();
	Camera(const std::map<std::string, std::string>& paramMap, STATE mode);
	Camera(const Camera& copyCamera);
	~Camera();

	friend class EPICSCameraInterface;
	friend class CameraFactory;

public:
	/*! get the type of the camera (e.g. vela_camera, clara_camera
	@param[out] type */
	TYPE getCamType()const;
private:
	/*! Type of camera, different camera types have different funcionality, defined in Master Lattice  */
	TYPE cam_type;

public:
	/*! Get the Sensor Temperature for the camera (degrees Celsius).
	@param[out] double, value */
	double getTemperature()const;
//                   ___  __  
//  |\ |  /\   |\/| |__  /__` 
//  | \| /~~\  |  | |___ .__/ 
//                            
//	Cameras can be referenced by their full name, an alias or the name of the screen 
//  they are looking at. Set in the main lattice. 
//	7
public:	
	/*! Get all the name alises for this Camera, defined in the main lattice. 
	@param[out] names, vector containing  all the alias names */
	std::vector<std::string> getAliases() const;
	/*! Get all the name alises for this Camera (python version)
	@param[out] names, python list containing all the alias names */
	boost::python::list getAliases_Py() const;
	/*! Get the screen names the camera is attached to.c
	@param[out] names, vector containing  all the screen names (and their aliases) */
	std::vector<std::string> getScreenNames() const;
	/*! Get the screen names the camera is attached to (python version).
	@param[out] names, python list containing all the screen names (and their aliases) */
	boost::python::list getScreenNames_Py() const;
	/*! Get the first screen name the camera is attached to, defined in the main lattice. 
	@param[out] name, */
	std::string getScreen()const;
//  
//   __         ___         ___  __                 
//  |__) | \_/ |__  |        |  /  \     |\/|  |\/| 
//  |    | / \ |___ |___     |  \__/     |  |  |  | 
//                                                  
//	On the physical mahcien there is a single pixle 2 mm conversion 
//	Seperate x, y conversions are provided but not yet used  
public:	
	/*! Get the pixels to mm value (the one that is actaully used in EPICS).
	@param[out] double, value */
	double getPixelToMM()const;
	/*! Set the pixels to mm value (the one that is actaully used in EPICS).
	@param[in] double, value
	@param[out] bool, if command got sent to EPICS (not if it was received). */
	bool setPixelToMM(double val)const;
	/*! Convert a length in pixels to mm for this camera.
	@param[in] double, length in pixels
	@param[out] double, length in mm*/
	double pix2mm(double value)const;
	/*! Convert a length in mm to pixels for this camera.
	@param[in] double, length in mm
	@param[out] double, length in pixels*/
	double mm2pix(double value)const;
	/*! Get the pixels to mm in x(horizontal) direction. (First set from the yaml config file).
	* This is not used on The Physcial Machine. 
	@param[out] double, value */
	double getpix2mmX()const;
	/*! Get the pixels to mm in y (vertical) direction. (First set from the yaml config file)
	* This is not used on The Physcial Machine.
	@param[out] double, value */
	double getpix2mmY()const;
	/*! Set the pixels to mm conversion factor in x(horizontal) direction.
	@param[in] double, new value */
	double setpix2mmX(double value);
	/*! Set the pixels to mm conversion factor in y(vertical) direction.
	@param[in] double, new value */
	double setpix2mmY(double value);
	/*! convert a horizontal length in mm to pixels for this camera
	@param[in] double, length in pixels
	@param[out] double, length in mm*/
	double pix2mmX(double value)const;
	/*! convert a vertical length in pixels to mm for this camera
	@param[in] double, length in pixels
	@param[out] double, length in mm*/
	double pix2mmY(double value)const;
	/*! convert a horizontal length in mm to pixels for this camera
	@param[in] double, length in pixels
	@param[out] double, length in mm*/
	double mm2pixX(double value)const;
	/*! convert a vertical length in mm to pixels for this camera
	@param[in] double, length in pixels
	@param[out] double, length in mm*/
	double mm2pixY(double value)const;
	/*! Get the pixel to mm conversion factor,
	@param[out] double, value */
	double getPix2mm()const;
	/*! Get the pixel bit depth for this cmaera,
	@param[out] size_t, value */
	size_t getBitDepth()const;

protected:
	/*! conversion factor for pixels to mm. Value and epicstimestamp. From Epics. */
	std::pair<epicsTimeStamp, double > pixel_to_mm;
	/*! conversion factor for pixels to mm in the x direction. From Main Lattice (not implemented in Physcial EPICS). */
	double pix2mmX_ratio;
	/*! conversion factor for pixels to mm in the y direction. From Main Lattice (not implemented in Physcial EPICS). */
	double pix2mmY_ratio;
	/*! How many bits each pixel has*/
	size_t bit_depth;


	// IMAGE ORIENTATION

public:
	/*! Get required rotations of raw image array to match installed orentation for image (0, 90, 180 or 270 degrees). 
	* 	@param[out] size_t, value */
	size_t getImageRotation()const;
	/*! Get should the image be flipped along the up/down axis (after rotation)  i.e. row order reversed) 
	* 	@param[out] bool, value */
	bool getImageFlipUD()const;
	/*! Get should the image be flipped along the left/right axis (after rotation) i.e. column order reversed) 
	* 	@param[out] bool, value */
	bool getImageFlipLR()const;

protected:

	size_t image_rotation;
	bool flip_ud;
	bool flip_lr;



//    __   ___      ___  ___  __      __   __      __                      __      __   __      __   __   __  
//   /  ` |__  |\ |  |  |__  |__)    /  \ |__)    /  \ |\ | __  /\  \_/ | /__`    /  ` /  \ __ /  \ |__) |  \	
//   \__, |___ | \|  |  |___ |  \    \__/ |  \    \__/ | \|    /~~\ / \ | .__/    \__, \__/    \__/ |  \ |__/ 
//                                                                                                         
//  A Central (0,0) Co-ordinate of an image can be defined in terms of pixel row / column 
//	Analysis resutls are given relative to this coordinate 
//	Pre-defined coords are given in the Main-Lattice for the "Mechanical Centre" physcial  and the "Operating-Centre." 
//  In general these should be calibrated and defined via meausrements. 
//
public:
	/*! Get the X Pixel defined as the "horizontal" centre of the image data.
	@param[out] long, value */
	long getCentreXPixel()const;
	/*! Get the Y Pixel defined as the "vertical" centre of the image-array.
	@param[out] long, value */
	long getCentreYPixel()const;
	/*! Set the X Pixel to be defined as the "horizontal" centre of the image-array.
	@param[in] long, value to set
	@param[out] bool, true if value got sent to epics (not if it was received)*/
	bool setCentreXPixel(long value);
	/*! Set the Y Pixel to be defined as the "vertical" centre of the image-array.
	@param[in] long, value to set
	@param[out] bool, true if value got sent to epics (not if it was received)*/
	bool setCentreYPixel(long value);
	/*! Set the X and Y Pixels to be defined as the centre of the image-array.
	@param[in] long, x value to set
	@param[in] long, y value to set
	@param[out] bool, true if value got sent to epics (not if it was received)*/
	bool setCentrePixels(long x, long y);
	/*! Sets the analysis centre to be the main lattice values for the mechanical center (in pixels).
	* Sets the analysis centre to be the main lattice values.
	@param[out] bool, if the value got sent to epics.*/
	bool setMechanicalCentre();
	/*! For the virtual Cathode the centre of the screen can be defined using the RF centre of the injector.
	* Sets the analysis centre to be the main lattice values (in pixels).
	@param[out] bool, if the value got sent to epics.*/
	bool setOperatingCenter();
	/*! Get the RF center X Pixel, from the Main Lattice.
	@param[out] long, value */
	long getOperatingCentreXPixel()const;
	/*! Get the RF center Y Pixel, from the Main Lattice.
	@param[out] long, value */
	long getOperatingCentreYPixel()const;
	/*! Get the mechanical center X Pixel, from the Main Lattice.
	@param[out] long, value */
	long getMechanicalCentreXPixel()const;
	/*! Get the mechanical center Y Pixel, from the Main Lattice.
	@param[out] long, value */
	long getMechanicalCentreYPixel()const;
protected:
	/*! An x pixel number to be the horizontal centre (probably set from the Master Lattice).	*/
	std::pair<epicsTimeStamp, long > x_center_pixel;
	/*! A y pixel number to be the vertical centre (probably set from the Master Lattice).	*/
	std::pair<epicsTimeStamp, long > y_center_pixel;
private:
	/*! For the virtual Cathode we can have an RF centre (x pxiel) defined in main lattice */
	long operating_centre_x;
	/*! For the virtual Cathode we can have an RF centre (x pxiel) defined in main lattice */
	long operating_centre_y;
	/*! The horizontal mechanical centre of the image in pixels defined in main lattice */
	long mechanical_centre_x;
	/*! The vertical mechanical centre of the image in pixels defined in main lattice */
	long mechanical_centre_y;
//  			  __   ___ __    __  ___
//  |  |\/|  /\  / _` |__ /__` |  / |__
//  |  |  | /~~\ \__> |___.__/ | /_ |___
//  
// size of the images in pixels. CF image array data over the network may be decimated
//
public:
	/*! Get the Number of pixels in the width of the (full) image (this values is from EPICS).
	@param[out] long, value */
	long getPixelWidth()const;
	/*! Get the Number of pixels in the height of the (full) image (this values is from EPICS).
	@param[out] long, value */
	long getPixelHeight()const;
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
private:
	/*! Full image array width in pixels, from EPICS. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, long> epics_pixel_width;
	/*! Full image array height in pixels. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, long> epics_pixel_height;
	/*! Number of pixels in 'horizontal axis' for array data accross the network. Defined in Main-lattice. */
	size_t array_data_num_pix_x;
	/*! Number of pixels in 'vertical' axis for array data accross the network. Defined in Main-lattice. */
	size_t array_data_num_pix_y;
	/* total number of pixels in the image array data*/
	size_t array_data_pixel_count;
	/*! Number of pixels in 'horizontal axis for full image saved to disc. Defined in Main-lattice. */
	size_t binary_num_pix_x;
	/*! Number of pixels in 'vertical axis for full image saved to disc. Defined in Main-lattice. */
	size_t binary_num_pix_y;
	/*! Total number of pixels in the full binary image data */
	size_t binary_data_pixel_count;
	/* !Total number of pixels in the ROI image data , (from ROI1:SizeX * ROI1:SizeY) */
	size_t roi_total_pixel_count;
//	 __   __                __        __  
//	/  ` |__) |  |    |    /  \  /\  |  \ 
//	\__, |    \__/    |___ \__/ /~~\ |__/ 
//	                                      
public:
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
private:
	/*! Total CPU usage (units??). Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, long> cpu_total;
	/*! CPU use by crop, subtract and mask (units??). Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, long> cpu_crop_sub_mask;
	/*! CPU use by NPoint analysis (units??). Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, long> cpu_npoint;
	/*! CPU use by dot product (units??). Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, long> cpu_dot;
//	              __   ___     __        ___  __                 __  
//	|  |\/|  /\  / _` |__     /  \ \  / |__  |__) |     /\  \ / /__` 
//	|  |  | /~~\ \__> |___    \__/  \/  |___ |  \ |___ /~~\  |  .__/ 
//	                                                                                                      
// Images can have overlays of the analysis region border and the analysis results.
// These overlays are only in the network data , they are NOT saved to disc.
public:
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
protected:
	/*! Results-overlay status. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, STATE> analysis_result_overlay;
	///*! Cross-overlay status. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, STATE> cross_hair_overlay;
	///*! Mask-overlay status. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, STATE> analysis_mask_overlay;
//	              __   ___     __        ___  ___  ___  __  
//	|  |\/|  /\  / _` |__     |__) |  | |__  |__  |__  |__) 
//	|  |  | /~~\ \__> |___    |__) \__/ |    |    |___ |  \ 
//	  
//  Cameras continually buffer the last 100 frames whcih can be dumped (with the next 10 frames)  
public:
	/*! Save the Camera Image buffer to disk (typically the last 100 shots and the next 10 after triggered).
	@param[out] bool, if command got sent to EPICS (not if it was accepted)	*/
	bool saveImageBuffer();
	/*! Get the buffer trigger value.
	@param[out] double, value */
	char getImageBufferTrigger()const;
	/*! Get the buffer dump file-path.
	@param[out] double, value */
	std::string getImageBufferFilePath()const;
	/*! Get the buffer dump file-name.
	@param[out] double, value */
	std::string getImageBufferFileName()const;
	/*! Get the buffer dump file-number.
	@param[out] double, value */
	long getImageBufferFileNumber()const;
	///*! Set the buffer trigger to dump the camera image buffer to disc.
	//@param[out] bool, true if value got sent to epics (not if it was received)*/
	//bool triggerImageBuffer();
	/*! Get the last directory / filename that the HDF5 buffer data was saved to.
	@param[out] string, value */
	std::string getLastImageBufferDirectoryandFileName() const;
	/*! Get the last directory that the HDF5 buffer data was saved to.
	@param[out] string, value */
	std::string getLastImageBufferDirectory()const;
	/*! Get the last filename that the HDF5 buffer data was saved to.
	@param[out] string, value */
	std::string getLastImageBufferFileName()const;
private:
	/* Set the directory and file name for the Camera Image Buffer dump.
	@param[out] bool, if the value got sent to epics */
	bool makeANewDirectoryAndNameBuffer();
protected:
	/*! Camera circular buffer trigger PROC. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, char> image_buffer_trigger;
	/*! Latest filepath to write camera buffer data to. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, std::string> image_buffer_filepath;
	/*! Latest filename to write camera buffer data to. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, std::string> image_buffer_filename;
	/*! Latest file number for camera buffer data to. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, long> image_buffer_filenumber;
//              __   ___     __        __  ___       __   ___               __      __             _ 
//|  |\/|  /\  / _` |__     /  `  /\  |__)  |  |  | |__) |__      /\  |\ | |  \    /__`  /\  \  / |__  
//|  |  | /~~\ \__> |___    \__, /~~\ |     |  \__/ |  \ |___    /~~\ | \| |__/    .__/ /~~\  \/  |___ 
//                                                                                                     
// for capturing images and saving to disk 
//
public:
	/*! Set the number of shots to capture when "collecting frames and writing to disc."
	@param[out] bool, if requested number is less than max_shots, and the value got sent to epics */
	bool setNumberOfShotsToCapture(size_t num);
	/*! Get the number of shots that will be "collected and written to disk."
	@param[out] bool, if requested number is less than max_shots, and the value got sent to epics */
	size_t getNumberOfShotsToCapture()const;
	/*! Capture and save images to disc, using the currently set number of shots to capture.
	@param[out] bool, if command got sent to EPICS (not if it was accepted)	*/
	bool captureAndSave();
	/*! Capture and save images to disc.
	@param[in] size_t, num_images, number of images to capture and write to file
	@param[out] bool, if command got sent to EPICS (not if it was accepted)	*/
	bool captureAndSave(size_t num_images);
	/*! Get the last directory / filename that HDF5 data was saved to.
	@param[out] string, value */
	std::string getLastDirectoryandFileName() const;
	/*! Get the last directory that HDF5 data was saved to.
	@param[out] string, value */
	std::string getLastDirectory()const;
	/*! Get the last filename that HDF5 data was saved to.
	@param[out] string, value */
	std::string getLastFileName()const;
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
	/*! Did the last capture and save procedure complete succesfully?
	@param[out] bool	*/
	bool didLastCaptureAndSaveSucceed()const;
protected:
	/*! Maximum number of images that can be captured in a burst.*/
	size_t max_shots_number;
	/*! alternative names for the camera (usually shorter than the full PV root),
	defined in the Master Lattice*/
	std::vector<std::string> aliases;
	/*! Names of scnres that this camera views, defined in the Master Lattice*/
	std::vector<std::string> screen_names;
	/*! Flag set if the last capture and save routine manged to complete  */
	bool last_capture_and_save_success;
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
	/*! Camera set new background STATE. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, long > capture_count;
	/*! Number of images to capture. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, long> num_capture;
private:
	/*! ImageCapture class to hold data for image capturing and saving thread */
	ImageCapture image_capture;
	/*! Set the directory and file name for the next capture and save
	@param[out] bool, if the value got sent to epics */
	bool makeANewDirectoryAndName(size_t numbOfShots);
	/*! Funciton that captures and saves aimges to disc (run in new thread) */
	void imageCaptureAndSave(size_t num_shots);
	/*! Kills any completed image collect threads */
	void killFinishedImageCollectThread();
	/*! static function that runs the capture and save procedure
	* 	@param[in] ImageCapture, a struct with information used during the image capture and save procedure */
	static void staticEntryImageCollectAndSave(ImageCapture& ic);
	/*! Capture images
	@param[out] bool, if the value got sent to epics */
	bool capture();
	/*! Write images to disc
	@param[out] bool, if the value got sent to epics */
	bool write();
//	 __        __      
//	|__) |  | /__` \ / 
//	|__) \__/ .__/  |  
//	                   
// When the camera is collecting and saving this flag is set and some functionality is stopped until collecting and saving is complete
public:
	/*! Is the camera busy doing some collect, capture, save, write procedure, busy == true
	* while busy attempts to write more data to disc will fail.
	@param[out] bool	*/
	bool isBusy()const;
	/*! Is the camera NOT busy doing some collect, capture, save, write procedure, busy == false
	* while busy attempts to write more data to disc will fail.
	@param[out] bool	*/
	bool isNotBusy()const;
private:
	/* Flag set if the camera is "busy" busy means performing any part of the capture and save routine,  */
	bool busy;
//	 __  ___       __  ___   /  __  ___  __   __           __   __          __          __  
//	/__`  |   /\  |__)  |   /  /__`  |  /  \ |__)     /\  /  ` /  \ |  | | |__) | |\ | / _` 
//	.__/  |  /~~\ |  \  |  /   .__/  |  \__/ |       /~~\ \__, \__X \__/ | |  \ | | \| \__> 
//	                                                                                        
//	Acquiring is whne the camera is cpaturing frames of data 
//	!!As a general rule to increase operational reliablity!!  stopAcquiringAndWait should be called befowe changing camera settings 
public:
	/*! Get IOC active camera limit. The maximum number of cameras that can be active for this IOC. 
	* Running at this limit does not imply that everything will run smoothly!)
	@param[out] double */
	double getActiveCameraLimit() const;
	/*! Get IOC active camera count. The number of cameras that are currently acquiring in this IOC.
	@param[out] double */
	double getActiveCameraCount() const;
	/*! Test if the active camera limit is greater than than the active camera count
	@param[out] bool */
	bool canStartCamera()const;
	/*! Stop image acquiring, and wait for the stop acquiring signal to be verified by the control system .
	* @param[in] size_t, timeout time in ms, (if no value is passed the default is 3000) 
	@param[out] bool, true if camera stopped before timeout ms, otherwise false*/
	bool stopAcquiringAndWait(size_t timeout = 3000);
	/*! Start image acquiring, if active_camera_count == active_camera_count this will likley not work. 
	@param[out] bool, if command got sent to EPICS (not if it was accepted)	*/
	bool startAcquiring();
	/*! Stop image acquiring.
	@param[out] bool, if command got sent to EPICS (not if it was accepted)	*/
	bool stopAcquiring();
	/*! Toggle the use Acquiring state between ACQUIRING / NOT_ACQUIRING 
	@param[out] bool, if command got sent to EPICS (not if it was accepted)	*/
	bool toggleAcquiring();
	/*! Is camera acquire state == ACQUIRING.
	@param[out] bool, result*/
	bool isAcquiring()const;
	/*! Is camera acquire state == NOT_ACQUIRING.
	@param[out] bool, result*/
	bool isNotAcquiring() const;
	/*! Get image acquire state .
	@param[out] STATE, value from acquire_state*/
	STATE getAcquireState()const;
	/*! Get the Acquire Time for the camera (shutter open time, units??).
	@param[out] double, value */
	double getAcquireTime()const;
	/*! Get the Acquire Period for the camera (shutter open period, units??).
	@param[out] double, value */
	double getAcquirePeriod()const;
	/*! Get the Array Rate for the camera (repetition rate, Hz).
	@param[out] double, value */
	double getArrayRate()const;
protected:
	/*! Camera acquire time. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, double > acquire_time;
	/*! Camera acquire period. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, double > acquire_period;
	/*! Camera array rate. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, double > array_rate;
	/*! Active camera limit for this IOC.	*/
	std::pair<epicsTimeStamp, double > active_camera_limit;
	/*! Active camera count for this IOC.	*/
	std::pair<epicsTimeStamp, double > active_camera_count;
	/*! Acquire status. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, STATE> acquire_state;
private:
	/*! Struct to hold data for the thread called by stopAcquiringAndWait */
	CamStopWaiter cam_stop_acquiring_waiter_struct;
	/*! Function in which the stop acquiring and wait procedure runs (in a new thread).
	@param[in] CamStopWaiter, struct holding data for the procedure*/
	static void staticEntryWaitForCamStopAcquiring(CamStopWaiter& csw);
//		          __   ___                             __     __  
//	|  |\/|  /\  / _` |__      /\  |\ |  /\  |    \ / /__` | /__` 
//	|  |  | /~~\ \__> |___    /~~\ | \| /~~\ |___  |  .__/ | .__/ 
//                                                              
private:
	/*! Function in which the stop analyzing and wait procedure runs (in a new thread).
	@param[in] CamStopWaiter, struct holding data for the procedure*/
	/*! Struct to hold data for the thread called by stopAcquiringAndWait */
	CamStopWaiter cam_stop_analysing_waiter_struct;
	static void staticEntryWaitForCamStopAnalyzing(CamStopWaiter& csw);
public:
	/*! Start image analysis.
	@param[out] bool, if command got sent to EPICS (not if it was accepted)	*/
	bool startAnalysing();
	/*! Stop image analysis.
	@param[out] bool, if command got sent to EPICS (not if it was accepted)	*/
	bool stopAnalysing();
	/*! Stop Analyzing, and wait for the signal to be verified by the control system .
	* @param[in] size_t, timeout time in ms, (if no value is passed the default is 3000)
	@param[out] bool, true if camera stopped before timeout ms, otherwise false*/
	bool stopAnalysingAndWait(size_t timeout);
	/*! Toggle the image analysis between ANALYSIING / NOT_ANALYSIING.
	@param[out] bool, true if value got sent to epics (not if it was received)*/
	bool toggleAnalysing();
	/*! Is camera analysis state == ANALYSING.
	@param[out] bool, result*/
	bool isAnalysing()const;
	/*! Is camera analysis state == NOT_ANALYSING.
	@param[out] bool, result*/
	bool isNotAnalysing() const;
	/*! Get image analysis state .
	@param[out] STATE, value from analysis_state*/
	STATE getAnalysisState()const;
	/*! Check if the image analysis results are updating.
	@param[out] bool, ture if updating */
	bool isAnalysisUpdating();

protected:
	/*! Analysis status. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, STATE> analysis_state;
private:
	bool epics_startAnalysing(epicsUInt16 v);
	bool epics_stopAnalysing(epicsUInt16 v);
//	                         __     __           __     __   ___     ___       __   __   __  
//	 /\  |\ |  /\  |    \ / /__` | /__`    |\ | /  \ | /__` |__     |__  |    /  \ /  \ |__) 
//	/~~\ | \| /~~\ |___  |  .__/ | .__/    | \| \__/ | .__/ |___    |    |___ \__/ \__/ |  \ 
//	                                                                                         
protected:
	/*! Image analysis is floor being used or not. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, STATE> use_floor;
	/*! Image analysis  Floor count level. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, long> floor_level;
	/*! Number of points rejected by floor. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, long> floored_pts_count;
	/*! Percentage of points rejected by floor. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, double> floored_pts_percent;
public:
	/*! Set the using a Noise Floor during image analysis to true .
	@param[out] bool, true if value got sent to epics (not if it was received)*/
	bool setUseFloor();
	/*! Set the using a Floor during image analysis to false.
	@param[out] bool, true if value got sent to epics (not if it was received)*/
	bool setDoNotUseFloor();
	/*! Toggle the use floor state between USING_FLOOR / NOT_USING_FLOOR (values below the floor level get set to zero).
	@param[out] bool, true if value got sent to epics (not if it was received)*/
	bool toggleUseFloor();
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
private:
	bool epics_setUseFloor(epicsUInt16 v);
	bool epics_setDoNotUseFloor(epicsUInt16 v);
	bool epics_setFloorLevel(long v);
	//	                         __     __           __   __         ___     __   __                    __  
	//	 /\  |\ | |     /\  \ / /__` | /__`    |\ | |__) /  \ | |\ |  |     /__` /  `  /\  |    | |\ | / _` 
	//	/~~\ | \| |___ /~~\  |  .__/ | .__/    | \| |    \__/ | | \|  |     .__/ \__, /~~\ |___ | | \| \__> 
	//	                                                                                                    
public:
	/*! Set use Npoint Scaling, (for more details see  VELA-EN-20150818 Method of Estimating Beam Size from VELA Screen Images). 
	@param[out] bool, true if value set correctly, false could mean you are trying to set a value for PHYSICAL CLARA   */
	bool setUseNPointScaling();
	/*! Set do not use Npoint Scaling, (for more details see  VELA-EN-20150818 Method of Estimating Beam Size from VELA Screen Images).
	@param[out] bool, true if value set correctly, false could mean you are trying to set a value for PHYSICAL CLARA   */
	bool setDoNotUseNPointScaling();
	/*! Toggle the use floor state between USING_FLOOR / NOT_USING_FLOOR (values below the floor level get set to zero).
	@param[out] bool, true if value got sent to epics (not if it was received)*/
	bool toggleUseNPointScaling();
	/*! get the use Npoint scaling state
	@param[out] STATE, */
	STATE getNPointScalingState()const;
	/*! check if analysis is using NPoint scaling
	@param[out] bool, true if using NPoint scaling*/
	bool isUsingNPointScaling()const;
	/*! check if analysis is Not using NPoint scaling
	@param[out] bool, true if using NPoint scaling*/
	bool isNotUsingNPointScaling()const;
	/*! get the Npoint scaling stepsize
	@param[out] long, value */
	long getNpointScalingStepSize()const;
	/*! set the Npoint scaling stepsize
	@param[in] long, new stepsize
	@param[out] bool, true if value got sent to epics (not if it was received)*/
	bool setNpointScalingStepSize(long val);
protected:
	/*! State of using NPoint scaling. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, STATE > use_npoint;
	/*! latest stepsize (Npoint scaling step size). Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, long > step_size;
private:
	bool epics_setUseNPointScaling(epicsUInt16 v);
	bool epics_setDoNotUseNPointScaling(epicsUInt16 v);
	bool epics_setNpointScalingStepSize(long val);


	//                          __     __      __        __        __   __   __             __     
	//  /\  |\ | |     /\  \ / /__` | /__`    |__)  /\  /  ` |__/ / _` |__) /  \ |  | |\ | |  \    
	// /~~\ | \| |___ /~~\  |  .__/ | .__/    |__) /~~\ \__, |  \ \__> |  \ \__/ \__/ | \| |__/    
	//                                                                                            
public:
	/*! Sets the next captured image to be the background image, that can be subtracted during the analysis routine.
	@param[out] bool, true if value set correctly, false could mean you are trying to set a value for PHYSICAL CLARA   */
	bool setNewBackgroundImage();
	/*! The stored background image will  be subtracted from the image data before the analysis calculation.
	@param[out] bool, true if value set correctly, false could mean you are trying to set a value for PHYSICAL CLARA   */
	bool setUseBackgroundImage();
	/*! The stored background image will NOT be subtracted from the image data before the analysis calculation.
	@param[out] bool, true if value set correctly, false could mean you are trying to set a value for PHYSICAL CLARA   */
	bool setDoNotUseBackgroundImage();
	/*! Toggle subtractign the stored background image.
	@param[out] bool, true if value got sent to epics (not if it was received)*/
	bool toggleUseBackgroundImage();
	/*! get the use background state
	@param[out] STATE, */
	STATE getUsingBackgroundImageState()const;
	/*! Check if analysis is using a background image.
	@param[out] bool, true if using background image during analysis */
	bool isUsingBackgroundImage()const;
	/*! Check if analysis is Not using a background image.
	@param[out] bool, true if NOT using background image during analysis*/
	bool isNotUsingBackgroundImage()const;
protected:
	/*! Camera set new background STATE. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, STATE > set_new_background;
	/*! State of background data  scaling. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, STATE > use_background;
private:
	bool epics_setUseBackgroundImage(epicsUInt16 v);
	bool epics_setDoNotUseBackgroundImage(epicsUInt16 v);
	//	                         __     __                 __       
	//	 /\  |\ |  /\  |    \ / /__` | /__`     |\/|  /\  /__` |__/ 
	//	/~~\ | \| /~~\ |___  |  .__/ | .__/     |  | /~~\ .__/ |  \ 
	//	                                                            
	//	The 1st and 2nd central moments about the mean (with simple background subtraction) can be performed on a defined sub-region (the analysis mask)  
	//	These function get / set that mask 
protected:
	/*! analaysis mask center x position (pixels). Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, long> mask_x_center;
	/*! analaysis mask center y position (pixels). Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, long> mask_y_center;
	/*! analaysis mask center x radius (pixels). Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, long> mask_x_radius;
	/*! analaysis mask center y radius (pixels). Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, long> mask_y_radius;
public:
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
	//	 __   __    
	//	|__) /  \ | 
	//	|  \ \__/ | 
	//	            
	// A region of interest can be defined, and the array data in this ROI is written to a sperate PV 
	//
protected:
	/*! Rectangular ROI X corner, ROI used to extract masked data. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, long> roi_min_x;
	/*! Rectangular ROI Y corner, ROI used to extract masked data. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, long> roi_min_y;
	/*! Rectangular ROI X size, ROI used to extract masked data. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, long> roi_size_x;
	/*! Rectangular ROI Y size, ROI used to extract masked data. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, long> roi_size_y;
public:
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
	bool setROI(long x_min, long  y_min, long x_size, long y_size);
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
	//	 __   __                  __                 __           __   __         __   __  
	//	|__) /  \ |     /\  |\ | |  \     |\/|  /\  /__` |__/    /  ` /  \  |\/| |__) /  \ 
	//	|  \ \__/ |    /~~\ | \| |__/     |  | /~~\ .__/ |  \    \__, \__/  |  | |__) \__/ 
	//	                                                                                   
protected:
	/*! Rectangular ROI AND Mask X centre. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, double> roi_and_mask_centre_x; // MAY NOT NEED AS MONITORS
	/*! Rectangular ROI AND Mask Y centre. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, double> roi_and_mask_centre_y; // MAY NOT NEED AS MONITORS
	/*! Rectangular ROI AND Mask X radius. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, double> roi_and_mask_radius_x;
	/*! Rectangular ROI AND Mask Y radius. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, double> roi_and_mask_radius_y;
public:
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
//	       ___   __   
//	|     |__   |  \  
//	|___ .|___ .|__/ .
//	                  
//  screen / cameras have LEDs to ilimunate them 
private:
	/* does this camera have an LED */
	bool has_led;
protected:
	/*! LED status. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, STATE> led_state;
public:
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
//	 __                            __      __             __                ___       ___      
//	/ _`  /\  | |\ |     /\  |\ | |  \    |__) |     /\  /  ` |__/    |    |__  \  / |__  |    
//	\__> /~~\ | | \|    /~~\ | \| |__/    |__) |___ /~~\ \__, |  \    |___ |___  \/  |___ |___ 
//	                                                                                           
protected:
	/*! Camera gain, for VELA_CAMERA type only. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, long > gain;
	/*! Camera black_level, for VELA_CAMERA type only. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, long > black_level;
public:
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
	//	 __             __   __        __  ___    
	//	/__` |\ |  /\  |__) /__` |__| /  \  |     
	//	.__/ | \| /~~\ |    .__/ |  | \__/  |     
	//
public:
	HardwareSnapshot getSnapshot();
	boost::python::dict getSnapshot_Py();

//	 __         ___          __   __            ___  __  
//	|__) | \_/ |__  |       /  ` /  \ |  | |\ |  |  /__` 
//	|    | / \ |___ |___    \__, \__/ \__/ | \|  |  .__/ 
//	                                                     
public:
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
protected:
	/*! latest Sum Intensity (sum of pixel values). Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, double > sum_intensity; // just intensity in pv name
	/*! latest Average Intensity (mean of pixel values). Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, double > avg_intensity;
/*
				  __   ___                             __     __
	|  |\/|  /\  / _` |__      /\  |\ |  /\  |    \ / /__` | /__`
	|  |  | /~~\ \__> |___    /~~\ | \| /~~\ |___  |  .__/ | .__/

*/
public:
// THESE ARE JUST FOR ANLAYSIS RESULTS WHEN USING VIRTUAL CLARA 
	/*! Get relevant analysis data in a map
	@param[out] std::map<std::string, double> */
	std::map<std::string, double> getAnalayisData() const;
	/*! Get relevant analysis data in a python dict
	@param[out] dict of analysis data */
	boost::python::dict getAnalayisData_Py() const;
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
	/*! get Anlaysis results in pixels
	@param[out] map<string, double>, values, keyed by names in master lattice */
	std::map<std::string, double> getAnalysisResultsPixels()const;
	/*! get Anlaysis results in pixels, Python version
	@param[out] map<string, double>, values, keyed by names in master lattice */
	boost::python::dict getAnalysisResultsPixels_Py()const;
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
	boost::python::list getMMResults_Py();
protected:
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
	/*! Pixel Results from Analysis: [x_position, y_position, x_sigma_position, y_sigma_position, covariance_pos]*/
	std::pair<epicsTimeStamp, std::vector<double> > pixelResults;
	/*! mm Results from Analysis: [x_position, y_position, x_sigma_position, y_sigma_position, covariance_pos]*/
	std::pair<epicsTimeStamp, std::vector<double> > mmResults;
	/*! Stores the last time the ANA Pixel Results were updated, will be used to set isResultUpdated*/
	epicsTimeStamp lastResultsUpdateTime;
	/*! Stores the last time the ANA MM Results were updated, purely used fro pushign to the running stats */
	epicsTimeStamp lastResultsUpdateTime_mm_ana_results;
/*
//							__     __      __                         __      __  ___      ___  __
//  /\  |\ |  /\  |    \ / /__` | /__`    |__) |  | |\ | |\ | | |\ | / _`    /__`  |   /\   |  /__`
// /~~\ | \| /~~\ |___  |  .__/ | .__/    |  \ \__/ | \| | \| | | \| \__>    .__/  |  /~~\  |  .__/	*/
// 
public:
	/*! Get the image analysis running stats data.
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
	/*! Clear all the values assoociated with the Running mean and variance stats.*/
	void clearAllRunningStats();
	/*! Set the size of all Running mean and variance stats.*/
	void setAllRunningStatSizes(size_t new_val);
	/*! Check if all running stats in the camera are full.
	* 	@param[out] bool, */
	bool areAllRunningStatsFull()const;
protected:
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
private:
	/*! analysis data results, x, y , sigma_x, sigma_y, sigma_xy (can be re-defined in config file ) */
	std::pair<epicsTimeStamp, std::vector<double>> analysis_data;
	/*! names for each element in the pixel results array, defined in the master lattice config file  */
	std::vector<std::string> analysis_data_names;
//	 	
//	      __   __       ___  ___               __      __   ___ ___                  __   ___     __       ___         
//	|  | |__) |  \  /\   |  |__      /\  |\ | |  \    / _` |__   |     |  |\/|  /\  / _` |__     |  \  /\   |   /\     
//	\__/ |    |__/ /~~\  |  |___    /~~\ | \| |__/    \__> |___  |     |  |  | /~~\ \__> |___    |__/ /~~\  |  /~~\    
//	                                                                                                                   
public:
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
	/*! Get a copy of the current image data (Numpy Python Version). Until an updateImage functiton is called this will be empty,
	to reduce network load Camera data arrays ARE NOT continuously monitored.
	@param[out] list, latest data */
	boost::python::numpy::ndarray getImageData_NumPy()const;
	/*! Get a copy of the current Region Of Interest data. Until an updateROI function is called this will be empty,
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
private:
	/*! Get the latest time stamp for an image / ROI  array
	@param[in] struct dbr_time_long*, pointer to DBR_TIME_LONG struct
	@param[in] const pvStruct&, pvStruct with channel information
	@param[in] epicsTimeStamp&, value to update with timestamp
	@param[out] bool, if command got sent to EPICS (not if it was accepted)	*/
	bool getArrayTimeStamp(struct dbr_time_long* dbr_struct, const pvStruct& pvs, epicsTimeStamp& ts_to_update);
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
//       ___  __   __        __   ___  __  
// |\/| |__  /__` /__`  /\  / _` |__  /__` 
// |  | |___ .__/ .__/ /~~\ \__> |___ .__/ 
//                                         
public:
	/*! Enable debug messages*/
	void debugMessagesOn();
	/*! Disbale debug messages*/
	void debugMessagesOff();
	/*! Enable messages*/
	void messagesOn();
	/*! Disable messages*/
	void messagesOff();


protected:
	/* 
		In general we put data that comes back from EPICS into a timestamp, data pair. 
		This is because we expect to be able to use the time stamps to synchonize different data streams 
	*/



	/*! Flag for showing that the ANA Pixel Results are updating */
	bool isResultUpdated;
	/*! ROI image data. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, std::vector<long>> roi_imagedata;

	/*! Camera temperature. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, double > temperature;

	/*! pointer to dbr_time_stuct, used to get timestmp for images*/
	struct dbr_time_long* dbr_image_data;
	/*! image_data vector to hold image data */
	std::pair<epicsTimeStamp, std::vector<long>> image_data;
	/*! pointer to dbr_time_stuct, used to get timestmp for roi*/
	struct dbr_time_long* dbr_roi_data;
	/*! roi_data vector to hold image data */
	std::pair<epicsTimeStamp, std::vector<long>> roi_data;







	/*! Keywords used for setting ROI and mask together, python version (OUR PREFFERRED INTERFACE!) */
	//boost::python::list mask_and_roi_keywords_Py;
	/*! Keywords used for setting mask, python version  (Not prefferred, use mask and ROI together) */
	//boost::python::list mask_keywords_Py;


	// protected so cam-factory can get these easily 
	double master_lattice_pixel_to_mm;
	long  master_lattice_centre_x;
	long  master_lattice_centre_y;

private:


	
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

	EPICSCameraInterface_sptr epicsInterface;
	std::map<std::string, std::string> CameraParamMap;

	// TODO make const at some point (HA!) 
	std::string roi_min_x_str; // This actually could be MAX! waiting on SK to update co-ord system! :( smh 
	std::string roi_min_y_str;
	std::string roi_x_size_str;
	std::string roi_y_size_str;
	std::string mask_x_str;
	std::string mask_y_str;
	std::string mask_rad_x_str;
	std::string mask_rad_y_str;
	std::string mask_and_roi_x_str;
	std::string mask_and_roi_y_str;
	std::string mask_and_roi_x_size_str;
	std::string mask_and_roi_y_size_str;
	/*! Keywords used for setting ROI and mask together (OUR PREFFERRED INTERFACE!) */
	std::vector<std::string> mask_and_roi_keywords;
	/*! Keywords used for setting mask (Not prefferred, use mask and ROI together) */
	std::vector<std::string> mask_keywords;
	/*! Keywords used for setting ROI (Not preffeered, use mask and ROI together) */
	std::vector<std::string> roi_keywords;


	/*! Set up the PV structs */
	void setPVStructs();
	/*! Parse the Master Lattice data for this camera */
	void getMasterLatticeData(const std::map<std::string, std::string>& paramMap, STATE mode);
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






	// TODO Now the templated fucniton pointers seem to work well - even more tidying up could be done 

	//template<typename T>
	//using epics_caput_function_ptr2 = bool(Camera::*)(T);
	//template<typename T> 
	//bool genericStopAcquiringApplySetting2(bool(Camera::*)(T), T value)
	// https://stackoverflow.com/questions/9779105/generic-member-function-pointer-as-a-template-parameter
	//template <typename T, typename ...Args>
	//bool genericStopAcquiringApplySetting2(bool(Camera::* mf)(Args...), Args &&... args)
	//{
	//	return (this.*mf)(std::forward<Args>(args)...);
	//}
	////https://isocpp.org/wiki/faq/pointers-to-members
	//typedef bool(Camera::* epics_caput_function_ptr)();
	//bool genericStopAcquiringApplySetting(epics_caput_function_ptr f);#
	//bool Camera::genericStopAcquiringApplySetting(epics_caput_function_ptr f)
	//{
	//	/* is the camera currently acquiring and analyzing? */
	//	bool is_acquiring_at_start = isAcquiring();
	//	bool is_analyzing_at_start = isAnalysing();
	//	/* flag to store result of stop acquiring attempt */
	//	bool stopped_acquiring = !is_acquiring_at_start;
	//	//messenger.printDebugMessage(hardwareName, " is_acquiring_at_start = ", is_acquiring_at_start, ", is_analyzing_at_start = ", is_analyzing_at_start);
	//	if (is_acquiring_at_start)
	//	{
	//		//messenger.printDebugMessage("is_acquiring_at_start, therefore stopAcquiringAndWait");
	//		stopped_acquiring = stopAcquiringAndWait();
	//	}
	//	if (stopped_acquiring)
	//	{
	//		//messenger.printDebugMessage("stopAcquiringAndWait success, now calling passed member function ptr ");
	//		bool sent_use_floor = (*this.*f)(); // TODO use std::invoke ?? 
	//		//bool sent_use_floor = (*this.*f)();
	//		if (sent_use_floor)
	//		{
	//			if (is_acquiring_at_start)
	//			{
	//				//messenger.printDebugMessage("is_acquiring_at_start, startAcquiring");
	//				return startAcquiring();
	//			}
	//			return true;
	//		}
	//	}
	//	else
	//	{
	//		//messenger.printDebugMessage("!!Failed!! stopAcquiringAndWait");
	//	}
	//	return false;
	//}
	template <typename T>
	bool genericStopAcquiringApplySetting(bool(Camera::*mf)(T), T v)
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
			stopped_acquiring = stopAcquiringAndWait(5000);// for gcc this arguement seems to be required!!!
		}
		if (stopped_acquiring)
		{
			//messenger.printDebugMessage("stopAcquiringAndWait success, now calling passed member function ptr ");
			bool sent_use_floor = (*this.*mf)(std::forward<T>(v));
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


};
#endif //CAMERA_H_