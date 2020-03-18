#include <ScreenPythonInterface.h>
#include <HardwarePythonInterface.h>
BOOST_PYTHON_MODULE(_Screen)
{
	BOOST_PYTHON_HARDWARE_INCLUDE::expose_hardware_object();
	BOOST_PYTHON_SCREEN_INCLUDE::expose_screen_object();
	BOOST_PYTHON_SCREEN_INCLUDE::expose_screen_factory_object();
}