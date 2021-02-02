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
		bool is_registered = (0 != boost::python::converter::registry::query(boost::python::type_id<LaserEnergyMeter>())->to_python_target_type());
		if (is_registered) return;
		boost::python::class_<LaserEnergyMeter, boost::python::bases<Hardware>, boost::noncopyable>("LaserEnergyMeter", boost::python::no_init)
			.add_property("name", &LaserEnergyMeter::getLaserEnergyMeterName)
			.add_property("type", &LaserEnergyMeter::getLaserEnergyMeterType)
			.add_property("overrange", &LaserEnergyMeter::getOverRange)
			.add_property("acquiring", &LaserEnergyMeter::getAcquiring)
			.add_property("monitoring", &LaserEnergyMeter::isMonitoring)
			.add_property("energybuffer", &LaserEnergyMeter::getEnergyBuffer)
			.add_property("energyvector", &LaserEnergyMeter::getEnergyVector)
			.add_property("energy", &LaserEnergyMeter::getEnergy)
			.def("getOverRange", &LaserEnergyMeter::getOverRange)
			.def("setStart", &LaserEnergyMeter::setStart)
			.def("setStop", &LaserEnergyMeter::setStop)
			.def("getAcquiring", &LaserEnergyMeter::getAcquiring)
			.def("isMonitoring", &LaserEnergyMeter::isMonitoring)
			.def("setRange", &LaserEnergyMeter::setRange)
			.def("getRange", &LaserEnergyMeter::getRange)
			.def("getEnergy", &LaserEnergyMeter::getEnergy)
			.def("getEnergyVector", &LaserEnergyMeter::getEnergyVector)
			.def("getEnergyBuffer", &LaserEnergyMeter::getEnergyBuffer)
			.def("setVectorSize", &LaserEnergyMeter::setVectorSize)
			.def("setBufferSize", &LaserEnergyMeter::setBufferSize)
			.def("isEnergyVectorFull", &LaserEnergyMeter::isEnergyVectorFull)
			.def("isEnergyBufferFull", &LaserEnergyMeter::isEnergyBufferFull)
			.def("monitorForNShots", &LaserEnergyMeter::monitorForNShots);
	}
	
	void expose_laser_energy_meter_factory_object() {

		//laser Factory Exposure
		bool is_registered = (0 != boost::python::converter::registry::query(boost::python::type_id<LaserEnergyMeterFactory>())->to_python_target_type());
		if (is_registered) return;
		boost::python::class_<LaserEnergyMeterFactory>("LaserEnergyMeterFactory", boost::python::no_init)
			.def(boost::python::init<STATE>())
			.def("setup", &LaserEnergyMeterFactory::setup)
			.add_property("laserEnergyMeterMap", &LaserEnergyMeterFactory::laserEnergyMeterMap)
			.def("getLaserEnergyMeter", &LaserEnergyMeterFactory::getLaserEnergyMeter, boost::python::return_value_policy<boost::python::reference_existing_object>())
			//.def("getLaserEnergyMeters", &LaserEnergyMeterFactory::getLaserEnergyMeters)
			//.def("getAllLaserEnergyMeters", &LaserEnergyMeterFactory::getAllLaserEnergyMeters)
			.def("setStart", &LaserEnergyMeterFactory::setStart)
			.def("setStop", &LaserEnergyMeterFactory::setStop)
			.def("isMonitoring", &LaserEnergyMeterFactory::isMonitoring)
			.def("getAcquiring", &LaserEnergyMeterFactory::getAcquiring)
			.def("getOverRange", &LaserEnergyMeterFactory::getOverRange)
			.def("getRange", &LaserEnergyMeterFactory::getRange)
			.def("setRange", &LaserEnergyMeterFactory::setRange)
			.def("getEnergy", &LaserEnergyMeterFactory::getEnergy)
			.def("getEnergyVector", &LaserEnergyMeterFactory::getEnergyVector_Py)
			.def("getEnergyBuffer", &LaserEnergyMeterFactory::getEnergyBuffer_Py)
			.def("setVectorSize", &LaserEnergyMeterFactory::setVectorSize)
			.def("setBufferSize", &LaserEnergyMeterFactory::setBufferSize)
			.def("isEnergyVectorFull", &LaserEnergyMeterFactory::isEnergyVectorFull)
			.def("isEnergyBufferFull", &LaserEnergyMeterFactory::isEnergyBufferFull)
			.def("monitorForNShots", &LaserEnergyMeterFactory::monitorForNShots);
	}


}
#endif