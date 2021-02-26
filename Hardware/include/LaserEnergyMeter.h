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
	/*! Default constructor call for laser energy meter object*/
	LaserEnergyMeter();
	/*! Custom constructor for laser energy meter object
		@param[in] laserParametersAndValuesMap: strings defining parameters extracted from YAML config files
		@param[in] mode: Defines the STATE of laser energy meter we create: PHYSICAL (connected to CLARA EPICS), VIRTUAL (connected to Virtual EPICS), Offline (no EPICS)
	*/
	LaserEnergyMeter(const std::map<std::string, std::string>& laserParametersAndValuesMap, STATE mode);
	/*! Copy constructor call for laser energy meter object*/
	LaserEnergyMeter(const LaserEnergyMeter& copyLaserEnergyMeter);
	EPICSLaserEnergyMeterInterface_sptr epicsInterface;
	/*! Returns the EPICS name of this laser energy meter instance */
	std::string getLaserEnergyMeterName() const;
	/*! Returns the alias name (not currently used) */
	std::vector<std::string> getAliases() const;
	/*! Returns the CATAP TYPE of the laser energy meter */
	TYPE getLaserEnergyMeterType() const;
	/*! A map for storing the parameters extracted from YAML config files and their values */
	std::map<std::string, std::string> laserParametersAndValuesMap;
	LoggingSystem messenger;
	/*! Goes through all PV records stored in LaserEnergyMeterRecords namespace, creates pvStructs and sets up their record/fullPVName before
	inserting them into a vector of pvStructs which is later used by the LaserEnergyMeterFactory to setup connections to EPICS.
	*/
	void setPVStructs();
	/*! turns debug messaging on for this LaserEnergyMeter instance*/
	void debugMessagesOn();
	/*! turns debug messaging off for this LaserEnergyMeter instance*/
	void debugMessagesOff();
	/*! turns messaging on for this LaserEnergyMeter instance*/
	void messagesOn();
	/*! turns messaging off for this LaserEnergyMeter instance*/
	void messagesOff();
	void offlineSet(const long& value);
	/*! returns true if acquiring.*/
	bool getAcquiring() const;
	/*! returns true if monitoring.*/
	bool isMonitoring() const;
	/*! start acquiring shots.*/
	bool setStart();
	/*! stop acquiring shots.*/
	bool setStop();
	/*! sets the laser energy range
	@param[in] value : The desired range -- 0 = 20nJ, 1 = 200nJ, 2 = 2uJ, 3 = 20uJ */
	bool setRange(int range);
	/*! returns the laser energy range setting.*/
	int getRange() const;
	/*! returns 1 if energy is out of range.*/
	int getOverRange() const;
	/*! returns the laser energy.
	@param[out] value: laser energy value.*/
	double getEnergy() const;
	/*! returns the laser energy vector.*/
	std::vector< double > getEnergyVector() const;
	/*! returns the laser energy buffer.*/
	boost::circular_buffer< double > getEnergyBuffer() const;
	/*! returns true if the laser energy vector is full.*/
	bool isEnergyVectorFull() const;
	/*! returns true if the laser energy buffer is full.*/
	bool isEnergyBufferFull() const;
	/*! sets the laser energy from EPICS.
	@param[in] value: laser energy value.*/
	bool setEnergy(const double& value);
	/*! sets the size of energy buffers.
	@param[in] value: buffer size.*/
	/*! fill up data vectors.
	@param[in] value: number of shots to monitor.*/
	void monitorForNShots(const size_t& value);
	/*! sets the size of energy buffers.
	@param[in] value: buffer size.*/
	void setBufferSize(const size_t& value);
	/*! sets the size of energy vector.
	@param[in] value: vector size.*/
	void setVectorSize(const size_t& value);
	/*! remove all data from buffers.*/
	void clearBuffers();
	/*! remove all data from energy vector.*/
	void clearVectors();
	/*! start button for acquiring laser energy.*/
	std::pair<epicsTimeStamp, int> start;
	/*! stop button for acquiring laser energy.*/
	std::pair<epicsTimeStamp, int> stop;
	/*! overrange parameter.*/
	std::pair<epicsTimeStamp, int> overrange;
	/*! laser energy range setting.*/
	std::pair<epicsTimeStamp, int> range;
	/*! current laser energy value.*/
	std::pair<epicsTimeStamp, double> energy;
	/*! vector of laser energy values (used in conjunction with monitorForNShots).*/
	std::vector< double > energyvector;
	/*! rolling buffer of laser energy values.*/
	boost::circular_buffer< double > energybuffer;
	/*! bool stating whether the energy meter is acquiring data.*/
	bool acquiring;
	/*! bool stating whether you are monitoring the laser energy.*/
	bool monitoring;
	/*! beamline position (useless).*/
	double position;
	/*! laser energy meter name.*/
	std::string name;
	/*! number of shots monitored.*/
	size_t shotsvector = 0;
	/*! rolling buffer shots.*/
	size_t shotsbuffer = 0;
	/*! Buffer size for energy monitoring.*/
	size_t bufferSize = 10;
	/*! Vector size for energy monitoring.*/
	size_t vectorSize = 10;
	friend class EPICSLaserEnergyMeterInterface;
protected:
	//what else does a laser need?
	std::vector<std::string> aliases;
	TYPE laserEnergyMeterType;
};

#endif //LASER_H_