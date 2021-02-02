#ifndef RF_PROTECTION_H_
#define RF_PROTECTION_H_

#include <Hardware.h>
#include <EPICSRFProtectionInterface.h>
#include <GlobalConstants.h>
#include <GlobalStateEnums.h>
#include <boost/make_shared.hpp>

class EPICSRFProtectionInterface;

typedef boost::shared_ptr<EPICSRFProtectionInterface> EPICSRFProtectionInterface_sptr;

class RFProtection : public Hardware
{
public:
	RFProtection();
	RFProtection(const std::map<std::string, std::string>& paramMap, STATE mode);
	RFProtection(const RFProtection& copyRFProtection);
	~RFProtection();

	TYPE getProtectionType() const;
	std::string getProtectionTypeAsStr() const;
	EPICSRFProtectionInterface_sptr epicsInterface;
	std::map<std::string, std::string> RFProtectionParamMap;
	void debugMessagesOn();
	void debugMessagesOff();
	void messagesOn();
	void messagesOff();
private:
	TYPE protectionType;

	void setMasterLatticeData();

	void setPVStructs();

};


#endif //RF_PROTECTION_H_