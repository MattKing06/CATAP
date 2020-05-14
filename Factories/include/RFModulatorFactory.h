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
	void setup(std::string version);
	bool hasBeenSetup;
	std::map<std::string, RFModulator> RFModulatorMap;
	void debugMessagesOn();
	void debugMessagesOff();
	void messagesOn();
	void messagesOff();
	bool isDebugOn();
	bool isMessagingOn();
};

#endif // RF_MODULATOR_FACTORY_H_