#ifndef IMG_FACTORY_H_
#define IMG_FACTORY_H_

#include <LoggingSystem.h>
#include <ConfigReader.h>
#include <IMG.h>
#include <GlobalFunctions.h>
#include <GlobalStateEnums.h>
#include <map>
#include <vector>
#include <boost/python/dict.hpp>
#include <boost/python/list.hpp>

class IMGFactory
{
public:
	/*! Default constructor for IMGFactory.
		Calls the custom constructor with mode = OFFLINE.*/
	IMGFactory();
	/*! Custom constructor for IMGFactory
	@param[in] mode The type of IMGFactory we wish to create.
	PHYSICAL IMGFactory will perform operations on CLARA IMGs.
	VIRTUAL IMGFactory will perform operations of Virtual IMGs.
	OFFLINE IMGFactory will perform operations on Offline IMGs.*/
	IMGFactory(STATE mode);
	/*! Copy constructor for IMGFactory (used for = operator)
	@param[in] copyIMGFactory the IMGFactory we want to copy over.*/
	IMGFactory(const IMGFactory& copyFactory);
	/*! IMGFactory destructor.
	Goes through all Valve objects in the valveMap and removes connections/subscriptions to EPICS if they exist.*/
	~IMGFactory();
	/*! The kind of IMGFactory that is created (PHYSICAL, VIRTUAL, OFFLINE), this variable is passed to the
	IMG hardware objects when they are created by ConfigReader*/
	STATE mode;
	/*! Flag to say whether the ValveFactory setup function has been completed successfully.*/
	bool setup(std::string version);
	/*! passes an empty IMGMap to the configReader to be populated by IMG objects that are
		created from the YAML Config files.*/
	bool hasBeenSetup;
	/*! Map containing all Valves created from ConfigReader, keyed by harwdare name*/
	std::map<std::string, IMG> IMGMap;
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
	void populateIMGMap();
	/*! iterates over all IMGs in IMG map and uses their pvStructs vector to create
		connections to EPICS if mode is PHYSICAL or VIRTUAL*/
	void setupChannels();
	/*! sets the pvStruct monitor flag to true if we want to monitor the PV
		and otherwise this flag is set to false
		@param[in] pvStruct the pvStruct which we want to set the monitor flag of.
	*/
	void retrieveMonitorStatus(pvStruct& pvStruct);
	/*! returns IMG by name
	@param name checks the name in the alias map if not full name.
			and returns the Valve stored in IMGMap at that full name
	*/
	IMG& getIMG(const std::string& fullValveName);
	/*!
	retrieves the full name of the IMG object from the alias map for IMGs
	@param[in] nameToCheck :a known name of the valve
	@param[out] fullName : the full hardware name associated with the alias.
	*/
	std::string getFullName(const std::string& nameToCheck) const;
	/*!gets all of the hardware names associated with all of the valves in IMGMap
	 *@param[out] IMGNames :a vector containing all of the harwdare names for the valves in IMGMap*/
	std::vector<std::string> getAllIMGNames() const;
	/*! gets the IMG state of a particular IMG
	 * @param[in] name :The full-name or alias of the valve
	 * @param[out] pressure :The state the IMG is currently in (OPEN,CLOSED,ERR)*/
	double getIMGPressure(const std::string& name) const;
	/*! gets IMG Pressure for multiple IMGs
	* @param[in] names : a vector of full-names/aliases for IMGs
	* @param[out] pressure : a map of full IMGs names and their current pressures*/
	std::map<std::string, double> getIMGPressures(const std::vector<std::string>& names) const;
	boost::python::dict getIMGPressures_Py(const boost::python::list& names) const;
	/*! gets the pressures of all IMGs
	* @param[out] states : a map of all IMG names and their current pressures*/
	std::map<std::string, double> getAllIMGPressure() const;
	/*! gets the pressure of all IMGS
	* @param[out] states : a map of all IMGS names and their current pressures*/
	boost::python::dict getAllIMGPressure_Py() const;
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
private:
	void updateAliasNameMap(const IMG& img);
	std::map<std::string, std::string> alias_name_map;
	IMG dummyIMG;
	/*access IMG functionality without crashing out when we cannot connect to EPICS.*/
};

#endif // IMG_FACTORY_H_