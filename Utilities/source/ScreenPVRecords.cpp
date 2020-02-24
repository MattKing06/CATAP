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

// Screen positions
const std::pair< int, STATE > ScreenRecords::HRETRACTED = std::make_pair(0, STATE::HRETRACTED);
const std::pair< int, STATE > ScreenRecords::HMAX = std::make_pair(1, STATE::HMAX);
const std::pair< int, STATE > ScreenRecords::HSLIT1 = std::make_pair(7, STATE::HSLIT1);
const std::pair< int, STATE > ScreenRecords::HSLIT2 = std::make_pair(8, STATE::HSLIT2);
const std::pair< int, STATE > ScreenRecords::HSLIT3 = std::make_pair(9, STATE::HSLIT3);
const std::pair< int, STATE > ScreenRecords::HAPT1 = std::make_pair(10, STATE::HAPT1);
const std::pair< int, STATE > ScreenRecords::HAPT2 = std::make_pair(11, STATE::HAPT2);
const std::pair< int, STATE > ScreenRecords::HAPT3 = std::make_pair(0, STATE::HAPT3);
const std::pair< int, STATE > ScreenRecords::VRETRACTED = std::make_pair(0, STATE::VRETRACTED);
const std::pair< int, STATE > ScreenRecords::VMAX = std::make_pair(1, STATE::VMAX);
const std::pair< int, STATE > ScreenRecords::VRF = std::make_pair(2, STATE::VRF);
const std::pair< int, STATE > ScreenRecords::VMIRROR = std::make_pair(3, STATE::VMIRROR);
const std::pair< int, STATE > ScreenRecords::VYAG = std::make_pair(4, STATE::VYAG);
const std::pair< int, STATE > ScreenRecords::VGRAT = std::make_pair(5, STATE::VGRAT);
const std::pair< int, STATE > ScreenRecords::VCOL = std::make_pair(6, STATE::VCOL);
const std::pair< int, STATE > ScreenRecords::VSLIT1 = std::make_pair(7, STATE::VSLIT1);
const std::pair< int, STATE > ScreenRecords::RETRACTED = std::make_pair(0, STATE::RETRACTED);
const std::pair< int, STATE > ScreenRecords::YAG = std::make_pair(4, STATE::YAG);

namespace ScreenRecords
{
	std::string const HPV = ":H:";
	std::string const VPV = ":V:";
	std::vector<std::string> screenRecordList { HSDEV, HTRIGGER, HEX, HTGTPOS, HJOG, HJDIFF, HMOVING, HREADY, HGETDEV, HDEVSTATE, HMAXPOS, HDEVCENT, HACTPOS, HEN,
												  VSDEV, VTRIGGER, VEX, VTGTPOS, VJOG, VJDIFF, VMOVING, VREADY, VGETDEV, VDEVSTATE, VMAXPOS, VDEVCENT, VACTPOS, VEN,
												  SDEV,  TRIGGER,  EX,  TGTPOS,  JOG,  JDIFF,  MOVING,  READY,  GETDEV,  DEVSTATE,  MAXPOS,  DEVCENT,  ACTPOS,  EN };
	std::string e = "e";
	void insertIntoMaps()
	{
		for (auto&& it : screenRecordList)
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
		extern std::pair< int, STATE > const HRETRACTED; // = "HRETRACTED";
		extern std::pair< int, STATE > const HMAX; // = "HMAX";
		extern std::pair< int, STATE > const HSLIT1; // = "HSLIT1";
		extern std::pair< int, STATE > const HSLIT2; // = "HSLIT2";
		extern std::pair< int, STATE > const HSLIT3; // = "HSLIT3";
		extern std::pair< int, STATE > const HAPT1; // = "HAPT1";
		extern std::pair< int, STATE > const HAPT2; // = "HAPT2";
		extern std::pair< int, STATE > const HAPT3; // = "HAPT3";
		extern std::pair< int, STATE > const VRETRACTED; // = "HRETRACTED";
		extern std::pair< int, STATE > const VMAX; // = "VMAX";
		extern std::pair< int, STATE > const VRF; // = "VRF";
		extern std::pair< int, STATE > const VMIRROR; // = "VMIRROR";
		extern std::pair< int, STATE > const VYAG; // = "VYAG";
		extern std::pair< int, STATE > const VGRAT; // = "VGRAT";
		extern std::pair< int, STATE > const VCOL; // = "VCOL";
		extern std::pair< int, STATE > const VSLIT1; // = "VSLIT1";
		extern std::pair< int, STATE > const RETRACTED; // = "RETRACTED";
		extern std::pair< int, STATE > const YAG; // = "YAG";
		extern std::pair< int, STATE > const RF; // = "RF";

		std::map<std::string, STATE> screenDevicesToEnum;
		screenDevicesToEnum.insert(std::make_pair("HRETRACTED", STATE::HRETRACTED));
		screenDevicesToEnum.insert(std::make_pair("HMAX", STATE::HMAX));
		screenDevicesToEnum.insert(std::make_pair("HSLIT1", STATE::HSLIT1));
		screenDevicesToEnum.insert(std::make_pair("HSLIT2", STATE::HSLIT2));
		screenDevicesToEnum.insert(std::make_pair("HSLIT3", STATE::HSLIT3));
		screenDevicesToEnum.insert(std::make_pair("HAPT1", STATE::HAPT1));
		screenDevicesToEnum.insert(std::make_pair("HAPT2", STATE::HAPT2));
		screenDevicesToEnum.insert(std::make_pair("VRETRACTED", STATE::VRETRACTED));
		screenDevicesToEnum.insert(std::make_pair("VMAX", STATE::VMAX));
		screenDevicesToEnum.insert(std::make_pair("VRF", STATE::VRF));
		screenDevicesToEnum.insert(std::make_pair("VMIRROR", STATE::VMIRROR));
		screenDevicesToEnum.insert(std::make_pair("VYAG", STATE::VYAG));
		screenDevicesToEnum.insert(std::make_pair("VGRAT", STATE::VGRAT));
		screenDevicesToEnum.insert(std::make_pair("VCOL", STATE::VCOL));
		screenDevicesToEnum.insert(std::make_pair("VSLIT1", STATE::VSLIT1));
		screenDevicesToEnum.insert(std::make_pair("RETRACTED", STATE::RETRACTED));
		screenDevicesToEnum.insert(std::make_pair("YAG", STATE::YAG));

		std::map<std::string, TYPE> screenTypeToEnum;
		screenTypeToEnum.insert(std::make_pair("CLARA_HV_MOVER", TYPE::CLARA_HV_MOVER));
		screenTypeToEnum.insert(std::make_pair("CLARA_V_MOVER", TYPE::CLARA_V_MOVER));
		screenTypeToEnum.insert(std::make_pair("CLARA_PNEUMATIC", TYPE::CLARA_PNEUMATIC));
		screenTypeToEnum.insert(std::make_pair("VELA_HV_MOVER", TYPE::VELA_HV_MOVER));
		screenTypeToEnum.insert(std::make_pair("VELA_PNEUMATIC", TYPE::VELA_PNEUMATIC));

		std::map<int, STATE> screenHElementMap;
		std::map<int, STATE> screenVElementMap;
		std::map<int, STATE> screenPElementMap;
		screenHElementMap.insert(HRETRACTED);
		screenHElementMap.insert(HMAX);
		screenHElementMap.insert(HSLIT1);
		screenHElementMap.insert(HSLIT2);
		screenHElementMap.insert(HSLIT3);
		screenHElementMap.insert(HAPT1);
		screenHElementMap.insert(HAPT2);
		screenHElementMap.insert(HAPT3);
		screenVElementMap.insert(VRETRACTED);
		screenVElementMap.insert(VMAX);
		screenVElementMap.insert(VRF);
		screenVElementMap.insert(VMIRROR);
		screenVElementMap.insert(VYAG);
		screenVElementMap.insert(VGRAT);
		screenVElementMap.insert(VCOL);
		screenVElementMap.insert(VSLIT1);
		screenPElementMap.insert(RETRACTED);
		screenPElementMap.insert(YAG);
	}
}

