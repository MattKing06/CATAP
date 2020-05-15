#ifndef LED_PYTHON_INTERFACE_H_
#define LED_PYTHON_INTERFACE_H_

#include <LED.h>
#include <LEDFactory.h>
#include <PythonTypeConversions.h>
#include <boost/python.hpp>


namespace BOOST_PYTHON_LED_INCLUDE
{

	void expose_led_object() 
	{
		bool is_registered = (0 != boost::python::converter::registry::query(boost::python::type_id<LED>())->to_python_target_type());
		if (is_registered) return;
		boost::python::class_<LED, boost::python::bases<Hardware>, boost::noncopyable>("LED", boost::python::no_init)
			.def("debugMessagesOn", &LED::debugMessagesOn)
			.def("debugMessagesOff", &LED::debugMessagesOff)
			.def("messagesOn", &LED::messagesOn)
			.def("messagesOff", &LED::messagesOff)
			.def("isDebugOn", &LED::isDebugOn)
			.def("isMessagingOn", &LED::isMessagingOn);
	}
	void expose_led_factory_object() 
	{
		bool is_registered = (0 != boost::python::converter::registry::query(boost::python::type_id<LEDFactory>())->to_python_target_type());
		if (is_registered) return;
		boost::python::class_<LEDFactory, boost::noncopyable>("LEDFactory", boost::python::no_init)
			.def("debugMessagesOn", &LEDFactory::debugMessagesOn)
			.def("debugMessagesOff", &LEDFactory::debugMessagesOff)
			.def("messagesOn", &LEDFactory::messagesOn)
			.def("messagesOff", &LEDFactory::messagesOff)
			.def("isDebugOn", &LEDFactory::isDebugOn)
			.def("isMessagingOn", &LEDFactory::isMessagingOn);
	}



}

#endif //LED_PYTHON_INTERFACE_H_