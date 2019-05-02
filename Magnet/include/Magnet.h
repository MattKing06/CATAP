#ifndef MAGNET_H_
#define MAGNET_H_
#include "LoggingSystem.h"
#include "Hardware.h"

class Magnet : public Hardware
{
	public:
		Magnet();
		~Magnet();
		//should need a magnet name (full PV root, or alias can be given)
		Magnet(Hardware hardware);
		Magnet(std::string knownNameOfMagnet);
		Magnet(std::multimap<std::string, std::string> &magnetParametersAndValuesMap);
		std::string getFullPVName();
		std::vector<std::string> getAliases();
		std::string getManufacturer();
		int getSerialNumber();
		std::string getMagnetType();
		std::string getMagnetRevType();
		double getRITolerance();
		int getNumberOfDegaussSteps();
		std::vector<double> getDegaussValues();
		double getDegaussTolerance();
		double getMagneticLength();
		std::string getFullPSUName();
		std::string getMeasurementDataLocation();
		std::multimap<std::string, std::string> magnetParametersAndValuesMap;
	protected:
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
		//(inherited) std::vector<pvStruct> MagnetPVStructs;
		//(iherited) std::string hardwareType;
		//(inherited) std::string machineArea;

};

#endif //MAGNET_H_