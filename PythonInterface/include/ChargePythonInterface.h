#ifndef CHARGE_PYTHON_INTERFACE_H_
#define CHARGE_PYTHON_INTERFACE_H_
#include "Charge.h"
#include "ChargeFactory.h"
#include "PythonTypeConversions.h"

#include <boost/python.hpp>

//using namespace boost::PYTHON;
//using namespace boost;
namespace BOOST_PYTHON_CHARGE_INCLUDE
{
	void expose_charge_object() {

		boost::python::class_<Charge, boost::python::bases<Hardware>, boost::noncopyable>("Charge", boost::python::no_init)
			.add_property("name", &Charge::getName)
			.add_property("charge_type", &Charge::getChargeDiagnosticType)
			.add_property("q", &Charge::getQ)
			.add_property("qbuffer", &Charge::getQBuffer)
			.add_property("qvector", &Charge::getQVector)
			.add_property("position", &Charge::getPosition)
			.add_property("buffersize", &Charge::getBufferSize, &Charge::setBufferSize)
			.def("getQ", &Charge::getQ)
			.def("getQBuffer", &Charge::getQBuffer)
			.def("getQVector", &Charge::getQVector)
			.def("monitorForNShots", &Charge::monitorForNShots)
			.def("ismonitoring", &Charge::ismonitoring)
			.def("setBufferSize", &Charge::setBufferSize);
	}

	//typedef std::pair<int, int> IntPair;
	//// This is our previous registratiOn functiOn (as above) using the cleaner typedef.
	//to_PYTHON_cOnverter<IntPair, PairToTupleCOnverter<int, int> >();
	//// Registers a cOnverter for a pair of int-pairs.
	//to_PYTHON_cOnverter<std::pair<IntPair, IntPair>, PairToTupleCOnverter<IntPair, IntPair> >();




	
	void expose_charge_factory_object() {

		//charge Factory Exposure
		boost::python::class_<ChargeFactory>("ChargeFactory", boost::python::no_init)
			.def(boost::python::init<STATE>())
			.def("setup", &ChargeFactory::setup)
			.add_property("chargeMap", &ChargeFactory::chargeMap)
			.def("getChargeDiagnostic", &ChargeFactory::getChargeDiagnostic, boost::python::return_value_policy<boost::python::reference_existing_object>())
			.def("getChargeDiagnostics", &ChargeFactory::getChargeDiagnostics)
			.def("getAllChargeDiagnostics", &ChargeFactory::getAllChargeDiagnostics)
			.def("monitorForNShots", &ChargeFactory::monitorForNShots)
			.def("monitorForNShots", &ChargeFactory::monitorForNShots_Py)
			.def("ismonitoring", &ChargeFactory::ismonitoring)
			.def("getPosition", &ChargeFactory::getPosition)
			.def("getQ", &ChargeFactory::getQ)
			.def("getQVector", &ChargeFactory::getQVector_Py)
			.def("getQBuffer", &ChargeFactory::getQBuffer_Py)
			.def("getPositions", &ChargeFactory::getPositions_Py)
			.def("getQVectors", &ChargeFactory::getQVectors_Py)
			.def("getQBuffers", &ChargeFactory::getQBuffers_Py)
			.def("getAllQ", &ChargeFactory::getAllQ)
			.def("getAllPosition", &ChargeFactory::getAllPosition_Py)
			.def("getAllQVector", &ChargeFactory::getAllQVector)
			.def("getAllQBuffer", &ChargeFactory::getAllQBuffer)
			.def("setBufferSize", &ChargeFactory::setBufferSize);

	}


}
#endif