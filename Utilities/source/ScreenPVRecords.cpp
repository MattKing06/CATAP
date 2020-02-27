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
const std::pair< int, STATE > ScreenRecords::HAPT3 = std::make_pair(12, STATE::HAPT3);
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
	std::vector<std::string> screenRecordList{ HSDEV, HTRIGGER, HEX, HTGTPOS, HJOG, HJDIFF, HMOVING, HREADY, HGETDEV, HDEVSTATE, HMAXPOS, HDEVCENT, HACTPOS, HEN,
												  VSDEV, VTRIGGER, VEX, VTGTPOS, VJOG, VJDIFF, VMOVING, VREADY, VGETDEV, VDEVSTATE, VMAXPOS, VDEVCENT, VACTPOS, VEN,
												  SDEV,  TRIGGER,  EX,  TGTPOS,  JOG,  JDIFF,  MOVING,  READY,  GETDEV,  DEVSTATE,  MAXPOS,  DEVCENT,  ACTPOS,  EN };
	std::vector<std::string> screenHVRecordList{ HSDEV, HTRIGGER, HEX, HTGTPOS, HJOG, HJDIFF, HMOVING, /*HREADY,*/ HGETDEV, HDEVSTATE, HMAXPOS, HDEVCENT, HACTPOS, HEN,
												  VSDEV, VTRIGGER, VEX, VTGTPOS, VJOG, VJDIFF, VMOVING, /*VREADY,*/ VGETDEV, VDEVSTATE, VMAXPOS, VDEVCENT, VACTPOS, VEN };
	std::vector<std::string> screenVRecordList{ VSDEV, VTRIGGER, VEX, VTGTPOS, VJOG, VJDIFF, VMOVING, VREADY, VGETDEV, VDEVSTATE, VMAXPOS, VDEVCENT, VACTPOS, VEN };
	std::vector<std::string> screenPRecordList{ SDEV,  TRIGGER,  EX,  TGTPOS,  JOG,  JDIFF,  MOVING,  READY,  GETDEV,  DEVSTATE,  MAXPOS,  DEVCENT,  ACTPOS,  EN };

	std::map<std::string, TYPE> screenDirectionList{ 
		std::pair<std::string, TYPE>(HSDEV,TYPE::HORIZONTAL),
		std::pair<std::string, TYPE>(HTRIGGER,TYPE::HORIZONTAL),
		std::pair<std::string, TYPE>(HEX,TYPE::HORIZONTAL),
		std::pair<std::string, TYPE>(HTGTPOS,TYPE::HORIZONTAL),
		std::pair<std::string, TYPE>(HJOG,TYPE::HORIZONTAL),
		std::pair<std::string, TYPE>(HJDIFF,TYPE::HORIZONTAL),
		std::pair<std::string, TYPE>(HMOVING,TYPE::HORIZONTAL),
		std::pair<std::string, TYPE>(HGETDEV,TYPE::HORIZONTAL),
		std::pair<std::string, TYPE>(HDEVSTATE,TYPE::HORIZONTAL),
		std::pair<std::string, TYPE>(HMAXPOS,TYPE::HORIZONTAL),
		std::pair<std::string, TYPE>(HDEVCENT,TYPE::HORIZONTAL),
		std::pair<std::string, TYPE>(HACTPOS,TYPE::HORIZONTAL),
		std::pair<std::string, TYPE>(HEN,TYPE::HORIZONTAL),
		std::pair<std::string, TYPE>(VSDEV,TYPE::VERTICAL),
		std::pair<std::string, TYPE>(VTRIGGER,TYPE::VERTICAL),
		std::pair<std::string, TYPE>(VEX,TYPE::VERTICAL),
		std::pair<std::string, TYPE>(VTGTPOS,TYPE::VERTICAL),
		std::pair<std::string, TYPE>(VJOG,TYPE::VERTICAL),
		std::pair<std::string, TYPE>(VJDIFF,TYPE::VERTICAL),
		std::pair<std::string, TYPE>(VMOVING,TYPE::VERTICAL),
		std::pair<std::string, TYPE>(VGETDEV,TYPE::VERTICAL),
		std::pair<std::string, TYPE>(VDEVSTATE,TYPE::VERTICAL),
		std::pair<std::string, TYPE>(VMAXPOS,TYPE::VERTICAL),
		std::pair<std::string, TYPE>(VDEVCENT,TYPE::VERTICAL),
		std::pair<std::string, TYPE>(VACTPOS,TYPE::VERTICAL),
		std::pair<std::string, TYPE>(VEN,TYPE::VERTICAL),
		std::pair<std::string, TYPE>(SDEV,TYPE::PNEUMATIC),
		std::pair<std::string, TYPE>(TRIGGER,TYPE::PNEUMATIC),
		std::pair<std::string, TYPE>(EX,TYPE::PNEUMATIC),
		std::pair<std::string, TYPE>(TGTPOS,TYPE::PNEUMATIC),
		std::pair<std::string, TYPE>(JOG,TYPE::PNEUMATIC),
		std::pair<std::string, TYPE>(JDIFF,TYPE::PNEUMATIC),
		std::pair<std::string, TYPE>(MOVING,TYPE::PNEUMATIC),
		std::pair<std::string, TYPE>(GETDEV,TYPE::PNEUMATIC),
		std::pair<std::string, TYPE>(DEVSTATE,TYPE::PNEUMATIC),
		std::pair<std::string, TYPE>(MAXPOS,TYPE::PNEUMATIC),
		std::pair<std::string, TYPE>(DEVCENT,TYPE::PNEUMATIC),
		std::pair<std::string, TYPE>(ACTPOS,TYPE::PNEUMATIC),
		std::pair<std::string, TYPE>(EN,TYPE::PNEUMATIC) };

	std::map<std::string, STATE> screenDevicesToEnum{
		std::pair<std::string, STATE>("HRETRACTED",STATE::HRETRACTED),
		std::pair<std::string, STATE>("HMAX",STATE::HMAX),
		std::pair<std::string, STATE>("HSLIT1",STATE::HSLIT1),
		std::pair<std::string, STATE>("HSLIT2",STATE::HSLIT2),
		std::pair<std::string, STATE>("HSLIT3",STATE::HSLIT3),
		std::pair<std::string, STATE>("HAPT1",STATE::HAPT1),
		std::pair<std::string, STATE>("HAPT2",STATE::HAPT2),
		std::pair<std::string, STATE>("HAPT3",STATE::HAPT3),
		std::pair<std::string, STATE>("VRETRACTED",STATE::VRETRACTED),
		std::pair<std::string, STATE>("VMAX",STATE::VMAX),
		std::pair<std::string, STATE>("VRF",STATE::VRF),
		std::pair<std::string, STATE>("VMIRROR",STATE::VMIRROR),
		std::pair<std::string, STATE>("VYAG",STATE::VYAG),
		std::pair<std::string, STATE>("VGRAT",STATE::VGRAT),
		std::pair<std::string, STATE>("VCOL",STATE::VCOL),
		std::pair<std::string, STATE>("VSLIT1",STATE::VSLIT1),
		std::pair<std::string, STATE>("RETRACTED",STATE::RETRACTED),
		std::pair<std::string, STATE>("YAG",STATE::YAG),
		std::pair<std::string, STATE>("RF",STATE::RF) };

	std::map<std::string, TYPE> screenTypeToEnum{
		std::pair<std::string, TYPE>("CLARA_HV_MOVER",TYPE::CLARA_HV_MOVER),
		std::pair<std::string, TYPE>("CLARA_V_MOVER",TYPE::CLARA_V_MOVER),
		std::pair<std::string, TYPE>("VELA_HV_MOVER",TYPE::VELA_HV_MOVER),
		std::pair<std::string, TYPE>("CLARA_PNEUMATIC",TYPE::CLARA_PNEUMATIC),
		std::pair<std::string, TYPE>("VELA_PNEUMATIC",TYPE::VELA_PNEUMATIC) };

	std::map<int, STATE> screenHElementMap{
		std::pair<int, STATE>(0,STATE::HRETRACTED),
		std::pair<int, STATE>(1,STATE::HMAX),
		std::pair<int, STATE>(7,STATE::HSLIT1),
		std::pair<int, STATE>(8,STATE::HSLIT2),
		std::pair<int, STATE>(9,STATE::HSLIT3),
		std::pair<int, STATE>(10,STATE::HAPT1),
		std::pair<int, STATE>(11,STATE::HAPT2),
		std::pair<int, STATE>(12,STATE::HAPT3) };
	std::map<int, STATE> screenVElementMap{
		std::pair<int, STATE>(0,STATE::VRETRACTED),
		std::pair<int, STATE>(1,STATE::VMAX),
		std::pair<int, STATE>(2,STATE::VRF),
		std::pair<int, STATE>(3,STATE::VMIRROR),
		std::pair<int, STATE>(4,STATE::VYAG),
		std::pair<int, STATE>(5,STATE::VGRAT),
		std::pair<int, STATE>(6,STATE::VCOL),
		std::pair<int, STATE>(7,STATE::VSLIT1) };
	std::map<int, STATE> screenPElementMap{
		std::pair<int, STATE>(0,STATE::RETRACTED),
		std::pair<int, STATE>(4,STATE::YAG) };
}

