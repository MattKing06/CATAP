#include <PILaserSystemPythonInterface.h>
#include <GlobalStatesPythonInterface.h>
#include <GlobalTypesPythonInterface.h>

BOOST_PYTHON_MODULE(_PILaserSystem)
{
	boost::python::docstring_options CATAP_docstring_options;
	CATAP_docstring_options.enable_all();
	CATAP_docstring_options.disable_cpp_signatures();
	BOOST_PYTHON_TYPES_INCLUDE::expose_global_type_enums();
	BOOST_PYTHON_STATES_INCLUDE::expose_global_state_enums();
	BOOST_PYTHON_LASER_FEEDBACK_INCLUDE::exposePILaserSystemObject();
}