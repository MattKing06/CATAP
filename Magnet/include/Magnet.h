#ifndef MAGNET_H_
#define MAGNET_H_
#include "Hardware.h"
#include "LoggingSystem.h"

class Magnet : public Hardware
{
	public:
		Magnet();
		//should need a magnet name (full PV root, or alias can be given)
		Magnet(std::string knownNameOfMagnet);
		//what else do a magnet need?
		std::string hardwareType;
		std::string fullPVName;
		std::vector<std::string> aliases;
		std::string manufacturer;
		int serialNumber;
		std::string magType;
		std::string magRevType;
		float RI_tolerance;
		int numberOfDegaussSteps;
		std::vector<int> degaussValues;
		float degaussTolerance;
		float magneticLength; 
		std::string fullPSUName;
		std::string measurementDataLocation;

};

#endif //MAGNET_H_