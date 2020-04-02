#include <GeneralMonitorPythonInterface.h>

BOOST_PYTHON_MODULE(_GeneralMonitor)
{
	BOOST_PYTHON_GENERAL_MONITOR_INCLUDE::expose_general_monitor_object();
	BOOST_PYTHON_GENERAL_MONITOR_INCLUDE::expose_general_monitor_factory_object();
}