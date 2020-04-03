#include <RFProtectionPythonInterface.h>

BOOST_PYTHON_MODULE(_RFProtection)
{
	BOOST_PYTHON_RF_PROTECTION_INCLUDE::expose_rf_protection_object();
	BOOST_PYTHON_RF_PROTECTION_INCLUDE::expose_rf_protection_factory_object();
}