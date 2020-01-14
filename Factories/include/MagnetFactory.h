#ifndef MAGNET_FACTORY_H_
#define MAGNET_FACTORY_H_

#include "LoggingSystem.h"
#include "ConfigReader.h"
#pragma once
#include "Magnet.h"
#include <vector>
#include <map>
#include <boost/python/dict.hpp>
#include <boost/python/list.hpp>

typedef void(*updateFunctionPtr)(struct event_handler_args args);
class Magnet;
class MagnetFactory
{
	public:
		MagnetFactory();
		MagnetFactory(STATE mode);
		MagnetFactory(const MagnetFactory& copyMagnetFactory);
		~MagnetFactory();
		/*NEED CONSTRUCTOR THAT TAKES VERSION??*/
		
		//MagnetFactory(std::string version);
		bool setup(const std::string &version);
		
		// private
		ConfigReader reader;

		// Get Magnets Objects referecnes
		// In the current design YOU CANNOT get a container of magnet object s (or ANY hardware object) 
		Magnet& getMagnet(const std::string& fullMagnetName);
		// !!! FUNCTIONS LIKE THESE CAN'T EXIST !!!!
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
		
		
		STATE turnOn(const std::string& name);
		std::map<std::string, STATE> turnOn(const std::vector<std::string>& names);
		boost::python::dict turnOn_Py(const boost::python::list names);
		bool turnOnAllMagnets();
		
		STATE turnOff(const std::string& name);
		std::map<std::string, STATE> turnOff(const std::vector<std::string>& names);
		boost::python::dict turnOff_Py(const boost::python::list names);
		bool turnOffAllMagnets();
		
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
		void retrieveMonitorStatus(pvStruct& pvStruct);

		// private
		bool hasBeenSetup;
		
		// offlien physical or virtual 
		STATE mode;

private:
		LoggingSystem messenger;

		void updateAliasNameMap(const Magnet& magnet);
		std::map<std::string, std::string> alias_name_map;

		// dummy_magnet is used to return values when users ask for a non-existing magnet's properties 
		Magnet dummy_magnet;

};


#endif // MAGNET_FACTORY_H_