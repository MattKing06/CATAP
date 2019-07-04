#ifndef MAGNET_FACTORY_H_
#define MAGNET_FACTORY_H_

#include "LoggingSystem.h"
#include "ConfigReader.h"
#pragma once
#include "Magnet.h"
#include <vector>
#include <map>
class MagnetFactory
{
	public:
		MagnetFactory();
		/*NEED CONSTRUCTOR THAT TAKES VERSION??*/
		//MagnetFactory(std::string version);
		bool setup(std::string version);
		LoggingSystem messenger;
		ConfigReader reader;
		Magnet* getMagnet(std::string fullMagnetName);
		std::vector<Magnet*> getMagnets(std::vector<std::string> magnetNames);
		std::vector<Magnet*> getAllMagnets();
		std::vector<Magnet*> magnetVec;
		bool hasBeenSetup;
		// methods for setting properties of magnet via PV name
		double getCurrent(std::string name);
		std::vector<double> getCurrents(std::vector<std::string> names);
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

};


#endif // MAGNET_FACTORY_H_