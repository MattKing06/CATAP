#ifndef MAGNET_H_
#define MAGNET_H_
#include "LoggingSystem.h"
#include "Hardware.h"

class Magnet : public Hardware
{
	public:
		Magnet();
		//should need a magnet name (full PV root, or alias can be given)
		Magnet(std::string knownNameOfMagnet);
		//what else do a magnet need?
		std::string fullPVName;
		std::vector<std::string> aliases;
		std::string manufacturer;
		int serialNumber;
		std::string magType;
		std::string magRevType;
		double RI_tolerance;
		int numberOfDegaussSteps;
		std::vector<double> degaussValues;
		double degaussTolerance;
		double magneticLength; 
		std::string fullPSUName;
		std::string measurementDataLocation;
		//std::vector<pvStruct> MagnetPVStructs;
};

#endif //MAGNET_H_