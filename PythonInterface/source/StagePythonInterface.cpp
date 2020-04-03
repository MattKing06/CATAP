#include <StagePythonInterface.h>

BOOST_PYTHON_MODULE(_Stage)
{
	BOOST_PYTHON_STAGE_INCLUDE::expose_stage_object();
	BOOST_PYTHON_STAGE_INCLUDE::expose_stage_factory_object();
}