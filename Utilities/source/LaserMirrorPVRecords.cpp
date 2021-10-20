#include <LaserMirrorPVRecords.h>

std::string const LaserMirrorRecords::H_MREL = "H_MREL";
std::string const LaserMirrorRecords::V_MREL = "V_MREL";
std::string const LaserMirrorRecords::H_POS = "H_POS";
std::string const LaserMirrorRecords::V_POS = "V_POS";
std::string const LaserMirrorRecords::POSBTN = "POSBTN";

namespace LaserMirrorRecords
{

	std::vector<std::string> laserMirrorRecordList = { H_MREL, V_MREL, H_POS, V_POS, POSBTN };
}