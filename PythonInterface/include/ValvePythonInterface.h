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
		bool is_registered = (0 != boost::python::converter::registry::query(boost::python::type_id<Valve>())->to_python_target_type());
		if (is_registered) return;
		boost::python::class_<Valve, boost::python::bases<Hardware>, boost::noncopyable>("Valve", boost::python::no_init)
			.add_property("state", &Valve::getValveState, &Valve::setValveState)
			.add_property("name", &Valve::getHardwareName)
			.def("attachContext", &Valve::attachToInitialContext)
			.def("open", &Valve::open, boost::python::arg("self"))
			.def("close", &Valve::close, boost::python::arg("self"))
			.def("isOpen", &Valve::isOpen, boost::python::arg("self"))
			.def("isClosed", &Valve::isClosed, boost::python::arg("self"))
			.def("isMoving", &Valve::isMoving, boost::python::arg("self"))
			.def("debugMessagesOn", &Valve::debugMessagesOn, boost::python::arg("self"))
			.def("debugMessagesOff", &Valve::debugMessagesOff, boost::python::arg("self"))
			.def("messagesOn", &Valve::messagesOn, boost::python::arg("self"))
			.def("messagesOff", &Valve::messagesOff, boost::python::arg("self"))
			;
	}

	void exposeValveFactoryObject()
	{
		bool is_registered = (0 != boost::python::converter::registry::query(boost::python::type_id<ValveFactory>())->to_python_target_type());
		if (is_registered) return;
		void(ValveFactory:: * openSingle)(const std::string&) = &ValveFactory::open;
		void(ValveFactory:: * closeSingle)(const std::string&) = &ValveFactory::close;
		bool(ValveFactory:: * loadSnapshotFile)(const std::string&) = &ValveFactory::loadSnapshot;
		void(ValveFactory:: * attachContext_single)(const std::string&) = &ValveFactory::attachContext;
		void(ValveFactory:: * attachContext_all)(void) = &ValveFactory::attachContext;
		boost::python::class_<ValveFactory, boost::noncopyable>("ValveFactory", boost::python::no_init)
			.def(boost::python::init<STATE>(boost::python::arg("mode")))
			.def(boost::python::init<STATE, const std::string>())
			.def("setup", &ValveFactory::setup,(boost::python::arg("self"), boost::python::arg("version")))
			.def("attachContext", &ValveFactory::attachContext_Py)
			.def("attachContext", attachContext_single)
			.def("attachContext", attachContext_all)
			.def("getValve", &ValveFactory::getValve,(boost::python::arg("self"), boost::python::arg("name")), boost::python::return_value_policy<boost::python::reference_existing_object>())
			.def("getAllValveNames", &ValveFactory::getAllValveNames_Py,(boost::python::arg("self")))
			.def("getValveState", &ValveFactory::getValveState, (boost::python::arg("self"), boost::python::arg("name")))
			.def("getValveStates", &ValveFactory::getValveStates_Py, (boost::python::arg("self"), boost::python::arg("names")))
			.def("getAllValveStates", &ValveFactory::getAllValveStates_Py, (boost::python::arg("self")))
			.def("open", openSingle, (boost::python::arg("self"), boost::python::arg("name")))
			.def("open", &ValveFactory::open_Py, (boost::python::arg("self"), boost::python::arg("names")))
			.def("openAllValves", &ValveFactory::openAllValves, (boost::python::arg("self")))
			.def("isOpen", &ValveFactory::isOpen, (boost::python::arg("self"), boost::python::arg("name")))
			.def("close", closeSingle, (boost::python::arg("self"), boost::python::arg("name")))
			.def("close", &ValveFactory::close_Py, (boost::python::arg("self"), boost::python::arg("names")))
			.def("closeAllValves", &ValveFactory::closeAllValves, (boost::python::arg("self")))
			.def("isClosed", &ValveFactory::isClosed, (boost::python::arg("self"), boost::python::arg("name")))
			.def("isMoving", &ValveFactory::isMoving, boost::python::args("self", "name"))
			.def("debugMessagesOn", &ValveFactory::debugMessagesOn, (boost::python::arg("self")))
			.def("debugMessagesOff", &ValveFactory::debugMessagesOff, (boost::python::arg("self")))
			.def("messagesOn", &ValveFactory::messagesOn, (boost::python::arg("self")))
			.def("messagesOff", &ValveFactory::messagesOff, (boost::python::arg("self")))
			.def("getSnapshot", &ValveFactory::getSnapshot_Py, (boost::python::args("self")))
			.def("saveSnapshot", &ValveFactory::exportSnapshotToYAML, (boost::python::args("self"), boost::python::args("location"), boost::python::args("filename")))
			.def("loadSnapshot", loadSnapshotFile)
			.def("loadSnapshot", &ValveFactory::loadSnapshot_Py)
			;
	}

}



#endif //VALVE_PYTHON_INTERFACE_H_