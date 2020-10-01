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

	bool setX(double value);
	bool setY(double value);
	bool setSigX(double value);
	bool setSigY(double value);
	bool setSigXY(double value);
	//bool setXPix(double value);
	//bool setYPix(double value);
	//bool setSigXPix(double value);
	//bool setSigYPix(double value);
	//bool setSigXYPix(double value);

	double getSumIntensity()const;
	double getAvgIntensity()const;
	bool setSumIntensity(double value);
	bool setAvgIntensity(double value);

	void debugMessagesOn();
	void debugMessagesOff();
	void messagesOn();
	void messagesOff();


	unsigned short getMaskXCenter()const;
	unsigned short getMaskYCenter()const;
	unsigned short getMaskXRadius()const;
	unsigned short getMaskYRadius()const;

	unsigned short setMaskXCenter(unsigned short val);
	unsigned short setMaskYCenter(unsigned short val);
	unsigned short setMaskXRadius(unsigned short val);
	unsigned short setMaskYRadius(unsigned short val);
	
	
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
	std::pair<epicsTimeStamp, unsigned short > mask_x_center;
	/*! analaysis mask center y position (pixels). Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, unsigned short > mask_y_center;
	/*! analaysis mask center x radius (pixels). Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, unsigned short > mask_x_radius;
	/*! analaysis mask center y radius (pixels). Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, unsigned short > mask_y_radius;
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

	///*! Write status check (did the last write operation succesfully complete). Value and epicstimestamp.	*/
	//std::pair<epicsTimeStamp, STATE> write_error_message;

	/*! Latest directory to write image data to. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, std::string> save_filepath;

	/*! Latest filename to write image data to. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, std::string> save_filename;


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

};


#endif //RF_HEARTBEAT_H_