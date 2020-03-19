#include <BPMPythonInterface.h>
#include <HardwarePythonInterface.h>
BOOST_PYTHON_MODULE(_BPM)
{
	BOOST_PYTHON_HARDWARE_INCLUDE::expose_hardware_object();
	BOOST_PYTHON_BPM_INCLUDE::expose_bpm_object();
	BOOST_PYTHON_BPM_INCLUDE::expose_bpm_factory_object();
}