#ifndef DATA_TYPES_PYTHON_INTERFACE_H_
#define DATA_TYPES_PYTHON_INTERFACE_H_

#include <boost/python.hpp>
#include <map>
#include <vector>
#include <boost/python/converter/registry.hpp>
#include <boost/python/suite/indexing/map_indexing_suite.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/circular_buffer.hpp>
#include <GlobalStateEnums.h>

namespace BOOST_PYTHON_DATA_TYPES_INCLUDE
{

	void expose_data_types()
	{
		boost::python::class_<std::map<std::string, double> >("numericalParamMap")
			.def(boost::python::map_indexing_suite<std::map<std::string, double> >());
		boost::python::class_<std::map<std::string, std::vector< double > > >("numericalVectorParamMap")
			.def(boost::python::map_indexing_suite<std::map<std::string, std::vector< double > > >());
		boost::python::class_<std::map<std::string, std::string> >("stringParamMap")
			.def(boost::python::map_indexing_suite<std::map<std::string, std::string> >());
		boost::python::class_<std::vector< double > >("stdVectorDouble")
			.def(boost::python::vector_indexing_suite<std::vector< double > >());
		boost::python::class_<boost::circular_buffer< double > >("circularBufferDouble")
			.def(boost::python::vector_indexing_suite<boost::circular_buffer< double > >());
		boost::python::class_<boost::circular_buffer< std::vector< double > > >("circularBufferDoubleVector")
			.def(boost::python::vector_indexing_suite<boost::circular_buffer< std::vector< double > > >());
		boost::python::class_<boost::circular_buffer< STATE > >("circularBufferState")
			.def(boost::python::vector_indexing_suite<boost::circular_buffer< STATE > >());
		boost::python::class_<std::vector< STATE > >("vectorState")
			.def(boost::python::vector_indexing_suite<std::vector< STATE > >());
		boost::python::class_<std::vector< boost::circular_buffer< STATE > > >("vectorcircularBufferState")
			.def(boost::python::vector_indexing_suite<boost::circular_buffer< STATE > >());
		boost::python::class_<std::vector< std::vector< STATE > > >("vectorvectorState")
			.def(boost::python::vector_indexing_suite<std::vector< std::vector< STATE > > >());
	}


}


#endif //DATA_TYPES_PYTHON_INTERFACE_H_