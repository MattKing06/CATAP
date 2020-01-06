#ifndef MAGNET_H_
#define MAGNET_H_
#include "LoggingSystem.h"
#ifndef HARDWARE_H_
#include "Hardware.h"
#endif //HARDWARE_H_
#ifndef EPICS_MAGNET_INTERFACE_H_
#include "EPICSMagnetInterface.h"
#endif //EPICS_MAGNET_INTERFACE_H_
#include <boost/shared_ptr.hpp>
#include <vector>
// forward declaration of EPICSMagnetInterface class
// tells compiler that we will use this class.
class EPICSMagnetInterface;
typedef boost::shared_ptr<EPICSMagnetInterface> EPICSMagnetInterface_sptr;

class Magnet : public Hardware
{
	public:
		Magnet();
		//should need a magnet name (full PV root, or alias can be given)
		//Magnet(Hardware hardware); // this should be possible, but isn't useful right now.
		Magnet(std::string knownNameOfMagnet);
		Magnet(const std::map<std::string, std::string>& magnetParametersAndValuesMap, bool isVirtual);
		Magnet(const Magnet& copyMagnet);
		EPICSMagnetInterface_sptr epicsInterface;
		double current;
		STATE psuState;
		double RICurrent;
		int ilkState;
		std::map<std::string, std::string> magnetParametersAndValuesMap;
		bool isVirtual;
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
		bool setCurrent(const double &value);
		bool setEPICSCurrent(const double &value);
		bool setPSUState(const STATE& value);
		bool setEPICSPSUState(const STATE& value);
		bool setRICurrent(const double &value);
		bool setILKState(const int& value);
		int getILKState() const;
		double getRICurrent() const;
		double getCurrent() const;
		STATE getPSUState() const;


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