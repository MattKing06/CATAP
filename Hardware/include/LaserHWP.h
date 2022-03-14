#ifndef LASER_HWP_H_
#define LASER_HWP_H_
#include "LoggingSystem.h"
#ifndef HARDWARE_H_
#include "Hardware.h"
#endif //HARDWARE_H_
#include "EPICSLaserHWPInterface.h"
#include "LaserHWPPVRecords.h"
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/circular_buffer.hpp>

class EPICSLaserHWPInterface;
typedef boost::shared_ptr<EPICSLaserHWPInterface> EPICSLaserHWPInterface_sptr;
class LaserHWP : public Hardware
{
public:
	LaserHWP();
	//should need a BPM name (full PV root, or alias can be given)
	//BPM(Hardware hardware); // this should be possible, but isn't useful right now.
	LaserHWP(std::string knownNameOfLaser);
	LaserHWP(const std::map<std::string, std::string>& laserParametersAndValuesMap, STATE mode);
	LaserHWP(const LaserHWP& copyLaser);
	EPICSLaserHWPInterface_sptr epicsInterface;
	/*! Attach to the initial CA Context for use in Multi-threaded applications*/
	void attachToInitialContext();
	/*! Detach from initial CA Context for use in Multi-threaded applications*/
	void detachFromInitialContext();
	std::string getLaserHWPName() const;
	std::vector<std::string> getAliases() const;
	TYPE getLaserHWPType() const;
	std::map<std::string, std::string> laserParametersAndValuesMap;
	LoggingSystem messenger;
	void setPVStructs();
	void debugMessagesOn();
	void debugMessagesOff();
	void messagesOn();
	void messagesOff();
	void offlineSet(const long& value);
	bool monitoringData;

	STATE getHWPEnableState()const;
	bool isHWPEnabled()const;
	bool isHWPDisabled()const;
	bool enableHWP();
	bool disableHWP();

	bool setHWP(double value);
	double getHWPSet() const;
	double getHWPRead() const;
	double position;
	std::string name;
	friend class EPICSLaserHWPInterface;
protected:
	//what else does a laser need?
	std::vector<std::string> aliases;

	std::pair<epicsTimeStamp, double> hwpset;
	std::pair<epicsTimeStamp, double> hwpread;
	std::pair<epicsTimeStamp, STATE> hwp_enable;

	

	TYPE laserType;
};

#endif //LASER_H_