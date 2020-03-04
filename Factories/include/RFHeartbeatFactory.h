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
	void setup(std::string version);
	bool hasBeenSetup;
	std::map<std::string, RFHeartbeat> RFHeartbeatMap;
};

#endif // RF_HEARTBEAT_FACTORY_H_