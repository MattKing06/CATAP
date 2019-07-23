#ifndef MAGNET_FACTORY_H_
#define MAGNET_FACTORY_H_

#include "LoggingSystem.h"
#include "ConfigReader.h"
#pragma once
#include "Magnet.h"
#include <vector>
#include <map>
#include <boost/python.hpp>
class MagnetFactory
{
	public:
		MagnetFactory();
		MagnetFactory(bool isVirtual);
		/*NEED CONSTRUCTOR THAT TAKES VERSION??*/
		//MagnetFactory(std::string version);
		typedef void (*updateFunctionPtr)(struct event_handler_args args);
		bool setup(std::string version);
		updateFunctionPtr findUpdateFunctionForRecord(std::string record, Magnet* mag);
		LoggingSystem messenger;
		ConfigReader reader;
		Magnet* getMagnet(std::string fullMagnetName);
		std::map<std::string, Magnet*> getMagnets(std::vector<std::string> magnetNames);
		std::map<std::string, Magnet*> getAllMagnets();
		std::map<std::string, Magnet*> magnetMap;
		bool hasBeenSetup;
		bool virtualMagnetFactory;
		// methods for setting properties of magnet via PV name
		double getCurrent(std::string name);
		std::map<std::string, double> getCurrents(std::vector<std::string> names);
		std::map<std::string, double> getAllMagnetCurrents();
		bool setCurrent(std::string name, double value);
		bool setCurrents(std::vector<std::string> names, double value);
		bool setAllMagnetCurrents(double value);
		bool turnOn(std::string name);
		bool turnOn(std::vector<std::string> names);
		bool turnOnAllMagnets();
		bool turnOff(std::string name);
		bool turnOff(std::vector<std::string> names);
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