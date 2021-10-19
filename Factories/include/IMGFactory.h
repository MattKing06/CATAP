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
/** @addtogroup factories
 @{*/
 /*! A class to store, setup, and perform operations on multiple IMG objects*/

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
	IMGFactory(STATE mode, const std::string& primeLatticeLocation);
	/*! Copy constructor for IMGFactory (used for = operator)
	@param[in] copyIMGFactory the IMGFactory we want to copy over.*/
	IMGFactory(const IMGFactory& copyFactory);
	/*! IMGFactory destructor.
	Goes through all IMG objects in the imgMap and removes connections/subscriptions to EPICS if they exist.*/
	~IMGFactory();
	
	/*! The kind of IMGFactory that is created (PHYSICAL, VIRTUAL, OFFLINE), this variable is passed to the
	IMG hardware objects when they are created by ConfigReader*/
	STATE mode;
	
	/*! Flag to say whether the IMGFactory setup function has been completed successfully.*/
	bool hasBeenSetup;
	
	/*! Map containing all IMGs created from ConfigReader, keyed by harwdare name*/
	std::map<std::string, IMG> IMGMap;
	
	/*! ConfigReader to parse YAML config files and create associated IMG objects*/
	ConfigReader reader;
	
	/*! Used to print messages both normal and debug, also sets the On/Off flags for debug and normal messages*/
	LoggingSystem messenger;
	
	/*! The main function that sets up EPICS connections and subscriptions to all
	IMGs in the IMG map. We set up all channels before sending them to EPICS
	for efficiency. After channels, the remaining data for pvStructs are set as well
	as any subscriptions that are needed for monitoring.
	@param[in] version NOT IN USE
	@param[out] hasBeenSetup false if no YAML config files to parse, true otherwise.*/
	bool setup(const std::string& version);
	
	/*! passes an empty IMGMap to the configReader to be populated by IMG objects that are
		created from the YAML Config files.*/
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
			and returns the IMGs stored in IMGMap at that full name
	*/
	IMG& getIMG(const std::string& fullIMGName);
	
	/*!
	retrieves the full name of the IMG object from the alias map for IMGs
	@param[in] nameToCheck :a known name of the IMG
	@param[out] fullName : the full hardware name associated with the alias.
	*/
	std::string getFullName(const std::string& nameToCheck) const;
	
	/*!gets all of the hardware names associated with all of the IMGs in IMGMap
	 *@param[out] IMGNames :a vector containing all of the hardware names for the IMGs in IMGMap*/
	std::vector<std::string> getAllIMGNames() const;
	
	/*! gets all IMG names as python list*/
	boost::python::list getAllIMGNames_Py()const;
	
	/*! gets the IMG state of a particular IMG
	 * @param[in] name :The full-name or alias of the IMG
	 * @param[out] pressure :The state the IMG is currently in (OK, ERR)*/
	double getIMGPressure(const std::string& name) const;
	
	/*! gets IMG Pressure for multiple IMGs
	* @param[in] names : a vector of full-names/aliases for IMGs
	* @param[out] pressure : a map of full IMGs names and their current pressures*/
	std::map<std::string, double> getIMGPressures(const std::vector<std::string>& names) const;
	
	/*! gets all IMG Pressures for multiple IMGs as python dictionary*/
	boost::python::dict getIMGPressures_Py(const boost::python::list& names) const;
	
	/*! gets the pressures of all IMGs
	* @param[out] states : a map of all IMG names and their current pressures*/
	std::map<std::string, double> getAllIMGPressure() const;
	
	/*! gets all IMG Pressures for all IMGs as python dictionary*/
	boost::python::dict getAllIMGPressure_Py() const;
	
	/*! gets the IMG state (OK,ERR) of a particular IMG
	 * @param[in] name :The full-name or alias of the IMG
	 * @param[out] IMGState :The state the IMG is currently in (OK,ERR)*/
	STATE getIMGState(const std::string& name) const;
	
	/*! gets all IMG states for multiple IMGs as python dictionary*/
	boost::python::dict getIMGStates_Py(const boost::python::list& names) const;
	
	/*! gets IMG states (OK, ERR) for multiple IMGs
	* @param[in] names : a vector of full-names/aliases for IMGs
	* @param[out] states : a map of full IMG names and their current states*/
	std::map<std::string, STATE> getIMGStates(const std::vector<std::string>& names) const;
	
	/*! gets all IMG states for all the IMGs as python dictionary*/
	boost::python::dict getAllIMGStates_Py() const;
	
	/*! gets the states of all IMGs
	* @param[out] states : a map of all IMG names and their current states*/
	std::map<std::string, STATE> getAllIMGStates() const;	
	
	/*! turns debug messages on for IMGFactory and calls same function in all IMGs and configReader*/
	void debugMessagesOn();
	
	/*! turns debug messages off for IMGFactory and calls same function in all IMGs and configReader*/
	void debugMessagesOff();
	
	/*! turns messages on for IMGFactory and calls same function in all IMGs and configReader*/
	void messagesOn();
	
	/*! turns messages off for IMGFactory and calls same function in all IMGs and configReader*/
	void messagesOff();
	
	/*! returns true if messenger debug flag is true, false otherwise*/
	bool isDebugOn();
	
	/*! returns true if messenger messages flag is true, false otherwise*/
	bool isMessagingOn();
private:
	void updateAliasNameMap(const IMG& img);
	std::map<std::string, std::string> alias_name_map;
	
	/*access IMG functionality without crashing out when we cannot connect to EPICS.*/
	IMG dummyIMG;

};

/** @}*/
#endif // IMG_FACTORY_H_
