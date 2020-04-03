#ifndef LIGHTING_PYTHON_INTERFACE_H_
#define LIGHTING_PYTHON_INTERFACE_H_

#include <Lighting.h>
#include <LightingFactory.h>
#include <PythonTypeConversions.h>
#include <boost/python.hpp>


namespace BOOST_PYTHON_LIGHTING_INCLUDE
{

	void expose_lighting_object() 
	{
		boost::python::class_<Lighting, boost::python::bases<Hardware>, boost::noncopyable>("Lighting", boost::python::no_init)
			.def("debugMessagesOn", &Lighting::debugMessagesOn)
			.def("debugMessagesOff", &Lighting::debugMessagesOff)
			.def("messagesOn", &Lighting::messagesOn)
			.def("messagesOff", &Lighting::messagesOff)
			.def("isDebugOn", &Lighting::isDebugOn)
			.def("isMessagingOn", &Lighting::isMessagingOn);
	}
	void expose_lighting_factory_object() 
	{
		boost::python::class_<LightingFactory, boost::noncopyable>("LightingFactory", boost::python::no_init)
			.def("debugMessagesOn", &LightingFactory::debugMessagesOn)
			.def("debugMessagesOff", &LightingFactory::debugMessagesOff)
			.def("messagesOn", &LightingFactory::messagesOn)
			.def("messagesOff", &LightingFactory::messagesOff)
			.def("isDebugOn", &LightingFactory::isDebugOn)
			.def("isMessagingOn", &LightingFactory::isMessagingOn);
	}



}

#endif //LIGHTING_PYTHON_INTERFACE_H_