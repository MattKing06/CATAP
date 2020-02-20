#include "ScreenPVRecords.h"

// Screen PV types
std::string const ScreenRecords::HSDEV = "H:SDEV";
std::string const ScreenRecords::HTRIGGER = "H:TRIGGER";
std::string const ScreenRecords::HEX = "H:EX";
std::string const ScreenRecords::HTGTPOS = "H:TGTPOS";
std::string const ScreenRecords::HJOG = "H:JOG";
std::string const ScreenRecords::HJDIFF = "H:JDIFF";
std::string const ScreenRecords::HMOVING = "H:MOVING";
std::string const ScreenRecords::HREADY = "H:READY";
std::string const ScreenRecords::HGETDEV = "H:GET_DEV";
std::string const ScreenRecords::HDEVSTATE = "H:DEV_STATE";
std::string const ScreenRecords::HMAXPOS = "H:MAX_POS";
std::string const ScreenRecords::HDEVCENT = "H:DEV_CENT";
std::string const ScreenRecords::HACTPOS = "H:ACTPOS";
std::string const ScreenRecords::HEN = "H:EN";
std::string const ScreenRecords::VSDEV = "V:SDEV";
std::string const ScreenRecords::VTRIGGER = "V:TRIGGER";
std::string const ScreenRecords::VEX = "V:EX";
std::string const ScreenRecords::VTGTPOS = "V:TGTPOS";
std::string const ScreenRecords::VJOG = "V:JOG";
std::string const ScreenRecords::VJDIFF = "V:JDIFF";
std::string const ScreenRecords::VMOVING = "V:MOVING";
std::string const ScreenRecords::VREADY = "V:READY";
std::string const ScreenRecords::VGETDEV = "V:GET_DEV";
std::string const ScreenRecords::VDEVSTATE = "V:DEV_STATE";
std::string const ScreenRecords::VMAXPOS = "V:MAX_POS";
std::string const ScreenRecords::VDEVCENT = "V:DEV_CENT";
std::string const ScreenRecords::VACTPOS = "V:ACTPOS";
std::string const ScreenRecords::VEN = "V:EN";
std::string const ScreenRecords::SDEV = "SDEV";
std::string const ScreenRecords::TRIGGER = "TRIGGER";
std::string const ScreenRecords::EX = "EX";
std::string const ScreenRecords::TGTPOS = "TGTPOS";
std::string const ScreenRecords::JOG = "JOG";
std::string const ScreenRecords::JDIFF = "JDIFF";
std::string const ScreenRecords::MOVING = "MOVING";
std::string const ScreenRecords::READY = "READY";
std::string const ScreenRecords::GETDEV = "GET_DEV";
std::string const ScreenRecords::DEVSTATE = "DEV_STATE";
std::string const ScreenRecords::MAXPOS = "MAX_POS";
std::string const ScreenRecords::DEVCENT = "DEV_CENT";
std::string const ScreenRecords::ACTPOS = "ACTPOS";
std::string const ScreenRecords::EN = "EN";
std::string const ScreenRecords::HPV = ":H:";
std::string const ScreenRecords::VPV = ":V:";

// Screen positions
const std::pair< std::pair< enum, STATE >, TYPE > ScreenRecords::HRETRACTED = std::make_pair(std::make_pair(0, STATE::HRETRACTED), TYPE::HORIZONTAL);
const std::pair< enum, TYPE > ScreenRecords::HMAX = std::make_pair(std::make_pair(0, STATE::HMAX), TYPE::HORIZONTAL);
const std::pair< enum, TYPE > ScreenRecords::HSLIT1 = std::make_pair(7, TYPE::HORIZONTAL);
const std::pair< enum, TYPE > ScreenRecords::HSLIT2 = std::make_pair(8, TYPE::HORIZONTAL);
const std::pair< enum, TYPE > ScreenRecords::HSLIT3 = std::make_pair(9, TYPE::HORIZONTAL);
const std::pair< enum, TYPE > ScreenRecords::HAPT1 = std::make_pair(10, TYPE::HORIZONTAL);
const std::pair< enum, TYPE > ScreenRecords::HAPT2 = std::make_pair(11, TYPE::HORIZONTAL);
const std::pair< enum, TYPE > ScreenRecords::HAPT3 = std::make_pair(0, TYPE::HORIZONTAL);
const std::pair< enum, TYPE > ScreenRecords::VRETRACTED = std::make_pair(0, TYPE::VERTICAL);
const std::pair< enum, TYPE > ScreenRecords::VMAX = std::make_pair(1, TYPE::VERTICAL);
const std::pair< enum, TYPE > ScreenRecords::VRF = std::make_pair(2, TYPE::VERTICAL);
const std::pair< enum, TYPE > ScreenRecords::VMIRROR = std::make_pair(3, TYPE::VERTICAL);
const std::pair< enum, TYPE > ScreenRecords::VYAG = std::make_pair(4, TYPE::VERTICAL);
const std::pair< enum, TYPE > ScreenRecords::VGRAT = std::make_pair(5, TYPE::VERTICAL);
const std::pair< enum, TYPE > ScreenRecords::VCOL = std::make_pair(6, TYPE::VERTICAL);
const std::pair< enum, TYPE > ScreenRecords::VSLIT1 = std::make_pair(7, TYPE::VERTICAL);
const std::pair< enum, TYPE > ScreenRecords::RETRACTED = std::make_pair(0, TYPE::PNEUMATIC);
const std::pair< enum, TYPE > ScreenRecords::YAG = std::make_pair(4, TYPE::PNEUMATIC);

namespace ScreenRecords
{
	std::vector<std::string> screenRecordList = { HSDEV, HTRIGGER, HEX, HTGTPOS, HJOG, HJDIFF, HMOVING, HREADY, HGETDEV, HDEVSTATE, HMAXPOS, HDEVCENT, HACTPOS, HEN,
												  VSDEV, VTRIGGER, VEX, VTGTPOS, VJOG, VJDIFF, VMOVING, VREADY, VGETDEV, VDEVSTATE, VMAXPOS, VDEVCENT, VACTPOS, VEN,
												  SDEV,  TRIGGER,  EX,  TGTPOS,  JOG,  JDIFF,  MOVING,  READY,  GETDEV,  DEVSTATE,  MAXPOS,  DEVCENT,  ACTPOS,  EN };
	std::map<std::string, TYPE> screenDirectionList;
	for(auto& it : screenRecordList)
	{
		if (it.rfind("H", 0) == 0)
		{
			std::pair<std::string, TYPE> recordAndDirectionPair = std::make_pair(it, TYPE::HORIZONTAL);
			screenDirectionList.insert(recordAndDirectionPair);
		}
		else if (it.rfind("V", 0) == 0)
		{
			std::pair<std::string, TYPE> recordAndDirectionPair = std::make_pair(it, TYPE::VERTICAL);
			screenDirectionList.insert(recordAndDirectionPair);
		}
		else
		{
			std::pair<std::string, TYPE> recordAndDirectionPair = std::make_pair(it, TYPE::PNEUMATIC);
			screenDirectionList.insert(recordAndDirectionPair);
		}
	}
	const std::pair< enum, TYPE > ScreenRecords::HRETRACTED = std::make_pair(0, TYPE::HORIZONTAL);
	const std::pair< enum, TYPE > ScreenRecords::HMAX = std::make_pair(1, TYPE::HORIZONTAL);
	const std::pair< enum, TYPE > ScreenRecords::HSLIT1 = std::make_pair(7, TYPE::HORIZONTAL);
	const std::pair< enum, TYPE > ScreenRecords::HSLIT2 = std::make_pair(8, TYPE::HORIZONTAL);
	const std::pair< enum, TYPE > ScreenRecords::HSLIT3 = std::make_pair(9, TYPE::HORIZONTAL);
	const std::pair< enum, TYPE > ScreenRecords::HAPT1 = std::make_pair(10, TYPE::HORIZONTAL);
	const std::pair< enum, TYPE > ScreenRecords::HAPT2 = std::make_pair(11, TYPE::HORIZONTAL);
	const std::pair< enum, TYPE > ScreenRecords::HAPT3 = std::make_pair(0, TYPE::HORIZONTAL);
	const std::pair< enum, TYPE > ScreenRecords::VRETRACTED = std::make_pair(0, TYPE::VERTICAL);
	const std::pair< enum, TYPE > ScreenRecords::VMAX = std::make_pair(1, TYPE::VERTICAL);
	const std::pair< enum, TYPE > ScreenRecords::VRF = std::make_pair(2, TYPE::VERTICAL);
	const std::pair< enum, TYPE > ScreenRecords::VMIRROR = std::make_pair(3, TYPE::VERTICAL);
	const std::pair< enum, TYPE > ScreenRecords::VYAG = std::make_pair(4, TYPE::VERTICAL);
	const std::pair< enum, TYPE > ScreenRecords::VGRAT = std::make_pair(5, TYPE::VERTICAL);
	const std::pair< enum, TYPE > ScreenRecords::VCOL = std::make_pair(6, TYPE::VERTICAL);
	const std::pair< enum, TYPE > ScreenRecords::VSLIT1 = std::make_pair(7, TYPE::VERTICAL);
	const std::pair< enum, TYPE > ScreenRecords::RETRACTED = std::make_pair(0, TYPE::PNEUMATIC);
	const std::pair< enum, TYPE > ScreenRecords::YAG = std::make_pair(4, TYPE::PNEUMATIC);
	std::map<enum, TYPE> screenElementMap;
	screenElementMap.insert(HRETRACTED);
	screenElementMap.insert(HMAX);
	screenElementMap.insert(HSLIT1);
	screenElementMap.insert(HSLIT2);
	screenElementMap.insert(HSLIT3);
	screenElementMap.insert(HAPT1);
	screenElementMap.insert(HAPT2);
	screenElementMap.insert(HAPT3);
	screenElementMap.insert(VRETRACTED);
	screenElementMap.insert(VMAX);
	screenElementMap.insert(VRF);
	screenElementMap.insert(VMIRROR);
	screenElementMap.insert(VYAG);
	screenElementMap.insert(VGRAT);
	screenElementMap.insert(VCOL);
	screenElementMap.insert(VSLIT1);
	screenElementMap.insert(RETRACTED);
	screenElementMap.insert(YAG);
}

