/*
		THIS CLASS IS FOR EXPOSURE TO PYTHON
		ALL IN OnE PLACE, SO WE DOn'T HAVE TO
		LINK EVERY CLASS WITH BOOST.PYTHON.
		HOPEFULLY, IT SHOULD ALL HAPPEN HERE.

		// This should include all the different HardwarePYTHONInterface.cpp files
*/

#ifdef PYTHON_INTERFACE_EXPORTS
#define PYTHON_INTERFACE_API __declspec(dllexport)
#else
#define PYTHON_INTERFACE_API __declspec(dllimport)
#endif 


#include <boost/python/suite/indexing/map_indexing_suite.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/enum.hpp>
#include "LoggingSystem.h"
#include "Hardware.h"
#include "HardwareFactory.h"
#include "MagnetPythonInterface.h"
#include "BPMPythonInterface.h"

//#include "Magnet.h"
//#include "MagnetFactory.h"
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
		.value("On", STATE::ON)
		.value("OFF", STATE::OFF)
		.value("ERROR", STATE::ERR)
		.value("UNKNOWN_NAME", STATE::UNKNOWN_NAME)
		.value("SUCCESS", STATE::SUCCESS)
		.value("OFFLINE", STATE::OFFLINE)
		.value("PHYSICAL", STATE::PHYSICAL)
		.value("VIRTUAL", STATE::VIRTUAL)
		.value("GOOD", STATE::GOOD)
		.value("BAD", STATE::BAD)
		.value("OK", STATE::OK)
		;
	boost::python::class_<EPICSInterface>("EPICSInterface", boost::python::no_init)
		.def("debugMessagesOn", &EPICSInterface::debugMessagesOn)
		.def("debugMessagesOff", &EPICSInterface::debugMessagesOff)
		.def("messagesOn", &EPICSInterface::messagesOn)
		.def("messagesOff", &EPICSInterface::messagesOff);

	//boost::python::class_<EPICSMagnetInterface, boost::python::bases<EPICSInterface>, boost::noncopyable>("EPICSMagnetInterface", boost::python::no_init);
	// Hardware Exposure
	boost::python::class_<Hardware>("Hardware", boost::python::no_init)
		.def_readonly("machineArea", &Hardware::machineArea)
		.def_readonly("hardwareType", &Hardware::hardwareType)
		.def_readonly("specificHardwareParameters", &Hardware::specificHardwareParameters)
		.def("getMachineArea", &Hardware::getMachineArea)
		.def("getHardwareType", &Hardware::getHardwareType)
		.def("getHardwareName", &Hardware::getHardwareName)
		.def("getMode", &Hardware::getMode)
		.def("debugMessagesOn", &Hardware::debugMessagesOn)
		.def("debugMessagesOff", &Hardware::debugMessagesOff)
		.def("messagesOn", &Hardware::messagesOn)
		.def("messagesOff", &Hardware::messagesOff)
		.def("getSpecificHardwareParameters", &Hardware::getSpecificHardwareParameters);


	// expose magnet object and magnetfactoryobejct
	BOOST_PYTHON_MAGNET_INCLUDE::expose_magnet_object();
	BOOST_PYTHON_MAGNET_INCLUDE::expose_magnet_factory_object();
	BOOST_PYTHON_MAGNET_INCLUDE::expose_magnet_state_struct();
	BOOST_PYTHON_BPM_INCLUDE::expose_bpm_object();
	BOOST_PYTHON_BPM_INCLUDE::expose_bpm_factory_object();


	//// Magnet Exposure
	//boost::python::class_<Magnet, boost::python::bases<Hardware>, boost::noncopyable>("Magnet", boost::python::no_init)
	//	.add_property("current", &Magnet::getCurrent, &Magnet::setEPICSCurrent)
	//	.add_property("psu_state", &Magnet::getPSUState, &Magnet::setEPICSPSUState)
	//	.add_property("ri_current", &Magnet::RICurrent)
	//	.add_property("name", &Magnet::getHardwareName)
	//	.add_property("manufacturer", &Magnet::getManufacturer)
	//	.add_property("serial_number", &Magnet::getSerialNumber)
	//	.add_property("magnet_type", &Magnet::getMagnetType)
	//	.add_property("epicsInterface", &Magnet::epicsInterface)
	//	.def("getCurrent", &Magnet::getCurrent)
	//	.def("setCurrent", &Magnet::setEPICSCurrent)
	//	.def("getRICurrent", &Magnet::getRICurrent);
	// BPM Exposure
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
		.def("ismonitoring", &Charge::ismonitoring);
		// Parameter Map Exposure
		boost::python::class_<std::map<std::string, double> >("numericalParamMap")
		.def(boost::python::map_indexing_suite<std::map<std::string, double> >());
	boost::python::class_<std::map<std::string, std::vector< double > > >("numericalVectorParamMap")
		.def(boost::python::map_indexing_suite<std::map<std::string, std::vector< double > > >());
	boost::python::class_<std::map<std::string, std::string> >("stringParamMap")
		.def(boost::python::map_indexing_suite<std::map<std::string, std::string> >());

	////Magnet Factory Exposure
	//STATE(MagnetFactory:: * turnOnSingle)(const std::string&) = &MagnetFactory::turnOn;
	//STATE(MagnetFactory:: * turnOffSingle)(const std::string&) = &MagnetFactory::turnOff;
	//boost::python::class_<MagnetFactory>("MagnetFactory", boost::python::no_init)
	//	.def(boost::python::init<STATE>())
	//	.def("setup", &MagnetFactory::setup)
	//	.add_property("magnetMap", &MagnetFactory::magnetMap)
	//	.def("getMagnet", &MagnetFactory::getMagnet, boost::python::return_value_policy<boost::python::reference_existing_object>())
	//	//.def("getMagnets", &MagnetFactory::getMagnets)
	//	//.def("getAllMagnets", &MagnetFactory::getAllMagnets)
	//	.def("getCurrent", &MagnetFactory::getCurrent)
	//	.def("getCurrents", &MagnetFactory::getCurrents_Py)
	//	.def("getAllMagnetCurrents", &MagnetFactory::getAllMagnetCurrents_Py)
	//	.def("setCurrent", &MagnetFactory::setCurrent)
	//	.def("setCurrents", &MagnetFactory::setCurrents_Py)
	//	.def("getRICurrent", &MagnetFactory::getRICurrent)
	//	.def("getRICurrents", &MagnetFactory::getRICurrents_Py)
	//	.def("turnOn", turnOnSingle)
	//	.def("turnOn", &MagnetFactory::turnOn_Py)
	//	.def("turnOff", turnOffSingle)
	//	.def("turnOff", &MagnetFactory::turnOff_Py)
	//	.def("debugMessagesOn", &MagnetFactory::debugMessagesOn)
	//	.def("debugMessagesOff", &MagnetFactory::debugMessagesOff)
	//	.def("messagesOn", &MagnetFactory::messagesOn)
	//	.def("messagesOff", &MagnetFactory::messagesOff);
		//BPM Factory Exposure
	//Charge Factory Exposure
	boost::python::class_<ChargeFactory>("ChargeFactory", boost::python::no_init)
		.def(boost::python::init<STATE>())
		.def("setup", &ChargeFactory::setup)
		.add_property("chargeMap", &ChargeFactory::chargeMap)
		.def("getChargeDiagnostic", &ChargeFactory::getChargeDiagnostic, boost::python::return_value_policy<boost::python::reference_existing_object>())
		.def("getChargeDiagnostics", &ChargeFactory::getChargeDiagnostics)
		.def("getAllChargeDiagnostics", &ChargeFactory::getAllChargeDiagnostics)
		.def("monitorForNShots", &ChargeFactory::monitorForNShots)
		.def("monitorForNShots", &ChargeFactory::monitorForNShots_Py)
		.def("ismonitoring", &ChargeFactory::ismonitoring)
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
		.def("getAllQBuffer", &ChargeFactory::getAllQBuffer);
		// Hardware Factory Exposure
	boost::python::class_<HardwareFactory>("HardwareFactory", boost::python::init<STATE>())
		//.def(boost::python::init<STATE>())
		.def("setup", &HardwareFactory::setup)
		.add_property("magnetFactory", &HardwareFactory::magnetFactory)
		.def("getMagnetFactory", &HardwareFactory::getMagnetFactory, boost::python::return_value_policy<boost::python::reference_existing_object>())
		.add_property("bpmFactory", &HardwareFactory::bpmFactory)
		.def("getBPMFactory", &HardwareFactory::getBPMFactory, boost::python::return_value_policy<boost::python::reference_existing_object>())
		.add_property("chargeFactory", &HardwareFactory::chargeFactory)
		.def("getChargeFactory", &HardwareFactory::getChargeFactory, boost::python::return_value_policy<boost::python::reference_existing_object>())
		.add_property("hardwareMap", &HardwareFactory::hardwareMap)
		.def("debugMessagesOn", &HardwareFactory::debugMessagesOn)
		.def("debugMessagesOff", &HardwareFactory::debugMessagesOff)
		.def("messagesOn", &HardwareFactory::messagesOn)
		.def("messagesOff", &HardwareFactory::messagesOff);


	boost::python::object dumpToFile_Py = boost::python::make_function(&LoggingSystem::dumpToFile);
	boost::python::def("dumpToFile", dumpToFile_Py);



	/*NOT SURE IF THESE NEED TO BE EXPOSED OR NOT*/
	//// Hardware Vector Exposure
	//boost::python::class_<std::map<std::string, Hardware*> >("hardwareMap")
	//	.def(boost::python::map_indexing_suite< std::map<std::string, Hardware*> >());
	////Magnet Vector Exposure
	//boost::python::class_<std::map<std::string, Magnet*> >("magnetMap")
	//	.def(boost::python::map_indexing_suite< std::map<std::string, Magnet*> >());

}
