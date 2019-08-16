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
		//should need a magnet name (full PV root, or alias can be given)
		//Magnet(Hardware hardware); // this should be possible, but isn't useful right now.
		Magnet(std::string knownNameOfMagnet);
		Magnet(const std::map<std::string, std::string>& magnetParametersAndValuesMap, bool isVirtual);
		Magnet(const Magnet& copyMagnet);
		EPICSMagnetInterface *epicsInterface;
		std::string getFullPVName() const;
		std::vector<std::string> getAliases() const;
		std::string getManufacturer() const;
		int getSerialNumber() const;
		std::string getMagnetType() const;
		std::string getMagnetRevType() const;
		double getRITolerance() const;
		int getNumberOfDegaussSteps() const;
		std::vector<double> getDegaussValues() const;
		double getDegaussTolerance() const;
		double getMagneticLength() const;
		std::string getFullPSUName() const;
		std::string getMeasurementDataLocation() const;
		std::map<std::string, std::string> magnetParametersAndValuesMap;
		bool isVirtual;
		bool setCurrent(const double &value);
		bool setEPICSCurrent(const double &value);
		double getCurrent() const;
		double current;
	protected:
		//what else does a magnet need?
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