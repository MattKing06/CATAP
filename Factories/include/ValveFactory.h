#ifndef VALVE_FACTORY_H_
#define VALVE_FACTORY_H_


#include <LoggingSystem.h>
#include <ConfigReader.h>
#include <Valve.h>
#include <map>
#include <vector>
#include <boost/python/dict.hpp>
#include <boost/python/list.hpp>
#include <boost/python/numpy.hpp>
#include <SnapshotFileManager.h>

/** @addtogroup factories 
 @{*/
/*! A class to store, setup, and perform operations on multiple Valve objects*/
class ValveFactory
{
public:
	/*! Default constructor for ValveFactory. 
		Calls the custom constructor with mode = OFFLINE.*/
	ValveFactory();
	/*! Custom constructor for ValveFactory
	@param[in] mode The type of ValveFactory we wish to create. 
	PHYSICAL ValveFactory will perform operations on CLARA Vacuum Valves.
	VIRTUAL ValveFactory will perform operations of Virtual Vacuum Valves.
	OFFLINE ValveFactory will perform operations on Offline Valves.*/
	ValveFactory(STATE mode);

	ValveFactory(STATE mode, const std::string& primeLatticeLocation);
	/*! Copy constructor for ValveFactory (used for = operator)
	@param[in] copyValveFactory the ValveFactory we want to copy over.*/
	ValveFactory(const ValveFactory& copyValveFactory);
	/*! ValveFactory destructor.
	Goes through all Valve objects in the valveMap and removes connections/subscriptions to EPICS if they exist.*/
	~ValveFactory();
	/*! Attach to the initial CA context for multi-threaded applications for a given valve
	@param[in] valveName: Name of the valve to attach to CA Context*/
	void attachContext(const std::string& valveName);
	/*! Attach to the initial CA context for multi-threaded applications for given valves
	@param[in] valveNames: Names of the valves to attach to CA Context*/
	void attachContext(std::vector<std::string>& valveNames);
	/*! Attach to the initial CA context for multi-threaded applications for given valves
	@param[in] valveNames: List of names of the valves to attach to CA Context*/
	void attachContext_Py(boost::python::list valveNames);
	/*! Attach to the initial CA context for multi-threaded applications for all valves*/
	void attachContext();
	/*! The kind of ValveFactory that is created (PHYSICAL, VIRTUAL, OFFLINE), this variable is passed to the
	Valve hardware objects when they are created by ConfigReader*/
	STATE mode;
	/*! Flag to say whether the ValveFactory setup function has been completed successfully.*/
	bool hasBeenSetup;
	/*! Map containing all Valves created from ConfigReader, keyed by harwdare name*/
	std::map<std::string, Valve> valveMap;
	/*! ConfigReader to parse YAML config files and create associated Valve objects*/
	ConfigReader reader;
	/*! Used to print messages both normal and debug, also sets the On/Off flags for debug and normal messages*/
	LoggingSystem messenger;

	/*! The main function that sets up EPICS connections and subscriptions to all
	    valves in the Valve map. We set up all channels before sending them to EPICS
		for efficiency. After channels, the remaining data for pvStructs are set as well
		as any subscriptions that are needed for monitoring.
		@param[in] version NOT IN USE
		@param[out] hasBeenSetup false if no YAML config files to parse, true otherwise.*/
	bool setup(const std::string& VERSION);
	/*! passes an empty valveMap to the configReader to be populated by Valve objects that are 
		created from the YAML Config files.*/
	void populateValveMap();
	/*! iterates over all valves in valve map and uses their pvStructs vector to create
		connections to EPICS if mode is PHYSICAL or VIRTUAL*/
	void setupChannels();
	/*! sets the pvStruct monitor flag to true if we want to monitor the PV
	    and otherwise this flag is set to false
		@param[in] pvStruct the pvStruct which we want to set the monitor flag of.
	*/
	void retrieveMonitorStatus(pvStruct& pvStruct) const;
	/*! returns Valve by name
	@param name checks the name in the alias map if not full name.
			and returns the Valve stored in ValveMap at that full name
	*/
	Valve& getValve(const std::string& fullValveName);
	/*! 
	retrieves the full name of the Valve object from the alias map for Valves
	@param[in] nameToCheck :a known name of the valve
	@param[out] fullName : the full hardware name associated with the alias.
	*/
	std::string getFullName(const std::string& nameToCheck) const;
	/*!gets all of the hardware names associated with all of the valves in valveMap
	 *@param[out] valveNames :a vector containing all of the harwdare names for the valves in valveMap*/
	std::vector<std::string> getAllValveNames() const;
	/*! gets the valve state (OPEN,CLOSED,ERR) of a particular valve
	 * @param[in] name :The full-name or alias of the valve
	 * @param[out] valveState :The state the valve is currently in (OPEN,CLOSED,ERR)*/
	STATE getValveState(const std::string& name) const;
	/*! gets valve states (OPEN,CLOSED,ERROR) for multiple valves
	* @param[in] names : a vector of full-names/aliases for valves
	* @param[out] states : a map of full valve names and their current states*/
	std::map<std::string, STATE> getValveStates(const std::vector<std::string>& names) const;
	/*! gets the states of all valves
	* @param[out] states : a map of all valve names and their current states*/
	std::map<std::string, STATE> getAllValveStates() const;
	/*! used by open and close functions to set the valve state of a particular valve.
	* depending on the mode of the ValveFactory, this will either send the state change to EPICS
	* (PHSYICAL, VIRTUAL) or just set the valveState for that valve (OFFLINE)
	* @param[in] name : the name of the valve we want to change
	* @param[in] state : the state we want to set the valve to (OPEN,CLOSED)*/
	void setValveState(const std::string& name, const STATE& state);
	/*! sets the state of the valve to closed 
	 *(if PHYSICAL or VIRTUAL this request is sent to EPICS, otherwise it just sets the valveState to CLOSED)
	 * @param[in] name : The name or alias of the valve we want to close.
	 */
	void close(const std::string& name);
	/*! sets the state of multiple valves to closed
	 *(if PHYSICAL or VIRTUAL this request is sent to EPICS, otherwise it just sets the valveState to CLOSED)
	 * @param[in] name : The names or aliases of the valves we want to close.
	 */
	void close(const std::vector<std::string>& names);
	/*! sets the state of all valves to closed
	 *(if PHYSICAL or VIRTUAL this request is sent to EPICS, otherwise it just sets the valveState to CLOSED)
	 */
	void closeAllValves();
	/*! sets the state of the valve to open
	 *(if PHYSICAL or VIRTUAL this request is sent to EPICS, otherwise it just sets the valveState to OPEN)
	 * @param[in] name : The name or alias of the valve we want to open.
	 */
	void open(const std::string& name);
	/*! sets the state of multiple valves to open
	 *(if PHYSICAL or VIRTUAL this request is sent to EPICS, otherwise it just sets the valveState to OPEN)
	 * @param[in] name : The names or aliases of the valves we want to open.
	 */
	void open(const std::vector<std::string>& names);
	/*! sets the state of all valves to open
	 *(if PHYSICAL or VIRTUAL this request is sent to EPICS, otherwise it just sets the valveState to OPEN)
	 */
	void openAllValves();
	/*! checks if the valve is open, returns true if valveState is OPEN, false otherwise
	* @param[in] name : the name or alias of the valve we want to check
	* @param[out] bool : true if valveState is OPEN, false otherwise*/
	bool isOpen(std::string name) const;
	bool isMoving(std::string name) const;
	/*! checks if the valve is closed, returns true if valveState is CLOSED, false otherwise
     * @param[in] name : the name or alias of the valve we want to check
     * @param[out] bool : true if valveState is CLOSED, false otherwise*/
	bool isClosed(std::string name) const;


	/*! turns debug messages on for valveFactory and calls same function in all valves and configReader*/
	void debugMessagesOn();
	/*! turns debug messages off for valveFactory and calls same function in all valves and configReader*/
	void debugMessagesOff();
	/*! turns messages on for valveFactory and calls same function in all valves and configReader*/
	void messagesOn();
	/*! turns messages off for valveFactory and calls same function in all valves and configReader*/
	void messagesOff();
	/*! returns true if messenger debug flag is true, false otherwise*/
	bool isDebugOn();
	/*! returns true if messenger messages flag is true, false otherwise*/
	bool isMessagingOn();

	std::map<std::string, HardwareSnapshot> getSnapshot();
	boost::python::dict getSnapshot_Py();
	bool exportSnapshotToYAML(const std::string& location, const std::string& filename);
	bool loadSnapshot(const std::string& location);
	bool loadSnapshot(const YAML::Node& settings);
	bool loadSnapshot_Py(const boost::python::dict& settings);
	/*! gets all valve names as python list*/
	boost::python::list getAllValveNames_Py() const;
	/*! gets valve states by name from the python list parameter, and returns a dict with key,value pairs of valve-name and state*/
	boost::python::dict getValveStates_Py(const boost::python::list& names) const;
	/*! returns a python dict with key,value pairs of valve-name and state for all valves in the valveMap*/
	boost::python::dict getAllValveStates_Py() const;
	/*! closes multiple valves using python list of valve names*/
	void close_Py(const boost::python::list& names);
	/*! opens multiple valves using python list of valve names*/
	void open_Py(const boost::python::list& names);

	boost::python::numpy::ndarray getFakeNumpyArray();

private:
	/*! function to update map of aliases to full-names of valves
	* @param[in] valve : The Valve object we want to update the alias map with.*/
	void updateAliasNameMap(const Valve& valve);
	/*! a map that provides the link between full names and associated aliases.*/
	std::map<std::string, std::string> aliasNameMap;
	/*! A valve to be returned if we cannot access a particular Valve, provides a way to 
	access Valve functionality without crashing out when we cannot connect to EPICS.*/
	Valve dummyValve;

};
/** @}*/
#endif //VALVE_FACTORY_H_