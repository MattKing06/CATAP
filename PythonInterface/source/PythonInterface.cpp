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
#include "LoggingSystem.h"
#include "Hardware.h"
#include "HardwareFactory.h"
#include "Magnet.h"
#include "MagnetFactory.h"
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
		.add_property("name", &Magnet::getHardwareName)
		.add_property("manufacturer", &Magnet::getManufacturer)
		.add_property("serial_number", &Magnet::getSerialNumber)
		.add_property("magnet_type", &Magnet::getMagnetType)
		.def("getCurrent", &Magnet::getCurrent)
		.def("setCurrent", &Magnet::setEPICSCurrent);
	// Parameter Map Exposure
	boost::python::class_<std::map<std::string, double> >("numericalParamMap")
		.def(boost::python::map_indexing_suite<std::map<std::string, double> >());
	boost::python::class_<std::map<std::string, std::string> >("stringParamMap")
		.def(boost::python::map_indexing_suite<std::map<std::string, std::string> >());
	//Magnet Factory Exposure
	bool(MagnetFactory::*turnOnSingle)(const std::string&) = &MagnetFactory::turnOn;
	bool(MagnetFactory::*turnOffSingle)(const std::string&) = &MagnetFactory::turnOff;
	boost::python::class_<MagnetFactory>("MagnetFactory", boost::python::no_init)
		.def(boost::python::init<bool>())
		.def("setup", &MagnetFactory::setup)
		.add_property("magnetMap", &MagnetFactory::magnetMap)
		.def("getMagnet", &MagnetFactory::getMagnet, boost::python::return_value_policy<boost::python::reference_existing_object>())
		.def("getMagnets", &MagnetFactory::getMagnets)
		.def("getAllMagnets", &MagnetFactory::getAllMagnets)
		.def("getCurrent", &MagnetFactory::getCurrent)
		.def("getCurrents", &MagnetFactory::getCurrents_Py)
		.def("getAllMagnetCurrents", &MagnetFactory::getAllMagnetCurrents)
		.def("setCurrent", &MagnetFactory::setCurrent)
		.def("setCurrents", &MagnetFactory::setCurrents_Py)
		.def("turnOn", turnOnSingle)
		.def("turnOn", &MagnetFactory::turnOn_Py)
		.def("turnOff", turnOffSingle)
		.def("turnOff", &MagnetFactory::turnOff_Py)
		.add_property("logger", &MagnetFactory::messenger);
	// Hardware Factory Exposure


	boost::python::class_<HardwareFactory>("HardwareFactory", boost::python::init<>())
		.def(boost::python::init<bool>())
		.def("setup", &HardwareFactory::setup)
		.add_property("magnetFactory", &HardwareFactory::magnetFactory)
		.def("getMagnetFactory", &HardwareFactory::getMagnetFactory, boost::python::return_value_policy<boost::python::reference_existing_object>())
		.add_property("hardwareMap", &HardwareFactory::hardwareMap);



	/*NOT SURE IF THESE NEED TO BE EXPOSED OR NOT*/
	//// Hardware Vector Exposure
	//boost::python::class_<std::map<std::string, Hardware*> >("hardwareMap")
	//	.def(boost::python::map_indexing_suite< std::map<std::string, Hardware*> >());
	////Magnet Vector Exposure
	//boost::python::class_<std::map<std::string, Magnet*> >("magnetMap")
	//	.def(boost::python::map_indexing_suite< std::map<std::string, Magnet*> >());

}
