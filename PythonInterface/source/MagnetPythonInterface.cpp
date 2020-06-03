#include <MagnetPythonInterface.h>
#include <HardwarePythonInterface.h>
#include <GlobalStatesPythonInterface.h>
#include <GlobalTypesPythonInterface.h>

BOOST_PYTHON_MODULE(_Magnet)
{
	boost::python::docstring_options CATAP_docstring_options;//(true, true, false);
	CATAP_docstring_options.enable_all();
	CATAP_docstring_options.disable_cpp_signatures();
	BOOST_PYTHON_TYPES_INCLUDE::expose_global_type_enums();
	BOOST_PYTHON_STATES_INCLUDE::expose_global_state_enums();
	BOOST_PYTHON_HARDWARE_INCLUDE::expose_hardware_object();
	BOOST_PYTHON_MAGNET_INCLUDE::expose_magnet_object();
	BOOST_PYTHON_MAGNET_INCLUDE::expose_magnet_factory_object();
	BOOST_PYTHON_MAGNET_INCLUDE::expose_magnet_dburt();
	BOOST_PYTHON_MAGNET_INCLUDE::expose_magnet_state();
	BOOST_PYTHON_MAGNET_INCLUDE::expose_magnet_states();
}
