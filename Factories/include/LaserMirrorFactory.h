#ifndef LASER_MIRROR_FACTORY_H_
#define LASER_MIRROR_FACTORY_H_

#include <ConfigReader.h>
#include <LaserMirror.h>
#include <GlobalStateEnums.h>
#include <GlobalFunctions.h>
#include <vector>
class LaserMirrorFactory
{
public:
	LaserMirrorFactory();
	LaserMirrorFactory(STATE mode);
	LaserMirrorFactory(STATE mode, const std::string& primeLatticeLocation);
	LaserMirrorFactory(const LaserMirrorFactory& copyFactory);
	~LaserMirrorFactory();
	/*! Attach to the initial CA context for multi-threaded applications for a given laserMirror
	@param[in] laserMirrorName: Name of the laserMirror to attach to CA Context*/
	void attachContext(const std::string& laserMirrorName);
	/*! Attach to the initial CA context for multi-threaded applications for given laserMirrors
	@param[in] laserMirrorNames: Names of the laserMirrors to attach to CA Context*/
	void attachContext(std::vector<std::string>& laserMirrorNames);
	/*! Attach to the initial CA context for multi-threaded applications for given laserMirrors
	@param[in] laserMirrorNames: List of names of the laserMirrors to attach to CA Context*/
	void attachContext_Py(boost::python::list laserMirrorNames);
	/*! Attach to the initial CA context for multi-threaded applications for all laserMirrors*/
	void attachContext();

	/*! detach to the initial CA context for multi-threaded applications for a given laserMirror
	@param[in] laserMirrorName: Name of the laserMirror to detach to CA Context*/
	void detachContext(const std::string& laserMirrorName);
	/*! detach to the initial CA context for multi-threaded applications for given laserMirrors
	@param[in] laserMirrorNames: Names of the laserMirrors to detach to CA Context*/
	void detachContext(std::vector<std::string>& laserMirrorNames);
	/*! detach to the initial CA context for multi-threaded applications for given laserMirrors
	@param[in] laserMirrorNames: List of names of the laserMirrors to detach to CA Context*/
	void detachContext_Py(boost::python::list laserMirrorNames);
	/*! detach to the initial CA context for multi-threaded applications for all laserMirrors*/
	void detachContext();
	bool setup(const std::string& version);
	void populateLaserMirrorMap();
	void retrieveMonitorStatus(pvStruct& pvStruct);
	void setupChannels();
	LaserMirror& getLaserMirror(const std::string& name);


	void setHStep(const std::string& mirrorName, const double& value);
	void setVStep(const std::string& mirrorName, const double& value);
	double getHStep(const std::string& mirrorName);
	double getVStep(const std::string& mirrorName);
	//bool moveHorizontally(const std::string& mirrorName);
///	bool moveVertically(const std::string& mirrorName);
	bool moveLeft(const std::string& mirrorName);
	bool moveRight(const std::string& mirrorName);
	bool moveUp(const std::string& mirrorName);
	bool moveDown(const std::string& mirrorName);
	double getCurrentHorizontalPosition(const std::string& mirrorName);
	double getCurrentVerticalPosition(const std::string& mirrorName);
	bool updatePositions(const std::string& mirrorName);
	double getMaximumStepSize(const std::string& mirrorName);
	double getLeftSense(const std::string& mirrorName);
	double getRightSense(const std::string& mirrorName);
	double getUpSense(const std::string& mirrorName);
	double getDownSense(const std::string& mirrorName);
	void debugMessagesOn();
	void debugMessagesOff();
	void messagesOn();
	void messagesOff();
	bool isDebugOn();
	bool isMessagingOn();
	LoggingSystem messenger;
	ConfigReader reader;
	bool hasBeenSetup;
	STATE mode;
	std::map<std::string, LaserMirror> laserMirrorMap;
};

#endif // LASER_MIRROR_FACTORY_H_