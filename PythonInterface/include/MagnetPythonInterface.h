#ifndef MAGNET_PYTHON_INTERFACE_H_
#define MAGNET_PYTHON_INTERFACE_H_
#include "Magnet.h"
#include "MagnetFactory.h"
#include "PythonTypeConversions.h"

#include <boost/python.hpp>

//using namespace boost::python;
//using namespace boost;
namespace BOOST_PYTHON_MAGNET_INCLUDE
{
	//class MagnetFactory;
	//boost::python::dict getAllPSUState_Py()
	//{
	//	return to_py_dict(MagnetFactory::getAllPSUState())
	//}

	void expose_magnet_object() {

		// magnet exposure
		boost::python::class_<Magnet, boost::python::bases<Hardware>, boost::noncopyable>("Magnet", boost::python::no_init)
			.add_property("SETI", &Magnet::getSETI, &Magnet::setEPICSSETI)
			.add_property("psu_state", &Magnet::getPSUState, &Magnet::setEPICSPSUState)
			.add_property("READI", &Magnet::getREADI)
			.add_property("name", &Magnet::getHardwareName)
			.add_property("manufacturer", &Magnet::getManufacturer)
			.add_property("serial_number", &Magnet::getSerialNumber)
			.add_property("magnet_type", &Magnet::getMagnetType)
			.add_property("epicsInterface", &Magnet::epicsInterface)
			.def("getSETI", &Magnet::getSETI)
			.def("SETI", &Magnet::SETI)
			.def("getREADI", &Magnet::getREADI);

	}

	//typedef std::pair<int, int> IntPair;
	//// This is our previous registration function (as above) using the cleaner typedef.
	//to_python_converter<IntPair, PairToTupleConverter<int, int> >();
	//// Registers a converter for a pair of int-pairs.
	//to_python_converter<std::pair<IntPair, IntPair>, PairToTupleConverter<IntPair, IntPair> >();

	
	void expose_magnet_factory_object() {

		//Magnet Factory Exposure
		STATE(MagnetFactory:: * turnOnSingle)(const std::string&) = &MagnetFactory::turnOn;
		STATE(MagnetFactory:: * turnOffSingle)(const std::string&) = &MagnetFactory::turnOff;
		
		void(MagnetFactory:: * SETISingle)(const std::string&, const double&) = &MagnetFactory::SETI;
		void(MagnetFactory:: * SETIMultiple)(const std::string&, const double&) = &MagnetFactory::SETI;

		boost::python::class_<MagnetFactory, boost::noncopyable>("MagnetFactory", boost::python::no_init)
			.def(boost::python::init<STATE>())
			.def("setup", &MagnetFactory::setup)
			//.add_property("magnetMap", &MagnetFactory::magnetMap)
			.def("getMagnet", &MagnetFactory::getMagnet, boost::python::return_value_policy<boost::python::reference_existing_object>())
			// NO you can't do this is you can't create a map of Magnet references 
			//.def("getMagnets", &MagnetFactory::getMagnets_Py, boost::python::return_value_policy<boost::python::reference_existing_object>())
			
			.def("getAllMagnetNames",&MagnetFactory::getAllMagnetNames_Py)

			.def("getSETI",    &MagnetFactory::getSETI)
			.def("getSETIs",    &MagnetFactory::getSETIs_Py)
			.def("getAllSETI", &MagnetFactory::getAllSETI)
			
			
			//.def("getAllMagnetCurrents", &MagnetFactory::getAllMagnetCurrents_Py)
			
			//.def("SETI", &MagnetFactory::SETI)
			.def("SETI", SETISingle)
			
			.def("getRICurrent", &MagnetFactory::getRICurrent)
			.def("getRICurrents", &MagnetFactory::getRICurrents_Py)
			
			.def("turnOn", turnOnSingle)
			.def("turnOn", &MagnetFactory::turnOn_Py)
			.def("turnOff", turnOffSingle)
			.def("turnOff", &MagnetFactory::turnOff_Py)

			.def("getAllPSUState", &MagnetFactory::getAllPSUState_Py);

		
		//.def("debugMessagesOn", &MagnetFactory::debugMessagesOn)
		//.def("debugMessagesOff", &MagnetFactory::debugMessagesOff)
		//.def("messagesOn", &MagnetFactory::messagesOn)
		//.def("messagesOff", &MagnetFactory::messagesOff);
	

	}


}
#endif