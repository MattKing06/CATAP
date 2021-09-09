#include "MagnetPVRecords.h"

namespace MagnetRecords
{
	std::string const GETSETI = "GETSETI";
	std::string const READI = "READI";
	std::string const RILK = "RILK";
	std::string const RPOWER = "RPOWER";
	std::string const SETI = "SETI";
	std::string const SPOWER = "SPOWER";
	std::string const K_DIP_P = "K_DIP_P"; 
	std::string const INT_STR_MM = "INT_STR_MM"; 
	std::string const INT_STR = "INT_STR"; 
	std::string const K_SET_P = "K_SET_P"; 
	std::string const K_ANG = "K_ANG"; 
	std::string const K_MRAD = "K_MRAD"; 
	std::string const K_VAL = "K_VAL"; 
	std::string const ILK_RESET = "ILK_RESET";

	std::vector<std::string> magnetRecordList = { GETSETI,READI,RILK,RPOWER,SETI,SPOWER,
		K_DIP_P,INT_STR_MM,INT_STR,K_SET_P,K_ANG,K_MRAD,K_VAL, ILK_RESET };

	std::vector<std::string> magnetMonitorRecordList = { GETSETI,READI,RILK,RPOWER,
		K_DIP_P,INT_STR_MM,INT_STR,K_SET_P,K_ANG,K_MRAD,K_VAL };

//	std::vector<std::string> magnetRecordList = { GETSETI,READI,RILK,RPOWER,SETI,SPOWER };
}

//namespace MagnetPSUValueToState
//{
//	 add a converter here 
//	std::map<std::string, STATE> magnetPSUStrToSTATE = {
//								{"ON", STATE::ON},
//								{"OFF", STATE::OFF},
//								{"ERR", STATE::ERR}
//	};
//
//	std::map<int, STATE> magnetPSUIntToSTATE = {
//							{1, STATE::ON},
//							{0, STATE::OFF}
//							};
//
//
//}
