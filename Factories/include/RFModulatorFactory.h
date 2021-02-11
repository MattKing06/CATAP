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
	RFModulatorFactory(const RFModulatorFactory& copyFactory);
	~RFModulatorFactory();



	// get a LLRF object (GUN, L01, etc... ) 
	RFModulator& getModulator(const std::string& name);

	std::string getFullName(const std::string& name_to_check) const;
	std::vector<std::string> getAllRFModulatorNames();
	boost::python::list getAllRFModulatorNames_Py();


	/*! get the name alises for this LLRF
	@param[out] names, vector containing  all the alias names */
	std::vector<std::string> getAliases(const std::string& name) const;
	/*! get the name alises for this LLRF (python version)
		@param[out] names, python list containing all the alias names */
	boost::python::list getAliases_Py(const std::string& name) const;

	STATE getMainState(const std::string& name)const;
	bool isInRFOn(const std::string& name) const;
	bool isInHVOn(const std::string& name) const;
	bool isInStandby(const std::string& name) const;
	bool isInOff(const std::string& name) const;

	STATE getErrorState(const std::string& name) const;
	bool isErrorStateGood(const std::string& name) const;
	STATE getStateSetRead(const std::string& name) const;

	bool isWarm(const std::string& name)const;
	bool isNotWarm(const std::string& name)const;

	std::map<std::string, double> getLowLevelNumercialData(const std::string& name)const;
	boost::python::dict getLowLevelNumercialData_Py(const std::string& name)const;
	std::map<std::string, std::string> getLowLevelStringData(const std::string& name)const;
	boost::python::dict getLowLevelStringData_Py(const std::string& name)const;
	boost::python::dict getLowLevelData(const std::string& name)const;


	void debugMessagesOn();
	void debugMessagesOff();
	void messagesOn();
	void messagesOff();
	bool isDebugOn();
	bool isMessagingOn();


	friend class HardwareFactory;

protected:

	bool hasBeenSetup;

private:


	LoggingSystem messenger;
	bool setup(std::string version, const std::vector<TYPE>& machine_areas_in);
	void populateRFModulatorMap();

	STATE mode;
	/*! ConfigReader to parse YAML config files and create associated LLRF objects*/
	ConfigReader reader;

	void retrieveUpdateFunctionForRecord();
	void updateAliasNameMap(const RFModulator& mod);
	void setMonitorStatus(pvStruct& pvStruct);
	void setupChannels();
	//void updateAliasNameMap(const RFModulator& mod);

	void setMachineAreaAndCutRFModulatorMap(const std::vector<TYPE>& machine_areas_in);
	void eraseFromRFModulatorMap(const std::vector<TYPE>& to_erase);

	std::map<std::string, RFModulator> RFModulatorMap;

	std::map<std::string, std::string> alias_name_map;

	// used when we need to return values from a requested rf_prot name that does not exist 
	RFModulator dummy_mod;

	std::vector<TYPE> machine_areas;

};

#endif // RF_MODULATOR_FACTORY_H_