#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <iostream>
#include <Screen.h>
#include <map>
#include <vector>
#include <numeric>
#include <math.h>
#include "boost/algorithm/string/split.hpp"
#include <boost/make_shared.hpp>
#include <boost/circular_buffer.hpp>

Screen::Screen()
{}
Screen::Screen(const std::map<std::string, std::string> & paramsMap, STATE mode) :
Hardware(paramsMap, mode),
screenType(paramsMap.find("screen_type")->second),
has_camera(paramsMap.find("has_camera")->second),
name(paramsMap.find("name")->second),
position(std::stod(paramsMap.find("position")->second))
{
messenger.printDebugMessage("constructor");
setPVStructs();
epicsInterface = boost::make_shared<EPICSScreenInterface>(EPICSScreenInterface());
epicsInterface->ownerName = hardwareName;
screenStateH.second = STATE::UNKNOWN;
screenStateV.second = STATE::UNKNOWN;
screenState.second = STATE::UNKNOWN;
screenSetStateH.second = STATE::UNKNOWN;
screenSetStateV.second = STATE::UNKNOWN;
screenSetState.second = STATE::UNKNOWN;
actposH.second = GlobalConstants::double_min;
actposV.second = GlobalConstants::double_min;
actpos.second = GlobalConstants::double_min;
tgtposH.second = GlobalConstants::double_min;
tgtposV.second = GlobalConstants::double_min;
tgtpos.second = GlobalConstants::double_min;
devcentH.second = GlobalConstants::double_min;
devcentV.second = GlobalConstants::double_min;
devcent.second = GlobalConstants::double_min;
jdiffH.second = GlobalConstants::double_min;
jdiffV.second = GlobalConstants::double_min;
jdiff.second = GlobalConstants::double_min;
jogH.second = GlobalConstants::double_min;
jogV.second = GlobalConstants::double_min;
jog.second = GlobalConstants::double_min;
enH.second = STATE::UNKNOWN;
enV.second = STATE::UNKNOWN;
exH.second = GlobalConstants::zero_int;
exV.second = GlobalConstants::zero_int;
triggerH.second = GlobalConstants::zero_int;
triggerVH.second = GlobalConstants::zero_int;
trigger.second = GlobalConstants::zero_int;
readyH.second = GlobalConstants::zero_int;
readyV.second = GlobalConstants::zero_int;
ready.second = GlobalConstants::zero_int;
movingH.second = GlobalConstants::zero_int;
movingV.second = GlobalConstants::zero_int;
moving.second = GlobalConstants::zero_int;
maxposH.second = GlobalConstants::zero_int;
maxposV.second = GlobalConstants::zero_int;
maxpos.second = GlobalConstants::zero_int;
}
Screen::Screen(const Screen & copyScreen) :
Hardware(copyScreen),
screenType(copyScreen.screenType),
name(copyScreen.name),
has_camera(copyScreen.has_camera),
position(copyScreen.position),
epicsInterface(copyScreen.epicsInterface)
{
}

void Screen::setPVStructs()
{
	messenger.printDebugMessage("in setPVstructs");

	for (auto&& record : ScreenRecords::bpmRecordList)
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

std::vector<std::string> Screen::getAliases() const
{
	return this->aliases;
}

std::string Screen::getScreenName() const
{
	return this->name;
}

double Screen::getPosition() const
{
	return this->position;
}

STATE Screen::getScreenState() const
{
	if (isHIn() || isHEnabled())
	{
		return this->screenStateH.second;
	}
	else if (isVIn() || isVEnabled())
	{
		return this->screenStateV.second;
	}
	else
		return this->screenState.second;
}

STATE Screen::getScreenSetState() const
{
	if (isHIn() || isHEnabled())
	{
		return this->screenSetStateH.second;
	}
	else if (isVIn() || isVEnabled())
	{
		return this->screenSetStateV.second;
	}
	else
		return this->screenSetState.second;
}

std::string Screen::getScreenType() const
{
	return this->screenType;
}

bool Screen::is_HandV_OUT() const
{
	return isHOut() && isVOut();
}

bool Screen::isScreenIn() const
{
	return isYAGIn();
}

bool Screen::isYAGIn() const
{
	if (isMover())
	{
		return screenStateV.second == STATE::VYAG;
	}
	else if (isPneumatic())
	{
		return screenState.second == STATE::YAG;
	}
}

bool Screen::isHMoving() const
{
	return movingH.second;
}

bool Screen::isVMoving() const
{
	return movingV.second;
}

bool Screen::isPMoving() const
{
	return moving.second;
}

bool Screen::isMoving() const
{
	if (movingH.second || movingV.second || moving.second)
	{
		return true;
	}
	return false;
}

bool Screen::isClearForBeam() const
{
	if (!isHIn() || !isVIn() || isRFCageIn())
	{
		return true;
	}
	return false;
}

bool Screen::isHOut() const
{
	return !isHIn();
}

bool Screen::isVOut() const
{
	return !isVIn();
}

bool Screen::isHIn() const
{
	if (screenStateH.second != ScreenRecords::HRETRACTED)
	{
		return true;
	}
	return false;
}

bool Screen::isVIn() const
{
	if (screenStateV.second != ScreenRecords::VRETRACTED)
	{
		return true;
	}
	return false;
}

bool Screen::isRFCageIn() const
{
	if (screenStateV.second != ScreenRecords::VRF || screenState.second != ScreenRecords::RF)
	{
		return true;
	}
	return false;
}

bool Screen::isMover() const
{
	if (isVMover() || isHVMover())
	{
		return true;
	}
	return false;
}

bool Screen::isVMover() const
{
	if (getScreenType() == TYPE::CLARA_V_MOVER)
	{
		return true;
	}
	return false;
}

bool Screen::isHVMover() const
{
	if (getScreenType() == TYPE::CLARA_HV_MOVER || TYPE::VELA_HV_MOVER)
	{
		return true;
	}
	return false;
}

bool Screen::isPneumatic() const
{
	if (getScreenType() == TYPE::VELA_PNEUMATIC)
	{
		return true;
	}
	return false;
}

bool Screen::isVELAPneumatic() const
{
	if (getScreenType() == TYPE::VELA_PNEUMATIC)
	{
		return true;
	}
	return false;
}

bool Screen::isHVMover() const
{
	if (getScreenType() == TYPE::CLARA_HV_MOVER || TYPE::VELA_HV_MOVER)
	{
		return true;
	}
	return false;
}

bool Screen::isVELAHVMover() const
{
	if (getScreenType() == TYPE::VELA_HV_MOVER)
	{
		return true;
	}
	return false;
}

bool Screen::isCLARAHVMover() const
{
	if (getScreenType() == TYPE::CLARA_HV_MOVER)
	{
		return true;
	}
	return false;
}

bool Screen::isCLARAVMover() const
{
	if (getScreenType() == TYPE::CLARA_V_MOVER)
	{
		return true;
	}
	return false;
}

bool Screen::isScreenInState(STATE sta) const
{
	if (isHElement(ScreenRecords::screenHElementMap(sta)))
	{
		return screenStateH.second == sta;
	}
	else if (isVElement(ScreenRecords::screenVElementMap(sta)))
	{
		return screenStateV.second == sta;
	}
	else if (isHElement(ScreenRecords::screenPElementMap(sta)))
	{
		return screenState.second == sta;
	}
}

bool Screen::isHElement(STATE state) const
{
	return isElement(ScreenRecords::screenHElementMap, state);
}

bool Screen::isVElement(STATE state) const
{
	return isElement(ScreenRecords::screenVElementMap, state);
}

bool Screen::isPElement(STATE state) const
{
	return isElement(ScreenRecords::screenPElementMap, state);
}

bool Screen::isHEnabled() const
{
	return enH.second == 1;
}

bool Screen::isVEnabled() const
{
	return enV.second == 1;
}

double Screen::getACTPOS() const
{
	if (isMover())
	{
		if (isHIn() || isHEnabled())
		{
			return actposH.second;
		}
		else if (isVIn() || isVEnabled())
		{
			return actposV.second;
		}
		else
		{
			return actposV.second;
		}
	}
	else
	{
		return actpos.second;
	}
}

double Screen::getJDiff() const
{
	if (isMover())
	{
		if (isHIn() || isHEnabled())
		{
			return jdiffH.second;
		}
		else if (isVIn() || isVEnabled())
		{
			return jdiffV.second;
		}
		else
		{
			return jdiffV.second;
		}
	}
	else
	{
		return jdiff.second;
	}
}

double Screen::getPosition() const
{
	return position.second;
}

double Screen::get_H_ACTPOS() const
{
	if (isHEnabled())
	{
		return actposH.second;
	}
}

double Screen::get_V_ACTPOS() const
{
	if (isVEnabled())
	{
		return actposV.second;
	}
}

void Screen::insertYAG()
{
	if(isMover())
	{
		moveScreenTo(STATE::VRF);
	}
	else
	{
		moveScreenTo(STATE::RF);
	}
}

void Screen::makeReadEqualSet()
{
	if (isMover())
	{
		if (screenStateV.second != screenSetStateV.second)
		{
			moveScreenTo(screenSetStateV.second);
		}
		else if (screenStateH.second != screenSetStateH.second)
		{
			moveScreenTo(screenSetStateH.second);
		}
	}
	else if (isPneumatic())
	{
		if (screenState.second != screenSetState.second)
		{
			moveScreenTo(screenSetState.second);
		}
	}
}

void Screen::makeSetEqualRead()
{
	if (isMover())
	{
		if (screenSetStateV.second != screenStateV.second)
		{
			moveScreenTo(screenStateV.second);
		}
		else if (screenSetStateH.second != screenStateH.second)
		{
			moveScreenTo(screenStateH.second);
		}
	}
	else if (isPneumatic())
	{
		if (screenSetState.second != screenState.second)
		{
			moveScreenTo(screenState.second);
		}
	}
}

void Screen::moveScreenOut()
{
	moveScreenTo(STATE::VRF);
}

void Screen::resetPosition()
{
	if (isMover)
	{
		if (isHIn() || isHEnabled())
		{
			setScreenTrigger(1, TYPE::HORIZONTAL);
		}
		else if(isVIn() || isVEnabled())
		{
			setScreenTrigger(1, TYPE::VERTICAL);
		}
	}
	else
	{
		setScreenTrigger(1, TYPE::PNEUMATIC);
	}
}

bool Screen::setScreenSDEV(STATE& state)
{
	setScreenSetState(state);
}

bool Screen::setScreenTrigger(const int& value, TYPE type)
{
	setTRIGGER(value, type);
}

bool Screen::moveScreenTo(STATE& state)
{
	setScreenSetState(state);
}

bool Screen::setScreenSetState(STATE& state)
{
	if (isHElement(state))
	{
		setSDEV(findByValue(screenHElementMap, state), TYPE::HORIZONTAL);
	}
	else if (isBElement(state))
	{
		setSDEV(findByValue(screenVElementMap, state), TYPE::VERTICAL);
	}
	else if (isPElement(state))
	{
		setSDEV(findByValue(screenPElementMap, state), TYPE::PNEUMATIC);
	}
	return true;
}

bool Screen::setTGTPOS(const double& value, TYPE type)
{
	switch (type)
	{
	case TYPE::HORIZONTAL:
		if (isHEnabled())
		{
			epicsInterface->setTGTPOS(value, pvStructs.at(ScreenRecords::HTGTPOS));
		}
		else
		{
			messenger.printMessage("SCREEN NOT IN HORIZONTAL STATE -- CANNOT SET TGTPOS");
			return false;
		}
		break;
	case TYPE::VERTICAL:
		if (isHIn())
		{
			epicsInterface->setTGTPOS(value, pvStructs.at(ScreenRecords::VTGTPOS));
			return false
		}
		else
		{
			messenger.printMessage("SCREEN NOT IN VERTICAL STATE -- CANNOT SET TGTPOS");
		}
		break;
	}
	return true;
}

bool Screen::setJOG(const double& value, TYPE type)
{
	switch (type)
	{
	case TYPE::HORIZONTAL:
		if (isHEnabled())
		{
			epicsInterface->setJOG(value, pvStructs.at(ScreenRecords::HJOG));
		}
		else
		{
			messenger.printMessage("SCREEN HORIZONTAL DRIVER NOT ENABLED -- CANNOT JOG");
			return false;
		}
		break;
	case TYPE::VERTICAL:
		if (isVEnabled())
		{
			epicsInterface->setJOG(value, pvStructs.at(ScreenRecords::VJOG));
		}
		else
		{
			messenger.printMessage("SCREEN VERTICAL DRIVER NOT ENABLED -- CANNOT JOG");
			return false;
		}
		break;
	}
	return true;
}

bool Screen::setTRIGGER(const int& value, TYPE type)
{
	switch (type)
	{
	case TYPE::HORIZONTAL:
		epicsInterface->setTRIGGER(value, pvStructs.at(ScreenRecords::HTRIGGER));
		break;
	case TYPE::VERTICAL:
		epicsInterface->setTRIGGER(value, pvStructs.at(ScreenRecords::VTRIGGER));
		break;
	case TYPE::PNEUMATIC:
		epicsInterface->setTRIGGER(value, pvStructs.at(ScreenRecords::TRIGGER));
		break;
	}
	return true;
}

bool Screen::setEX(const int& value, TYPE type)
{
	switch (type)
	{
	case TYPE::HORIZONTAL:
		epicsInterface->setEX(value, pvStructs.at(ScreenRecords::HEX));
		break;
	case TYPE::VERTICAL:
		epicsInterface->setEX(value, pvStructs.at(ScreenRecords::VEX));
		break;
	case TYPE::PNEUMATIC:
		epicsInterface->setEX(value, pvStructs.at(ScreenRecords::EX));
		break;
	}
	return true;
}

bool Screen::setEN(const int& value, TYPE type)
{
	switch (type)
	{
	case TYPE::HORIZONTAL:
		epicsInterface->setEN(value, pvStructs.at(ScreenRecords::HEN));
		break;
	case TYPE::VERTICAL:
		epicsInterface->setEN(value, pvStructs.at(ScreenRecords::VEN));
		break;
	case TYPE::PNEUMATIC:
		epicsInterface->setEN(value, pvStructs.at(ScreenRecords::EN));
		break;
	}
	return true;
}

bool Screen::setDEVSTATE(int& state, TYPE type)
{
	switch (type)
	{
	case TYPE::HORIZONTAL:
		screenStateH.second = ScreenRecords::screenHElementMap.at(state);
		break;
	case TYPE::VERTICAL:
		screenStateV.second = ScreenRecords::screenVElementMap.at(state);
		break;
	case TYPE::PNEUMATIC:
		screenState.second = ScreenRecords::screenPElementMap.at(state);
		break;
	}
	return true;
}

bool Screen::setGETDEV(int& state, TYPE type)
{
	switch (type)
	{
	case TYPE::HORIZONTAL:
		screenSetStateH.second = ScreenRecords::screenHElementMap.at(state);
		break;
	case TYPE::VERTICAL:
		screenSetStateV.second = ScreenRecords::screenVElementMap.at(state);
		break;
	case TYPE::PNEUMATIC:
		screenSetState.second = ScreenRecords::screenPElementMap.at(state);
		break;
	}
	return true;
}

bool Screen::setSDEV(int& state, TYPE type)
{
	switch (type)
	{
	case TYPE::HORIZONTAL:
		epicsInterface->setSDEV(state, type);
		screenSetStateH.second = ScreenRecords::screenHElementMap.at(state);
		break;
	case TYPE::VERTICAL:
		epicsInterface->setSDEV(state, type);
		screenSetStateV.second = ScreenRecords::screenVElementMap.at(state);
		break;
	case TYPE::PNEUMATIC:
		epicsInterface->setSDEV(state, type);
		screenSetState.second = ScreenRecords::screenPElementMap.at(state);
		break;
	}
	return true;
}

int Screen::findByValue(std::map<int, STATE> mapOfElemen, STATE value)
{
	auto it = mapOfElemen.begin();
	// Iterate through the map
	while (it != mapOfElemen.end())
	{
		// Check if value of this entry matches with given value
		if (it->second == value)
		{
			return it->first;
		}
		// Go to next entry in map
		it++;
	}
	int fail = 0;
	return fail;
}

bool Screen::isElement(std::map<int, STATE> mapOfElemen, STATE value)
{
	auto it = mapOfElemen.begin();
	// Iterate through the map
	while (it != mapOfElemen.end())
	{
		// Check if value of this entry matches with given value
		if (it->second == value)
		{
			return true;
		}
		// Go to next entry in map
		it++;
	}
	return false
}