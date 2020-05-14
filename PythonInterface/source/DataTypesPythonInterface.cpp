#include <DataTypesPythonInterface.h>

BOOST_PYTHON_MODULE(_DataTypes)
{
	boost::python::docstring_options CATAP_docstring_options;//(true, true, false);
	CATAP_docstring_options.enable_all();
	CATAP_docstring_options.disable_cpp_signatures();
	BOOST_PYTHON_DATA_TYPES_INCLUDE::expose_data_types();
}