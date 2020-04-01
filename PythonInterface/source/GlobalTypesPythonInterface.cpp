#include <boost/python.hpp>
#include <GlobalTypesPythonInterface.h>

BOOST_PYTHON_MODULE(_GlobalTypes)
{
	BOOST_PYTHON_TYPES_INCLUDE::expose_global_type_enums();
}

