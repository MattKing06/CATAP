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
#include <MagnetPythonInterface.h>
#include <ValvePythonInterface.h>
#include <BPMPythonInterface.h>
#include <ChargePythonInterface.h>
#include <ScreenPythonInterface.h>
#include <vector>
#include <map>




BOOST_PYTHON_MODULE(CATAP)
{

	boost::python::docstring_options CATAP_docstring_options;//(true, true, false);
	CATAP_docstring_options.enable_all();
	CATAP_docstring_options.disable_cpp_signatures();

	//Global State Enum exposure
	// TODO move to function
	boost::python::enum_<STATE>("STATE","Global Enums used for displaying States")
		.value("ON", STATE::ON)
		.value("OFF", STATE::OFF)
		// tow names for ERR, (CF atm you can't use ERR in windows due to some windows header stuff)
		.value("ERR", STATE::ERR)
		.value("ERROR", STATE::ERR)
		.value("UNKNOWN_NAME", STATE::UNKNOWN_NAME)
		.value("SUCCESS", STATE::SUCCESS)
		.value("FAIL", STATE::FAIL)
		.value("OFFLINE", STATE::OFFLINE)
		.value("PHYSICAL", STATE::PHYSICAL)
		.value("VIRTUAL", STATE::VIRTUAL)
		.value("GOOD", STATE::GOOD)
		.value("BAD", STATE::BAD)
		.value("OK", STATE::OK)
		.value("OPEN", STATE::OPEN)
		.value("CLOSED", STATE::CLOSED)
		.value("NONLINEAR", STATE::NONLINEAR)
		.value("UNKNOWN", STATE::UNKNOWN)
		.value("ENABLED", STATE::ENABLED)
		.value("DISABLED", STATE::DISABLED)
		.value("HRETRACTED", STATE::HRETRACTED)
		.value("HMAX", STATE::HMAX)
		.value("HSLIT1", STATE::HSLIT1)
		.value("HSLIT2", STATE::HSLIT2)
		.value("HSLIT3", STATE::HSLIT3)
		.value("HAPT1", STATE::HAPT1)
		.value("HAPT2", STATE::HAPT2)
		.value("HAPT3", STATE::HAPT3)
		.value("VRETRACTED", STATE::VRETRACTED)
		.value("VMAX", STATE::VMAX)
		.value("VRF", STATE::VRF)
		.value("VMIRROR", STATE::VMIRROR)
		.value("VYAG", STATE::VYAG)
		.value("VGRAT", STATE::VGRAT)
		.value("VCOL", STATE::VCOL)
		.value("VSLIT1", STATE::VSLIT1)
		.value("RETRACTED", STATE::RETRACTED)
		.value("YAG", STATE::YAG)
		.value("RF", STATE::RF)
		;

	// TODOD: move to function
	boost::python::enum_<TYPE>("TYPE")
		.value("VELA_PNEUMATIC", TYPE::VELA_PNEUMATIC)
		.value("VELA_HV_MOVER", TYPE::VELA_HV_MOVER)
		.value("CLARA_HV_MOVER", TYPE::CLARA_HV_MOVER)
		.value("CLARA_V_MOVER", TYPE::CLARA_V_MOVER)
		.value("CLARA_PNEUMATIC", TYPE::CLARA_PNEUMATIC)
		.value("UNKNOWN_SCREEN_TYPE", TYPE::UNKNOWN_SCREEN_TYPE)
		.value("HORIZONTAL", TYPE::HORIZONTAL)
		.value("VERTICAL", TYPE::VERTICAL)
		.value("PNEUMATIC", TYPE::PNEUMATIC)
		.value("UNKNOWN_TYPE", TYPE::UNKNOWN_TYPE)
		.value("MAGNET", TYPE::MAGNET)
		.value("QUADRUPOLE", TYPE::QUADRUPOLE)
		.value("DIPOLE", TYPE::DIPOLE)
		//.value("HVCOR", TYPE::HVCOR)
		.value("VERTICAL_CORRECTOR", TYPE::VERTICAL_CORRECTOR)
		.value("HORIZONTAL_CORRECTOR", TYPE::HORIZONTAL_CORRECTOR)
		.value("SOLENOID", TYPE::SOLENOID)
		.value("BUCKING_SOLENOID", TYPE::BUCKING_SOLENOID)
		.value("NR", TYPE::NR)
		.value("BIPOLAR", TYPE::BIPOLAR)
		.value("NR_GANGED", TYPE::NR_GANGED)
		.value("POSITIVE_ONLY", TYPE::POSITIVE_ONLY)
		.value("BA1", TYPE::BA1)
		.value("BA2", TYPE::BA2)
		.value("VELA", TYPE::VELA)
		.value("GUN", TYPE::GUN)
		.value("S01", TYPE::S01)
		.value("S02", TYPE::S02)
		.value("C2V", TYPE::C2V)
		.value("S01", TYPE::S01)
		.value("CLARA_PH1", TYPE::CLARA_PH1)
		.value("CLARA_2_BA1", TYPE::CLARA_2_BA1)
		.value("CLARA_2_BA1_BA2", TYPE::CLARA_2_BA1_BA2)
		;

	//boost::python::class_<EPICSMagnetInterface, boost::python::bases<EPICSInterface>, boost::noncopyable>("EPICSMagnetInterface", boost::python::no_init);
	// Hardware Exposure
	// TODOD: move to function
	boost::python::class_<Hardware>("Hardware", boost::python::no_init)
		.def_readonly("machineArea", &Hardware::machineArea)
		.def_readonly("hardwareType", &Hardware::hardwareType)
		.def_readonly("specificHardwareParameters", &Hardware::specificHardwareParameters)
		.def("getMachineAreaString", &Hardware::getMachineAreaString)
		.def("getHardwareTypeString", &Hardware::getHardwareTypeString)
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
	BOOST_PYTHON_MAGNET_INCLUDE::expose_magnet_state_struct_and_dburt();
	BOOST_PYTHON_MAGNET_INCLUDE::expose_magnet_object();
	BOOST_PYTHON_MAGNET_INCLUDE::expose_magnet_factory_object();

	//BOOST_PYTHON_MAGNET_INCLUDE::expose_magnet_state_struct();
	BOOST_PYTHON_BPM_INCLUDE::expose_bpm_object();
	BOOST_PYTHON_BPM_INCLUDE::expose_bpm_factory_object();
	BOOST_PYTHON_CHARGE_INCLUDE::expose_charge_object();
	BOOST_PYTHON_CHARGE_INCLUDE::expose_charge_factory_object();
	BOOST_PYTHON_SCREEN_INCLUDE::expose_screen_object();
	BOOST_PYTHON_SCREEN_INCLUDE::expose_screen_factory_object();

	//expose valve object and valveFactory object
	BOOST_PYTHON_VALVE_INCLUDE::exposeValveObject();
	BOOST_PYTHON_VALVE_INCLUDE::exposeValveFactoryObject();

	// BPM Exposure
		// Charge Exposure

		// Parameter Map Exposure
	boost::python::class_<std::map<std::string, double> >("numericalParamMap")
		.def(boost::python::map_indexing_suite<std::map<std::string, double> >());
	boost::python::class_<std::map<std::string, std::vector< double > > >("numericalVectorParamMap")
		.def(boost::python::map_indexing_suite<std::map<std::string, std::vector< double > > >());
	boost::python::class_<std::map<std::string, std::string> >("stringParamMap")
		.def(boost::python::map_indexing_suite<std::map<std::string, std::string> >());
	boost::python::class_<std::map<std::string, Screen> >("screenMap")
		.def(boost::python::map_indexing_suite<std::map<std::string, Screen> >());
	boost::python::class_<std::vector< double > >("stdVectorDouble")
		.def(boost::python::vector_indexing_suite<std::vector< double > >());
	boost::python::class_<boost::circular_buffer< double > >("circularBufferDouble")
		.def(boost::python::vector_indexing_suite<boost::circular_buffer< double > >());
	boost::python::class_<boost::circular_buffer< std::vector< double > > >("circularBufferDoubleVector")
		.def(boost::python::vector_indexing_suite<boost::circular_buffer< std::vector< double > > >());
	boost::python::class_<boost::circular_buffer< STATE > >("circularBufferState")
		.def(boost::python::vector_indexing_suite<boost::circular_buffer< STATE > >());
	boost::python::class_<std::vector< STATE > >("vectorState")
		.def(boost::python::vector_indexing_suite<std::vector< STATE > >());
	boost::python::class_<std::vector< boost::circular_buffer< STATE > > >("vectorcircularBufferState")
		.def(boost::python::vector_indexing_suite<boost::circular_buffer< STATE > >());
	boost::python::class_<std::vector< std::vector< STATE > > >("vectorvectorState")
		.def(boost::python::vector_indexing_suite<std::vector< std::vector< STATE > > >());

		//BPM Factory Exposure
	//Charge Factory Exposure


			// OVERLOADED SETUP FUNCTIONS TO ALLOW USER FULL CONTROL AND "FUTURE PROOVED VERSION PARAMETER" 
	MagnetFactory& (HardwareFactory::* getMagnetFactory_NoArg)() = &HardwareFactory::getMagnetFactory;
	MagnetFactory& (HardwareFactory::* setup_AreaArg)(TYPE ) = &HardwareFactory::getMagnetFactory;
	MagnetFactory& (HardwareFactory::* setup_AreasArg)(const boost::python::list&) = &HardwareFactory::getMagnetFactory;


		// Hardware Factory Exposure
	boost::python::class_<HardwareFactory>("HardwareFactory", "The holder of all hardware", 
		boost::python::init<STATE>((boost::python::args("self"),boost::python::args("mode"))))
		.def("setup",&HardwareFactory::setup, (boost::python::args("self"),boost::python::arg("hardwareType"),boost::python::args("version")))
		.add_property("magnetFactory", &HardwareFactory::magnetFactory)
		.def("getMagnetFactory", getMagnetFactory_NoArg, boost::python::arg("self"), boost::python::return_value_policy<boost::python::reference_existing_object>())
		.def("getMagnetFactory", setup_AreaArg, boost::python::arg("self"), boost::python::return_value_policy<boost::python::reference_existing_object>())
		.def("getMagnetFactory", setup_AreasArg, boost::python::arg("self"), boost::python::return_value_policy<boost::python::reference_existing_object>())
		.add_property("bpmFactory", &HardwareFactory::bpmFactory)
		.def("getBPMFactory", &HardwareFactory::getBPMFactory, boost::python::arg("self"), boost::python::return_value_policy<boost::python::reference_existing_object>())
		.add_property("chargeFactory", &HardwareFactory::chargeFactory)
		.def("getChargeFactory", &HardwareFactory::getChargeFactory, boost::python::arg("self"), boost::python::return_value_policy<boost::python::reference_existing_object>())
		.add_property("screenFactory", &HardwareFactory::screenFactory)
		.def("getScreenFactory", &HardwareFactory::getScreenFactory, boost::python::arg("self"), boost::python::return_value_policy<boost::python::reference_existing_object>())
		.add_property("valveFactory", &HardwareFactory::valveFactory)
		.def("getValveFactory", &HardwareFactory::getValveFactory, boost::python::arg("self"), boost::python::return_value_policy<boost::python::reference_existing_object>())
		.add_property("hardwareMap", &HardwareFactory::hardwareMap)
		.def("debugMessagesOn", &HardwareFactory::debugMessagesOn, boost::python::arg("self"))
		.def("debugMessagesOff", &HardwareFactory::debugMessagesOff, boost::python::arg("self"))
		.def("messagesOn", &HardwareFactory::messagesOn, boost::python::arg("self"))
		.def("messagesOff", &HardwareFactory::messagesOff, boost::python::arg("self"));


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
