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
		bool is_registered = (0 != boost::python::converter::registry::query(boost::python::type_id<LaserHWP>())->to_python_target_type());
		if (is_registered) return;
		boost::python::class_<LaserHWP, boost::python::bases<Hardware>, boost::noncopyable>("LaserHWP", boost::python::no_init)
			.add_property("name", &LaserHWP::getLaserHWPName)
			.add_property("type", &LaserHWP::getLaserHWPType)
			.add_property("hwpset", &LaserHWP::getHWPSet)
			.add_property("hwpread", &LaserHWP::getHWPRead)
			.def("attachContext", &LaserHWP::attachToInitialContext)
			.def("detachContext", &LaserHWP::detachFromInitialContext)
			.def("setHWP", &LaserHWP::setHWP)
			.def("getHWPSet", &LaserHWP::getHWPSet)
			.def("getHWPRead", &LaserHWP::getHWPRead)
			.def("isHWPEnabled", &LaserHWP::isHWPEnabled)
			.def("isHWPDisabled", &LaserHWP::isHWPDisabled)
			.def("enableHWP", &LaserHWP::enableHWP)
			.def("disableHWP", &LaserHWP::disableHWP)
			.def("getHWPEnableState", &LaserHWP::getHWPEnableState);
	}
	
	void expose_laser_hwp_factory_object() {

		//laser Factory Exposure
		bool is_registered = (0 != boost::python::converter::registry::query(boost::python::type_id<LaserHWPFactory>())->to_python_target_type());
		if (is_registered) return;
		void(LaserHWPFactory:: * attachContext_single)(const std::string&) = &LaserHWPFactory::attachContext;
		void(LaserHWPFactory:: * attachContext_all)(void) = &LaserHWPFactory::attachContext;
		void(LaserHWPFactory:: * detachContext_single)(const std::string&) = &LaserHWPFactory::detachContext;
		void(LaserHWPFactory:: * detachContext_all)(void) = &LaserHWPFactory::detachContext;
		boost::python::class_<LaserHWPFactory>("LaserHWPFactory", boost::python::no_init)
			.def(boost::python::init<STATE>())
			.def(boost::python::init<STATE, const std::string>())
			.def("attachContext", &LaserHWPFactory::attachContext_Py)
			.def("attachContext", attachContext_single)
			.def("attachContext", attachContext_all)
			.def("detachContext", &LaserHWPFactory::detachContext_Py)
			.def("detachContext", detachContext_single)
			.def("detachContext", detachContext_all)
			.def("setup", &LaserHWPFactory::setup)
			.add_property("laserMap", &LaserHWPFactory::laserHWPMap)
			.def("getLaserHWP", &LaserHWPFactory::getLaserHWP, boost::python::return_value_policy<boost::python::reference_existing_object>())
			.def("getLaserHWPs", &LaserHWPFactory::getLaserHWPs)
			//.def("getAllLaserHWPs", &LaserHWPFactory::getAllLaserHWPs)
			.def("setHWP", &LaserHWPFactory::setHWP)
			.def("getHWPSet", &LaserHWPFactory::getHWPSet)
			.def("getHWPRead", &LaserHWPFactory::getHWPRead);
	}

	
}
#endif