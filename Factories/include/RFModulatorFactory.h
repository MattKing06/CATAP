#ifndef RF_MODULATOR_FACTORY_H_
#define RF_MODULATOR_FACTORY_H_
#include <ConfigReader.h>
#include <RFModulator.h>
#include <GlobalStateEnums.h>
#include <vector>

class RFModulatorFactory
{
public:
	RFModulatorFactory();
	RFModulatorFactory(STATE mode);
	RFModulatorFactory(STATE mode, const std::string& primeLatticeLocation);
	RFModulatorFactory(const RFModulatorFactory& copyFactory);
	~RFModulatorFactory();
	/*! Attach to the initial CA context for multi-threaded applications for a given RFModulator
	@param[in] RFModulatorName: Name of the RFModulator to attach to CA Context*/
	void attachContext(const std::string& RFModulatorName);
	/*! Attach to the initial CA context for multi-threaded applications for given RFModulators
	@param[in] RFModulatorNames: Names of the RFModulators to attach to CA Context*/
	void attachContext(std::vector<std::string>& RFModulatorNames);
	/*! Attach to the initial CA context for multi-threaded applications for given RFModulators
	@param[in] RFModulatorNames: List of names of the RFModulators to attach to CA Context*/
	void attachContext_Py(boost::python::list RFModulatorNames);
	/*! Attach to the initial CA context for multi-threaded applications for all RFModulators*/
	void attachContext();

	/*! detach to the initial CA context for multi-threaded applications for a given RFModulator
	@param[in] RFModulatorName: Name of the RFModulator to detach to CA Context*/
	void detachContext(const std::string& RFModulatorName);
	/*! detach to the initial CA context for multi-threaded applications for given RFModulators
	@param[in] RFModulatorNames: Names of the RFModulators to detach to CA Context*/
	void detachContext(std::vector<std::string>& RFModulatorNames);
	/*! detach to the initial CA context for multi-threaded applications for given RFModulators
	@param[in] RFModulatorNames: List of names of the RFModulators to detach to CA Context*/
	void detachContext_Py(boost::python::list RFModulatorNames);
	/*! detach to the initial CA context for multi-threaded applications for all RFModulators*/
	void detachContext();
	/*! messaneger to print messages to std.out THIS SHOULD BE PUBLIC */
	LoggingSystem messenger;
	/*! Set up all modulator objects for this  factory */
	bool setup(const std::string& version);
	/*! Set up modulator objects for this factory based on passed machien areas */
	bool setup(const std::string& version, const std::vector<TYPE>& machine_areas_in);

	/*! get the rf modulator object 
		@param[in] string, name of modulator
		@param[out] RFModulator,  */
	RFModulator& getModulator(const std::string& name);
	/*! get the Full name of the object associated with the passed name
	@param[in] string, name of modulator to get Fullname for
	@param[out] string, full name (or UNKNONW_NAME if th epassed name is not recognized). */
	std::string getFullName(const std::string& name_to_check) const;
	/*! get the names of each modulator in the factory
	@param[in] string, name of modulator
	@param[out] vector<string>, name */
	std::vector<std::string> getAllRFModulatorNames();
	/*! get the names of each modulator in the factory (Python version)
	@param[in] string, name of modulator
	@param[out] vector<string>, name */
	boost::python::list getAllRFModulatorNames_Py();
	/*! get the name aliases of modulator with the passed name
	@param[in] string, name of modulator
	@param[out] vector<string>, name aliases */
	std::vector<std::string> getAliases(const std::string& name) const;
	/*! get the name aliases of modulator with the passed name (Python version)
	@param[in] string, name of modulator
	@param[out] vector<string>, name aliases */
	boost::python::list getAliases_Py(const std::string& name) const;
	/*! get the main STATE of a modulator 
		@param[in] string, name of modulator
		@param[out] STATE,  current main state */
	STATE getMainState(const std::string& name)const;
	/*! is the modulator in RF_ON STATE (For the gun Modulator RF_ON is the same as TRIG on the low level RF control)
		@param[in] string, name of modulator
		@param[out] bool,  */
	bool isInRFOn(const std::string& name) const;
	/*! is the modulator in HV_ON STATE 
		@param[in] string, name of modulator
		@param[out] bool,  */
	bool isInHVOn(const std::string& name) const;
	/*! is the modulator in STANDBY STATE
		@param[in] string, name of modulator
		@param[out] bool,  */
	bool isInStandby(const std::string& name) const;
	/*! is the modulator in OFF STATE
		@param[in] string, name of modulator
		@param[out] bool,  */
	bool isInOff(const std::string& name) const;
	/*! get the rf modulator error state 
		@param[in] string, name of modulator
		@param[out] STATE,  */
	STATE getErrorState(const std::string& name) const;
	/*! Compare the rf modulator error state to GOOD
		@param[in] string, name of modulator
		@param[out] bool,  */
	bool isErrorStateGood(const std::string& name) const;
	/*! Get the (read value) of modulator state-set 
		@param[in] string, name of modulator
		@param[out] STATE,  */
	STATE getStateSetRead(const std::string& name) const;
	/*! Has the warmup time for the modulator completed (can also change depending on the main state)
		@param[in] string, name of modulator
		@param[out] bool,  */
	bool isWarm(const std::string& name)const;
	/*! Has the warmup time for the modulator NOT completed (can also change depending on the main state)
		@param[in] string, name of modulator
		@param[out] bool,  */
	bool isNotWarm(const std::string& name)const;

	bool isInHoldRFOn(const std::string& name) const;

	bool isInManualOperation(const std::string& name) const;
	
	bool isInHoldRFOnCon(const std::string& name) const;
	
	STATE getHoldRFOnState(const std::string& name) const;

	void setHoldRFOnState(const std::string& name, STATE holdRFOnState);

	void setHoldRFOnToManualOperation(const std::string& name);

	void setHoldRFOnToHoldRFOnCon(const std::string& name);

	void setHoldRFOnToHoldRFOn(const std::string& name);

	/*! get the low level numerical data for this modulator (voltages, vacuumpressures etc) 
	@param[in] string, name of modulator
	@param[out] map<strign,double>, all low level numerical data in map   */
	std::map<std::string, double> getLowLevelNumericalData(const std::string& name)const;
	/*! get the low level numerical data for this modulator (voltages, vacuumpressures etc) Python Version
	@param[in] string, name of modulator
	@param[out] dict, all low level numerical data in map   */
	boost::python::dict getLowLevelNumericalData_Py(const std::string& name)const;
	/*! get the low level string data for this modulator (error messages) 
	@param[in] string, name of modulator
	@param[out] dict, all low level numerical data in map   */
	std::map<std::string, std::string> getLowLevelStringData(const std::string& name)const;
	/*! get the low level string data for this modulator (error messages) Python Version
	@param[in] string, name of modulator
	@param[out] dict, all low level string data */
	boost::python::dict getLowLevelStringData_Py(const std::string& name)const;
	/*! get all the low level data for this modulator (string and numerical) Python Version
	@param[in] string, name of modulator
	@param[out] dict, all low level string and numerical data */
	boost::python::dict getLowLevelData(const std::string& name)const;

	void debugMessagesOn();
	void debugMessagesOff();
	void messagesOn();
	void messagesOff();
	bool isDebugOn();
	bool isMessagingOn();

	friend class HardwareFactory;
protected:
	/*! has the factory been setuiup yet */
	bool hasBeenSetup;
private:
	/*! Fill the object map, based on values passed to set-up */
	void populateRFModulatorMap();
	/*! current mode (cirtual, offline, physical */
	STATE mode;
	/*! ConfigReader to parse YAML config files and create associated LLRF objects*/
	ConfigReader reader;
	/*! update the alias_name_map after reading the master lattice 
		@param[in] RFModulator, modulator to get alias names from */
	void updateAliasNameMap(const RFModulator& mod);
	/*! Set the montiro status (monitored or not monitored for this PV 
		@param[in] pvStruct, */
	void setMonitorStatus(pvStruct& pvStruct);
	/*! sets up the CHID for each PV  */
	void setupChannels();
	/*! The actualy modulators used can be cut by passing machine areas TYPEs, this function sets the member variable machine_areas
	! accounting for similar ways to refer to the GUN L01, it then calls eraseFromRFModulatorMap() with the modualtors TYPE to erase  */
	void setMachineAreaAndCutRFModulatorMap(const std::vector<TYPE>& machine_areas_in);
	/*! Erase modulators from the RFModulatorMap based on the based machine area TYPE
	 THIS function should not be called independantly from setMachineAreaAndCutRFModulatorMap, unless you know what you are doing
	! @param[in] vector<TYPE>, modualtors to erase 	*/
	void eraseFromRFModulatorMap(const std::vector<TYPE>& to_erase);
	/*! Map of modulator objects keyed by their full name */
	std::map<std::string, RFModulator> RFModulatorMap;
	/*! Map of modulator alise name keyed by their full name */
	std::map<std::string, std::string> alias_name_map;
	/*!  used when we need to return values from a requested name that does not exist */
	RFModulator dummy_mod;
	/*!  machien areas used in this factory */
	std::vector<TYPE> machine_areas;

};

#endif // RF_MODULATOR_FACTORY_H_