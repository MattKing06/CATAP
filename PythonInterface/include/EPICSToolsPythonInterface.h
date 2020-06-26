#ifndef EPICS_TOOLS_PYTHON_INTERFACE_H_
#define EPICS_TOOLS_PYTHON_INTERFACE_H_

#include <EPICSTools.h>
#include <EPICSToolsFactory.h>
#include <PythonTypeConversions.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

namespace BOOST_PYTHON_EPICS_TOOLS_INCLUDE
{

	void expose_epics_tools_object() 
	{
		bool is_registered = (0 != boost::python::converter::registry::query(boost::python::type_id<EPICSTools>())->to_python_target_type());
		if (is_registered) return;
		boost::python::class_<EPICSTools, boost::python::bases<Hardware>, boost::noncopyable>("EPICSTools")
			.def("debugMessagesOn", &EPICSTools::debugMessagesOn)
			.def("debugMessagesOff", &EPICSTools::debugMessagesOff)
			.def("messagesOn", &EPICSTools::messagesOn)
			.def("messagesOff", &EPICSTools::messagesOff)
			.def("isDebugOn", &EPICSTools::isDebugOn)
			.def("isMessagingOn", &EPICSTools::isMessagingOn);
	}
	void expose_epics_tools_factory_object() 
	{
		bool is_registered = (0 != boost::python::converter::registry::query(boost::python::type_id<EPICSToolsFactory>())->to_python_target_type());
		if (is_registered) return;
		boost::python::class_<EPICSToolsFactory, boost::noncopyable>("EPICSToolsFactory", boost::python::init<STATE>((boost::python::args("self"), boost::python::args("mode"))))
			.def("monitor", &EPICSToolsFactory::monitor_Py)
			.def("debugMessagesOn", &EPICSToolsFactory::debugMessagesOn)
			.def("debugMessagesOff", &EPICSToolsFactory::debugMessagesOff)
			.def("messagesOn", &EPICSToolsFactory::messagesOn)
			.def("messagesOff", &EPICSToolsFactory::messagesOff)
			.def("isDebugOn", &EPICSToolsFactory::isDebugOn)
			.def("isMessagingOn", &EPICSToolsFactory::isMessagingOn);
	}



}

#endif //EPICS_TOOLS_PYTHON_INTERFACE_H_