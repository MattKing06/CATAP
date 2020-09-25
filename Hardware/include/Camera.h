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

#ifndef _RUNNING_STATS_H_
#define _RUNNING_STATS_H_

#include <iostream>
#include "PythonTypeConversions.h"

class RunningStats
{
	// thanks to https://www.johndcook.com/blog/standard_deviation/

public:
	RunningStats() :
		m_n(0),
		max_n(0),
		rs_complete(false)
	{
		setBufferSize(10); // MAGIC
	}
	/*! Clear the running stats values */
	void Clear()
	{
		m_n = 0;
		rs_complete = false;
	}
	/*! Set maximum number of entires to be added to running stats
		@param[in] number of entries         */
	void setMaxCount(const size_t value)
	{
		max_n = value;
	}
	/*! Get the maximum number of entires
		@param[out] max number of entries         */
	size_t getMaxCount() const
	{
		return max_n;
	}
	/*! Add a new value to the runing stats , templated version
		@param[in] value to add, !!must be a umerica simple type!! */
	template<typename T>
	void Push(T x)
	{
		doPush((double)x);
	}

	/*! Add a new value to the runing stats , templated version
		@param[in] value to add, !!must be a umerica simple type!! */
	size_t NumDataValues() const
	{
		return m_n;
	}
	/*! Add a new value to the runing stats , templated version
		@param[in] value to add, !!must be a umerica simple type!! */
	double Mean() const
	{
		return (m_n > 0) ? m_newM : 0.0;
	}
	/*! Add a new value to the runing stats , templated version
		@param[in] value to add, !!must be a umerica simple type!! */
	double Variance() const
	{
		return ((m_n > 1) ? m_newS / (m_n - 1) : 0.0);
	}
	/*! Add a new value to the runing stats , templated version
		@param[in] value to add, !!must be a umerica simple type!! */
	double StandardDeviation() const
	{
		return sqrt(Variance());
	}
	/*! Add a new value to the runing stats , templated version
		@param[in] value to add, !!must be a umerica simple type!! */
	bool Full() const
	{
		return rs_complete;
	}
	/*! Add a new value to the runing stats , templated version
		@param[in] value to add, !!must be a umerica simple type!! */
	bool NotFull() const
	{
		return !Full();
	}
	/*! updat epassed referecnes with teh latest value
		@param[in] value to add, !!must be a umerica simple type!!
		@param[in] value to add, !!must be a umerica simple type!!
		@param[in] value to add, !!must be a umerica simple type!! */
	void getCurrentState(size_t& m_n_ret, double& m_oldM_ret, double& m_oldS_ret)
	{
		m_n_ret = m_n;
		m_oldM_ret = m_oldM;
		m_oldS_ret = m_oldS;
	}
	void setInitialValues(size_t m_n_init, double m_oldM_init, double m_oldS_init)
	{
		m_n = m_n_init;
		m_oldM = m_oldM_init;
		m_oldS = m_oldS_init;
	}


	std::vector<double> Buffer()const
	{
		return buffer;
	}
	boost::python::list Buffer_Py()const
	{
		return to_py_list<double>(buffer);
	}

	/*
	  get/set the current settings, so that you can prime the running-stat with
		some known values
	*/
	boost::python::dict getRunningStats()const
	{
		boost::python::dict r;
		r["NumDataValues"] = NumDataValues();
		r["Mean"] = Mean();
		r["Variance"] = Variance();
		r["StandardDeviation"] = StandardDeviation();
		r["Full"] = Full();
		r["Buffer"] = Buffer_Py();
		return r;
	}

	size_t setBufferSize(size_t s)
	{
		clearBuffer();
		buffer.resize(s);
		return getBuferSize();
	}

	size_t getBuferSize()const
	{
		return buffer.size();
	}

	void clearBuffer()
	{
		buffer.clear();
		buffer_n = 0;//MAGIC
	}

private:
	size_t m_n;
	size_t buffer_n;
	double m_oldM, m_newM, m_oldS, m_newS;
	size_t max_n;
	bool rs_complete;

	bool can_add()
	{
		if (max_n == 0)
		{
			return true;
		}
		if (rs_complete)
		{
			return false;
		}
		if (m_n == max_n)
		{
			rs_complete = true;
			return false;
		}
		return true;
	}

	std::vector<double> buffer;

	void bufferPush(double value)
	{
		if (buffer_n < getBuferSize())
		{
			buffer[buffer_n] = value;
			buffer_n += 1; //MAGIC
		}

	}


	/*! Add a new value to the runing stats, assuming passed value is double
	@param[in] value to add */
	void doPush(double x)
	{
		if (can_add())
		{
			m_n++;
			// See Knuth TAOCP vol 2, 3rd edition, page 232
			if (m_n == 1)
			{
				m_oldM = m_newM = x;
				m_oldS = 0.0;
			}
			else
			{
				m_newM = m_oldM + (x - m_oldM) / m_n;
				m_newS = m_oldS + (x - m_oldM) * (x - m_newM);
				// set up for next iteration
				m_oldM = m_newM;
				m_oldS = m_newS;
			}
		}
		bufferPush(x);
	}

};
#endif // _RUNNING_STATS_H_





class Camera;
/*
	Image collection and saving happens in a new thread,
	this struct is passed to the new thread function
*/
class ImageCollector
{
	ImageCollector() :
		//camInterface(nullptr),
		thread(nullptr),
		isBusy(false),
		success(false),
		numShots(0)
	{}
	/*
		https://stackoverflow.com/questions/4937180/a-base-class-pointer-can-point-to-a-derived-class-object-why-is-the-vice-versa
	*/
	//cameraBase* camInterface;
	Camera* cam;
	std::thread*   thread;
	size_t       numShots;
	bool           isBusy;
	bool           success;
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

	bool captureAndSave(size_t num_shots);
		
	STATE getCaptureState()const;
	bool isCapturing()const;
	


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


private:

	TYPE cam_type;


	/*! alternative names for the magnet (usually shorter thna the full PV root),
	defined in the master lattice yaml file	*/
	std::vector<std::string> aliases;
	std::vector<std::string> screen_names;


	double pix2mmX_ratio;
	double pix2mmY_ratio;


	size_t max_shots_number;


	// collect and save stuff 

	bool setNumberOfShotsToCapture(size_t num);
	bool collect();

	EPICSCameraInterface_sptr epicsInterface;
	std::map<std::string, std::string> CameraParamMap;



};


#endif //RF_HEARTBEAT_H_