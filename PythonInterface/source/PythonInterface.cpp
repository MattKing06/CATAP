/*
		THIS CLASS IS FOR EXPOSURE TO PYTHON
		ALL IN ONE PLACE, SO WE DON'T HAVE TO
		LINK EVERY CLASS WITH BOOST.PYTHON.
		HOPEFULLY, IT SHOULD ALL HAPPEN HERE.
*/

#ifdef PYTHON_INTERFACE_EXPORTS
#define PYTHON_INTERFACE_API __declspec(dllexport)
#else
#define PYTHON_INTERFACE_API __declspec(dllimport)
#endif 

#include <boost/python.hpp>
#include <boost/python/suite/indexing/map_indexing_suite.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/enum.hpp>
#include "LoggingSystem.h"
#include "Hardware.h"
#include "HardwareFactory.h"
#include "Magnet.h"
#include "MagnetFactory.h"
#include "BPM.h"
#include "BPMFactory.h"
#include "Charge.h"
#include "ChargeFactory.h"
#include "ConfigReader.h"
#include <vector>
#include <map>


BOOST_PYTHON_MODULE(CATAP)
{
	//Global State Enum exposure
	boost::python::enum_<STATE>("STATE")
		.value("ON", STATE::ON)
		.value("OFF", STATE::OFF);
	//boost::python::class_<EPICSInterface>("EPICSInterface", boost::python::no_init)
	//	.def("debugMessagesOn", &EPICSInterface::debugMessagesOn)
	//	.def("debugMessagesOff", &EPICSInterface::debugMessagesOff)
	//	.def("messagesOn", &EPICSInterface::messagesOn)
	//	.def("messagesOff", &EPICSInterface::messagesOff);
	//boost::python::class_<EPICSMagnetInterface, boost::python::bases<EPICSInterface>, boost::noncopyable>("EPICSMagnetInterface", boost::python::no_init);
	// Hardware Exposure
	boost::python::class_<Hardware>("Hardware", boost::python::no_init)
		.def_readonly("machineArea", &Hardware::machineArea)
		.def_readonly("hardwareType", &Hardware::hardwareType)
		.def_readonly("specificHardwareParameters", &Hardware::specificHardwareParameters)
		.def("getMachineArea", &Hardware::getMachineArea)
		.def("getHardwareType", &Hardware::getHardwareType)
		.def("getHardwareName", &Hardware::getHardwareName)
		//.def("debugMessagesOn", &Hardware::debugMessagesOn)
		//.def("debugMessagesOff", &Hardware::debugMessagesOff)
		//.def("messagesOn", &Hardware::messagesOn)
		//.def("messagesOff", &Hardware::messagesOff)
		.def("getSpecificHardwareParameters", &Hardware::getSpecificHardwareParameters);
	// Magnet Exposure
	boost::python::class_<Magnet, boost::python::bases<Hardware>, boost::noncopyable>("Magnet", boost::python::no_init)
		.add_property("current", &Magnet::getCurrent, &Magnet::setEPICSCurrent)
		.add_property("psu_state", &Magnet::getPSUState, &Magnet::setEPICSPSUState)
		.add_property("ri_current", &Magnet::RICurrent)
		.add_property("name", &Magnet::getHardwareName)
		.add_property("manufacturer", &Magnet::getManufacturer)
		.add_property("serial_number", &Magnet::getSerialNumber)
		.add_property("magnet_type", &Magnet::getMagnetType)
		.add_property("epicsInterface", &Magnet::epicsInterface)
		.def("getCurrent", &Magnet::getCurrent)
		.def("setCurrent", &Magnet::setEPICSCurrent)
		.def("getRICurrent", &Magnet::getRICurrent);
	// BPM Exposure
	boost::python::class_<BPM, boost::python::bases<Hardware>, boost::noncopyable>("BPM", boost::python::no_init)
		.add_property("name", &BPM::getBPMName)
		.add_property("x", &BPM::getXFromPV)
		.add_property("y", &BPM::getYFromPV)
		.add_property("data", &BPM::getData)
		.add_property("q", &BPM::getQ)
		.add_property("xpvbuffer", &BPM::getXPVBuffer)
		.add_property("ypvbuffer", &BPM::getYPVBuffer)
		.add_property("databuffer", &BPM::getDataBuffer)
		.add_property("qbuffer", &BPM::getQBuffer)
		.add_property("xpvvector", &BPM::getXPVVector)
		.add_property("ypvvector", &BPM::getYPVVector)
		.add_property("datavector", &BPM::getDataVector)
		.add_property("qvector", &BPM::getQVector)
		.add_property("xbuffer", &BPM::getXPVBuffer)
		.add_property("ybuffer", &BPM::getYPVBuffer)
		.add_property("databuffer", &BPM::getDataBuffer)
		.add_property("qbuffer", &BPM::getQBuffer)
		.add_property("position", &BPM::getPosition)
		.add_property("resolution", &BPM::getResolution)
		.add_property("ra1", &BPM::getRA1)
		.add_property("ra2", &BPM::getRA2)
		.add_property("rd1", &BPM::getRD1)
		.add_property("rd2", &BPM::getRD2)
		.add_property("sa1", &BPM::getSA1, &BPM::setSA1)
		.add_property("sa2", &BPM::getSA2, &BPM::setSA2)
		.add_property("sd1", &BPM::getSD1, &BPM::setSD1)
		.add_property("sd2", &BPM::getSD2, &BPM::setSD2)
		.add_property("buffersize", &BPM::getBufferSize, &BPM::setBufferSize)
		.def("getXFromPV", &BPM::getXFromPV)
		.def("getYFromPV", &BPM::getYFromPV)
		.def("getData", &BPM::getData)
		.def("getQ", &BPM::getQ)
		.def("getXPVBuffer", &BPM::getXPVBuffer)
		.def("getYPVBuffer", &BPM::getYPVBuffer)
		.def("getDataBuffer", &BPM::getDataBuffer)
		.def("getQBuffer", &BPM::getQBuffer)
		.def("getXPVVector", &BPM::getXPVVector)
		.def("getYPVVector", &BPM::getYPVVector)
		.def("getDataVector", &BPM::getDataVector)
		.def("getQVector", &BPM::getQVector)
		.def("monitorForNShots", &BPM::monitorForNShots)
		.def("isMonitoring", &BPM::isMonitoring)
		.def("isMonitoringXPV", &BPM::isMonitoringXPV)
		.def("isMonitoringYPV", &BPM::isMonitoringYPV)
		.def("isMonitoringData", &BPM::isMonitoringData)
		.def("reCalAttenuation", &BPM::reCalAttenuation)
		.def("isXPVBufferFull", &BPM::isXPVBufferFull)
		.def("isYPVBufferFull", &BPM::isYPVBufferFull)
		.def("isDataBufferFull", &BPM::isDataBufferFull)
		.def("getRA1", &BPM::getRA1)
		.def("getRA2", &BPM::getRA2)
		.def("getRD1", &BPM::getRD1)
		.def("getRD2", &BPM::getRD2)
		.def("getSA1", &BPM::getSA1)
		.def("getSA2", &BPM::getSA2)
		.def("getSD1", &BPM::getSD1)
		.def("getSD2", &BPM::getSD2)
		.def("setSA1", &BPM::setSA1)
		.def("setSA2", &BPM::setSA2)
		.def("setSD1", &BPM::setSD1)
		.def("setSD2", &BPM::setSD2);
		// Charge Exposure
		boost::python::class_<Charge, boost::python::bases<Hardware>, boost::noncopyable>("Charge", boost::python::no_init)
		.add_property("name", &Charge::getName)
		.add_property("charge_type", &Charge::getChargeDiagnosticType)
		.add_property("q", &Charge::getQ)
		.add_property("qbuffer", &Charge::getQBuffer)
		.add_property("qvector", &Charge::getQVector)
		.add_property("position", &Charge::getPosition)
		.add_property("buffersize", &Charge::getBufferSize, &Charge::setBufferSize)
		.def("getQ", &Charge::getQ)
		.def("getQBuffer", &Charge::getQBuffer)
		.def("getQVector", &Charge::getQVector)
		.def("monitorForNShots", &Charge::monitorForNShots)
		.def("isMonitoring", &Charge::isMonitoring);
		// Parameter Map Exposure
		boost::python::class_<std::map<std::string, double> >("numericalParamMap")
		.def(boost::python::map_indexing_suite<std::map<std::string, double> >());
	boost::python::class_<std::map<std::string, std::vector< double > > >("numericalVectorParamMap")
		.def(boost::python::map_indexing_suite<std::map<std::string, std::vector< double > > >());
	boost::python::class_<std::map<std::string, std::string> >("stringParamMap")
		.def(boost::python::map_indexing_suite<std::map<std::string, std::string> >());

	//Magnet Factory Exposure
	bool(MagnetFactory:: * turnOnSingle)(const std::string&) = &MagnetFactory::turnOn;
	bool(MagnetFactory:: * turnOffSingle)(const std::string&) = &MagnetFactory::turnOff;
	boost::python::class_<MagnetFactory>("MagnetFactory", boost::python::no_init)
		.def(boost::python::init<bool>())
		.def("setup", &MagnetFactory::setup)
		.add_property("magnetMap", &MagnetFactory::magnetMap)
		.def("getMagnet", &MagnetFactory::getMagnet, boost::python::return_value_policy<boost::python::reference_existing_object>())
		.def("getMagnets", &MagnetFactory::getMagnets)
		.def("getAllMagnets", &MagnetFactory::getAllMagnets)
		.def("getCurrent", &MagnetFactory::getCurrent)
		.def("getCurrents", &MagnetFactory::getCurrents_Py)
		.def("getAllMagnetCurrents", &MagnetFactory::getAllMagnetCurrents_Py)
		.def("setCurrent", &MagnetFactory::setCurrent)
		.def("setCurrents", &MagnetFactory::setCurrents_Py)
		.def("getRICurrent", &MagnetFactory::getRICurrent)
		.def("getRICurrents", &MagnetFactory::getRICurrents_Py)
		.def("turnOn", turnOnSingle)
		.def("turnOn", &MagnetFactory::turnOn_Py)
		.def("turnOff", turnOffSingle)
		.def("turnOff", &MagnetFactory::turnOff_Py);
		//.def("debugMessagesOn", &MagnetFactory::debugMessagesOn)
		//.def("debugMessagesOff", &MagnetFactory::debugMessagesOff)
		//.def("messagesOn", &MagnetFactory::messagesOn)
		//.def("messagesOff", &MagnetFactory::messagesOff);
		//BPM Factory Exposure
		boost::python::class_<BPMFactory>("BPMFactory", boost::python::no_init)
		.def(boost::python::init<bool>())
		.def("setup", &BPMFactory::setup)
		.add_property("bpmMap", &BPMFactory::bpmMap)
		.def("getBPM", &BPMFactory::getBPM, boost::python::return_value_policy<boost::python::reference_existing_object>())
		.def("getBPMs", &BPMFactory::getBPMs)
		.def("getAllBPMs", &BPMFactory::getAllBPMs)
		.def("getRA1", &BPMFactory::getRA1)
		.def("getRA2", &BPMFactory::getRA2)
		.def("getRD1", &BPMFactory::getRD1)
		.def("getRD2", &BPMFactory::getRD2)
		.def("getSA1", &BPMFactory::getSA1)
		.def("getSA2", &BPMFactory::getSA2)
		.def("getSD1", &BPMFactory::getSD1)
		.def("getSD2", &BPMFactory::getSD2)
		.def("setSA1", &BPMFactory::setSA1)
		.def("setSA2", &BPMFactory::setSA2)
		.def("setSD1", &BPMFactory::setSD1)
		.def("setSD2", &BPMFactory::setSD2)
		.def("monitorForNShots", &BPMFactory::monitorForNShots)
		.def("isMonitoringXPV", &BPMFactory::isMonitoringXPV)
		.def("isMonitoringYPV", &BPMFactory::isMonitoringYPV)
		.def("isMonitoringData", &BPMFactory::isMonitoringData)
		.def("isMonitoring", &BPMFactory::isMonitoring)
		.def("getPosition", &BPMFactory::getPosition)
		.def("getResolution", &BPMFactory::getResolution)
		.def("getX", &BPMFactory::getX)
		.def("getXFromPV", &BPMFactory::getXFromPV)
		.def("getY", &BPMFactory::getY)
		.def("getYFromPV", &BPMFactory::getYFromPV)
		.def("getQ", &BPMFactory::getQ)
		.def("getData", &BPMFactory::getData_Py)
		.def("getXYPosition", &BPMFactory::getXYPosition)
		.def("getXPVVector", &BPMFactory::getXPVVector_Py)
		.def("getYPVVector", &BPMFactory::getYPVVector_Py)
		.def("getQVector", &BPMFactory::getQVector_Py)
		.def("getDataVector", &BPMFactory::getDataVector_Py)
		.def("getXPVBuffer", &BPMFactory::getXPVBuffer_Py)
		.def("getYPVBuffer", &BPMFactory::getYPVBuffer_Py)
		.def("getQBuffer", &BPMFactory::getQBuffer_Py)
		.def("getDataBuffer", &BPMFactory::getDataBuffer_Py)
		//.def("getXYPositionVector", &BPMFactory::getXYPositionVector_Py)
		.def("getResolutions", &BPMFactory::getResolutions_Py)
		.def("getPositions", &BPMFactory::getPositions_Py)
		.def("getXPVVectors", &BPMFactory::getXPVVectors_Py)
		.def("getYPVVectors", &BPMFactory::getYPVVectors_Py)
		.def("getQVectors", &BPMFactory::getQVectors_Py)
		.def("getDataVectors", &BPMFactory::getDataVectors_Py)
		.def("getXPVBuffers", &BPMFactory::getXPVBuffers_Py)
		.def("getYPVBuffers", &BPMFactory::getYPVBuffers_Py)
		.def("getQBuffers", &BPMFactory::getQBuffers_Py)
		.def("getDataBuffers", &BPMFactory::getDataBuffers_Py)
		.def("getAllX", &BPMFactory::getAllX)
		.def("getAllXFromPV", &BPMFactory::getAllXFromPV)
		.def("getAllY", &BPMFactory::getAllY)
		.def("getAllYFromPV", &BPMFactory::getAllYFromPV)
		.def("getAllQ", &BPMFactory::getAllQ)
		.def("getAllData", &BPMFactory::getAllData)
		.def("getAllXYPosition", &BPMFactory::getAllXYPosition_Py)
		.def("getAllResolution", &BPMFactory::getAllResolution_Py)
		.def("getAllPosition", &BPMFactory::getAllPosition_Py)
		.def("getAllXPVVector", &BPMFactory::getAllXPVVector)
		.def("getAllYPVVector", &BPMFactory::getAllYPVVector)
		.def("getAllQVector", &BPMFactory::getAllQVector)
		.def("getAllDataVector", &BPMFactory::getAllDataVector)
		.def("getAllXPVBuffer", &BPMFactory::getAllXPVBuffer)
		.def("getAllYPVBuffer", &BPMFactory::getAllYPVBuffer)
		.def("getAllQBuffer", &BPMFactory::getAllQBuffer)
		.def("getAllDataBuffer", &BPMFactory::getAllDataBuffer)
		.def("getAllXYPositionVectors", &BPMFactory::getAllXYPositionVectors_Py)
		.add_property("logger", &BPMFactory::messenger);
	//Charge Factory Exposure
	boost::python::class_<ChargeFactory>("ChargeFactory", boost::python::no_init)
		.def(boost::python::init<bool>())
		.def("setup", &ChargeFactory::setup)
		.add_property("chargeMap", &ChargeFactory::chargeMap)
		.def("getChargeDiagnostic", &ChargeFactory::getChargeDiagnostic, boost::python::return_value_policy<boost::python::reference_existing_object>())
		.def("getChargeDiagnostics", &ChargeFactory::getChargeDiagnostics)
		.def("getAllChargeDiagnostics", &ChargeFactory::getAllChargeDiagnostics)
		.def("monitorForNShots", &ChargeFactory::monitorForNShots)
		.def("monitorForNShots", &ChargeFactory::monitorForNShots_Py)
		.def("isMonitoring", &ChargeFactory::isMonitoring)
		.def("getPosition", &ChargeFactory::getPosition)
		.def("getQ", &ChargeFactory::getQ)
		.def("getQVector", &ChargeFactory::getQVector_Py)
		.def("getQBuffer", &ChargeFactory::getQBuffer_Py)
		.def("getPositions", &ChargeFactory::getPositions_Py)
		.def("getQVectors", &ChargeFactory::getQVectors_Py)
		.def("getQBuffers", &ChargeFactory::getQBuffers_Py)
		.def("getAllQ", &ChargeFactory::getAllQ)
		.def("getAllPosition", &ChargeFactory::getAllPosition_Py)
		.def("getAllQVector", &ChargeFactory::getAllQVector)
		.def("getAllQBuffer", &ChargeFactory::getAllQBuffer)
		.add_property("logger", &ChargeFactory::messenger);
		// Hardware Factory Exposure
	boost::python::class_<HardwareFactory>("HardwareFactory", boost::python::init<>())
		.def(boost::python::init<bool>())
		.def("setup", &HardwareFactory::setup)
		.add_property("magnetFactory", &HardwareFactory::magnetFactory)
		.def("getMagnetFactory", &HardwareFactory::getMagnetFactory, boost::python::return_value_policy<boost::python::reference_existing_object>())
		.add_property("bpmFactory", &HardwareFactory::bpmFactory)
		.def("getBPMFactory", &HardwareFactory::getBPMFactory, boost::python::return_value_policy<boost::python::reference_existing_object>())
		.add_property("chargeFactory", &HardwareFactory::chargeFactory)
		.def("getChargeFactory", &HardwareFactory::getChargeFactory, boost::python::return_value_policy<boost::python::reference_existing_object>())
		.add_property("hardwareMap", &HardwareFactory::hardwareMap);
		//.def("debugMessagesOn", &HardwareFactory::debugMessagesOn)
		//.def("debugMessagesOff", &HardwareFactory::debugMessagesOff)
		//.def("messagesOn", &HardwareFactory::messagesOn)
		//.def("messagesOff", &HardwareFactory::messagesOff);

	boost::python::object debugMessagesOn_Py = boost::python::make_function(&LoggingSystem::debugMessagesOn);
	boost::python::def("debugMessagesOn", debugMessagesOn_Py);
	boost::python::object debugMessagesOff_Py = boost::python::make_function(&LoggingSystem::debugMessagesOff);
	boost::python::def("debugMessagesOff", debugMessagesOff_Py);
	boost::python::object messagesOn_Py = boost::python::make_function(&LoggingSystem::messagesOn);
	boost::python::def("messagesOn", messagesOn_Py);
	boost::python::object messagesOff_Py = boost::python::make_function(&LoggingSystem::messagesOff);
	boost::python::def("messagesOff", messagesOff_Py);
	boost::python::object isMessagingOn_Py = boost::python::make_function(&LoggingSystem::isMessagingOn);
	boost::python::def("isMessagingOn", isMessagingOn_Py);
	boost::python::object isDebugOn_Py = boost::python::make_function(&LoggingSystem::isDebugOn);
	boost::python::def("isDebugOn", isDebugOn_Py);



	/*NOT SURE IF THESE NEED TO BE EXPOSED OR NOT*/
	//// Hardware Vector Exposure
	//boost::python::class_<std::map<std::string, Hardware*> >("hardwareMap")
	//	.def(boost::python::map_indexing_suite< std::map<std::string, Hardware*> >());
	////Magnet Vector Exposure
	//boost::python::class_<std::map<std::string, Magnet*> >("magnetMap")
	//	.def(boost::python::map_indexing_suite< std::map<std::string, Magnet*> >());

}
