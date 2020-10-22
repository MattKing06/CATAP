#include "LaserHWPPVRecords.h"


std::string const LaserHWPRecords::RPOS = "ROT:RPOS";
std::string const LaserHWPRecords::MABS = "ROT:MABS";

namespace LaserHWPRecords
{
	std::vector<std::string> laserHWPRecordList = { RPOS,MABS };
	std::map<std::string, TYPE> laserHWPList{
		std::pair<std::string, TYPE>(RPOS,TYPE::HWP),
		std::pair<std::string, TYPE>(MABS,TYPE::HWP) };

	std::map<std::string, TYPE> laserHWPTypeToEnum{
		std::pair<std::string, TYPE>("HWP",TYPE::HWP) };


}

