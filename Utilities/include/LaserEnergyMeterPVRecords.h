#ifndef LASER_ENERGY_METER_RECORDS
#define LASER_ENERGY_METER_RECORDS
#include <string>
#include <vector>

#include "GlobalStateEnums.h"
#include "GlobalConstants.h"


namespace LaserEnergyMeterRecords
{
	/*Defined at std::string const so that the value of the variable cannot be changed.*/
	extern std::string const OVERRANGE; // = "X";
	extern std::string const RANGESP; // = "Y";
	extern std::string const RUNSP; // = "RA1";

	extern std::vector<std::string> laserEnergyMeterRecordList;
	extern std::map<std::string, TYPE> laserTypeList;
	extern std::map<int, STATE> laserSPElementMap;
	extern std::map<std::string, TYPE> laserEnergyMeterList;
	extern std::map<std::string, TYPE> laserEnergyMeterTypeToEnum;
}

namespace LaserEnergyMeterState
{
	struct laserEnergyMeterStateStruct
	{   // proviude a default constructor
		laserEnergyMeterStateStruct() :
			numLasers(GlobalConstants::zero_sizet)
		{};
		size_t numLasers;
		//HWC_ENUM::MACHINE_AREA machineArea;
		std::vector<std::string> laserNames;
		std::vector<STATE> laserStates;
	};
}

#endif