#include <boost/python.hpp>
#include <GlobalStatesPythonInterface.h>

BOOST_PYTHON_MODULE(_GlobalStates)
{
	boost::python::docstring_options CATAP_docstring_options;//(true, true, false);
	CATAP_docstring_options.enable_all();
	CATAP_docstring_options.disable_cpp_signatures();
	BOOST_PYTHON_STATES_INCLUDE::expose_global_state_enums();
}

