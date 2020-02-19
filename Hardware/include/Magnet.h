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
/** @addtogroup hardware
 *@{*/
 /*! A class to store information and communicate via EPICS to control Magnets on VELA/CLARA.
	 A collection of these classes will be stored in map in the MagnetFactory class.
 *
*/
class Magnet : public Hardware
{
	public:
	/*! Default constructor call for Magnet object*/
		Magnet();
		//should need a magnet name (full PV root, or alias can be given)
		//Magnet(Hardware hardware); // this should be possible, but isn't useful right now.
		//Magnet(std::string knownNameOfMagnet);
		
	/*! Custom constructor for Magnet object
		@param[in] magnetParametersAndValuesMap strings defining parameters extracted from YAML config files
		@param[in] mode Defines the STATE of Magnet we create: PHYSICAL (connected to CLARA EPICS), VIRTUAL (connected to Virtual EPICS), Offline (no EPICS)
	*/
		Magnet(const std::map<std::string, std::string>& magnetParametersAndValuesMap, STATE mode);
	/*! Copy constructor for Magnet object
		@param[in] copyMagnet references to magnet to be copied
	*/
		Magnet(const Magnet& copyMagnet);


		/*! Copy constructor for Magnet object
		@param[in] copyMagnet references to magnet to be copied
	*/
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

	/*! set the current to value
		@param[in] value			*/
		bool SETI(const double value); 
	/*! set the current to zero	*/
		bool SETIZero(); 	
	/*! switch the magnet PSU on	*/
		bool switchOn();
	/*! switch the magnet PSU off	*/
		bool switchOff();
	/*! switch the magnet PSU on to STATE value	
		@param[in] value, can be ON or OFF (in OFFLINE mode can probably be an arbitrary value)*/
		bool setPSUState(const STATE value);

	/*! switch the magnet PSU on to STATE value
		@param[in] value, can be ON or OFF (in OFFLINE mode can probably be an arbitrary value)*/
		bool offlineSetILKState(const STATE value);
		/*! switch the magnet PSU on	*/
		void debugMessagesOn();
		/*! switch the magnet PSU on	*/
		void debugMessagesOff();
		/*! switch the magnet PSU on	*/
		void messagesOn();
		/*! switch the magnet PSU on	*/
		void messagesOff();
						
		// maybe be more specific and only allow certain functions / vairables?? 
		friend class EPICSMagnetInterface;
		friend class MagnetFactory;

		/*! switch the magnet PSU on	*/
		bool isDegaussing;
		/*! switch the magnet PSU on	*/
		bool last_degauss_success;
		/*! switch the magnet PSU on	*/
		size_t current_degauss_step;

	protected:
	// called from EPICS to update the GETSETI variable! 
	/*! switch the magnet PSU on	*/
		void updateGETSETI(const double& value);

	/*! latest readi value and epicstimestamp 	*/
		std::pair<epicsTimeStamp, double > READI;
	/*! latest getseti value and epicstimestamp 	*/
		std::pair<epicsTimeStamp, double > GETSETI;
	/*! latest psu state value and epicstimestamp 	*/
		std::pair<epicsTimeStamp, STATE > psuState;
	/*! latest interlock state value and epicstimestamp 	*/
		std::pair<epicsTimeStamp, STATE > ilkState;
		
	private:
	/*! Copy constructor for Magnet object
		@param[in] copyMagnet references to magnet to be copied
	*/
		EPICSMagnetInterface_sptr epicsInterface;

		void setPVStructs();

	/*! switch the magnet PSU on to STATE value
		@param[in] value, can be ON or OFF (in OFFLINE mode can probably be an arbitrary value)*/
		void offlineSETI(const double& value);
	/*! switch the magnet PSU on to STATE value
		@param[in] value, can be ON or OFF (in OFFLINE mode can probably be an arbitrary value)*/
		bool offlineSetPSUState(const STATE value);


		Degauss degausser;
	/*! function run in new thread during degaussing 
		@param[in] degauss-struct conatins data required for degaussing */
		static void staticEntryDeGauss(const Degauss& ds);
	/*! current values to set during degauss procedure, defined in the master lattice yaml file	and setable 	*/
		std::vector<double> degaussValues;
	/*! tolerance used durign degausing to decide if readi == seti,		*/
		double degaussTolerance;
	/*! alternative names for the magnet (usually shorter thna the full PV root), defined in the master lattice yaml file	*/
		std::vector<std::string> aliases;
	/*! magnet manufacturer, defined in the master lattice yaml file	*/
		std::string manufacturer;
	/*! magnet serial number, defined in the master lattice yaml file	*/
		int serialNumber;
	/*! magnet type, e.g. SOL, DIP, QUAD, VCOR, HCOR, defined in the master lattice yaml file	*/
		std::string magType;
	/*! , defined in the master lattice yaml file	*/
		std::string magRevType;
	/*! , defined in the master lattice yaml file	*/
		double RI_tolerance;
	/*! , defined in the master lattice yaml file	*/
		int numberOfDegaussSteps; // TODO: this should be a size_t or uint
	/*! magnetic length, defined in the master lattice yaml file */
		double magneticLength;
	/*! PSU epics PV, defined in the master lattice yaml file */
		std::string fullPSUName;
	/*! path to measurment data, defined in the master lattice yaml file  */
		std::string measurementDataLocation;


	/*! check if readi is the same, to within a tolerance, as value
		@param[in] value to compare with readi 
		@param[in] tolerance tolerance for the comaprison
		@param[out] true or false, depending on result of comparison */
		bool isREADIequalValue(const double value, const double tolerance);
	/*! switch the magnet PSU on to STATE value
		@param[in] value, can be ON or OFF (in OFFLINE mode can probably be an arbitrary value)
		@param[in] value, can be ON or OFF (in OFFLINE mode can probably be an arbitrary value)
		@param[in] value, can be ON or OFF (in OFFLINE mode can probably be an arbitrary value)
		@param[out] true or false, can be ON or OFF (in OFFLINE mode can probably be an arbitrary value)
		*/
		bool waitForMagnetToSettle(const double values, const double tolerance, const time_t waitTime);

};
/**\copydoc Hardware*/
/**@}*/

#endif //MAGNET_H_