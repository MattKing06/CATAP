#ifndef HARDWARE_PYTHON_INTERFACE_H_
#define HARDWARE_PYTHON_INTERFACE_H_

#include <Hardware.h>
#include <boost/python.hpp>
#include <boost/python/converter/registry.hpp>
namespace BOOST_PYTHON_HARDWARE_INCLUDE
{
	void expose_hardware_object()
	{
		bool is_registered = (0 != boost::python::converter::registry::query(boost::python::type_id<Hardware>())->to_python_target_type());
		if (is_registered) return;
		boost::python::class_<Hardware>("Hardware", boost::python::no_init)
			.def_readonly("machineArea", &Hardware::machine_area)
			.def_readonly("hardwareType", &Hardware::hardware_type)
			.def_readonly("specificHardwareParameters", &Hardware::specificHardwareParameters)
			.def("getMachineArea", &Hardware::getMachineArea)
			.def("getHardwareType", &Hardware::getHardwareType)
			.def("getHardwareName", &Hardware::getHardwareName)
			.def("getMachineAreaStr", &Hardware::getMachineAreaStr)
			.def("getHardwareTypeStr", &Hardware::getHardwareTypeStr)
			.def("getMode", &Hardware::getMode)
			.def("isVirtual", &Hardware::isVirtual)
			.def("isPhysical", &Hardware::isPhysical)
			.def("isOffline", &Hardware::isOffline)
			.def("debugMessagesOn", &Hardware::debugMessagesOn)
			.def("debugMessagesOff", &Hardware::debugMessagesOff)
			.def("messagesOn", &Hardware::messagesOn)
			.def("messagesOff", &Hardware::messagesOff)
			.def("getSpecificHardwareParameters", &Hardware::getSpecificHardwareParameters);
	}
}

#endif //HARDWARE_PYTHON_INTERFACE_H_