#ifndef MAGNET_PYTHON_INTERFACE_H_
#define MAGNET_PYTHON_INTERFACE_H_
#include "Magnet.h"
#include "MagnetFactory.h"
#include "PythonTypeConversions.h"

#include <boost/python.hpp>

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
	void expose_magnet_state_struct_and_dburt() {
 
		boost::python::class_<magnetStateStruct>
			("magnetStateStruct", "magnetStateStruct Doc String")
			.add_property("numMags", &magnetStateStruct::numMags)
			.add_property("magNames", &magnetStateStruct::magNames_Py)
			.add_property("psuStates", &magnetStateStruct::psuStates_Py)
			.add_property("setiValues", &magnetStateStruct::setiValues_Py)
			.add_property("readiValues", &magnetStateStruct::readiValues_Py)
			;
		boost::python::class_<dburt>
			("dburt", "dburt Doc String")
			.add_property("comment", &dburt::comment)
			.add_property("timestamp", &dburt::timestamp)
			.add_property("magnetstates", &dburt::magnetstates)
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
			.def("switchOff", &Magnet::switchOff)
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
		STATE(MagnetFactory::*turnOnSingle)(const std::string&) = &MagnetFactory::switchOn;
		STATE(MagnetFactory::*turnOffSingle)(const std::string&) = &MagnetFactory::switchOff;
		
		STATE(MagnetFactory::*SETISingle)(const std::string&, const double&) = &MagnetFactory::SETI;
		//void(MagnetFactory::*SETIMultiple)(const std::string&, const double&) = &MagnetFactory::SETI;


		bool(MagnetFactory::*writeDBURT_file)(const std::string&)const = &MagnetFactory::writeDBURT;
		bool(MagnetFactory::*writeDBURT_file_comment)(const std::string&, const std::string&)const = &MagnetFactory::writeDBURT;
		bool(MagnetFactory::*writeDBURT_path_file_comment)(const std::string&, const std::string&, const std::string&)const = &MagnetFactory::writeDBURT;

		dburt(MagnetFactory::*readDBURTT_file)(const std::string&)const = &MagnetFactory::readDBURT;
		dburt(MagnetFactory::*readDBURT_path_file)(const std::string&, const std::string&)const = &MagnetFactory::readDBURT;


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
			
		
			.def("getREADI", &MagnetFactory::getREADI)
			.def("getREAD", &MagnetFactory::getREADI_Py)
			.def("getAllREADI", &MagnetFactory::getAllREADI_Py)
			
			.def("getPSUState", &MagnetFactory::getPSUState)
			.def("getPSUState", &MagnetFactory::getPSUState_Py)
			.def("getAllPSUState", &MagnetFactory::getAllPSUState)

			.def("getILKState", &MagnetFactory::getILKState)
			.def("getILKState", &MagnetFactory::getILKState_Py)
			.def("getAllILKState", &MagnetFactory::getAllILKState_Py)

			.def("readDBURT", readDBURTT_file)
			.def("readDBURT", readDBURT_path_file)
			.def("writeDBURT", writeDBURT_file)
			.def("writeDBURT", writeDBURT_file_comment)
			.def("writeDBURT", writeDBURT_path_file_comment)

			//.def("getAllMagnetCurrents", &MagnetFactory::getAllMagnetCurrents_Py)
			//.def("SETI", &MagnetFactory::SETI)
			.def("SETI", SETISingle)
			.def("SETI", &MagnetFactory::SETI_Py)

			.def("turnOn", turnOnSingle)
			.def("turnOn", &MagnetFactory::switchOn_Py)
			.def("turnOff", turnOffSingle)
			.def("turnOff", &MagnetFactory::switchOff_Py)

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