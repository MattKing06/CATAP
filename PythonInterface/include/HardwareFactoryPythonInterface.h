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


		// Hardware Factory Exposure
		boost::python::class_<HardwareFactory>("HardwareFactory", "The holder of all hardware", boost::python::init<STATE>((boost::python::args("self"), boost::python::args("mode"))))
			.def("setup", &HardwareFactory::setup, (boost::python::args("self"), boost::python::arg("hardwareType"), boost::python::args("version")))
			//.def("getLLRFFactory", getLLRFFactory_oneArea, boost::python::arg("self"), boost::python::arg("machine_area"), boost::python::return_value_policy<boost::python::reference_existing_object>())
			
			.def("getLLRFFactory", &HardwareFactory::getLLRFFactory_Single, boost::python::arg("self"), boost::python::arg("machine_area"), boost::python::return_value_policy<boost::python::reference_existing_object>())

			.def("getLLRFFactory", &HardwareFactory::getLLRFFactory_Py, boost::python::arg("self"), boost::python::arg("machine_areas"), boost::python::return_value_policy<boost::python::reference_existing_object>())
			.add_property("magnetFactory", &HardwareFactory::magnetFactory)
			.def("getMagnetFactory", &HardwareFactory::getMagnetFactory, boost::python::arg("self"), boost::python::return_value_policy<boost::python::reference_existing_object>())
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

	}
}



#endif //HARDWARE_FACTORY_PYTHON_INTERFACE_H_