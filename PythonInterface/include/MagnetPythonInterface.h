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
			.add_property("current", &Magnet::getCurrent, &Magnet::setEPICSCurrent)
			.add_property("psu_state", &Magnet::getPSUState, &Magnet::setEPICSPSUState)
			.add_property("ri_current", &Magnet::RICurrent)
			.add_property("name", &Magnet::getHardwareName)
			.add_property("manufacturer", &Magnet::getManufacturer)
			.add_property("serial_number", &Magnet::getSerialNumber)
			.add_property("magnet_type", &Magnet::getMagnetType)
			.add_property("epicsInterface", &Magnet::epicsInterface)
			.def("getCurrent", &Magnet::getCurrent)
			.def("setCurrent", &Magnet::setEPICSCurrent)
			.def("getRICurrent", &Magnet::getRICurrent);

	}

	//typedef std::pair<int, int> IntPair;
	//// This is our previous registration function (as above) using the cleaner typedef.
	//to_python_converter<IntPair, PairToTupleConverter<int, int> >();
	//// Registers a converter for a pair of int-pairs.
	//to_python_converter<std::pair<IntPair, IntPair>, PairToTupleConverter<IntPair, IntPair> >();

	template<class T1, class T2>
	struct map_to_dict_converter {
		static PyObject* convert(const std::map<T1, T2>& map) {
			//return incref(make_map(map.first, map.second).ptr());
			return incref(to_py_dict(map).ptr());
		}
	};
	typedef std::map<std::string, STATE> string_state_map;
	 boost::python::to_python_converter <string_state_map, map_to_dict_converter < std::string , STATE> >;
	// This is our previous registration function (as above) using the cleaner typedef.
	//boost::python::to_python_converter < string_state_map, map_to_dict_converter < std::string , STATE>>();
	
	void expose_magnet_factory_object() {

		//Magnet Factory Exposure
		STATE(MagnetFactory:: * turnOnSingle)(const std::string&) = &MagnetFactory::turnOn;
		STATE(MagnetFactory:: * turnOffSingle)(const std::string&) = &MagnetFactory::turnOff;
		boost::python::class_<MagnetFactory, boost::noncopyable>("MagnetFactory", boost::python::no_init)
			.def(boost::python::init<bool>())
			.def("setup", &MagnetFactory::setup)
			.add_property("magnetMap", &MagnetFactory::magnetMap)
			.def("getMagnet", &MagnetFactory::getMagnet, boost::python::return_value_policy<boost::python::reference_existing_object>())
			.def("getMagnets", &MagnetFactory::getMagnets)
			.def("getAllMagnets", &MagnetFactory::getAllMagnets)
			.def("getCurrent", &MagnetFactory::getCurrent)
			.def("getCurrents", &MagnetFactory::getCurrents_Py)
			.def("getAllMagnetCurrents", &MagnetFactory::getAllMagnetCurrents_Py)
			.def("setCurrent", &MagnetFactory::setCurrent)
			.def("setCurrents", &MagnetFactory::setCurrents_Py)
			.def("getRICurrent", &MagnetFactory::getRICurrent)
			.def("getRICurrents", &MagnetFactory::getRICurrents_Py)
			.def("turnOn", turnOnSingle)
			.def("turnOn", &MagnetFactory::turnOn_Py)
			.def("turnOff", turnOffSingle)
			.def("turnOff", &MagnetFactory::turnOff_Py)

			.def("getAllPSUState", &MagnetFactory::getAllPSUState_Py);
			boost::python::to_python_converter <string_state_map, map_to_dict_converter < std::string, STATE> >;

		//.def("debugMessagesOn", &MagnetFactory::debugMessagesOn)
		//.def("debugMessagesOff", &MagnetFactory::debugMessagesOff)
		//.def("messagesOn", &MagnetFactory::messagesOn)
		//.def("messagesOff", &MagnetFactory::messagesOff);
	

	}


}
#endif