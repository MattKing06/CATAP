#ifndef BPM_FACTORY_H_
#define BPM_FACTORY_H_

#include "LoggingSystem.h"
#include "ConfigReader.h"
#pragma once
#include "BPM.h"
#include <vector>
#include <map>
#include <utility>
#include <boost/python.hpp>
#include <boost/circular_buffer.hpp>

typedef void(*updateFunctionPtr)(struct event_handler_args args);

/** @addtogroup factories
 @{*/
 /*! A class to store, setup, and perform operations on multiple BPM objects*/
class BPM;
class BPMFactory
{
public:
	/*! Default constructor for BPMFactory.
		Calls the custom constructor with mode = OFFLINE.*/
	BPMFactory();
	/*! Custom constructor for BPMFactory
	@param[in] mode The type of BPMFactory we wish to create.
	PHYSICAL BPMFactory will perform operations on CLARA/VELA BPMs.
	VIRTUAL BPMFactory will perform operations of Virtual BPMs.
	OFFLINE BPMFactory will perform operations on Offline BPMs.*/
	BPMFactory(STATE mode);
	BPMFactory(STATE mode, const std::string& primeLatticeLocation);
	/*! Copy constructor for BPMFactory (used for = operator)
	@param[in] copyBPMFactory: the BPMFactory we want to copy over.*/
	BPMFactory(const BPMFactory& copyBPMFactory);
	/*! BPMFactory destructor.
	Goes through all BPM objects in the bpmMap and removes connections/subscriptions to EPICS if they exist.*/
	~BPMFactory();
	/*! The main function that sets up EPICS connections and subscriptions to all
		screens in the BPM map. We set up all channels before sending them to EPICS
		for efficiency. After channels, the remaining data for pvStructs are set as well
		as any subscriptions that are needed for monitoring.
		@param[in] version NOT IN USE
		@param[out] hasBeenSetup false if no YAML config files to parse, true otherwise.*/
	bool setup(const std::string& VERSION);
	/*! Attach to the initial CA context for multi-threaded applications for a given bpm
	@param[in] bpmName: Name of the bpm to attach to CA Context*/
	void attachContext(const std::string& bpmName);
	/*! Attach to the initial CA context for multi-threaded applications for given bpms
	@param[in] bpmNames: Names of the bpms to attach to CA Context*/
	void attachContext(std::vector<std::string>& bpmNames);
	/*! Attach to the initial CA context for multi-threaded applications for given bpms
	@param[in] bpmNames: List of names of the bpms to attach to CA Context*/
	void attachContext_Py(boost::python::list bpmNames);
	/*! Atach to the initial CA context for multi-threaded applications for all bpms*/
	void attachContext();
	/*! Detach to the initial CA context for multi-threaded applications for a given bpm
	@param[in] bpmName: Name of the bpm to attach to CA Context*/
	void detachContext(const std::string& bpmName);
	/*! Detach to the initial CA context for multi-threaded applications for given bpms
	@param[in] bpmNames: Names of the bpms to attach to CA Context*/
	void detachContext(std::vector<std::string>& bpmNames);
	/*! Detach to the initial CA context for multi-threaded applications for given bpms
	@param[in] bpmNames: List of names of the bpms to attach to CA Context*/
	void detachContext_Py(boost::python::list bpmNames);
	/*! Detach to the initial CA context for multi-threaded applications for all bpms*/
	void detachContext();
	/*! The kind of BPMFactory that is created (PHYSICAL, VIRTUAL, OFFLINE), this variable is passed to the
	BPM hardware objects when they are created by ConfigReader*/
	STATE mode;
	/*! Used to print messages both normal and debug, also sets the On/Off flags for debug and normal messages*/
	LoggingSystem messenger;
	/*! ConfigReader to parse YAML config files and create associated BPM objects*/
	ConfigReader reader;
	/*! Map containing all BPMs created from ConfigReader, keyed by harwdare name*/
	std::map<std::string, BPM> bpmMap;
	/*! Flag to say whether the BPMFactory setup function has been completed successfully.*/
	bool hasBeenSetup;
	/*! passes an empty bpmMap to the configReader to be populated by BPM objects that are
	created from the YAML Config files.*/
	void populateBPMMap();
	/*! iterates over all BPMs in bpmMap and uses their pvStructs vector to create
		connections to EPICS if mode is PHYSICAL or VIRTUAL*/
	void setupChannels();
	/*! sets the pvStruct monitor flag to true if we want to monitor the PV
		and otherwise this flag is set to false
		@param[in] pvStruct the pvStruct which we want to set the monitor flag of.
	*/
	void retrievemonitorStatus(pvStruct& pvStruct);
	/*! returns BPM by name
	@param name checks the name in the alias map if not full name.
			and returns the BPM hardware stored in bpmMap at that full name
	*/
	BPM& getBPM(const std::string& fullBPMName);
	/*!gets all the full name of a certain BPM.
	@param[in] name: the name of the BPM.
	@param[out] name : the name of the BPM. Not sure why this function is here.*/
	std::string getBPMName(const std::string& name);
	/*!returns all BPM names defined in the config file.
	@param[out] std::vector<std::string,BPM>: returns a map of BPM object names.*/
	std::vector<std::string> getAllBPMNames();
	/*!returns the speficied BPM hardware objects.
	@param[in] std::vector<std::string> names: the names of the BPMs.
	@param[out] std::map<std::string,BPM>: returns a map of BPM objects, keyed by name.*/
	std::map<std::string, BPM> getBPMs(std::vector<std::string> bpmNames);
	/*!returns all BPM hardware objects.
	@param[out] std::map<std::string,BPM>: returns a map of BPM objects, keyed by name.*/
	std::map<std::string, BPM> getAllBPMs();
	/*!records X, Y and raw data in vectors (accessible directly through the BPM hardware object, or the getAVector functions).
	@param[in] name: the name of the BPM.
	@param[in] value: the number of shots to record.*/
	void monitorForNShots(const std::string& name, const size_t& value);
	/*!is the X PV being monitored (see monitorForNShots).
	@param[in] name: the name of the BPM.
	@param[out] bool: true if yes.*/
	bool ismonitoringXPV(const std::string& name);
	/*!is the Y PV being monitored (see monitorForNShots).
	@param[in] name: the name of the BPM.
	@param[out] bool: true if yes.*/
	bool ismonitoringYPV(const std::string& name);
	/*!is the raw data PV being monitored (see monitorForNShots).
	@param[in] name: the name of the BPM.
	@param[out] bool: true if yes.*/
	bool ismonitoringData(const std::string& name);
	/*!is the BPM being monitored (see monitorForNShots).
	@param[in] name: the name of the BPM.
	@param[out] bool: true if yes.*/
	bool ismonitoring(const std::string& name);
	/*!returns X values calculated from the raw data vector for the specified BPMs.
	@param[in] names: the names of the BPMs.
	@param[out] values: a map of the X  values, keyed by name.*/
	std::map<std::string, double> getXs(const std::vector<std::string>& names);
	/*!returns X PV values for the specified BPMs.
	@param[in] names: the names of the BPMs.
	@param[out] values: a map of the X PV values, keyed by name.*/
	std::map<std::string, double> getXsFromPV(const std::vector<std::string>& names);
	/*!returns Y values calculated from the raw data vector for the specified BPMs.
	@param[in] names: the names of the BPMs.
	@param[out] values: a map of the Y values, keyed by name.*/
	std::map<std::string, double> getYs(const std::vector<std::string>& names);
	/*!returns Y PV values for the specified BPMs.
	@param[in] names: the names of the BPMs.
	@param[out] values: a map of the Y PV values, keyed by name.*/
	std::map<std::string, double> getYsFromPV(const std::vector<std::string>& names);
	/*!returns charge values for the specified BPMs.
	@param[in] names: the names of the BPMs.
	@param[out] values: a map of the charge values, keyed by name.*/
	std::map<std::string, double> getQs(const std::vector<std::string>& names);
	/*!returns status parameters for the specified BPMs.
	@param[in] names: the names of the BPMs.
	@param[out] states: a map of the statuses, keyed by name.*/
	std::map<std::string, STATE> getStatuses(const std::vector<std::string>& names);
	/*!returns raw data vectors for the specified BPMs.
	@param[in] names: the names of the BPMs.
	@param[out] values: a map of the raw data vectors, keyed by name.*/
	std::map<std::string, std::vector< double > > getDatas(const std::vector<std::string>& names);
	/*!returns X/Y values for the specified BPMs.
	@param[in] names: the names of the BPMs.
	@param[out] values: a map of the X/Y values, keyed by name.*/
	std::map<std::string, std::pair<double, double>> getXYPositions(const std::vector<std::string>& names);
	/*!returns X PV vectors for the specified BPMs (after using monitorForNShots).
	@param[in] names: the names of the BPMs.
	@param[out] values: a map of the X PV vectors, keyed by name.*/
	std::map<std::string, std::vector< double > > getXPVVectors(const std::vector<std::string>& names);
	/*!returns Y PV vectors for the specified BPMs (after using monitorForNShots).
	@param[in] names: the names of the BPMs.
	@param[out] values: a map of the Y PV vectors, keyed by name.*/
	std::map<std::string, std::vector< double > > getYPVVectors(const std::vector<std::string>& names);
	/*!returns charge vectors for the specified BPMs (after using monitorForNShots).
	@param[in] names: the names of the BPMs.
	@param[out] values: a map of the charge vectors, keyed by name.*/
	std::map<std::string, std::vector< double > > getQVectors(const std::vector<std::string>& names);
	/*!returns status vectors for the specified BPMs (after using monitorForNShots).
	@param[in] names: the names of the BPMs.
	@param[out] states: a map of the status vectors, keyed by name.*/
	std::map<std::string, std::vector< STATE > > getStatusVectors(const std::vector<std::string>& names);
	/*!returns raw data vectors for the specified BPMs (after using monitorForNShots).
	@param[in] names: the names of the BPMs.
	@param[out] values: a map of the raw data vectors, keyed by name.*/
	std::map<std::string, std::vector< std::vector< double > > > getDataVectors(const std::vector<std::string>& names);
	/*!returns X/Y position vectors for the specified BPMs (after using monitorForNShots).
	@param[in] names: the names of the BPMs.
	@param[out] values: a map of the X/Y position vectors, keyed by name.*/
	std::map<std::string, std::pair<std::vector< double >, std::vector< double > > > getXYPositionVectors(const std::vector<std::string>& names);
	/*!returns X PV buffers for the specified BPMs.
	@param[in] names: the names of the BPMs.
	@param[out] values: a map of the X PV buffers, keyed by name.*/
	std::map<std::string, boost::circular_buffer< double > > getXPVBuffers(const std::vector<std::string>& names);
	/*!returns Y PV buffers for the specified BPMs.
	@param[in] names: the names of the BPMs.
	@param[out] values: a map of the Y PV buffers, keyed by name.*/
	std::map<std::string, boost::circular_buffer< double > > getYPVBuffers(const std::vector<std::string>& names);
	/*!returns charge buffers for the specified BPMs.
	@param[in] names: the names of the BPMs.
	@param[out] values: a map of the charge buffers, keyed by name.*/
	std::map<std::string, boost::circular_buffer< double > > getQBuffers(const std::vector<std::string>& names);
	/*!returns status buffers for the specified BPMs.
	@param[in] names: the names of the BPMs.
	@param[out] states: a map of the status buffers, keyed by name.*/
	std::map<std::string, boost::circular_buffer< STATE > > getStatusBuffers(const std::vector<std::string>& names);
	/*!returns raw data buffers for the specified BPMs.
	@param[in] names: the names of the BPMs.
	@param[out] values: a map of the raw data buffers, keyed by name.*/
	std::map<std::string, boost::circular_buffer< std::vector< double > > > getDataBuffers(const std::vector<std::string>& names);
	/*!returns the resolution the specified BPMs.
	@param[in] names: the names of the BPMs.
	@param[out] values: a map of the resolution values, keyed by name.*/
	std::map<std::string, double> getResolutions(const std::vector<std::string>& names);
	/*!returns the beamline positions the specified BPMs.
	@param[in] names: the names of the BPMs.
	@param[out] values: a map of the positions, keyed by name.*/
	std::map<std::string, double> getPositions(const std::vector<std::string>& names);
	/*!recalibrates the attenuation values of the specified BPMs.
	@param[in] names: the names of the BPMs.
	@param[in] charge: the current bunch charge.
	@param[out] bools: a map of bools, keyed by name; true if the recalibration worked.*/
	std::map<std::string, bool> reCalAttenuations(const std::vector<std::string>& names, const double& charge);
	/*!returns X values calculated from the raw data vector for all BPMs.
	@param[out] values: a map of the X values, keyed by name.*/
	std::map<std::string, double> getAllX();
	/*!returns X PV values for all BPMs.
	@param[out] values: a map of the X PV values, keyed by name.*/
	std::map<std::string, double> getAllXFromPV();
	/*!returns Y values calculated from the raw data vector for all BPMs.
	@param[out] values: a map of the Y values, keyed by name.*/
	std::map<std::string, double> getAllY();
	/*!returns Y PV values for all BPMs.
	@param[out] values: a map of the Y PV values, keyed by name.*/
	std::map<std::string, double> getAllYFromPV();
	/*!returns charge values for all BPMs.
	@param[out] values: a map of the charge values, keyed by name.*/
	std::map<std::string, double> getAllQ();
	/*!returns the status of all BPMs.
	@param[out] states: a map of the statuses, keyed by name.*/
	std::map<std::string, STATE> getAllStatus();
	/*!returns raw data vectors for all BPMs.
	@param[out] values: a map of the raw data vectors, keyed by name.*/
	std::map<std::string, std::vector< double > > getAllData();
	/*!returns the resolution all BPMs.
	@param[out] values: a map of the resolution values, keyed by name.*/
	std::map<std::string, double> getAllResolution();
	/*!returns beamline position values for all BPMs.
	@param[out] values: a map of the position values, keyed by name.*/
	std::map<std::string, double> getAllPosition();
	/*!returns X/Y PV values for all BPMs.
	@param[out] values: a map of the X/Y PV values, keyed by name.*/
	std::map<std::string, std::pair<double, double>> getAllXYPosition();
	/*!returns X PV vectors for all BPMs (after using monitorForNShots).
	@param[out] values: a map of the X PV values, keyed by name.*/
	std::map<std::string, std::vector< double > > getAllXPVVector();
	/*!returns Y PV vectors for all BPMs (after using monitorForNShots).
	@param[out] values: a map of the Y PV values, keyed by name.*/
	std::map<std::string, std::vector< double > > getAllYPVVector();
	/*!returns charge vectors for all BPMs (after using monitorForNShots).
	@param[out] values: a map of the charge values, keyed by name.*/
	std::map<std::string, std::vector< double > > getAllQVector();
	/*!returns status vectors for all BPMs (after using monitorForNShots).
	@param[out] values: a map of the status vectors, keyed by name.*/
	std::map<std::string, std::vector< STATE > > getAllStatusVector();
	/*!returns raw data vectors for all BPMs (after using monitorForNShots).
	@param[out] values: a map of the raw data values, keyed by name.*/
	std::map<std::string, std::vector< std::vector< double > > > getAllDataVector();
	/*!returns X/Y PV vectors for all BPMs.
	@param[out] values: a map of the X/Y PV vectors, keyed by name.*/
	std::map<std::string, std::pair<std::vector< double >, std::vector< double > > > getAllXYPositionVectors();
	/*!returns X PV buffers for all BPMs.
	@param[out] values: a map of the X PV buffers, keyed by name.*/
	std::map<std::string, boost::circular_buffer< double > > getAllXPVBuffer();
	/*!returns Y PV buffers for all BPMs.
	@param[out] values: a map of the Y PV buffers, keyed by name.*/
	std::map<std::string, boost::circular_buffer< double > > getAllYPVBuffer();
	/*!returns charge buffers for all BPMs.
	@param[out] values: a map of the charge buffers, keyed by name.*/
	std::map<std::string, boost::circular_buffer< double > > getAllQBuffer();
	/*!returns status buffers for all BPMs.
	@param[out] values: a map of the status buffers, keyed by name.*/
	std::map<std::string, boost::circular_buffer< STATE > > getAllStatusBuffer();
	/*!returns raw data buffers for all BPMs.
	@param[out] values: a map of the raw data buffers, keyed by name.*/
	std::map<std::string, boost::circular_buffer< std::vector< double > > > getAllDataBuffer();
	/*!recalibrates the attenuation values of all specified BPMs.
	@param[in] charge: the current bunch charge.
	@param[out] bools: a map of bools, keyed by name; true if the recalibration worked.*/
	std::map<std::string, bool> reCalAllAttenuation(const double& charge);
	/*!returns the X value calculated from raw data of the specified BPM.
	@param[in] name: the name of the BPM.
	@param[out] value: the X value.*/
	double getX(const std::string& name);
	/*!returns the X PV value of the specified BPM.
	@param[in] name: the name of the BPM.
	@param[out] value: the X PV value.*/
	double getXFromPV(const std::string& name);
	/*!returns the Y value calculated from raw data of the specified BPM.
	@param[in] name: the name of the BPM.
	@param[out] value: the Y value.*/
	double getY(const std::string& name);
	/*!returns the Y PV value of the specified BPM.
	@param[in] name: the name of the BPM.
	@param[out] value: the Y PV value.*/
	double getYFromPV(const std::string& name);
	/*!returns the charge at the specified BPM.
	@param[in] name: the name of the BPM.
	@param[out] value: the charge value.*/
	double getQ(const std::string& name);
	/*!returns the status of the specified BPM.
	@param[in] name: the name of the BPM.
	@param[out] state: the status.*/
	STATE getStatus(const std::string& name);
	/*!returns the raw data vector of the specified BPM.
	@param[in] name: the name of the BPM.
	@param[out] value: the raw data vector.*/
	std::vector< double > getData(const std::string& name);
	/*!returns the resolution of the specified BPM.
	@param[in] name: the name of the BPM.
	@param[out] value: the resolution.*/
	double getResolution(const std::string& name);
	/*!returns the beamline position of the specified BPM.
	@param[in] name: the name of the BPM.
	@param[out] value: the position.*/
	double getPosition(const std::string& name);
	/*!recalibrates the attenuation of the specified BPM based on bunch charge.
	@param[in] name: the name of the BPM.
	@param[in] charge: the bunch charge.
	@param[out] bool: true if the procedure worked.*/
	bool reCalAttenuation(const std::string& name, const double& charge);
	/*!returns the X/Y position of the specified BPM.
	@param[in] name: the name of the BPM.
	@param[out] value: the X/Y position.*/
	std::pair<double, double> getXYPosition(const std::string& name);
	/*!returns a vector of X PV values of the specified BPM (after using monitorForNShots).
	@param[in] name: the name of the BPM.
	@param[out] values: the X PV vector.*/
	std::vector< double > getXPVVector(const std::string& name);
	/*!returns a vector of Y PV values of the specified BPM (after using monitorForNShots).
	@param[in] name: the name of the BPM.
	@param[out] values: the Y PV vector.*/
	std::vector< double > getYPVVector(const std::string& name);
	/*!returns a vector of charge values of the specified BPM (after using monitorForNShots).
	@param[in] name: the name of the BPM.
	@param[out] values: the charge vector.*/
	std::vector< double > getQVector(const std::string& name);
	/*!returns a vector of status values of the specified BPM (after using monitorForNShots).
	@param[in] name: the name of the BPM.
	@param[out] states: the status vector.*/
	std::vector< STATE > getStatusVector(const std::string& name);
	/*!returns a vector of raw data vectors of the specified BPM (after using monitorForNShots).
	@param[in] name: the name of the BPM.
	@param[out] values: the raw data vectors.*/
	std::vector< std::vector< double > > getDataVector(const std::string& name);
	/*!returns a vector of X/Y positions of the specified BPM (after using monitorForNShots).
	@param[in] name: the name of the BPM.
	@param[out] values: the X/Y position vectors.*/
	std::pair<std::vector< double >, std::vector< double > > getXYPositionVector(const std::string& name);
	/*!returns a buffer of X PV values of the specified BPM.
	@param[in] name: the name of the BPM.
	@param[out] values: the X PV buffer.*/
	boost::circular_buffer< double > getXPVBuffer(const std::string& name);
	/*!returns a buffer of Y PV values of the specified BPM.
	@param[in] name: the name of the BPM.
	@param[out] values: the Y PV buffer.*/
	boost::circular_buffer< double > getYPVBuffer(const std::string& name);
	/*!returns a buffer of charge values of the specified BPM.
	@param[in] name: the name of the BPM.
	@param[out] values: the charge buffer.*/
	boost::circular_buffer< double > getQBuffer(const std::string& name);
	/*!returns a buffer of status values of the specified BPM.
	@param[in] name: the name of the BPM.
	@param[out] values: the status buffer.*/
	boost::circular_buffer< STATE > getStatusBuffer(const std::string& name);
	/*!returns a buffer of raw data vectors of the specified BPM.
	@param[in] name: the name of the BPM.
	@param[out] values: the raw data vector buffer.*/
	boost::circular_buffer< std::vector< double > > getDataBuffer(const std::string& name);
	/*!sets the horizontal attenuation of the specified BPM.
	@param[in] name: the name of the BPM.
	@param[in] value: the desired SA1.
	@param[out] bool: true if it worked.*/
	bool setSA1(const std::string& name, const long& value);
	/*!sets the vertical attenuation of the specified BPM.
	@param[in] name: the name of the BPM.
	@param[in] value: the desired SA2.
	@param[out] bool: true if it worked.*/
	bool setSA2(const std::string& name, const long& value);
	/*!sets the horizontal delay of the specified BPM.
	@param[in] name: the name of the BPM.
	@param[in] value: the desired SD1.
	@param[out] bool: true if it worked.*/
	bool setSD1(const std::string& name, const long& value);
	/*!sets the vertical delay of the specified BPM.
	@param[in] name: the name of the BPM.
	@param[in] value: the desired SD2.
	@param[out] bool: true if it worked.*/
	bool setSD2(const std::string& name, const long& value);
	/*!returns the set horizontal attenuation of the specified BPM.
	@param[in] name: the name of the BPM.
	@param[out] value: the value of SA1.*/
	long getSA1(const std::string& name);
	/*!returns the set vertical attenuation of the specified BPM.
	@param[in] name: the name of the BPM.
	@param[out] value: the value of SA2.*/
	long getSA2(const std::string& name);
	/*!returns the set horizontal delay of the specified BPM.
	@param[in] name: the name of the BPM.
	@param[out] value: the value of SD1.*/
	long getSD1(const std::string& name);
	/*!returns the set vertical delay of the specified BPM.
	@param[in] name: the name of the BPM.
	@param[out] value: the value of SD2.*/
	long getSD2(const std::string& name);
	/*!returns the read horizontal attenuation of the specified BPM.
	@param[in] name: the name of the BPM.
	@param[out] value: the value of RA1.*/
	long getRA1(const std::string& name);
	/*!returns the read vertical attenuation of the specified BPM.
	@param[in] name: the name of the BPM.
	@param[out] value: the value of RA2.*/
	long getRA2(const std::string& name);
	/*!returns the read horizontal delay of the specified BPM.
	@param[in] name: the name of the BPM.
	@param[out] value: the value of RD1.*/
	long getRD1(const std::string& name);
	/*!returns the read vertical delay of the specified BPM.
	@param[in] name: the name of the BPM.
	@param[out] value: the value of RD2.*/
	long getRD2(const std::string& name);
	STATE getState(const std::string& name) const;
	/*!returns a Python list for the raw data vector of the specified BPM.
	@param[in] name: the name of the BPM.
	@param[out] list: the raw data list.*/
	boost::python::list getData_Py(const std::string& name);
	/*!returns a Python dict of X values for the specified BPMs.
	@param[in] names: the names of the BPMs.
	@param[out] dict: the X values, keyed by name.*/
	boost::python::dict getXs_Py(boost::python::list names);
	/*!returns a Python dict of X PV values for the specified BPMs.
	@param[in] names: the names of the BPMs.
	@param[out] dict: the X PV values, keyed by name.*/
	boost::python::dict getXsFromPV_Py(boost::python::list names);
	/*!returns a Python dict of Y values for the specified BPMs.
	@param[in] names: the names of the BPMs.
	@param[out] dict: the Y values, keyed by name.*/
	boost::python::dict getYs_Py(boost::python::list names);
	/*!returns a Python dict of Y PV values for the specified BPMs.
	@param[in] names: the names of the BPMs.
	@param[out] dict: the Y PV values, keyed by name.*/
	boost::python::dict getYsFromPV_Py(boost::python::list names);
	/*!returns a Python dict of charge values for the specified BPMs.
	@param[in] names: the names of the BPMs.
	@param[out] dict: the charge values, keyed by name.*/
	boost::python::dict getQs_Py(boost::python::list names);
	/*!returns a Python dict of status values for the specified BPMs.
	@param[in] names: the names of the BPMs.
	@param[out] dict: the status values, keyed by name.*/
	boost::python::dict getStatuses_Py(boost::python::list names);
	/*!returns a Python dict of raw data values for the specified BPMs.
	@param[in] names: the names of the BPMs.
	@param[out] dict: the raw data values, keyed by name.*/
	boost::python::dict getDatas_Py(boost::python::list names);
	/*!returns a Python dict of X/Y position values for the specified BPMs.
	@param[in] names: the names of the BPMs.
	@param[out] dict: the X/Y position values, keyed by name.*/
	boost::python::dict getXYPositions_Py(boost::python::list names);
	/*!returns a Python list of X PV values for the specified BPM (after using monitorForNShots).
	@param[in] name: the names of the BPM.
	@param[out] list: the X PV vector.*/
	boost::python::list getXPVVector_Py(const std::string& name);
	/*!returns a Python list of Y PV values for the specified BPM (after using monitorForNShots).
	@param[in] name: the names of the BPM.
	@param[out] list: the Y PV vector.*/
	boost::python::list getYPVVector_Py(const std::string& name);
	/*!returns a Python list of charge values for the specified BPM (after using monitorForNShots).
	@param[in] name: the names of the BPM.
	@param[out] list: the charge vector.*/
	boost::python::list getQVector_Py(const std::string& name);
	/*!returns a Python list of status values for the specified BPM (after using monitorForNShots).
	@param[in] name: the names of the BPM.
	@param[out] list: the status vector.*/
	boost::python::list getStatusVector_Py(const std::string& name);
	/*!returns a Python list of raw data vectors for the specified BPM (after using monitorForNShots).
	@param[in] name: the names of the BPM.
	@param[out] list: theraw data vectors.*/
	boost::python::list getDataVector_Py(const std::string& name);
	/*!returns a Python list of X PV values for the specified BPM.
	@param[in] name: the names of the BPM.
	@param[out] list: the X PV buffer.*/
	boost::python::list getXPVBuffer_Py(const std::string& name);
	/*!returns a Python list of Y PV values for the specified BPM.
	@param[in] name: the names of the BPM.
	@param[out] list: the Y PV buffer.*/
	boost::python::list getYPVBuffer_Py(const std::string& name);
	/*!returns a Python list of charge values for the specified BPM.
	@param[in] name: the names of the BPM.
	@param[out] list: the charge buffer.*/
	boost::python::list getQBuffer_Py(const std::string& name);
	/*!returns a Python list of status values for the specified BPM.
	@param[in] name: the names of the BPM.
	@param[out] list: the status buffer.*/
	boost::python::list getStatusBuffer_Py(const std::string& name);
	/*!returns a Python list of raw data values for the specified BPM.
	@param[in] name: the names of the BPM.
	@param[out] list: the raw data buffer.*/
	boost::python::list getDataBuffer_Py(const std::string& name);
	/*!returns a Python dict of X PV values for the specified BPMs (after using monitorForNShots).
	@param[in] names: the names of the BPMs.
	@param[out] dict: the X PV vectors, keyed by name.*/
	boost::python::dict getXPVVectors_Py(boost::python::list names);
	/*!returns a Python dict of Y PV values for the specified BPMs (after using monitorForNShots).
	@param[in] names: the names of the BPMs.
	@param[out] dict: the Y PV vectors, keyed by name.*/
	boost::python::dict getYPVVectors_Py(boost::python::list names);
	/*!returns a Python dict of charge values for the specified BPMs (after using monitorForNShots).
	@param[in] names: the names of the BPMs.
	@param[out] dict: the charge vectors, keyed by name.*/
	boost::python::dict getQVectors_Py(boost::python::list names);
	/*!returns a Python dict of status values for the specified BPMs (after using monitorForNShots).
	@param[in] names: the names of the BPMs.
	@param[out] dict: the status vectors, keyed by name.*/
	boost::python::dict getStatusVectors_Py(boost::python::list names);
	/*!returns a Python dict of raw data values for the specified BPMs (after using monitorForNShots).
	@param[in] names: the names of the BPMs.
	@param[out] dict: the raw data vectors, keyed by name.*/
	boost::python::dict getDataVectors_Py(boost::python::list names);
	/*!returns a Python dict of X/Y position values for the specified BPMs (after using monitorForNShots).
	@param[in] names: the names of the BPMs.
	@param[out] dict: the X/Y position vectors keyed by name.*/
	boost::python::dict getXYPositionVectors_Py(boost::python::list names);
	/*!returns a Python dict of X PV values for the specified BPMs.
	@param[in] names: the names of the BPMs.
	@param[out] dict: the X PV buffers, keyed by name.*/
	boost::python::dict getXPVBuffers_Py(boost::python::list names);
	/*!returns a Python dict of Y PV values for the specified BPMs.
	@param[in] names: the names of the BPMs.
	@param[out] dict: the Y PV buffers, keyed by name.*/
	boost::python::dict getYPVBuffers_Py(boost::python::list names);
	/*!returns a Python dict of charge values for the specified BPMs.
	@param[in] names: the names of the BPMs.
	@param[out] dict: the charge buffers, keyed by name.*/
	boost::python::dict getQBuffers_Py(boost::python::list names);
	/*!returns a Python dict of status values for the specified BPMs.
	@param[in] names: the names of the BPMs.
	@param[out] dict: the status buffers, keyed by name.*/
	boost::python::dict getStatusBuffers_Py(boost::python::list names);
	/*!returns a Python dict of raw data values for the specified BPMs.
	@param[in] names: the names of the BPMs.
	@param[out] dict: the raw data buffers, keyed by name.*/
	boost::python::dict getDataBuffers_Py(boost::python::list names);
	/*!returns a Python dict of resolutions of the specified BPMs.
	@param[in] names: the names of the BPMs.
	@param[out] dict: the resolutions, keyed by name.*/
	boost::python::dict getResolutions_Py(boost::python::list names);
	/*!returns a Python dict of beamline positions for the specified BPMs.
	@param[in] names: the names of the BPMs.
	@param[out] dict: the positions, keyed by name.*/
	boost::python::dict getPositions_Py(boost::python::list names);
	/*!recalibrates attenuations for the specified BPMs based on bunch charge.
	@param[in] names: the names of the BPMs.
	@param[in] charge: the bunch charge.
	@param[out] dict: a dict of bools, true if it worked, keyed by name.*/
	boost::python::dict reCalAttenuations_Py(boost::python::list names, const double& charge);
	/*!returns a Python dict of X values of all BPMs.
	@param[out] dict: the X values, keyed by name.*/
	boost::python::dict getAllX_Py();
	/*!returns a Python dict of X PV values of all BPMs.
	@param[out] dict: the X PV values, keyed by name.*/
	boost::python::dict getAllXFromPV_Py();
	/*!returns a Python dict of Y values of all BPMs.
	@param[out] dict: the Y values, keyed by name.*/
	boost::python::dict getAllY_Py();
	/*!returns a Python dict of Y values of all BPMs.
	@param[out] dict: the Y PV values, keyed by name.*/
	boost::python::dict getAllYFromPV_Py();
	/*!returns a Python dict of charge values of all BPMs.
	@param[out] dict: the charge values, keyed by name.*/
	boost::python::dict getAllQ_Py();
	/*!returns a Python dict of status values of all BPMs.
	@param[out] dict: the status values, keyed by name.*/
	boost::python::dict getAllStatus_Py();
	/*!returns a Python dict of raw data values of all BPMs.
	@param[out] dict: the raw data vectors, keyed by name.*/
	boost::python::dict getAllData_Py();
	/*!returns a Python dict of X/Y position values of all BPMs.
	@param[out] dict: the X/Y position values, keyed by name.*/
	boost::python::dict getAllXYPosition_Py();
	/*!returns a Python dict of X PV vectors of all BPMs (after using monitorForNShots).
	@param[out] dict: the X PV vectors, keyed by name.*/
	boost::python::dict getAllXPVVector_Py();
	/*!returns a Python dict of Y PV vectors of all BPMs (after using monitorForNShots).
	@param[out] dict: the Y PV vectors, keyed by name.*/
	boost::python::dict getAllYPVVector_Py();
	/*!returns a Python dict of charge vectors of all BPMs (after using monitorForNShots).
	@param[out] dict: the charge vectors, keyed by name.*/
	boost::python::dict getAllQVector_Py();
	/*!returns a Python dict of status vectors of all BPMs (after using monitorForNShots).
	@param[out] dict: the status vectors, keyed by name.*/
	boost::python::dict getAllStatusVector_Py();
	/*!returns a Python dict of raw data vectors of all BPMs (after using monitorForNShots).
	@param[out] dict: the raw data vectors, keyed by name.*/
	boost::python::dict getAllDataVector_Py();
	/*!returns a Python dict of X/Y position vectors of all BPMs (after using monitorForNShots).
	@param[out] dict: the X/Y position vectors, keyed by name.*/
	boost::python::dict getAllXYPositionVectors_Py();
	/*!returns a Python dict of X PV buffers of all BPMs.
	@param[out] dict: the X PV buffers, keyed by name.*/
	boost::python::dict getAllXPVBuffer_Py();
	/*!returns a Python dict of Y PV buffers of all BPMs.
	@param[out] dict: the Y PV buffers, keyed by name.*/
	boost::python::dict getAllYPVBuffer_Py();
	/*!returns a Python dict of charge buffers of all BPMs.
	@param[out] dict: the charge buffers, keyed by name.*/
	boost::python::dict getAllQBuffer_Py();
	/*!returns a Python dict of status buffers of all BPMs.
	@param[out] dict: the status buffers, keyed by name.*/
	boost::python::dict getAllStatusBuffer_Py();
	/*!returns a Python dict of raw data buffers of all BPMs.
	@param[out] dict: the raw data buffers, keyed by name.*/
	boost::python::dict getAllDataBuffer_Py();
	/*!returns a Python dict of resolution buffers of all BPMs.
	@param[out] dict: the resolution buffers, keyed by name.*/
	boost::python::dict getAllResolution_Py();
	/*!returns a Python dict of beamline position values of all BPMs.
	@param[out] dict: the beamline position values, keyed by name.*/
	boost::python::dict getAllPosition_Py();
	/*!returns a python list of all BPM names defined in the config file.
	@param[out] list: returns a list of BPM object names.*/
	boost::python::list getAllBPMNames_Py();
	/*!recalibrates the attenuations of all BPMs based on bunch charge.
	@param[in] charge: the bunch charge.
	@param[out] dict: true if it worked, keyed by name.*/
	boost::python::dict reCalAllAttenuation_Py(const double& charge);

	/*! turns debug messages on for BPMFactory and calls same function in all BPMs and configReader*/
	void debugMessagesOn();
	/*! turns debug messages off for BPMFactory and calls same function in all BPMs and configReader*/
	void debugMessagesOff();
	/*! turns messages on for BPMFactory and calls same function in all BPMs and configReader*/
	void messagesOn();
	/*! turns messages off for BPMFactory and calls same function in all BPMs and configReader*/
	void messagesOff();
	/*! returns true if messenger debug flag is true, false otherwise*/
	bool isDebugOn();
	/*! returns true if messenger messages flag is true, false otherwise*/
	bool isMessagingOn();
};
/** @}*/
#endif // BPM_FACTORY_H_
