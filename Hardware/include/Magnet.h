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


	/*! degauss a magnet 
		@param[in] reset_to_zero, whether to set zero current or can be true or false,  
	*/
		bool degauss(const bool reset_to_zero);
	/*! get the name alises for this magnet 
		@param[out] names, vector contianing all the alias names */
		std::vector<std::string> getAliases() const;
	/*! get the name alises for this magnet (python version
		@param[out] names, python list contianing all the alias names */
		boost::python::list getAliases_Py() const;
	/*! get the magnet manufacturer, defined in the master lattice yaml file
		@param[out] result  */
		std::string getManufacturer() const;
	/*! get the magnet serial number, defined in the master lattice yaml file
		@param[out] result  */
		int getSerialNumber() const;
	/*! get the type of magnet, defined in the master lattice yaml file
		@param[out] result  */
		std::string getMagnetType() const;
	/*! get the magnet reverse type, defined in the master lattice yaml file
		@param[out] result  */
		std::string getMagnetRevType() const;
	/*! get the magnetic length, defined in the master lattice yaml file
		@param[out] result  */
		double getMagneticLength() const;
	/*! get the full-name (PV root) for the magnet's PSU, defined in the master lattice yaml file
		@param[out] result  */
		std::string getFullPSUName() const;
	/*! get the measurement data location, defined in the master lattice yaml file
		@param[out] result  */
		std::string getMeasurementDataLocation() const;
	/*! get the number if current vlaues that are set during the deguass procedure, initally defined in the master lattice yaml file
		@param[out] result  */
		int getNumberOfDegaussSteps() const;
	/*! get the actual values used during teh degauss procedure, initially defined in the master lattice yaml file
		@param[out] result  */
		std::vector<double> getDegaussValues() const;
	/*! get the actual values used during teh degauss procedure, initially defined in the master lattice yaml file (Python version)
		@param[out] result  */
		boost::python::list getDegaussValues_Py() const;
	/*! get the tolerance used to check if readi = seti when deguassing, initially defined in the master lattice yaml file
		@param[out] result  */
		double getDegaussTolerance() const;
	/*! get the tolerance used to check if readi = seti when checking isREADIequalSETI, initially defined in the master lattice yaml file
		@param[out] result  */
		double getRITolerance() const;
	/*! get the type of magnet, defined in the master lattice yaml file
		@param[out] result  */
		int setNumberOfDegaussSteps(const int value);
	/*! get the type of magnet, defined in the master lattice yaml file
		@param[out] result  */
		std::vector<double> setDegaussValues(const std::vector<double>& values);
	/*! get the type of magnet, defined in the master lattice yaml file
		@param[out] result  */
		boost::python::list setDegaussValues_Py(const boost::python::list& values);
	/*! get the type of magnet, defined in the master lattice yaml file
		@param[out] result  */
		double setDegaussTolerance(const double value);
	/*! get the type of magnet, defined in the master lattice yaml file
		@param[out] result  */
		double setRITolerance(const double value);
	/*! get the current READI 
		@param[out] result  */
		double getREADI() const;
	/*! get the current SETI value 
		@param[out] result  */
		double getSETI() const;
	/*! get the current state of the interlock 
		@param[out] result  */
		STATE getILKState() const;
	/*! get the current state of the PSU 
		@param[out] result  */
		STATE getPSUState() const;
	/*! set the current to value
		@param[in] value			
		@param[out] bool, if the command got sent to epics (not if setting that current was successfull!) 	*/
		bool SETI(const double value); 
	/*! set the current to zero	
		@param[out] bool, if the command got sent to epics  (not if setting that current was successfull!)	*/
		bool SETIZero(); 	
	/*! switch the magnet PSU on	
		@param[out] bool, if the command got sent to epics  (not if setting that current was successfull!)	*/
		bool switchOn();
	/*! switch the magnet PSU off	
		@param[out] bool, if the command got sent to epics  (not if setting that current was successfull!)	*/
		bool switchOff();
	/*! switch the magnet PSU on to STATE value	
		@param[in] value, can be ON or OFF (in OFFLINE mode can probably be an arbitrary value)
		@param[out] bool, if the command got sent to epics  (not if setting that current was successfull!)	*/
		bool setPSUState(const STATE value);
	/*! switch the magnet PSU on to STATE value
		@param[in] value, can be ON or OFF (in OFFLINE mode can probably be an arbitrary value)
		@param[out] bool, if the command got sent (not if setting that current was successfull!)	*/
		bool offlineSetILKState(const STATE value);
	/*! enable debug-messages for this magnet 	*/
		void debugMessagesOn();
	/*! disbale debug0messages for this magnet 	*/
		void debugMessagesOff();
	/*! enable messages for this magnet 		*/
		void messagesOn();
	/*! disable messages for this magnet		*/
		void messagesOff();
						
		friend class EPICSMagnetInterface;
		friend class MagnetFactory;
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

		std::map<std::string, std::string> magnetParametersAndValuesMap;
	/*! Copy constructor for Magnet object
		@param[in] copyMagnet references to magnet to be copied
	*/
		EPICSMagnetInterface_sptr epicsInterface;
	/*! sets the PV struct for the magnet parameters that are monitored through epics, INFO ABOUT PV STRUCTS?*/
		void setPVStructs();
	/*! switch the magnet PSU on to STATE value
		@param[in] value, can be ON or OFF (in OFFLINE mode can probably be an arbitrary value)*/
		void offlineSETI(const double& value);
	/*! switch the magnet PSU on to STATE value
		@param[in] value, can be ON or OFF (in OFFLINE mode can probably be an arbitrary value)*/
		bool offlineSetPSUState(const STATE value);
	/*! flag set to True when this magnet is degaussing, otherwise false*/
		bool isDegaussing;
	/*! flag 	*/
		bool last_degauss_success;
	/*! current step in the deagussing cycle	*/
		size_t current_degauss_step;
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
		@param[out] true or false, true if the magnet settled, false if it didn;t befor ea time out  */
		bool waitForMagnetToSettle(const double values, const double tolerance, const time_t waitTime);
};
/**\copydoc Hardware*/
/**@}*/

#endif //MAGNET_H_