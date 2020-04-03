#include <LightingPythonInterface.h>

BOOST_PYTHON_MODULE(_Lighting)
{
	BOOST_PYTHON_LIGHTING_INCLUDE::expose_lighting_object();
	BOOST_PYTHON_LIGHTING_INCLUDE::expose_lighting_factory_object();
}