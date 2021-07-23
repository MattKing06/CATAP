#ifndef LASER_MIRROR_H_
#define LASER_MIRROR_H_

#include <Hardware.h>
#include <EPICSLaserMirrorInterface.h>
#include <LaserMirrorPVRecords.h>
#include <GlobalConstants.h>
#include <GlobalStateEnums.h>
#include <boost/make_shared.hpp>

class EPICSLaserMirrorInterface;

typedef boost::shared_ptr<EPICSLaserMirrorInterface> EPICSLaserMirrorInterface_sptr;

class LaserMirror : public Hardware
{
public:
	LaserMirror();
	LaserMirror(const std::map<std::string, std::string>& paramMap, STATE mode);
	LaserMirror(const LaserMirror& copyLaserMirror);
	~LaserMirror();
	void setPVStructs();
	EPICSLaserMirrorInterface_sptr epicsInterface;
	std::map<std::string, std::string> LaserMirrorParamMap;
	std::pair<epicsTimeStamp, double> currentHorizontalPosition;
	std::pair<epicsTimeStamp, double> currentVerticalPosition;
	double hStep;
	double vStep;
	double maximumStepSize;
	double leftSense;
	double rightSense;
	double upSense;
	double downSense;

	void setHStep(const double& value);
	void setVStep(const double& value);
	double getHStep();
	double getVStep();
	bool moveHorizontally();
	bool moveVertically();
	bool moveLeft(const double& value);
	bool moveRight(const double& value);
	bool moveUp(const double& value);
	bool moveDown(const double& value);
	double getCurrentHorizontalPosition();
	double getCurrentVerticalPosition();
	bool updatePositions();
	double getMaximumStepSize();
	double getLeftSense();
	double getRightSense();
	double getUpSense();
	double getDownSense();

	void debugMessagesOn();
	void debugMessagesOff();
	void messagesOn();
	void messagesOff();
};


#endif //LASER_MIRROR_H_