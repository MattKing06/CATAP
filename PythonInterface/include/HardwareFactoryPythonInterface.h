#ifndef HARDWARE_FACTORY_PYTHON_INTERFACE_H_
#define HARDWARE_FACTORY_PYTHON_INTERFACE_H_

#include <HardwareFactory.h>
#include <boost/python.hpp>

namespace BOOST_PYTHON_HARDWARE_FACTORY_INCLUDE
{
	void exposeHardwareFactoryObject()
	{
		// Hardware Factory Exposure
		boost::python::class_<HardwareFactory>("HardwareFactory", "The holder of all hardware", boost::python::init<STATE>((boost::python::args("self"), boost::python::args("mode"))))
			.def("setup", &HardwareFactory::setup, (boost::python::args("self"), boost::python::arg("hardwareType"), boost::python::args("version")))
			.add_property("magnetFactory", &HardwareFactory::magnetFactory)
			.def("getMagnetFactory", &HardwareFactory::getMagnetFactory, boost::python::arg("self"), boost::python::return_value_policy<boost::python::reference_existing_object>())
			.add_property("bpmFactory", &HardwareFactory::bpmFactory)
			.def("getBPMFactory", &HardwareFactory::getBPMFactory, boost::python::arg("self"), boost::python::return_value_policy<boost::python::reference_existing_object>())
			.add_property("chargeFactory", &HardwareFactory::chargeFactory)
			.def("getChargeFactory", &HardwareFactory::getChargeFactory, boost::python::arg("self"), boost::python::return_value_policy<boost::python::reference_existing_object>())
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