#ifndef CAMERA_FACTORY_H_
#define CAMERA_FACTORY_H_

#include <ConfigReader.h>
#include <Camera.h>
#include <GlobalStateEnums.h>
#include <vector>
class CameraFactory
{
public:
	CameraFactory();
	CameraFactory(STATE mode);
	CameraFactory(STATE mode, const std::string& primeLatticeLocation);
	CameraFactory(const CameraFactory& copyFactory);
	~CameraFactory();
	LoggingSystem messenger;

	/*! default setup function, uses default values to read files and connect to EPICS etc.
	@param[out] bool, for success or failure	*/
	bool setup();
	/*! setup function using version parameter
	@param[in] version (a placeholder for future extensions)
	@param[out] bool, for success or failure	*/
	bool setup(const std::string& version);
	/*! setup function using machineArea parameter
	@param[in] machineArea, only setup magnets that match machineArea
	@param[out] bool, for success or failure	*/
	bool setup(TYPE machineArea);
	/*! setup function using version and machineArea parameters
	@param[in] version (a placeholder for future extensions)
	@param[in] machineArea, only setup magnets that match machineArea
	@param[out] bool, for success or failure	*/
	bool setup(const std::string& version, TYPE machineArea);
	/*! setup function using std::vector of machineAreas
	@param[in] machineAreas, only setup magnets that match an area in machineAreas
	@param[out] bool, for success or failure	*/
	bool setup(const std::vector<TYPE>& machineAreas);
	/*! setup function using python::list of machineAreas
	@param[in] machineAreas, only setup magnets that match an area in machineAreas
	@param[out] bool, for success or failure	*/
	bool setup(const boost::python::list& machineAreas);
	/*! setup function using python::list of machineAreas
	@param[in] version, (a placeholder for future extensions)
	@param[in] machineAreas, only setup magnets that match an area in machineAreas
	@param[out] bool, for success or failure	*/
	bool setup(const std::string& version, const boost::python::list& machineAreas);
	/*! setup function using std::vector of machineAreas
	@param[in] version, (a placeholder for future extensions)
	@param[in] machineAreas, only setup magnets that match an area in machineAreas
	@param[out] bool, for success or failure	*/
	bool setup(const std::string& version, const std::vector<TYPE>& machineAreas);
	/*! setup function using std::vector of camera names 
		@param[in] version, (a placeholder for future extensions)
		@param[in] names, only setup magnets that match a (fullname) in names 
		@param[out] bool, for success or failure	*/
	bool setup(const std::string& version, const std::vector<std::string>& names);
	/*! get a reference to a camera object
	@param[in] cam_name, name of camera object to return
	@param[out] camera object*/
	Camera& getCamera(const std::string& cam_name);
	//
	//                   ___  __  
	//  |\ |  /\   |\/| |__  /__` 
	//  | \| /~~\  |  | |___ .__/ 
	//
	/*! get the names of all the cameras in Factory 
	@param[out] vector of strings, camera names*/
	std::vector<std::string> getCameraNames();
	/*! get the names of all the cameras in Factory, python version
	@param[out] python list of strings, camera names*/
	boost::python::list getCameraNames_Py();
	/*! get the name alises for this Camera
	@param[in] std::string, name
	@param[out] names, vector containing  all the alias names */
	std::vector<std::string> getAliases(const std::string& name) const;
	/*! get the name alises for this Camera (python version)
	@param[in] std::string, name
	@param[out] names, python list containing all the alias names */
	boost::python::list getAliases_Py(const std::string& name) const;
	/*! get the screen name (and aliases) the camera is attached to
	@param[in] std::string, name
	@param[out] names, vector containing  all the screen names (and their aliases) */
	std::vector<std::string> getScreenNames(const std::string& name) const;
	/*! get the screen name (and aliases) the camera is attached to (python version)
	@param[in] std::string, name
	@param[out] names, python list containing all the screen names (and their aliases) */
	boost::python::list getScreenNames_Py(const std::string& name) const;
	/*! get the screen name the camera is attached to (defined in master lattice)
	@param[in] std::string, name
	@param[out] name, */
	std::string getScreen(const std::string& name)const;
	// 
	//  __         ___         ___  __                 
	// |__) | \_/ |__  |        |  /  \     |\/|  |\/| 
	// |    | / \ |___ |___     |  \__/     |  |  |  | 
	//  
	/*! Get the pixel to mm conversion factor,
	@param[in] std::string, name
	@param[out] double, value */
	double getPixelToMM(const std::string& name)const;
	/*! Set the pixel to mm conversion factor,
	@param[in] std::string, name
	@param[in] double, value 
	@param[out] bool, if command got sent to EPICS (not if it was accepted) */
	bool setPixelToMM(const std::string& name, double value);
	/*! Convert a length in pixels to mm.
	@param[in] std::string, name
	@param[in] double, length in pixels
	@param[out] double, length in mm*/
	double pix2mm(const std::string& name, double value)const;
	/*! Convert a length in mm to pixels.
	@param[in] std::string, name
	@param[in] double, length in mm
	@param[out] double, length in pixels*/
	double mm2pix(const std::string& name, double value)const;
	/*! convert a horizontal length in pixels to mm for this camera
	@param[in] std::string, name
	@param[in] length in pixels
	@param[out] length in mm*/
	double pix2mmX(const std::string& name, double value)const;
	/*! convert a vertical length in pixels to mm for this camera
	@param[in] std::string, name
	@param[in] length in pixels
	@param[out] length in mm*/
	double pix2mmY(const std::string& name, double value)const;
	/*! convert a horizontal length in mm to pixels for this camera
	@param[in] std::string, name
	@param[in] length in pixels
	@param[out] length in mm*/
	double mm2pixX(const std::string& name, double value)const;
	/*! convert a vertical length in mm to pixels for this camera
	@param[in] std::string, name
	@param[in] length in pixels
	@param[out] length in mm*/
	double mm2pixY(const std::string& name, double value)const;
	/*! Get the pixels to mm in x(horizontal) direction. (First set from the yaml config file)
	@param[in] std::string, name
	@param[out] double, value */
	double getpix2mmX(const std::string& name)const;
	/*! Get the pixels to mm in y (horizontal) direction. (First set from the yaml config file)
	@param[in] std::string, name
	@param[out] double, value */
	double getpix2mmY(const std::string& name)const;
	/*! Set the pixels to mm in x(horizontal) direction.
	@param[in] std::string, name
	@param[in] double, new value */
	double setpix2mmX(const std::string& name, double value);
	/*! Set the pixels to mm in y(horizontal) direction.
	@param[in] std::string, name
	@param[in] double, new value */
	double setpix2mmY(const std::string& name, double value);




	/*! Get the pixel bit depth for a cmaera,
	@param[in] std::string, name
	@param[in] size_t, value */
	size_t getBitDepth(const std::string& name);
	/*! Get required rotations of raw image array to match installed orentation for image (0, 90, 180 or 270 degrees).
	@param[in] std::string, name
	@param[in] size_t, value */
	size_t getImageRotation(const std::string& name);
	/*! Get should the image be flipped along the up/down axis (after rotation)  i.e. row order reversed)
	@param[in] std::string, name
	@param[in] size_t, value */
	bool getImageFlipUD(const std::string& name);
	/*! Get should the image be flipped along the left/right axis (after rotation) i.e. column order reversed)
	@param[in] std::string, name
	@param[in] size_t, value */
	bool getImageFlipLR(const std::string& name);




	//    __   ___      ___  ___  __      __   __      __                      __      __   __      __   __   __  
	//   /  ` |__  |\ |  |  |__  |__)    /  \ |__)    /  \ |\ | __  /\  \_/ | /__`    /  ` /  \ __ /  \ |__) |  \	
	//   \__, |___ | \|  |  |___ |  \    \__/ |  \    \__/ | \|    /~~\ / \ | .__/    \__, \__/    \__/ |  \ |__/ 
	//  
	/*! Get the X Pixel defined as the "horizontal" centre of the image data.
	* 	@param[in] std::string, name
	*	@param[out] long, value */
	long getCentreXPixel(const std::string& name)const;
	/*! Get the Y Pixel defined as the "vertical" centre of the image-array.
	* 	@param[in] std::string, name
	*	@param[out] long, value */
	long getCentreYPixel(const std::string& name)const;
	/*! Set the X Pixel to be defined as the "horizontal" centre of the image-array.
	* 	@param[in] std::string, name
	*	@param[in] long, value to set
	*	@param[out] bool, true if value got sent to epics (not if it was received)*/
	bool setCentreXPixel(const std::string& name,long value);
	/*! Set the Y Pixel to be defined as the "vertical" centre of the image-array.
	* 	@param[in] std::string, name
	*	@param[in] long, value to set
	*	@param[out] bool, true if value got sent to epics (not if it was received)*/
	bool setCentreYPixel(const std::string& name, long value);
	/*! Set the X and Y Pixels to be defined as the centre of the image-array.
	* 	@param[in] std::string, name
	*	@param[in] long, x value to set
	*	@param[in] long, y value to set
	*	@param[out] bool, true if value got sent to epics (not if it was received)*/
	bool setCentrePixels(const std::string& name,long x, long y);
	/*! Sets the analysis centre to be the main lattice values for the mechanical center (in pixels).
	* 	@param[in] std::string, name
	*   Sets the analysis centre to be the main lattice values.
	*	@param[out] bool, if the value got sent to epics.*/
	bool setMechanicalCentre(const std::string& name);
	/*! For the virtual Cathode the centre of the screen can be defined using the RF centre of the injector.
	* 	@param[in] std::string, name
	*   Sets the analysis centre to be the main lattice values (in pixels).
	*	@param[out] bool, if the value got sent to epics.*/
	bool setOperatingCenter(const std::string& name);
	/*! Get the RF center X Pixel, from the Main Lattice.
	* 	@param[in] std::string, name
	*	@param[out] long, value */
	long getOperatingCentreXPixel(const std::string& name)const;
	/*! Get the RF center Y Pixel, from the Main Lattice.
	* 	@param[in] std::string, name
	*	@param[out] long, value */
	long getOperatingCentreYPixel(const std::string& name)const;
	/*! Get the mechanical center X Pixel, from the Main Lattice.
	* 	@param[in] std::string, name
	*	@param[out] long, value */
	long getMechanicalCentreXPixel(const std::string& name)const;
	/*! Get the mechanical center Y Pixel, from the Main Lattice.
	* 	@param[in] std::string, name
	*	@param[out] long, value */
	long getMechanicalCentreYPixel(const std::string& name)const;
	//  			  __   ___ __    __  ___
	//  |  |\/|  /\  / _` |__ /__` |  / |__
	//  |  |  | /~~\ \__> |___.__/ | /_ |___
	// 
	/*! Get the Number of pixels in the width of the (full) image.
	@param[in] std::string, name
	@param[out] long, value */
	long getPixelWidth(const std::string& name)const;
	/*! Get the Number of pixels in the height of the (full) image.
	@param[in] std::string, name
	@param[out] long, value */
	long getPixelHeight(const std::string& name)const;
		/*! Get the total number of pixels in X for the standard image sent over the network
	@param[in] std::string, name
	@param[out] size_t, value */
	size_t getArrayDataPixelCountX(const std::string& name)const;
	/*! Get the total number of pixels in Y for the standard image sent over the network
	@param[in] std::string, name
	@param[out] size_t, value */
	size_t getArrayDataPixelCountY(const std::string& name)const;
	/*! Get the total number of pixels in X for the full binary image saved to disc
	@param[in] std::string, name
	@param[out] size_t, value */
	size_t getBinaryDataPixelCountX(const std::string& name)const;
	/*! Get the total number of pixels in Y for the full binary image saved to disc
	@param[in] std::string, name
	@param[out] size_t, value */
	size_t getBinaryDataPixelCountY(const std::string& name)const;
	//	 __   __                __        __  
	//	/  ` |__) |  |    |    /  \  /\  |  \ 
	//	\__, |    \__/    |___ \__/ /~~\ |__/ 
	//	                                      
	/*! Get the total CPU usage of the camera.
	@param[in] std::string, name
	@param[out] long, value */
	long getCPUTotal(const std::string& name)const;
	/*! Get the CPU usage of the CropSubMask procedure.
	@param[in] std::string, name
	@param[out] long, value */
	long getCPUCropSubMask(const std::string& name)const;
	/*! Get the CPU usage of the NPoint scaling procedure.
	@param[in] std::string, name
	@param[out] long, value */
	long getCPUNPoint(const std::string& name)const;
	/*! Get the CPU usage of the dot product procedure.
	@param[in] std::string, name
	@param[out] long, value */
	long getCPUDot(const std::string& name)const;
	/*! Get the Acquire Time for the camera (shutter open time, units??).
	@param[in] std::string, name
	@param[out] double, value */
	//	              __   ___     __        ___  ___  ___  __  
	//	|  |\/|  /\  / _` |__     |__) |  | |__  |__  |__  |__) 
	//	|  |  | /~~\ \__> |___    |__) \__/ |    |    |___ |  \ 
	//
	/*! Get the buffer trigger value.
	@param[in] std::string, name
	@param[out] double, value */
	char getImageBufferTrigger(const std::string& name)const;
	/*! Get the buffer dump file-path.
	@param[in] std::string, name
	@param[out] double, value */
	std::string getImageBufferFilePath(const std::string& name)const;
	/*! Get the buffer dump file-name.
	@param[in] std::string, name
	@param[out] double, value */
	std::string getImageBufferFileName(const std::string& name)const;
	/*! Get the buffer dump file-number.
	@param[in] std::string, name
	@param[out] double, value */
	long getImageBufferFileNumber(const std::string& name)const;
	/*! Get the last directory / filename that the HDF5 buffer data was saved to.
	@param[in] std::string, name
	@param[out] string, value */
	std::string getLastImageBufferDirectoryandFileName(const std::string& name) const;
	/*! Get the last directory that the HDF5 buffer data was saved to.
	@param[in] std::string, name
	@param[out] string, value */
	std::string getLastImageBufferDirectory(const std::string& name)const;
	/*! Get the last filename that the HDF5 buffer data was saved to.
	@param[in] std::string, name
	@param[out] string, value */
	std::string getLastImageBufferFileName(const std::string& name)const;
	//	              __   ___     __        __  ___       __   ___               __      __             _ 
	//	|  |\/|  /\  / _` |__     /  `  /\  |__)  |  |  | |__) |__      /\  |\ | |  \    /__`  /\  \  / |__  
	//	|  |  | /~~\ \__> |___    \__, /~~\ |     |  \__/ |  \ |___    /~~\ | \| |__/    .__/ /~~\  \/  |___ 
	//	                                                                                                     
	/*! Set the number of shots that will be "collected and written to disk."
	@param[out] bool, if requested number is less than max_shots, and the value got sent to epics */
	bool setNumberOfShotsToCapture(const std::string& name, size_t num);
	/* set the number of shots that will be "collected and written to disk."
	@param[out] bool, if requested number is less than max_shots, and the value got sent to epics */
	size_t getNumberOfShotsToCapture(const std::string& name)const;
	/*! Capture and save images to disc, using the currently set number of shots to capture.
	@param[out] bool, if command got sent to EPICS (not if it was accepted)	*/
	bool captureAndSave(const std::string& name);
	/*! Capture and save images to disc.
	@param[in] std::string, name
	@param[in] size_t, num_images, number of images to capture and write to file
	@param[out] bool, if command got sent to EPICS (not if it was accepted)	*/
	bool captureAndSave(const std::string& name, size_t num_images);
	/*! Get Capture state.
	@param[in] std::string, name
	@param[out] STATE, value from capture_state*/
	STATE getCaptureState(const std::string& name)const;
	/*! Is camera capturing images, capture_state == CAPTURING.
	@param[in] std::string, name
	@param[out] bool*/
	bool isCapturing(const std::string& name)const;
	/*! Is camera capturing images, capture_state == NOT_CAPTURING.
	@param[in] std::string, name
	@param[out] bool*/
	bool isNotCapturing(const std::string& name)const;
	/*! Is camera writing data to file, write_state == WRITING.
	@param[in] std::string, name
	@param[out] bool*/
	bool isWriting(const std::string& name)const;
	/*! Is camera writing data to file, write_state == NOT_WRITING.
	@param[in] std::string, name
	@param[out] bool*/
	bool isNotWriting(const std::string& name)const;
	/*! Is camera capture_state == CAPTURING  OR is write_state == NOT_WRITING.
	@param[in] std::string, name
	@param[out] bool*/
	bool isCapturingOrSaving(const std::string& name)const;
	/*! Did the last capture and save procedure complete succesfully?
	* 	@param[in] std::string, name
	@param[out] bool	*/
	bool didLastCaptureAndSaveSucceed(const std::string& name)const;





	/*! Is the camera busy doing some collect, capture, save, write procedure, busy == true
	* while busy attempts to write more data to disc will fail.
	@param[in] std::string, name
	@param[out] bool	*/
	bool isBusy(const std::string& name);
	/*! Is the camera NOT busy doing some collect, capture, save, write procedure, busy == false
	* while busy attempts to write more data to disc will fail.
	@param[in] std::string, name
	@param[out] bool	*/
	bool isNotBusy(const std::string& name);



	/*! get Anlaysis results in pixels
	@param[in] std::string, name
	@param[out] map<string, double>, values, keyed by names in master lattice */
	std::map<std::string, double> getAnalysisResultsPixels(const std::string& name)const;
	/*! get Anlaysis results in pixels, Python version
	@param[in] std::string, name
	@param[out] map<string, double>, values, keyed by names in master lattice */
	boost::python::dict getAnalysisResultsPixels_Py(const std::string& name)const;

	/*! Set the black level (for vela camera types only),
	@param[in] std::string, name
	@param[in] long, values
	@param[out] bool, values */
	bool setBlackLevel(const std::string& name, long value);
	/*! Set the black level (for VELA camera types only),
	@param[in] std::string, name
	@param[out] bool, if command got sent to EPICS (not if it was accepted) */
	long getBlackLevel(const std::string& name )const;
	/*! Set the black level (for VELA camera types only),
	@param[in] std::string, name
	@param[out] long, latest value */
	bool setGain(const std::string& name, long value);
	/*! Get the gain (for VELA camera types only),
	@param[in] std::string, name
	@param[out] long, latest value */
	long getGain(const std::string& name )const;


	/*! get the type of the camera (e.g. vela_camera, clara_camera
	@param[in] std::string, name
	@param[out] type */
	TYPE getCamType(const std::string& name)const;


	// THESE ARE JUST FOR ANLAYSIS RESULTS WHEN USING VIRTUAL CLARA 
	/*! Set the x position in mm from the online analysis (only available VIRTUAL mode).
	@param[in] std::string, name
	@param[in] double, value
	@param[out] bool, true if value set correctly, false could mean you are trying to set a value for PHYSICAL CLARA   */
	bool setX(const std::string& name, double value);
	/*! Set the y position in mm for the online analysis (only available VIRTUAL mode).
	@param[in] std::string, name
	@param[in] double, value
	@param[out] bool, true if value set correctly, false could mean you are trying to set a value for PHYSICAL CLARA   */
	bool setY(const std::string& name, double value);
	/*! Set the x width in mm for the online analysis (only available VIRTUAL mode).
	@param[in] std::string, name
	@param[in] double, value
	@param[out] bool, true if value set correctly, false could mean you are trying to set a value for PHYSICAL CLARA   */
	bool setSigX(const std::string& name, double value);
	/*! Set the y width mm for the online analysis (only available VIRTUAL mode).
	@param[in] std::string, name
	@param[in] double, value
	@param[out] bool, true if value set correctly, false could mean you are trying to set a value for PHYSICAL CLARA   */
	bool setSigY(const std::string& name, double value);
	/*! Set the x-y correlation in mm for the online analysis (only available VIRTUAL mode).
	@param[in] std::string, name
	@param[in] double, value
	@param[out] bool, true if value set correctly, false could mean you are trying to set a value for PHYSICAL CLARA   */
	bool setSigXY(const std::string& name, double value);
	/*! Get the x position in mm from the online analysis.
	@param[in] std::string, name
	@param[out] double, value */
	double getX(const std::string& name)const;
	/*! Get the y position in mm from the online analysis.
	@param[in] std::string, name
	@param[out] double, value */
	double getY(const std::string& name)const;
	/*! Get the x width in mm from the online analysis.
	@param[in] std::string, name
	@param[out] double, value */
	double getSigX(const std::string& name)const;
	/*! Get the y width in mm from the online analysis.
	@param[in] std::string, name
	@param[out] double, value */
	double getSigY(const std::string& name)const;
	/*! Get the x-y coviariance in mm from the online analysis.
	@param[in] std::string, name
	@param[out] double, value */
	double getSigXY(const std::string& name)const;
	/*! Get the x position in pixels from the online analysis.
	@param[in] std::string, name
	@param[out] double, value */
	double getXPix(const std::string& name)const;
	/*! Get the y position in pixels from the online analysis.
	@param[in] std::string, name
	@param[out] double, value */
	double getYPix(const std::string& name)const;
	/*! Get the x width in pixels from the online analysis.
	@param[in] std::string, name
	@param[out] double, value */
	double getSigXPix(const std::string& name)const;
	/*! Get the y width in pixels from the online analysis.
	@param[in] std::string, name
	@param[out] double, value */
	double getSigYPix(const std::string& name)const;
	/*! Get the x-y coviariance in pixels from the online analysis.
	@param[in] std::string, name
	@param[out] double, value */
	double getSigXYPix(const std::string& name)const;



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


	double getAcquireTime(const std::string& name)const;
	/*! Get the Acquire Period for the camera (shutter open time, units??).
	@param[in] std::string, name
	@param[out] double, value */
	double getAcquirePeriod(const std::string& name)const;
	/*! Get the Array Rate for the camera (repetition rate, Hz).
	@param[in] std::string, name
	@param[out] double, value */
	double getArrayRate(const std::string& name)const;
	/*! Get the Sensor Temperature for the camera (degrees Celsius).
	@param[in] std::string, name
	@param[out] double, value */
	double getTemperature(const std::string& name)const;

	/*! Set the Region Of Interest minimum x pixel.
	@param[in] std::string, name
	@param[in] long, new val
	@param[out] bool, true if value got sent to epics (not if it was received)*/
	bool setROIMinX(const std::string& name, long val);
	/*! Set the Region Of Interest minimum y pixel.
	@param[in] std::string, name
	@param[in] long, new val
	@param[out] bool, true if value got sent to epics (not if it was received)*/
	bool setROIMinY(const std::string& name, long val);
	/*! Set the Region Of Interest size in x pixels.
	@param[in] std::string, name
	@param[in] long, new val
	@param[out] bool, true if value got sent to epics (not if it was received)*/
	bool setROISizeX(const std::string& name, long val);
	/*! Set the Region Of Interest size in y pixels.
	@param[in] std::string, name
	@param[in] long, new val
	@param[out] bool, true if value got sent to epics (not if it was received)*/
	bool setROISizeY(const std::string& name, long val);
	/*! Set the 4 Region Of Interest size in y pixels.
	@param[in] std::string, name
	@param[in] long, new x_pos value
	@param[in] long, new y_pos value
	@param[in] long, new x_size value
	@param[in] long, new y_size value
	@param[out] bool, true if value got sent to epics (not if it was received)*/
	bool setROI(const std::string& name, long x, long  y, long x_size, long y_size);
	/*! Set the Region Of Interest parameters.
	@param[in] std::string, name
	@param[in] map<std::string, long>, with new values (for keywords see roi_keywords)
	@param[out] bool, true if value got sent to epics (not if it was received)*/
	bool setROI(const std::string& name, std::map<std::string, long> settings);
	/*! Set the Region Of Interest parameters.
	@param[in] std::string, name
	@param[in] long, dict with new values, (for keywords see roi_keywords)
	@param[out] bool, true if value got sent to epics (not if it was received)*/
	bool setROI_Py(const std::string& name, boost::python::dict settings);
	/*! Get the Region Of Interest minimum x pixel.
	@param[in] std::string, name
	@param[out] long, value*/
	long getROIMinX(const std::string& name)const;
	/*! Get the Region Of Interest minimum y (pixels).
	@param[in] std::string, name
	@param[out] long, value*/
	long getROIMinY(const std::string& name)const;
	/*! Get the Region Of Interest minimum x (pixels).
	@param[in] std::string, name
	@param[out] long, value*/
	long getROISizeX(const std::string& name)const;
	/*! Get the Region Of Interest minimum y (pixels).
	@param[in] std::string, name
	@param[out] long, value*/
	long getROISizeY(const std::string& name)const;
	/*! Get the Region Of Interest values.
	@param[in] std::string, name
	@param[out] map<std::string, long>, with new values  */
	std::map<std::string, long> getROI(const std::string& name)const;
	/*! Get the Region Of Interest values.
	@param[in] std::string, name
	@param[out] dict, with new values  */
	boost::python::dict getROI_Py(const std::string& name)const;
	//	                         __     __           __     __   ___     ___       __   __   __  
	//	 /\  |\ |  /\  |    \ / /__` | /__`    |\ | /  \ | /__` |__     |__  |    /  \ /  \ |__) 
	//	/~~\ | \| /~~\ |___  |  .__/ | .__/    | \| \__/ | .__/ |___    |    |___ \__/ \__/ |  \ 
	//
	/*! Set the using a Floor during image analysis to true .
	@param[in] std::string, name
	@param[out] bool, true if value got sent to epics (not if it was received)*/
	bool setUseFloor(const std::string& name);
	/*! Set the using a Floor during image analysis to false.
	@param[in] std::string, name
	@param[out] bool, true if value got sent to epics (not if it was received)*/
	bool setDoNotUseFloor(const std::string& name);
	/*! Toggle the use florr state between USING_FLOOR / NOT_USING_FLOOR (values below the floor level get set to zero).
	@param[out] bool, true if value got sent to epics (not if it was received)*/
	bool toggleUseFloor(const std::string& name);
	/*! Set the Floor level to use during image analysis (value below floor get set to zero).
	@param[in] std::string, name
	@param[in] long, value to use
	@param[out] bool, true if value got sent to epics (not if it was received)*/
	bool setFloorLevel(const std::string& name, long v);
	/*! Get the state of use_floor.
	@param[in] std::string, name
	@param[in] long, value to use
	@param[out] bool, true if value got sent to epics (not if it was received)*/
	STATE getUseFloorState(const std::string& name)const;
	/*! Return true if use_floor == STATE::USING_FLOOR, otherwise false
	@param[in] std::string, name
	@param[out] bool, value*/
	bool isUsingFloor(const std::string& name)const;
	/*! Return true if use_floor == STATE::NOT_USING_FLOOR, otherwise false
	@param[in] std::string, name
	@param[out] bool, value*/
	bool isNotUsingFloor(const std::string& name)const;
	/*! get the level of the analysis floor
	@param[in] std::string, name
	@param[out] long, value*/
	long getFloorLevel(const std::string& name)const;
	/*! get the number of pixels that have been floored
	@param[in] std::string, name
	@param[out] long, value*/
	long getFlooredPtsCount(const std::string& name)const;
	/*! get the percentage of pixels that have been floored
	@param[in] std::string, name	
	@param[out] long, value*/
	double getFlooredPtsPercent(const std::string& name)const;
	//	                         __     __           __   __         ___     __   __                    __  
	//	 /\  |\ | |     /\  \ / /__` | /__`    |\ | |__) /  \ | |\ |  |     /__` /  `  /\  |    | |\ | / _` 
	//	/~~\ | \| |___ /~~\  |  .__/ | .__/    | \| |    \__/ | | \|  |     .__/ \__, /~~\ |___ | | \| \__> 
	//	                                                                                                    
	/*! Set use Npoint scaling. 
	@param[in] std::string, name
	@param[out] bool, true if value set correctly, false could mean you are trying to set a value for PHYSICAL CLARA   */
	bool setUseNPointScaling(const std::string& name);
	/*! Set do not use Npoint scaling.
	@param[in] std::string, name
	@param[out] bool, true if value set correctly, false could mean you are trying to set a value for PHYSICAL CLARA   */
	bool setDoNotUseNPointScaling(const std::string& name);
	/*! Toggle Npoint scaling on / off. 
	@param[in] std::string, name
	@param[out] bool, true if value set correctly, false could mean you are trying to set a value for PHYSICAL CLARA   */
	bool toggleUseNPointScaling(const std::string& name);
	/*! get the use Npoint scaling state
	@param[in] std::string, name
	@param[out] STATE, */
	STATE getNPointScalingState(const std::string& name)const;
	/*! check if analysis is using NPoint scaling
	@param[in] std::string, name
	@param[out] bool, true if using NPoint scaling*/
	bool isUsingNPointScaling(const std::string& name)const;
	/*! check if analysis is Not using NPoint scaling
	@param[in] std::string, name
	@param[out] bool, true if using NPoint scaling*/
	bool isNotUsingNPointScaling(const std::string& name)const;
	/*! Set the Npoint scaling step size.  
	@param[in] std::string, name
	@param[in] long, val to set
	@param[out] bool, true if value got sent to epics (not if it was received)*/
	bool setNpointScalingStepSize(const std::string& name, long val);
	/*! Set the Npoint scaling step size.
	@param[in] std::string, name
	@param[out] long, value */
	long getNpointScalingStepSize(const std::string& name)const;
	//                         __     __      __        __        __   __   __             __     
	// /\  |\ | |     /\  \ / /__` | /__`    |__)  /\  /  ` |__/ / _` |__) /  \ |  | |\ | |  \    
	///~~\ | \| |___ /~~\  |  .__/ | .__/    |__) /~~\ \__, |  \ \__> |  \ \__/ \__/ | \| |__/    
	//                                                                                            
	/*! Get the state of the use background image flag.
	@param[in] std::string, name of camera
	@param[out] STATE, value fo flag, YES or NO */
	STATE getUsingBackgroundImageState(const std::string& name);
	/*! set the netx image to be the background image subtracted during the analysis procedure.
	@param[in] std::string, name of camera
	@param[out] bool, true if value sent to EPICSt, not if it was succesfully applied */
	bool setNewBackgroundImage(const std::string& name);
	/*! Set using the stored background image will be subtracted from the image data before the analysis calculation.. 
	@param[in] std::string, name of camera
	@param[out] bool, true if value sent to EPICSt, not if it was succesfully applied */
	bool setUseBackgroundImage(const std::string& name);
	/*! Set use the background image during the analysis  procedure
	@param[in] std::string, name
	@param[in] bool, true to use the background, False to not use the background
	@param[out] bool, true if value set correctly, false could mean you are trying to set a value for PHYSICAL CLARA   */
	bool setDoNotUseBackgroundImage(const std::string& name);
	/*! Toggle using the background image during the analysis procedure.
	@param[in] std::string, name 
	@param[out] bool, true if value set correctly, false could mean you are trying to set a value for PHYSICAL CLARA   */
	bool toggleUseBackgroundImage(const std::string& name);
	/*! check if analysis is subtracting a background image.
	@param[in] std::string, name
	@param[out] bool, true if using background image during analysis */
	bool isUsingBackgroundImage(const std::string& name)const;
	/*! check if analysis is NOT subtracting a background data
	@param[in] std::string, name
	@param[out] bool, true if NOT  using background image during analysis*/
	bool isNotUsingBackgroundImage(const std::string& name)const;

                                                                                          
//	                         __     __                 __       
//	 /\  |\ |  /\  |    \ / /__` | /__`     |\/|  /\  /__` |__/ 
//	/~~\ | \| /~~\ |___  |  .__/ | .__/     |  | /~~\ .__/ |  \ 
//	                                                            
//	 __   __    
//	|__) /  \ | 
//	|  \ \__/ | 
//	 

//	 __                            __      __             __                ___       ___      
//	/ _`  /\  | |\ |     /\  |\ | |  \    |__) |     /\  /  ` |__/    |    |__  \  / |__  |    
//	\__> /~~\ | | \|    /~~\ | \| |__/    |__) |___ /~~\ \__, |  \    |___ |___  \/  |___ |___ 
//	 

//	 __   __                  __                 __           __   __         __   __  
//	|__) /  \ |     /\  |\ | |  \     |\/|  /\  /__` |__/    /  ` /  \  |\/| |__) /  \ 
//	|  \ \__/ |    /~~\ | \| |__/     |  | /~~\ .__/ |  \    \__, \__/  |  | |__) \__/ 
//	

//	       ___   __   
//	|     |__   |  \  
//	|___ .|___ .|__/ .
//

	//----------------------------------------------------------------------------------------------------------------
	//			 __             __   __        __  ___    
	//			/__` |\ |  /\  |__) /__` |__| /  \  |     
	//			.__/ | \| /~~\ |    .__/ |  | \__/  |     
	//

//	 __         ___          __   __            ___  __  
//	|__) | \_/ |__  |       /  ` /  \ |  | |\ |  |  /__` 
//	|    | / \ |___ |___    \__, \__/ \__/ | \|  |  .__/ 
//	        

/*
				  __   ___                             __     __
	|  |\/|  /\  / _` |__      /\  |\ |  /\  |    \ / /__` | /__`
	|  |  | /~~\ \__> |___    /~~\ | \| /~~\ |___  |  .__/ | .__/

*/


	/*! get the latest pixel sum for the image
	@param[in] std::string, name	
	@param[out] double, value */
	double getSumIntensity(const std::string& name)const;
	/*! get the latest pixel average for the image
	@param[in] std::string, name
	@param[out] double, value */
	double getAvgIntensity(const std::string& name)const;
	/*! Set the latest pixel sum for the image (Only available when NOT in PHYSICAL mode)
	@param[in] std::string, name
	@param[in] double, value
	@param[out] bool, value */
	bool setSumIntensity(const std::string& name, double value);
	/*! Set the latest pixel average for the image (Only available when NOT in PHYSICAL mode)
	@param[in] std::string, name
	@param[in] double, value
	@param[out] bool, value */
	bool setAvgIntensity(const std::string& name, double value);
	/*! Get the last directory / filename that HDF5 data was saved to.
	@param[in] std::string, name
	@param[out] string, value */
	std::string getLastDirectoryandFileName(const std::string& name) const;
	/*! Get the last directory that HDF5 data was saved to.
	@param[in] std::string, name
	@param[out] string, value */
	std::string getLastDirectory(const std::string& name)const;
	/*! Get the last filename that HDF5 data was saved to.
	@param[in] std::string, name
	@param[out] string, value */
	std::string getLastFileName(const std::string& name)const;

	/*! Set the mask x center position, setting the Mask and ROI together is preferred using setMaskAndROIxPos
	@param[in] std::string, name
	@param[in] long, value
	@param[out] bool, if command got sent to EPICS (not if it was accepted)	*/
	bool setMaskXCenter(const std::string& name, long val);
	/*! Set the mask y center position, setting the Mask and ROI together is preferred using setMaskAndROIyPos
	@param[in] std::string, name
	@param[in] long, value
	@param[out] bool, if command got sent to EPICS (not if it was accepted)	*/
	bool setMaskYCenter(const std::string& name, long val);
	/*! Set the mask x radius, setting the Mask and ROI together is preferred using setMaskAndROIxSize
	@param[in] std::string, name
	@param[in] long, value
	@param[out] bool, if command got sent to EPICS (not if it was accepted)	*/
	bool setMaskXRadius(const std::string& name, long val);
	/*! Set the mask y radius, setting the Mask and ROI together is preferred using setMaskAndROIySize
	@param[in] std::string, name
	@param[in] long, value
	@param[out] bool, if command got sent to EPICS (not if it was accepted)	*/
	bool setMaskYRadius(const std::string& name, long val);
	/*! Set the mask, setting the Mask and ROI together is preferred using setMaskandROI
	@param[in] std::string, name
	@param[in] long, x_pos value
	@param[in] long, y_pos value
	@param[in] long, x_size value
	@param[in] long, y_size value
	@param[out] bool, if command got sent to EPICS (not if it was accepted)	*/
	bool setMask(const std::string& name, long x_pos, long  y_pos, long x_size, long y_size);
	/*! Set the mask using map. Setting the Mask and ROI together is preferred using setMaskandROI
	@param[in] std::string, name
	@param[in] map<string, long>, values to set (keywords must be 'mask_keywords')
	@param[out] long, value */
	bool setMask(const std::string& name, std::map<std::string, long> settings);
	/*! Set the mask using python dict. Setting the Mask and ROI together is preferred using setMaskandROI_Py
	@param[in] std::string, name
	@param[in] dict, values to set (keywords must be 'mask_keywords')
	@param[out] long, value */
	bool setMask_Py(const std::string& name, boost::python::dict settings);
	/*! Get the mask x center position.
	@param[in] std::string, name
	@param[in] long, value */
	long getMaskXCenter(const std::string& name)const;
	/*! Get the mask y center position.
	@param[in] std::string, name
	@param[out] long, value */
	long getMaskYCenter(const std::string& name)const;
	/*! Get the mask x radius.
	@param[in] std::string, name
	@param[in] long, value */
	long getMaskXRadius(const std::string& name)const;
	/*! Get the mask y radius.
	@param[in] std::string, name
	@param[in] long, value */
	long getMaskYRadius(const std::string& name)const;
	/*! Get the mask settings.
	@param[in] std::string, name
	@param[in] map<string, long>, value */
	std::map<std::string, long> getMask(const std::string& name)const;
	/*! Get the mask settings (python version).
	@param[in] map<string, long>, value */
	boost::python::dict getMask_Py(const std::string& name)const;
	/*! Set the mask and ROI x position,
	@param[in] std::string, name
	@param[in] long, value (lower left hand pixel of ROI)
	@param[out] bool, if command got sent to EPICS (not if it was accepted)	*/
	bool setMaskAndROIxMax(const std::string& name, long val);
	/*! Set the mask and ROI y position,
	@param[in] std::string, name
	@param[in] long, value (left-most pixel of ROI)
	@param[out] bool, if command got sent to EPICS (not if it was accepted)	*/
	bool setMaskAndROIyMax(const std::string& name, long val);
	/*! Set the mask and ROI x size,
	@param[in] std::string, name
	@param[in] long, value (width of ROI)
	@param[out] bool, if command got sent to EPICS (not if it was accepted)	*/
	bool setMaskAndROIxSize(const std::string& name, long val);
	/*! Set the mask and ROI y size,
	@param[in] std::string, name
	@param[in] long, value (height of ROI)
	@param[out] bool, if command got sent to EPICS (not if it was accepted)	*/
	bool setMaskAndROIySize(const std::string& name, long val);
	/*! Set the mask and ROI x position,
	@param[in] std::string, name
	@param[out] long, value	*/
	long getMaskAndROIxMax(const std::string& name)const;
	/*! Set the mask and ROI y position,
	@param[in] std::string, name
	@param[out] long, value	*/
	long getMaskAndROIyMax(const std::string& name)const;
	/*! Set the mask and ROI x size,
	@param[in] std::string, name
	@param[out] long, value	*/
	long getMaskAndROIxSize(const std::string& name)const;
	/*! Set the mask and ROI y size,
	@param[in] std::string, name
	@param[out] long, value	*/
	long getMaskAndROIySize(const std::string& name)const;
	/*! Set the mask and ROI
	@param[in] std::string, name
	@param[in] long, x_pos value
	@param[in] long, y_pos value
	@param[in] long, x_size value
	@param[in] long, y_size value
	@param[out] bool, if command got sent to EPICS (not if it was accepted)	*/
	bool setMaskandROI(const std::string& name, long x_pos, long  y_pos, long x_size, long y_size);
	/*! Set the mask and ROI using a map
	@param[in] std::string, name
	@param[in] map<string, long>, values to set (keywords must be 'mask_and_roi_keywords')
	@param[out] long, value */
	bool setMaskandROI(const std::string& name, std::map<std::string, long> settings);
	/*! Set the mask using python dict. Setting the Mask and ROI together is preferred using setMaskAndROIxPos
	@param[in] std::string, name
	@param[in] dict, values to set (keywords must be 'mask_and_roi_keywords')
	@param[out] long, value */
	bool setMaskandROI_Py(const std::string& name, boost::python::dict settings);
	/*! Get the mask and ROI settings
	@param[in] std::string, name
	@param[out] map<string, long>, values ) */
	std::map<std::string, long> getMaskandROI(const std::string& name)const;
	/*! Get the mask and ROI settings (Python version).
	@param[in] std::string, name
	@param[out] dict, values ) */
	boost::python::dict getMaskandROI_Py(const std::string& name)const;

	/*! Get IOC active camera limit. The maximum number of cameras that can be active for this IOC. Running at this limit does not imply that everything will run smoothly!)
	@param[in] std::string, name
	@param[out] double, values ) */
	double getActiveCameraLimit(const std::string& name) const;
	/*! Get IOC active camera count. The number of cameras that are currently acquiring in this IOC.
	@param[in] std::string, name
	@param[out] double, values ) */
	double getActiveCameraCount(const std::string& name) const;
	/*! Test if the active camera limit is greater than than the active camera count
	@param[in] std::string, name
	@param[out] bool, values ) */
	bool canStartCamera(const std::string& name)const;


	/*! Stop All cameras acquiring.
	@param[out] bool, if command got sent to EPICS (not if it was accepted)	*/
	bool stopAllAcquiring();

	/*! Stop All cameras acquiring, and wait for verification by the control system (or timeout after 10000 ms).
	@param[out] bool, true if all cameras have stopped acquiring before timeout */
	bool stopAllAcquiringAndWait();

	/*! Stop All VELA cameras acquiring, and wait for verification by the control system (or timeout after 10000 ms).
	@param[out] bool, true if all VELA cameras have stopped acquiring before timeout */
	bool stopAllVELACamsAndWait();
	/*! Stop All CLARA cameras acquiring, and wait for verification by the control system (or timeout after 10000 ms).
	@param[out] bool, if all CLARA cameras have stopped acquiring before timeout */
	bool stopAllCLARACamsAndWait();
	/*! Stop cameras acquiring, and wait for verification by the control system (or timeout) (Python Version).
* 	@param[in] list, names of cmaeras to stop
	@param[out] bool, true if all cameras successfull y stopped */
	bool stopAcquiringAndWait_Py(const boost::python::list& cam_names);

	/*! Stop cameras acquiring, and wait for verification by the control system (or timeout after 10000 ms).
	* 	@param[in] vector<string>, names of cmaeras to stop
	@param[out] bool, true if all cameras successfull y stopped */
	bool stopAcquiringAndWait(const std::vector<std::string>& cam_names);



	/*! Start image acquiring.
	@param[in] std::string, name
	@param[out] bool, if command got sent to EPICS (not if it was accepted)	*/
	bool startAcquiring(const std::string& name);
	/*! Stop image acquiring.
	@param[in] std::string, name
	@param[out] bool, if command got sent to EPICS (not if it was accepted)	*/
	bool stopAcquiring(const std::string& name);
	/*! Stop image acquiring, and wait for the stop acquiring to be verified by the control system (or timeout).
	@param[in] std::string, name
	@param[in] size_t, wait time, in ms, before a timeout	
	@param[out] bool, true if camera stopped before timeout ms, otherwsie false*/
	bool stopAcquiringAndWait(const std::string& name, size_t timeout);
	/*! Is camera acquire state == ACQUIRING.
	@param[in] std::string, name
	@param[out] bool, result*/
	bool isAcquiring(const std::string& name)const;
	/*! Is camera acquire state == NOT_ACQUIRING.
	@param[in] std::string, name
	@param[out] bool, result*/
	bool isNotAcquiring(const std::string& name) const;
	/*! Get image acquire state .
	@param[in] std::string, name
	@param[out] STATE, value from acquire_state*/
	STATE getAcquireState(const std::string& name)const;
	/*! Start image analysis.
	@param[in] std::string, name
	@param[out] bool, if command got sent to EPICS (not if it was accepted)	*/
	bool startAnalysing(const std::string& name);
	/*! Stop image analysis.
	@param[in] std::string, name
	@param[out] bool, if command got sent to EPICS (not if it was accepted)	*/
	bool stopAnalysing(const std::string& name);
	/*! Is camera analysis state == ANALYSING.
	@param[out] bool, result*/
	bool isAnalysing(const std::string& name)const;
	/*! Is camera analysis state == NOT_ANALYSING.
	@param[in] std::string, name
	@param[out] bool, result*/
	bool isNotAnalysing(const std::string& name) const;
	/*! Get image analysis state .
	@param[in] std::string, name
	@param[out] STATE, value from analysis_state*/
	STATE getAnalysisState(const std::string& name)const;
	


	/*! Does the camera / screen combination have an LED.
	@param[in] std::string, name
	@param[out] bool, value of has_led	*/
	bool hasLED(const std::string& name)const;
	/*! Turn the LED on for this camera / screen.
	@param[in] std::string, name
	@param[out] bool, if command got sent to EPICS (not if it was accepted)	*/
	bool setLEDOn(const std::string& name);
	/*! Turn the LED off for this camera / screen.
	@param[in] std::string, name
	@param[out] bool, if command got sent to EPICS (not if it was accepted)	*/
	bool setLEDOff(const std::string& name);
	/*! Is LED on, led_state == ON.
	@param[in] std::string, name
	@param[out] bool*/
	bool isLEDOn(const std::string& name)const;
	/*! Is LED off, led_state == OFF.
	@param[in] std::string, name
	@param[out] bool*/
	bool isLEDOff(const std::string& name)const;
	/*! Get LED state.
	@param[in] std::string, name
	@param[out] STATE, value from led_state*/
	STATE getLEDState(const std::string& name)const;
	/*! Get the latest image data (decimated array for passing accross network, not full image).
	@param[in] std::string, name
	@param[out] bool, did commands get sent to EPICS (should also mean data was updated)*/
	bool updateImageData(const std::string& name);
	/*! Get the latest image data AND time stamp.
	It is possible in the current implementation that the time stamp is not synchronized with the data.
	(decimated array for passing accross network, not full image).
	@param[in] std::string, name
	@param[out] bool, did commands get sent to EPICS (should also mean data was updated)*/
	bool updateImageDataWithTimeStamp(const std::string& name);
	/*! Get the latest ROI data (decimated array for passing accross network, not full image, !!ROI is new, not been tested!!).
	@param[in] std::string, name
	@param[out] bool, did commands get sent to EPICS (should also mean data was updated)*/
	bool updateROIData(const std::string& name);
	/*! Get the latest ROI data AND time stamp.
	It is possible in the current implementation that the time stamp is not synchronized with the data.
	(decimated array for passing accross network, not full image).
	@param[in] std::string, name
	@param[out] bool, did commands get sent to EPICS (should also mean data was updated)*/
	bool updateROIDataWithTimeStamp(const std::string& name);
	/*! Get a copy of the current image data. Until an updateImage function is called this will be empty,
	to reduce network load Camera data ARE NOT continuously monitored.
	@param[in] std::string, name
	@param[out] vector<long>, latest data */
	std::vector<long> getImageData(const std::string& name)const;
	/*! Get a copy of the current image data (Python Version). Until an updateImage functiton is called this will be empty,
	to reduce network load Camera data arrays ARE NOT continuously monitored.
	@param[in] std::string, name
	@param[out] list, latest data */
	boost::python::list getImageData_Py(const std::string& name)const;
	/*! Get a copy of the current image data. Until an updateROI function is called this will be empty,
	to reduce network load Camera data arrays ARE NOT continuously monitored.
	@param[in] std::string, name
	@param[out] vector<long>, latest data */
	std::vector<long> getROIData(const std::string& name)const;
	/*! Get a copy of the current image data (python version). Until an updateROI function is called this will be empty,
	to reduce network load Camera data arrays ARE NOT continuously monitored.
	@param[in] std::string, name
	@param[out] list, latest data */
	boost::python::list getROIData_Py(const std::string& name)const;
	/*! Get a reference to the current image data. Gives access to image data without copying
	@param[in] std::string, name
	@param[out] vector<long>&, reference to latest data, When exposed to python this function returns a std_vector_long  */
	std::vector<long>& getImageDataConstRef(const std::string& name);
	/*! Get a reference to the current ROI data. Gives access to data without copying
	@param[in] std::string, name
	@param[out] vector<long>&, reference to latest data, When exposed to python this function returns a std_vector_long  */
	std::vector<long>& getROIDataConstRef(const std::string& name);
	/*! Get the size of the running stats buffer, running_stats_buffer_size
	@param[in] std::string, name
	@param[out] size_t, value */
	size_t getBufferSize(const std::string& name)const;
	/*! Set the size of the running stats buffer, running_stats_buffer_size
	@param[in] std::string, name
	@param[in] size_t, value */
	void setBufferSize(const std::string& name, size_t v);
	/*! Clear all runing stats buffers */
	void clearBuffers(const std::string& name);
	/*! sets all RS size (m_max) for a given camera*/
	void setRunningStatSize(const std::string& name, size_t size);
	/*! clears the RS object for a given camera */
	void clearRunningStats(const std::string& name);




	bool areAllRunningStatsFull(const std::string& name)const;


	size_t getRunningStatNumDataValues(const std::string& name)const;

	/*! Enable the Analysis Mask Overlay in the Camera Image data, (NB I think this is enabled JUST for camera data sent over the network!?)
	@param[in] std::string, name
	@param[out] bool, was command sent to EPICS, (not if it worked) */
	bool enableOverlayMask(const std::string& name);
	/*! Enable the Cross Hair Overlay in the Camera Image data, (NB I think this is enabled JUST for camera data sent over the network!?)
	@param[in] std::string, name
	@param[out] bool, was command sent to EPICS, (not if it worked) */
	bool enableOverlayCross(const std::string& name);
	/*! Enable the Center Of Mask Overlay in the Camera Image data, (NB I think this is enabled JUST for camera data sent over the network!?)
	@param[in] std::string, name
	@param[out] bool, was command sent to EPICS, (not if it worked) */
	bool enableOverlayResult(const std::string& name);
	/*! Disable the Analysis Mask Overlay in the Camera Image data, (NB I think this is enabled JUST for camera data sent over the network!?)
	* 	@param[in] std::string, name
	@param[out] bool, was command sent to EPICS, (not if it worked) */
	bool disableOverlayMask(const std::string& name);
	/*! Disable the Cross Hair Overlay in the Camera Image data,
	* 	@param[in] std::string, name
	@param[out] bool, was command sent to EPICS, (not if it worked) */
	bool disableOverlayCross(const std::string& name);
	/*! Disable the Centre of Mass Overlay in the Camera Image data,
	* 	@param[in] std::string, name
	@param[out] bool, was command sent to EPICS, (not if it worked) */
	bool disableOverlayResult(const std::string& name);
	/*! Disable all overlays for the named camera,
	* 	@param[in] std::string, name
	@param[out] bool, was command sent to EPICS, (not if it worked) */
	bool disableAllOverlay(const std::string& name);
	/*! Disable all overlays, for all cameras in this factory, 
	@param[out] bool, was command sent to EPICS, (not if it worked) */
	bool disableAllOverlayForAllCameras();
	/*! Get the state of the Analysis Mask Overlay 
	* 	@param[in] std::string, name
	@param[out] STATE, one of ON, OFF, UNKNONWN */
	STATE getOverlayMaskState(const std::string& name)const;
	/*! Get the state of the Cross Hair Overlay 
	* 	@param[in] std::string, name
	@param[out] bool, was command sent to EPICS, (not if it worked) */
	STATE getOverlayCrossState(const std::string& name)const;
	/*! Get the state of the Centre of Mass Overlay 
	* 	@param[in] std::string, name
	@param[out] bool, was command sent to EPICS, (not if it worked) */
	STATE getOverlayResultState(const std::string& name)const;
	/*! get the Overlay state for each camera object and overlay
	@param[out] bool, was command sent to EPICS, (not if it worked) */
	std::map<std::string, STATE> getAllOverlayStates()const;
	/*! get the Overlay state for each camera object and overlay, Python version 
	@param[out] bool, was command sent to EPICS, (not if it worked) */
	boost::python::dict getAllOverlayStates_Py()const;

	/*! Get the running stats buffer,
	@param[in] std::string, name
	@param[out] dict, values */
	boost::python::dict getAllRunningStats(const std::string& name)const;

//
// TODO implment snapshot functions 
//----------------------------------------------------------------------------------------------------------------
//			 __             __   __        __  ___    
//			/__` |\ |  /\  |__) /__` |__| /  \  |     
//			.__/ | \| /~~\ |    .__/ |  | \__/  |     
//			                                          
	/*! Save the current factory settings to the default filepath and filename
		@param[in] string, comments	(optional input, default as empty)
		@param[out] STATE, success, failure, etc.	*/
	STATE saveSnapshot(const std::string& comments = "");
	/*! Save the current factory settings to filepath and filename
	@param[in] string, filepath
	@param[in] string, filename
	@param[in] string, comments	(optional input, default as empty)
	@param[out] STATE, success, failure, etc.			*/
	STATE saveSnapshot(const std::string& filepath, const std::string& filename, const std::string& comments = "");
	/*! Save snap_dict to the default filepath and filename
	@param[in] dict, snap_dict
	@param[in] string, comments	(optional input, default as empty)
	@param[out] STATE, success, failure, etc.			*/
	STATE saveSnapshot_Pydict(const boost::python::dict& snap_dict, const std::string& comments = "");
	/*! Save snap_dict to filepath and filename
	@param[in] string, filepath
	@param[in] string, filename
	@param[in] dict, snap_dict
	@param[out] STATE, success, failure, etc.			*/
	STATE saveSnapshot_Pyfile(const std::string& filepath, const std::string& filename, const boost::python::dict& snapshot_dict, const std::string& comments = "");
	/*! Load the snapshot at filename, filepath and copy the data into the member variable hardwareSnapshotMap. NB this function does not apply the settings.
	@param[in] string, filepath
	@param[in] string, filename
	@param[out] STATE, success, failure, etc.			*/
	STATE loadSnapshot(const std::string filepath, const std::string& filename); // read into hardwareSnapshotMap
	/*! Load snapshot_dict by copying the data into the member variable hardwareSnapshotMap. NB this function does not apply the settings.
	@param[in] dict, snapshot_dict
	@param[out] STATE, success, failure, etc.			*/
	STATE loadSnapshot_Py(const boost::python::dict& snapshot_dict); // put d into hardwareSnapshotMap
	/*! Get the latest snapshot data for this factory.
	@param[out] map<string, HardwareSnapshot>, Map of HardwareSnapshot data for each object, keyed by the object name */
	std::map<std::string, HardwareSnapshot> getSnapshot(); // c++ version 
	/*! Get the latest snapshot data for this factory. Python Version
	@param[out] dict, dict of HardwareSnapshot data for each object, keyed by the object name */
	boost::python::dict getSnapshot_Py(); // return current state as py dict 
	/*! Get the snapshot data from filepath and filename. Python Version
	@param[in] string, filepath
	@param[in] string, filename
	@param[out] dict, dict of HardwareSnapshot data for each object, keyed by the object name */
	boost::python::dict getSnapshotFromFile_Py(const std::string& filepath, const std::string& filename); // return file contents as py dict 
	/*! Apply a Python dict snapshot.
	@param[out] dict, dict of HardwareSnapshot data for each object, keyed by the object name
	@param[in] TYPE, apply only to magnets that match this type (if left empty, defaults to all magnet typpes)
	@param[out] STATE, success, failure, etc.			*/
	STATE applySnaphot(const boost::python::dict& snapshot_dict, TYPE type = TYPE::CAMERA_TYPE);
	/*! Apply a snapshot data from filepath and filename.
	@param[in] string, filepath
	@param[in] string, filename
	@param[in] TYPE, apply only to magnets that match this type (if left empty, defaults to all magnet typpes)
	@param[out] STATE, success, failure, etc.			*/
	STATE applySnaphot(const std::string& filepath, const std::string& filename, TYPE type = TYPE::CAMERA_TYPE);
	/*! Apply the member variable hardwareSnapshotMap (which could have already been loaded with loadSnapshot()).
	@param[in] TYPE, apply only to magnets that match this type (if left empty, defaults to all magnet typpes)
	@param[out] STATE, success, failure, etc.			*/
	STATE applyLoadedSnaphost(TYPE type = TYPE::CAMERA_TYPE);

	STATE checkLastAppliedSnapshot(TYPE type_to_check);
	/*! Get the last filename that was used to save a snapshot
	@param[out] string, filename */
	std::string getLastSnapshotFilename()const;
	/*! Get the last filename that was used to save a snapshot
	@param[out] string, directory */
	std::string getLastSnapshotDirectory()const;
private:
	/*! Snaphsot data is placed here, when loaded from file, or applied  */
	std::map<std::string, HardwareSnapshot> hardwareSnapshotMap;
	/*! Each factory must know how to convert a YAML NODE read from snaphsot YAML file into a map of hardwareSnapshots.
		It must be done in the factory so we know how to convert the type for each record
		This function should also check the YAML::NODE is compliant with our definitions.
		@param[in] YAML::Node, input_node to convert
		@param[out] map<string, HardwareSnapshot>, return map */
	std::map<std::string, HardwareSnapshot> yamlNodeToHardwareSnapshotMap(const YAML::Node& input_node);
	/*! Each factory must know how to convert a ython Dictionary into a map of hardwareSnapshots.
		It must be done in the factory so we know how to convert the type for each record
		@param[in] dict, input_dict to convert
		@param[out] map<string, HardwareSnapshot>, return map */
	std::map<std::string, HardwareSnapshot> pyDictToHardwareSnapshotMap(const boost::python::dict& input_dict);

	YAML::Node hardwareSnapshotMapToYAMLNode(const std::map<std::string, HardwareSnapshot>& hardwaresnapshot_map);
	/*! This function actually tries applying a Map of harwwdare snapshots, it will only apply data that are well formatted and typed
		@param[in] map, Map of camera HardwareSnapshot objects, keyed by the camera name, to apply
		@param[out] STATE, success, failure, etc */
	STATE applyhardwareSnapshotMap(const std::map<std::string, HardwareSnapshot>& hardwaresnapshot_map, TYPE type = TYPE::CAMERA_TYPE);

private:
	std::string last_snapshot_save_filename;
	std::string last_snapshot_save_directory;

public:

	std::string getFullName(const std::string& name_to_check) const;

	/*! get the name alises for this 
	@param[out] names, vector containing  all the alias names */
	std::vector<std::string> getNameAliases(const std::string& cam_name) const;
	/*! get the name alises for this LLRF (python version)
		@param[out] names, python list containing all the alias names */
	boost::python::list getNameAliases_Py(const std::string& cam_name) const;
	

	/*! Enable debug messages*/
	void debugMessagesOn();
	/*! Disbale debug messages*/
	void debugMessagesOff();
	/*! Enable messages*/
	void messagesOn();
	/*! Disable messages*/
	void messagesOff();
	bool isDebugOn();
	bool isMessagingOn();

	std::vector<long> array_data_dummy;

	/*! Flag to say whether the LLRFFactory setup function has been completed successfully.*/
	bool hasBeenSetup; // PUBLIC as it is used by hardwarefactory, could make it a friend
private:
	/*! The kind of CameraFactory that is created (PHYSICAL, VIRTUAL, OFFLINE), this variable is passed to the
	hardware objects when they are created by ConfigReader*/
	STATE mode;
	/*! ConfigReader to parse YAML config files and create associated LLRF objects*/
	ConfigReader reader;
	/*! Fill camera_map with camera objects */
	void populateCameraMap();
	/*! Cut cameras from camera_map that are not in the requested machine area */
	void cutLHarwdareMapByMachineAreas();
	/*! Cut cameras from camera_map that do nto have a name in names 
		@param[in] names, vector containing  all the names of cmaeras to keep 	*/
	void cutLHarwdareMapByNames(const std::vector<std::string>& names);
	/*! set up connections to epics */
	void setupChannels();
	/*! sets the pvStruct monitor flag to true if the record is hardcoded as a record to monitor 
	@param[in] pvStruct, pvStruct to set monitor status for */
	void setMonitorStatus(pvStruct& pvStruct);
	
	/*! All camera objects are held in here */
	std::map<std::string, Camera> camera_map;
	
	/*! After setup has finished connecting channels, some values are set to their Master Lattice values, pixel to mm, centre_x and y row/column (maybe some othhers). */
	void caputMasterLatticeParametersAfterSetup();

	std::vector<TYPE> machineAreas;

	void updateAliasNameMap(const Camera& camera);
	std::map<std::string, std::string> alias_name_map;

	// used when we need to return values from a requested camera name that does not exist 
	Camera dummy_cam;

	//const std::vector<std::string> cam_monitor_records;

};

#endif // CAMERA_FACTORY_H_




//double pix2mmX(const std::string& name, double value)const;
//double pix2mmY(const std::string& name, double value)const;
//double mm2pixX(const std::string& name, double value)const;
//double mm2pixY(const std::string& name, double value)const;
//double getpix2mmX(const std::string& name)const;
//double getpix2mmY(const std::string& name)const;
//double setpix2mmX(const std::string& name, double value);
//double setpix2mmY(const std::string& name, double value);
//double getX(const std::string& name)const;
//double getY(const std::string& name)const;
//double getSigX(const std::string& name)const;
//double getSigY(const std::string& name)const;
//double getSigXY(const std::string& name)const;
//double getXPix(const std::string& name)const;
//double getYPix(const std::string& name)const;
//double getSigXPix(const std::string& name)const;
//double getSigYPix(const std::string& name)const;
//double getSigXYPix(const std::string& name)const;
///*! get the Npoint scaling stepsize
//@param[in] const std::string, name of camera to set value for
//@param[out] long, value */
//long getStepSize(const std::string& name)const;
///*! set the Npoint scaling stepsize
//@param[in] const std::string, name of camera to set value for
//@param[in] long, new stepsize
//@param[out] bool, value */
//bool setStepSize(const std::string& name, long val);
//boost::python::dict getRunningStats(const std::string& name)const;
//boost::python::dict getAllRunningStats()const;
//bool setX(const std::string& name, double value);
//bool setY(const std::string& name, double value);
//bool setSigX(const std::string& name, double value);
//bool setSigY(const std::string& name, double value);
//bool setSigXY(const std::string& name, double value);
////bool setXPix(const std::string& name, double value);
////bool setYPix(const std::string& name, double value);
////bool setSigXPix(const std::string& name, double value);
////bool setSigYPix(const std::string& name, double value);
////bool setSigXYPix(const std::string& name, double value);
//double getSumIntensity(const std::string& name)const;
//double getAvgIntensity(const std::string& name)const;
//bool setSumIntensity(const std::string& name, double value);
//bool setAvgIntensity(const std::string& name, double value);
////unsigned short getMaskXCenter(const std::string& name)const;
////unsigned short getMaskYCenter(const std::string& name)const;
////unsigned short getMaskXRadius(const std::string& name)const;
////unsigned short getMaskYRadius(const std::string& name)const;
////unsigned short setMaskXCenter(const std::string& name, unsigned short val);
////unsigned short setMaskYCenter(const std::string& name, unsigned short val);
////unsigned short setMaskXRadius(const std::string& name, unsigned short val);
////unsigned short setMaskYRadius(const std::string& name, unsigned short val);
//long getMaskXCenter(const std::string& name)const;
//long getMaskYCenter(const std::string& name)const;
//long getMaskXRadius(const std::string& name)const;
//long getMaskYRadius(const std::string& name)const;
//
//long setMaskXCenter(const std::string& name, long val);
//long setMaskYCenter(const std::string& name, long val);
//long setMaskXRadius(const std::string& name, long val);
//long setMaskYRadius(const std::string& name, long val);

//bool useNPoint(const std::string& name, bool v);
//bool useBackground(const std::string& name, bool v);
//STATE getNPointState(const std::string& name)const;

//bool isUsingNPoint(const std::string& name)const;
//bool isUsingBackground(const std::string& name)const;
//STATE getUsingBackgroundState(const std::string& name)const;


//bool stopAcquiring(const std::string& cam_name);
//bool stopAllAcquiring();
//bool stopAllAcquiringExceptVC();
//bool startAcquiring(const std::string& cam_name, bool stop_all = true);
//std::map<std::string, STATE> getAllAcquireStates()const;
//boost::python::dict getAllAcquireStates_Py()const;

//bool isAcquiring(const std::string& name)const;
//bool isNotAcquiring(const std::string& name) const;
//STATE getAcquireState(const std::string& name)const;

//bool startAnalysing(const std::string& name);
//bool stopAnalysing(const std::string& name);
//bool isAnalysing(const std::string& name)const;
//bool isNotAnalysing(const std::string& name) const;
//STATE getAnalysisState(const std::string& name)const;
//std::map<std::string, STATE> getAllAnalysisState()const;
//boost::python::dict getAllAnalysisState_Py()const;