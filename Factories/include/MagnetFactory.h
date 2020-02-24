#ifndef MAGNET_FACTORY_H_
#define MAGNET_FACTORY_H_

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
struct magnetStateStruct
{   // proviude a default constructor
	magnetStateStruct():
		numMags(GlobalConstants::one_sizet) 
		{};
	size_t numMags;
	std::vector<std::string> magNames;
	std::vector<STATE> psuStates;
	std::vector<double> setiValues, readiValues;

	boost::python::list magNames_Py;
	boost::python::list psuStates_Py;
	boost::python::list readiValues_Py;
	boost::python::list setiValues_Py;
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
	magnetStateStruct magnetstates;
	//magnetStateStruct magnetstates_Py;
};

class MagnetFactory
{
	public:
		MagnetFactory();
		MagnetFactory(STATE mode);
		MagnetFactory(const MagnetFactory& copyMagnetFactory);
		~MagnetFactory();
		/*NEED constRUCTOR THAT TAKES VERSION??*/
		//MagnetFactory(std::string VERSION);
		bool setup(const std::string &VERSION);
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
		boost::python::dict degauss_Py(const boost::python::list& names, const bool reset_to_zero);
		
		
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


		int getNumberOfDegaussSteps(const std::string& name) const;
		std::map<std::string, int> getNumberOfDegaussSteps(const std::vector<std::string>& name) const;
		boost::python::dict  getNumberOfDegaussSteps_Py(const boost::python::list& name) const;

		std::vector<double> getDegaussValues(const std::string& name) const;
		boost::python::list getDegaussValues_Py1(const std::string& name) const;
		std::map<std::string, std::vector<double>> getDegaussValues(const std::vector<std::string>& names) const;
		boost::python::dict getDegaussValues_Py2(const boost::python::list& name) const;

		double getDegaussTolerance(const std::string& name) const;
		std::map<std::string, double> getDegaussTolerance(const std::vector < std::string>& names) const;
		boost::python::dict getDegaussTolerance_Py(const boost::python::list& name) const;

		double getRITolerance(const std::string& name) const;
		std::map<std::string, double> getRITolerance(const std::vector < std::string>& names) const;
		boost::python::dict getRITolerance_Py(const boost::python::list& name) const;


		//int setNumberOfDegaussSteps(const int value);
		// THINK ABOUT THIS ONE!!! 
		//std::vector<double> setDegaussValues(const std::string& name, const std::vector<double>& values);
		//boost::python::list setDegaussValues_Py1(const std::string& name, const boost::python::list& values);
		//std::map<std::string, std::vector<double>> setDegaussValues(const std::vector < std::string>& names, const std::vector<double>& values);
		//boost::python::dict setDegaussValues_Py2(const boost::python::list& values);
		
		
		double setDegaussTolerance(const std::string& name, const double value);
		double setRITolerance(const std::string& name, const double value);
		//STATE getILKState(const std::string& name ) const;
		bool SETIZero(const std::string& name);
		bool setPSUState(const std::string& name, const STATE value);
		bool offlineSetILKState(const std::string& name, const STATE value);






		// magnet states and DBURTS 
		magnetStateStruct getMagnetState() const;
		std::map<std::string, std::map<std::string, STATE>> applyMagnetState(const magnetStateStruct& ms);

			   
		/// Write a DBURT
		bool writeDBURT(const std::string& fileName)const;
		bool writeDBURT(const std::string& fileName, const std::string& commment)const;
		bool writeDBURT(const std::string& filePath, const std::string& fileName, const std::string& commment)const;

		// read a dburt from file
		dburt readDBURT(const std::string& fileName)const;
		dburt readDBURT(const std::string& filePath, const std::string& fileName)const;

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

private:
		LoggingSystem messenger;

	// private surely! 
		std::map<std::string, Magnet> magnetMap;


		bool writeDBURTToFile(const boost::filesystem::path& full_path, const dburt& dburt_to_write) const;
		dburt readDBURTFile(const boost::filesystem::path& full_path) const;
		std::pair<bool, std::string> isDBURTFileAlias(const boost::filesystem::path& full_path)const;


		void updateAliasNameMap(const Magnet& magnet);
		std::map<std::string, std::string> alias_name_map;

		// dummy_magnet is used to return values when users ask for a nOn-existing magnet's properties 
		Magnet dummy_magnet;


		// private
		ConfigReader reader;

};


#endif // MAGNET_FACTORY_H_