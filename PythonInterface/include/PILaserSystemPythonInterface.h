#ifndef LASER_FEEDBACK_PYTHON_INTERFACE_H_
#define LASER_FEEDBACK_PYTHON_INTERFACE_H_

#include <PILaserSystem.h>
#include <PythonTypeConversions.h>
#include <boost/python.hpp>

namespace BOOST_PYTHON_LASER_FEEDBACK_INCLUDE
{
	void exposePILaserSystemObject()
	{
		bool is_registered = (0 != boost::python::converter::registry::query(boost::python::type_id<PILaserSystem>())->to_python_target_type());
		if (is_registered) return;
		boost::python::class_<PILaserSystem, boost::noncopyable>("PILaserSystem", boost::python::no_init)
			.def(boost::python::init<STATE>(boost::python::args("mode")))
			.def("setup", &PILaserSystem::setup, (boost::python::arg("self"), boost::python::arg("version")))
			.def("getCameraName", &PILaserSystem::getCameraName_Py)
			.def("getQ", &PILaserSystem::getQ)
			.def("getEnergy", &PILaserSystem::getEnergy)
			.def("getEnergyRange", &PILaserSystem::getEnergyRange)
			.def("openShutter", &PILaserSystem::openLaserShutter)
			;
	}

}





#endif // LASER_FEEDBACK_PYTHON_INTERFACE_H_