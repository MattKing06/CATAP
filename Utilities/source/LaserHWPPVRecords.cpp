#include "LaserHWPPVRecords.h"

namespace LaserHWPRecords
{
std::string const RPOS = "ROT:RPOS";
std::string const MABS = "ROT:MABS";
std::string const ENABLE = "ENABLE";


	std::vector<std::string> laserHWPRecordList = { RPOS,MABS, ENABLE };
	std::map<std::string, TYPE> laserHWPList{
		std::pair<std::string, TYPE>(RPOS,TYPE::HWP),
		std::pair<std::string, TYPE>(MABS,TYPE::HWP),
		std::pair<std::string, TYPE>(ENABLE,TYPE::HWP)
	
	};


	// TODO moved into a global TYPE map, but in the future we may bring back enums for hardware specific items
	// OR what is thsi and why do we "need it" ??? (just asking) 
	std::map<std::string, TYPE> laserHWPTypeToEnum{
		std::pair<std::string, TYPE>("HWP",TYPE::HWP) };


}

