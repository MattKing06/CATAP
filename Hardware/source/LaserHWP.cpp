#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <iostream>
#include <LaserHWP.h>
#include <map>
#include <vector>
#include <numeric>
#include <math.h>
#include "boost/algorithm/string/split.hpp"
#include <boost/make_shared.hpp>
#include <boost/circular_buffer.hpp>

LaserHWP::LaserHWP()
{}
LaserHWP::LaserHWP(const std::map<std::string, std::string> & paramsMap, STATE mode) :
Hardware(paramsMap, mode),
laserType(LaserHWPRecords::laserHWPTypeToEnum.at(paramsMap.find("laser_pv_type")->second)),
name(paramsMap.find("name")->second),
position(std::stod(paramsMap.find("position")->second))
{
messenger.printDebugMessage("constructor");
setPVStructs();
epicsInterface = boost::make_shared<EPICSLaserHWPInterface>(EPICSLaserHWPInterface());
epicsInterface->ownerName = hardwareName;
}
LaserHWP::LaserHWP(const LaserHWP & copyLaser) :
Hardware(copyLaser),
laserType(copyLaser.laserType),
name(copyLaser.name),
position(copyLaser.position),
epicsInterface(copyLaser.epicsInterface)
{
}

void LaserHWP::setPVStructs()
{
	messenger.printDebugMessage("in setPVstructs");

	for (auto&& record : LaserHWPRecords::laserHWPRecordList)
	{
		messenger.printDebugMessage("in loop");
		pvStructs[record] = pvStruct();
		messenger.printDebugMessage(record);
		pvStructs[record].pvRecord = record;

		// TODO NO ERROR CHECKING! (we assum config file is good??? 
		std::string PV = specificHardwareParameters.find(record)->second.data();
		// iterate through the list of matches and set up a pvStruct to add to pvStructs.
		messenger.printDebugMessage("Constructing PV information for ", record);

		/*TODO
		  This should be put into some general function: generateVirtualPV(PV) or something...
		  Unless virtual PVs are to be included in the YAML files, they can be dealt with on
		  The config reader level if that is the case.
		  DJS maybe they should, how certian cna we be all virtual PVs will get a VM- prefix???
		  */
		if (mode == STATE::VIRTUAL)
		{
			pvStructs[record].fullPVName = "VM-" + PV;
		}
		else
		{
			pvStructs[record].fullPVName = PV;
		}
		//pv.pvRecord = record;
		//chid, count, mask, chtype are left undefined for now.
		//pvStructs[pv.pvRecord] = pv;
	}

}

std::vector<std::string> LaserHWP::getAliases() const
{
	return this->aliases;
}

TYPE LaserHWP::getLaserHWPType() const
{
	return this->laserType;
}

std::string LaserHWP::getLaserHWPName() const
{
	return this->name;
}

bool LaserHWP::setHWP(double set)
{
	epicsInterface->setMABS(set, pvStructs.at(LaserHWPRecords::MABS));
	hwpset.second = set;
	return true;
}

double LaserHWP::getHWPSet() const
{
	return hwpset.second;
}

double LaserHWP::getHWPRead() const
{
	return hwpread.second;
}

void LaserHWP::debugMessagesOff()
{
	messenger.printDebugMessage(hardwareName, " - DEBUG OFF");
	messenger.debugMessagesOff();
	epicsInterface->debugMessagesOff();
}

void LaserHWP::debugMessagesOn()
{
	messenger.debugMessagesOn();
	messenger.printDebugMessage(hardwareName, " - DEBUG On");
	epicsInterface->debugMessagesOn();
}

void LaserHWP::messagesOff()
{
	messenger.printMessage(hardwareName, " - MESSAGES OFF");
	messenger.messagesOff();
	epicsInterface->messagesOff();
}

void LaserHWP::messagesOn()
{
	messenger.messagesOn();
	messenger.printMessage(hardwareName, " - MESSAGES On");
	epicsInterface->messagesOn();
}