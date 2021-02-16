#ifndef RF_HEARTBEAT_H_
#define RF_HEARTBEAT_H_

#include <Hardware.h>
#include <EPICSRFHeartbeatInterface.h>
#include <GlobalConstants.h>
#include <GlobalStateEnums.h>
#include <boost/make_shared.hpp>

class EPICSRFHeartbeatInterface;

typedef boost::shared_ptr<EPICSRFHeartbeatInterface> EPICSRFHeartbeatInterface_sptr;

class RFHeartbeat : public Hardware
{
public:
	RFHeartbeat();
	RFHeartbeat(const std::map<std::string, std::string>& paramMap, STATE mode);
	RFHeartbeat(const RFHeartbeat& copyRFHeartbeat);
	~RFHeartbeat();
	void setPVStructs();

	std::vector <std::string > getAliases()const;

	EPICSRFHeartbeatInterface_sptr epicsInterface;
	std::map<std::string, std::string> RFHeartbeatParamMap;
	
	// TODO shoudl be base claass emmer
	/*! alternative names for the hearteat (usually shorter thna the full PV root), defined master lattice */
	std::vector<std::string> aliases;
	void debugMessagesOn();
	void debugMessagesOff();
	void messagesOn();
	void messagesOff();
};


#endif //RF_HEARTBEAT_H_