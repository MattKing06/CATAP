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
		MagnetFactory();
		MagnetFactory(STATE mode);
		MagnetFactory(STATE mode, TYPE machineArea);
		MagnetFactory(STATE mode, const std::vector<TYPE>& machineAreas);
		MagnetFactory(STATE mode, const boost::python::list& machineAreas);
		MagnetFactory(const MagnetFactory& copyMagnetFactory);
		~MagnetFactory();
		/*NEED constRUCTOR THAT TAKES VERSION??*/
		//MagnetFactory(std::string VERSION);
		
		// OVERLOADED SETUP FUNCTIONS TO ALLWO USER FULL CONTROL AND "FUTURE PROOVED VERSION PARAMETER" 
		bool setup();
		bool setup(const std::string& version);
		bool setup(TYPE machineArea);
		bool setup(const std::string& version, TYPE machineArea);
		bool setup(const std::vector<TYPE>& machineAreas);
		bool setup(const boost::python::list& machineAreas);
		bool setup(const std::string& version, const boost::python::list& machineAreas);
		bool setup(const std::string& version, const std::vector<TYPE>& machineAreas);

		void setupChannels();

		// Get Magnets Objects referecnes
		// In the current design YOU CANNOT get a cOntainer of magnet object s (or ANY hardware object) 
		Magnet& getMagnet(const std::string& fullMagnetName);
		// !!! FUNCTIOnS LIKE THESE CAN'T EXIST !!!!
		//std::map<std::string, Magnet&> getMagnets(const std::vector<std::string>& magnetNames);
		//boost::python::dict getAllMagnets_Py();

		std::vector<std::string> getAllMagnetNames()const;
		boost::python::list getAllMagnetNames_Py()const;
		
		// getSETI
		double getSETI(const std::string& name)const;
		std::map<std::string, double> getSETIs(const std::vector<std::string>& names) const;
		boost::python::dict getSETIs_Py(const boost::python::list& names) const;
		std::map<std::string, double> getAllSETI() const;
		boost::python::dict getAllSETI_Py() const;
		
		// getREADI
		double getREADI(const std::string& name) const;
		std::map<std::string, double> getREADIs(const std::vector<std::string>& names) const;
		boost::python::dict getREADI_Py(const boost::python::list& names) const;
		std::map<std::string, double> getAllREADI() const;
		boost::python::dict getAllREADI_Py() const;
		
		// getPSUState
		STATE getPSUState(const std::string& name) const;
		std::map<std::string, STATE> getPSUStates(const std::vector<std::string>& namess) const;
		boost::python::dict getPSUState_Py(const boost::python::list& names) const;
		std::map<std::string, STATE> getAllPSUState() const;
		boost::python::dict getAllPSUState_Py() const;

		STATE getILKState(const std::string& name) const;
		std::map<std::string, STATE> getILKStates(const std::vector<std::string>& names) const;
		boost::python::dict getILKState_Py(const boost::python::list& names) const;
		std::map<std::string, STATE>  getAllILKState() const;
		boost::python::dict getAllILKState_Py() const;

	
		STATE SETI(const std::string& name, const double &value);
		std::map<std::string, STATE> SETI(const std::map<std::string, double> &namesAndCurrentsMap);
		boost::python::dict SETI_Py(boost::python::dict& namesAndCurrentsMap);
		std::map<std::string, STATE> SETIAllZero();
		boost::python::dict SETIAllZero_Py();

		
		STATE switchOn(const std::string& name);
		std::map<std::string, STATE> switchOn(const std::vector<std::string>& names);
		boost::python::dict switchOn_Py(const boost::python::list names);
		//bool switchOnAllMagnets();
		
		STATE switchOff(const std::string& name);
		std::map<std::string, STATE> switchOff(const std::vector<std::string>& names);
		boost::python::dict switchOff_Py(const boost::python::list& names);
		//bool switchOffAllMagnets();
		
		std::map<std::string, STATE> switchOffAll();
		boost::python::dict switchOffAll_Py();

		std::map<std::string, STATE> switchOnAll();
		boost::python::dict switchOnAll_Py();
					   
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
		
		std::string getMagnetType(const std::string& name) const;
		std::map<std::string, std::string> getMagnetType(const std::vector<std::string>& name) const;
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

			   		 

		// magnet states and DBURTS 
		
		//magnetState getMagnetState()const;
		//bool setMagnetState(const magnetState& ms);
		//bool isInState(const magnetState& ms) const;
		//bool isInSETIandPSUState(const magnetState& ms)const;
		//

		magnetState getMagnetState(const std::string& name)const;
		magnetStates getMagnetStates() const;
		bool setMagnetState(const magnetState& ms) ;
		std::map<std::string, bool> applyMagnetStates(const magnetStates& ms);

					   
		/// Write a DBURT
		bool writeDBURT(const std::string& fileName)const;
		bool writeDBURT(const std::string& fileName, const std::string& commment)const;
		bool writeDBURT(const std::string& filePath, const std::string& fileName, const std::string& commment)const;

		// read a dburt from file
		dburt readDBURT(const std::string& fileName)const;
		dburt readDBURT(const std::string& filePath, const std::string& fileName)const;

		bool readAndApplyDBURT(const std::string& fileName);

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


		bool writeDBURTToFile(const boost::filesystem::path& full_path, const dburt& dburt_to_write) const;
		dburt readDBURTFile(const boost::filesystem::path& full_path) const;
		std::pair<bool, std::string> isDBURTFileAlias(const boost::filesystem::path& full_path)const;


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