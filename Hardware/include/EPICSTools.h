#ifndef GENERAL_MONITOR_H_
#define GENERAL_MONITOR_H_

#include <Hardware.h>
#include <EPICSToolsInterface.h>
#include <GlobalConstants.h>
#include <PythonTypeConversions.h>
#include <GlobalStateEnums.h>
#include <boost/make_shared.hpp>
#include <boost/python.hpp>

class EPICSToolsInterface;

typedef boost::shared_ptr<EPICSToolsInterface> EPICSToolsInterface_sptr;

class EPICSTools : public Hardware
{
public:
	EPICSTools();
	EPICSTools(STATE mode, std::string pv);
	EPICSTools(const EPICSTools& copyEPICSTools);
	~EPICSTools();
	void monitor();
	void setPVStructs();
	EPICSToolsInterface_sptr epicsInterface;
	std::map<std::string, std::string> generalMonitorParamMap;
	void debugMessagesOn();
	void debugMessagesOff();
	void messagesOn();
	void messagesOff();
	std::string pvToMonitor;
};


#endif //GENERAL_MONITOR_H_