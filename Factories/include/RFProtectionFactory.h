#ifndef RF_PROTECTION_FACTORY_H_
#define RF_PROTECTION_FACTORY_H_

#include <ConfigReader.h>
#include <RFProtection.h>
#include <GlobalStateEnums.h>
#include <vector>
class RFProtectionFactory
{
public:
	RFProtectionFactory();
	RFProtectionFactory(STATE mode);
	RFProtectionFactory(const RFProtectionFactory& copyFactory);
	~RFProtectionFactory();
	LoggingSystem messenger;
	void setup(std::string version);
	bool hasBeenSetup;
	std::map<std::string, RFProtection> RFProtectionMap;
	void debugMessagesOn();
	void debugMessagesOff();
	void messagesOn();
	void messagesOff();
	bool isDebugOn();
	bool isMessagingOn();
};

#endif // RF_PROTECTION_FACTORY_H_