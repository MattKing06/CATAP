#ifndef BPM_FACTORY_H_
#define BPM_FACTORY_H_

#include "LoggingSystem.h"
#include "ConfigReader.h"
#pragma once
#include "BPM.h"
#include <vector>
#include <map>
#include <utility>
#include <boost/python.hpp>

typedef void(*updateFunctionPtr)(struct event_handler_args args);
class BPM;
class BPMFactory
{
	public:
		BPMFactory();
		BPMFactory(bool isVirtual);
		BPMFactory(const BPMFactory& copyBPMFactory);
		~BPMFactory();
		/*NEED CONSTRUCTOR THAT TAKES VERSION??*/
		//BPMFactory(std::string version);
		bool setup(const std::string &version);
		LoggingSystem messenger;
		ConfigReader reader;
		BPM& getBPM(const std::string& fullBPMName);
		std::map<std::string, BPM> getBPMs(std::vector<std::string> bpmNames);
		std::map<std::string, BPM> getAllBPMs();
		std::map<std::string, BPM> bpmMap;
		void populateBPMMap();
		void retrieveMonitorStatus(pvStruct& pvStruct);
		bool hasBeenSetup;
		bool isVirtual;
		// methods for setting properties of bpm via PV name
		double getCurrent(const std::string& name);
		std::map<std::string, double> getX(const std::vector<std::string>& names);
		std::map<std::string, double> getXFromPV(const std::vector<std::string>& names);
		std::map<std::string, double> getY(const std::vector<std::string>& names);
		std::map<std::string, double> getYFromPV(const std::vector<std::string>& names);
		std::map<std::string, double> getQ(const std::vector<std::string>& names);
		std::map<std::string, std::pair<double, double>> getXYPosition(const std::vector<std::string>& names);
		std::map<std::string, double> getAllX();
		std::map<std::string, double> getAllXFromPV();
		std::map<std::string, double> getAllY();
		std::map<std::string, double> getAllYFromPV();
		std::map<std::string, double> getAllQ();
		std::map<std::string, std::pair<double, double>> getAllXYPosition();
		double getX(const std::string& name);
		double getXFromPV(const std::string& name);
		double getY(const std::string& name);
		double getYFromPV(const std::string& name);
		double getQ(const std::string& name);
		double getResolution(const std::string& name);
		double getPosition(const std::string& name);
		std::pair<double, double> getXYPosition(const std::string& name);
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
		boost::python::dict getX_Py(boost::python::list names);
		boost::python::dict getXFromPV_Py(boost::python::list names);
		boost::python::dict getY_Py(boost::python::list names);
		boost::python::dict getYFromPV_Py(boost::python::list names);
		boost::python::dict getQ_Py(boost::python::list names);
		boost::python::dict getXYPosition_Py(boost::python::list names);
		boost::python::dict getAllX_Py();
		boost::python::dict getAllXFromPV_Py();
		boost::python::dict getAllY_Py();
		boost::python::dict getAllYFromPV_Py();
		boost::python::dict getAllQ_Py();
		boost::python::dict getAllXYPositions_Py();
};


#endif // BPM_FACTORY_H_