#ifndef SHUTTER_FACTORY_H_
#define SHUTTER_FACTORY_H_

#include <ConfigReader.h>
#include <Shutter.h>
#include <GlobalStateEnums.h>
#include <vector>
class ShutterFactory
{
public:
	ShutterFactory();
	ShutterFactory(STATE mode);
	ShutterFactory(STATE mode, const std::string& primeLatticeLocation);
	ShutterFactory(const ShutterFactory& copyFactory);
	~ShutterFactory();
	bool hasBeenSetup;

	/*! Try to open a shutter.
	@param[in] string, name of shutter 
	@param[out] bool, true if command got sent to EPICS, (!!does not indicate whether the shutter opened succesfully!!)*/
	bool open(const std::string& name);
	/*! Try to close the shutter.
	@param[out] bool, true if command got sent to EPICS, (!!does not indicate whether the shutter closed succesfully!!)*/
	bool close(const std::string& name);

	/*! checks whether the current shutter_state is STATE::OPEN.
	@param[in] string, name of shutter 
	@param[out] bool true if shutter_state is STATE::OPEN, false if shutter_state is not STATE::OPEN */
	bool isOpen(const std::string& name);
	/*! checks whether the current shutter_state is STATE::CLOSED.
	@param[in] string, name of shutter 
	@param[out] bool true if shutter_state is STATE::CLOSED, false if shutter_state is not STATE::CLOSED */
	bool isClosed(const std::string& name);

	/*! Get the state of a shutter.
	@param[in] string, name of shutter 
	@param[out] STATE, one of OPEN, CLOSED, ERR or UNKNOWN*/
	STATE getState(const std::string& name)const;



	/*! Get the cmi values (integer which interlocks are based off) .
	@param[in] string, name of shutter
	@param[out] int, cmi value*/
	int getCMI(const std::string& name)const;


	/*! Get the interlock state map for each interlock in the shutter 
	@param[in] name, shutter to get interlock state 
	@param[out] map<string, STATE>, interlock STATES keyed by theinterlock value, (defeind in master lattice yaml file */
	std::map<std::string, STATE> getCMIBitMap(const std::string& name)const;
	
	/*! Get the interlock state map for each interlock in the shutter
	@param[in] name, shutter to get interlock state
	@param[out] dict, interlock STATES keyed by the interlock value, (defeind in master lattice yaml file */
	boost::python::dict getCMIBitMap_Py(const std::string& name)const;
		
	/*!gets all of the names associated with all of the shutters in shutterMap
	 *@param[out] vector<string>, a vector containing all of the shutter names*/
	std::vector<std::string> getAllShutterNames() const;
	/*!gets all of the names associated with all of the shutters in shutterMap
	 *@param[out] vector<string>, a vector containing all of the shutter names*/
	boost::python::list getAllShutterNames_Py() const;


	/*! Get the full name for a Shutter object 
	@param[in] name to check for its full name 
	@param[out] string, full name associated with passed name */
	std::string getFullName(const std::string& name_to_check) const;
	
	
	/*! Get a Shutter object
	@param[in] name of shutter object to return 
	@param[out] Shutter, shutter objecr  */
	Shutter& getShutter(const std::string& shutter_name);

	/*! Get the alias names for a Shutter 
	@param[in] string shutter name, name of shutter to get alias names for
	@param[out] vector<string> alias names  */
	std::vector<std::string> getAliases(const std::string& name) const;

	/*! Get the ShutterState for each Shutter in the factory 
	@param[out] map<string, ShutterState>, map of ShutterState objects, keyed by the Shutter name*/
	std::map<std::string, ShutterState> getShutterStates()const;
	/*! Get the ShutterState for each Shutter in the factory, Python version 
	@param[out] dict, dict of ShutterState objects, keyed by the Shutter name*/
	boost::python::dict getShutterStates_Py()const;

	/*! Get the ShutterState
	@param[in] string, name of shutter
	@param[out] ShutterState, ShutterState structured data */
	ShutterState getShutterState(const std::string& name)const;

	///*! Get the ShutterState, Python version 
	//@param[in] string, name of shutter
	//@param[out] ShutterState, ShutterState structured data */
	//ShutterState getShutterState_Py(const std::string& shutter_name)const;

	/*! Get the ShutterState data in a python dictionary
	@param[in] string, name of shutter 
	@param[out] dict, dictionary of shutter state data, keyed by the name of each data type */
	boost::python::dict getShutterStateDictionary(const std::string& name)const;

	/*! Get the ShutterState data for all shautters in a python dictionary
	@param[in] string, name of shutter
	@param[out] dict, dictionary of shutter state dictionaries (!), keyed by the name of each shutter */
	boost::python::dict getShutterStateDictionaries()const;


	/*! Compare the energy interlock to STATE::GOOD (energy interlock name is hardcoded, and should match master lattice name.)
		@param[in] string, name of shutter to compare
		@param[out] bool, true if state is GOOD, otherwise false  */
	bool isEnergyInterlockGood(const std::string& name)const;
	/*! Compare the energy interlock to STATE::BAD (energy interlock name is hardcoded, and should match master lattice name.)
		@param[in] string, name of shutter to compare
		@param[out] bool, true if state is BAD, otherwise false (including if passed name is not recognized)  */
	bool isEnergyInterlockBad(const std::string& name)const;

	/*! Compare the charge interlock to STATE::BAD (charge interlock name is hardcoded, and should match master lattice name.)
		@param[in] string, name of shutter to compare
		@param[out] bool, true if state is GOOD, otherwise false (including if passed name is not recognized)   */
	bool isChargeInterlockGood(const std::string& name)const;
	/*! Compare the charge interlock to STATE::BAD (charge interlock name is hardcoded, and should match master lattice name.)
		@param[in] string, name of shutter to compare
		@param[out] bool, true if state is BAD, otherwise false (including if passed name is not recognized)  */
	bool isChargeInterlockBad(const std::string& name)const;

	/*! Compare both the PS GLA, GLB interlocks to STATE::GOOD (names are hardcoded, and should match master lattice name.)
		@param[in] string, name of shutter to compare
		@param[out] bool, true if both states are GOOD, otherwise false (including if passed name is not recognized)  */
	bool isPSInterlockGood(const std::string& name)const;
	/*! Compare both the PS GLA, GLB interlocks to STATE::BAD (names are hardcoded, and should match master lattice name.)
		@param[in] string, name of shutter to compare
		@param[out] bool, true if either states are BAD, otherwise false (including if passed name is not recognized)  */
	bool isPSInterlockBad(const std::string& name)const;



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




	bool setup(const std::string version);
	LoggingSystem messenger;

	friend class HardwareFactory;
private:

	void populateShutterMap();
	/*! iterates over all valves in valve map and uses their pvStructs vector to create
		connections to EPICS if mode is PHYSICAL or VIRTUAL*/
	void setupChannels();


	void updateAliasNameMap(const Shutter& shutter);
	std::map<std::string, std::string> alias_name_map;

	STATE mode;

	// dummy_magnet is used to return values when users ask for a nOn-existing magnet's properties 
	Shutter dummy_shutter;

	std::map<std::string, Shutter> shutterMap;

	void setMonitorStatus(pvStruct& pvStruct);

	// private
	ConfigReader reader;

};

#endif // SHUTTER_FACTORY_H_