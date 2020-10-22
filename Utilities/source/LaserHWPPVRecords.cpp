#include "LaserHWPPVRecords.h"

namespace LaserHWPRecords
{
std::string const RPOS = "ROT:RPOS";
std::string const MABS = "ROT:MABS";


	std::vector<std::string> laserHWPRecordList = { RPOS,MABS };
	std::map<std::string, TYPE> laserHWPList{
		std::pair<std::string, TYPE>(RPOS,TYPE::HWP),
		std::pair<std::string, TYPE>(MABS,TYPE::HWP) };

	std::map<std::string, TYPE> laserHWPTypeToEnum{
		std::pair<std::string, TYPE>("HWP",TYPE::HWP) };


}

