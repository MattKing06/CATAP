#ifndef GENERAL_MONITOR_PYTHON_INTERFACE_H_
#define GENERAL_MONITOR_PYTHON_INTERFACE_H_

#include <GeneralMonitor.h>
#include <GeneralMonitorFactory.h>
#include <PythonTypeConversions.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

namespace BOOST_PYTHON_GENERAL_MONITOR_INCLUDE
{

	void expose_general_monitor_object() 
	{
		bool is_registered = (0 != boost::python::converter::registry::query(boost::python::type_id<GeneralMonitor>())->to_python_target_type());
		if (is_registered) return;
		boost::python::class_<GeneralMonitor, boost::python::bases<Hardware>, boost::noncopyable>("GeneralMonitor")
			.def("monitor", &GeneralMonitor::monitor_Py)
			.def("debugMessagesOn", &GeneralMonitor::debugMessagesOn)
			.def("debugMessagesOff", &GeneralMonitor::debugMessagesOff)
			.def("messagesOn", &GeneralMonitor::messagesOn)
			.def("messagesOff", &GeneralMonitor::messagesOff)
			.def("isDebugOn", &GeneralMonitor::isDebugOn)
			.def("isMessagingOn", &GeneralMonitor::isMessagingOn);
	}
	void expose_general_monitor_factory_object() 
	{
		bool is_registered = (0 != boost::python::converter::registry::query(boost::python::type_id<GeneralMonitorFactory>())->to_python_target_type());
		if (is_registered) return;
		boost::python::class_<GeneralMonitorFactory, boost::noncopyable>("GeneralMonitorFactory", boost::python::no_init)
			.def("debugMessagesOn", &GeneralMonitorFactory::debugMessagesOn)
			.def("debugMessagesOff", &GeneralMonitorFactory::debugMessagesOff)
			.def("messagesOn", &GeneralMonitorFactory::messagesOn)
			.def("messagesOff", &GeneralMonitorFactory::messagesOff)
			.def("isDebugOn", &GeneralMonitorFactory::isDebugOn)
			.def("isMessagingOn", &GeneralMonitorFactory::isMessagingOn);
	}



}

#endif //GENERAL_MONITOR_PYTHON_INTERFACE_H_