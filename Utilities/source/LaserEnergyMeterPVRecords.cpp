#include "LaserEnergyMeterPVRecords.h"

namespace LaserEnergyMeterRecords
{
std::string const OVERRANGE = "OVERRANGE";
std::string const RUNSP = "RUNSP";
std::string const RANGESP = "RANGESP";
std::string const ENERGYREADBACK = "ENERGYREADBACK";


	std::vector<std::string> laserEnergyMeterRecordList = { OVERRANGE,RANGESP,RUNSP,ENERGYREADBACK };
	std::map<std::string, TYPE> laserEnergyMeterList{
		std::pair<std::string, TYPE>(OVERRANGE,TYPE::ENERGYMETER),
		std::pair<std::string, TYPE>(RANGESP,TYPE::ENERGYMETER),
		std::pair<std::string, TYPE>(ENERGYREADBACK,TYPE::ENERGYMETER),
		std::pair<std::string, TYPE>(RUNSP,TYPE::ENERGYMETER) };

	std::map<std::string, TYPE> laserEnergyMeterTypeToEnum{
		std::pair<std::string, TYPE>("ENERGYMETER",TYPE::ENERGYMETER) };


}

