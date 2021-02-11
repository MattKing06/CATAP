#ifndef HARDWARE_FACTORY_PYTHON_INTERFACE_H_
#define HARDWARE_FACTORY_PYTHON_INTERFACE_H_

#include <HardwareFactory.h>
#include <LLRFFactory.h>
#include <boost/python.hpp>
//#include <GlobalTypeEnum.h>

namespace BOOST_PYTHON_HARDWARE_FACTORY_INCLUDE
{
	void exposeHardwareFactoryObject()
	{



		bool is_registered = (0 != boost::python::converter::registry::query(boost::python::type_id<HardwareFactory>())->to_python_target_type());
		if (is_registered) return;

		//STATE(MagnetFactory:: * switchOffSingle)(const std::string&) = &MagnetFactory::switchOff;
		//&LLRFFactory( HardwareFactory:: * getLLRFFactory_oneArea)(const TYPE)= &HardwareFactory::getLLRFFactory;
		//&LLRFFactory(HardwareFactory::*getLLRFFactory_manyAreas)(const boost::python::list&)= &HardwareFactory::getLLRFFactory;

		// yay function pointers for ambiguous overloads
		MagnetFactory&(HardwareFactory::*getMagnetFactory_noArea)()= &HardwareFactory::getMagnetFactory;
		MagnetFactory&(HardwareFactory::*getMagnetFactory_oneArea)(const TYPE)= &HardwareFactory::getMagnetFactory;
		MagnetFactory&(HardwareFactory::*getMagnetFactory_listOfAreas)(const boost::python::list&)= &HardwareFactory::getMagnetFactory;


		RFModulatorFactory&(HardwareFactory::*getRFModulatorFactory_noarg)()= &HardwareFactory::getRFModulatorFactory;
		RFModulatorFactory&(HardwareFactory::*getRFModulatorFactory_listOfAreas)(const std::vector<TYPE>&)= &HardwareFactory::getRFModulatorFactory;




		// Hardware Factory Exposure
		boost::python::class_<HardwareFactory>("HardwareFactory", "The holder of all hardware", boost::python::init<STATE>((boost::python::args("self"), boost::python::args("mode"))))
			.def("setup", &HardwareFactory::setup, (boost::python::args("self"), boost::python::arg("hardwareType"), boost::python::args("version")))
			.add_property("llrfFactory", &HardwareFactory::llrffactory)
			.def("getLLRFFactory", &HardwareFactory::getLLRFFactory_Single, boost::python::arg("self"), boost::python::arg("machine_area"), boost::python::return_value_policy<boost::python::reference_existing_object>())
			.def("getLLRFFactory", &HardwareFactory::getLLRFFactory_Py, boost::python::arg("self"), boost::python::arg("machine_areas"), boost::python::return_value_policy<boost::python::reference_existing_object>())

			.add_property("cameraFactory", &HardwareFactory::cameraFactory)
			.def("getCameraFactory", &HardwareFactory::getCameraFactory, boost::python::arg("self"), boost::python::return_value_policy<boost::python::reference_existing_object>())
			.def("getCameraFactory", &HardwareFactory::getCameraFactory_Single, boost::python::arg("self"), boost::python::arg("machine_area"), boost::python::return_value_policy<boost::python::reference_existing_object>())
			.def("getCameraFactory", &HardwareFactory::getCameraFactory_Mulitple_Py, boost::python::arg("self"), boost::python::arg("machine_areas"),boost::python::return_value_policy<boost::python::reference_existing_object>())

			.def("getCameraFactory", &HardwareFactory::getCameraFactory_ByName, boost::python::arg("self"), boost::python::arg("name"), boost::python::return_value_policy<boost::python::reference_existing_object>())
			.def("getCameraFactory", &HardwareFactory::getCameraFactory_ByNames_Py, boost::python::arg("self"), boost::python::arg("names"), boost::python::return_value_policy<boost::python::reference_existing_object>())

			.add_property("magnetFactory", &HardwareFactory::magnetFactory)


			//.def("getMagnetFactory", &HardwareFactory::getMagnetFactory, boost::python::arg("self"), boost::python::return_value_policy<boost::python::reference_existing_object>())

			.def("getMagnetFactory", getMagnetFactory_noArea, boost::python::return_value_policy<boost::python::reference_existing_object>())
			.def("getMagnetFactory", getMagnetFactory_oneArea, boost::python::return_value_policy<boost::python::reference_existing_object>())
			.def("getMagnetFactory", getMagnetFactory_listOfAreas, boost::python::return_value_policy<boost::python::reference_existing_object>())


			.add_property("bpmFactory", &HardwareFactory::bpmFactory)
			.def("getBPMFactory", &HardwareFactory::getBPMFactory, boost::python::arg("self"), boost::python::return_value_policy<boost::python::reference_existing_object>())
			.add_property("chargeFactory", &HardwareFactory::chargeFactory)
			.def("getChargeFactory", &HardwareFactory::getChargeFactory, boost::python::arg("self"), boost::python::return_value_policy<boost::python::reference_existing_object>())
			.add_property("screenFactory", &HardwareFactory::screenFactory)
			.def("getScreenFactory", &HardwareFactory::getScreenFactory, boost::python::arg("self"), boost::python::return_value_policy<boost::python::reference_existing_object>())
			.add_property("valveFactory", &HardwareFactory::valveFactory)
			.def("getValveFactory", &HardwareFactory::getValveFactory, boost::python::arg("self"), boost::python::return_value_policy<boost::python::reference_existing_object>())
			.add_property("IMGFactory", &HardwareFactory::imgFactory)
			.def("getIMGFactory", &HardwareFactory::getIMGFactory, boost::python::arg("self"), boost::python::return_value_policy<boost::python::reference_existing_object>())
			.add_property("laserEnergyMeterFactory", &HardwareFactory::laserEnergyMeterFactory)
			.def("getLaserEnergyMeterFactory", &HardwareFactory::getLaserEnergyMeterFactory, boost::python::arg("self"), boost::python::return_value_policy<boost::python::reference_existing_object>())
			.add_property("laserHWPFactory", &HardwareFactory::laserHWPFactory)
			.def("getLaserHWPFactory", &HardwareFactory::getLaserHWPFactory, boost::python::arg("self"), boost::python::return_value_policy<boost::python::reference_existing_object>())
			.add_property("hardwareMap", &HardwareFactory::hardwareMap)

			.def("getShutterFactory", &HardwareFactory::getShutterFactory, boost::python::arg("self"), boost::python::return_value_policy<boost::python::reference_existing_object>())
			.add_property("shutterFactory", &HardwareFactory::shutterFactory)


			.def("getRFModulatorFactory", &HardwareFactory::getRFModulatorFactory_Single, boost::python::arg("self"), boost::python::return_value_policy<boost::python::reference_existing_object>())
			.def("getRFModulatorFactory", &HardwareFactory::getRFModulatorFactory_Py, boost::python::arg("self"), boost::python::return_value_policy<boost::python::reference_existing_object>())
			.def("getRFModulatorFactory", getRFModulatorFactory_noarg, boost::python::arg("self"), boost::python::return_value_policy<boost::python::reference_existing_object>())
			.def("getRFModulatorFactory", getRFModulatorFactory_listOfAreas, boost::python::arg("self"), boost::python::return_value_policy<boost::python::reference_existing_object>())

			
			



			.def("getRFProtectionFactory", &HardwareFactory::getRFProtectionFactory, boost::python::arg("self"), boost::python::return_value_policy<boost::python::reference_existing_object>())
			.add_property("shutterFactory", &HardwareFactory::rfProtectionFactory)


			.def("debugMessagesOn", &HardwareFactory::debugMessagesOn, boost::python::arg("self"))
			.def("debugMessagesOff", &HardwareFactory::debugMessagesOff, boost::python::arg("self"))
			.def("messagesOn", &HardwareFactory::messagesOn, boost::python::arg("self"))
			.def("messagesOff", &HardwareFactory::messagesOff, boost::python::arg("self"));

	}
}



#endif //HARDWARE_FACTORY_PYTHON_INTERFACE_H_