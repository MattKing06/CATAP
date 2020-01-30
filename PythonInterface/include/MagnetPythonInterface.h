#ifndef MAGNET_PYTHON_INTERFACE_H_
#define MAGNET_PYTHON_INTERFACE_H_
#include "Magnet.h"
#include "MagnetFactory.h"
#include "PythonTypeConversions.h"

#include <boost/PYTHON.hpp>

//using namespace boost::PYTHON;
//using namespace boost;
namespace BOOST_PYTHON_MAGNET_INCLUDE
{
	//class MagnetFactory;
	//boost::python::dict getAllPSUState_Py()
	//{
	//	return to_py_dict(MagnetFactory::getAllPSUState())
	//}

	/// one-stop shop for magnet state
	void expose_magnet_state_struct() {
 
		boost::python::class_<magnetStateStruct>
			("magnetStateStruct", "magnetStateStruct Doc String")
			.add_property("numMags", &magnetStateStruct::numMags)
			.add_property("magNames", &magnetStateStruct::magNames_Py)
			.add_property("psuStates", &magnetStateStruct::psuStates_Py)
			.add_property("setiValues", &magnetStateStruct::setiValues_Py)
			.add_property("readiValues", &magnetStateStruct::readiValues_Py)
			;
	};


	void expose_magnet_object() {

		// magnet exposure
		boost::python::class_<Magnet, boost::python::bases<Hardware>, boost::noncopyable>("Magnet", boost::python::no_init)
			.add_property("SETI", &Magnet::getSETI, &Magnet::SETI)
			.add_property("psu_state", &Magnet::getPSUState, &Magnet::setPSUState)
			.add_property("READI", &Magnet::getREADI)
			.add_property("name", &Magnet::getHardwareName)
			.add_property("manufacturer", &Magnet::getManufacturer)
			.add_property("serial_number", &Magnet::getSerialNumber)
			.add_property("magnet_type", &Magnet::getMagnetType)
			//.add_property("epicsInterface", &Magnet::epicsInterface)
			.def("switchOff", &Magnet::switchOFF)
			.def("switchOn", &Magnet::switchOn)
			.def("getSETI", &Magnet::getSETI)
			.def("SETI", &Magnet::SETI)
			.def("getREADI", &Magnet::getREADI)
			
			.def("degauss", &Magnet::degauss)
			
			.def("debugMessagesOn", &Magnet::debugMessagesOn)
			.def("debugMessagesOff", &Magnet::debugMessagesOff)
			.def("messagesOn", &Magnet::messagesOn)
			.def("messagesOff", &Magnet::messagesOff)
			;

	}

	//typedef std::pair<int, int> IntPair;
	//// This is our previous registratiOn functiOn (as above) using the cleaner typedef.
	//to_PYTHON_cOnverter<IntPair, PairToTupleCOnverter<int, int> >();
	//// Registers a cOnverter for a pair of int-pairs.
	//to_PYTHON_cOnverter<std::pair<IntPair, IntPair>, PairToTupleCOnverter<IntPair, IntPair> >();




	
	void expose_magnet_factory_object() {

		//Magnet Factory Exposure
		STATE(MagnetFactory:: * turnOnSingle)(const std::string&) = &MagnetFactory::switchOn;
		STATE(MagnetFactory:: * turnOffSingle)(const std::string&) = &MagnetFactory::switchOFF;
		
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
			
			.def("readDBURT", &MagnetFactory::readDBURT)
			.def("writeDBURT", &MagnetFactory::writeDBURT)
			
			
			//.def("getAllMagnetCurrents", &MagnetFactory::getAllMagnetCurrents_Py)
			
			//.def("SETI", &MagnetFactory::SETI)
			.def("SETI", SETISingle)
			
			.def("getRICurrent", &MagnetFactory::getRICurrent)
			.def("getRICurrents", &MagnetFactory::getRICurrents_Py)
			
			.def("turnOn", turnOnSingle)
			.def("turnOn", &MagnetFactory::switchOn_Py)
			.def("turnOff", turnOffSingle)
			.def("turnOff", &MagnetFactory::switchOFF_Py)

			.def("getAllPSUState", &MagnetFactory::getAllPSUState_Py)

			.def("debugMessagesOn", &MagnetFactory::debugMessagesOn)
			.def("debugMessagesOff", &MagnetFactory::debugMessagesOff)
			.def("messagesOn", &MagnetFactory::messagesOn)
			.def("messagesOff", &MagnetFactory::messagesOff)
			;
		//.def("debugMessagesOn", &MagnetFactory::debugMessagesOn)
		//.def("debugMessagesOff", &MagnetFactory::debugMessagesOff)
		//.def("messagesOn", &MagnetFactory::messagesOn)
		//.def("messagesOff", &MagnetFactory::messagesOff);
	

	}


}
#endif