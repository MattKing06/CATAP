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
#include "GlobalTypeEnums.h"

#include <boost/python/dict.hpp>
#include <boost/python/list.hpp>

// forward declaration of EPICSMagnetInterface class
// tells compiler that we will use this class.
class EPICSMagnetInterface;
class Degauss;
class Magnet;
typedef boost::shared_ptr<EPICSMagnetInterface> EPICSMagnetInterface_sptr;


struct magnetState
{   // proviude a default constructor
	magnetState() :
		name(GlobalConstants::DUMMY_NAME),
		psuState(STATE::ERR),
		ilkState(STATE::ERR),
		seti(GlobalConstants::double_min),
		readi(GlobalConstants::double_min)
	{};
	std::string name;
	STATE psuState, ilkState;
	double seti, readi;
};

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
		@param[in] mode Defines the STATE of Magnet we create: PHYSICAL (connected to CLARA EPICS), VIRTUAL (connected to Virtual EPICS), Offline (no EPICS)			*/
		Magnet(const std::map<std::string, std::string>& magnetParametersAndValuesMap, STATE mode);
	/*! Copy constructor for Magnet object
		@param[in] copyMagnet references to magnet to be copied					*/
		Magnet(const Magnet& copyMagnet);

	/*! get a magnetState (structured data with magnet name and latest, readi, seti, ilkstate and psustae
		@param[out] magnetState structured data									*/
		magnetState getMagnetState()const;
	/*! set a magnetState (structured data with magnet name and latest, readi, seti, ilkstate and psustae
		@param[in] magnetState structured data to set
		@param[out] bool, for if commands successfully got sent to EPICS		*/
		bool setMagnetState(const magnetState& ms);
	/*! check if the current magnet state is the same as the passed in state 
		@param[in] magnetState structured data to check
		@param[out] bool, for if in same state or not							*/
		bool isInState(const magnetState& ms) const;
	/*! check if the current magnet SETI and PSU state is the same as the passed in magnetState
		@param[in] magnetState structured data to check
		@param[out] bool, for if in same state or not							*/
		bool isInSETIandPSUState(const magnetState& ms)const;
	/*! degauss a magnet 
		@param[in] reset_to_zero, whether to set zero current or can be true or false,			*/
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
	/*! get the magnet serial number, defined in the master lattice yaml file
		@param[out] result  */
		std::string getSerialNumber() const;
	/*! get the magnet data measurmetn location 
		@param[out] result  */
		std::string getMeasurementDataLocation() const;
	/*! get the number of values that are set during the deguass procedure, initally defined in the master lattice yaml file, can be changed with setDegaussValues
		@param[out] result  */
		size_t getNumberOfDegaussSteps() const;
	/*! get the actual values used during the degauss procedure, initially defined in the master lattice yaml file
		@param[out] result  */
		std::vector<double> getDegaussValues() const;
	/*! get the actual values used during the degauss procedure, initially defined in the master lattice yaml file (Python version)
		@param[out] result  */
		boost::python::list getDegaussValues_Py() const;
	/*! get the tolerance used to check if readi = seti when deguassing, initially defined in the master lattice yaml file
		@param[out] result  */
		double getDegaussTolerance() const;
	/*! get the tolerance used to check if readi = seti when checking isREADIequalSETI, initially defined in the master lattice yaml file
		@param[out] READI_toelrance after settign new value  */
		double getREADITolerance() const;
	/*! set the READI_tolerance, this value is used to say if READI == SETI to within READI_tolerance
		@param[in] value to set READ_tolerance to
		@param[out] READI_tolerance after setting new value */
		double setREADITolerance(const double value);
	/*! set the values used during degaussing
		@param[out] new value sthat will be used */
		std::vector<double> setDegaussValues(const std::vector<double>& values);
	/*! set the values used during degaussing (Python version)
		@param[out] result  */
		boost::python::list setDegaussValues_Py(const boost::python::list& values);
	/*! set new degaussTolerance (for checking when READ == SETI during degaussing), initially defined in the master lattice yaml file
		@param[in] new degaussTolerance
		@param[out] degaussTolerance now being used */
		double setDegaussTolerance(const double value);
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
	//	void updateGETSETI(const double& value);
	/*! latest readi value and epicstimestamp 	*/
		std::pair<epicsTimeStamp, double > READI;
	/*! latest getseti value and epicstimestamp 	*/
		std::pair<epicsTimeStamp, double > GETSETI;
	/*! latest psu state value and epicstimestamp 	*/
		std::pair<epicsTimeStamp, STATE > psuState;
	/*! latest interlock state value and epicstimestamp 	*/
		std::pair<epicsTimeStamp, STATE > ilkState;
	private:
	/*! map containing all the magnet objects				*/
		std::map<std::string, std::string> magnetParametersAndValuesMap;
	/*! object to interface to epics						*/
		EPICSMagnetInterface_sptr epicsInterface;
	/*! sets the PV struct for the magnet parameters that are monitored through epics, INFO ABOUT PV STRUCTS?*/
		void setPVStructs();
	/*! switch the magnet PSU on to STATE value
		@param[in] value, can be ON or OFF (in OFFLINE mode can probably be an arbitrary value)*/
		void offlineSETI(const double& value);
	/*! switch the magnet PSU on to STATE value
		@param[in] value, can be ON or OFF (in OFFLINE mode can probably be an arbitrary value)*/
		bool offlineSetPSUState(const STATE value);
		Degauss degausser;
	/*! flag set to True when this magnet is degaussing, otherwise false*/
		bool isDegaussing;
	/*! flag 	*/
		bool last_degauss_success;
	/*! current step in the deagussing cycle	*/
		size_t current_degauss_step;
	/*! number of steps in the degaussing procedure, defined in the master lattice yaml file	*/
		size_t numberOfDegaussSteps;
	/*! function run in new thread during degaussing 
		@param[in] degauss-struct contains data required for degaussing */
		static void staticEntryDeGauss(const Degauss& ds);
	/*! current values to set during degauss procedure, defined in the master lattice yaml file	and setable 	*/
		std::vector<double> degaussValues;
	/*! tolerance used during degausing to decide if readi == seti,		*/
		double degaussTolerance;
	/*! alternative names for the magnet (usually shorter thna the full PV root), defined in the master lattice yaml file	*/
		std::vector<std::string> aliases;
	/*! magnet manufacturer, defined in the master lattice yaml file	*/
		std::string manufacturer;

	/*! magnet type, e.g. SOL, DIP, QUAD, VCOR, HCOR, defined in the master lattice yaml file	*/
		std::string magType;
		TYPE magtype;
	/*! how the magnet reverses polarity (as seen in the control system, or defined offline), defined in the master lattice yaml file	*/
		std::string magRevType;
	/*! The tolerance used when checking if READI is eqal to SETI, defined in the master lattice yaml file	*/
		double READI_tolerance;
	/*! magnetic length, defined in the master lattice yaml file */
		double magneticLength;
	/*! magnet serial number, defined in the master lattice yaml file	*/
		std::string serialNumber;

		//std::string magRevType;
		double RI_tolerance;

		int numberOfDegaussSteps; // TODO: this should be a size_t or uint
		


		bool isREADIequalValue(const double value, const double tolerance);
		bool waitForMagnetToSettle(const double values,
								   const double tolerance,
								   const time_t waitTime);



		double magneticLength;
	/*! PSU epics PV, defined in the master lattice yaml file */
		std::string fullPSUName;
	/*! path to measurment data, defined in the master lattice yaml file  */
		std::string measurementDataLocation;
	/*! check if the current READI is equal to a value, to within a tolerance
		@param[in]  value to compare with READI
		@param[in]  tolerance for comparisaon
		@param[out] bool, true if value is equal READI, otherwise false 		*/
		bool isREADIequalValue(const double value, const double tolerance) const;
	/*! wait up to waitTime for the magnet READI to reach a value (to within a tolerance) or the magnet PSU on to STATE value
		@param[in] value to wait for READI to reach 
		@param[in] tolerance for comparison 
		@param[in] waitTime, maximum time to wait before returning false
		@param[out] true or false, true if the magnet settled, false if it didn;t befor ea time out  */
		bool waitForMagnetToSettle(const double value, const double tolerance, const time_t waitTime)const;

		static std::map<std::string, TYPE> create_map()
		{
			std::map<std::string, TYPE> m;
			m["Magnet"] = TYPE::MAGNET;
			m["QUADRUPOLE"] = TYPE::QUADRUPOLE;
			m["DIPOLE"] = TYPE::DIPOLE;
			m["HVCOR"] = TYPE::HVCOR;
			m["VCOR"] = TYPE::VCOR;
			m["HCOR"] = TYPE::HCOR;
			m["SOLENOID"] = TYPE::SOLENOID;
			return m;
		}
		static const std::map<std::string, TYPE> magnet_string_to_type_map;
};
/**\copydoc Hardware*/
/**@}*/

#endif //MAGNET_H_