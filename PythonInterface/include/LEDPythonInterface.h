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