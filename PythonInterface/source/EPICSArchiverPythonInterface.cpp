#include <EPICSArchiverPythonInterface.h>

BOOST_PYTHON_MODULE(_EPICSArchiver)
{
	boost::python::docstring_options CATAP_docstring_options;//(true, true, false);
	CATAP_docstring_options.enable_all();
	CATAP_docstring_options.disable_cpp_signatures();
	BOOST_PYTHON_EPICS_ARCHIVER_INCLUDE::expose_epics_archiver_object();
}