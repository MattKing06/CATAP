#ifndef LASER_ENERGY_METER_FACTORY_H_
#define LASER_ENERGY_METER_FACTORY_H_

#include "LoggingSystem.h"
#include "ConfigReader.h"
#pragma once
#include "LaserEnergyMeter.h"
#include <vector>
#include <map>
#include <utility>
#include <boost/python.hpp>
#include <boost/circular_buffer.hpp>

typedef void(*updateFunctionPtr)(struct event_handler_args args);
class LaserEnergyMeter;
class LaserEnergyMeterFactory
{
public:
	LaserEnergyMeterFactory();
	LaserEnergyMeterFactory(STATE mode);
	LaserEnergyMeterFactory(const LaserEnergyMeterFactory& copyLaserEnergyMeterFactory);
	~LaserEnergyMeterFactory();
	/*NEED constRUCTOR THAT TAKES VERSION??*/
	//LaserFactory(std::string VERSION);
	bool setup(const std::string& VERSION);
    LoggingSystem messenger;
	void debugMessagesOn();
	void debugMessagesOff();
	void messagesOn();
	void messagesOff();
	bool isDebugOn();
	bool isMessagingOn();
	ConfigReader reader;
	LaserEnergyMeter& getLaserEnergyMeter(const std::string& fullLaserName);
	std::map<std::string, LaserEnergyMeter> getLaserEnergyMeters(std::vector<std::string> laserNames);
	std::map<std::string, LaserEnergyMeter> getAllLaserEnergyMeters();
	std::map<std::string, LaserEnergyMeter> laserEnergyMeterMap;
	std::string getLaserEnergyMeterName(const std::string& name);
	void populateLaserEnergyMeterMap();
	void retrievemonitorStatus(pvStruct& pvStruct);
	void setupChannels();
	bool hasBeenSetup;
	//bool isVirtual;
	STATE mode;
	bool getAcquiring(const std::string& name) const;
	bool setStart(const std::string& name);
	bool setStop(const std::string& name);
	bool setRange(const std::string& name,int range);
	bool setOverRange(const std::string& name) const;
	bool setNotOverRange(const std::string& name) const;
	int getOverRange(const std::string& name) const;
	int getRange(const std::string& name) const;
};


#endif // LASER_FACTORY_H_