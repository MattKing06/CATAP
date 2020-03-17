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
 
		//boost::python::class_<magnetStateStruct>
		//	("magnetStateStruct", "magnetStateStruct Doc String")
		//	.add_property("numMags", &magnetStateStruct::numMags)
		//	.add_property("magNames", &magnetStateStruct::magNames_Py)
		//	.add_property("psuStates", &magnetStateStruct::psuStates_Py)
		//	.add_property("setiValues", &magnetStateStruct::setiValues_Py)
		//	.add_property("readiValues", &magnetStateStruct::readiValues_Py)
		//	;
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
		STATE(MagnetFactory::*swtichOnSingle)(const std::string&) = &MagnetFactory::switchOn;
		STATE(MagnetFactory::*switchOffSingle)(const std::string&) = &MagnetFactory::switchOff;

		STATE(MagnetFactory::*SETI_single)(const std::string&, const double&) = &MagnetFactory::SETI;
		//void(MagnetFactory::*SETIMultiple)(const std::string&, const double&) = &MagnetFactory::SETI;


		bool(MagnetFactory::*writeDBURT_file)(const std::string&)const = &MagnetFactory::writeDBURT;
		bool(MagnetFactory::*writeDBURT_file_comment)(const std::string&, const std::string&)const = &MagnetFactory::writeDBURT;
		bool(MagnetFactory::*writeDBURT_path_file_comment)(const std::string&, const std::string&, const std::string&)const = &MagnetFactory::writeDBURT;

		dburt(MagnetFactory::*readDBURTT_file)(const std::string&)const = &MagnetFactory::readDBURT;
		dburt(MagnetFactory::*readDBURT_path_file)(const std::string&, const std::string&)const = &MagnetFactory::readDBURT;


		std::string(MagnetFactory::*getManufacturer_single)(const std::string&)const = &MagnetFactory::getManufacturer;
		//int(MagnetFactory::*getSerialNumber_single)(const std::string&)const = &MagnetFactory::getSerialNumber;
		std::string(MagnetFactory::*getMagnetType_single)(const std::string&)const = &MagnetFactory::getMagnetType;
		std::string(MagnetFactory::*getMagnetRevType_single)(const std::string&)const = &MagnetFactory::getMagnetRevType;
		std::string(MagnetFactory::*getFullPSUName_single)(const std::string&)const = &MagnetFactory::getFullPSUName;
		std::string(MagnetFactory::*getMeasurementDataLocation_single)(const std::string&)const = &MagnetFactory::getMeasurementDataLocation;
		double (MagnetFactory::*getMagneticLength_single)(const std::string&)const = &MagnetFactory::getMagneticLength;
		double (MagnetFactory::*getDegaussTolerance_single)(const std::string&)const = &MagnetFactory::getDegaussTolerance;
		double (MagnetFactory::*getREADITolerance_single)(const std::string&)const = &MagnetFactory::getREADITolerance;
		double (MagnetFactory::*setDegaussTolerance_single)(const std::string&, const double) = &MagnetFactory::setDegaussTolerance;
		double (MagnetFactory::*setREADITolerance_single)(const std::string&, const double) = &MagnetFactory::setREADITolerance;
		size_t(MagnetFactory::*getNumberOfDegaussSteps_single)(const std::string&)const = &MagnetFactory::getNumberOfDegaussSteps;




		Magnet& getMagnet(const std::string & fullMagnetName);
		//boost::python::list getAllMagnetNames_Py()const;

		// getSETI

		bool(MagnetFactory::*degauss_single)(const std::string &, const bool)= &MagnetFactory::degauss;


		boost::python::class_<MagnetFactory, boost::noncopyable>("MagnetFactory", boost::python::no_init)
			.def(boost::python::init<STATE>())
			.def("setup", &MagnetFactory::setup)
			//.add_property("magnetMap", &MagnetFactory::magnetMap)
			.def("getMagnet", &MagnetFactory::getMagnet, boost::python::return_value_policy<boost::python::reference_existing_object>())
			// NO you can't do this is you can't create a map of Magnet references 
			//.def("getMagnets", &MagnetFactory::getMagnets_Py, boost::python::return_value_policy<boost::python::reference_existing_object>())
			
			.def("getAllMagnetNames",&MagnetFactory::getAllMagnetNames_Py)
					
			.def("getPSUState", &MagnetFactory::getPSUState)
			.def("getPSUState", &MagnetFactory::getPSUState_Py)
			.def("getAllPSUState", &MagnetFactory::getAllPSUState)

			.def("getILKState", &MagnetFactory::getILKState)
			.def("getILKState", &MagnetFactory::getILKState_Py)
			.def("getAllILKState", &MagnetFactory::getAllILKState_Py)
			

			.def("getSETI", &MagnetFactory::getSETI)
			.def("getSETIs", &MagnetFactory::getSETIs_Py)
			.def("getAllSETI", &MagnetFactory::getAllSETI_Py)
			
			.def("SETI", SETI_single)
			
			.def("SETI", &MagnetFactory::SETI_Py)
			.def("SETIAllZero", &MagnetFactory::SETIAllZero_Py)
			
			.def("getREADI", &MagnetFactory::getREADI)
			.def("getREAD", &MagnetFactory::getREADI_Py)
			.def("getAllREADI", &MagnetFactory::getAllREADI_Py)
		
			.def("swtichOn", swtichOnSingle)
			.def("swtichOn", &MagnetFactory::switchOn_Py)
			.def("switchOnAll", &MagnetFactory::switchOnAll_Py)
			
			.def("swtichOff", switchOffSingle)
			.def("swtichOff", &MagnetFactory::switchOff_Py)
			.def("switchOffAll", &MagnetFactory::switchOffAll_Py)
		
			.def("degauss", degauss_single)
			.def("degauss", &MagnetFactory::degauss_Py)
			
			.def("getAliases", &MagnetFactory::getAliases_Py1)
			.def("getAliases", &MagnetFactory::getAliases_Py2)
			

			.def("getManufacturer", getManufacturer_single)
			.def("getManufacturer_Py", &MagnetFactory::getManufacturer_Py)

			//.def("getSerialNumber", getSerialNumber_single)
			//.def("getSerialNumber", &MagnetFactory::getSerialNumber_Py)

			.def("getMagnetType", getMagnetType_single)
			.def("getMagnetType", &MagnetFactory::getMagnetType_Py)

			.def("getMagnetRevType", getMagnetRevType_single)
			.def("getMagnetRevType", &MagnetFactory::getMagnetRevType_Py)

			.def("getFullPSUName", getFullPSUName_single)
			.def("getFullPSUName", &MagnetFactory::getFullPSUName_Py)

			.def("getMeasurementDataLocation", getMeasurementDataLocation_single)
			.def("getMeasurementDataLocation", &MagnetFactory::getMeasurementDataLocation_Py)

			.def("getNumberOfDegaussSteps", getNumberOfDegaussSteps_single)
			.def("getNumberOfDegaussSteps", &MagnetFactory::getNumberOfDegaussSteps_Py)


		//boost::python::dict getNumberOfDegaussSteps_Py(const boost::python::list & name) const;
		//boost::python::list getDegaussValues_Py1(const std::string & name) const;
		//boost::python::dict getDegaussValues_Py2(const boost::python::list & name) const;
		//boost::python::dict getDegaussTolerance_Py(const boost::python::list & name) const;
		//boost::python::dict getRITolerance_Py(const boost::python::list & name) const;
		//boost::python::list setDegaussValuesSingle_Py(const std::string & name, const boost::python::list & values);
		//boost::python::dict setDegaussValuesMulti_Py(const boost::python::list & names, const boost::python::list & values);
		//bool SETIZero(const std::string & name);
		//bool setPSUState(const std::string & name, const STATE value);
		//bool offlineSetILKState(const std::string & name, const STATE value);
		//magnetState getMagnetState(const std::string & name)const;
		//magnetStates getMagnetStates() const;
		//bool setMagnetState(const magnetState & ms);
		//dburt readDBURT(const std::string & fileName)const;
		//dburt readDBURT(const std::string & filePath, const std::string & fileName)const;
		//bool readAndApplyDBURT(const std::string & fileName);
		//bool isMagnetStateEqualDBURT(const std::string & fileName);
		//std::string getFullName(const std::string & name_to_check) const;
		


			.def("readDBURT", readDBURTT_file)
			.def("readDBURT", readDBURT_path_file)
			.def("writeDBURT", writeDBURT_file)
			.def("writeDBURT", writeDBURT_file_comment)
			.def("writeDBURT", writeDBURT_path_file_comment)
			.def("readAndApplyDBURT", &MagnetFactory::readAndApplyDBURT)
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