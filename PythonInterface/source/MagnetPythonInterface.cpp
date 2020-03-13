#include <MagnetPythonInterface.h>
#include <HardwarePythonInterface.h>
BOOST_PYTHON_MODULE(_Magnet)
{
	BOOST_PYTHON_HARDWARE_INCLUDE::expose_hardware_object();
	BOOST_PYTHON_MAGNET_INCLUDE::expose_magnet_object();
	BOOST_PYTHON_MAGNET_INCLUDE::expose_magnet_state_struct();
	BOOST_PYTHON_MAGNET_INCLUDE::expose_magnet_factory_object();
}
