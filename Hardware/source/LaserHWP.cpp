#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <iostream>
#include <LaserHWP.h>
#include <map>
#include <vector>
#include <numeric>
#include <math.h>
#include <GlobalFunctions.h>
#include "boost/algorithm/string/split.hpp"
#include <boost/make_shared.hpp>
#include <boost/circular_buffer.hpp>

LaserHWP::LaserHWP()
{}
LaserHWP::LaserHWP(const std::map<std::string, std::string> & paramsMap, STATE mode) :
Hardware(paramsMap, mode)
{
	messenger.printDebugMessage("LaserHWP constructor");

	if (GlobalFunctions::entryExists<std::string>(paramsMap, "laser_pv_type"))
	{
		laserType = LaserHWPRecords::laserHWPTypeToEnum.at(paramsMap.find("laser_pv_type")->second);
	}
	if (GlobalFunctions::entryExists<std::string>(paramsMap, "position"))
	{
		position = std::stod(paramsMap.find("position")->second);
	}
	if (GlobalFunctions::entryExists<std::string>(paramsMap, "name"))
	{
		name = paramsMap.find("name")->second;
	}

setPVStructs();
epicsInterface = boost::make_shared<EPICSLaserHWPInterface>(EPICSLaserHWPInterface());
epicsInterface->ownerName = hardwareName;
messenger.printDebugMessage("LaserHWP constructor complete");
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


//____________________________________________________________________________________________
bool LaserHWP::isHWPEnabled() const
{
	return hwp_enable.second == STATE::ON;
}
//____________________________________________________________________________________________
bool LaserHWP::isHWPDisabled()const
{
	return hwp_enable.second == STATE::OFF;
}
//____________________________________________________________________________________________
STATE LaserHWP::getHWPEnableState()const
{
	return hwp_enable.second;
}

//____________________________________________________________________________________________
bool LaserHWP::enableHWP()
{
	//messenger.printDebugMessage("pilaserInterface::enableHWP ");
	//messenger.printDebugMessage("CHTYPE =  ", pilaser.pvComStructs.at(pilaserStructs::PILASER_PV_TYPE::HALF_WAVE_PLATE_ENABLE).CHTYPE);
	//messenger.printDebugMessage("CHID =  ", pilaser.pvComStructs.at(pilaserStructs::PILASER_PV_TYPE::HALF_WAVE_PLATE_ENABLE).CHID);
	//    pilaserStructs::pvStruct& pvs = pilaser.pvComStructs.at(pilaserStructs::PILASER_PV_TYPE::HALF_WAVE_PLATE_ENABLE);
	//    dbr_enum_t c = (dbr_enum_t)UTL::ONE_US;
	//need to figure out how to move th elaser from command line...
	return epicsInterface->putValue2<unsigned short>(pvStructs.at(LaserHWPRecords::ENABLE), GlobalConstants::one_ushort);
}
//____________________________________________________________________________________________
bool LaserHWP::disableHWP()
{
	//message("pilaserInterface::disableHWP ");
	//message("CHTYPE =  ", pilaser.pvComStructs.at(pilaserStructs::PILASER_PV_TYPE::HALF_WAVE_PLATE_ENABLE).CHTYPE);
	//message("CHID =  ", pilaser.pvComStructs.at(pilaserStructs::PILASER_PV_TYPE::HALF_WAVE_PLATE_ENABLE).CHID);
	//need to figure out how to move the laser from command line...
	//pilaserStructs::pvStruct& pvs = pilaser.pvComStructs.at(pilaserStructs::PILASER_PV_TYPE::HALF_WAVE_PLATE_ENABLE);
	return epicsInterface->putValue2<unsigned short>(pvStructs.at(LaserHWPRecords::ENABLE), GlobalConstants::zero_ushort);
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