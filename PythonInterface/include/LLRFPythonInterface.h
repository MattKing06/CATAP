#ifndef LLLRF_PYTHON_INTERFACE_H_
#define LLLRF_PYTHON_INTERFACE_H_

#include <LLRF.h>
#include <LLRFFactory.h>
#include <PythonTypeConversions.h>
#include <boost/python.hpp>


namespace BOOST_PYTHON_LED_INCLUDE
{

	void expose_llrf_object()
	{
		bool is_registered = (0 != boost::python::converter::registry::query(boost::python::type_id<LED>())->to_python_target_type());
		if (is_registered) return;
		boost::python::class_<LLRF, boost::python::bases<Hardware>, boost::noncopyable>("LLRF", boost::python::no_init)
			.def("debugMessagesOn", &LLRF::debugMessagesOn)
			.def("debugMessagesOff", &LLRF::debugMessagesOff)
			.def("messagesOn", &LLRF::messagesOn)
			.def("messagesOff", &LLRF::messagesOff)
			.def("isDebugOn", &LLRF::isDebugOn)
			.def("isMessagingOn", &LLRF::isMessagingOn);
	}
	void expose_llrf_factory_object()
	{



		bool is_registered = (0 != boost::python::converter::registry::query(boost::python::type_id<LEDFactory>())->to_python_target_type());
		if (is_registered) return;
		boost::python::class_<LLRFFactory, boost::noncopyable>("LLRFFactory", boost::python::no_init)
			.def("debugMessagesOn", &LLRFFactory::debugMessagesOn)
			.def("debugMessagesOn", &LLRFFactory::debugMessagesOn)
			.def("debugMessagesOff", &LLRFFactory::debugMessagesOff)
			.def("messagesOn", &LLRFFactory::messagesOn)
			.def("messagesOff", &LLRFFactory::messagesOff)
			.def("isDebugOn", &LLRFFactory::isDebugOn)
			.def("isMessagingOn", &LLRFFactory::isMessagingOn);
	}

}

#endif //LED_PYTHON_INTERFACE_H_