#include <boost/python.hpp>
#include <GlobalStatesPythonInterface.h>

BOOST_PYTHON_MODULE(_GlobalStates)
{
	BOOST_PYTHON_STATES_INCLUDE::expose_global_state_enums();
}

