#include <ValvePythonInterface.h>
BOOST_PYTHON_MODULE(_Valve)
{
	BOOST_PYTHON_VALVE_INCLUDE::exposeValveObject();
	BOOST_PYTHON_VALVE_INCLUDE::exposeValveFactoryObject();
}