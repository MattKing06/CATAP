#ifndef CHARGE_FACTORY_H_
#define CHARGE_FACTORY_H_

#include "LoggingSystem.h"
#include "ConfigReader.h"
#pragma once
#include "Charge.h"
#include <vector>
#include <map>
#include <utility>
#include <boost/python.hpp>
#include <boost/circular_buffer.hpp>

typedef void(*updateFunctionPtr)(struct event_handler_args args);

/** @addtogroup factories
 @{*/
 /*! A class to store, setup, and perform operations on multiple Charge objects*/
class Charge;
class ChargeFactory
{
public:
	/*! Default constructor for ChargeFactory.
	Calls the custom constructor with mode = OFFLINE.*/
	ChargeFactory();
	/*! Custom constructor for ChargeFactory
	@param[in] mode The type of ChargeFactory we wish to create.
	PHYSICAL ChargeFactory will perform operations on CLARA/VELA charge diagnostics.
	VIRTUAL ChargeFactory will perform operations of Virtual charge diagnostics.
	OFFLINE ChargeFactory will perform operations on Offline charge diagnostics.*/
	ChargeFactory(STATE mode);
	/*! Copy constructor for ChargeFactory (used for = operator)
	@param[in] copyChargeFactory: the ChargeFactory we want to copy over.*/
	ChargeFactory(const ChargeFactory& copyChargeFactory);
	/*! ChargeFactory destructor.
	Goes through all Charge objects in the screenMap and removes connections/subscriptions to EPICS if they exist.*/
	~ChargeFactory();
	/*! The main function that sets up EPICS connections and subscriptions to all
		screens in the Charge map. We set up all channels before sending them to EPICS
		for efficiency. After channels, the remaining data for pvStructs are set as well
		as any subscriptions that are needed for monitoring.
		@param[in] version NOT IN USE
		@param[out] hasBeenSetup false if no YAML config files to parse, true otherwise.*/
	bool setup(const std::string &VERSION);
	/*! Used to print messages both normal and debug, also sets the On/Off flags for debug and normal messages*/
	LoggingSystem messenger;
	/*! ConfigReader to parse YAML config files and create associated Charge objects*/
	ConfigReader reader;
	/*! Map containing all Charge hardware objects created from ConfigReader, keyed by harwdare name*/
	std::map<std::string, Charge> chargeMap;
	/*! Flag to say whether the ChargeFactory setup function has been completed successfully.*/
	bool hasBeenSetup;
	/*! passes an empty chargeMap to the configReader to be populated by Charge objects that are
	created from the YAML Config files.*/
	void populateChargeMap();
	/*! sets the pvStruct monitor flag to true if we want to monitor the PV
		and otherwise this flag is set to false
		@param[in] pvStruct the pvStruct which we want to set the monitor flag of.
	*/
	void retrievemonitorStatus(pvStruct& pvStruct);
	/*! iterates over all BPMs in chargeMap and uses their pvStructs vector to create
		connections to EPICS if mode is PHYSICAL or VIRTUAL*/
	void setupChannels();
	/*! returns Charge hardware object by name
	@param[in] name: checks the name in the alias map if not full name.
	and returns the Charge hardware stored in chargeMap at that those name
	*/
	Charge& getChargeDiagnostic(const std::string& name);
	/*! returns Charge hardware objects by name
	@param[in] names: checks the names in the alias map if not full name.
	@param[out] returns the Charge hardware objects stored in chargeMap at those full name
	*/
	std::map<std::string, Charge> getChargeDiagnostics(std::vector<std::string> chargeNames);
	/*! returns all Charge hardware objects
	@param[out] returns all Charge hardware objects stored in chargeMap
	*/
	std::map<std::string, Charge> getAllChargeDiagnostics();
	/*!gets all the full name of a certain Charge object.
	@param[in] name: the name of the charge object.
	@param[out] name : the name of the charge object. Not sure why this function is here.*/
	std::string getChargeDiagnosticName(const std::string& name);

	/*! gets the names for all known charge diagnostics
	@param[out] name list: The names of all charge objects*/
	boost::python::list getAllChargeDiagnosticNames_Py();

	/*!sets the buffer size of the given charge object.
	@param[in] name: sets the size of charge buffers.
	@param[in] value: sets the size of charge buffers.*/
	void setBufferSize(const std::string& name, const size_t& value);
	/*!sets the buffer size of all charge objects
	@param[in] value: sets the size of charge buffers.*/
	void setAllBufferSize(const size_t& value);
	/*!records charge in a vector (accessible directly through the Charge hardware object, or the getQVector functions).
	@param[in] name: the name of the charge object.
	@param[in] value: the number of shots to record.*/
	void monitorForNShots(const std::string& name, const size_t& value);
	/*!records charge in vectors for multiple objects (accessible directly through the Charge hardware objects, or the getQVector functions).
	@param[in] names: the names of the charge objects.
	@param[in] value: the number of shots to record.*/
	void monitorMultipleForNShots(const std::vector< std::string >& names, const size_t& value);
	/*!records charge in vectors for all objects (accessible directly through the Charge hardware objects, or the getQVector functions).
	@param[in] value: the number of shots to record.*/
	void monitorAllForNShots(const size_t& value);
		
	STATE mode;
	/*!is charge being monitored?
	@param[in] name: the Charge object name.
	@param[out] bool: yes if true.*/
	bool ismonitoringQ(const std::string& name);
	/*!is charge being monitored?
	@param[in] name: the Charge object name.
	@param[out] bool: yes if true.*/
	bool ismonitoring(const std::string& name);
	/*!get charge from multiple Charge objects
	@param[in] names: the Charge object names.
	@param[out] values: vector of charge values.*/
	std::map<std::string, double> getQs(const std::vector<std::string>& names);
	/*!get beamline position of multiple Charge diagnostic devices
	@param[in] names: the Charge object names.
	@param[out] values: vector of positions.*/
	std::map<std::string, double> getPositions(const std::vector<std::string>& names);
	/*!get charge from all Charge objects
	@param[out] values: vector of charge values.*/
	std::map<std::string, double> getAllQ();
	/*!get beamline position of all Charge diagnostic devices
	@param[out] values: vector of positions.*/
	std::map<std::string, double> getAllPosition();
	/*!get vector of charge values for all devices (after using monitorForNShots)
	@param[out] values: map of vectors of charge values, keyed by name.*/
	std::map<std::string, std::vector< double > > getAllQVector();
	/*!get vector of charge values for multiple devices (after using monitorForNShots)
	@param[out] names: the names of the Charge objects.
	@param[out] values: map of vectors of charge values, keyed by name.*/
	std::map < std::string, std::vector< double > > getQVectors(const std::vector<std::string> & names);
	/*!get buffer of charge values for multiple devices
	@param[in]: names: the names of the Charge objects
	@param[out] values: map of buffers of charge values, keyed by name.*/
	std::map < std::string, boost::circular_buffer< double > > getQBuffers(const std::vector<std::string> & names);
	/*!get buffer of charge values for all devices
	@param[out] values: map of buffers of charge values, keyed by name.*/
	std::map < std::string, boost::circular_buffer< double > > getAllQBuffer();
	/*!get the charge value
	@param[in] name: the name of the Charge object.
	@param[out] value: bunch charge.*/
	double getQ(const std::string& name);
	/*!get the beamline position of the charge diagnostic
	@param[in] name: the name of the Charge object.
	@param[out] position: beamline position.*/
	double getPosition(const std::string& name);
	/*!get the vector of charge values (after using monitorForNShots)
	@param[in] name: the name of the Charge object.
	@param[out] values: bunch charge vector.*/
	std::vector< double > getQVector(const std::string& name);
	/*!get the buffer vecto of charge values
	@param[in] name: the name of the Charge object.
	@param[out] values: bunch charge buffer.*/
	boost::circular_buffer< double > getQBuffer(const std::string& name);
	/*fills up vectors of charge values for the Charge objects specified in the python list
	@param[in] names: python list of names of the Charge objects.
	@param[in] value: number of shots to monitor.*/
	void monitorForNShots_Py(boost::python::list names, const size_t& value);
	/*returns a Python dict of charge reading at multiple devices
	@param[in] names: python list of names of the Charge objects.
	@param[out] dict: charge values, keyed by name.*/
	boost::python::dict getQs_Py(boost::python::list names);
	/*returns a Python list of the vector of charge values (after using monitorForNShots)
	@param[in] name: the name of Charge object.
	@param[out] list: charge vector.*/
	boost::python::list getQVector_Py(const std::string& name);
	/*returns a Python list of the charge buffer
	@param[in] name: the name of Charge object.
	@param[out] list: charge buffer.*/
	boost::python::list getQBuffer_Py(const std::string& name);
	/*returns a Python dict of charge values (after using monitorForNShots)
	@param[in] names: the names of Charge objects.
	@param[out] dict: charge vectors, keyed by name.*/
	boost::python::dict getQVectors_Py(boost::python::list names);
	/*returns a Python dict of beamline positions of charge diagnostic devices
	@param[in] names: the names of Charge objects.
	@param[out] dict: positions, keyed by name.*/
	boost::python::dict getPositions_Py(boost::python::list names);
	/*returns a Python dict of buffers of charge values
	@param[out] dict: charge buffers, keyed by name.*/
	boost::python::dict getQBuffers_Py(boost::python::list names);
	/*returns a Python dict of buffers of charge values
	@param[out] dict: charge buffers, keyed by name.*/
	boost::python::dict getAllQBuffer_Py();
	/*returns a Python dict of charge values
	@param[out] dict: charge values, keyed by name.*/
	boost::python::dict getAllQ_Py();
	/*returns a Python dict of vectors of charge values (after using monitorForNShots)
	@param[out] dict: charge vectors, keyed by name.*/
	boost::python::dict getAllQVector_Py();
	/*returns a Python dict of beamline positions of charge diagnostic devices
	@param[out] dict: positions, keyed by name.*/
	boost::python::dict getAllPosition_Py();

	/*! turns debug messages on for ChargeFactory and calls same function in all charge diagnostics and configReader*/
	void debugMessagesOn();
	/*! turns debug messages off for ChargeFactory and calls same function in all charge diagnostics and configReader*/
	void debugMessagesOff();
	/*! turns messages on for ChargeFactory and calls same function in all charge diagnostics and configReader*/
	void messagesOn();
	/*! turns messages off for ChargeFactory and calls same function in all charge diagnostics and configReader*/
	void messagesOff();
	/*! returns true if messenger debug flag is true, false otherwise*/
	bool isDebugOn();
	/*! returns true if messenger messages flag is true, false otherwise*/
	bool isMessagingOn();
};
/** @}*/
#endif // CHARGE_FACTORY_H_