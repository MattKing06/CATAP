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
	/*! Default Constuctor (typically not used) */
	RFHeartbeat();
	/*! Constuctor, 	
	!	@param[in] map<string, string>, data from master lattice file 
	!	@param[in] STATE, virtual / physcial / offline mode flag   */
	RFHeartbeat(const std::map<std::string, std::string>& paramMap, STATE mode);
	/*! Bespoke copy constructor  */
	RFHeartbeat(const RFHeartbeat& copyRFHeartbeat);
	/*! Destructor */
	~RFHeartbeat();
	/*! Attach to the initial CA Context for use in Multi-threaded applications*/
	void attachToInitialContext();
	/*! set up the map of pvStructs for this object  */
	void setPVStructs();
	/*! get the name alises for this object 
!	@param[out] vector<string>, all alias names */
	std::vector <std::string > getAliases()const;
	/*! pointer to interface object */
	EPICSRFHeartbeatInterface_sptr epicsInterface;
	/*! info ,
!	@param[out] double, latest heartbeat value */
	double getValue() const;
	/*! set a particular value for the heartbeat 
!	@param[in] double, new value to set */
	void setValue(double v);
	/*! send a pulse to the heartbeat, forcing a change in current vlaue */
	void pulse();
	/*! turns debug messages on for BPMFactory and calls same function in all BPMs and configReader*/
	void debugMessagesOn();
	/*! turns debug messages off for BPMFactory and calls same function in all BPMs and configReader*/
	void debugMessagesOff();
	/*! turns messages on for BPMFactory and calls same function in all BPMs and configReader*/
	void messagesOn();
	/*! turns messages off for BPMFactory and calls same function in all BPMs and configReader*/
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
	/*! local copy of paramMap passed in constructor */
	std::map<std::string, std::string> RFHeartbeatParamMap;
};
#endif //RF_HEARTBEAT_H_