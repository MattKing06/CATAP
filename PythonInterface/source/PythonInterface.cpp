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

//#include <MagnetPythonInterface.h>
//#include <ValvePythonInterface.h>
//#include <BPMPythonInterface.h>
//#include <ChargePythonInterface.h>
#include <ScreenPythonInterface.h>
//#include <IMGPythonInterface.h>
#include <vector>
#include <map>




BOOST_PYTHON_MODULE(CATAP)
{

	boost::python::docstring_options CATAP_docstring_options;//(true, true, false);
	CATAP_docstring_options.enable_all();
	CATAP_docstring_options.disable_cpp_signatures();

	////Global State Enum exposure
	//boost::python::enum_<STATE>("STATE","Global Enums used for displaying States")
	//	.value("On", STATE::ON)
	//	.value("OFF", STATE::OFF)
	//	.value("ERROR", STATE::ERR)
	//	.value("UNKNOWN_NAME", STATE::UNKNOWN_NAME)
	//	.value("SUCCESS", STATE::SUCCESS)
	//	.value("OFFLINE", STATE::OFFLINE)
	//	.value("PHYSICAL", STATE::PHYSICAL)
	//	.value("VIRTUAL", STATE::VIRTUAL)
	//	.value("GOOD", STATE::GOOD)
	//	.value("BAD", STATE::BAD)
	//	.value("OK", STATE::OK)
	//	.value("OPEN", STATE::OPEN)
	//	.value("CLOSED", STATE::CLOSED)
	//	.value("NONLINEAR", STATE::NONLINEAR)
	//	.value("UNKNOWN", STATE::UNKNOWN)
	//	.value("ENABLED", STATE::ENABLED)
	//	.value("DISABLED", STATE::DISABLED)
	//	.value("HRETRACTED", STATE::HRETRACTED)
	//	.value("HMAX", STATE::HMAX)
	//	.value("HSLIT1", STATE::HSLIT1)
	//	.value("HSLIT2", STATE::HSLIT2)
	//	.value("HSLIT3", STATE::HSLIT3)
	//	.value("HAPT1", STATE::HAPT1)
	//	.value("HAPT2", STATE::HAPT2)
	//	.value("HAPT3", STATE::HAPT3)
	//	.value("VRETRACTED", STATE::VRETRACTED)
	//	.value("VMAX", STATE::VMAX)
	//	.value("VRF", STATE::VRF)
	//	.value("VMIRROR", STATE::VMIRROR)
	//	.value("VYAG", STATE::VYAG)
	//	.value("VGRAT", STATE::VGRAT)
	//	.value("VCOL", STATE::VCOL)
	//	.value("VSLIT1", STATE::VSLIT1)
	//	.value("RETRACTED", STATE::RETRACTED)
	//	.value("YAG", STATE::YAG)
	//	.value("RF", STATE::RF)
	//	;

	//boost::python::enum_<TYPE>("TYPE")
	//	.value("VELA_PNEUMATIC", TYPE::VELA_PNEUMATIC)
	//	.value("VELA_HV_MOVER", TYPE::VELA_HV_MOVER)
	//	.value("CLARA_HV_MOVER", TYPE::CLARA_HV_MOVER)
	//	.value("CLARA_V_MOVER", TYPE::CLARA_V_MOVER)
	//	.value("CLARA_PNEUMATIC", TYPE::CLARA_PNEUMATIC)
	//	.value("UNKNOWN_SCREEN_TYPE", TYPE::UNKNOWN_SCREEN_TYPE)
	//	.value("HORIZONTAL", TYPE::HORIZONTAL)
	//	.value("VERTICAL", TYPE::VERTICAL)
	//	.value("PNEUMATIC", TYPE::PNEUMATIC)
	//	;

	//boost::python::class_<EPICSMagnetInterface, boost::python::bases<EPICSInterface>, boost::noncopyable>("EPICSMagnetInterface", boost::python::no_init);

	//BOOST_PYTHON_MAGNET_INCLUDE::expose_magnet_object();
	//BOOST_PYTHON_MAGNET_INCLUDE::expose_magnet_factory_object();
	//BOOST_PYTHON_MAGNET_INCLUDE::expose_magnet_state_struct();
	//BOOST_PYTHON_BPM_INCLUDE::expose_bpm_object();
	//BOOST_PYTHON_BPM_INCLUDE::expose_bpm_factory_object();
	//BOOST_PYTHON_CHARGE_INCLUDE::expose_charge_object();
	//BOOST_PYTHON_CHARGE_INCLUDE::expose_charge_factory_object();

	//BOOST_PYTHON_SCREEN_INCLUDE::expose_screen_object();
	//BOOST_PYTHON_SCREEN_INCLUDE::expose_screen_factory_object();


	//expose valve object and valveFactory object
	//BOOST_PYTHON_VALVE_INCLUDE::exposeValveObject();
	//BOOST_PYTHON_VALVE_INCLUDE::exposeValveFactoryObject();

	//expose IMG object and IMGFactory object
	//BOOST_PYTHON_IMG_INCLUDE::expose_img_object();
	//BOOST_PYTHON_IMG_INCLUDE::expose_img_factory_object();

	// BPM Exposure
		// Charge Exposure


		//BPM Factory Exposure
	//Charge Factory Exposure



	boost::python::object dumpToFile_Py = boost::python::make_function(&LoggingSystem::dumpToFile);
	boost::python::def("dumpToFile", dumpToFile_Py);


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

	/*NOT SURE IF THESE NEED TO BE EXPOSED OR NOT*/
	//// Hardware Vector Exposure
	//boost::python::class_<std::map<std::string, Hardware*> >("hardwareMap")
	//	.def(boost::python::map_indexing_suite< std::map<std::string, Hardware*> >());
	////Magnet Vector Exposure
	//boost::python::class_<std::map<std::string, Magnet*> >("magnetMap")
	//	.def(boost::python::map_indexing_suite< std::map<std::string, Magnet*> >());

}
