#include "IMGPVRecords.h"

std::string const IMGRecords::Pressure = "P";
std::string const IMGRecords::Sta = "Sta";

namespace IMGRecords
{
	std::vector<std::string> imgRecordList = { Pressure, Sta };
}