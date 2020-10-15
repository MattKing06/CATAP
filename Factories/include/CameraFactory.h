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


	/*! get the names of all the cameras in Factory 
	@param[out] vector of strings, camera names*/
	std::vector<std::string> getCameraNames();
	/*! get the names of all the cameras in Factory, python version
	@param[out] python list of strings, camera names*/
	boost::python::list getCameraNames_Py();

	/*! get a reference to a camera object 
	@param[in] cam_name, name of camera object to return 
	@param[out] camera object*/
	Camera& getCamera(const std::string& cam_name);
	

	double pix2mmX(const std::string& name, double value)const;
	double pix2mmY(const std::string& name, double value)const;
	double mm2pixX(const std::string& name, double value)const;
	double mm2pixY(const std::string& name, double value)const;

	double getpix2mmX(const std::string& name)const;
	double getpix2mmY(const std::string& name)const;
	double setpix2mmX(const std::string& name, double value);
	double setpix2mmY(const std::string& name, double value);


	double getX(const std::string& name)const;
	double getY(const std::string& name)const;
	double getSigX(const std::string& name)const;
	double getSigY(const std::string& name)const;
	double getSigXY(const std::string& name)const;
	double getXPix(const std::string& name)const;
	double getYPix(const std::string& name)const;
	double getSigXPix(const std::string& name)const;
	double getSigYPix(const std::string& name)const;
	double getSigXYPix(const std::string& name)const;


	boost::python::dict getRunningStats(const std::string& name)const;
	boost::python::dict getAllRunningStats()const;

	bool setX(const std::string& name, double value);
	bool setY(const std::string& name, double value);
	bool setSigX(const std::string& name, double value);
	bool setSigY(const std::string& name, double value);
	bool setSigXY(const std::string& name, double value);
	//bool setXPix(const std::string& name, double value);
	//bool setYPix(const std::string& name, double value);
	//bool setSigXPix(const std::string& name, double value);
	//bool setSigYPix(const std::string& name, double value);
	//bool setSigXYPix(const std::string& name, double value);

	double getSumIntensity(const std::string& name)const;
	double getAvgIntensity(const std::string& name)const;
	bool setSumIntensity(const std::string& name, double value);
	bool setAvgIntensity(const std::string& name, double value);

	//unsigned short getMaskXCenter(const std::string& name)const;
	//unsigned short getMaskYCenter(const std::string& name)const;
	//unsigned short getMaskXRadius(const std::string& name)const;
	//unsigned short getMaskYRadius(const std::string& name)const;

	//unsigned short setMaskXCenter(const std::string& name, unsigned short val);
	//unsigned short setMaskYCenter(const std::string& name, unsigned short val);
	//unsigned short setMaskXRadius(const std::string& name, unsigned short val);
	//unsigned short setMaskYRadius(const std::string& name, unsigned short val);

	long getMaskXCenter(const std::string& name)const;
	long getMaskYCenter(const std::string& name)const;
	long getMaskXRadius(const std::string& name)const;
	long getMaskYRadius(const std::string& name)const;
	
	long setMaskXCenter(const std::string& name, long val);
	long setMaskYCenter(const std::string& name, long val);
	long setMaskXRadius(const std::string& name, long val);
	long setMaskYRadius(const std::string& name, long val);




	bool stopAcquiring(const std::string& cam_name);
	bool stopAllAcquiring();
	bool stopAllAcquiringExceptVC();
	bool startAcquiring(const std::string& cam_name, bool stop_all = true);
	std::map<std::string, STATE> getAllAcquireStates()const;
	boost::python::dict getAllAcquireStates_Py()const;

	bool isAcquiring(const std::string& name)const;
	bool isNotAcquiring(const std::string& name) const;
	STATE getAcquireState(const std::string& name)const;

	bool startAnalysing(const std::string& name);
	bool stopAnalysing(const std::string& name);
	bool isAnalysing(const std::string& name)const;
	bool isNotAnalysing(const std::string& name) const;
	STATE getAnalysisState(const std::string& name)const;
	std::map<std::string, STATE> getAllAnalysisState()const;
	boost::python::dict getAllAnalysisState_Py()const;



	std::string getFullName(const std::string& name_to_check) const;

	/*! get the name alises for this 
	@param[out] names, vector containing  all the alias names */
	std::vector<std::string> getNameAliases(const std::string cam_name) const;
	/*! get the name alises for this LLRF (python version)
		@param[out] names, python list containing all the alias names */
	boost::python::list getNameAliases_Py(const std::string cam_name) const;
	
	std::string getScreen(const std::string cam_name) const;
	std::vector<std::string> getScreenNames(const std::string cam_name) const;
	boost::python::list getScreenNames_Py(const std::string cam_name) const;

	void debugMessagesOn();
	void debugMessagesOff();
	void messagesOn();
	void messagesOff();
	bool isDebugOn();
	bool isMessagingOn();


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
	/*! set up connections to epics */
	void setupChannels();
	/* sets the pvStruct monitor flag to true if the record is hardcoded as a record to monitor */
	void setMonitorStatus(pvStruct& pvStruct);
		
	std::map<std::string, Camera> camera_map;

	std::vector<TYPE> machineAreas;

	void updateAliasNameMap(const Camera& camera);
	std::map<std::string, std::string> alias_name_map;

	// used when we need to return values from a requested camera name that does not exist 
	Camera dummy_cam;

	//const std::vector<std::string> cam_monitor_records;

};

#endif // CAMERA_FACTORY_H_