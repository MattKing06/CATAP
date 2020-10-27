#ifndef LASER_ENERGY_METER_H_
#define LASER_ENERGY_METER_H_
#include "LoggingSystem.h"
#ifndef HARDWARE_H_
#include "Hardware.h"
#endif //HARDWARE_H_
#include "EPICSLaserEnergyMeterInterface.h"
#include "LaserEnergyMeterPVRecords.h"
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/circular_buffer.hpp>

class EPICSLaserEnergyMeterInterface;
typedef boost::shared_ptr<EPICSLaserEnergyMeterInterface> EPICSLaserEnergyMeterInterface_sptr;
class LaserEnergyMeter : public Hardware
{
public:
	LaserEnergyMeter();
	//should need a BPM name (full PV root, or alias can be given)
	//BPM(Hardware hardware); // this should be possible, but isn't useful right now.
	LaserEnergyMeter(std::string knownNameOfLaser);
	LaserEnergyMeter(const std::map<std::string, std::string>& laserParametersAndValuesMap, STATE mode);
	LaserEnergyMeter(const LaserEnergyMeter& copyLaserEnergyMeter);
	EPICSLaserEnergyMeterInterface_sptr epicsInterface;
	std::string getLaserEnergyMeterName() const;
	std::vector<std::string> getAliases() const;
	TYPE getLaserEnergyMeterType() const;
	std::map<std::string, std::string> laserParametersAndValuesMap;
	LoggingSystem messenger;
	void setPVStructs();
	void debugMessagesOn();
	void debugMessagesOff();
	void messagesOn();
	void messagesOff();
	void offlineSet(const long& value);
	bool monitoringData = false;
	bool getAcquiring() const;
	bool setStart();
	bool setStop();
	bool setRange(int range);
	bool setOverRange() const;
	bool setNotOverRange() const;
	int getRange() const;
	int getOverRange() const;
	std::pair<epicsTimeStamp, int> start;
	std::pair<epicsTimeStamp, int> stop;
	std::pair<epicsTimeStamp, int> overrange;
	std::pair<epicsTimeStamp, int> range;
	bool acquiring;
	double position;
	std::string name;
	friend class EPICSLaserEnergyMeterInterface;
protected:
	//what else does a laser need?
	std::vector<std::string> aliases;
	TYPE laserEnergyMeterType;
};

#endif //LASER_H_