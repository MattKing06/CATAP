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
		.def("getHardwareType", &Hardware::getHardwareType);
		// Magnet Exposure
	boost::python::class_<Magnet, boost::python::bases<Hardware>, boost::noncopyable>("Magnet", boost::python::no_init)
		.add_property("current", &Magnet::getCurrent, &Magnet::setCurrent)
		.add_property("name", &Magnet::getFullPVName)
		.add_property("manufacturer", &Magnet::getManufacturer)
		.add_property("serial_number", &Magnet::getSerialNumber)
		.add_property("magnet_type", &Magnet::getMagnetType)
		.def("getCurrent", &Magnet::getCurrent)
		.def("setCurrent", &Magnet::setCurrent);
	// Parameter Map Exposure
	boost::python::class_<std::map<std::string, double> >("numericalParamMap")
		.def(boost::python::map_indexing_suite<std::map<std::string, double> >());
	boost::python::class_<std::map<std::string, std::string> >("stringParamMap")
		.def(boost::python::map_indexing_suite<std::map<std::string, std::string> >());
	// Hardware Vector Exposure
	boost::python::class_<std::vector<Hardware> >("hardwareVec")
		.def(boost::python::vector_indexing_suite< std::vector<Hardware> >());
	//Magnet Vector Exposure
	boost::python::class_<std::vector<Magnet*> >("magnetVec")
		.def(boost::python::vector_indexing_suite< std::vector<Magnet*> >());
	//Magnet Factory Exposure
	boost::python::class_<MagnetFactory>("MagnetFactory", boost::python::no_init)
		.def("setup", &MagnetFactory::setup)
		.def_readonly("magnetVector", &MagnetFactory::magnetVec)
		.def("getMagnet", &MagnetFactory::getMagnet, boost::python::return_value_policy<boost::python::reference_existing_object>())
		.def("getMagnets", &MagnetFactory::getMagnets)
		.def("getAllMagnets", &MagnetFactory::getAllMagnets)
		.def("getCurrent", &MagnetFactory::getCurrent)
		.def("getCurrents", &MagnetFactory::getCurrents)
		.def("getAllMagnetCurrents", &MagnetFactory::getAllMagnetCurrents)
		.def("setMagnetCurrent", &MagnetFactory::setCurrent)
		.add_property("logger", &MagnetFactory::messenger);
	// Hardware Factory Exposure
	boost::python::class_<HardwareFactory>("HardwareFactory", boost::python::init<>())
		.def("setup", &HardwareFactory::setup)
		.def("getMagnetFactory", &HardwareFactory::getMagnetFactory, boost::python::return_value_policy<boost::python::reference_existing_object>());

}
