#ifndef GENERAL_MONITOR_H_
#define GENERAL_MONITOR_H_

#include <Hardware.h>
#include <EPICSGeneralMonitorInterface.h>
#include <GlobalConstants.h>
#include <GlobalStateEnums.h>
#include <boost/make_shared.hpp>

class EPICSGeneralMonitorInterface;

typedef boost::shared_ptr<EPICSGeneralMonitorInterface> EPICSGeneralMonitorInterface_sptr;

class GeneralMonitor : public Hardware
{
public:
	GeneralMonitor();
	GeneralMonitor(const std::map<std::string, std::string>& paramMap, STATE mode);
	GeneralMonitor(const GeneralMonitor& copyGeneralMonitor);
	~GeneralMonitor();
	void setPVStructs();
	EPICSGeneralMonitorInterface_sptr epicsInterface;
	std::map<std::string, std::string> generalMonitorParamMap;
	void debugMessagesOn();
	void debugMessagesOff();
	void messagesOn();
	void messagesOff();

};


#endif //GENERAL_MONITOR_H_