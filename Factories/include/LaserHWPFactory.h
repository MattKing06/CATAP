#ifndef LASER_HWP_FACTORY_H_
#define LASER_HWP_FACTORY_H_

#include "LoggingSystem.h"
#include "ConfigReader.h"
#pragma once
#include "LaserHWP.h"
#include <vector>
#include <map>
#include <utility>
#include <boost/python.hpp>
#include <boost/circular_buffer.hpp>

typedef void(*updateFunctionPtr)(struct event_handler_args args);
class LaserHWP;
class LaserHWPFactory
{
public:
	LaserHWPFactory();
	LaserHWPFactory(STATE mode);
	LaserHWPFactory(const LaserHWPFactory& copyLaserHWPFactory);
	~LaserHWPFactory();
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
	LaserHWP& getLaserHWP(const std::string& fullLaserHWPName);
	std::map<std::string, LaserHWP> getLaserHWPs(std::vector<std::string> laserHWPNames);
	std::map<std::string, LaserHWP> getAllLaserHWPs();
	std::map<std::string, LaserHWP> laserHWPMap;
	std::string getLaserHWPName(const std::string& name);
	void populateLaserHWPMap();
	void retrievemonitorStatus(pvStruct& pvStruct);
	void setupChannels();
	bool hasBeenSetup;
	//bool isVirtual;
	STATE mode;
	bool setHWP(const std::string& name,double value);
	double getHWPSet(const std::string& name) const;
	double getHWPRead(const std::string& name) const;
};


#endif // LASER_FACTORY_H_