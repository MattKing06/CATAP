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
		//Magnet(std::string knownNameOfMagnet);
		
		Magnet(const std::map<std::string, std::string>& magnetParametersAndValuesMap, STATE mode);
		Magnet(const Magnet& copyMagnet);
		EPICSMagnetInterface_sptr epicsInterface;
		

		//test


		std::map<std::string, std::string> magnetParametersAndValuesMap;
		
		std::vector<std::string> getAliases() const;
		std::string getManufacturer() const;
		int getSerialNumber() const;
		std::string getMagnetType() const;
		std::string getMagnetRevType() const;
		int getNumberOfDegaussSteps() const;
		std::vector<double> getDegaussValues() const;
		double getDegaussTolerance() const;
		double getMagneticLength() const;
		std::string getFullPSUName() const;
		std::string getMeasurementDataLocation() const;
		
		
		double getRITolerance() const;

		// Set A current
		void SETI(const double& value);
		void setEPICSSETI(const double &value);
		void offlineSETI(const double& value);
		
		// called from EPICS to update the GETSETI variable! 
		void updateGETSETI(const double& value);


		
		bool setPSUState(const STATE& value);
		bool setEPICSPSUState(const STATE& value);
		
		
		void setREADI(const double &value);
		double getREADI() const;

		
		bool setILKState(const STATE& value);
		STATE getILKState() const;
		STATE getPSUState() const;
		void debugMessagesOn();
		void debugMessagesOff();
		void messagesOn();
		void messagesOff();

		double getSETI() const;
		


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

		// variables should be private 
		double READI;
		STATE psuState;
		
		// WE NEVER MONITOR THIS IT IS JUST USED FO RCAPUT
		//double SETI;
		
		double GETSETI;
		STATE ilkState; // This should be a state?? 
		
		//STATE mode;

};

#endif //MAGNET_H_