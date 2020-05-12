#ifndef RF_MODULATOR_PYTHON_INTERFACE_H_
#define RF_MODULATOR_PYTHON_INTERFACE_H_

#include <RFModulator.h>
#include <RFModulatorFactory.h>
#include <PythonTypeConversions.h>
#include <boost/python.hpp>


namespace BOOST_PYTHON_RF_MODULATOR_INCLUDE
{

	void expose_rf_modulator_object() 
	{
		bool is_registered = (0 != boost::python::converter::registry::query(boost::python::type_id<RFModulator>())->to_python_target_type());
		if (is_registered) return;
		boost::python::class_<RFModulator, boost::python::bases<Hardware>, boost::noncopyable>("RFModulator", boost::python::no_init)
			.def("debugMessagesOn", &RFModulator::debugMessagesOn)
			.def("debugMessagesOff", &RFModulator::debugMessagesOff)
			.def("messagesOn", &RFModulator::messagesOn)
			.def("messagesOff", &RFModulator::messagesOff)
			.def("isDebugOn", &RFModulator::isDebugOn)
			.def("isMessagingOn", &RFModulator::isMessagingOn);
	}
	void expose_rf_modulator_factory_object() 
	{
		bool is_registered = (0 != boost::python::converter::registry::query(boost::python::type_id<RFModulatorFactory>())->to_python_target_type());
		if (is_registered) return;
		boost::python::class_<RFModulatorFactory, boost::noncopyable>("RFModulatorFactory", boost::python::no_init)
			.def("debugMessagesOn", &RFModulatorFactory::debugMessagesOn)
			.def("debugMessagesOff", &RFModulatorFactory::debugMessagesOff)
			.def("messagesOn", &RFModulatorFactory::messagesOn)
			.def("messagesOff", &RFModulatorFactory::messagesOff)
			.def("isDebugOn", &RFModulatorFactory::isDebugOn)
			.def("isMessagingOn", &RFModulatorFactory::isMessagingOn);
	}



}

#endif //RF_MODULATOR_PYTHON_INTERFACE_H_