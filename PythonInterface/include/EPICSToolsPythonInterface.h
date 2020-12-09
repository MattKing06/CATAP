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
			.def("getValue", &Listener::getValue_Py, (boost::python::arg("self")), "get current value of the monitor")
			.def("getArray", &Listener::getArray_Py, boost::python::arg("self"), "get current array of the monitor")
			.def("setBufferSize", &Listener::setBufferSize, boost::python::arg("self"), boost::python::arg("size"), "set the size of the buffer (default size is 10)")
			.def("setArrayBufferSize", &Listener::setArrayBufferSize, boost::python::arg("self"), boost::python::arg("size"), "set the size of the array buffer (default size is 10)")
			.def("getBuffer", &Listener::getBuffer_Py, boost::python::arg("self"), "get the buffer of past values from EPICS")
			.def("getBufferAverage", &Listener::getBufferAverage, boost::python::arg("self"), "get the mean of the values in buffer")
			.def("getBufferStdDev", &Listener::getBufferStdDeviation, boost::python::args("self"), "get the standard deviation of the values in buffer")
			.def("getArrayBuffer", &Listener::getArrayBuffer_Py, boost::python::arg("self"), "get the buffer of past arrays from EPICS")
			.def("getArrayBufferAverage", &Listener::getArrayBufferAverageArray_Py, boost::python::arg("self"), "get the mean array of arrays in buffer");

	}
	void expose_epics_tools_object() 
	{
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
		bool is_registered = (0 != boost::python::converter::registry::query(boost::python::type_id <EPICSTools>())->to_python_target_type());
		if (is_registered) return;
		boost::python::class_<EPICSTools, boost::noncopyable>("EPICSTools", boost::python::no_init)
			.def(boost::python::init<>(boost::python::arg("self"),"self"))
			.def(boost::python::init<STATE>(boost::python::arg("self")))
			.def("monitor", &EPICSTools::monitor_Py, "begin monitoring")
			.def("monitor", monitor_single)
			.def("stopMonitoring", &EPICSTools::stopMonitoring)
			.def("restartMonitoring", &EPICSTools::restartMonitoring)
			.def("getMonitor", &EPICSTools::getMonitor, boost::python::return_value_policy<boost::python::reference_existing_object>())
			.def("getAllMonitorNames", &EPICSTools::getAllMonitorNames_Py)
			.def("get", get_single)
			.def("get", get_multiple)
			.def("getArray", &EPICSTools::getArray_Py)
			.def("getArray", &EPICSTools::getArray_Py2)
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