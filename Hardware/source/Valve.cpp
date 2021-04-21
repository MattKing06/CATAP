#include <Valve.h>
#include <boost/make_shared.hpp>

Valve::Valve() {}

Valve::Valve(const std::map<std::string, std::string>& paramMap, STATE mode) : 
	Hardware(paramMap, mode),
	valveState(std::make_pair<epicsTimeStamp,STATE>(epicsTimeStamp(), STATE::ERR))
{
	messenger.printDebugMessage("Valve Constructor called ");
	setPVStructs();
	messenger.printDebugMessage("setPVStructs set");
	epicsInterface = boost::make_shared<EPICSValveInterface>(EPICSValveInterface());
	epicsInterface->ownerName = hardwareName;
	messenger = LoggingSystem(true, true);
}

void Valve::setPVStructs() 
{
	for (auto&& record : ValveRecords::valveRecordList)
	{
		pvStructs[record] = pvStruct();
		pvStructs[record].pvRecord = record;
		std::string PV = specificHardwareParameters.find(record)->second.data();
		messenger.printDebugMessage("Constructing PV information for ", PV);
		switch (mode) {
		case STATE::VIRTUAL:
			pvStructs[record].fullPVName = "VM-" + PV;
			break;
		default:
			pvStructs[record].fullPVName = PV;
			break;
		}
	}
}

std::vector<std::string> Valve::getAliases() const
{
	return aliases;
}

void Valve::open()
{
	switch (mode)
	{
		case STATE::PHYSICAL:
			epicsInterface->setNewValveState(STATE::OPEN, pvStructs.at(ValveRecords::On));
			break;
		case STATE::VIRTUAL:
			epicsInterface->setNewValveState(STATE::OPEN, pvStructs.at(ValveRecords::On));
			break;
		default:
			offlineSetValveState(STATE::OPEN);
	}
}

void Valve::close()
{
	switch (mode)
	{
	case STATE::PHYSICAL:
		epicsInterface->setNewValveState(STATE::CLOSED, pvStructs.at(ValveRecords::Off));
		break;
	case STATE::VIRTUAL:
		epicsInterface->setNewValveState(STATE::CLOSED, pvStructs.at(ValveRecords::Off));
		break;
	default:
		offlineSetValveState(STATE::CLOSED);
	}
}

HardwareState Valve::getState() 
{
	currentState.update(ValveRecords::Sta, valveState.second);
	return currentState;
}

boost::python::dict Valve::getState_Py()
{
	currentState.update(ValveRecords::Sta, valveState.second);
	return currentState.getState_Py();
}

void Valve::setValveState(const STATE& state)
{
	switch (state)
	{
	case STATE::OPEN: valveState.second = STATE::OPEN; break;
	case STATE::CLOSED: valveState.second = STATE::CLOSED; break;
	default:
		valveState.second = STATE::ERR;
	}
}

STATE Valve::getValveState() const
{
	return valveState.second;
}

bool Valve::isOpen() const
{
	return getValveState() == STATE::OPEN;
}

bool Valve::isClosed() const
{
	return getValveState() == STATE::CLOSED;
}

void Valve::offlineSetValveState(STATE state)
{
	valveState.second = state;
}

void Valve::debugMessagesOn()
{
	messenger.debugMessagesOn();
	messenger.printDebugMessage(hardwareName, " - DEBUG ON");
	epicsInterface->debugMessagesOn();
}

void Valve::debugMessagesOff()
{
	messenger.printDebugMessage(hardwareName, " - DEBUG OFF");
	messenger.debugMessagesOff();
	epicsInterface->debugMessagesOff();
}

void Valve::messagesOn()
{
	messenger.messagesOn();
	messenger.printMessage(hardwareName, " - MESSAGES ON");
	epicsInterface->messagesOn();
}

void Valve::messagesOff()
{
	messenger.printMessage(hardwareName, " - MESSAGES OFF");
	messenger.messagesOff();
	epicsInterface->messagesOff();
}
