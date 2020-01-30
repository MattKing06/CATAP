#include "MagnetPVRecords.h"


std::string const MagnetRecords::GETSETI = "GETSETI";
std::string const MagnetRecords::READI = "READI";
std::string const MagnetRecords::RILK = "RILK";
std::string const MagnetRecords::RPOWER = "RPOWER";
std::string const MagnetRecords::SETI = "SETI";
std::string const MagnetRecords::SPOWER = "SPOWER";

namespace MagnetRecords
{
	std::vector<std::string> magnetRecordList = { GETSETI,READI,RILK,RPOWER,SETI,SPOWER };
}

