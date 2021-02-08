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
	LoggingSystem messenger;
	bool setup(std::string version);
	bool hasBeenSetup;
	void populateRFModulatorMap();
	std::vector<std::string> getAllRFModulatorNames();



	void debugMessagesOn();
	void debugMessagesOff();
	void messagesOn();
	void messagesOff();
	bool isDebugOn();
	bool isMessagingOn();

private:
	STATE mode;
	/*! ConfigReader to parse YAML config files and create associated LLRF objects*/
	ConfigReader reader;

	void retrieveUpdateFunctionForRecord();
	void updateAliasNameMap(const RFModulator& mod);
	void setMonitorStatus(pvStruct& pvStruct);
	void setupChannels();
	//void updateAliasNameMap(const RFModulator& mod);

	std::map<std::string, RFModulator> RFModulatorMap;

	std::map<std::string, std::string> alias_name_map;

	// used when we need to return values from a requested rf_prot name that does not exist 
	RFModulator dummy_prot;

};

#endif // RF_MODULATOR_FACTORY_H_