#include "LaserEnergyMeterPVRecords.h"


std::string const LaserEnergyMeterRecords::OVERRANGE = "OVERRANGE";
std::string const LaserEnergyMeterRecords::RUNSP = "RUNSP";
std::string const LaserEnergyMeterRecords::RANGESP = "RANGESP";

namespace LaserEnergyMeterRecords
{
	std::vector<std::string> laserEnergyMeterRecordList = { OVERRANGE,RANGESP,RUNSP };
	std::map<std::string, TYPE> laserEnergyMeterList{
		std::pair<std::string, TYPE>(OVERRANGE,TYPE::ENERGYMETER),
		std::pair<std::string, TYPE>(RANGESP,TYPE::ENERGYMETER),
		std::pair<std::string, TYPE>(RUNSP,TYPE::ENERGYMETER) };

	std::map<std::string, TYPE> laserEnergyMeterTypeToEnum{
		std::pair<std::string, TYPE>("ENERGYMETER",TYPE::ENERGYMETER) };


}

