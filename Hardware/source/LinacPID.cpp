#include <LinacPID.h>
#include <boost/make_shared.hpp>

LinacPID::LinacPID() {}

LinacPID::LinacPID(const std::map<std::string, std::string>& paramMap, STATE mode)
	: Hardware(paramMap, mode)
{
	setPVStructs();
	epicsInterface = boost::make_shared<EPICSLinacPIDInterface>(EPICSLinacPIDInterface());
	epicsInterface->ownerName = hardwareName;
	messenger = LoggingSystem(true, true);
}

void LinacPID::setPVStructs()
{
	for (auto&& record : LinacPIDRecords::linacPIDRecordList)
	{
		pvStructs[record] = pvStruct();
		pvStructs[record].pvRecord = record;
		std::string PV = specificHardwareParameters.find(record)->second.data();
		switch (mode)
		{
		case STATE::VIRTUAL:
			pvStructs[record].fullPVName = "VM-" + PV;
			break;
		default:
			pvStructs[record].fullPVName = PV;
			break;
		}
	}
}

std::vector<std::string> LinacPID::getAliases() const
{
	return aliases;
}

void LinacPID::debugMessagesOn()
{
	messenger.debugMessagesOn();
	messenger.printDebugMessage(hardwareName, " - DEBUG ON");
	epicsInterface->debugMessagesOn();
}

void LinacPID::debugMessagesOff()
{
	messenger.printDebugMessage(hardwareName, " - DEBUG OFF");
	messenger.debugMessagesOff();
	epicsInterface->debugMessagesOff();
}

void LinacPID::messagesOn()
{
	messenger.messagesOn();
	messenger.printMessage(hardwareName, " - MESSAGES ON");
	epicsInterface->messagesOn();
}

void LinacPID::messagesOff()
{
	messenger.printMessage(hardwareName, " - MESSAGES OFF");
	messenger.messagesOff();
	epicsInterface->messagesOff();
}