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
			.add_property("mirror", &PILaserSystem::mirror)
			.add_property("energyMeter", &PILaserSystem::energyMeter)
			.add_property("halfwavePlate", &PILaserSystem::halfwavePlate)
			.add_property("shutter1", &PILaserSystem::laserShutter01)
			.add_property("shutter2", &PILaserSystem::laserShutter02)
			.add_property("virtualCathode", &PILaserSystem::virtualCathodeCamera)
			.add_property("wallCurrentMonitor", &PILaserSystem::wallCurrentMonitor)
			//.def("getCameraName", &PILaserSystem::getCameraName_Py)
			//.def("getQ", &PILaserSystem::getQ)
			//.def("getEnergy", &PILaserSystem::getEnergy)
			//.def("getEnergyRange", &PILaserSystem::getEnergyRange)
			//.def("setEnergyRange20uJ", &PILaserSystem::setEnergyRange20uJ)
			//.def("setEnergyRange2uJ", &PILaserSystem::setEnergyRange2uJ)
			//.def("setEnergyRange200nJ", &PILaserSystem::setEnergyRange200nJ)
			//.def("setEnergyRange20nJ", &PILaserSystem::setEnergyRange20nJ)
			//.def("openShutter", &PILaserSystem::openLaserShutter)
			//.def("getHWPRead", &PILaserSystem::getHWPRead)
			//.def("getHWPSet", &PILaserSystem::getHWPSet)
			//.def("setHStep", &PILaserSystem::setHStep)
			//.def("setVStep", &PILaserSystem::setVStep)
			//.def("moveLeft", &PILaserSystem::moveLeft)
			//.def("moveRight", &PILaserSystem::moveRight)
			//.def("moveUp", &PILaserSystem::moveUp)
			//.def("moveDown", &PILaserSystem::moveDown)
			//.def("getCurrentVerticalPosition", &PILaserSystem::getCurrentVerticalPosition)
			//.def("getCurrentHorizontalPosition", &PILaserSystem::getCurrentHorizontalPosition)
			//.def("getLeftSense", &PILaserSystem::getLeftSense)
			//.def("getRightSense", &PILaserSystem::getRightSense)
			//.def("getUpSense", &PILaserSystem::getUpSense)
			//.def("getDownSense", &PILaserSystem::getDownSense)
			;
	}

}





#endif // LASER_FEEDBACK_PYTHON_INTERFACE_H_