#include <HardwareFactoryPythonInterface.h>
#include <HardwarePythonInterface.h>
BOOST_PYTHON_MODULE(_HardwareFactory)
{
	boost::python::docstring_options CATAP_docstring_options;//(true, true, false);
	CATAP_docstring_options.enable_all();
	CATAP_docstring_options.disable_cpp_signatures();
	BOOST_PYTHON_HARDWARE_INCLUDE::expose_hardware_object();
	BOOST_PYTHON_HARDWARE_FACTORY_INCLUDE::exposeHardwareFactoryObject();
}