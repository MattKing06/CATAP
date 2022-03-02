#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <iostream>
#include <Screen.h>
#include <map>
#include <vector>
#include <numeric>
#include <mutex>
#include <math.h>
#include "boost/algorithm/string/split.hpp"
#include <boost/make_shared.hpp>
#include <boost/circular_buffer.hpp>
#include "GlobalFunctions.h"

Screen::Screen(){}

Screen::Screen(const std::map<std::string, std::string> & paramsMap, STATE mode) :
Hardware(paramsMap, mode),
epicsInterface (boost::make_shared<EPICSScreenInterface>(EPICSScreenInterface())),
screenType(ScreenRecords::screenTypeToEnum.at(paramsMap.at("screen_type"))),
name(paramsMap.at("name")),
position(std::stod(paramsMap.at("position"))),
camera_name(GlobalConstants::UNKNOWN),
has_camera(false),
is_busy(false),
screenDeviceVector(std::vector< STATE >()),
screenStateH(std::make_pair(epicsTimeStamp(), STATE::UNKNOWN)),
screenStateV(std::make_pair(epicsTimeStamp(), STATE::UNKNOWN)),
screenState(std::make_pair(epicsTimeStamp(), STATE::UNKNOWN)),
screenSta(std::make_pair(epicsTimeStamp(), STATE::UNKNOWN)),
screenSetStateH(std::make_pair(epicsTimeStamp(), STATE::UNKNOWN)),
screenSetStateV(std::make_pair(epicsTimeStamp(), STATE::UNKNOWN)),
screenSetState(std::make_pair(epicsTimeStamp(), STATE::UNKNOWN)),
actposH(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
actposV(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
actpos(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
tgtposH(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
tgtposV(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
tgtpos(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
devcentH(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
devcentV(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
devcent(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
jdiffH(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
jdiffV(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
jdiff(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
jogH(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
jogV(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
jog(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
enH(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
enV(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
en(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
exH(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
exV(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
ex(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
triggerH(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
triggerV(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
trigger(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
readyH(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
readyV(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
ready(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
movingH(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
movingV(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
moving(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
maxposH(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
maxposV(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min)),
maxpos(std::make_pair(epicsTimeStamp(), GlobalConstants::double_min))
{
	messenger.printDebugMessage("constructor");
	epicsInterface->ownerName = hardwareName;
	// get devices from yaml file 
	std::vector<std::string> screenDeviceStringVector;
	boost::split(screenDeviceStringVector, paramsMap.at("devices"), [](char c) {return c == ','; });
	for (auto value : screenDeviceStringVector) 
	{ 
		screenDeviceVector.push_back(ScreenRecords::screenDevicesToEnum.at(value)); 
	}
	// based on the SCREEN TYPE we know what possible PVs there will be, we don't need to do this, but 
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
	// camera_name = paramsMap.at("camera_name");
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
	else
	{ 
		messenger.printDebugMessage(hardwareName, " !!WARNING!! can't find name_alias"); 
	}

	messenger.printDebugMessage(hardwareName, " find has_camera");
	if (GlobalFunctions::entryExists(paramsMap, "has_camera"))
	{
		has_camera = GlobalFunctions::stringToBool(paramsMap.at("has_camera"));
	}
	else
	{
		messenger.printDebugMessage(hardwareName, " !!WARNING!! can't find has_camera");
	}

	messenger.printDebugMessage(hardwareName, " find camera_name");
	if (GlobalFunctions::entryExists(paramsMap, "camera_name"))
	{
		camera_name = paramsMap.at("camera_name");
	}
	else
	{
		messenger.printDebugMessage(hardwareName, " !!WARNING!! can't find camera_name");
	}
	
}

Screen::Screen(const Screen & copyScreen) :
Hardware(copyScreen),
screenType(copyScreen.screenType),
name(copyScreen.name),
position(copyScreen.position),
epicsInterface(copyScreen.epicsInterface),
camera_name(copyScreen.camera_name),
has_camera(copyScreen.has_camera),
screenDeviceVector(copyScreen.screenDeviceVector),
screenStateH(copyScreen.screenStateH),
screenStateV(copyScreen.screenStateV),
screenState(copyScreen.screenState),
screenSetStateH(copyScreen.screenSetStateH),
screenSetStateV(copyScreen.screenSetStateV),
screenSta(copyScreen.screenSta),
screenSetState(copyScreen.screenSetState),
actposH(copyScreen.actposH),
actposV(copyScreen.tgtposV),
actpos(copyScreen.tgtposV),
tgtposH(copyScreen.tgtposV),
tgtposV(copyScreen.tgtposV),
tgtpos(copyScreen.tgtposV),
devcentH(copyScreen.devcentH),
devcentV(copyScreen.devcentV),
devcent(copyScreen.devcent),
jdiffH(copyScreen.jdiffH),
jdiffV(copyScreen.jdiffV),
jdiff(copyScreen.jdiff),
jogH(copyScreen.jogH),
jogV(copyScreen.jogV),
jog(copyScreen.jog),
enH(copyScreen.enH),
enV(copyScreen.enV),
en(copyScreen.en),
exH(copyScreen.exH),
exV(copyScreen.exV),
ex(copyScreen.ex),
triggerH(copyScreen.triggerH),
triggerV(copyScreen.triggerV),
trigger(copyScreen.trigger),
readyH(copyScreen.readyH),
readyV(copyScreen.readyV),
ready(copyScreen.ready),
movingH(copyScreen.movingH),
movingV(copyScreen.movingV),
moving(copyScreen.moving),
maxposH(copyScreen.maxposH),
maxposV(copyScreen.maxposV),
maxpos(copyScreen.maxpos)
{}

void Screen::attachToInitialContext()
{
	epicsInterface->attachTo_thisCaContext();
}

void Screen::detachFromInitialContext()
{
	epicsInterface->detachFrom_thisCaContext();
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

		// TODO NO ERROR CHECKING! (we assume config file is good??? 
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

/*
///  
///   __           __        ___     __   ___ ___ ___  ___  __   __
///  /__` |  |\/| |__) |    |__     / _` |__   |   |  |__  |__) /__`
///  .__/ |  |  | |    |___ |___    \__> |___  |   |  |___ |  \ .__/
///  
///  simple getters for the main parameters 
*/
std::vector<std::string> Screen::getAliases() const { return aliases; }
boost::python::list Screen::getAliases_Py() const { return to_py_list<std::string>(getAliases()); }
std::string Screen::getScreenName() const { return name; }
double Screen::getPosition() const { return position; }
STATE Screen::getSetStateH() const{	return screenSetStateH.second;}
STATE Screen::getSetStateV() const{	return screenSetStateV.second;}
STATE Screen::getSetState()  const{	return screenSetState.second;}
STATE Screen::getStateH() const{	return screenStateH.second;}
STATE Screen::getStateV() const{	return screenStateV.second;}
STATE Screen::getState() const{	return screenState.second;}
double Screen::getDEVCENTH() const{	return devcentH.second;}
double Screen::getDEVCENTV() const{	return devcentV.second;}
double Screen::getDEVCENT() const{	return devcent.second;}
double Screen::getACTPOSH() const{	return actposH.second;}
double Screen::getACTPOSV() const{	return actposV.second;}
double Screen::getTGTPOSH() const{	return tgtposH.second;}
double Screen::getTGTPOSV() const{	return tgtposV.second;}
double Screen::getTGTPOS() const{	return tgtpos.second;}
double Screen::getJDiffH() const{	return jdiffH.second;}
double Screen::getJDiffV() const{	return jdiffV.second;}
double Screen::getJOGH() const{	return jogH.second;}
double Screen::getJOGV() const{	return jogV.second;}
int Screen::getENH() const{	return enH.second;}
int Screen::getENV() const{	return enV.second;}
int Screen::getEXH() const{	return exH.second;}
int Screen::getEXV() const{	return exV.second;}
double Screen::getTriggerH() const{	return triggerH.second;}
double Screen::getTriggerV() const{	return triggerV.second;}
double Screen::getTrigger() const{	return trigger.second;}
int Screen::getReadyH() const{	return readyH.second;}
int Screen::getReadyV() const{	return readyV.second;}
int Screen::getReady() const{	return ready.second;}
int Screen::getMovingH() const{	return movingH.second;}
int Screen::getMovingV() const{	return movingV.second;}
int Screen::getMoving() const{	return moving.second;}
double Screen::getMaxPosH() const{	return maxposH.second;}
double Screen::getMaxPosV() const{	return maxposV.second;}
double Screen::getMaxPos() const{	return maxpos.second;}
TYPE Screen::getScreenType() const{	return screenType;}
std::vector< STATE > Screen::getAvailableDevices() const{	return screenDeviceVector;}
boost::python::list Screen::getAvailableDevices_Py() const { return to_py_list<STATE>(getAvailableDevices()); }
double Screen::get_H_ACTPOS() const { if (isHEnabled()) { return actposH.second; } }
double Screen::get_V_ACTPOS() const { if (isVEnabled()) { return actposV.second; } }
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
std::pair< STATE, TYPE > Screen::getScreenStatePair(TYPE direction) const
{
	if (direction == TYPE::HORIZONTAL)
	{
		if (isHVMover())
		{
			return std::make_pair(this->screenStateH.second, direction);
		}
		else
		{
			messenger.printMessage("SCREEN IS NOT AN HV MOVER!!!!!!!");
			//return std::make_pair(STATE::ERR, TYPE::UNKNOWN_SCREEN_TYPE );
		}
	}
	else if (direction == TYPE::VERTICAL)
	{
		if (isVMover())
		{
			return std::make_pair(this->screenStateV.second, direction);
		}
		else
		{
			messenger.printMessage("SCREEN IS NOT A V MOVER!!!!!!!");
			//return std::make_pair(STATE::ERR, TYPE::UNKNOWN_SCREEN_TYPE );
		}
	}
	else if (direction == TYPE::PNEUMATIC)
	{
		if (isPneumatic())
		{
			return std::make_pair(this->screenState.second, direction);
		}
		else
		{
			messenger.printMessage("SCREEN IS NOT PNEUMATIC!!!!!!!");
			//return std::make_pair(STATE::ERR, TYPE::UNKNOWN_SCREEN_TYPE );
		}
	}
	return std::make_pair(STATE::ERR, TYPE::UNKNOWN_SCREEN_TYPE);
}
boost::python::list Screen::getScreenStatePair_Py(TYPE direction) const
{
	return to_py_list<STATE, TYPE>(getScreenStatePair(direction));
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
std::pair< STATE, TYPE > Screen::getScreenSetStatePair(TYPE direction) const
{
	if (direction == TYPE::HORIZONTAL)
	{
		if (isHVMover())
		{
			return std::make_pair(this->screenSetStateH.second, direction);
		}
		else
		{
			messenger.printMessage("SCREEN IS NOT AN HV MOVER!!!!!!!");
			return std::make_pair(STATE::ERR, TYPE::UNKNOWN_SCREEN_TYPE);
		}
	}
	else if (direction == TYPE::VERTICAL)
	{
		if (isVMover())
		{
			return std::make_pair(this->screenSetStateV.second, direction);
		}
		else
		{
			messenger.printMessage("SCREEN IS NOT A V MOVER!!!!!!!");
			return std::make_pair(STATE::ERR, TYPE::UNKNOWN_SCREEN_TYPE);
		}
	}
	else if (direction == TYPE::PNEUMATIC)
	{
		if (isPneumatic())
		{
			return std::make_pair(this->screenSetState.second, direction);
		}
		else
		{
			messenger.printMessage("SCREEN IS NOT PNEUMATIC!!!!!!!");
			return std::make_pair(STATE::ERR, TYPE::UNKNOWN_SCREEN_TYPE);
		}
	}
	else
		return std::make_pair(STATE::ERR, TYPE::UNKNOWN_SCREEN_TYPE);
}
/*
///  __   __        __    ___    __                     __        ___  __        __
/// /  ` /  \ |\ | |  \ |  |  | /  \ |\ |  /\  |       /  ` |__| |__  /  ` |__/ /__`
/// \__, \__/ | \| |__/ |  |  | \__/ | \| /~~\ |___    \__, |  | |___ \__, |  \ .__/
///
*/
bool Screen::is_HandV_OUT() const{	return isHOut() && isVOut();}
bool Screen::isScreenIn() const{	return isYAGIn();}
bool Screen::isYAGIn() const{
	if (isMover())
	{
		return screenStateV.second == STATE::VYAG;
	}
	else if (isPneumatic())
	{
		return screenState.second == STATE::YAG;
	}
	return false;
}
bool Screen::isYAGNotIn()const { return !isYAGIn(); }
bool Screen::isHMoving() const{	return movingH.second == GlobalConstants::one_us;}
bool Screen::isVMoving() const{	return movingV.second == GlobalConstants::one_us;}
bool Screen::isPMoving() const{	return moving.second == GlobalConstants::one_us;}
bool Screen::isMoving() const
{
	std::lock_guard<std::mutex> lg(epicsInterface->screen_is_moving_mtx);  // This now locked your mutex mtx.lock();
	return is_moving == true;
}
bool Screen::isNotMoving()const{	return !isMoving(); }
bool Screen::isClearForBeam() const
{
	if (!isHIn() || !isVIn() || isRFCageIn())
	{
		return true;
	}
	return false;
}
bool Screen::isHOut() const{ return !isHIn();}
bool Screen::isVOut() const{ return !isVIn();}
bool Screen::isHIn() const{	return screenStateH.second != STATE::HRETRACTED;}
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
bool Screen::isVMover() const{	return getScreenType() == TYPE::CLARA_V_MOVER;}
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
bool Screen::isPneumatic() const{ return getScreenType() == TYPE::VELA_PNEUMATIC;}
bool Screen::isVELAPneumatic() const { return getScreenType() == TYPE::VELA_PNEUMATIC; }
bool Screen::isVELAHVMover() const{ return getScreenType() == TYPE::VELA_HV_MOVER;}
bool Screen::isVELAVMover() const{ return getScreenType() == TYPE::VELA_V_MOVER;}
bool Screen::isCLARAHVMover() const{ return getScreenType() == TYPE::CLARA_HV_MOVER;}
bool Screen::isCLARAVMover() const{ return getScreenType() == TYPE::CLARA_V_MOVER ;}
bool Screen::isHElement(STATE state) const{	return GlobalFunctions::entryExists<STATE>(ScreenRecords::screenHElements, state);}
bool Screen::isVElement(STATE state) const{	return GlobalFunctions::entryExists<STATE>(ScreenRecords::screenVElements, state);}
bool Screen::isPElement(STATE state) const{	return GlobalFunctions::entryExists<STATE>(ScreenRecords::screenPElements, state);}
bool Screen::isHEnabled() const{ return enH.second == GlobalConstants::one_us;}
bool Screen::isVEnabled() const{ return enV.second == GlobalConstants::one_us;}
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
void Screen::resetPosition()
{
	if (isMover())
	{
		if (isHIn() || isHEnabled())
		{
			setScreenTriggerWDir(1, TYPE::HORIZONTAL);
		}
		else if (isVIn() || isVEnabled())
		{
			setScreenTriggerWDir(1, TYPE::VERTICAL);
		}
	}
	else
	{
		setScreenTriggerWDir(1, TYPE::PNEUMATIC);
	}
}
void Screen::updateIsmoving()
{
	std::lock_guard<std::mutex> lg(epicsInterface->screen_is_moving_mtx);  // This now locked your mutex mtx.lock();
	if (isHMoving() || isVMoving() || isPMoving())
	{
		is_moving == true;
	}
	else
	{
		is_moving == false;
	}
}



bool  Screen::insertYAG()
{
	if(isMover())
	{
		return moveScreenTo(STATE::VYAG);
	}
	else
	{
		return moveScreenTo(STATE::YAG);
	}
}
bool Screen::moveScreenOut()
{
	if (isMover())
	{
		return moveScreenTo(STATE::VRF);
	}
	else if (isPneumatic())
	{
		return moveScreenTo(STATE::RF);
	}
}
bool Screen::removeYAG() { return moveScreenOut(); }
bool Screen::moveScreenIn() {	return insertYAG();}
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
bool Screen::setScreenTriggerWDir(const double value, TYPE type){	return setTRIGGER(value, type);}
bool Screen::setScreenSDEV(STATE& state){	return setScreenSetState(state);}
bool Screen::setScreenTrigger(const double value)
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


bool Screen::moveScreenTo(STATE state)
{
	//if( isScreenInState(state))
	//{
	//	messenger.printMessage(hardwareName, " screen is already in requested state ", ENUM_TO_STRING(state));
	//	return true;
	//}
	if (!isMoving())
	{
		messenger.printMessage("moveScreenTo, setDevice to ", ENUM_TO_STRING(state));
		bool sdev = setScreenSetState(state);
		if (sdev)
		{
			if (isVElement(state))
			{
				return setScreenTriggerWDir(GlobalConstants::one_double, TYPE::VERTICAL);
			}
			else if (isHElement(state))
			{
				return setScreenTriggerWDir(GlobalConstants::one_double, TYPE::HORIZONTAL);
			}
			else if (isPElement(state))
			{
				std::cout << "isPElement" << std::endl;
				
				if (state == STATE::YAG)
				{
					epicsInterface->setIN(pvStructs.at(ScreenRecords::PNEUMATIC_ON));
				}
				else if (state == STATE::RF)
				{
					epicsInterface->setOUT(pvStructs.at(ScreenRecords::PNEUMATIC_OFF));
				}
				return setScreenTriggerWDir(GlobalConstants::one_double, TYPE::PNEUMATIC);
			}
		}
	}
	else
	{
		messenger.printMessage("!!WARNING!! SCREEN IS MOVING -- CANNOT MOVE -- please wait for current movement to finish first.");
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
	using namespace ScreenRecords;
	if (isHElement(state))
	{
		if (GlobalFunctions::entryExists(h_state_to_device_map, state))
		{
			return epicsInterface->setSDEV(h_state_to_device_map.at(state), pvStructs.at(ScreenRecords::H_SDEV));
		}
	}
	else if (isVElement(state))
	{
		if (GlobalFunctions::entryExists(v_state_to_device_map, state))
		{
			return epicsInterface->setSDEV(v_state_to_device_map.at(state), pvStructs.at(ScreenRecords::V_SDEV));
		}
	}
	else if (isPElement(state))
	{
		if (GlobalFunctions::entryExists(p_state_to_device_map, state))
		{
			return epicsInterface->setSDEV(p_state_to_device_map.at(state), pvStructs.at(ScreenRecords::SDEV));
		}
//		return setSDEV(findByValue(ScreenRecords::screenPElementMap, state), TYPE::PNEUMATIC);
	}
	return false;
}

bool Screen::setTRIGGER(const double value, TYPE type)
{
	switch (type)
	{
	case TYPE::HORIZONTAL:
	{
		epicsInterface->setTRIGGER(value, pvStructs.at(ScreenRecords::H_TRIGGER));
		//trigger.second = triggerH.second;
		break;
	}
	case TYPE::VERTICAL:
	{
		epicsInterface->setTRIGGER(value, pvStructs.at(ScreenRecords::V_TRIGGER));
		//trigger.second = triggerV.second;
		break;
	}
	case TYPE::PNEUMATIC:
		epicsInterface->setTRIGGER(value, pvStructs.at(ScreenRecords::TRIGGER));
		break;
	}
	return true;
}



// AB original code 
//bool Screen::setScreenSetState(STATE state)
//{
//	using namespace ScreenRecords;
//	if (isHElement(state))
//	{
//		 screenSetStateV.second = STATE::VRETRACTED;
//		return setSDEV(findByValue(ScreenRecords::screenHElementMap, state), TYPE::HORIZONTAL);
//	}
//	else if (isVElement(state))
//	{
//		screenSetStateH.second = STATE::HRETRACTED;
//		return setSDEV(findByValue(ScreenRecords::screenVElementMap, state), TYPE::VERTICAL);
//	}
//	else if (isPElement(state))
//	{
//		return setSDEV(findByValue(ScreenRecords::screenPElementMap, state), TYPE::PNEUMATIC);
//	}
//	return false;
//}

bool Screen::setTGTPOS(const double& value, TYPE type)
{
	switch (type)
	{
		case TYPE::HORIZONTAL:
		if (isHEnabled())
		{
			epicsInterface->setTGTPOS(value, pvStructs.at(ScreenRecords::H_TGTPOS));
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
			epicsInterface->setTGTPOS(value, pvStructs.at(ScreenRecords::V_TGTPOS));
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
			epicsInterface->setJOG(value, pvStructs.at(ScreenRecords::H_JOG));
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
			epicsInterface->setJOG(value, pvStructs.at(ScreenRecords::V_JOG));
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
	if (isNotPhysical())
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
	return false;
}

bool Screen::setMAXPOS(const double& value, TYPE type)
{
	if (isNotPhysical())
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
	return false;
}

bool Screen::setMOVING(const int& value, TYPE type)
{
	if (isNotPhysical())
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
	return false;
}

bool Screen::setEX(const int& value, TYPE type)
{
	if (isNotPhysical())
	{
		switch (type)
		{
			case TYPE::HORIZONTAL:
			{
				epicsInterface->setEX(value, pvStructs.at(ScreenRecords::H_EX));
				break;
			}
			case TYPE::VERTICAL:
			{
				epicsInterface->setEX(value, pvStructs.at(ScreenRecords::V_EX));
				break;
			}
			case TYPE::PNEUMATIC:
				epicsInterface->setEX(value, pvStructs.at(ScreenRecords::EX));
				break;
		}
		return true;
	}
	return false;
}

bool Screen::setEN(const int& value, TYPE type)
{
	if (isNotPhysical())
	{
		switch (type)
		{
			case TYPE::HORIZONTAL:
			{
				epicsInterface->setEN(value, pvStructs.at(ScreenRecords::H_EN));
				break;
			}
			case TYPE::VERTICAL:
			{
				epicsInterface->setEN(value, pvStructs.at(ScreenRecords::V_EN));
				break;
			}
			case TYPE::PNEUMATIC:
				epicsInterface->setEN(value, pvStructs.at(ScreenRecords::EN));
				break;
		}
		return true;
	}
	return false;
}

bool Screen::setDEVSTATE(int& state, TYPE type)
{
	if ( isNotPhysical() )
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
	return false;
}

bool Screen::setGETDEV(int& state, TYPE type)
{
	if (isNotPhysical())
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
	return false;
}


//bool Screen::isElement(std::map<int, STATE>& mapOfElemen, STATE value) const
//{
//	auto it = mapOfElemen.begin();
//	// Iterate through the map
//	while (it != mapOfElemen.end())
//	{
//		// Check if value of this entry matches with given value
//		if (it->second == value)
//		{
//			return true;
//		}
//		// Go to next entry in map
//		it++;
//	}
//	return false;
//}


bool Screen::getIsBusy()const
{
	return is_busy;
}
bool Screen::isBusy()const
{
	return is_busy == true;
}
bool Screen::isNotBusy()const
{
	return is_busy == false;
}

std::string Screen::getCameraName() const
{
	return camera_name;
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