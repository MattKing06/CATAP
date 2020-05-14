#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <iostream>
#include <IMG.h>
#include <IMGPVRecords.h>
#include <map>
#include <vector>
#include <functional>
#include "boost/algorithm/string/split.hpp"
#include <boost/make_shared.hpp>

#include "PythonTypeConversions.h"
#include "GlobalConstants.h"
#include "GlobalFunctions.h"

IMG::IMG()
{
}

IMG::IMG(const std::map<std::string, std::string>& paramMap, STATE mode) : Hardware(paramMap, mode),
state(std::make_pair(epicsTimeStamp(), STATE::ERR)),
pressure(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min))
{
	setPVStructs();
	epicsInterface = boost::make_shared<EPICSIMGInterface>(EPICSIMGInterface());
	epicsInterface->ownerName = hardwareName;
	messenger = LoggingSystem(true, true);
}

IMG::IMG(const IMG& copyIMG)
{
}

IMG::~IMG()
{
}

void IMG::setPVStructs() {

	for (auto&& record : IMGRecords::imgRecordList)
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

std::vector<std::string> IMG::getAliases() const
{
	return aliases;
}

double IMG::getIMGPressure() const
{
	return pressure.second;
}

STATE IMG::getIMGState() const
{
	return state.second;
}

void IMG::setIMGState(const STATE& states)
{
	switch (states)
	{
	case STATE::ON: state.second = STATE::ON; break;
	case STATE::OFF: state.second = STATE::OFF; break;
	default:
		state.second = STATE::ERR;
	}
}

void IMG::debugMessagesOn()
{
	messenger.debugMessagesOn();
	messenger.printDebugMessage(hardwareName, " - DEBUG ON");
	epicsInterface->debugMessagesOn();
}

void IMG::debugMessagesOff()
{
	messenger.debugMessagesOff();
	messenger.printDebugMessage(hardwareName, " - DEBUG OFF");
	epicsInterface->debugMessagesOff();
}

void IMG::messagesOn()
{
	messenger.messagesOn();
	messenger.printMessage(hardwareName, " - MESSAGES ON");
	epicsInterface->messagesOn();
}

void IMG::messagesOff()
{
	messenger.printMessage(hardwareName, " - MESSAGES OFF");
	messenger.messagesOff();
	epicsInterface->messagesOff();
}


