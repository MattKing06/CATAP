#ifndef DATA_TYPES_PYTHON_INTERFACE_H_
#define DATA_TYPES_PYTHON_INTERFACE_H_

#include <boost/python.hpp>
#include <map>
#include <vector>
#include <boost/python/converter/registry.hpp>
#include <boost/python/suite/indexing/map_indexing_suite.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/converter/registry.hpp>
#include <boost/circular_buffer.hpp>
#include <GlobalStateEnums.h>

namespace BOOST_PYTHON_DATA_TYPES_INCLUDE
{

	void expose_data_types()
	{
		bool is_numerical_map_registered = (0 != boost::python::converter::registry::query(boost::python::type_id<std::map<std::string, double> >())->to_python_target_type());
		if (is_numerical_map_registered) return;
		boost::python::class_<std::map<std::string, double> >("numericalParamMap")
			.def(boost::python::map_indexing_suite<std::map<std::string, double> >());

		bool is_numerical_vector_registered = (0 != boost::python::converter::registry::query(boost::python::type_id<std::map<std::string, std::vector<double>>>())->to_python_target_type());
		if (is_numerical_vector_registered) return;
		boost::python::class_<std::map<std::string, std::vector< double > > >("numericalVectorParamMap")
			.def(boost::python::map_indexing_suite<std::map<std::string, std::vector< double > > >());

		bool is_string_map_registered = (0 != boost::python::converter::registry::query(boost::python::type_id<std::map<std::string, std::string>>())->to_python_target_type());
		if (is_string_map_registered) return;
		boost::python::class_<std::map<std::string, std::string> >("stringParamMap")
			.def(boost::python::map_indexing_suite<std::map<std::string, std::string> >());

		bool is_double_vector_registered = (0 != boost::python::converter::registry::query(boost::python::type_id<std::vector<double>>())->to_python_target_type());
		if (is_double_vector_registered) return;
		boost::python::class_<std::vector< double > >("stdVectorDouble")
			.def(boost::python::vector_indexing_suite<std::vector< double > >());

		bool is_double_circ_buffer_registered = (0 != boost::python::converter::registry::query(boost::python::type_id<boost::circular_buffer<double>>())->to_python_target_type());
		if (is_double_circ_buffer_registered) return;
		boost::python::class_<boost::circular_buffer< double > >("circularBufferDouble")
			.def(boost::python::vector_indexing_suite<boost::circular_buffer< double > >());

		bool is_double_vector_circ_buffer_registered = (0 != boost::python::converter::registry::query(boost::python::type_id<boost::circular_buffer<std::vector< double>>>())->to_python_target_type());
		if (is_double_vector_circ_buffer_registered) return;
		boost::python::class_<boost::circular_buffer< std::vector< double > > >("circularBufferDoubleVector")
			.def(boost::python::vector_indexing_suite<boost::circular_buffer< std::vector< double > > >());

		bool is_state_circ_buffer_register = (0 != boost::python::converter::registry::query(boost::python::type_id<boost::circular_buffer<STATE>>())->to_python_target_type());
		if (is_state_circ_buffer_register) return;
		boost::python::class_<boost::circular_buffer< STATE > >("circularBufferState")
			.def(boost::python::vector_indexing_suite<boost::circular_buffer< STATE > >());

		bool is_state_vector_registered = (0 != boost::python::converter::registry::query(boost::python::type_id<std::vector<STATE>>())->to_python_target_type());
		if (is_state_vector_registered) return;
		boost::python::class_<std::vector< STATE > >("vectorState")
			.def(boost::python::vector_indexing_suite<std::vector< STATE > >());

		bool is_state_circ_buffer_vector_registered = (0 != boost::python::converter::registry::query(boost::python::type_id<std::vector<boost::circular_buffer<STATE>>>())->to_python_target_type());
		if (is_state_circ_buffer_vector_registered) return;
		boost::python::class_<std::vector< boost::circular_buffer< STATE > > >("vectorcircularBufferState")
			.def(boost::python::vector_indexing_suite<boost::circular_buffer< STATE > >());

		bool is_state_vector_vector_registered = (0 != boost::python::converter::registry::query(boost::python::type_id<std::vector<std::vector<STATE>>>())->to_python_target_type());
		if (is_state_vector_vector_registered) return;
		boost::python::class_<std::vector< std::vector< STATE > > >("vectorvectorState")
			.def(boost::python::vector_indexing_suite<std::vector< std::vector< STATE > > >());
	}


}


#endif //DATA_TYPES_PYTHON_INTERFACE_H_