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
	RFProtectionFactory(STATE mode, const std::string& primeLatticeLocation);
	RFProtectionFactory(const RFProtectionFactory& copyFactory);
	~RFProtectionFactory();
	LoggingSystem messenger;
	void setupChannels();
	void setMonitorStatus(pvStruct& pv);
	bool setup(const std::string& version);
	void populateRFProtectionMap();
	std::vector<std::string> getAllRFProtectionNames();


	bool resetGun();
	bool resetL01();
	bool enableGun();
	bool enableL01();
	bool disableGun();
	bool disableL01();

	STATE getStatus(const std::string& name_string)const;


	std::vector<int> getKeyBits(const std::string& name_string)const;
	boost::python::list getKeyBits_Py(const std::string& name_string)const;

	std::vector<bool> getKeyBitValues(const std::string& name_string)const;
	boost::python::list getKeyBitValues_Py(const std::string& name_string)const;


	std::string getFullName(const std::string& name_to_check) const;

	void debugMessagesOn();
	void debugMessagesOff();
	void messagesOn();
	void messagesOff();
	bool isDebugOn();
	bool isMessagingOn();


	// public so hardware factory can access, maybe protected / friend ? 
	bool hasBeenSetup;

private:
	ConfigReader reader;
	STATE mode;
	std::map<std::string, RFProtection> RFProtectionMap;

	void updateNameAliasMap(const RFProtection& prot);


	std::map<std::string, std::string> alias_name_map;

	// used when we need to return values from a requested rf_prot name that does not exist 
	RFProtection dummy_prot;

};

#endif // RF_PROTECTION_FACTORY_H_