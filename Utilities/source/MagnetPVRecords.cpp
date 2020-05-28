#include "MagnetPVRecords.h"

namespace MagnetRecords
{
	std::string const GETSETI = "GETSETI";
	std::string const READI = "READI";
	std::string const RILK = "RILK";
	std::string const RPOWER = "RPOWER";
	std::string const SETI = "SETI";
	std::string const SPOWER = "SPOWER";


	std::vector<std::string> magnetRecordList = { GETSETI,READI,RILK,RPOWER,SETI,SPOWER };
}

namespace MagnetPSUValueToState
{
	// add a converter here 
	std::map<std::string, STATE> magnetPSUStrToSTATE = {
								{"ON", STATE::ON},
								{"OFF", STATE::OFF},
								{"ERR", STATE::ERR}
	};

	std::map<int, STATE> magnetPSUIntToSTATE = {
							{1, STATE::ON},
							{0, STATE::OFF}
							};


}
