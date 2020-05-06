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
{   // magnetStates a default constructor
	magnetStates():
		numMags(GlobalConstants::zero_sizet) 
		{};
	size_t numMags;
	std::map<std::string, magnetState> magnetStatesMap;
	boost::python::dict magnetStateMap_Py;
};
/// DBURTs are magnet-states plus comment and timestamp 
struct dburt
{   // proviude a default constructor
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
		/*! Returns the GETSETI values for all magnets in the factory 
		@param[out] std::map of <name, value>, */
		std::map<std::string, double> getAllSETI() const;
		/*! Returns the GETSETI values for all magnets in the factory
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
		/*! Returns the READI values for all magnets in the factory
		@param[out] std::map of <name, value>, */
		std::map<std::string, double> getAllREADI() const;
		/*! Returns the READI values for all magnets in the factory
		@param[out] python::dict of <name, value>, */
		boost::python::dict getAllREADI_Py() const;
		
		// getPSUState

		/*! Returns the PSU STATE for a magnet, i.e ON, OFF, ERROR, ... 
		@param[in] name, full-name or name-alias of magnet object
		@param[out] value, if name can't be found std::min is returned */
		STATE getPSUState(const std::string& name) const;
		/*! Returns the SU STATE  value for std::vector of a magnet names
		@param[in] names, full-names or name-aliases of magnets
		@param[out] std::map of <name, value>, if name can't be found std::min is returned */
		std::map<std::string, STATE> getPSUStates(const std::vector<std::string>& namess) const;
		/*! Returns the SU STATE  value for python::list of a magnet names
		@param[in] names, full-names or name-aliases of magnets
		@param[out] python::dict of <name, value>, if name can't be found std::min is returned */
		boost::python::dict getPSUState_Py(const boost::python::list& names) const;
		/*! Returns the SU STATE  values for all magnets in the factory
		@param[out] std::map of <name, value>, */
		std::map<std::string, STATE> getAllPSUState() const;
		/*! Returns the SU STATE  values for all magnets in the factory
		@param[out] python::dict of <name, value>, */
		boost::python::dict getAllPSUState_Py() const;


		/*! Returns the interlock STATE for a magnet, i.e GOOD, BAD, ...
		@param[in] name, full-name or name-alias of magnet object
		@param[out] value, if name can't be found std::min is returned */
		STATE getILKState(const std::string& name) const;
		/*! Returns the SU STATE  value for std::vector of a magnet names
		@param[in] names, full-names or name-aliases of magnets
		@param[out] std::map of <name, value>, if name can't be found std::min is returned */
		std::map<std::string, STATE> getILKStates(const std::vector<std::string>& names) const;
		/*! Returns the SU STATE  value for python::list of a magnet names
		@param[in] names, full-names or name-aliases of magnets
		@param[out] python::dict of <name, value>, if name can't be found std::min is returned */
		boost::python::dict getILKState_Py(const boost::python::list& names) const;
		/*! Returns the SU STATE  values for all magnets in the factory
		@param[out] std::map of <name, value>, */
		std::map<std::string, STATE>  getAllILKState() const;
		/*! Returns the SU STATE  values for all magnets in the factory
		@param[out] python::dict of <name, value>, */
		boost::python::dict getAllILKState_Py() const;

		/*! Set the current for a magnet object 
		@param[in] name, full-name or name-alias of magnet object
		@param[in] value, new current value 
		@param[out] state, either STATE::SUCCESS, STATE::FAIL if the comnmad was succesfully sent to epics \
		(not if the magnet actuallyu switched on, or STATE::UNKNOWN_NAME if the passed name cannot be found*/
		STATE SETI(const std::string& name, const double &value);
		/*! Set the current for a map of multiple magnets and values 
		@param[in] namesAndCurrentsMap, map of new magnet currents keyed by the magnet object full-name or name-alias 
		@param[out] std::map of return states for each magnet, keyed by the passed names */
		std::map<std::string, STATE> SETI(const std::map<std::string, double> &namesAndCurrentsMap);
		/*! Set the current for a python::dict of multiple magnets and values
		@param[in] namesAndCurrentsMap, map of new magnet currents keyed by the magnet object full-name or name-alias
		@param[out] python::dict of return states for each magnet, keyed by the passed names */
		boost::python::dict SETI_Py(boost::python::dict& namesAndCurrentsMap);
		/*! Set the current for all magnets in the magnetFactory to zero  (c++ version)
		@param[out] python::dict of return states for each magnet, keyed by their full-name*/
		std::map<std::string, STATE> SETIAllZero();
		/*! Set the current for all magnets in the magnetFactory to zero (python version)
		@param[out] python::dict of return states from each magnet, keyed by their full-name*/
		boost::python::dict SETIAllZero_Py();

		/*! Attempt to swtch ON the PSU for a magnet 
		@param[in] name, full-name or name-alias of magnet object
		@param[out] state, either STATE::SUCCESS, STATE::FAIL if the comnmad was succesfully sent to epics \
		(not if the magnet actuallyu switched on, or STATE::UNKNOWN_NAME if the passed name cannot be found*/
		STATE switchOn(const std::string& name);
		std::map<std::string, STATE> switchOn(const std::vector<std::string>& names);
		boost::python::dict switchOn_Py(const boost::python::list names);
		std::map<std::string, STATE> switchOnAll();
		boost::python::dict switchOnAll_Py();		
		
		STATE switchOff(const std::string& name);
		std::map<std::string, STATE> switchOff(const std::vector<std::string>& names);
		boost::python::dict switchOff_Py(const boost::python::list& names);
		std::map<std::string, STATE> switchOffAll();
		boost::python::dict switchOffAll_Py();


					   
		bool degauss(const std::string& name, const bool reset_to_zero);
		std::map<std::string, bool> degauss(const std::vector<std::string>& names, const bool reset_to_zero);
		std::map<std::string, bool> degaussALL(const bool reset_to_zero);
		boost::python::dict degauss_Py(const boost::python::list& names, const bool reset_to_zero);
		boost::python::dict degaussAll_Py(const bool reset_to_zero);
		
		
		std::vector<std::string> getAliases(const std::string& name) const;
		boost::python::list getAliases_Py1(const std::string& name) const;
		std::map<std::string, std::vector<std::string>> getAliases(const std::vector<std::string>& names) const;
		boost::python::dict getAliases_Py2(const boost::python::list& name) const;
		
		std::string getManufacturer(const std::string& name) const;
		std::map<std::string, std::string> getManufacturer(const std::vector<std::string>& name) const;
		boost::python::dict getManufacturer_Py(const boost::python::list& name) const;

		// TODO Should be string
		int getSerialNumber(const std::string& name) const;
		std::map<std::string, int> getSerialNumber(const std::vector<std::string>& name) const;
		boost::python::dict getSerialNumber_Py(const boost::python::list& name) const;
		
		TYPE getMagnetType(const std::string& name) const;
		std::map<std::string, TYPE> getMagnetType(const std::vector<std::string>& name) const;
		boost::python::dict getMagnetType_Py(const boost::python::list& name) const;


		std::string getMagnetRevType(const std::string& name) const;
		std::map<std::string, std::string> getMagnetRevType(const std::vector<std::string>& name) const;
		boost::python::dict getMagnetRevType_Py(const boost::python::list& name) const;

		double getMagneticLength(const std::string& name) const;
		std::map<std::string, double> getMagneticLength(const std::vector<std::string>& name) const;
		boost::python::dict getMagneticLength_Py(const boost::python::list& name) const;
		
		std::string getFullPSUName(const std::string& name) const;
		std::map<std::string, std::string>getFullPSUName(const std::vector<std::string>& name) const;
		boost::python::dict getFullPSUName_Py(const boost::python::list& name) const;
		
		std::string getMeasurementDataLocation(const std::string& name) const;
		std::map<std::string, std::string> getMeasurementDataLocation(const std::vector<std::string>& name) const;
		boost::python::dict getMeasurementDataLocation_Py(const boost::python::list& name) const;
		
		size_t getNumberOfDegaussSteps(const std::string& name) const;
		std::map<std::string, size_t> getNumberOfDegaussSteps(const std::vector<std::string>& name) const;
		boost::python::dict  getNumberOfDegaussSteps_Py(const boost::python::list& name) const;

		std::vector<double> getDegaussValues(const std::string& name) const;
		boost::python::list getDegaussValuesSingle_Py(const std::string& name) const;
		std::map<std::string, std::vector<double>> getDegaussValues(const std::vector<std::string>& names) const;
		boost::python::dict getDegaussValuesMulti_Py(const boost::python::list& name) const;

		double getDegaussTolerance(const std::string& name) const;
		std::map<std::string, double> getDegaussTolerance(const std::vector < std::string>& names) const;
		boost::python::dict getDegaussTolerance_Py(const boost::python::list& name) const;

		double getREADITolerance(const std::string& name) const;
		std::map<std::string, double> getREADITolerance(const std::vector < std::string>& names) const;
		boost::python::dict getREADITolerance_Py(const boost::python::list& name) const;


		// get min I 
		double getMinI(const std::string& name)const;
		std::map<std::string, double> getMinIs(const std::vector<std::string>& names) const;
		boost::python::dict getMinIs_Py(const boost::python::list& names) const;
		std::map<std::string, double> getAllMinI() const;
		boost::python::dict getAllMinI_Py() const;
		// get max I 
		double getMaxI(const std::string& name)const;
		std::map<std::string, double> getMaxIs(const std::vector<std::string>& names) const;
		boost::python::dict getMaxIs_Py(const boost::python::list& names) const;
		std::map<std::string, double> getAllMaxI() const;
		boost::python::dict getAllMaxI_Py() const;



		//int setNumberOfDegaussSteps(const int value);
		// THINK ABOUT THIS ONE!!! 
		std::vector<double> setDegaussValues(const std::string& name, const std::vector<double>& values);
		boost::python::list setDegaussValuesSingle_Py(const std::string& name, const boost::python::list& values);
		std::map<std::string, std::vector<double>> setDegaussValues(const std::vector < std::string>& names, const std::vector<double>& values);
		boost::python::dict setDegaussValuesMulti_Py(const boost::python::list& names,const boost::python::list& values);
		
		
		double setDegaussTolerance(const std::string& name, const double value);
		std::map<std::string, double> setDegaussTolerance(const std::vector < std::string>& names, const double value);
		boost::python::dict setDegaussTolerance_Py(const boost::python::list& name, const double value);


		double setREADITolerance(const std::string& name, const double value);
		std::map<std::string, double> setREADITolerance(const std::vector < std::string>& names, const double value);
		boost::python::dict setREADITolerance_Py(const boost::python::list& names, const double value);



		bool offlineSetILKState(const std::string& name, const STATE value);

			   		 
		bool isAType(const std::string& name, const TYPE type)const;
		bool isAQuad(const std::string& name)const;
		bool isADip(const std::string& name)const;
		bool isASol(const std::string& name)const;
		bool isABSol(const std::string& name)const;
		bool isAVCor(const std::string& name)const;
		bool isAHCor(const std::string& name)const;
		bool isACor(const std::string& name)const;



		// magnet states and DBURTS 
		
		//magnetState getMagnetState()const;
		//bool setMagnetState(const magnetState& ms);
		//bool isInState(const magnetState& ms) const;
		//bool isInSETIandPSUState(const magnetState& ms)const;
		//

		magnetState getMagnetState(const std::string& name)const;
		magnetStates getMagnetStates() const;
		bool setMagnetState(const magnetState& ms) ;
		std::map<std::string, bool> applyMagnetStates(const magnetStates& ms, const std::vector<TYPE>& types);
							   
		/// Write a DBURT
		bool writeDBURT(const std::string& fileName)const;
		bool writeDBURT(const std::string& fileName, const std::string& commment)const;
		bool writeDBURT(const std::string& filePath, const std::string& fileName, const std::string& commment)const;

		// read a dburt from file
		dburt readDBURT(const std::string& fileName)const;
		dburt readDBURT(const std::string& filePath, const std::string& fileName)const;



		bool applyDBURT(const std::string& filePath, const std::string& fileName);
		bool applyDBURTQuadOnly(const std::string& filePath,  const std::string& fileName);
		bool applyDBURTCorOnly(const std::string& filePath, const std::string& fileName);

		bool applyDBURT(const std::string& fileName);
		bool applyDBURTQuadOnly(const std::string& fileName);
		bool applyDBURTCorOnly(const std::string& fileName);

		bool isMagnetStateEqualDBURT(const std::string& fileName);


		//bool readAndApplyDburt();
		//bool applyDBURT(const dburt& dburt_to_apply);



		std::string getFullName(const std::string& name_to_check) const;
		// private
		void populateMagnetMap();
		void setMonitorStatus(pvStruct& pvStruct);
		// private
		bool hasBeenSetup;
		// offlien physical or virtual 
		STATE mode;


		void debugMessagesOn();
		void debugMessagesOff();
		void messagesOn();
		void messagesOff();
		bool isDebugOn();
		bool isMessagingOn();

		LoggingSystem messenger;
private:
	// private surely! 
		std::map<std::string, Magnet> magnetMap;



		void setupChannels();

		bool writeDBURTToFile(const boost::filesystem::path& full_path, const dburt& dburt_to_write) const;
		dburt readDBURTFile(const boost::filesystem::path& full_path) const;
		std::pair<bool, std::string> isDBURTFileAlias(const std::string& full_path)const;


		void updateAliasNameMap(const Magnet& magnet);
		std::map<std::string, std::string> alias_name_map;

		// dummy_magnet is used to return values when users ask for a nOn-existing magnet's properties 
		Magnet dummy_magnet;

		/* which areas need to be included */
		std::vector<TYPE> machineAreas;
		void cutMagnetMapByMachineAreas();

		// private
		ConfigReader reader;

};


#endif // MAGNET_FACTORY_H_