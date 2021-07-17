#include <RunningStats.h>
#include <boost/python.hpp>

namespace BOOST_PYTHON_RUNNING_STATS_INCLUDE
{
	void expose_running_stats_object()
	{
		bool is_registered = (0 != boost::python::converter::registry::query(boost::python::type_id<RunningStats>())->to_python_target_type());
		if (is_registered) return;

		boost::python::class_<RunningStats, boost::noncopyable>("RunningStats", boost::python::no_init)
			.def("clear", &RunningStats::Clear)
			.def("setMaxCount", &RunningStats::setMaxCount)
			.def("getMaxCount", &RunningStats::getMaxCount)
			.def("numberOfValues", &RunningStats::NumDataValues)
			.def("mean", &RunningStats::Mean)
			.def("variance", &RunningStats::Variance)
			.def("standardDeviation", &RunningStats::StandardDeviation)
			.def("isFull", &RunningStats::Full)
			.def("isNotFull", &RunningStats::NotFull)
			
			/// TODO not properly implmented yet 
			.def("buffer", &RunningStats::Buffer_Py)
			.def("getRunningStats", &RunningStats::getRunningStats)
			.def("setBufferSize", &RunningStats::setBufferSize)
			.def("getBufferSize", &RunningStats::getBuferSize)
			.def("clearBuffer", &RunningStats::clearBuffer)
			;

	}
}