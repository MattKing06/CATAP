#include <ChargePythonInterface.h>
BOOST_PYTHON_MODULE(_Charge)
{
	BOOST_PYTHON_CHARGE_INCLUDE::expose_charge_object();
	BOOST_PYTHON_CHARGE_INCLUDE::expose_charge_factory_object();
}