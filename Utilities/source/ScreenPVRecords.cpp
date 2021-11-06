#include "ScreenPVRecords.h"
namespace ScreenRecords
{
// Screen PV types
std::string const H_SDEV = "H_SDEV";
std::string const H_TRIGGER = "H_TRIGGER";
std::string const H_EX = "H_EX";
std::string const H_TGTPOS = "H_TGTPOS";
std::string const H_JOG = "H_JOG";
std::string const H_JDIFF = "H_JDIFF";
std::string const H_MOVING = "H_MOVING";
std::string const H_READY = "H_READY";
std::string const H_GETDEV = "H_GET_DEV";
std::string const H_DEVSTATE = "H_DEV_STATE";
std::string const H_MAXPOS = "H_MAX_POS";
std::string const H_DEVCENT = "H_DEV_CENT";
std::string const H_ACTPOS = "H_ACTPOS";
std::string const H_EN = "H_EN";

std::string const V_SDEV = "V_SDEV";
std::string const V_TRIGGER = "V_TRIGGER";
std::string const V_EX = "V_EX";
std::string const V_TGTPOS = "V_TGTPOS";
std::string const V_JOG = "V_JOG";
std::string const V_JDIFF = "V_JDIFF";
std::string const V_MOVING = "V_MOVING";
std::string const V_READY = "V_READY";
std::string const V_GETDEV = "V_GET_DEV";
std::string const V_DEVSTATE = "V_DEV_STATE";
std::string const V_MAXPOS = "V_MAX_POS";
std::string const V_DEVCENT = "V_DEV_CENT";
std::string const V_ACTPOS = "V_ACTPOS";
std::string const V_EN = "V_EN";

std::string const SDEV = "SDEV";
std::string const TRIGGER = "TRIGGER";
std::string const EX = "EX";
std::string const TGTPOS = "TGTPOS";
std::string const JOG = "JOG";
std::string const JDIFF = "JDIFF";
std::string const MOVING = "MOVING";
std::string const READY = "READY";
std::string const GETDEV = "GET_DEV";
std::string const DEVSTATE = "DEV_STATE";
std::string const MAXPOS = "MAX_POS";
std::string const DEVCENT = "DEV_CENT";
std::string const ACTPOS = "ACTPOS";
std::string const EN = "EN";
std::string const PNEUMATIC_ON = "PNEUMATIC_ON"; // = "EN";
std::string const PNEUMATIC_OFF = "PNEUMATIC_OFF"; // = "EN";
std::string const PNEUMATIC_STA = "PNEUMATIC_STA"; // = "EN";

								 // Screen positions
const std::pair< int, STATE > HRETRACTED = std::make_pair(0, STATE::HRETRACTED);
const std::pair< int, STATE > HMAX = std::make_pair(1, STATE::HMAX);
const std::pair< int, STATE > HSLIT1 = std::make_pair(7, STATE::HSLIT1);
const std::pair< int, STATE > HSLIT2 = std::make_pair(8, STATE::HSLIT2);
const std::pair< int, STATE > HSLIT3 = std::make_pair(9, STATE::HSLIT3);
const std::pair< int, STATE > HAPT1 = std::make_pair(10, STATE::HAPT1);
const std::pair< int, STATE > HAPT2 = std::make_pair(11, STATE::HAPT2);
const std::pair< int, STATE > HAPT3 = std::make_pair(12, STATE::HAPT3);
const std::pair< int, STATE > VRETRACTED = std::make_pair(0, STATE::VRETRACTED);
const std::pair< int, STATE > VMAX = std::make_pair(1, STATE::VMAX);
const std::pair< int, STATE > VRF = std::make_pair(2, STATE::VRF);
const std::pair< int, STATE > VMIRROR = std::make_pair(3, STATE::VMIRROR);
const std::pair< int, STATE > VYAG = std::make_pair(4, STATE::VYAG);
const std::pair< int, STATE > VGRAT = std::make_pair(5, STATE::VGRAT);
const std::pair< int, STATE > VCOL = std::make_pair(6, STATE::VCOL);
const std::pair< int, STATE > VSLIT1 = std::make_pair(7, STATE::VSLIT1);
const std::pair< int, STATE > RETRACTED = std::make_pair(0, STATE::RETRACTED);
const std::pair< int, STATE > RF = std::make_pair(2, STATE::RF);
const std::pair< int, STATE > YAG = std::make_pair(4, STATE::YAG);


//	std::string const HPV = ":H:";
//	std::string const VPV = ":V:";
	std::vector<std::string> screenRecordList{ H_SDEV, H_TRIGGER, H_EX, H_TGTPOS, H_JOG, H_JDIFF, H_MOVING, H_READY, H_GETDEV, H_DEVSTATE, H_MAXPOS, H_DEVCENT, H_ACTPOS, H_EN,
												  V_SDEV, V_TRIGGER, V_EX, V_TGTPOS, V_JOG, V_JDIFF, V_MOVING, V_READY, V_GETDEV, V_DEVSTATE, V_MAXPOS, V_DEVCENT, V_ACTPOS, V_EN,
												  SDEV,  TRIGGER,  EX,  TGTPOS,  JOG,  JDIFF,  MOVING,  READY,  GETDEV,  DEVSTATE,  MAXPOS,  DEVCENT,  ACTPOS,  EN };
	std::vector<std::string> screenHVRecordList{ H_SDEV, H_TRIGGER, H_EX, H_TGTPOS, H_JOG, H_JDIFF, H_MOVING, H_READY, H_GETDEV, H_DEVSTATE, H_MAXPOS, H_DEVCENT, H_ACTPOS, H_EN,
												  V_SDEV, V_TRIGGER, V_EX, V_TGTPOS, V_JOG, V_JDIFF, V_MOVING, V_READY, V_GETDEV, V_DEVSTATE, V_MAXPOS, V_DEVCENT, V_ACTPOS, V_EN };
	std::vector<std::string> screenVRecordList{ V_SDEV, V_TRIGGER, V_EX, V_TGTPOS, V_JOG, V_JDIFF, V_MOVING, V_READY, V_GETDEV, V_DEVSTATE, V_MAXPOS, V_DEVCENT, V_ACTPOS, V_EN };
	std::vector<std::string> screenPRecordList{ SDEV, TRIGGER, MOVING, READY, GETDEV, DEVSTATE, MAXPOS, DEVCENT, PNEUMATIC_ON,  PNEUMATIC_OFF, PNEUMATIC_STA };

	std::map<std::string, TYPE> screenDirectionList{ 
		std::pair<std::string, TYPE>(H_SDEV,TYPE::HORIZONTAL),
		std::pair<std::string, TYPE>(H_TRIGGER,TYPE::HORIZONTAL),
		std::pair<std::string, TYPE>(H_EX,TYPE::HORIZONTAL),
		std::pair<std::string, TYPE>(H_TGTPOS,TYPE::HORIZONTAL),
		std::pair<std::string, TYPE>(H_JOG,TYPE::HORIZONTAL),
		std::pair<std::string, TYPE>(H_JDIFF,TYPE::HORIZONTAL),
		std::pair<std::string, TYPE>(H_MOVING,TYPE::HORIZONTAL),
		std::pair<std::string, TYPE>(H_READY,TYPE::HORIZONTAL),
		std::pair<std::string, TYPE>(H_GETDEV,TYPE::HORIZONTAL),
		std::pair<std::string, TYPE>(H_DEVSTATE,TYPE::HORIZONTAL),
		std::pair<std::string, TYPE>(H_MAXPOS,TYPE::HORIZONTAL),
		std::pair<std::string, TYPE>(H_DEVCENT,TYPE::HORIZONTAL),
		std::pair<std::string, TYPE>(H_ACTPOS,TYPE::HORIZONTAL),
		std::pair<std::string, TYPE>(H_EN,TYPE::HORIZONTAL),
		std::pair<std::string, TYPE>(V_SDEV,TYPE::VERTICAL),
		std::pair<std::string, TYPE>(V_TRIGGER,TYPE::VERTICAL),
		std::pair<std::string, TYPE>(V_EX,TYPE::VERTICAL),
		std::pair<std::string, TYPE>(V_TGTPOS,TYPE::VERTICAL),
		std::pair<std::string, TYPE>(V_JOG,TYPE::VERTICAL),
		std::pair<std::string, TYPE>(V_JDIFF,TYPE::VERTICAL),
		std::pair<std::string, TYPE>(V_MOVING,TYPE::VERTICAL),
		std::pair<std::string, TYPE>(V_READY,TYPE::VERTICAL),
		std::pair<std::string, TYPE>(V_GETDEV,TYPE::VERTICAL),
		std::pair<std::string, TYPE>(V_DEVSTATE,TYPE::VERTICAL),
		std::pair<std::string, TYPE>(V_MAXPOS,TYPE::VERTICAL),
		std::pair<std::string, TYPE>(V_DEVCENT,TYPE::VERTICAL),
		std::pair<std::string, TYPE>(V_ACTPOS,TYPE::VERTICAL),
		std::pair<std::string, TYPE>(V_EN,TYPE::VERTICAL),
		std::pair<std::string, TYPE>(SDEV,TYPE::PNEUMATIC),
		std::pair<std::string, TYPE>(TRIGGER,TYPE::PNEUMATIC),
		std::pair<std::string, TYPE>(MOVING,TYPE::PNEUMATIC),
		std::pair<std::string, TYPE>(READY,TYPE::PNEUMATIC),
		std::pair<std::string, TYPE>(GETDEV,TYPE::PNEUMATIC),
		std::pair<std::string, TYPE>(DEVSTATE,TYPE::PNEUMATIC),
		std::pair<std::string, TYPE>(MAXPOS,TYPE::PNEUMATIC),
		std::pair<std::string, TYPE>(DEVCENT,TYPE::PNEUMATIC) };

	std::map<std::string, STATE> screenDevicesToEnum{
		{ENUM_TO_STRING(STATE::HRETRACTED),STATE::HRETRACTED	},
		{ENUM_TO_STRING(STATE::HMAX),STATE::HMAX				},
		{ENUM_TO_STRING(STATE::HSLIT1),STATE::HSLIT1			},
		{ENUM_TO_STRING(STATE::HSLIT2),STATE::HSLIT2			},
		{ENUM_TO_STRING(STATE::HSLIT3),STATE::HSLIT3			},
		{ENUM_TO_STRING(STATE::HAPT1),STATE::HAPT1				},
		{ENUM_TO_STRING(STATE::HAPT2),STATE::HAPT2				},
		{ENUM_TO_STRING(STATE::HAPT3),STATE::HAPT3				},
		{ENUM_TO_STRING(STATE::VRETRACTED),STATE::VRETRACTED	},
		{ENUM_TO_STRING(STATE::VMAX),STATE::VMAX				},
		{ENUM_TO_STRING(STATE::VRF),STATE::VRF					},
		{ENUM_TO_STRING(STATE::VMIRROR),STATE::VMIRROR			},
		{ENUM_TO_STRING(STATE::VYAG),STATE::VYAG				},
		{ENUM_TO_STRING(STATE::VGRAT),STATE::VGRAT				},
		{ENUM_TO_STRING(STATE::VCOL),STATE::VCOL				},
		{ENUM_TO_STRING(STATE::VSLIT1),STATE::VSLIT1			},
		{ENUM_TO_STRING(STATE::RETRACTED),STATE::RETRACTED		},
		{ENUM_TO_STRING(STATE::YAG),STATE::YAG					},
		{ENUM_TO_STRING(STATE::RF),STATE::RF					} };

	std::map<std::string, TYPE> screenTypeToEnum{
		{ENUM_TO_STRING(TYPE::CLARA_HV_MOVER),TYPE::CLARA_HV_MOVER},
		{ENUM_TO_STRING(TYPE::CLARA_V_MOVER),TYPE::CLARA_V_MOVER},
		{ENUM_TO_STRING(TYPE::VELA_HV_MOVER),TYPE::VELA_HV_MOVER},
		{ENUM_TO_STRING(TYPE::CLARA_PNEUMATIC),TYPE::CLARA_PNEUMATIC},
		{ENUM_TO_STRING(TYPE::VELA_PNEUMATIC),TYPE::VELA_PNEUMATIC} };


	//std::map<int, STATE> screenVElementMap{
	//	std::pair<int, STATE>(0,STATE::VRETRACTED),
	//	std::pair<int, STATE>(1,STATE::VMAX),
	//	std::pair<int, STATE>(2,STATE::VRF),
	//	std::pair<int, STATE>(3,STATE::VMIRROR),
	//	std::pair<int, STATE>(4,STATE::VYAG),
	//	std::pair<int, STATE>(5,STATE::VGRAT),
	//	std::pair<int, STATE>(6,STATE::VCOL),
	//	std::pair<int, STATE>(7,STATE::VSLIT1) };
	//std::map<int, STATE> screenPElementMap{
	//	std::pair<int, STATE>(0,STATE::RETRACTED),
	//	std::pair<int, STATE>(2,STATE::RF),
	//	std::pair<int, STATE>(4,STATE::YAG) };

	// TODO GET RID OF THIS
	std::map<int, STATE> screenHElementMap{
	std::pair<int, STATE>(0,STATE::HRETRACTED),
	std::pair<int, STATE>(1,STATE::HMAX),
	std::pair<int, STATE>(7,STATE::HSLIT1),
	std::pair<int, STATE>(8,STATE::HSLIT2),
	std::pair<int, STATE>(9,STATE::HSLIT3),
	std::pair<int, STATE>(10,STATE::HAPT1),
	std::pair<int, STATE>(11,STATE::HAPT2),
	std::pair<int, STATE>(12,STATE::HAPT3) };
	// TODO GET RID OF THIS
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
		std::pair<int, STATE>(2,STATE::RF),
		std::pair<int, STATE>(4,STATE::YAG) };


	const std::vector<STATE> screenPElements{ STATE::RETRACTED,STATE::RF,STATE::YAG };
	const std::vector<STATE> screenHElements{
	STATE::HRETRACTED,
	STATE::HMAX,
	STATE::HSLIT1,
	STATE::HSLIT2,
	STATE::HSLIT3,
	STATE::HAPT1,
	STATE::HAPT2,
	STATE::HAPT3 };

	const std::vector<STATE> screenVElements{ STATE::VRETRACTED, STATE::VMAX,STATE::VRF,
		STATE::VMIRROR,STATE::VYAG,STATE::VGRAT,STATE::VCOL,STATE::VSLIT1};


	const std::map<unsigned short, STATE> h_device_to_state_map = {
		{GlobalConstants::zero_ushort,STATE::HRETRACTED},
		{GlobalConstants::one_ushort,STATE::HMAX},
		{GlobalConstants::seven_ushort, STATE::HSLIT1},
		{GlobalConstants::eight_ushort,STATE::HSLIT2},
		{GlobalConstants::nine_ushort,STATE::HSLIT3},
		{GlobalConstants::ten_ushort,STATE::HAPT1},
		{GlobalConstants::eleven_ushort,STATE::HAPT2},
		{GlobalConstants::twelve_ushort,STATE::HAPT3} };

	const std::map<STATE, unsigned short> h_state_to_device_map = {
		{STATE::HRETRACTED,GlobalConstants::zero_ushort},
		{STATE::HMAX,GlobalConstants::two_ushort},
		{STATE::HSLIT1,GlobalConstants::seven_ushort},
		{STATE::HSLIT2,GlobalConstants::eight_ushort},
		{STATE::HSLIT3,GlobalConstants::nine_ushort},
		{STATE::HAPT1,GlobalConstants::ten_ushort},
		{STATE::HAPT2,GlobalConstants::eleven_ushort},
		{STATE::HAPT3,GlobalConstants::twelve_ushort} };


	const std::map<unsigned short, STATE> v_device_to_state_map = {
		{GlobalConstants::zero_ushort,STATE::VRETRACTED},
		{GlobalConstants::one_ushort,STATE::VMAX},
		{GlobalConstants::two_ushort,STATE::VRF},
		{GlobalConstants::three_ushort,STATE::VMIRROR},
		{GlobalConstants::four_ushort,STATE::VYAG},
		{GlobalConstants::five_ushort,STATE::VGRAT},
		{GlobalConstants::six_ushort,STATE::VCOL},
		{GlobalConstants::seven_ushort,STATE::VSLIT1} };

	const std::map<STATE, unsigned short> v_state_to_device_map = {
		{STATE::VRETRACTED, GlobalConstants::zero_ushort},
		{STATE::VMAX,		GlobalConstants::one_ushort},
		{STATE::VRF,		GlobalConstants::two_ushort},
		{STATE::VMIRROR,	GlobalConstants::three_ushort},
		{STATE::VYAG,		GlobalConstants::four_ushort},
		{STATE::VGRAT,		GlobalConstants::five_ushort},
		{STATE::VCOL,		GlobalConstants::six_ushort},
		{STATE::VSLIT1,		GlobalConstants::seven_ushort} };

	const std::map<STATE, unsigned short> p_state_to_device_map{
	{STATE::RETRACTED,	GlobalConstants::zero_ushort},
	{STATE::RF,			GlobalConstants::two_ushort},
	{STATE::YAG,		GlobalConstants::four_ushort} };

	const std::map<unsigned short, STATE> p_device_to_state_map{
	{GlobalConstants::zero_ushort,STATE::RETRACTED},
	{GlobalConstants::two_ushort,STATE::RF},
	{GlobalConstants::four_ushort,STATE::YAG} };




}

