#include <LEDPythonInterface.h>

BOOST_PYTHON_MODULE(_LED)
{
	BOOST_PYTHON_LED_INCLUDE::expose_led_object();
	BOOST_PYTHON_LED_INCLUDE::expose_led_factory_object();
}