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



	std::vector<std::string> getCameraNames();
	boost::python::list getCameraNames_Py();

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

	
	std::string getFullName(const std::string& name_to_check) const;

	/*! get the name alises for this 
	@param[out] names, vector containing  all the alias names */
	std::vector<std::string> getAliases(const std::string cam_name) const;
	/*! get the name alises for this LLRF (python version)
		@param[out] names, python list containing all the alias names */
	boost::python::list getAliases_Py(const std::string cam_name) const;
	
	std::string getScreen(const std::string cam_name) const;
	std::vector<std::string> getScreenNames(const std::string cam_name) const;
	boost::python::list getScreenNames_Py(const std::string cam_name) const;

	void debugMessagesOn();
	void debugMessagesOff();
	void messagesOn();
	void messagesOff();
	bool isDebugOn();
	bool isMessagingOn();


	bool stopAcquiring(const std::string& cam_name);
	bool stopAllAcquiring();
	bool stopAllAcquiringExceptVC();
	bool startAcquiring(const std::string& cam_name, bool stop_all = true);



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

};

#endif // CAMERA_FACTORY_H_