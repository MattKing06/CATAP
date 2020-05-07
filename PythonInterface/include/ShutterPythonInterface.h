#ifndef SHUTTER_PYTHON_INTERFACE_H_
#define SHUTTER_PYTHON_INTERFACE_H_

#include <Shutter.h>
#include <ShutterFactory.h>
#include <PythonTypeConversions.h>
#include <boost/python.hpp>


namespace BOOST_PYTHON_SHUTTER_INCLUDE
{

	void expose_shutter_object() 
	{
		bool is_registered = (0 != boost::python::converter::registry::query(boost::python::type_id<Shutter>())->to_python_target_type());
		if (is_registered) return;
		boost::python::class_<Shutter, boost::python::bases<Hardware>, boost::noncopyable>("Shutter", boost::python::no_init)
			.def("debugMessagesOn", &Shutter::debugMessagesOn)
			.def("debugMessagesOff", &Shutter::debugMessagesOff)
			.def("messagesOn", &Shutter::messagesOn)
			.def("messagesOff", &Shutter::messagesOff)
			.def("isDebugOn", &Shutter::isDebugOn)
			.def("isMessagingOn", &Shutter::isMessagingOn);
	}
	void expose_shutter_factory_object()
	{
		bool is_registered = (0 != boost::python::converter::registry::query(boost::python::type_id<ShutterFactory>())->to_python_target_type());
		if (is_registered) return;
		boost::python::class_<ShutterFactory, boost::noncopyable>("ShutterFactory", boost::python::no_init)
			.def("debugMessagesOn", &ShutterFactory::debugMessagesOn)
			.def("debugMessagesOff", &ShutterFactory::debugMessagesOff)
			.def("messagesOn", &ShutterFactory::messagesOn)
			.def("messagesOff", &ShutterFactory::messagesOff)
			.def("isDebugOn", &ShutterFactory::isDebugOn)
			.def("isMessagingOn", &ShutterFactory::isMessagingOn);
	}



}

#endif //SHUTTER_PYTHON_INTERFACE_H_