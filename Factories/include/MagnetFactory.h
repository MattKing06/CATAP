#ifndef MAGNET_FACTORY_H_
#define MAGNET_FACTORY_H_

#include "LoggingSystem.h"
#include "ConfigReader.h"
#pragma once
#include "Magnet.h"
#include <vector>
#include <map>
#include <boost/python.hpp>

typedef void(*updateFunctionPtr)(struct event_handler_args args);
class Magnet;
class MagnetFactory
{
	public:
		MagnetFactory();
		MagnetFactory(bool isVirtual);
		MagnetFactory(const MagnetFactory& copyMagnetFactory);
		~MagnetFactory();
		/*NEED CONSTRUCTOR THAT TAKES VERSION??*/
		//MagnetFactory(std::string version);
		bool setup(const std::string &version);
		updateFunctionPtr findUpdateFunctionForRecord(const std::string& record, const Magnet& mag) const;
		LoggingSystem messenger;
		ConfigReader reader;
		Magnet getMagnet(const std::string& fullMagnetName);
		std::map<std::string, Magnet> getMagnets(std::vector<std::string> magnetNames);
		std::map<std::string, Magnet> getAllMagnets();
		std::map<std::string, Magnet> magnetMap;
		void populateMagnetMap();
		bool hasBeenSetup;
		bool isVirtual;
		// methods for setting properties of magnet via PV name
		double getCurrent(const std::string& name);
		std::map<std::string, double> getCurrents(const std::vector<std::string>& names);
		std::map<std::string, double> getAllMagnetCurrents();
		bool setCurrent(const std::string& name, const double &value);
		bool setCurrents(const std::map<std::string, double> &namesAndCurrentsMap);
		bool setAllMagnetCurrents(const double& value);
		bool turnOn(const std::string& name);
		bool turnOn(const std::vector<std::string>& names);
		bool turnOnAllMagnets();
		bool turnOff(const std::string& name);
		bool turnOff(const std::vector<std::string>& names);
		bool turnOffAllMagnets();

		//THESE METHODS SHOULD BE MOVED TO A UTILITY PACKAGE
		template< typename typeOfNewVector>
		std::vector< typeOfNewVector > to_std_vector(const boost::python::object& iterable);
		template< typename typeOfVectorToConvert >
		boost::python::list to_py_list(std::vector<typeOfVectorToConvert> vector);
		template< class key, class value>
		boost::python::dict to_py_dict(std::map<key, value> map);
		// python methods
		boost::python::dict getCurrents_Py(boost::python::list magNames);
		bool setCurrents_Py(boost::python::dict magNamesAndCurrentValues);
};


#endif // MAGNET_FACTORY_H_