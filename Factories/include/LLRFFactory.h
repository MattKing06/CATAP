#ifndef LLRF_FACTORY_H_
#define LLRF_FACTORY_H_

#include <ConfigReader.h>
#include <LLRF.h>
#include <GlobalStateEnums.h>
#include <vector>
class LLRFFactory
{
public:
	LLRFFactory();
	LLRFFactory(STATE mode);
	LLRFFactory(const LLRFFactory& copyFactory);
	~LLRFFactory();
	LoggingSystem messenger;


	/*! default setup function, uses default values to read files and connect to EPICS etc.
	@param[out] bool, for success or failure	*/
	bool setup();
	/*! setup function using version parameter
	@param[in] version (a placeholder for future extensions)
	@param[out] bool, for success or failure	*/
	bool setup(const std::string& version);
	/*! setup function using machineArea parameter
	@param[in] machineArea, only setup magnets that match machineArea
	@param[out] bool, for success or failure	*/
	bool setup(TYPE machineArea);
	/*! setup function using version and machineArea parameters
	@param[in] version (a placeholder for future extensions)
	@param[in] machineArea, only setup magnets that match machineArea
	@param[out] bool, for success or failure	*/
	bool setup(const std::string& version, TYPE machineArea);
	/*! setup function using std::vector of machineAreas
	@param[in] machineAreas, only setup magnets that match an area in machineAreas
	@param[out] bool, for success or failure	*/
	bool setup(const std::vector<TYPE>& machineAreas);
	/*! setup function using python::list of machineAreas
	@param[in] machineAreas, only setup magnets that match an area in machineAreas
	@param[out] bool, for success or failure	*/
	bool setup(const boost::python::list& machineAreas);
	/*! setup function using python::list of machineAreas
	@param[in] version, (a placeholder for future extensions)
	@param[in] machineAreas, only setup magnets that match an area in machineAreas
	@param[out] bool, for success or failure	*/
	bool setup(const std::string& version, const boost::python::list& machineAreas);
	/*! setup function using std::vector of machineAreas
	@param[in] version, (a placeholder for future extensions)
	@param[in] machineAreas, only setup magnets that match an area in machineAreas
	@param[out] bool, for success or failure	*/
	bool setup(const std::string& version, const std::vector<TYPE>& machineAreas);


	void debugMessagesOn();
	void debugMessagesOff();
	void messagesOn();
	void messagesOff();
	bool isDebugOn();
	bool isMessagingOn();


	/*! Flag to say whether the LLRFFactory setup function has been completed successfully.*/
	bool hasBeenSetup; // PUBLIC as it is used by hardwarefactory, could make it a friend
private:

	void populateLLRFMap();

	/*! The kind of LLRFFactory that is created (PHYSICAL, VIRTUAL, OFFLINE), this variable is passed to the
	LLRF hardware objects when they are created by ConfigReader*/
	STATE mode;
	/*! ConfigReader to parse YAML config files and create associated LLRF objects*/
	ConfigReader reader;


	std::vector<TYPE> machineAreas;

	void updateAliasNameMap(const LLRF& llrf);
	std::map<std::string, std::string> alias_name_map;

	std::map<std::string, LLRF> LLRFMap;

};

#endif // RF_MODULATOR_FACTORY_H_