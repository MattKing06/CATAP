#ifndef MAGNET_FACTORY_H_
#define MAGNET_FACTORY_H_

#include "LoggingSystem.h"
#include "ConfigReader.h"
#pragma Once
#include "Magnet.h"
#include <vector>
#include <map>
#include <boost/PYTHON/dict.hpp>
#include <boost/PYTHON/list.hpp>

typedef void(*updateFunctiOnPtr)(struct event_handler_args args);
class Magnet;
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
		double getSETI(const std::string& name) const;
		std::map<std::string, double> getSETIs(const std::vector<std::string>& names) const;
		boost::python::dict getSETIs_Py(const boost::python::list& magNames) const;
		std::map<std::string, double> getAllSETI() const;
		boost::python::dict getAllSETI_Py() const;
		
		// getREADI
		double getREADI(const std::string& name) const;
		std::map<std::string, double> getREADIs(const std::vector<std::string>& names) const;
		boost::python::dict getREADIs_Py(const boost::python::list& magNames) const;
		std::map<std::string, double> getAllREADI() const;
		boost::python::dict getAllREADI_Py() const;
		
		//bool setAllMagnetCurrents(const double& value);
		//boost::python::dict getAllMagnetCurrents_Py();


		double getRICurrent(const std::string& name);
		std::map<std::string, double> getRICurrents(const std::vector<std::string>& names);
		std::map<std::string, double> getAllMagnetRICurrents();
		
		void SETI(const std::string& name, const double &value);
		
		void SETI(const std::map<std::string, double> &namesAndCurrentsMap);
		//void SETI_Py(const std::map<std::string, double> &namesAndCurrentsMap);
		
		
		STATE switchOn(const std::string& name);
		std::map<std::string, STATE> switchOn(const std::vector<std::string>& names);
		boost::python::dict switchOn_Py(const boost::python::list names);
		//bool switchOnAllMagnets();
		
		STATE switchOFF(const std::string& name);
		std::map<std::string, STATE> switchOFF(const std::vector<std::string>& names);
		boost::python::dict switchOFF_Py(const boost::python::list names);
		//bool switchOFFAllMagnets();
		
		STATE getPSUState(const std::string& name) const;
		std::map<std::string, STATE> getAllPSUState() const;
		boost::python::dict getAllPSUState_Py() const;
		
		int getILKState(const std::string& name) const;


		bool setCurrents_Py(boost::python::dict magNamesAndCurrentValues);
		boost::python::dict getRICurrents_Py(boost::python::list names);

		std::string getFullName(const std::string& name_to_check) const;
		// private surely! 
		std::map<std::string, Magnet> magnetMap;
		// private
		void populateMagnetMap();
		void retrievemonitorStatus(pvStruct& pvStruct);
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


		void updateAliasNameMap(const Magnet& magnet);
		std::map<std::string, std::string> alias_name_map;

		// dummy_magnet is used to return values when users ask for a nOn-existing magnet's properties 
		Magnet dummy_magnet;

};


#endif // MAGNET_FACTORY_H_