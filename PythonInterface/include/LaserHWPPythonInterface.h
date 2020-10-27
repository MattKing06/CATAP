#ifndef LASER_HWP_PYTHON_INTERFACE_H_
#define LASER_HWP_PYTHON_INTERFACE_H_
#include "LaserHWP.h"
#include "LaserHWPFactory.h"
#include "PythonTypeConversions.h"

#include <boost/python.hpp>

//using namespace boost::PYTHON;
//using namespace boost;
namespace BOOST_PYTHON_LASER_HWP_INCLUDE
{
	void expose_laser_hwp_object() {

		// laser exposure
		boost::python::class_<LaserHWP, boost::python::bases<Hardware>, boost::noncopyable>("LaserHWP", boost::python::no_init)
			.add_property("name", &LaserHWP::getLaserHWPName)
			.add_property("type", &LaserHWP::getLaserHWPType)
			.add_property("hwpset", &LaserHWP::getHWPSet)
			.add_property("hwpread", &LaserHWP::getHWPRead)
			.def("setHWP", &LaserHWP::setHWP)
			.def("getHWPSet", &LaserHWP::getHWPSet)
			.def("getHWPRead", &LaserHWP::getHWPRead);
	}
	
	void expose_laser_hwp_factory_object() {

		//laser Factory Exposure
		boost::python::class_<LaserHWPFactory>("LaserHWPFactory", boost::python::no_init)
			.def(boost::python::init<STATE>())
			.def("setup", &LaserHWPFactory::setup)
			.add_property("laserMap", &LaserHWPFactory::laserHWPMap)
			.def("getLaserHWP", &LaserHWPFactory::getLaserHWP, boost::python::return_value_policy<boost::python::reference_existing_object>())
			.def("getLaserHWPs", &LaserHWPFactory::getLaserHWPs)
			.def("getAllLaserHWPs", &LaserHWPFactory::getAllLaserHWPs)
			.def("setHWP", &LaserHWPFactory::setHWP)
			.def("getHWPSet", &LaserHWPFactory::getHWPSet)
			.def("getHWPRead", &LaserHWPFactory::getHWPRead);
	}


}
#endif