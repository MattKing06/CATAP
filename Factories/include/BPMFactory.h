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
#include <boost/circular_buffer.hpp>

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
		std::string getBPMName(const std::string& name);
		void populateBPMMap();
		void retrieveMonitorStatus(pvStruct& pvStruct);
		void monitorForNShots(const std::string& name, const size_t& value);
		bool hasBeenSetup;
		bool isVirtual;
		bool isMonitoringXPV(const std::string& name);
		bool isMonitoringYPV(const std::string& name);
		bool isMonitoringData(const std::string& name);
		bool isMonitoring(const std::string& name);
		// methods for setting properties of bpm via PV name
		std::map<std::string, double> getXs(const std::vector<std::string>& names);
		std::map<std::string, double> getXsFromPV(const std::vector<std::string>& names);
		std::map<std::string, double> getYs(const std::vector<std::string>& names);
		std::map<std::string, double> getYsFromPV(const std::vector<std::string>& names);
		std::map<std::string, double> getQs(const std::vector<std::string>& names);
		std::map<std::string, std::vector< double > > getDatas(const std::vector<std::string>& names);
		std::map<std::string, std::pair<double, double>> getXYPositions(const std::vector<std::string>& names);
		std::map<std::string, std::vector< double > > getXPVVectors(const std::vector<std::string>& names);
		std::map<std::string, std::vector< double > > getYPVVectors(const std::vector<std::string>& names);
		std::map<std::string, std::vector< double > > getQVectors(const std::vector<std::string>& names);
		std::map<std::string, std::vector< std::vector< double > > > getDataVectors(const std::vector<std::string>& names);
		std::map<std::string, std::pair<std::vector< double >, std::vector< double > > > getXYPositionVectors(const std::vector<std::string>& names);
		std::map<std::string, double> getResolutions(const std::vector<std::string>& names);
		std::map<std::string, double> getPositions(const std::vector<std::string>& names);
		std::map<std::string, bool> reCalAttenuations(const std::vector<std::string>& names, const double& charge);
		std::map<std::string, double> getAllX();
		std::map<std::string, double> getAllXFromPV();
		std::map<std::string, double> getAllY();
		std::map<std::string, double> getAllYFromPV();
		std::map<std::string, double> getAllQ();
		std::map<std::string, std::vector< double > > getAllData();
		std::map<std::string, double> getAllResolution();
		std::map<std::string, double> getAllPosition();
		std::map<std::string, std::pair<double, double>> getAllXYPosition();
		std::map<std::string, std::vector< double > > getAllXPVVector();
		std::map<std::string, std::vector< double > > getAllYPVVector();
		std::map<std::string, std::vector< double > > getAllQVector();
		std::map<std::string, std::vector< std::vector< double > > > getAllDataVector();
		std::map<std::string, std::pair<std::vector< double >, std::vector< double > > > getAllXYPositionVectors();
		std::map<std::string, bool> reCalAllAttenuation(const double& charge);
		double getX(const std::string& name);
		double getXFromPV(const std::string& name);
		double getY(const std::string& name);
		double getYFromPV(const std::string& name);
		double getQ(const std::string& name);
		std::vector< double > getData(const std::string& name);
		double getResolution(const std::string& name);
		double getPosition(const std::string& name);
		bool reCalAttenuation(const std::string& name, const double& charge);
		std::pair<double, double> getXYPosition(const std::string& name);
		std::vector< double > getXPVVector(const std::string& name);
		std::vector< double > getYPVVector(const std::string& name);
		std::vector< double > getQVector(const std::string& name);
		std::vector< std::vector< double > > getDataVector(const std::string& name);
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
		boost::python::dict getDatas_Py(boost::python::list names);
		boost::python::dict getXYPositions_Py(boost::python::list names);
		boost::python::list getXPVVector_Py(const std::string& name);
		boost::python::list getYPVVector_Py(const std::string& name);
		boost::python::list getQVector_Py(const std::string& name);
		boost::python::list getDataVector_Py(const std::string& name);
		boost::python::dict getXPVVectors_Py(boost::python::list names);
		boost::python::dict getYPVVectors_Py(boost::python::list names);
		boost::python::dict getQVectors_Py(boost::python::list names);
		boost::python::dict getDataVectors_Py(boost::python::list names);
		boost::python::dict getXYPositionVectors_Py(boost::python::list names);
		boost::python::dict getResolutions_Py(boost::python::list names);
		boost::python::dict getPositions_Py(boost::python::list names);
		boost::python::dict reCalAttenuations_Py(boost::python::list names, const double& charge);
		boost::python::dict getAllX_Py();
		boost::python::dict getAllXFromPV_Py();
		boost::python::dict getAllY_Py();
		boost::python::dict getAllYFromPV_Py();
		boost::python::dict getAllQ_Py();
		boost::python::dict getAllData_Py();
		boost::python::dict getAllXYPosition_Py();
		boost::python::dict getAllXPVVector_Py();
		boost::python::dict getAllYPVVector_Py();
		boost::python::dict getAllQVector_Py();
		boost::python::dict getAllDataVector_Py();
		boost::python::dict getAllXYPositionVectors_Py();
		boost::python::dict getAllResolution_Py();
		boost::python::dict getAllPosition_Py();
		boost::python::dict reCalAllAttenuation_Py(const double& charge);
};


#endif // BPM_FACTORY_H_