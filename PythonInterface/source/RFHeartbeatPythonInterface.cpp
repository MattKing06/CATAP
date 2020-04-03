#include <RFHeartbeatPythonInterface.h>

BOOST_PYTHON_MODULE(_RFHeartbeat)
{
	BOOST_PYTHON_RF_HEARTBEAT_INCLUDE::expose_rf_heartbeat_object();
	BOOST_PYTHON_RF_HEARTBEAT_INCLUDE::expose_rf_heartbeat_factory_object();
}