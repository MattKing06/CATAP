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
#include "ConfigReader.h"
#include <vector>
#include <map>


BOOST_PYTHON_MODULE(CATAP)
{
	// Logging System Exposure
	boost::python::class_<LoggingSystem>("LoggingSystem", boost::python::init<bool, bool>())
		.def("isDebugOn", &LoggingSystem::isDebugOn)
		.def("isMessagingOn", &LoggingSystem::isMessagingOn)
		.def("messagesOn", &LoggingSystem::messagesOn)
		.def("messagesOff", &LoggingSystem::messagesOff)
		.def("debugMessagesOn", &LoggingSystem::debugMessagesOn)
		.def("debugMessagesOff", &LoggingSystem::debugMessagesOff)
		.def("printDebugMessage", &LoggingSystem::printDebugMessage)
		.def("printMessage", &LoggingSystem::printMessage);
	//Global State Enum exposure
	boost::python::enum_<STATE>("STATE")
		.value("ON", STATE::ON)
		.value("OFF", STATE::OFF);
	// Hardware Exposure
	boost::python::class_<Hardware>("Hardware", boost::python::no_init)
		.def_readonly("machineArea", &Hardware::machineArea)
		.def_readonly("hardwareType", &Hardware::hardwareType)
		.def_readonly("specificHardwareParameters", &Hardware::specificHardwareParameters)
		.def("getMachineArea", &Hardware::getMachineArea)
		.def("getHardwareType", &Hardware::getHardwareType)
		.def("getHardwareName", &Hardware::getHardwareName)
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
		.def("turnOff", &MagnetFactory::turnOff_Py)
		.add_property("logger", &MagnetFactory::messenger);
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
		//.def("getXYPositionVector", &BPMFactory::getXYPositionVector_Py)
		.def("getResolutions", &BPMFactory::getResolutions_Py)
		.def("getPositions", &BPMFactory::getPositions_Py)
		.def("getXPVVectors", &BPMFactory::getXPVVectors_Py)
		.def("getYPVVectors", &BPMFactory::getYPVVectors_Py)
		.def("getQVectors", &BPMFactory::getQVectors_Py)
		.def("getDataVectors", &BPMFactory::getDataVectors_Py)
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
		.def("getAllXYPositionVectors", &BPMFactory::getAllXYPositionVectors_Py)
		.add_property("logger", &BPMFactory::messenger);
	// Hardware Factory Exposure


	boost::python::class_<HardwareFactory>("HardwareFactory", boost::python::init<>())
		.def(boost::python::init<bool>())
		.def("setup", &HardwareFactory::setup)
		.add_property("magnetFactory", &HardwareFactory::magnetFactory)
		.def("getMagnetFactory", &HardwareFactory::getMagnetFactory, boost::python::return_value_policy<boost::python::reference_existing_object>())
		.def("getBPMFactory", &HardwareFactory::getBPMFactory, boost::python::return_value_policy<boost::python::reference_existing_object>())
		.add_property("hardwareMap", &HardwareFactory::hardwareMap);



	/*NOT SURE IF THESE NEED TO BE EXPOSED OR NOT*/
	//// Hardware Vector Exposure
	//boost::python::class_<std::map<std::string, Hardware*> >("hardwareMap")
	//	.def(boost::python::map_indexing_suite< std::map<std::string, Hardware*> >());
	////Magnet Vector Exposure
	//boost::python::class_<std::map<std::string, Magnet*> >("magnetMap")
	//	.def(boost::python::map_indexing_suite< std::map<std::string, Magnet*> >());

}
