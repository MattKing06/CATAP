#ifndef HARDWARE_PYTHON_INTERFACE_H_
#define HARDWARE_PYTHON_INTERFACE_H_

#include <Hardware.h>
#include <boost/python.hpp>

namespace BOOST_PYTHON_HARDWARE_INCLUDE
{
	void expose_hardware_object()
	{
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
	}
}

#endif //HARDWARE_PYTHON_INTERFACE_H_