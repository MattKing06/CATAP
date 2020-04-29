#include <ChargePythonInterface.h>
#include <HardwarePythonInterface.h>
#include <GlobalStatesPythonInterface.h>
#include <GlobalTypesPythonInterface.h>
BOOST_PYTHON_MODULE(_Charge)
{
	boost::python::docstring_options CATAP_docstring_options;//(true, true, false);
	CATAP_docstring_options.enable_all();
	CATAP_docstring_options.disable_cpp_signatures();
	BOOST_PYTHON_STATES_INCLUDE::expose_global_state_enums();
	BOOST_PYTHON_TYPES_INCLUDE::expose_global_type_enums();
	BOOST_PYTHON_HARDWARE_INCLUDE::expose_hardware_object();
	BOOST_PYTHON_CHARGE_INCLUDE::expose_charge_object();
	BOOST_PYTHON_CHARGE_INCLUDE::expose_charge_factory_object();
}