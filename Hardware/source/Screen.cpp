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
#include "GlobalFunctions.h"

Screen::Screen()
{}
Screen::Screen(const std::map<std::string, std::string> & paramsMap, STATE mode) :
Hardware(paramsMap, mode),
screenType(ScreenRecords::screenTypeToEnum.at(paramsMap.at("screen_type"))),
name(paramsMap.at("name")),
position(std::stod(paramsMap.at("position")))
{
	messenger.printDebugMessage("constructor");
	switch (screenType)
	{
	case TYPE::CLARA_HV_MOVER:
	{
		setPVStructs(ScreenRecords::screenHVRecordList);
		break;
	}
	case TYPE::CLARA_V_MOVER:
	{
		setPVStructs(ScreenRecords::screenVRecordList);
		break;
	}
	case TYPE::VELA_HV_MOVER:
	{
		setPVStructs(ScreenRecords::screenHVRecordList);
		break;
	}
	case TYPE::VELA_V_MOVER:
	{
		setPVStructs(ScreenRecords::screenVRecordList);
		break;
	}
	case TYPE::CLARA_PNEUMATIC:
	{
		setPVStructs(ScreenRecords::screenPRecordList);
		break;
	}
	case TYPE::VELA_PNEUMATIC:
	{
		setPVStructs(ScreenRecords::screenPRecordList);
		break;
	}
}
epicsInterface = boost::make_shared<EPICSScreenInterface>(EPICSScreenInterface());
epicsInterface->ownerName = hardwareName;
std::vector<std::string> screenDeviceStringVector;
boost::split(screenDeviceStringVector, paramsMap.at("devices"), [](char c) {return c == ','; });
for (auto value : screenDeviceStringVector) { screenDeviceVector.push_back(ScreenRecords::screenDevicesToEnum.at(value)); }
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
en.second = STATE::UNKNOWN;
exH.second = GlobalConstants::zero_int;
exV.second = GlobalConstants::zero_int;
ex.second = GlobalConstants::zero_int;
triggerH.second = GlobalConstants::zero_int;
triggerV.second = GlobalConstants::zero_int;
trigger.second = GlobalConstants::zero_int;
readyH.second = GlobalConstants::zero_int;
readyV.second = GlobalConstants::zero_int;
ready.second = GlobalConstants::zero_int;
movingH.second = GlobalConstants::zero_int;
movingV.second = GlobalConstants::zero_int;
moving.second = GlobalConstants::zero_int;
maxposH.second = GlobalConstants::double_min;
maxposV.second = GlobalConstants::double_min;
maxpos.second = GlobalConstants::double_min;


messenger.printDebugMessage(hardwareName, " find name_alias");
if (GlobalFunctions::entryExists(paramsMap, "name_alias"))
{
	boost::split(aliases, paramsMap.at("name_alias"), [](char c) {return c == ','; });
	for (auto& name : aliases)
	{
		name.erase(std::remove_if(name.begin(), name.end(), isspace), name.end());
		messenger.printDebugMessage(hardwareName, " added aliase " + name);
	}
}
else { messenger.printDebugMessage(hardwareName, " !!WARNING!!"); }

}



Screen::Screen(const Screen & copyScreen) :
Hardware(copyScreen),
screenType(copyScreen.screenType),
name(copyScreen.name),
//has_camera(copyScreen.has_camera),
position(copyScreen.position),
epicsInterface(copyScreen.epicsInterface)
{
}

void Screen::setPVStructs(std::vector<std::string> recordList)
{
	messenger.printDebugMessage("in setPVstructs");
	for (auto&& record : recordList)
	{
		messenger.printDebugMessage("in loop");
		pvStructs[record] = pvStruct();
		messenger.printDebugMessage(record);
		pvStructs[record].pvRecord = record;

		// TODO NO ERROR CHECKING! (we assum config file is good??? 
		std::string PV = specificHardwareParameters.at(record).data();
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
#ifdef BUILD_PYTHON
boost::python::list Screen::getAliases_Py() const
{
	return to_py_list<std::string>(getAliases());
}
#endif //BUILD_PYTHON


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

std::pair< STATE, TYPE > Screen::getScreenStatePair(TYPE dir) const
{
	if (dir == TYPE::HORIZONTAL)
	{
		if (isHVMover())
		{
			return std::make_pair( this->screenStateH.second, dir );
		}
		else
		{
			messenger.printMessage("SCREEN IS NOT AN HV MOVER!!!!!!!");
			return std::make_pair(STATE::ERR, TYPE::UNKNOWN_SCREEN_TYPE );
		}
	}
	else if (dir == TYPE::VERTICAL)
	{
		if (isVMover())
		{
			return std::make_pair( this->screenStateV.second, dir );
		}
		else
		{
			messenger.printMessage("SCREEN IS NOT A V MOVER!!!!!!!");
			return std::make_pair(STATE::ERR, TYPE::UNKNOWN_SCREEN_TYPE );
		}
	}
	else if (dir == TYPE::PNEUMATIC)
	{
		if (isPneumatic())
		{
			return std::make_pair( this->screenState.second, dir );
		}
		else
		{
			messenger.printMessage("SCREEN IS NOT PNEUMATIC!!!!!!!");
			return std::make_pair(STATE::ERR, TYPE::UNKNOWN_SCREEN_TYPE );
		}
	}
	else
		return std::make_pair(STATE::ERR, TYPE::UNKNOWN_SCREEN_TYPE );
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

std::pair< STATE, TYPE > Screen::getScreenSetStatePair(TYPE dir) const
{
	if (dir == TYPE::HORIZONTAL)
	{
		if (isHVMover())
		{
			return std::make_pair( this->screenSetStateH.second, dir );
		}
		else
		{
			messenger.printMessage("SCREEN IS NOT AN HV MOVER!!!!!!!");
			return std::make_pair(STATE::ERR, TYPE::UNKNOWN_SCREEN_TYPE );
		}
	}
	else if (dir == TYPE::VERTICAL)
	{
		if (isVMover())
		{
			return std::make_pair( this->screenSetStateV.second, dir );
		}
		else
		{
			messenger.printMessage("SCREEN IS NOT A V MOVER!!!!!!!");
			return std::make_pair(STATE::ERR, TYPE::UNKNOWN_SCREEN_TYPE );
		}
	}
	else if (dir == TYPE::PNEUMATIC)
	{
		if (isPneumatic())
		{
			return std::make_pair( this->screenSetState.second, dir );
		}
		else
		{
			messenger.printMessage("SCREEN IS NOT PNEUMATIC!!!!!!!");
			return std::make_pair(STATE::ERR, TYPE::UNKNOWN_SCREEN_TYPE );
		}
	}
	else
		return std::make_pair( STATE::ERR, TYPE::UNKNOWN_SCREEN_TYPE );
}

STATE Screen::getSetStateH() const
{
	return screenSetStateH.second;
}

STATE Screen::getSetStateV() const
{
	return screenSetStateV.second;
}

STATE Screen::getSetState() const
{
	return screenSetState.second;
}

STATE Screen::getStateH() const
{
	return screenStateH.second;
}

STATE Screen::getStateV() const
{
	return screenStateV.second;
}

STATE Screen::getState() const
{
	return screenState.second;
}

double Screen::getDEVCENTH() const
{
	return devcentH.second;
}

double Screen::getDEVCENTV() const
{
	return devcentV.second;
}

double Screen::getDEVCENT() const
{
	return devcent.second;
}

double Screen::getACTPOSH() const
{
	return actposH.second;
}

double Screen::getACTPOSV() const
{
	return actposV.second;
}

double Screen::getTGTPOSH() const
{
	return tgtposH.second;
}

double Screen::getTGTPOSV() const
{
	return tgtposV.second;
}

double Screen::getTGTPOS() const
{
	return tgtpos.second;
}

double Screen::getJDiffH() const
{
	return jdiffH.second;
}

double Screen::getJDiffV() const
{
	return jdiffV.second;
}

double Screen::getJOGH() const
{
	return jogH.second;
}

double Screen::getJOGV() const
{
	return jogV.second;
}

int Screen::getENH() const
{
	return enH.second;
}

int Screen::getENV() const
{
	return enV.second;
}

int Screen::getEXH() const
{
	return exH.second;
}

int Screen::getEXV() const
{
	return exV.second;
}

int Screen::getTriggerH() const
{
	return triggerH.second;
}

int Screen::getTriggerV() const
{
	return triggerV.second;
}

int Screen::getTrigger() const
{
	return trigger.second;
}

int Screen::getReadyH() const
{
	return readyH.second;
}

int Screen::getReadyV() const
{
	return readyV.second;
}

int Screen::getReady() const
{
	return ready.second;
}

int Screen::getMovingH() const
{
	return movingH.second;
}

int Screen::getMovingV() const
{
	return movingV.second;
}

int Screen::getMoving() const
{
	return moving.second;
}

double Screen::getMaxPosH() const
{
	return maxposH.second;
}

double Screen::getMaxPosV() const
{
	return maxposV.second;
}

double Screen::getMaxPos() const
{
	return maxpos.second;
}

TYPE Screen::getScreenType() const
{
	return this->screenType;
}

std::vector< STATE > Screen::getAvailableDevices() const
{
	return screenDeviceVector;
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
	if (screenStateH.second != STATE::HRETRACTED)
	{
		return true;
	}
	return false;
}

bool Screen::isVIn() const
{
	if (screenStateV.second != STATE::VRETRACTED || screenStateV.second != STATE::VRF)
	{
		return true;
	}
	return false;
}

bool Screen::isRFCageIn() const
{
	if (screenStateV.second == STATE::VRF || screenState.second == STATE::RF)
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
	if (getScreenType() == TYPE::CLARA_HV_MOVER)
	{
		return true;
	}
	else if (getScreenType() == TYPE::VELA_HV_MOVER)
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

bool Screen::isVELAHVMover() const
{
	if (getScreenType() == TYPE::VELA_HV_MOVER)
	{
		return true;
	}
	return false;
}

bool Screen::isVELAVMover() const
{
	if (getScreenType() == TYPE::VELA_V_MOVER)
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
	if (isHElement(ScreenRecords::screenHElementMap.at(sta)))
	{
		return screenStateH.second == sta;
	}
	else if (isVElement(ScreenRecords::screenVElementMap.at(sta)))
	{
		return screenStateV.second == sta;
	}
	else if (isPElement(ScreenRecords::screenPElementMap.at(sta)))
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
		moveScreenTo(STATE::VYAG);
	}
	else
	{
		moveScreenTo(STATE::YAG);
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
	if (isMover())
	{
		moveScreenTo(STATE::VRF);
	}
	else if (isPneumatic())
	{
		moveScreenTo(STATE::RF);
	}
}

void Screen::resetPosition()
{
	if (isMover())
	{
		if (isHIn() || isHEnabled())
		{
			setScreenTriggerWDir(1, TYPE::HORIZONTAL);
		}
		else if(isVIn() || isVEnabled())
		{
			setScreenTriggerWDir(1, TYPE::VERTICAL);
		}
	}
	else
	{
		setScreenTriggerWDir(1, TYPE::PNEUMATIC);
	}
}

bool Screen::jogScreen(const double& value)
{
	if (isHEnabled() || isHIn())
	{
		bool jog = setJOG(value, TYPE::HORIZONTAL);
		if (jog)
		{
			return setScreenTriggerWDir(1, TYPE::VERTICAL);
		}
		else
		{
			return false;
		}
	}
	else if (isVEnabled() || isVIn())
	{
		bool jog = setJOG(value, TYPE::VERTICAL);
		if (jog)
		{
			return setScreenTriggerWDir(1, TYPE::VERTICAL);
		}
		else
		{
			return false;
		}
	}
	else
	{
		if (isPneumatic())
		{
			messenger.printMessage("Screen is pneumatic -- cannot jog");
			return false;
		}
		messenger.printMessage("Screen is not inserted -- cannot jog");
		return false;
	}
}

bool Screen::setScreenSDEV(STATE& state)
{
	return setScreenSetState(state);
}

bool Screen::setScreenTrigger(const int& value)
{
	if (isMover())
	{
		if (isHIn() || isHEnabled())
		{
			return setTRIGGER(value, TYPE::HORIZONTAL);
		}
		else if (isVIn() || isVEnabled())
		{
			return setTRIGGER(value, TYPE::VERTICAL);
		}
		else
		{
			messenger.printMessage("Neither H nor V is in -- specify a direction");
			return false;
		}
	}
	else if (isPneumatic())
	{
		return setTRIGGER(value, TYPE::PNEUMATIC);
	}
}

bool Screen::setScreenTriggerWDir(const int& value, TYPE type)
{
	return setTRIGGER(value, type);
}

bool Screen::moveScreenTo(STATE state)
{
	if (!isMoving())
	{
		bool sdev = setScreenSetState(state);
		if (sdev)
		{
			if (isVElement(state))
			{
				return setScreenTriggerWDir(1, TYPE::VERTICAL);
			}
			else if (isHElement(state))
			{
				return setScreenTriggerWDir(1, TYPE::HORIZONTAL);
			}
			else if (isPElement(state))
			{
				std::cout << "isPElement" << std::endl;
				return setScreenTriggerWDir(1, TYPE::PNEUMATIC);
			}
		}
	}
	else
	{
		messenger.printMessage("SCREEN IS MOVING -- CANNOT MOVE");
	}
	return false;
}

bool Screen::setPosition(const double& value, TYPE type)
{
	bool tgt = setTGTPOS(value, type);
	if (tgt)
	{
		return setEX(1, type);
	}
	return false;
}

bool Screen::setScreenSetState(STATE state)
{
	if (isHElement(state))
	{
		screenSetStateV.second = STATE::VRETRACTED;
		return setSDEV(findByValue(ScreenRecords::screenHElementMap, state), TYPE::HORIZONTAL);
	}
	else if (isVElement(state))
	{
		screenSetStateH.second = STATE::HRETRACTED;
		return setSDEV(findByValue(ScreenRecords::screenVElementMap, state), TYPE::VERTICAL);
	}
	else if (isPElement(state))
	{
		return setSDEV(findByValue(ScreenRecords::screenPElementMap, state), TYPE::PNEUMATIC);
	}
	return false;
}

bool Screen::setTGTPOS(const double& value, TYPE type)
{
	switch (type)
	{
	case TYPE::HORIZONTAL:
		if (isHEnabled())
		{
			epicsInterface->setTGTPOS(value, pvStructs.at(ScreenRecords::HTGTPOS));
			tgtpos.second = tgtposH.second;
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
			tgtpos.second = tgtposV.second;
		}
		else
		{
			messenger.printMessage("SCREEN NOT IN VERTICAL STATE -- CANNOT SET TGTPOS");
			return false;
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
			jogH.second = value;
			jog.second = jogH.second;
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
			jogV.second = value;
			jog.second = jogV.second;
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

bool Screen::setREADY(const int& value, TYPE type)
{
	switch (type)
	{
	case TYPE::HORIZONTAL:
	{
		readyH.second = value;
		ready.second = readyH.second;
		break;
	}
	case TYPE::VERTICAL:
	{
		readyV.second = value;
		ready.second = readyV.second;
		break;
	}
	case TYPE::PNEUMATIC:
	{
		ready.second = value;
		break;
	}
	}
	return true;
}

bool Screen::setMAXPOS(const double& value, TYPE type)
{
	switch (type)
	{
	case TYPE::HORIZONTAL:
	{
		maxposH.second = value;
		maxpos.second = value;
		break;
	}
	case TYPE::VERTICAL:
	{
		maxposV.second = value;
		maxpos.second = value;
		break;
	}
	case TYPE::PNEUMATIC:
	{
		maxpos.second = value;
		break;
	}
	}
	return true;
}

bool Screen::setMOVING(const int& value, TYPE type)
{
	switch (type)
	{
	case TYPE::HORIZONTAL:
	{
		movingH.second = value;
		moving.second = value;
		break;
	}
	case TYPE::VERTICAL:
	{
		movingV.second = value;
		moving.second = value;
		break;
	}
	case TYPE::PNEUMATIC:
		moving.second = value;
		break;
	}
	return true;
}

bool Screen::setTRIGGER(const int& value, TYPE type)
{
	switch (type)
	{
	case TYPE::HORIZONTAL:
	{
		epicsInterface->setTRIGGER(value, pvStructs.at(ScreenRecords::HTRIGGER));
		trigger.second = triggerH.second;
		break;
	}
	case TYPE::VERTICAL:
	{
		epicsInterface->setTRIGGER(value, pvStructs.at(ScreenRecords::VTRIGGER));
		trigger.second = triggerV.second;
		break;
	}
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
	{
		epicsInterface->setEX(value, pvStructs.at(ScreenRecords::HEX));
		break;
	}
	case TYPE::VERTICAL:
	{
		epicsInterface->setEX(value, pvStructs.at(ScreenRecords::VEX));
		break;
	}
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
	{
		epicsInterface->setEN(value, pvStructs.at(ScreenRecords::HEN));
		break;
	}
	case TYPE::VERTICAL:
	{
		epicsInterface->setEN(value, pvStructs.at(ScreenRecords::VEN));
		break;
	}
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
	{
		screenStateH.second = ScreenRecords::screenHElementMap.at(state);
		screenState.second = screenStateH.second;
		break;
	}
	case TYPE::VERTICAL:
	{
		screenStateV.second = ScreenRecords::screenVElementMap.at(state);
		screenState.second = screenStateV.second;
		break;
	}
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
	{
		screenSetStateH.second = ScreenRecords::screenHElementMap.at(state);
		screenSetState.second = screenSetStateH.second;
		break;
	}
	case TYPE::VERTICAL:
	{
		screenSetStateV.second = ScreenRecords::screenVElementMap.at(state);
		screenSetState.second = screenSetStateV.second;
		break;
	}
	case TYPE::PNEUMATIC:
		screenSetState.second = ScreenRecords::screenPElementMap.at(state);
		break;
	}
	return true;
}

bool Screen::setSDEV(int state, TYPE type)
{
	switch (type)
	{
	case TYPE::HORIZONTAL:
	{
		epicsInterface->setSDEV(state, pvStructs.at(ScreenRecords::HSDEV));
		screenSetStateH.second = ScreenRecords::screenHElementMap.at(state);
		screenSetState.second = screenSetStateH.second;
		return true;
	}
	case TYPE::VERTICAL:
	{
		epicsInterface->setSDEV(state, pvStructs.at(ScreenRecords::VSDEV));
		screenSetStateV.second = ScreenRecords::screenVElementMap.at(state);
		screenSetState.second = screenSetStateV.second;
		return true;
	}
	case TYPE::PNEUMATIC:
	{
		epicsInterface->setSDEV(state, pvStructs.at(ScreenRecords::SDEV));
		screenSetState.second = ScreenRecords::screenPElementMap.at(state);
		return true;
	}
	}
	return false;
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

bool Screen::isElement(std::map<int, STATE> mapOfElemen, STATE value) const
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
	return false;
}

void Screen::debugMessagesOff()
{
	messenger.printDebugMessage(hardwareName, " - DEBUG OFF");
	messenger.debugMessagesOff();
	epicsInterface->debugMessagesOff();
}

void Screen::debugMessagesOn()
{
	messenger.debugMessagesOn();
	messenger.printDebugMessage(hardwareName, " - DEBUG On");
	epicsInterface->debugMessagesOn();
}

void Screen::messagesOff()
{
	messenger.printMessage(hardwareName, " - MESSAGES OFF");
	messenger.messagesOff();
	epicsInterface->messagesOff();
}

void Screen::messagesOn()
{
	messenger.messagesOn();
	messenger.printMessage(hardwareName, " - MESSAGES On");
	epicsInterface->messagesOn();
}