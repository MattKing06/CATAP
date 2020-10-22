#ifndef LASER_ENERGY_METER_PYTHON_INTERFACE_H_
#define LASER_ENERGY_METER_PYTHON_INTERFACE_H_
#include "LaserEnergyMeter.h"
#include "LaserEnergyMeterFactory.h"
#include "PythonTypeConversions.h"

#include <boost/python.hpp>

//using namespace boost::PYTHON;
//using namespace boost;
namespace BOOST_PYTHON_LASER_ENERGY_METER_INCLUDE
{
	void expose_laser_energy_meter_object() {

		// laser exposure
		boost::python::class_<LaserEnergyMeter, boost::python::bases<Hardware>, boost::noncopyable>("LaserEnergyMeter", boost::python::no_init)
			.add_property("name", &LaserEnergyMeter::getLaserEnergyMeterName)
			.add_property("type", &LaserEnergyMeter::getLaserEnergyMeterType)
			.add_property("overrange", &LaserEnergyMeter::getOverRange)
			.add_property("acquiring", &LaserEnergyMeter::getAcquiring)
			.def("getOverRange", &LaserEnergyMeter::getOverRange)
			.def("setStart", &LaserEnergyMeter::setStart)
			.def("setStop", &LaserEnergyMeter::setStop)
			.def("getAcquiring", &LaserEnergyMeter::getAcquiring)
			.def("getRange", &LaserEnergyMeter::getRange)
			.def("setRange", &LaserEnergyMeter::setRange);

	}
	
	void expose_laser_energy_meter_factory_object() {

		//laser Factory Exposure
		boost::python::class_<LaserEnergyMeterFactory>("LaserEnergyMeterFactory", boost::python::no_init)
			.def(boost::python::init<STATE>())
			.def("setup", &LaserEnergyMeterFactory::setup)
			.add_property("laserEnergyMeterMap", &LaserEnergyMeterFactory::laserEnergyMeterMap)
			.def("getLaserEnergyMeter", &LaserEnergyMeterFactory::getLaserEnergyMeter, boost::python::return_value_policy<boost::python::reference_existing_object>())
			.def("getLaserEnergyMeters", &LaserEnergyMeterFactory::getLaserEnergyMeters)
			.def("getAllLaserEnergyMeters", &LaserEnergyMeterFactory::getAllLaserEnergyMeters)
			.def("setStart", &LaserEnergyMeter::setStart)
			.def("setStop", &LaserEnergyMeter::setStop)
			.def("getAcquiring", &LaserEnergyMeter::getAcquiring)
			.def("getRange", &LaserEnergyMeter::getRange)
			.def("setRange", &LaserEnergyMeter::setRange);;
	}


}
#endif