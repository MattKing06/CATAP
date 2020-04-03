#include <ShutterPythonInterface.h>

BOOST_PYTHON_MODULE(_Shutter)
{
	BOOST_PYTHON_SHUTTER_INCLUDE::expose_shutter_object();
	BOOST_PYTHON_SHUTTER_INCLUDE::expose_shutter_factory_object();
}