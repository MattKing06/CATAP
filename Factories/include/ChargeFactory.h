#ifndef CHARGE_FACTORY_H_
#define CHARGE_FACTORY_H_

#include "LoggingSystem.h"
#include "ConfigReader.h"
#pragma once
#include "Charge.h"
#include <vector>
#include <map>
#include <utility>
#include <boost/python.hpp>
#include <boost/circular_buffer.hpp>

typedef void(*updateFunctionPtr)(struct event_handler_args args);
class Charge;
class ChargeFactory
{
	public:
		ChargeFactory();
		ChargeFactory(STATE mode);
		ChargeFactory(const ChargeFactory& copyChargeFactory);
		~ChargeFactory();
		/*NEED constRUCTOR THAT TAKES VERSION??*/
		//BPMFactory(std::string VERSION);
		bool setup(const std::string &VERSION);
		LoggingSystem messenger;
		void debugMessagesOn();
		void debugMessagesOff();
		void messagesOn();
		void messagesOff();
		bool isDebugOn();
		bool isMessagingOn();
		ConfigReader reader;
		Charge& getChargeDiagnostic(const std::string& fullChargeName);
		std::map<std::string, Charge> getChargeDiagnostics(std::vector<std::string> chargeNames);
		std::map<std::string, Charge> getAllChargeDiagnostics();
		std::map<std::string, Charge> chargeMap;
		std::string getChargeDiagnosticName(const std::string& name);
		void populateChargeMap();
		void retrievemonitorStatus(pvStruct& pvStruct);
		void setupChannels();
		void setBufferSize(const size_t& value);
		void monitorForNShots(const std::string& name, const size_t& value);
		void monitorMultipleForNShots(const std::vector< std::string >& name, const size_t& value);
		bool hasBeenSetup;
		
		STATE mode;
		
		bool ismonitoringQ(const std::string& name);
		bool ismonitoring(const std::string& name);
		std::map<std::string, double> getQs(const std::vector<std::string>& names);
		std::map<std::string, double> getPositions(const std::vector<std::string>& names);
		std::map<std::string, double> getAllQ();
		std::map<std::string, double> getAllPosition();
		std::map<std::string, std::vector< double > > getAllQVector();
		std::map < std::string, std::vector< double > > getQVectors(const std::vector<std::string> & names);
		std::map < std::string, boost::circular_buffer< double > > getQBuffers(const std::vector<std::string> & names);
		std::map < std::string, boost::circular_buffer< double > > getAllQBuffer();
		double getQ(const std::string& name);
		double getPosition(const std::string& name);
		std::vector< double > getQVector(const std::string& name);
		boost::circular_buffer< double > getQBuffer(const std::string& name);
		void monitorForNShots_Py(boost::python::list name, const size_t& value);
		boost::python::dict getQs_Py(boost::python::list names);
		boost::python::list getQVector_Py(const std::string& name);
		boost::python::list getQBuffer_Py(const std::string& name);
		boost::python::dict getQVectors_Py(boost::python::list names);
		boost::python::dict getPositions_Py(boost::python::list names);
		boost::python::dict getQBuffers_Py(boost::python::list names);
		boost::python::dict getAllQBuffer_Py();
		boost::python::dict getAllQ_Py();
		boost::python::dict getAllQVector_Py();
		boost::python::dict getAllPosition_Py();
};


#endif // CHARGE_FACTORY_H_