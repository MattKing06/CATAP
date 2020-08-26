#ifndef EPICS_TOOLS_PYTHON_INTERFACE_H_
#define EPICS_TOOLS_PYTHON_INTERFACE_H_

#include <EPICSTools.h>
#include <PythonTypeConversions.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

namespace BOOST_PYTHON_EPICS_TOOLS_INCLUDE
{
	void expose_listener_object()
	{
		//need to define individual methods for getValue types..
		bool is_registered = (0 != boost::python::converter::registry::query(boost::python::type_id<Listener>())->to_python_target_type());
		if (is_registered) return;
		boost::python::class_<Listener, boost::noncopyable>("Monitor", boost::python::no_init)
			.add_property("PV", &Listener::pvToMonitor)
			.def("getValue", &Listener::getValue_Py)
			.def("getArray", &Listener::getArray_Py)
			.def("setBufferSize", &Listener::setBufferSize)
			.def("setArrayBufferSize", &Listener::setArrayBufferSize)
			.def("getBuffer", &Listener::getBuffer_Py)
			.def("getBufferAverage", &Listener::getBufferAverage)
			.def("getArrayBuffer", &Listener::getArrayBuffer_Py);

	}
	void expose_epics_tools_object() 
	{
		bool is_registered = (0 != boost::python::converter::registry::query(boost::python::type_id <EPICSTools>())->to_python_target_type());
		if (is_registered) return;
		void(EPICSTools:: * monitor_single)(const std::string&) = &EPICSTools::monitor;
		boost::python::object(EPICSTools:: * get_single)(const std::string&) = &EPICSTools::get_Py;
		boost::python::dict(EPICSTools:: * get_multiple)(boost::python::list) = &EPICSTools::get_Py;
		void(EPICSTools:: * put_single)(const std::string&, boost::python::object) = &EPICSTools::put_Py;
		void(EPICSTools:: * put_multiple)(boost::python::dict) = &EPICSTools::put_Py;
		boost::python::list(EPICSTools:: * getBuffer_single)(const std::string&) = &EPICSTools::getBuffer_Py;
		boost::python::dict(EPICSTools:: * getBuffer_multiple)(boost::python::list) = &EPICSTools::getBuffer_Py;
		double(EPICSTools:: * getBufferAverage_single)(const std::string&) = &EPICSTools::getBufferAverage_Py;
		boost::python::dict(EPICSTools:: * getBufferAverage_multiple)(boost::python::list) = &EPICSTools::getBufferAverage_Py;
		double(EPICSTools:: * getBufferStdDeviation_single)(const std::string&) = &EPICSTools::getBufferStdDeviation;
		boost::python::dict(EPICSTools:: * getBufferStdDeviation_multiple)(boost::python::list) = &EPICSTools::getBufferStdDeviation_Py;
		boost::python::class_<EPICSTools, boost::noncopyable>("EPICSTools", boost::python::no_init)
			.def(boost::python::init<>())
			.def(boost::python::init<STATE>())
			.def("monitor", &EPICSTools::monitor_Py)
			.def("monitor", monitor_single)
			.def("stopMonitoring", &EPICSTools::stopMonitoring)
			.def("restartMonitoring", &EPICSTools::restartMonitoring)
			.def("getMonitor", &EPICSTools::getMonitor, boost::python::return_value_policy<boost::python::reference_existing_object>())
			.def("getAllMonitorNames", &EPICSTools::getAllMonitorNames_Py)
			.def("get", get_single)
			.def("get", get_multiple)
			.def("getArray", &EPICSTools::getArray_Py)
			.def("put", put_single)
			.def("put", put_multiple)
			.def("putArray", &EPICSTools::putArray_Py)
			.def("getBuffer", getBuffer_single)
			.def("getBuffer", getBuffer_multiple)
			.def("getArrayBuffer", &EPICSTools::getArrayBuffer_Py)
			.def("getBufferAverage", getBufferAverage_single)
			.def("getBufferAverage", getBufferAverage_multiple)
			.def("getBufferStdDev", getBufferStdDeviation_single)
			.def("getBufferStdDev", getBufferStdDeviation_multiple)
			.def("getCount", &EPICSTools::getCount)
			.def("getType", &EPICSTools::getType)
			.def("getEPICSInfo", &EPICSTools::getEPICSInfo)
			;
	}



}

#endif //EPICS_TOOLS_PYTHON_INTERFACE_H_