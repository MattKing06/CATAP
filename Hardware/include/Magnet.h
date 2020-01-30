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
#include "MagnetPVRecords.h"
#include "GlobalConstants.h"

#include <boost/python/dict.hpp>
#include <boost/python/list.hpp>

// forward declaration of EPICSMagnetInterface class
// tells compiler that we will use this class.
class EPICSMagnetInterface;
class Degauss;
class Magnet;
typedef boost::shared_ptr<EPICSMagnetInterface> EPICSMagnetInterface_sptr;

#include <thread>

class Degauss
{   
	public:
		// proviude a default constructor
		Degauss();
		Magnet* magnet;
		std::vector<double>  degauss_values;
		size_t				 current_step;
		bool                resetToZero;
		double degaussTolerance;
		std::thread* degauss_thread;
		time_t wait_time;
};

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
		
		void setPVStructs();
		//test

		bool degauss(const bool reset_to_zero);
		//bool degaussToZero();


		std::map<std::string, std::string> magnetParametersAndValuesMap;
		
		// parameters that can be got, but NOT SET
		std::vector<std::string> getAliases() const;
		boost::python::list getAliases_Py() const;

		std::string getManufacturer() const;
		int getSerialNumber() const;
		std::string getMagnetType() const;
		std::string getMagnetRevType() const;
		double getMagneticLength() const;
		std::string getFullPSUName() const;
		std::string getMeasurementDataLocation() const;

		// paramEters that can be get AND set 
		int getNumberOfDegaussSteps() const;
		std::vector<double> getDegaussValues() const;
		boost::python::list getDegaussValues_Py() const;
		
		double getDegaussTolerance() const;
		double getRITolerance() const;
		
		int setNumberOfDegaussSteps(const int value);
		std::vector<double> setDegaussValues(const std::vector<double>& values);
		boost::python::list setDegaussValues_Py(const boost::python::list& values);
		
		double setDegaussTolerance(const double value);
		double setRITolerance(const double value);

		// Dynamic EPICS Value getters
		double getREADI() const;
		double getSETI() const;
		STATE getILKState() const;
		STATE getPSUState() const;

		// Set A current 
		bool SETI(const double value); // expposed to PYTHON
		// set Zero current in magnet 
		bool SETIZero(); // expposed to PYTHON

		// set a psu state	
		bool switchOn();
		bool switchOFF();
		bool setPSUState(const STATE value);

		// when in OFFLINE mode you can set an ilk state
		bool offlineSetILKState(const STATE value);

		void debugMessagesOn();
		void debugMessagesOff();
		void messagesOn();
		void messagesOff();
						
		// maybe be more specific and only allow certain functions / vairables?? 
		friend class EPICSMagnetInterface;
		friend class MagnetFactory;


		bool isDegaussing;
		bool last_degauss_success;
		size_t current_degauss_step;


	protected:
		// called from EPICS to update the GETSETI variable! 
		void updateGETSETI(const double& value);

		// EPICS calls this funciotn 
		//void setPSUState(const int& value);

		//bool setEPICSPSUState(const STATE& value);
		//void setEPICSSETI(const double& value);
		//void setREADI(const double& value);
		//bool setILKState(const STATE& value);

		std::pair<epicsTimeStamp, double > READI;
		std::pair<epicsTimeStamp, double > GETSETI;
		std::pair<epicsTimeStamp, STATE > psuState;
		std::pair<epicsTimeStamp, STATE > ilkState;
		
	private:

		void offlineSETI(const double& value);
		bool offlineSetPSUState(const STATE value);



		//  DEGAUSS STUFF
		static void staticEntryDeGauss(const Degauss& ds);
		Degauss degausser;
		std::vector<double> degaussValues;
		double degaussTolerance;

		//what else does a magnet need?
		std::vector<std::string> aliases;
		std::string manufacturer;
		int serialNumber;
		std::string magType;
		std::string magRevType;
		double RI_tolerance;

		int numberOfDegaussSteps; // TODO: this should be a size_t or uint
		


		bool isREADIequalValue(const double value, const double tolerance);
		bool waitForMagnetToSettle(const double values,
								   const double tolerance,
								   const time_t waitTime);


		double magneticLength;
		std::string fullPSUName;
		std::string measurementDataLocation;

		// variables should be private 
		//double READI;
		//STATE psuState;
		// WE NEVER monitor THIS IT IS JUST USED FO RCAPUT
		//double SETI;
		//double GETSETI;
		//STATE ilkState; 



};

#endif //MAGNET_H_