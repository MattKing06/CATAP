#include <GeneralMonitor.h>
#include <vector>

GeneralMonitor::GeneralMonitor() : Hardware()
{
}

GeneralMonitor::GeneralMonitor(const std::map<std::string, std::string>& paramMap, STATE mode)
{
}

GeneralMonitor::GeneralMonitor(const GeneralMonitor& copyGeneralMonitor)
{
}

GeneralMonitor::~GeneralMonitor()
{
}

void GeneralMonitor::monitor(std::vector<std::string> pvList)
{
	for (auto& pv : pvList)
	{
		std::cout << pv << std::endl;
	}
}

void GeneralMonitor::monitor_Py(boost::python::list pvList)
{
	std::vector<std::string> pvVector = to_std_vector<std::string>(pvList);
	monitor(pvVector);
}

void GeneralMonitor::setPVStructs()
{
}

void GeneralMonitor::debugMessagesOn()
{
	messenger.debugMessagesOn();
	messenger.printDebugMessage(hardwareName, " - DEBUG ON");
	//epicsInterface->debugMessagesOn();
}

void GeneralMonitor::debugMessagesOff()
{
	messenger.printDebugMessage(hardwareName, " - DEBUG OFF");
	messenger.debugMessagesOff();
	//epicsInterface->debugMessagesOff();
}

void GeneralMonitor::messagesOn()
{
	messenger.messagesOn();
	messenger.printMessage(hardwareName, " - MESSAGES ON");
	//epicsInterface->messagesOn();
}

void GeneralMonitor::messagesOff()
{
	messenger.printMessage(hardwareName, " - MESSAGES OFF");
	messenger.messagesOff();
	//epicsInterface->messagesOff();
}
