#include <ValvePythonInterface.h>
#include <HardwarePythonInterface.h>
BOOST_PYTHON_MODULE(_Valve)
{
	BOOST_PYTHON_HARDWARE_INCLUDE::expose_hardware_object();
	BOOST_PYTHON_VALVE_INCLUDE::exposeValveObject();
	BOOST_PYTHON_VALVE_INCLUDE::exposeValveFactoryObject();
}