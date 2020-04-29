#include <GeneralMonitorPythonInterface.h>
#include <GlobalStatesPythonInterface.h>
#include <GlobalTypesPythonInterface.h>
BOOST_PYTHON_MODULE(_GeneralMonitor)
{
	boost::python::docstring_options CATAP_docstring_options;//(true, true, false);
	CATAP_docstring_options.enable_all();
	CATAP_docstring_options.disable_cpp_signatures();
	BOOST_PYTHON_STATES_INCLUDE::expose_global_state_enums();
	BOOST_PYTHON_TYPES_INCLUDE::expose_global_type_enums();
	BOOST_PYTHON_GENERAL_MONITOR_INCLUDE::expose_general_monitor_object();
	BOOST_PYTHON_GENERAL_MONITOR_INCLUDE::expose_general_monitor_factory_object();
}