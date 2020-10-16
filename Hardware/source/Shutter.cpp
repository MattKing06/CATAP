#include <Shutter.h>
#include <ShutterPVRecords.h>

// boost
#include "boost/algorithm/string/split.hpp"
#include <boost/make_shared.hpp>
// CATAP includes
#include "PythonTypeConversions.h"
#include "GlobalConstants.h"
#include "GlobalFunctions.h"

Shutter::Shutter()
{
}

Shutter::Shutter(const std::map<std::string, std::string>& paramsMap, STATE mode):
Hardware(paramsMap, mode),
shutterState(std::make_pair(epicsTimeStamp(), STATE::ERR)),
cmi(std::make_pair(epicsTimeStamp(), GlobalConstants::int_min)),
// if you get a compile error due to something below its probably that you haven;t added hardware to the copy constructor 
epicsInterface(boost::make_shared<EPICSShutterInterface_sptr>(EPICSShutterInterface())) // calls copy constructor and destroys 
{
	epicsInterface->ownerName = hardwareName;
	boost::split(aliases, paramsMap.find("name_alias")->second, [](char c) {return c == ','; });
}

Shutter::Shutter(const Shutter& copyShutter):
	Hardware(copyShutter),
	aliases(copyShutter.aliases),
	epicsInterface(copyShutter.epicsInterface)
{
}

Shutter::~Shutter(){}


bool Shutter::isClosed() const
{
	return shutterState.second == STATE::CLOSED;
}
bool Shutter::isOpen() const
{
	return shutterState.second == STATE::OPEN;
}

STATE Shutter::getState()const
{
	return shutterState.second;
}

std::vector<std::string> Shutter::getAliases() const
{
	return this->aliases;
}
boost::python::list Shutter::getAliases_Py() const
{
	return to_py_list<std::string>(getAliases());
}


bool Shutter::close()
{
	// TODO move into epicsShutterInterface
	if(epicsInterface->putValue2(pvStructs.at(ShutterRecords::Close), GlobalConstants::EPICS_ACTIVATE))
	{
		return epicsInterface->putValue2(pvStructs.at(ShutterRecords::Close), GlobalConstants::EPICS_SEND);
	}
	return false;
}
bool Shutter::open()
{
	// TODO move into epicsShutterInterface
	if (epicsInterface->putValue2(pvStructs.at(ShutterRecords::Open), GlobalConstants::EPICS_ACTIVATE))
	{
		return epicsInterface->putValue2(pvStructs.at(ShutterRecords::Open), GlobalConstants::EPICS_SEND);
	}
	return false;
}

int Shutter::getCMI()const
{
	return cmi.second;
}
std::map<std::string, bool> Shutter::getCMIBitMap()const
{
	return cmiBitMap;
}
boost::python::dict  Shutter::getCMIBitMap_Py()const
{
	return to_py_dict<std::string, bool>(cmiBitMap);
}



void Shutter::setPVStructs()
{
	for(auto&& record : ShutterRecords::shutterRecordList)
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

void Shutter::debugMessagesOff()
{
	messenger.printDebugMessage(hardwareName, " - DEBUG OFF");
	messenger.debugMessagesOff();
	epicsInterface->debugMessagesOff();
}

void Shutter::debugMessagesOn()
{
	messenger.debugMessagesOn();
	messenger.printDebugMessage(hardwareName, " - DEBUG ON");
	epicsInterface->debugMessagesOn();
}

void Shutter::messagesOff()
{
	messenger.printMessage(hardwareName, " - MESSAGES OFF");
	messenger.messagesOff();
	epicsInterface->messagesOff();
}

void Shutter::messagesOn()
{
	messenger.messagesOn();
	messenger.printMessage(hardwareName, " - MESSAGES ON");
	epicsInterface->messagesOn();
}
