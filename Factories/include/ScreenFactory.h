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
	std::map<std::string, Screen> getScreens(std::vector<std::string> bpmNames);
	std::map<std::string, Screen> getAllScreens();
	std::map<std::string, Screen> screenMap;
	std::string getScreenName(const std::string& name);
	void populateScreenMap();
	void retrievemonitorStatus(pvStruct& pvStruct);
	void setupChannels();
	bool hasBeenSetup;
	//bool isVirtual;
	STATE state;
	STATE mode;
	std::string getScreenType();
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
	std::pair< STATE, TYPE > getScreenState(const std::string& name, TYPE type);
	std::pair< STATE, TYPE > getScreenSetState(const std::string& name, TYPE type);
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
	bool setScreenTrigger(const std::string& name, const int& value, TYPE& type);
	bool setEX(const std::string& name, const int& value, TYPE type);
	bool setEN(const std::string& name, const int& value, TYPE direction);
	bool setTGTPOS(const std::string& name, const double& value, TYPE direction);
	bool setScreenSetState(const std::string& name, STATE state);
	std::map<std::string, STATE> getAllStatus();
	std::map<std::string, std::vector< double > > getAllData();
	std::map<std::string, double> getAllResolution();
	std::map<std::string, double> getAllPosition();
	std::map<std::string, std::pair<double, double>> getAllXYPosition();
	std::map<std::string, std::vector< double > > getAllXPVVector();
	std::map<std::string, std::vector< double > > getAllYPVVector();
	std::map<std::string, std::vector< double > > getAllQVector();
	std::map<std::string, std::vector< STATE > > getAllStatusVector();
	std::map<std::string, std::vector< std::vector< double > > > getAllDataVector();
	std::map<std::string, boost::circular_buffer< double > > getAllXPVBuffer();
	std::map<std::string, boost::circular_buffer< double > > getAllYPVBuffer();
	std::map<std::string, boost::circular_buffer< double > > getAllQBuffer();
	std::map<std::string, boost::circular_buffer< STATE > > getAllStatusBuffer();
	std::map<std::string, boost::circular_buffer< std::vector< double > > > getAllDataBuffer();
	std::map<std::string, std::pair<std::vector< double >, std::vector< double > > > getAllXYPositionVectors();
	std::map<std::string, bool> reCalAllAttenuation(const double& charge);
	double getX(const std::string& name);
	double getXFromPV(const std::string& name);
	double getY(const std::string& name);
	double getYFromPV(const std::string& name);
	double getQ(const std::string& name);
	STATE getStatus(const std::string& name);
	std::vector< double > getData(const std::string& name);
	double getResolution(const std::string& name);
	bool reCalAttenuation(const std::string& name, const double& charge);
	std::pair<double, double> getXYPosition(const std::string& name);
	std::vector< double > getXPVVector(const std::string& name);
	std::vector< double > getYPVVector(const std::string& name);
	std::vector< double > getQVector(const std::string& name);
	std::vector< STATE > getStatusVector(const std::string& name);
	std::vector< std::vector< double > > getDataVector(const std::string& name);
	boost::circular_buffer< double > getXPVBuffer(const std::string& name);
	boost::circular_buffer< double > getYPVBuffer(const std::string& name);
	boost::circular_buffer< double > getQBuffer(const std::string& name);
	boost::circular_buffer< STATE > getStatusBuffer(const std::string& name);
	boost::circular_buffer< std::vector< double > > getDataBuffer(const std::string& name);
	std::pair<std::vector< double >, std::vector< double > > getXYPositionVector(const std::string& name);
	bool setSA1(const std::string& name, const long& value);
	bool setSA2(const std::string& name, const long& value);
	bool setSD1(const std::string& name, const long& value);
	bool setSD2(const std::string& name, const long& value);
	long getSA1(const std::string& name);
	long getSA2(const std::string& name);
	long getSD1(const std::string& name);
	long getSD2(const std::string& name);
	long getRA1(const std::string& name);
	long getRA2(const std::string& name);
	long getRD1(const std::string& name);
	long getRD2(const std::string& name);
	STATE getState(const std::string& name) const;
	boost::python::list getData_Py(const std::string& name);
	boost::python::dict getXs_Py(boost::python::list names);
	boost::python::dict getXsFromPV_Py(boost::python::list names);
	boost::python::dict getYs_Py(boost::python::list names);
	boost::python::dict getYsFromPV_Py(boost::python::list names);
	boost::python::dict getQs_Py(boost::python::list names);
	boost::python::dict getStatuses_Py(boost::python::list names);
	boost::python::dict getDatas_Py(boost::python::list names);
	boost::python::dict getXYPositions_Py(boost::python::list names);
	boost::python::list getXPVVector_Py(const std::string& name);
	boost::python::list getYPVVector_Py(const std::string& name);
	boost::python::list getQVector_Py(const std::string& name);
	boost::python::list getStatusVector_Py(const std::string& name);
	boost::python::list getDataVector_Py(const std::string& name);
	boost::python::list getXPVBuffer_Py(const std::string& name);
	boost::python::list getYPVBuffer_Py(const std::string& name);
	boost::python::list getQBuffer_Py(const std::string& name);
	boost::python::list getStatusBuffer_Py(const std::string& name);
	boost::python::list getDataBuffer_Py(const std::string& name);
	boost::python::dict getXPVVectors_Py(boost::python::list names);
	boost::python::dict getYPVVectors_Py(boost::python::list names);
	boost::python::dict getQVectors_Py(boost::python::list names);
	boost::python::dict getStatusVectors_Py(boost::python::list names);
	boost::python::dict getDataVectors_Py(boost::python::list names);
	boost::python::dict getXPVBuffers_Py(boost::python::list names);
	boost::python::dict getYPVBuffers_Py(boost::python::list names);
	boost::python::dict getQBuffers_Py(boost::python::list names);
	boost::python::dict getStatusBuffers_Py(boost::python::list names);
	boost::python::dict getDataBuffers_Py(boost::python::list names);
	boost::python::dict getXYPositionVectors_Py(boost::python::list names);
	boost::python::dict getResolutions_Py(boost::python::list names);
	boost::python::dict getPositions_Py(boost::python::list names);
	boost::python::dict reCalAttenuations_Py(boost::python::list names, const double& charge);
	boost::python::dict getAllX_Py();
	boost::python::dict getAllXFromPV_Py();
	boost::python::dict getAllY_Py();
	boost::python::dict getAllYFromPV_Py();
	boost::python::dict getAllQ_Py();
	boost::python::dict getAllStatus_Py();
	boost::python::dict getAllData_Py();
	boost::python::dict getAllXYPosition_Py();
	boost::python::dict getAllXPVVector_Py();
	boost::python::dict getAllYPVVector_Py();
	boost::python::dict getAllQVector_Py();
	boost::python::dict getAllStatusVector_Py();
	boost::python::dict getAllDataVector_Py();
	boost::python::dict getAllXPVBuffer_Py();
	boost::python::dict getAllYPVBuffer_Py();
	boost::python::dict getAllQBuffer_Py();
	boost::python::dict getAllStatusBuffer_Py();
	boost::python::dict getAllDataBuffer_Py();
	boost::python::dict getAllXYPositionVectors_Py();
	boost::python::dict getAllResolution_Py();
	boost::python::dict getAllPosition_Py();
	boost::python::dict reCalAllAttenuation_Py(const double& charge);
};


#endif // Screen_FACTORY_H_