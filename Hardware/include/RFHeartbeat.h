#ifndef RF_HEARTBEAT_H_
#define RF_HEARTBEAT_H_

#include <Hardware.h>
#include <EPICSRFHeartbeatInterface.h>
#include <GlobalConstants.h>
#include <GlobalStateEnums.h>
#include <boost/make_shared.hpp>
#include <boost/python/dict.hpp>
#include <boost/python/list.hpp>

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

	
	double getValue() const;
	void setValue(double v);
	void pulse();


	void debugMessagesOn();
	void debugMessagesOff();
	void messagesOn();
	void messagesOff();


	friend class EPICSRFHeartbeatInterface;
	friend class RFHeartbeatFactory;
protected:


	/*! latest heartbeat value. Value and epicstimestamp.	*/
	std::pair<epicsTimeStamp, double > hb_value;


private:

	// TODO should be base class member
	/*! alternative names for the hearteat (usually shorter thna the full PV root), defined master lattice */
	std::vector<std::string> aliases;

	std::map<std::string, std::string> RFHeartbeatParamMap;

};


#endif //RF_HEARTBEAT_H_