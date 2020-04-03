#ifndef RF_HEARTBEAT_FACTORY_H_
#define RF_HEARTBEAT_FACTORY_H_

#include <ConfigReader.h>
#include <RFHeartbeat.h>
#include <GlobalStateEnums.h>
#include <vector>
class RFHeartbeatFactory
{
public:
	RFHeartbeatFactory();
	RFHeartbeatFactory(STATE mode);
	RFHeartbeatFactory(const RFHeartbeatFactory& copyFactory);
	~RFHeartbeatFactory();
	LoggingSystem messenger;
	void setup(std::string version);
	bool hasBeenSetup;
	std::map<std::string, RFHeartbeat> RFHeartbeatMap;
	void debugMessagesOn();
	void debugMessagesOff();
	void messagesOn();
	void messagesOff();
	bool isDebugOn();
	bool isMessagingOn();
};

#endif // RF_HEARTBEAT_FACTORY_H_