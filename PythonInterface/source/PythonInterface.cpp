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
#include "ConfigReader.h"
#include <vector>
#include <map>


BOOST_PYTHON_MODULE(CATAP)
{
	//Global State Enum exposure
	boost::python::enum_<STATE>("STATE")
		.value("ON", STATE::ON)
		.value("OFF", STATE::OFF);
	boost::python::class_<EPICSInterface>("EPICSInterface", boost::python::no_init)
		.def("debugMessagesOn", &EPICSInterface::debugMessagesOn)
		.def("debugMessagesOff", &EPICSInterface::debugMessagesOff)
		.def("messagesOn", &EPICSInterface::messagesOn)
		.def("messagesOff", &EPICSInterface::messagesOff);
	boost::python::class_<EPICSMagnetInterface, boost::python::bases<EPICSInterface>, boost::noncopyable>("EPICSMagnetInterface", boost::python::no_init);
	// Hardware Exposure
	boost::python::class_<Hardware>("Hardware", boost::python::no_init)
		.def_readonly("machineArea", &Hardware::machineArea)
		.def_readonly("hardwareType", &Hardware::hardwareType)
		.def_readonly("specificHardwareParameters", &Hardware::specificHardwareParameters)
		.def("getMachineArea", &Hardware::getMachineArea)
		.def("getHardwareType", &Hardware::getHardwareType)
		.def("getHardwareName", &Hardware::getHardwareName)
		.def("getSpecificHardwareParameters", &Hardware::getSpecificHardwareParameters)
		.def("debugMessagesOn", &Hardware::debugMessagesOn)
		.def("debugMessagesOff", &Hardware::debugMessagesOff)
		.def("messagesOn", &Hardware::messagesOn)
		.def("messagesOff", &Hardware::messagesOff);
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
		.def("getAllMagnetCurrents", &MagnetFactory::getAllMagnetCurrents_Py)
		.def("setCurrent", &MagnetFactory::setCurrent)
		.def("setCurrents", &MagnetFactory::setCurrents_Py)
		.def("getRICurrent", &MagnetFactory::getRICurrent)
		.def("getRICurrents", &MagnetFactory::getRICurrents_Py)
		.def("turnOn", turnOnSingle)
		.def("turnOn", &MagnetFactory::turnOn_Py)
		.def("turnOff", turnOffSingle)
		.def("turnOff", &MagnetFactory::turnOff_Py)
		.def("debugMessagesOn", &MagnetFactory::debugMessagesOn)
		.def("debugMessagesOff", &MagnetFactory::debugMessagesOff)
		.def("messagesOn", &MagnetFactory::messagesOn)
		.def("messagesOff", &MagnetFactory::messagesOff);
	// Hardware Factory Exposure


	boost::python::class_<HardwareFactory>("HardwareFactory", boost::python::init<>())
		.def(boost::python::init<bool>())
		.def("setup", &HardwareFactory::setup)
		.add_property("magnetFactory", &HardwareFactory::magnetFactory)
		.def("getMagnetFactory", &HardwareFactory::getMagnetFactory, boost::python::return_value_policy<boost::python::reference_existing_object>())
		.add_property("hardwareMap", &HardwareFactory::hardwareMap)
		.def("debugMessagesOn", &HardwareFactory::debugMessagesOn)
		.def("debugMessagesOff", &HardwareFactory::debugMessagesOff)
		.def("messagesOn", &HardwareFactory::messagesOn)
		.def("messagesOff", &HardwareFactory::messagesOff);



	/*NOT SURE IF THESE NEED TO BE EXPOSED OR NOT*/
	//// Hardware Vector Exposure
	//boost::python::class_<std::map<std::string, Hardware*> >("hardwareMap")
	//	.def(boost::python::map_indexing_suite< std::map<std::string, Hardware*> >());
	////Magnet Vector Exposure
	//boost::python::class_<std::map<std::string, Magnet*> >("magnetMap")
	//	.def(boost::python::map_indexing_suite< std::map<std::string, Magnet*> >());

}
