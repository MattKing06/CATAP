#include <RFModulatorPythonInterface.h>

BOOST_PYTHON_MODULE(_RFModulator)
{
	BOOST_PYTHON_RF_MODULATOR_INCLUDE::expose_rf_modulator_object();
	BOOST_PYTHON_RF_MODULATOR_INCLUDE::expose_rf_modulator_factory_object();
}