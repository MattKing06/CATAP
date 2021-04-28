#ifndef MAGNET_FACTORY_H_
#define MAGNET_FACTORY_H_

#include "GlobalStateEnums.h"
#include "GlobalTypeEnums.h"
#include "GlobalFunctions.h"
#include "LoggingSystem.h"
#include "ConfigReader.h"
#pragma Once
#include "Magnet.h"
#include <vector>
#include <map>
#include <boost/python/dict.hpp>
#include <boost/python/list.hpp>
#include <boost/filesystem.hpp>


typedef void(*updateFunctionPtr)(struct event_handler_args args);
class Magnet;
/// one-stop shop for magnet state
struct magnetStates
{   
	/*! Default constructor call for magnetStates */
	magnetStates():
		magnet_count(GlobalConstants::zero_sizet)
		{};
	size_t magnet_count;
	std::map<std::string, magnetState> magnet_states_map;
	boost::python::dict magnet_states_dict;
};
/// DBURTs are magnet-states plus comment and timestamp 
struct dburt
{   /*! Default constructor call for dburt */
	dburt() :
		comment("NO COMMENT"),
		timestamp("NO TIME")
	{};
	std::string comment;
	std::string timestamp;
	magnetStates magnetstates;
	//magnetStateStruct magnetstates_Py;
};

class MagnetFactory
{
	public:
		/*! Default constructor call for MagnetFactory object*/
		MagnetFactory();
		/*! Custom constructor for MagnetFactory object
		@param[in] STATE mode, mode is OFFLINE, VIRTUAL, or PHYSICAL, dependign on which machine to try and connect to */
		MagnetFactory(STATE mode);
		/*! Copy constructor for MagnetFactory object
			@param[in] copyMagnetFactory references to magnet to be copied					*/
		MagnetFactory(const MagnetFactory& copyMagnetFactory);
		~MagnetFactory();
		/*NEED constRUCTOR THAT TAKES VERSION??*/
		//MagnetFactory(std::string VERSION);

		/*! default setup function, uses default values to read files and connect to EPICS etc. 
		@param[out] bool, for success or failure	*/
		bool setup();
		/*! setup function using version parameter 
		@param[in] version (a placeholder for future extensions)
		@param[out] bool, for success or failure	*/
		bool setup(const std::string& version);
		/*! setup function using machineArea parameter 
		@param[in] machineArea, only setup magnets that match machineArea
		@param[out] bool, for success or failure	*/
		bool setup(TYPE machineArea);
		/*! setup function using version and machineArea parameters
		@param[in] version (a placeholder for future extensions)
		@param[in] machineArea, only setup magnets that match machineArea
		@param[out] bool, for success or failure	*/
		bool setup(const std::string& version, TYPE machineArea);
		/*! setup function using std::vector of machineAreas 
		@param[in] machineAreas, only setup magnets that match an area in machineAreas
		@param[out] bool, for success or failure	*/
		bool setup(const std::vector<TYPE>& machineAreas);
		/*! setup function using python::list of machineAreas
		@param[in] machineAreas, only setup magnets that match an area in machineAreas
		@param[out] bool, for success or failure	*/
		bool setup(const boost::python::list& machineAreas);
		/*! setup function using python::list of machineAreas
		@param[in] version, (a placeholder for future extensions)
		@param[in] machineAreas, only setup magnets that match an area in machineAreas	
		@param[out] bool, for success or failure	*/
		bool setup(const std::string& version, const boost::python::list& machineAreas);
		/*! setup function using std::vector of machineAreas
		@param[in] version, (a placeholder for future extensions)
		@param[in] machineAreas, only setup magnets that match an area in machineAreas  
		@param[out] bool, for success or failure	*/
		bool setup(const std::string& version, const std::vector<TYPE>& machineAreas);


		std::map<std::string, HardwareSnapshot> getSnapshot();
		boost::python::dict getSnapshot_Py();
		bool saveSnapshot(const std::string& location, const std::string& filename);
		bool importSnapshotToMagnets(const std::string location, const std::string& stateFile);



		/*! Get a reference to magnet object, will return a dummy_magnet if magnetName is not found  
		@param[in] magnetName, full-name or name-alias of magnet object to return 
		@param[out] Magnet&, returned magnet object */
		Magnet& getMagnet(const std::string& magnetName);

		/*! Get the full name of every magnet in the factory (c++ version)
		@param[out] vector of magnet names*/
		std::vector<std::string> getAllMagnetNames()const;
		/*! Get the full name of every magnet in the factory (python version)
		@param[out] python::list of magnet names*/
		boost::python::list getAllMagnetNames_Py()const;

		/*! Get the full name of every dipole magnet in the factory (c++ version)
		@param[out] vector of magnet names*/
		std::vector<std::string> getAllDipoleNames()const;
		/*! Get the full name of every quad magnet in the factory (python version)
		@param[out] python::list of magnet names*/
		boost::python::list getAllDipoleNames_Py()const;

		/*! Get the full name of every quad magnet in the factory (c++ version)
		@param[out] vector of magnet names*/
		std::vector<std::string> getAllQuadNames()const;
		/*! Get the full name of every quad magnet in the factory (python version)
		@param[out] python::list of magnet names*/
		boost::python::list getAllQuadNames_Py()const;

		/*! Get the full name of every solenoid magnet in the factory (c++ version)
		@param[out] vector of magnet names*/
		std::vector<std::string> getAllSolNames()const;
		/*! Get the full name of every solenoid magnet in the factory (python version)
		@param[out] python::list of magnet names*/
		boost::python::list getAllSolNames_Py()const;

		/*! Get the full name of every corrector magnet in the factory (c++ version)
		@param[out] vector of magnet names*/
		std::vector<std::string> getAllCorrectorNames()const;
		/*! Get the full name of every corrector magnet in the factory (python version)
		@param[out] python::list of magnet names*/
		boost::python::list getAllCorrectorNames_Py()const;

		/*! Get the full name of every horizontal corrector magnet in the factory (c++ version)
		@param[out] vector of magnet names*/
		std::vector<std::string> getAllHCorrectorNames()const;
		/*! Get the full name of every horizontal corrector magnet in the factory (python version)
		@param[out] python::list of magnet names*/
		boost::python::list getAllHCorrectorNames_Py()const;

		/*! Get the full name of every vertical corrector magnet in the factory (c++ version)
		@param[out] vector of magnet names*/
		std::vector<std::string> getAllVCorrectorNames()const;
		/*! Get the full name of every vertical corrector magnet in the factory (python version)
		@param[out] python::list of magnet names*/
		boost::python::list getAllVCorrectorNames_Py()const;

		/*! Returns the GETSETI value for a magnet. GETSETI is the requested set current.  
		@param[in] name, full-name or name-alias of magnet object 
		@param[out] value, if name can't be found std::min is returned */
		double getSETI(const std::string& name)const;
		/*! Returns the GETSETI value for std::vector of a magnet names
		@param[in] names, full-names or name-aliases of magnets
		@param[out] std::map of <name, value>, if name can't be found std::min is returned */
		std::map<std::string, double> getSETIs(const std::vector<std::string>& names) const;
		/*! Returns the GETSETI value for python::list of a magnet names
		@param[in] names, full-names or name-aliases of magnets
		@param[out] python::dict of <name, value>, if name can't be found std::min is returned */
		boost::python::dict getSETIs_Py(const boost::python::list& names) const;
		/*! Returns the GETSETI values for all magnets in the factory   (c++ version)
		@param[out] std::map of <name, value>, */
		std::map<std::string, double> getAllSETI() const;   
		/*! Returns the GETSETI values for all magnets in the factory (python version)
		@param[out] python::dict of <name, value>, */
		boost::python::dict getAllSETI_Py() const;

		/*! Returns the READI value for a magnet, READI is the (crudely estimated!) current in the magnet coils 
		@param[in] name, full-name or name-alias of magnet object
		@param[out] value, if name can't be found std::min is returned */
		double getREADI(const std::string& name) const;
		/*! Returns the READI value for std::vector of a magnet names
		@param[in] names, full-names or name-aliases of magnets
		@param[out] std::map of <name, value>, if name can't be found std::min is returned */
		std::map<std::string, double> getREADIs(const std::vector<std::string>& names) const;
		/*! Returns the READI value for python::list of a magnet names
		@param[in] names, full-names or name-aliases of magnets
		@param[out] python::dict of <name, value>, if name can't be found std::min is returned */
		boost::python::dict getREADI_Py(const boost::python::list& names) const;
		/*! Returns the READI values for all magnets in the factory  (c++ version)
		@param[out] std::map of <name, value>, */
		std::map<std::string, double> getAllREADI() const;   
		/*! Returns the READI values for all magnets in the factory (python version)
		@param[out] python::dict of <name, value>, */
		boost::python::dict getAllREADI_Py() const;
		
		/*! Returns the PSU STATE for a magnet, i.e ON, OFF, ERROR, ... 
		@param[in] name, full-name or name-alias of magnet object
		@param[out] value, if name can't be found std::min is returned */
		STATE getPSUState(const std::string& name) const;
		/*! Returns the PSU STATE  value for std::vector of a magnet names
		@param[in] names, full-names or name-aliases of magnets
		@param[out] std::map of <name, value>, if name can't be found std::min is returned */
		std::map<std::string, STATE> getPSUStates(const std::vector<std::string>& namess) const;
		/*! Returns the PSU STATE  value for python::list of a magnet names
		@param[in] names, full-names or name-aliases of magnets
		@param[out] python::dict of <name, value>, if name can't be found std::min is returned */
		boost::python::dict getPSUState_Py(const boost::python::list& names) const;
		/*! Returns the SU STATE  values for all magnets in the factory  (c++ version)
		@param[out] std::map of <name, value>, */
		std::map<std::string, STATE> getAllPSUState() const;
		/*! Returns the PSU STATE  values for all magnets in the factory (python version)
		@param[out] python::dict of <name, value>, */
		boost::python::dict getAllPSUState_Py() const;
		
		/*! Returns the interlock STATE for a magnet, i.e GOOD, BAD, ...
		@param[in] name, full-name or name-alias of magnet object
		@param[out] value, if name can't be found std::min is returned */
		STATE getILKState(const std::string& name) const;
		/*! Returns the interlock STATE  value for std::vector of a magnet names
		@param[in] names, full-names or name-aliases of magnets
		@param[out] std::map of <name, value>, if name can't be found std::min is returned */
		std::map<std::string, STATE> getILKStates(const std::vector<std::string>& names) const;
		/*! Returns the interlock STATE  value for python::list of a magnet names
		@param[in] names, full-names or name-aliases of magnets
		@param[out] python::dict of <name, value>, if name can't be found std::min is returned */
		boost::python::dict getILKState_Py(const boost::python::list& names) const;
		/*! Returns the interlock STATE  values for all magnets in the factory (c++ version)
		@param[out] std::map of <name, value>, */
		std::map<std::string, STATE>  getAllILKState() const;
		/*! Returns the interlock STATE  values for all magnets in the factory (python version)
		@param[out] python::dict of <name, value>, */
		boost::python::dict getAllILKState_Py() const;
		
		
		// TODO
		/*! set the ILCK state for the magnet (only available for a magnet factory in offline mode) 
		@param[in] name, full-names or name-aliases of magnet
		@param[in] value, new STATE to set 
		@param[out] std::map of <name, value>, if name can't be found std::min is returned */
		//bool offlineSetILKState(const std::string& name, const STATE value);




		/*! Set the current for a magnet object 
		@param[in] name, full-name or name-alias of magnet object
		@param[in] value, new current value 
		@param[out] state, either STATE::SUCCESS, STATE::FAIL if the comnmad was succesfully sent to epics \
		(not if the magnet actually set the requested current, or STATE::UNKNOWN_NAME if the passed name cannot be found*/
		STATE SETI(const std::string& name, const double &value);
		/*! Set the current for a map of multiple magnets and values 
		@param[in] namesAndCurrentsMap, map of new magnet currents keyed by the magnet object full-name or name-alias 
		@param[out] std::map of return states for each magnet, keyed by the passed names */
		std::map<std::string, STATE> SETI(const std::map<std::string, double> &namesAndCurrentsMap);
		/*! Set the current for a python::dict of multiple magnets and values
		@param[in] namesAndCurrentsMap, map of new magnet currents keyed by the magnet object full-name or name-alias
		@param[out] python::dict of return states for each magnet, keyed by the passed names */
		boost::python::dict SETI_Py(const boost::python::dict& namesAndCurrentsMap);
		/*! Set the current for all magnets in the magnetFactory to zero  (c++ version)
		@param[out] python::dict of return states for each magnet, keyed by their full-name*/
		std::map<std::string, STATE> SETIAllZero();
		/*! Set the current for all magnets in the magnetFactory to zero (python version)
		@param[out] python::dict of return states from each magnet, keyed by their full-name*/
		boost::python::dict SETIAllZero_Py();



		/*! Set the assumed beam momentum (MeV/c) used for magnet field strength / bend angle calculations
		@param[in] name, full-name or name-alias of magnet object
		@param[in] value, new momentum value
		@param[out] result  */
		STATE setKSetP(const std::string& name, const double value);

		/*! Set the assumed beam momentum (MeV/c) used magnet field strength / bend angle calculations for a vector of magnets
		@param[in] names, vector of magnet full-names or name-aliases of magnet object
		@param[in] value, new momentum value
		@param[out] result  */
		std::map<std::string, STATE> setKSetP(const std::vector<std::string>& names, const double value);

		/*! Set the assumed beam momentum (MeV/c) used magnet field strength / bend angle calculations for a list of magnets
		@param[in] names, list of magnet full-names or name-aliases of magnet object
		@param[in] value, new momentum value
		@param[out] result  */
		boost::python::dict  setKSetP_Py(const boost::python::list& names, const double value);

		/*! Set the assumed beam momentum (MeV/c) used for magnet field strength / bend angle calculations for magnets in a machine area
		@param[in] area, machine area to apply settings to 
		@param[in] value, new momentum value
		@param[out] result  */
		std::map<std::string, STATE> setKSetP(const TYPE area, const double value);
		/*! Set the assumed beam momentum (MeV/c) used for magnet field strength / bend angle calculations for magnets in a machine area (python version)
		@param[in] area, machine area to apply settings to
		@param[in] value, new momentum value
		@param[out] result  */
		boost::python::dict setKSetP_area_Py(TYPE area, const double value);
		/*! Set the assumed beam momentum (MeV/c) used for magnet field strength / bend angle calculations for magnets in a vector of machine areas
		@param[in] area, machine areas to apply settings to
		@param[in] value, new momentum value
		@param[out] result  */
		std::map<std::string, STATE> setKSetP(const std::vector<TYPE>& areas, const double value);

		/*! Set the assumed beam momentum (MeV/c) used for magnet field strength / bend angle calculations for magnets in a python list of machine areas
		@param[in] area, machine areas to apply settings to
		@param[in] value, new momentum value
		@param[out] result  */
		boost::python::dict setKSetP_Areas_py(const boost::python::list& areas, const double value);



		/*! Switch ON the PSU for a magnet 
		@param[in] name, full-name or name-alias of magnet object
		@param[out] state, either STATE::SUCCESS, STATE::FAIL if the comnmad was succesfully sent to epics \
		(not if the magnet actually switched on, or STATE::UNKNOWN_NAME if the passed name cannot be found*/
		STATE switchOn(const std::string& name);
		/*! Switch ON the PSUs for a std::vector of magnets  (c++ version)
		@param[in] names, std::vector magnet object full-names or name-aliases
		@param[out] std::map of return states for each magnet, keyed by the passed names */
		std::map<std::string, STATE> switchOn(const std::vector<std::string>& names);
		/*! Switch ON the PSUs for a python::list of magnets  (python version)
		@param[in] names, magnet object full-names or name-aliases
		@param[out] python::dict of return states for each magnet, keyed by the passed names */
		boost::python::dict switchOn_Py(const boost::python::list names);
		/*! Switch ON the PSU for all magnets in the magnetFactory (c++ version)
		@param[out] std::map of return states for each magnet, keyed by their full-names */
		std::map<std::string, STATE> switchOnAll();
		/*! Switch ON the PSU for all magnets in the magnetFactory (python version)
		@param[out] python::dict  of return states for each magnet, keyed by their full-names */
		boost::python::dict switchOnAll_Py();
		
		/*! Switch OFF the PSU for a magnet
		@param[in] name, full-name or name-alias of magnet object
		@param[out] state, either STATE::SUCCESS, STATE::FAIL if the comnmad was succesfully sent to epics \
		(not if the magnet actually switched off, or STATE::UNKNOWN_NAME if the passed name cannot be found*/
		STATE switchOff(const std::string& name);
		/*! Switch OFF the PSUs for a std::vector of magnets  (c++ version)
		@param[in] names, std::vector magnet object full-names or name-aliases
		@param[out] std::map of return states for each magnet, keyed by the passed names */
		std::map<std::string, STATE> switchOff(const std::vector<std::string>& names);
		/*! Switch OFF the PSUs for a python::list of magnets  (python version)
		@param[in] names, magnet object full-names or name-aliases
		@param[out] python::dict of return states for each magnet, keyed by the passed names */
		boost::python::dict switchOff_Py(const boost::python::list& names);
		/*! Switch OFF the PSU for all magnets in the magnetFactory (c++ version)
		@param[out] std::map of return states for each magnet, keyed by their full-names */
		std::map<std::string, STATE> switchOffAll();
		/*! Switch OFF the PSU for all magnets in the magnetFactory (python version)
		@param[out] python::dict of return states from each magnet, keyed by their full-names */
		boost::python::dict switchOffAll_Py();
			   
		/*! degauss a magnet, following the degauss settings held by the magnet object, initially defined in the magnet object \
		config files, (these can be dynamically changed, see setDegaussValues)
		@param[in] name, full-name or name-alias of magnet object
		@param[in] reset_to_zero, if TRUE sets zero after deguassing, if FALSE sets current before degaussing 
		@param[out] bool, TRUE the degauss proedure successfully started, FALSE then the magnet may already be degaussing or anotehrerror occurred*/
		bool degauss(const std::string& name, const bool reset_to_zero);
		/*! degauss multiple magnets  (c++ verion)
		@param[in] names, std::vector full-name or name-alias of magnet objects to be degaussed 
		@param[in] reset_to_zero, if TRUE sets zero after deguassing, if FALSE sets current before degaussing
		@param[out] std::map of return bool for each magnet, keyed by the passed names */
		std::map<std::string, bool> degauss(const std::vector<std::string>& names, const bool reset_to_zero);
		/*! degauss multiple magnets  (python verion)
		@param[in] names, python::list full-name or name-alias of magnet objects to be degaussed
		@param[in] reset_to_zero, if TRUE sets zero after deguassing, if FALSE sets current before degaussing
		@param[out] python::dict of return bool for each magnet, keyed by the passed names */
		boost::python::dict degauss_Py(const boost::python::list& names, const bool reset_to_zero);
		/*! degauss all magnets in the magnet factory (c++verion)
		@param[in] reset_to_zero, if TRUE sets zero after deguassing, if FALSE sets current before degaussing
		@param[out] std::map of return bool for each magnet, keyed by the full-name */
		std::map<std::string, bool> degaussALL(const bool reset_to_zero);
		/*! degauss all magnets in the magnet factory (python verion)
		@param[in] reset_to_zero, if TRUE sets zero after deguassing, if FALSE sets current before degaussing
		@param[out] python::dict of return bool for each magnet, keyed by the full-name */
		boost::python::dict degaussAll_Py(const bool reset_to_zero);

		//int setNumberOfDegaussSteps(const int value); 	//TODO: THINK ABOUT THIS ONE!!! 
		
		/*! set the values in the degauss proccedure for a magnet (can be useful for "quick" deguassing when the previous (c++ version)
		field history of the magnet is well known) 
		@param[in] name, full-name or name-alias of magnet object
		@param[in] values, vetcor oif values to use when degaussing 
		@param[out] values to be used when degaussing (should match passed values) */
		std::vector<double> setDegaussValues(const std::string& name, const std::vector<double>& values);
		/*! set the values in the degauss procedure for a magnet (Python version)
		@param[in] name, full-name or name-alias of magnet object
		@param[in] values, python::list of values to use when degaussing
		@param[out] values to be used when degaussing (should match passed values) */
		boost::python::list setDegaussValuesSingle_Py(const std::string& name, const boost::python::list& values);
		/*! set the values in the degauss procedure for multiple magnets (c++ version)
		@param[in] names, full-name or name-alias of magnet object
		@param[in] values, std::vector of values to use when degaussing
		@param[out] values to be used when degaussing (should match passed values) */
		std::map<std::string, std::vector<double>> setDegaussValues(const std::vector < std::string>& names, const std::vector<double>& values);
		/*! set the values in the degauss procedure for multiple magnets (python version)
		@param[in] names, full-name or name-alias of magnet object
		@param[in] values, python::list  of values to use when degaussing
		@param[out] values to be used when degaussing (should match passed values) */
		boost::python::dict setDegaussValuesMulti_Py(const boost::python::list& names, const boost::python::list& values);

		/*! Get the values used in the degauss proccedure for a magnet (c++ version)
		@param[in] name, full-name or name-alias of magnet object
		@param[out] values to be used when degaussing */
		std::vector<double> getDegaussValues(const std::string& name) const;
		/*! Get the values used in the degauss proccedure for a magnet (Python version)
		@param[in] name, full-name or name-alias of magnet object
		@param[out] values to be used when degaussing */
		boost::python::list getDegaussValuesSingle_Py(const std::string& name) const;
		/*! Get the values used in the degauss proccedure for multiple magnets (c++ version)
		@param[in] names, std::vector of full-name or name-alias of magnet objects
		@param[out] std::map of values to be used keyed by the passed magnet names */
		std::map<std::string, std::vector<double>> getDegaussValues(const std::vector<std::string>& names) const;
		/*! Get the values used in the degauss proccedure for multiple magnets (c++ version)
		@param[in] names, python::list of full-name or name-alias of magnet objects
		@param[out] python::dict of values to be used keyed by the passed magnet names */
		boost::python::dict getDegaussValuesMulti_Py(const boost::python::list& name) const;

		/*! Set the tolerance used when checking if magnets have reached the desired current values during the degauss procedure. 
		During deguassing the READI values must match the requested SETI to within this tolerance before the next current value is applied.
		@param[in] name, full-name or name-alias of magnet object
		@param[in] value to use when degaussing
		@param[out] value of degauss tolerance, (should match passed value) */
		double setDegaussTolerance(const std::string& name, const double value);
		/*! Set the degauss tolerance for multiple magnets (c++ version).
		@param[in] names, std::vector of full-name or name-alias of magnet object
		@param[in] value to use when degaussing
		@param[out] std::map of degauss tolerances keyed by the passed names, (should match passed value) */
		std::map<std::string, double> setDegaussTolerance(const std::vector < std::string>& names, const double value);
		/*! Set the degauss tolerance for multiple magnets (c++ version).
		@param[in] names, python::list of full-name or name-alias of magnet object
		@param[in] value to use when degaussing
		@param[out] python::dict of degauss tolerances keyed by the passed names, (should match passed value) */
		boost::python::dict setDegaussTolerance_Py(const boost::python::list& name, const double value);

		/*! Get the tolerance used when checking if magnets have reached the desired current values during the degauss procedure.
		During deguassing the READI values must match the requested SETI to within this tolerance before the next current value is applied.
		@param[in] name, full-name or name-alias of magnet object
		@param[out] value of degauss tolerance,*/
		double getDegaussTolerance(const std::string& name) const;
		/*! Get the degauss tolerance for multiple magnets (c++ version).
		@param[in] names, std::vector of full-name or name-alias of magnet object
		@param[out] std::map of degauss tolerances keyed by the passed names, (should match passed value) */
		std::map<std::string, double> getDegaussTolerance(const std::vector < std::string>& names) const;
		/*! Get the degauss tolerance for multiple magnets (c++ version).
		@param[in] names, python::list of full-name or name-alias of magnet object
		@param[in] value to use when degaussing
		@param[out] python::dict of degauss tolerances keyed by the passed names, (should match passed value) */
		boost::python::dict getDegaussTolerance_Py(const boost::python::list& name) const;


		/*! Get the name aliases of a magnet. Name alisases can be used as shorthand version of the full magnet name. 
		The full name is defined by the PV root used to control a magnet (c++ verion).  
		@param[in] name, full-name or name-alias of magnet object
		@param[out] std::vector of names alises that can be used to refer to the passed name */
		std::vector<std::string> getAliases(const std::string& name) const;
		/*! Get the name aliases of a magnet.Name alisases can be used as shorthand version of the full magnet name.
		The full name is defined by the PV root used to control a magnet (python verion).
		@param[in] name, full-name or name-alias of magnet object
		@param[out] python::list of names alises that can be used to refer to the passed name */
		boost::python::list getAliases_Py1(const std::string& name) const;
		/*! Get the name aliases of multiple magnets (c++ verion).
		@param[in] names, std:vector of full-names or name-alias of magnet object
		@param[out] std::map of names alises that can be used to refer to the passed name, keyed by the passed name */
		std::map<std::string, std::vector<std::string>> getAliases(const std::vector<std::string>& names) const;
		/*! Get the name aliases of multiple magnets (python verion).
		@param[in] names, python::list of full-names or name-alias of magnet object
		@param[out] python::dict of names alises that can be used to refer to the passed name, keyed by the passed name */
		boost::python::dict getAliases_Py2(const boost::python::list& name) const;
		
		/*! Get the magnet manufacturer. 
		@param[in] name, full-name or name-alias of magnet object
		@param[out] manufacturer name */
		std::string getManufacturer(const std::string& name) const;
		/*! Get the manufacturer for mutiple magnets (c++ verions).
		@param[in] names, std::vector of full-name or name-alias of magnet object
		@param[out] std::map of manufacturer names keyed by the passed magnet name */
		std::map<std::string, std::string> getManufacturer(const std::vector<std::string>& name) const;
		/*! Get the manufacturer for mutiple magnets (python verions).
		@param[in] names,python::list pf full-names or name-aliases of magnet object
		@param[out] python::dict of manufacturer names keyed by the passed magnet name */
		boost::python::dict getManufacturer_Py(const boost::python::list& name) const;

		// TODO Should be string
		int getSerialNumber(const std::string& name) const;
		std::map<std::string, int> getSerialNumber(const std::vector<std::string>& name) const;
		boost::python::dict getserial_number_Py(const boost::python::list& name) const;
		
		/*! Get magnet TYPE 
		@param[in] name, full-name or name-alias of magnet object
		@param[out] magnet object TYPE, (QUADRUPOLE, DIPOLE, etc.)  */
		TYPE getMagnetType(const std::string& name) const;
		/*! Get the TYPE for multiple magnets  (c++ version)
		@param[in] names, std::vector of full-names or name-aliases of magnet objects
		@param[out] std::map of magnet object TYPEs keyed by the passed names */
		std::map<std::string, TYPE> getMagnetType(const std::vector<std::string>& name) const;
		/*! Get the TYPE for multiple magnets  (python version)
		@param[in] names, python::list of full-names or name-aliases of magnet objects
		@param[out] magnet object TYPE, (QUADRUPOLE, DIPOLE, etc.)  
		@param[out] python::dict of magnet object TYPEs keyed by the passed names */
		boost::python::dict getMagnetType_Py(const boost::python::list& name) const;


		/*! Get the TYPE of PSU polarity reversing for a magnet 
		@param[in] name, full-name or name-alias of magnet object
		@param[out] magnet object TYPE, (BIPOLAR, NR-SWICTHED, etc.)  */
		std::string getMagnetRevType(const std::string& name) const;
		/*! Get the TYPE for multiple magnets  (c++ version)
		@param[in] names, std::vector of full-names or name-aliases of magnet objects
		@param[out] std::map of magnet object TYPEs keyed by the passed names */
		std::map<std::string, std::string> getMagnetRevType(const std::vector<std::string>& name) const;
		/*! Get the TYPE for multiple magnets  (python version)
		@param[in] names, python::list of full-names or name-aliases of magnet objects
		@param[out] python::dict of magnet object TYPEs keyed by the passed names */
		boost::python::dict getMagnetRevType_Py(const boost::python::list& name) const;

		/*! Get the magnetic length of a magnet 
		@param[in] name, full-name or name-alias of magnet object
		@param[out] magnetic length */
		double getMagneticLength(const std::string& name) const;
		/*! Get the magnetic length for multiple magnets  (c++ version)
		@param[in] names, std::vector of full-names or name-aliases of magnet objects
		@param[out] std::map of magnet object lengths keyed by the passed names */
		std::map<std::string, double> getMagneticLength(const std::vector<std::string>& name) const;
		/*! Get the magnetic length for multiple magnets  (python version)
		@param[in] names, python::list of full-names or name-aliases of magnet objects
		@param[out] python::dict of magnetc lengths keyed by the passed names */
		boost::python::dict getMagneticLength_Py(const boost::python::list& name) const;
		
		/*! Get the full-name of the PSU for a magnet (some magnets, typically correctors, are on a shared PSU)
		@param[in] name, full-name or name-alias of magnet object
		@param[out] PSU name */
		std::string getFullPSUName(const std::string& name) const;
		/*! Get the full-name of the PSU for multiple magnets  (c++ version)
		@param[in] names, std::vector of full-names or name-aliases of magnet objects
		@param[out] std::map of magnet object PSU full-names keyed by the passed names */
		std::map<std::string, std::string>getFullPSUName(const std::vector<std::string>& name) const;
		/*! Get the TYPE for multiple magnets  (python version)
		@param[in] names, python::list of full-names or name-aliases of magnet objects
		@param[out] python::dict of magnet object PSU full-names keyed by the passed names */
		boost::python::dict getFullPSUName_Py(const boost::python::list& name) const;
		
		/*! Get the path and filename for the mmagnet measurement data 
		@param[in] name, full-name or name-alias of magnet object
		@param[out] magnet measurement data location */
		std::string getMeasurementDataLocation(const std::string& name) const;
		/*! Get the path and filename for the measurement data of multiple magnets  (c++ version)
		@param[in] names, std::vector of full-names or name-aliases of magnet objects
		@param[out] std::map of magnet object measuremetn data paths and filenames for keyed by the passed names */
		std::map<std::string, std::string> getMeasurementDataLocation(const std::vector<std::string>& name) const;
		/*! Get the path and filename for the measurement data of multiple magnets  (python version)
		@param[in] names, python::list of full-names or name-aliases of magnet objects
		@param[out] python::dict of magnet object TYPEs keyed by the passed names */
		boost::python::dict getMeasurementDataLocation_Py(const boost::python::list& name) const;
		
		/*! Get the number of steps in the degauss procedure 
		@param[in] name, full-name or name-alias of magnet object
		@param[out] number of steps */
		size_t getNumberOfDegaussSteps(const std::string& name) const;
		/*! Get the number of steps in the degauss procedure  multiple magnets  (c++ version)
		@param[in] names, std::vector of full-names or name-aliases of magnet objects
		@param[out] std::map of magnet number of degauss steps keyed by the passed names */
		std::map<std::string, size_t> getNumberOfDegaussSteps(const std::vector<std::string>& name) const;
		/*! Get the number of steps in the degauss procedure  multiple magnets  (c++ version)
		@param[in] names, python::list of full-names or name-aliases of magnet objects
		@param[out] python::dict of magnet number of degauss steps keyed by the passed names */
		boost::python::dict  getNumberOfDegaussSteps_Py(const boost::python::list& name) const;


		/*! Get the READI tolerance of a magnet. This tolerance is used when determining if the READ is equal the SETI.
		We check if READI == SETI (to within a tolerance), when waiting for magnets to reach the requested current.
		Initially defined in the config file
		@param[in] name, full-name or name-alias of magnet object
		@param[out] magnetic length */
		double getREADITolerance(const std::string& name) const;
		/*!  Get the READI tolerance for multiple magnets  (c++ version)
		@param[in] names, std::vector of full-names or name-aliases of magnet objects
		@param[out] std::map of magnet READI tolerances keyed by the passed names */
		std::map<std::string, double> getREADITolerance(const std::vector < std::string>& names) const;
		/*! Get the TYPE for multiple magnets  (python version)
		@param[in] names, python::list of full-names or name-aliases of magnet objects
		@param[out] python::dict of magnet READI tolerances  keyed by the passed names */
		boost::python::dict getREADITolerance_Py(const boost::python::list& name) const;

		/*! Set the READI tolerance of a magnet. This tolerance is used when determining if the READ is equal the SETI.
		We check if READI == SETI (to within a tolerance), when waiting for magnets to reach the requested current.
		Initially defined in the config file
		@param[in] name, full-name or name-alias of magnet object
		@param[out] magnetic length */
		double setREADITolerance(const std::string& name, const double value);
		/*!  Set the READI tolerance for multiple magnets  (c++ version)
		@param[in] names, std::vector of full-names or name-aliases of magnet objects
		@param[out] std::map of magnet READI tolerances keyed by the passed names */
		std::map<std::string, double> setREADITolerance(const std::vector < std::string>& names, const double value);
		/*! Set the TYPE for multiple magnets  (python version)
		@param[in] names, python::list of full-names or name-aliases of magnet objects
		@param[out] python::dict of magnet READI tolerances  keyed by the passed names */
		boost::python::dict setREADITolerance_Py(const boost::python::list& names, const double value);

		/*! Get the minimum SETI value that can be applied, as defined in the config file.
		@param[in] name, full-name or name-alias of magnet object
		@param[out] minimum SETI */
		double getMinI(const std::string& name)const;
		/*! Get the minimum SETI value for multiple magnets (c++ version)
		@param[in] names, std::vector of full-names or name-aliases of magnet objects
		@param[out] std::map of magnet  minimum SETIs keyed by the passed names */
		std::map<std::string, double> getMinIs(const std::vector<std::string>& names) const;
		/*! Get the minimum SETI value for multiple magnets (python version)
		@param[in] names, python::list of full-names or name-aliases of magnet objects
		@param[out] python::dict of magnet  minimum SETIs keyed by the passed names */
		boost::python::dict getMinIs_Py(const boost::python::list& names) const;
		/*! Get the minimum SETI value for all magnets (c++ version)
		@param[in] names, std::vector of full-names or name-aliases of magnet objects
		@param[out] std::map of magnet minimum SETIs keyed by the magnet names */
		std::map<std::string, double> getAllMinI() const;
		/*! Get the minimum SETI value for all magnets (Python version)
		@param[in] names, python::list of full-names or name-aliases of magnet objects
		@param[out] python::dict of magnet minimum SETIs keyed by the magnet names */
		boost::python::dict getAllMinI_Py() const;

		/*! Get the maximum SETI value that can be applied, as defined in the config file.
		@param[in] name, full-name or name-alias of magnet object
		@param[out] maximum SETI */
		double getMaxI(const std::string& name)const;
		/*! Get the maximum SETI value for multiple magnets (c++ version)
		@param[in] names, std::vector of full-names or name-aliases of magnet objects
		@param[out] std::map of magnet  maximum SETIs keyed by the passed names */
		std::map<std::string, double> getMaxIs(const std::vector<std::string>& names) const;
		/*! Get the maximum SETI value for multiple magnets (python version)
		@param[in] names, python::list of full-names or name-aliases of magnet objects
		@param[out] python::dict of magnet  maximum SETIs keyed by the passed names */
		boost::python::dict getMaxIs_Py(const boost::python::list& names) const;
		/*! Get the maximum SETI value for all magnets (c++ version)
		@param[in] names, std::vector of full-names or name-aliases of magnet objects
		@param[out] std::map of magnet maximum SETIs keyed by the magnet names */
		std::map<std::string, double> getAllMaxI() const;
		/*! Get the maximum SETI value for all magnets (Python version)
		@param[in] names, python::list of full-names or name-aliases of magnet objects
		@param[out] python::dict of magnet maximum SETIs keyed by the magnet names */
		boost::python::dict getAllMaxI_Py() const;
  
		/*! Get the momementum based on dipole current (?) I THINK - TO BE TESTED.
		@param[in] name, full-name or name-alias of magnet object
		@param[out] momentum*/
		double getKDipP(const std::string& name)const;
		/*! Get the momementum based on dipole current (?) I THINK - TO BE TESTED.
		@param[in] names, full-name or name-alias of magnet objects
		@param[out] momenta*/
		std::map<std::string, double> getKDipPs(const std::vector<std::string>& names) const;
		/*! Get the momementum based on dipole current (?) I THINK - TO BE TESTED. Python version
		@param[in] names, full-name or name-alias of magnet objects
		@param[out] momenta*/
		boost::python::dict getKDipPs_Py(const boost::python::list& names) const;
		/*! Get the momementum based on dipole current for each dipole (?) I THINK - TO BE TESTED.
		@param[in] names, full-name or name-alias of magnet objects
		@param[out] momenta*/
		std::map<std::string, double> getAllKDipP() const;
		/*! Get the momementum based on dipole current for each dipole (?) I THINK - TO BE TESTED. Python version
		@param[in] names, full-name or name-alias of magnet objects
		@param[out] momenta*/
		boost::python::dict getAllKDipP_Py() const;


		/*! Get the integrated strength. returns different unit: T for quads and T/mm for correctors, solenoids and dipoles
		@param[in] name, full-name or name-alias of magnet object
		@param[out] integrated strength*/
		double getIntStr_mm(const std::string& name)const;
		/*! Get the integrated strength for multiple magnets. returns different unit: T for quads and T/mm for correctors, solenoids and dipoles
		@param[in] name, full-name or name-alias of magnet object
		@param[out] integrated strength*/
		std::map<std::string, double> getIntStrs_mm(const std::vector<std::string>& names) const;
		/*! Get the integrated strength for multiple magnets Python version. returns different unit: T for quads and T/mm for correctors, solenoids and dipoles
		@param[in] name, full-name or name-alias of magnet object
		@param[out] integrated strength*/
		boost::python::dict getIntStrs_mm_Py(const boost::python::list& names) const;
		/*! Get the integrated strength for all magnets. returns different unit: T for quads and T/mm for correctors, solenoids and dipoles
		@param[in] name, full-name or name-alias of magnet object
		@param[out] integrated strength*/
		std::map<std::string, double> getAllIntStr_mm() const;
		/*! Get the integrated strength for all magnets Python version. returns different unit: T for quads and T/mm for correctors, solenoids and dipoles
		@param[in] name, full-name or name-alias of magnet object
		@param[out] integrated strength*/
		boost::python::dict getAllIntStr_mm_Py() const;


		/*! Get the integrated strength. In T 
		@param[in] name, full-name or name-alias of magnet object
		@param[out] integrated strength*/
		double getIntStr(const std::string& name)const;
		/*! Get the integrated strength for multiple magnets. In T 
		@param[in] name, full-name or name-alias of magnet object
		@param[out] integrated strength*/
		std::map<std::string, double> getIntStrs(const std::vector<std::string>& names) const;
		/*! Get the integrated strength for multiple magnets In T, Python version
		@param[in] name, full-name or name-alias of magnet object
		@param[out] integrated strength*/
		boost::python::dict getIntStrs_Py(const boost::python::list& names) const;
		/*! Get the integrated strength for all magnets. In T 
		@param[in] name, full-name or name-alias of magnet object
		@param[out] integrated strength*/
		std::map<std::string, double> getAllIntStr() const;
		/*! Get the integrated strength for all magnets, In T. Python version. 
		@param[in] name, full-name or name-alias of magnet object
		@param[out] integrated strength*/
		boost::python::dict getAllIntStr_Py() const;


		///*! Get the momentum used to calculate various 'K' parameters 
		//@param[in] name, full-name or name-alias of magnet object
		//@param[out] momentum*/
		//double getKSetP(const std::string& name)const;
		///*! Get the momentum used to calculate various 'K' parameters
		//@param[in] name, full-name or name-alias of magnet object
		//@param[out] momentum*/
		//std::map<std::string, double> getKSetPs(const std::vector<std::string>& names) const;
		///*!  Get the momentum used to calculate various 'K' parameters, Python version
		//@param[in] name, full-name or name-alias of magnet object
		//@param[out] integrated strength*/
		//boost::python::dict getKSetPs_Py(const boost::python::list& names) const;
		///*!  Get the momentum used to calculate various 'K' parameters
		//@param[in] name, full-name or name-alias of magnet object
		//@param[out] integrated strength*/
		//std::map<std::string, double> getAllKSetP() const;
		///*!  Get the momentum used to calculate various 'K' parameters, Python version.
		//@param[in] name, full-name or name-alias of magnet object
		//@param[out] integrated strength*/
		//boost::python::dict getAllKSetP_Py() const;

		/*! Get the bend angle from a dipole, or coorector 
		@param[in] name, full-name or name-alias of magnet object
		@param[out] momentum*/
		double getKAng(const std::string& name)const;
		/*! Get the bean angle from a dipole, or coorector 
		@param[in] name, full-name or name-alias of magnet object
		@param[out] momentum*/
		std::map<std::string, double> getKAngs(const std::vector<std::string>& names) const;
		/*! Get the bend angle from a dipole, or coorector  Python version
		@param[in] name, full-name or name-alias of magnet object
		@param[out] integrated strength*/
		boost::python::dict getKAngs_Py(const boost::python::list& names) const;
		/*!  Get the bend angle from a dipole, or coorector 
		@param[in] name, full-name or name-alias of magnet object
		@param[out] integrated strength*/
		std::map<std::string, double> getAllKAng() const;
		/*!  Get the bend angle from a dipole, or coorector , Python version.
		@param[in] name, full-name or name-alias of magnet object
		@param[out] integrated strength*/
		boost::python::dict getAllKAng_Py() const;
		/*!  Get the bend angle from a dipole, or coorector
		@param[in] name, full-name or name-alias of magnet object
		@param[out] integrated strength*/
		std::map<std::string, double> getAllDipoleKAng() const;
		/*!  Get the bend angle from a dipole, or coorector , Python version.
		@param[in] name, full-name or name-alias of magnet object
		@param[out] integrated strength*/
		boost::python::dict getAllDipoleKAng_Py() const;


		/*! Get the momentum used to calculate various 'K' parameters
		@param[in] name, full-name or name-alias of magnet object
		@param[out] momentum*/
		double getKSetP(const std::string& name)const;
		/*! Get the momentum used to calculate various 'K' parameters
		@param[in] name, full-name or name-alias of magnet object
		@param[out] momentum*/
		std::map<std::string, double> getKSetPs(const std::vector<std::string>& names) const;
		/*!  Get the momentum used to calculate various 'K' parameters, Python version
		@param[in] name, full-name or name-alias of magnet object
		@param[out] integrated strength*/
		boost::python::dict getKSetPs_Py(const boost::python::list& names) const;
		/*!  Get the momentum used to calculate various 'K' parameters
		@param[in] name, full-name or name-alias of magnet object
		@param[out] integrated strength*/
		std::map<std::string, double> getAllKSetP() const;
		/*!  Get the momentum used to calculate various 'K' parameters, Python version.
		@param[in] name, full-name or name-alias of magnet object
		@param[out] integrated strength*/
		boost::python::dict getAllKSetP_Py() const;

		/*! Get the bend angle in mrad (valid on correctors only?)
		@param[in] name, full-name or name-alias of magnet object
		@param[out] bend angle, mrad*/
		double getKmrad(const std::string& name)const;
		/*! Get the bend angle in mrad for mulitple magnets (valid on correctors only?)
		@param[in] name, full-name or name-alias of magnet object
		@param[out] map of magnets names and bend angles, mrad*/
		std::map<std::string, double> getKmrads(const std::vector<std::string>& names) const;
		/*! Get the bend angle in mrad for mulitple magnets, python version (valid on correctors only?)
		@param[in] name, full-name or name-alias of magnet object
		@param[out] dict of magnets names and bend angles, mrad*/
		boost::python::dict getKmrad_Py(const boost::python::list& names) const;
		/*! Get the bend angles in mrad for all magnets, (valid on correctors only?)
		@param[out] map of magnets names and bend angles, mrad*/
		std::map<std::string, double> getAllKmrad() const;
		/*! Get the bend angles in mrad for all magnets, Python version (valid on correctors only?)
		@param[out] dict of magnets names and bend angles, mrad*/
		boost::python::dict getAllKmrad_Py() const;
		/*! Get the bend angles in mrad for all corrector magnets, 
		@param[out] dict of magnets names and bend angles, mrad*/
		std::map<std::string, double> getAllCorrectorKmrad() const;
		/*! Get the bend angles in mrad for all corrector magnets, (Python version)
		@param[out] dict of magnets names and bend angles, mrad*/
		boost::python::dict getAllCorrectorKmrad_Py() const;
		/*! Get the bend angles in mrad for all corrector magnets,
		@param[out] dict of magnets names and bend angles, mrad*/
		std::map<std::string, double> getAllHCorrKmrad() const;
		/*! Get the bend angles in mrad for all horizontal corrector magnets, (Python version)
			@param[out] dict of magnets names and bend angles, mrad*/
		boost::python::dict getAllHCorrKmrad_Py() const;
		/*! Get the bend angles in mrad for all vertical corrector magnets,
		@param[out] dict of magnets names and bend angles, mrad*/
		std::map<std::string, double> getAllVCorrKmrad() const;
		/*! Get the bend angles in mrad for all vertical  corrector magnets,(Python version)
		@param[out] dict of magnets names and bend angles, mrad*/
		boost::python::dict getAllVCorrKmrad_Py() const;


		/*! Get the K factor  in m^(-2) (only really valid for Quads ??)
		@param[in] name, full-name or name-alias of magnet object
		@param[out] K  m^(-2)*/
		double getKVal(const std::string& name)const;
		/*! Get the K factor in m^(-2) for multiple magnets (only really valid for Quads)
		@param[in] name, full-name or name-alias of magnet object
		@param[out] map of magnet names, K  m^(-2)*/
		std::map<std::string, double> getKVals(const std::vector<std::string>& names) const;
		/*! Get the K factor in m^(-2) for multiple magnets (only really valid for Quads), Python version  
		@param[in] name, full-name or name-alias of magnet object
		@param[out] dict of magnet names, K  m^(-2)*/
		boost::python::dict getKVals_Py(const boost::python::list& names) const;
		/*! Get the K factor for all magnets in m^(-2) for multiple magnets (only really valid for Quads), Python version
		@param[out] dict of magnet names, K  m^(-2)*/
		std::map<std::string, double> getAllKVal() const;
		/*! Get the K factor for all magnets in m^(-2) for multiple magnets (only really valid for Quads), Python version
		@param[out] dict of magnet names, K  m^(-2)*/
		boost::python::dict getAllKVal_Py() const;

		/*! Get the K factor  in m^(-2) for each quadrupole in the magnet factory 
		@param[out] map of magnet names and K  m^(-2)*/
		std::map<std::string, double> getAllQuadKVals() const;
		/*! Get the K factor  in m^(-2) for each quadrupole in the magnet factory (Python version)
		@param[out] dict of magnet names and K  m^(-2)*/
		boost::python::dict getAllQuadKVals_Py() const;

		   		 
		/*! Check if a magnet is a particular TYPE
		@param[in] name, full-name or name-aliase of magnet 
		@param[in] type, TYPE to check against magnetType 
		@param[out] bool, result of comparison */		
		bool isAType(const std::string& name, const TYPE type)const;
		/*! Check if a magnet is a QUADRUPOLE TYPE
		@param[in] name, full-name or name-aliase of magnet
		@param[out] bool, result of comparison */
		bool isAQuad(const std::string& name)const;
		/*! Check if a magnet is a DIPOLE TYPE
		@param[in] name, full-name or name-aliase of magnet
		@param[out] bool, result of comparison */
		bool isADip(const std::string& name)const;
		/*! Check if a magnet is a SOLENOID TYPE
		@param[in] name, full-name or name-aliase of magnet
		@param[out] bool, result of comparison */
		bool isASol(const std::string& name)const;
		/*! Check if a magnet is a BUCKING_SOLENOID TYPE
		@param[in] name, full-name or name-aliase of magnet
		@param[out] bool, result of comparison */
		bool isABSol(const std::string& name)const;
		/*! Check if a magnet is a VERTICAl_CORRECTOR	TYPE
		@param[in] name, full-name or name-aliase of magnet
		@param[out] bool, result of comparison */
		bool isAVCor(const std::string& name)const;
		/*! Check if a magnet is a HORIZONTAL_CORRECTOR TYPE
		@param[in] name, full-name or name-aliase of magnet
		@param[out] bool, result of comparison */
		bool isAHCor(const std::string& name)const;
		/*! Check if a magnet is a VERTICAl_CORRECTOR or HORIZONTAL_CORRECTOR TYPE
		@param[in] name, full-name or name-aliase of magnet
		@param[out] bool, result of comparison */
		bool isACor(const std::string& name)const;

		/*! Return a magnetstate object for a magnet, a magnetsatte object defines the main settings accessed through EPICS,
		(SETI, READ, psue_state, see magnetstate for more info)
		@param[in] name, full-name or name-aliase of magnet
		@param[out] magnetstate*/
		magnetState getMagnetState(const std::string& name)const;
		/*! Return a magnetStates object for all magnets in the factory, a magnetStates has the magnetState for multiple magnets 
		@param[out] magnetsates */
		magnetStates getMagnetStates() const;

		/*! Return a magnetState as a python dictrionary  
		@param[in] name of magnet to get state for  
		@param[out] magnetState as pythjon dict  */
		boost::python::dict getMagnetState_Py(const std::string& name)const;
		/*! Return magnetState for all magnets in the factory, 
		@param[out] python dictionary version of a magnetStates */
		boost::python::dict getAllMagnetState_Py() const;


		/*! Set the magnet states for a magnet 
		@param[in] magnetsate to be applied  
		@param[out] bool, TRUE if magnet could be found in Factory and magnet.setMagnetState returns TRUE, otherwise FALSE */
		bool setMagnetState(const magnetState& ms) ;

		/*! Set the magnet states for all magnets defined in the magnetstate object, that are of a TYPE passed 
		@param[in] magnet states to be applied
		@param[in] types, std::vector of magnet TYPE that a magnetstate will be applied to 
		@param[out] magnetsate */
		std::map<std::string, bool> applyMagnetStates(const magnetStates& ms, const std::vector<TYPE>& types);
							   

		/*! Get the names of the magnets that are in an area 
		@param[in] TYPE, area 
		@param[out] std::vector<std::string>, names of magnets found in area  */
		std::vector<std::string> getNamesInArea(TYPE area) const;
		/*! Get the names of the magnets that are in an area
		@param[in] TYPE, area
		@param[out] boost::python::list, names of magnets found in area  */
		boost::python::list getNamesInArea_Py(TYPE area) const;
		/*! Get the names of the magnets that are in multiple areas
		@param[in] std::vector<TYPE>, areas
		@param[out] std::vector<std::string>, names of magnets found in area  */
		std::vector<std::string> getNamesInAreas(const std::vector<TYPE>& areas) const;
		/*! Get the names of the magnets that are  in multiple areas
		@param[in] TYPE, area
		@param[out] boost::python::list, names of magnets found in area  */
		boost::python::list getNamesInAreas_Py(const boost::python::list& areas) const;


		/*! Write a DBURT file to the default location 
		@param[in] DBURT filename to write
		@param[out] bool, if file was succesfully wwritten or not */
		bool writeDBURT(const std::string& fileName)const;
		/*! Write a DBURT file to the default location with passed comments 
		@param[in] DBURT filename to write
		@param[in] DBURT comments to write to file 
		@param[out] bool, if file was succesfully wwritten or not */
		bool writeDBURT(const std::string& fileName, const std::string& commment)const;
		/*! Write a DBURT file to the passed location with passed comments
		@param[in] DBURT filepath, location wher efile will be written 
		@param[in] DBURT filename to write
		@param[in] DBURT comments to write to file
		@param[out] bool, if file was succesfully wwritten or not */
		bool writeDBURT(const std::string& filePath, const std::string& fileName, const std::string& commment)const;

		/*! Read a DBURT file from the default location
		@param[in] DBURT filename to read 
		@param[out] dburt, dburt object generated after file has been parsed */
		dburt readDBURT(const std::string& fileName)const;
		/*! Read a DBURT file from a passed location 
		@param[in] DBURT filepath to look for file 
		@param[in] DBURT filename to read
		@param[out] dburt, dburt object generated after file has been parsed */
		dburt readDBURT(const std::string& filePath, const std::string& fileName)const;

		/*! Apply a DBURT file from passed location
		@param[in] DBURT filepath to file 
		@param[in] DBURT filename to read
		@param[out] bool, if dburt was applied */
		bool applyDBURT(const std::string& filePath, const std::string& fileName);
		/*! Apply a DBURT file to quadrupole magnets only  from passed location
		@param[in] DBURT filepath to file
		@param[in] DBURT filename to read
		@param[out] bool, if dburt was applied */
		bool applyDBURTQuadOnly(const std::string& filePath,  const std::string& fileName);
		/*! Apply a DBURT file to corrector magnets only from passed location
		@param[in] DBURT filepath to file
		@param[in] DBURT filename to read
		@param[out] bool, if dburt was applied */
		bool applyDBURTCorOnly(const std::string& filePath, const std::string& fileName);

		/*! Apply a DBURT file from the default location 
		@param[in] DBURT filename to read
		@param[out] bool, if dburt was applied */
		bool applyDBURT(const std::string& fileName);
		/*! Apply a DBURT file to quadrupole magnets only  from passed location
		@param[in] DBURT filename to read
		@param[out] bool, if dburt was applied */
		bool applyDBURTQuadOnly(const std::string& fileName);
		/*! Apply a DBURT file to corrector magnets only from passed location
		@param[in] DBURT filename to read
		@param[out] bool, if dburt was applied */
		bool applyDBURTCorOnly(const std::string& fileName);

		/*! Compare a DBURT file in teh default lcoation to the curent maget settings 
		@param[in] DBURT filename to read
		@param[out] bool, if dburt file matches current settings */
		bool isMagnetStateEqualDBURT(const std::string& fileName);


		/*! Get the full name of a magnet 
		@param[in] name to lookup 
		@param[out] fullname of magnet, or dummy magnet name if passed name does not exist*/
		std::string getFullName(const std::string& name_to_check) const;






		// private
		void populateMagnetMap();
		void setMonitorStatus(pvStruct& pvStruct);
		// private
		bool hasBeenSetup;
		// offline physical or virtual 
		STATE mode;


		/*! turns debug messages on for valveFactory and calls same function in all valves and configReader*/
		void debugMessagesOn();
		/*! turns debug messages off for valveFactory and calls same function in all valves and configReader*/
		void debugMessagesOff();
		/*! turns messages on for valveFactory and calls same function in all valves and configReader*/
		void messagesOn();
		/*! turns messages off for valveFactory and calls same function in all valves and configReader*/
		void messagesOff();
		/*! returns true if messenger debug flag is true, false otherwise*/
		bool isDebugOn();
		/*! returns true if messenger messages flag is true, false otherwise*/
		bool isMessagingOn();

		LoggingSystem messenger;
private:

		std::map<std::string, Magnet> magnetMap;


		/*! setup the EPCIS channels */
		void setupChannels();

		/*! Write a dburt file to disk, private function that does the actual writing
		@param[in] path object 
		@param[in] dburt data 
		@param[out] successfully written */
		bool writeDBURTToFile(const boost::filesystem::path& full_path, const dburt& dburt_to_write) const;
		/*! Read a dburt file from disk, private function that does the actual reading
		@param[in] path object
		@param[out] dburt data*/
		dburt readDBURTFile(const boost::filesystem::path& full_path) const;
		std::pair<bool, std::string> isDBURTFileAlias(const std::string& full_path)const;

		/*! Update the alias-name-map, 
		@param[in] magnet object top updat emap with */
		void updateAliasNameMap(const Magnet& magnet);
		std::map<std::string, std::string> alias_name_map;

		// dummy_magnet is used to return values when users ask for a nOn-existing magnet's properties 
		Magnet dummy_magnet;

		/* which areas need to be included */
		std::vector<TYPE> machineAreas;
		/*! Delete magnet objects that are not of the machineArea TYPE for this factory, called during setup*/
		void cutMagnetMapByMachineAreas();

		// private
		ConfigReader reader;
};
#endif // MAGNET_FACTORY_H_