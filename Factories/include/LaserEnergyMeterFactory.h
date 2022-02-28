#ifndef LASER_ENERGY_METER_FACTORY_H_
#define LASER_ENERGY_METER_FACTORY_H_

#include "LoggingSystem.h"
#include "ConfigReader.h"
#pragma once
#include "LaserEnergyMeter.h"
#include <vector>
#include <map>
#include <utility>
#include <boost/python.hpp>
#include <boost/circular_buffer.hpp>

typedef void(*updateFunctionPtr)(struct event_handler_args args);
class LaserEnergyMeter;
class LaserEnergyMeterFactory
{
public:
	/*! Default constructor for LaserEnergyMeterFactory.
		Calls the custom constructor with mode = OFFLINE.*/
	LaserEnergyMeterFactory();
	/*! Custom constructor for LaserEnergyMeterFactory
	@param[in] mode The type of LaserEnergyMeterFactory we wish to create.
	PHYSICAL LaserEnergyMeterFactory will perform operations on CLARA/VELA laser energy meters.
	VIRTUAL LaserEnergyMeterFactory will perform operations of Virtual laser energy meters.
	OFFLINE LaserEnergyMeterFactory will perform operations on Offline laser energy meters.*/
	LaserEnergyMeterFactory(STATE mode);
	LaserEnergyMeterFactory(STATE mode, const std::string& primeLatticeLocation);
	/*! Copy constructor for LaserEnergyMeterFactory (used for = operator)
	@param[in] copyLaserEnergyMeterFactory: the LaserEnergyMeterFactory we want to copy over.*/
	LaserEnergyMeterFactory(const LaserEnergyMeterFactory& copyLaserEnergyMeterFactory);
	/*! LaserEnergyMeterFactory destructor.
	Goes through all LaserEnergyMeter objects in the LaserEnergyMeterFactoryMap and removes connections/subscriptions to EPICS if they exist.*/
	~LaserEnergyMeterFactory();
	/*! The main function that sets up EPICS connections and subscriptions to all
		screens in the laser energy meter map. We set up all channels before sending them to EPICS
		for efficiency. After channels, the remaining data for pvStructs are set as well
		as any subscriptions that are needed for monitoring.
		@param[in] version NOT IN USE
		@param[out] hasBeenSetup false if no YAML config files to parse, true otherwise.*/
	bool setup(const std::string& VERSION);
    LoggingSystem messenger;
	ConfigReader reader;
	/*! Attach to the initial CA context for multi-threaded applications for a given LaserEnergyMeter
	@param[in] LaserEnergyMeterName: Name of the LaserEnergyMeter to attach to CA Context*/
	void attachContext(const std::string& LaserEnergyMeterName);
	/*! Attach to the initial CA context for multi-threaded applications for given LaserEnergyMeters
	@param[in] LaserEnergyMeterNames: Names of the LaserEnergyMeters to attach to CA Context*/
	void attachContext(std::vector<std::string>& LaserEnergyMeterNames);
	/*! Attach to the initial CA context for multi-threaded applications for given LaserEnergyMeters
	@param[in] LaserEnergyMeterNames: List of names of the LaserEnergyMeters to attach to CA Context*/
	void attachContext_Py(boost::python::list LaserEnergyMeterNames);
	/*! Attach to the initial CA context for multi-threaded applications for all LaserEnergyMeters*/
	void attachContext();
	/*! returns laser energy meter by name
	@param name checks the name in the alias map if not full name.
			and returns the laser energy meter hardware object stored in laserEnergyMeterMap at that full name
	*/
	LaserEnergyMeter& getLaserEnergyMeter(const std::string& fullLaserName);
	std::map<std::string, LaserEnergyMeter> getLaserEnergyMeters(std::vector<std::string> laserNames);
	std::map<std::string, LaserEnergyMeter> getAllLaserEnergyMeters();
	std::map<std::string, LaserEnergyMeter> laserEnergyMeterMap;
	std::vector<std::string> getAllLaserEnergyMeterNames();
	boost::python::list getAllLaserEnergyMeterNames_Py();
	/*!gets all the full name of a certain laser energy meter.
	@param[in] name: the name of the laser energy meter.
	@param[out] name : the name of the laser energy meter. Not sure why this function is here.*/
	std::string getLaserEnergyMeterName(const std::string& name);
	/*! passes an empty laserEnergyMeterMap to the configReader to be populated by laser energy meter objects that are
	created from the YAML Config files.*/
	void populateLaserEnergyMeterMap();
	/*! sets the pvStruct monitor flag to true if we want to monitor the PV
		and otherwise this flag is set to false
		@param[in] pvStruct the pvStruct which we want to set the monitor flag of.
	*/
	void retrievemonitorStatus(pvStruct& pvStruct);
	/*! iterates over all BPMs in bpmMap and uses their pvStructs vector to create
		connections to EPICS if mode is PHYSICAL or VIRTUAL*/
	void setupChannels();
	bool hasBeenSetup;
	//bool isVirtual;
	STATE mode;
	/*! is the meter acquiring laser energy values?
	@param[in] name: the laser energy meter name.
	@param[out] bool: true if yes. */
	bool getAcquiring(const std::string& name) const;
	/*! starts acquiring laser energy
	@param[in] name: the laser energy meter name.
	@param[out] bool: true if successful. */
	bool setStart(const std::string& name);
	/*! stops acquiring laser energy
	@param[in] name: the laser energy meter name.
	@param[out] bool: true if successful. */
	bool setStop(const std::string& name);
	/*! set the range of the laser energy meter
	@param[in] name: the laser energy meter name.
	@param[in] int : The desired range -- 0 = 20nJ, 1 = 200nJ, 2 = 2uJ, 3 = 20uJ
	@param[out] bool: true if successful.*/
	bool setRange(const std::string& name, int range);
	/*! returns the over-range parameter for the laser energy meter
	@param[in] name: the laser energy meter name.
	@param[out] int: 0 if not overrange, 1 if overrange.*/
	int getOverRange(const std::string& name) const;
	/*! returns the set range of the laser energy meter
	@param[in] name: the laser energy meter name.
	@param[out] int: range set point.*/
	int getRange(const std::string& name) const;
	/*! returns the laser energy
	@param[in] name: the laser energy meter name.
	@param[out] double: laser energy value.*/
	double getEnergy(const std::string& name) const;
	/*! returns the laser energy vector
	@param[in] name: the laser energy meter name.
	@param[out] vector: laser energy vector.*/
	std::vector< double > getEnergyVector(const std::string& name) const;
	/*! returns the laser energy buffer
	@param[in] name: the laser energy meter name.
	@param[out] buffer: laser energy buffer.*/
	boost::circular_buffer< double > getEnergyBuffer(const std::string& name) const;
	/*! is the laser energy vector being filled?
	@param[in] name: the laser energy meter name.
	@param[out] bool: true if yes.*/
	bool isMonitoring(const std::string& name) const;
	/*! is the laser energy vector full?
	@param[in] name: the laser energy meter name.
	@param[out] bool: true if yes.*/
	bool isEnergyVectorFull(const std::string& name) const;
	/*! is the laser energy buffer full?
	@param[in] name: the laser energy meter name.
	@param[out] bool: true if yes.*/
	bool isEnergyBufferFull(const std::string& name) const;
	/*! fill up n shots of laser energy to the laser energy vector.
	@param[in] name: the laser energy meter name.
	@param[in] value: number of shots to monitor.*/
	void monitorForNShots(const std::string& name, const size_t& value);

	/*! sets the RS size (m_max) for a given laser energy meter*/
	void setRunningStatSize(const std::string& name, const size_t& size);
	/*! clears the RS object for a given laser energy meter */
	void clearRunningStats(const std::string& name);
	/*! clears the RS object for a given laser energy meter */
	bool areRunningStatsFull(const std::string& name);
	/*! Get the current number of data values being used by the Running Stats.
	@param[in] name: the laser energy meter name.
	@param[out] size_t: number of data values.*/
	size_t getRunningStatNumDataValues(const std::string& name)const;

	/*! set the size of the laser energy vector.
	@param[in] name: the laser energy meter name.
	@param[in] value: vector size.*/
	void setVectorSize(const std::string& name, const size_t& value);
	/*! set the size of the laser energy buffer.
	@param[in] name: the laser energy meter name.
	@param[in] value: buffer size.*/
	void setBufferSize(const std::string& name, const size_t& value);
	/*! remove all data from laser energy vector.
	@param[in] name: the laser energy meter name. */
	void clearVectors(const std::string& name);
	/*! remove all data from laser energy buffer.
	@param[in] name: the laser energy meter name. */
	void clearBuffers(const std::string& name);
	/*!returns a Python list of laser energy PV values from the vector for the specified laser energy meter (used w/ monitorForNShots).
	@param[in] name: the laser energy meter name.
	@param[out] list: the laser energy vector.*/
	boost::python::list getEnergyVector_Py(const std::string& name);
	/*!returns a Python list of laser energy PV values from the buffer for the specified laser energy meter.
	@param[in] name: the laser energy meter name.
	@param[out] list: the laser energy buffer.*/
	boost::python::list getEnergyBuffer_Py(const std::string& name);
	/*! turns debug messages on for LaserEnergyMeterFactory and calls same function in all LaserEnergyMeters and configReader*/
	void debugMessagesOn();
	/*! turns debug messages off for LaserEnergyMeterFactory and calls same function in all LaserEnergyMeters and configReader*/
	void debugMessagesOff();
	/*! turns messages on for LaserEnergyMeterFactory and calls same function in all LaserEnergyMeters and configReader*/
	void messagesOn();
	/*! turns messages off for LaserEnergyMeterFactory and calls same function in all LaserEnergyMeters and configReader*/
	void messagesOff();
	/*! returns true if messenger debug flag is true, false otherwise*/
	bool isDebugOn();
	/*! returns true if messenger messages flag is true, false otherwise*/
	bool isMessagingOn();
};


#endif // LASER_FACTORY_H_