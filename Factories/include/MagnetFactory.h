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
		
		// private
		ConfigReader reader;

		// Get Magnets Objects referecnes
		// In the current design YOU CANNOT get a cOntainer of magnet object s (or ANY hardware object) 
		Magnet& getMagnet(const std::string& fullMagnetName);
		// !!! FUNCTIOnS LIKE THESE CAN'T EXIST !!!!
		//std::map<std::string, Magnet&> getMagnets(const std::vector<std::string>& magnetNames);
		//boost::python::dict getAllMagnets_Py();

		std::vector<std::string> getAllMagnetNames()const;
		boost::python::list getAllMagnetNames_Py()const;


		// methods for setting properties of magnet via PV name (or alias)

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
		boost::python::dict switchOff_Py(const boost::python::list names);
		//bool switchOffAllMagnets();
		

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

};


#endif // MAGNET_FACTORY_H_