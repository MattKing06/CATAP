#ifndef RF_HEARTBEAT_FACTORY_H_
#define RF_HEARTBEAT_FACTORY_H_
#include <ConfigReader.h>
#include <RFHeartbeat.h>
#include <GlobalStateEnums.h>
#include <vector>

class RFHeartbeatFactory
{
public:
	/*! Default Constuctor (typically not used) */
	RFHeartbeatFactory();
	/*! Default Constuctor (typically not used) */
	RFHeartbeatFactory(STATE mode);
	RFHeartbeatFactory(STATE mode, const std::string& primeLatticeLocation);
	/*! Bespoke copy constructor  */
	RFHeartbeatFactory(const RFHeartbeatFactory& copyFactory);
	/*! Destructor */
	~RFHeartbeatFactory();
	/*! Set up all Heartbeat objects for this factory */
	bool setup(std::string version);
	/*! get a RFHeartbeatobject from the factory
	!	@param[in] string, name of object to return 
	!	@param[out] RFHeartbeat&, object reference */
	RFHeartbeat& getRFHeartbeat(const std::string& name);
	/*! get the latest value 
	!	@param[in] string, name of object to get value for 
	!	@param[out] double, return value */
	double getValue(const std::string& name) const;
	/*! set a value
	!	@param[in] string, name of object to set value for */
	void setValue(const std::string& name, double v);
	/*! pulse the heartbeat (will set a value different to current value) 
	!@param[in] string, name of object to pulse*/
	void pulse(const std::string& name);
	/*! Get the (full) names  of the RFHeartbeat objects owned by this factory 
	!@param[out] vector<string>, object names */
	std::vector<std::string> getHeartbeatNames()const;
#ifdef BUILD_PYTHON
	/*! Get the (full) names  of the RFHeartbeat objects owned by this factory, Python version 
	!@param[out] list, object names */
	boost::python::list getHeartbeatNames_Py()const;
#endif //BUILD_PYTHON
	/*! Get the (full) names  of an RFHeartbeat objects 
	!@param[out] list, object names */
	std::string getFullName(const std::string& name_to_check) const;
	/*! turns debug messages on for BPMFactory and calls same function in all BPMs and configReader*/
	void debugMessagesOn();
	/*! turns debug messages off for BPMFactory and calls same function in all BPMs and configReader*/
	void debugMessagesOff();
	/*! turns messages on for BPMFactory and calls same function in all BPMs and configReader*/
	void messagesOn();
	/*! turns messages off for BPMFactory and calls same function in all BPMs and configReader*/
	void messagesOff();
	/*! returns true if messenger debug flag is true, false otherwise*/
	bool isDebugOn();
	/*! returns true if messenger messages flag is true, false otherwise*/
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
	/*!  messenger, used to print messages to stdout */
	LoggingSystem messenger;
};

#endif // RF_HEARTBEAT_FACTORY_H_