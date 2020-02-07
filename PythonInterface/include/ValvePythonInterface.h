#ifndef VALVE_PYTHON_INTERFACE_H_
#define VALVE_PYTHON_INTERFACE_H_

#include <Valve.h>
#include <ValveFactory.h>
#include <PythonTypeConversions.h>
#include <boost/python.hpp>


namespace BOOST_PYTHON_VALVE_INCLUDE
{

	void exposeValveObject()
	{
		boost::python::class_<Valve, boost::python::bases<Hardware>, boost::noncopyable>("Valve", boost::python::no_init)
			.add_property("state", &Valve::getValveState, &Valve::setValveState)
			.add_property("name", &Valve::getHardwareName)
			.def("open", &Valve::open)
			.def("close", &Valve::close)
			.def("isOpen", &Valve::isOpen)
			.def("isClosed", &Valve::isClosed)
			.def("debugMessagesOn", &Valve::debugMessagesOn)
			.def("debugMessagesOff", &Valve::debugMessagesOff)
			.def("messagesOn", &Valve::messagesOn)
			.def("messagesOff", &Valve::messagesOff)
			;
	}

	void exposeValveFactoryObject()
	{
		void(ValveFactory:: * openSingle)(const std::string&) = &ValveFactory::open;
		void(ValveFactory:: * closeSingle)(const std::string&) = &ValveFactory::close;
		boost::python::class_<ValveFactory, boost::noncopyable>("ValveFactory", boost::python::no_init)
			.def(boost::python::init<STATE>())
			.def("setup", &ValveFactory::setup)
			.def("getValve", &ValveFactory::getValve, boost::python::return_value_policy<boost::python::reference_existing_object>())
			.def("getAllValveNames", &ValveFactory::getAllValveNames_Py)
			.def("getValveState", &ValveFactory::getValveState)
			.def("getValveStates", &ValveFactory::getValveStates_Py)
			.def("getAllValveStates", &ValveFactory::getAllValveStates_Py)
			.def("open", openSingle)
			.def("open", &ValveFactory::open_Py)
			.def("openAllValves", &ValveFactory::openAllValves)
			.def("isOpen", &ValveFactory::isOpen)
			.def("close", closeSingle)
			.def("close", &ValveFactory::close_Py)
			.def("closeAllValves", &ValveFactory::closeAllValves)
			.def("isClosed", &ValveFactory::isClosed)
			.def("debugMessagesOn", &ValveFactory::debugMessagesOn)
			.def("debugMessagesOff", &ValveFactory::debugMessagesOff)
			.def("messagesOn", &ValveFactory::messagesOn)
			.def("messagesOff", &ValveFactory::messagesOff)
			;
	}

}



#endif //VALVE_PYTHON_INTERFACE_H_