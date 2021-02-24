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

	/*! Set up all Heartbeat objects for this factory */
	bool setup(std::string version);


	RFHeartbeat& getRFHeartbeat(const std::string& name);
	double getValue(const std::string& name) const;
	void setValue(const std::string& name, double v);
	void pulse(const std::string& name);


	std::vector<std::string> getHeartbeatNames()const;
	boost::python::list getHeartbeatNames_Py()const;
	std::string getFullName(const std::string& name_to_check) const;


	void debugMessagesOn();
	void debugMessagesOff();
	void messagesOn();
	void messagesOff();
	bool isDebugOn();
	bool isMessagingOn();

	friend class HardwareFactory;
protected:
	/*! has the factory been setup yet */
	bool hasBeenSetup;
private:
	/*! Fill the object map, based on values passed to set-up */
	void populateRFHeartbeatMap();
	/*! sets PV struct function pointer */
	void retrieveUpdateFunctionForRecord();
	/*! Set the montior status (monitored or not monitored for this PV
	! @param[in] pvStruct, */
	void setMonitorStatus(pvStruct& pvStruct);
	/*! current mode (virtual, offline, physical */
	STATE mode;
	/*! ConfigReader to parse YAML config files and create associated Heartbeat objects*/
	ConfigReader reader;

	/*! Map of RFHeartbeat objects keyed by RFHeartbeatMap full name */
	std::map<std::string, RFHeartbeat> RFHeartbeatMap;
	/*! Map of RFHeartbeat alise names keyed by their full name (or the other way around!)  */
	std::map<std::string, std::string> alias_name_map;
	/*! update the alias_name_map after reading the master lattice
	! @param[in] RFHeartbeat, object to get alias names from */
	void updateAliasNameMap(const RFHeartbeat& rf_hb);

	/*! sets up the CHID for each PV  */
	void setupChannels();

	/*!  used when we need to return values from a requested name that does not exist */
	RFHeartbeat dummy_rfHeartbeat;


};

#endif // RF_HEARTBEAT_FACTORY_H_