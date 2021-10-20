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
		bool is_registered = (0 != boost::python::converter::registry::query(boost::python::type_id<Lighting>())->to_python_target_type());
		if (is_registered) return;
		boost::python::class_<Lighting, boost::python::bases<Hardware>, boost::noncopyable>("Lighting", boost::python::no_init)
			
			.def("allLEDOn", &Lighting::allLEDOn)
			.def("allLEDOff", &Lighting::allLEDOff)
			.def("getClaraLEDState", &Lighting::getClaraLEDState)
			.def("setClaraLEDOn", &Lighting::setClaraLEDOn)
			.def("setClaraLEDOff", &Lighting::setClaraLEDOff)
			.def("isClaraLEDOn", &Lighting::isClaraLEDOn)
			.def("isClaraLEDOff", &Lighting::isClaraLEDOff)
			.def("toggleClaraLED", &Lighting::toggleClaraLED)
			.def("getVelaLEDState", &Lighting::getVelaLEDState)
			.def("setVelaLEDOn", &Lighting::setVelaLEDOn)
			.def("setVelaLEDOff", &Lighting::setVelaLEDOff)
			.def("isVelaLEDOn", &Lighting::isVelaLEDOn)
			.def("isVelaLEDOff", &Lighting::isVelaLEDOff)
			.def("toggleVelaLED", &Lighting::toggleVelaLED)
			.def("getLEDState", &LightingFactory::getLEDState_Py)
			.def("getLightingState", &LightingFactory::getLightingState_Py)
			
			
			.def("debugMessagesOn", &Lighting::debugMessagesOn)
			.def("debugMessagesOff", &Lighting::debugMessagesOff)
			.def("messagesOn", &Lighting::messagesOn)
			.def("messagesOff", &Lighting::messagesOff)
			.def("isDebugOn", &Lighting::isDebugOn)
			.def("isMessagingOn", &Lighting::isMessagingOn);
	}
	void expose_lighting_factory_object() 
	{
		bool is_registered = (0 != boost::python::converter::registry::query(boost::python::type_id<LightingFactory>())->to_python_target_type());
		if (is_registered) return;
		boost::python::class_<LightingFactory, boost::noncopyable>("LightingFactory", boost::python::no_init)
			
			.def("getFullName", &LightingFactory::getFullName)
			.def("getLighting", &LightingFactory::getLighting, boost::python::return_value_policy<boost::python::reference_existing_object>())
			.def("allLEDOn", &LightingFactory::allLEDOn)
			.def("allLEDOff", &LightingFactory::allLEDOff)
			.def("getClaraLEDState", &LightingFactory::getClaraLEDState)
			.def("setClaraLEDOn", &LightingFactory::setClaraLEDOn)
			.def("setClaraLEDOff", &LightingFactory::setClaraLEDOff)
			.def("isClaraLEDOn", &LightingFactory::isClaraLEDOn)
			.def("isClaraLEDOff", &LightingFactory::isClaraLEDOff)
			.def("toggleClaraLED", &LightingFactory::toggleClaraLED)
			.def("getVelaLEDState", &LightingFactory::getVelaLEDState)
			.def("setVelaLEDOn", &LightingFactory::setVelaLEDOn)
			.def("setVelaLEDOff", &LightingFactory::setVelaLEDOff)
			.def("isVelaLEDOn", &LightingFactory::isVelaLEDOn)
			.def("isVelaLEDOff", &LightingFactory::isVelaLEDOff)
			.def("toggleVelaLED", &LightingFactory::toggleVelaLED)
			.def("getLEDState", &LightingFactory::getLEDState_Py)
			.def("getLightingState", &LightingFactory::getLightingState_Py)
	

			.def("debugMessagesOn", &LightingFactory::debugMessagesOn)
			.def("debugMessagesOff", &LightingFactory::debugMessagesOff)
			.def("messagesOn", &LightingFactory::messagesOn)
			.def("messagesOff", &LightingFactory::messagesOff)
			.def("isDebugOn", &LightingFactory::isDebugOn)
			.def("isMessagingOn", &LightingFactory::isMessagingOn);
	}



}

#endif //LIGHTING_PYTHON_INTERFACE_H_