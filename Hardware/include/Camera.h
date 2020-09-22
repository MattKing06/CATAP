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

	double pix2mmX(double value)const;
	double pix2mmY(double value)const;
	double mm2pixX(double value)const;
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


	double getMaskXCenter()const;
	double getMaskYCenter()const;
	double getMaskXRadius()const;
	double getMaskYRadius()const;

	bool setMaskXCenter(double val);
	bool setMaskYCenter(double val);
	bool setMaskXRadius(double val);
	bool setMaskYRadius(double val);
	
	double getPix2mm()double;
	bool setPix2mm(double val);


	double getSigXY()const;
	double getXPix()const;
	double getYPix()const;
	double getSigXPix()const;
	double getSigYPix()const;
	double getSigXYPix()const;

	/*! analaysis mask center x position (pixels). Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, int > mask_x_center;
	/*! analaysis mask center y position (pixels). Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, int > mask_y_center;
	/*! analaysis mask center x radius (pixels). Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, int > mask_x_radius;
	/*! analaysis mask center y radius (pixels). Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, int > mask_y_radius;
	/*! analaysis center x NOT SURE WHAT THIS IS YET. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, int > x_center;
	/*! analaysis center y NOT SURE WHAT THIS IS YET. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, int > y_center;
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

	/*! latest Sum Intensity (sum of pixel values). Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, double > sum_intensity;
	/*! latest Average Intensity (mean of pixel values). Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, double > avg_intensity;

	/*! analaysis mask center x position (pixels). Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, double > mask_x_center;
	/*! analaysis mask center y position (pixels). Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, double > mask_y_center;
	/*! analaysis mask center x radius (pixels). Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, double > mask_x_radius;
	/*! analaysis mask center y radius (pixels). Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, double > mask_y_radius;
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


private:

	TYPE cam_type;


	/*! alternative names for the magnet (usually shorter thna the full PV root),
	defined in the master lattice yaml file	*/
	std::vector<std::string> aliases;


	double pix2mmX_ratio;
	double pix2mmY_ratio;


	std::vector<std::string> screen_names;

	

	EPICSCameraInterface_sptr epicsInterface;
	std::map<std::string, std::string> CameraParamMap;



};


#endif //RF_HEARTBEAT_H_