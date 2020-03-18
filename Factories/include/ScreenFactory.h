#ifndef SCREEN_FACTORY_H_
#define SCREEN_FACTORY_H_

#include "LoggingSystem.h"
#include "ConfigReader.h"
#pragma once
#include "Screen.h"
#include <vector>
#include <map>
#include <utility>
#include <boost/python.hpp>
#include <boost/circular_buffer.hpp>

typedef void(*updateFunctionPtr)(struct event_handler_args args);
class Screen;
class ScreenFactory
{
public:
	ScreenFactory();
	ScreenFactory(STATE mode);
	ScreenFactory(const ScreenFactory& copyScreenFactory);
	~ScreenFactory();
	/*NEED constRUCTOR THAT TAKES VERSION??*/
	//ScreenFactory(std::string VERSION);
	bool setup(const std::string& VERSION);
	LoggingSystem messenger;
	void debugMessagesOn();
	void debugMessagesOff();
	void messagesOn();
	void messagesOff();
	bool isDebugOn();
	bool isMessagingOn();
	ConfigReader reader;
	Screen& getScreen(const std::string& fullScreenName);
	std::vector<std::string> getAllScreenNames();
	std::map<std::string, Screen> screenMap;
	std::string getScreenName(const std::string& name);
	void populateScreenMap();
	void retrievemonitorStatus(pvStruct& pvStruct);
	void setupChannels();
	bool hasBeenSetup;
	//bool isVirtual;
	STATE state;
	STATE mode;
	std::map<std::string, std::string> screenParametersAndValuesMap;
	void setPVStructs();
	void offlineSet(const long& value);
	bool isHOut(const std::string& name);
	bool isVOut(const std::string& name);
	bool isHIn(const std::string& name);
	bool isVIn(const std::string& name);
	bool is_HandV_OUT(const std::string& name);
	bool isScreenIn(const std::string& name);
	bool isHMoving(const std::string& name);
	bool isVMoving(const std::string& name);
	bool isPMoving(const std::string& name);
	bool isMoving(const std::string& name);
	bool isClearForBeam(const std::string& name);
	bool isMover(const std::string& name);
	bool isVMover(const std::string& name);
	bool isHVMover(const std::string& name);
	bool isPneumatic(const std::string& name);
	STATE getScreenState(const std::string& name);
	STATE getScreenSetState(const std::string& name);
	std::pair< STATE, TYPE > getScreenStatePair(const std::string& name, TYPE type);
	std::pair< STATE, TYPE > getScreenSetStatePair(const std::string& name, TYPE type);
	TYPE getScreenType(const std::string& name);
	std::vector< STATE > getAvailableDevices(const std::string& name);
	bool isScreenInState(const std::string& name, STATE sta);
	bool isYAGIn(const std::string& name);
	bool isHElement(const std::string& name, STATE e);
	bool isVElement(const std::string& name, STATE e);
	bool isPElement(const std::string& name, STATE e);
	bool isHEnabled(const std::string& name);
	bool isVEnabled(const std::string& name);
	bool isRFCageIn(const std::string& name);
	double getACTPOS(const std::string& name);
	double getJDiff(const std::string& name);
	double getDevicePosition(const std::string& name, STATE state);
	double getPosition(const std::string& name);
	bool isVELAPneumatic(const std::string& name);
	bool isVELAHVMover(const std::string& name);
	bool isCLARAHVMover(const std::string& name);
	bool isCLARAVMover(const std::string& name);
	double get_H_ACTPOS(const std::string& name);
	double get_V_ACTPOS(const std::string& name);
	std::map<std::string, STATE> getScreenStates(std::vector<std::string> names);
	std::map<std::string, STATE> getScreenSetStates(std::vector<std::string> names);
	std::map<std::string, Screen> getScreens(std::vector<std::string> names);
	std::map<std::string, STATE> getAllScreenSetStates();
	std::map<std::string, STATE> getAllScreenStates();
	std::map<std::string, Screen> getAllScreens();
	/// SETTERS
	void moveScreenTo(const std::string& name, STATE& state);
	void insertYAG(const std::string& name);
	void makeReadEqualSet(const std::string& name);
	void makeSetEqualRead(const std::string& name);
	void moveScreenOut(const std::string& name);
	void resetPosition(const std::string& name);
	void jogScreen(const std::string& name, const double jog);
	void setPosition(const std::string& name, const double setPos, TYPE type);
	bool setScreenSDEV(const std::string& name, STATE& state);
	bool setScreenTrigger(const std::string& name, const int& value);
	bool setScreenTriggerWDir(const std::string& name, const int& value, TYPE& type);
	bool setEX(const std::string& name, const int& value, TYPE type);
	bool setEN(const std::string& name, const int& value, TYPE direction);
	bool setTGTPOS(const std::string& name, const double& value, TYPE direction);
	bool setScreenSetState(const std::string& name, STATE state);
	STATE getState(const std::string& name) const;
	boost::python::list getAllScreenNames_Py();
	boost::python::dict getAllScreens_Py();
	boost::python::dict getAllScreenSetStates_Py();
	boost::python::dict getAllScreenStates_Py();
	boost::python::dict getScreenStates_Py(boost::python::list names);
	boost::python::dict getScreenSetStates_Py(boost::python::list names);
	boost::python::dict getScreens_Py(boost::python::list names);
};


#endif // Screen_FACTORY_H_