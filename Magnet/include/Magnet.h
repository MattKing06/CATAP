#ifndef MAGNET_H_
#define MAGNET_H_
#include "LoggingSystem.h"
#ifndef HARDWARE_H_
#include "Hardware.h"
#endif //HARDWARE_H_
#ifndef EPICS_MAGNET_INTERFACE_H_
#include "EPICSMagnetInterface.h"
#endif //EPICS_MAGNET_INTERFACE_H_

// forward declaration of EPICSMagnetInterface class
// tells compiler that we will use this class.
class EPICSMagnetInterface;
class Magnet : public Hardware
{
	public:
		Magnet();
		~Magnet();

		EPICSMagnetInterface *epicsInterface;
		//should need a magnet name (full PV root, or alias can be given)
		//Magnet(Hardware hardware); // this should be possible, but isn't useful right now.
		Magnet(std::string knownNameOfMagnet);
		Magnet(std::map<std::string, std::string> &magnetParametersAndValuesMap, bool isVirtual);
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
		std::map<std::string, std::string> magnetParametersAndValuesMap;
		bool isVirtual;
		bool setCurrent(double value);
		bool setEPICSCurrent(double value);
		double getCurrent();
		double current;
	protected:
		//what else does a magnet need?
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
		//(inherited) std::string hardwareType;
		//(inherited) std::string machineArea;

};

#endif //MAGNET_H_