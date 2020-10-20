#include <Shutter.h>
#include <ShutterPVRecords.h>

// boost
#include "boost/algorithm/string/split.hpp"
#include <boost/make_shared.hpp>
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
shutter_state(std::make_pair(epicsTimeStamp(), STATE::ERR)),
cmi(std::make_pair(epicsTimeStamp(), GlobalConstants::int_min)),
// if you get a compile error due to something below its probably that you haven;t added hardware to the copy constructor 
epicsInterface(boost::make_shared<EPICSShutterInterface>(EPICSShutterInterface())) // calls copy constructor and destroys 
{
	epicsInterface->ownerName = hardwareName;
	boost::split(aliases, paramsMap.find("name_alias")->second, [](char c) {return c == ','; });
	boost::split(interlock_bit_names, paramsMap.find("shutter_interlock_names")->second, [](char c) {return c == ','; });

	// set up the default state of each named interlock to false
	for(auto&& name : interlock_bit_names)
	{
		cmi_bit_map[name] = STATE::ERR;
		messenger.printDebugMessage("added interlock name = ", name);
	}
	setPVStructs();
}

Shutter::Shutter(const Shutter& copyShutter):
	Hardware(copyShutter),
	aliases(copyShutter.aliases),
	epicsInterface(copyShutter.epicsInterface)
{
}

Shutter::~Shutter(){}



ShutterState Shutter::getShutterState()const
{
	ShutterState r;
	r.Cmi = getCMI();
	r.state = getState();
	r.name = getHardwareName();
	r.interlock_states = getCMIBitMap();
	r.interlock_states_Py = getCMIBitMap_Py();
	return r;
}

boost::python::dict Shutter::getShutterStateDictionary()const
{
	boost::python::dict r;
	r[std::string("Cmi")] = getCMI();

	// extract values for printing to test, and during debugging this, which was surprisingly painful 
	//int y = boost::python::extract<int>(r["Cmi"]);
	//messenger.printDebugMessage("Cmi = ", getCMI());
	//messenger.printDebugMessage("Cmi2 = ", y);
	r[std::string("state")] = getState();
	//STATE s = boost::python::extract<STATE>(r["state"]);
	//messenger.printDebugMessage("state = ", getState());
	//messenger.printDebugMessage("state2 = ", s);
	r[std::string("name")] = getHardwareName();
	//std::string n = boost::python::extract<std::string>(r["name"]);
	//messenger.printDebugMessage("getHardwareName = ", getHardwareName());
	//messenger.printDebugMessage("getHardwareNamee2 = ", n);

	// after much much faffing, (no really) the best way i found to combine 2 dicts is this!!! 
	// merge the return dictionary with the cmi_bit_map one 
	r.update( getCMIBitMap_Py() );
	return r;
}



bool Shutter::isClosed() const
{
	return shutter_state.second == STATE::CLOSED;
}
bool Shutter::isOpen() const
{
	return shutter_state.second == STATE::OPEN;
}

STATE Shutter::getState()const
{
	return shutter_state.second;
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
std::map<std::string, STATE> Shutter::getCMIBitMap()const
{
	return cmi_bit_map;
}
boost::python::dict  Shutter::getCMIBitMap_Py()const
{
	return to_py_dict<std::string, STATE>(cmi_bit_map);
}


void Shutter::setPVStructs()
{
	messenger.printDebugMessage("setPVStructs");
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
