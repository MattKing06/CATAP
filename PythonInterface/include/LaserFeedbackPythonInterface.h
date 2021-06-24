#ifndef LASER_FEEDBACK_PYTHON_INTERFACE_H_
#define LASER_FEEDBACK_PYTHON_INTERFACE_H_

#include <LaserFeedback.h>
#include <PythonTypeConversions.h>
#include <boost/python.hpp>

namespace BOOST_PYTHON_LASER_FEEDBACK_INCLUDE
{
	void exposeLaserFeedbackObject()
	{
		bool is_registered = (0 != boost::python::converter::registry::query(boost::python::type_id<LaserFeedback>())->to_python_target_type());
		if (is_registered) return;
		boost::python::class_<LaserFeedback, boost::noncopyable>("LaserFeedback", boost::python::no_init)
			.def(boost::python::init<STATE>(boost::python::args("mode")))
			.def("setup", &LaserFeedback::setup, (boost::python::arg("self"), boost::python::arg("version")))
			.def("getCameraName", &LaserFeedback::getCameraName_Py)
			;
	}

}





#endif // LASER_FEEDBACK_PYTHON_INTERFACE_H_