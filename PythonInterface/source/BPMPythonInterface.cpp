#include <BPMPythonInterface.h>
BOOST_PYTHON_MODULE(_BPM)
{
	BOOST_PYTHON_BPM_INCLUDE::expose_bpm_object();
	BOOST_PYTHON_BPM_INCLUDE::expose_bpm_factory_object();
}