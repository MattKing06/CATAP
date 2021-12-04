#ifndef EPICS_ARCHIVER_PYTHON_INTERFACE_H_
#define EPICS_ARCHIVER_PYTHON_INTERFACE_H_

#include <EPICSArchiverHandler.h>
#include <PythonTypeConversions.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

namespace BOOST_PYTHON_EPICS_ARCHIVER_INCLUDE
{
	void expose_epics_archiver_object()
	{
		boost::python::class_<EPICSArchiverHandler, boost::noncopyable>("EPICSArchiver", boost::python::no_init)
			.add_property("__str__", &EPICSArchiverHandler::getAddress)
			;
	}
}


#endif //EPICS_ARCHIVER_PYTHON_INTERFACE_H_