#ifndef EPICS_RF_PROTECTION_INTERFACE_H_
#define EPICS_RF_PROTECTION_INTERFACE_H_
#include <EPICSInterface.h>
#ifndef RF_PROTECTION_H_
#include <RFProtection.h>
#endif // RF_PROTECTION_H_
class EPICSRFProtectionInterface : public EPICSInterface
{
public:
	EPICSRFProtectionInterface();
	EPICSRFProtectionInterface(const EPICSRFProtectionInterface& copyRFProtection);
	~EPICSRFProtectionInterface();
	void retrieveUpdateFunctionForRecord(const pvStruct& pv);
	static void updateProtectionStatus(const struct event_handler_args args);
	static void updateProtectionCmi(const struct event_handler_args args);

	bool sendCmmands(const std::vector<const chtype*>& CHTYPE, const std::vector<const chid*>& CHID);
	

	bool setProtectionOn(const pvStruct& pv);
	bool setProtectionOff(const pvStruct& pv);
	bool resetProtection(const pvStruct& pv);
	std::string ownerName;
	// We need to a STATIC messenger so that the static call back functions can use it to print messages 
	static LoggingSystem messenger;
};

#endif //EPICS_RF_PROTECTION_INTERFACE_H_