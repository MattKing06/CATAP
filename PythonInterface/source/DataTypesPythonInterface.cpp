#include <DataTypesPythonInterface.h>

BOOST_PYTHON_MODULE(_DataTypes)
{
	BOOST_PYTHON_DATA_TYPES_INCLUDE::expose_data_types();
}