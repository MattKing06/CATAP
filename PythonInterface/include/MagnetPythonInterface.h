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
	void expose_magnet_state() {

		bool is_registered = (0 != boost::python::converter::registry::query(boost::python::type_id<magnetState>())->to_python_target_type());
		if (is_registered) return;

		boost::python::class_<magnetState>
			("magnetStateStruct", "magnetStateStruct Doc String")
			.add_property("name", &magnetState::name)
			.add_property("psuState", &magnetState::psuState)
			.add_property("ilkState", &magnetState::ilkState)
			.add_property("seti", &magnetState::seti)
			.add_property("readi", &magnetState::readi)
			;
	};
	void expose_magnet_states() {
 
		bool is_registered = (0 != boost::python::converter::registry::query(boost::python::type_id<magnetStates>())->to_python_target_type());
		if (is_registered) return;
		boost::python::class_<magnetStates>
			("dburt", "dburt Doc String")
			.add_property("numMags", &magnetStates::numMags)
			.add_property("magnetStateMap_Py", &magnetStates::magnetStateMap_Py)
			;
	};

	void expose_magnet_dburt() {

		bool is_registered = (0 != boost::python::converter::registry::query(boost::python::type_id<dburt>())->to_python_target_type());
		if (is_registered) return;
		boost::python::class_<dburt>
			("dburt", "dburt Doc String")
			.add_property("comment", &dburt::comment)
			.add_property("magnetstates", &dburt::magnetstates)
			;
	};

	
	void expose_magnet_object() {

		bool is_registered = (0 != boost::python::converter::registry::query(boost::python::type_id<Magnet>())->to_python_target_type());
		if (is_registered) return;
		// magnet exposure
		boost::python::class_<Magnet, boost::python::bases<Hardware>, 
			boost::noncopyable>("Magnet", boost::python::no_init)
			// Note some variables are exposed as properties and with functions 
			// TODO make these complete 
			.add_property("SETI", &Magnet::getSETI, &Magnet::SETI)
			.add_property("K_SET_P", &Magnet::getKSetP, &Magnet::setKSetP)
			.add_property("psu_state", &Magnet::getPSUState, &Magnet::setPSUState)
			.add_property("READI", &Magnet::getREADI)
			.add_property("name", &Magnet::getHardwareName)
			.add_property("manufacturer", &Magnet::getManufacturer)
			.add_property("serial_number", &Magnet::getSerialNumber)
			.add_property("min_i", &Magnet::getMinI)
			.add_property("max_i", &Magnet::getMaxI)
			.add_property("is_degaussing", &Magnet::isDegaussing)
			.add_property("position", &Magnet::getPosition)
			.add_property("magnetic_length", &Magnet::getMagneticLength)
			.add_property("mag_type", &Magnet::getMagType)
			//.add_property("epicsInterface", &Magnet::epicsInterface)
			.def("switchOff", &Magnet::switchOff)
			.def("switchOn", &Magnet::switchOn)
			.def("setPSUState", &Magnet::setPSUState)
			.def("getSETI", &Magnet::getSETI)
			.def("SETIZero", &Magnet::SETIZero)
			.def("getREADI", &Magnet::getREADI)
			.def("getREADITolerance", &Magnet::getREADITolerance)
			.def("setREADITolerance", &Magnet::setREADITolerance)
			.def("getMinI", &Magnet::getMinI)
			.def("getMaxI", &Magnet::getMaxI)
			.def("degauss", &Magnet::degauss)
			.def("isDegaussing", &Magnet::isDegaussing)
			.def("getDegaussValues", &Magnet::getDegaussValues_Py)
			.def("getDegaussTolerance", &Magnet::getDegaussTolerance)
			.def("getFieldIntegralCoefficients", &Magnet::getFieldIntegralCoefficients_Py)
			.def("setDegaussValues", &Magnet::setDegaussValues_Py)
			.def("setDegaussTolerance", &Magnet::setDegaussTolerance)
			.def("setREADITolerance", &Magnet::setREADITolerance)
			.def("offlineSetILKState", &Magnet::offlineSetILKState)
			.def("offlineSetILKState", &Magnet::getFullPSUName)
			.def("getMagnetType", &Magnet::getMagnetType)
			.def("getFullPSUName", &Magnet::getFullPSUName)
			.def("debugMessagesOff", &Magnet::debugMessagesOff)
			.def("setMagnetState", &Magnet::setMagnetState)
			.def("getMagnetState", &Magnet::getMagnetState)
			.def("isInState", &Magnet::isInState)
			.def("isInSETIandPSUState", &Magnet::isInSETIandPSUState)
			.def("getManufacturer", &Magnet::getManufacturer)
			.def("getMagneticLength", &Magnet::getMagneticLength)
			.def("getMagneticLength", &Magnet::getMagneticLength)
			.def("getAliases", &Magnet::getAliases_Py)
			.def("messagesOn", &Magnet::messagesOn)
			.def("messagesOff", &Magnet::messagesOff)
			
			
			.def("K_DIP_P", &Magnet::getKDipP)
			.def("INT_STR_MM", &Magnet::getIntStr_mm)
			.def("INT_STR", &Magnet::getIntStr)

			.def("K_ANG", &Magnet::getKAng)
			.def("K_VAL", &Magnet::getKVal)
			.def("K_MRAD", &Magnet::getKmrad)


	

			;

	}

	//typedef std::pair<int, int> IntPair;
	//// This is our previous registratiOn functiOn (as above) using the cleaner typedef.
	//to_PYTHON_cOnverter<IntPair, PairToTupleCOnverter<int, int> >();
	//// Registers a cOnverter for a pair of int-pairs.
	//to_PYTHON_cOnverter<std::pair<IntPair, IntPair>, PairToTupleCOnverter<IntPair, IntPair> >();




	
	void expose_magnet_factory_object() {

		bool is_registered = (0 != boost::python::converter::registry::query(boost::python::type_id<MagnetFactory>())->to_python_target_type());
		if (is_registered) return;
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
		TYPE(MagnetFactory::*getMagnetType_single)(const std::string&)const = &MagnetFactory::getMagnetType;
		std::string(MagnetFactory::*getMagnetRevType_single)(const std::string&)const = &MagnetFactory::getMagnetRevType;
		std::string(MagnetFactory::*getFullPSUName_single)(const std::string&)const = &MagnetFactory::getFullPSUName;
		std::string(MagnetFactory::*getMeasurementDataLocation_single)(const std::string&)const = &MagnetFactory::getMeasurementDataLocation;
		double (MagnetFactory::*getMagneticLength_single)(const std::string&)const = &MagnetFactory::getMagneticLength;
		double (MagnetFactory::*getDegaussTolerance_single)(const std::string&)const = &MagnetFactory::getDegaussTolerance;
		double (MagnetFactory::*getREADITolerance_single)(const std::string&)const = &MagnetFactory::getREADITolerance;
		double (MagnetFactory::*setDegaussTolerance_single)(const std::string&, const double) = &MagnetFactory::setDegaussTolerance;
		double (MagnetFactory::*setREADITolerance_single)(const std::string&, const double) = &MagnetFactory::setREADITolerance;
		size_t(MagnetFactory::*getNumberOfDegaussSteps_single)(const std::string&)const = &MagnetFactory::getNumberOfDegaussSteps;




		//Magnet& getMagnet(const std::string & fullMagnetName);

		bool(MagnetFactory::*degauss_single)(const std::string &, const bool)= &MagnetFactory::degauss;

		// OVERLOADED SETUP FUNCTIONS TO ALLOW USER FULL CONTROL AND "FUTURE PROOVED VERSION PARAMETER" 
		bool (MagnetFactory::* setup_NoArg)() = &MagnetFactory::setup;
		bool (MagnetFactory::* setup_VersionArg)(const std::string&) = &MagnetFactory::setup;
		bool (MagnetFactory::* setup_TypeArg)(TYPE)= &MagnetFactory::setup;
		bool (MagnetFactory::* setup_VersionTypeArg)(const std::string&, TYPE  )= &MagnetFactory::setup;
		bool (MagnetFactory::* setup_ListArg)(const boost::python::list&) = &MagnetFactory::setup;
		bool (MagnetFactory::* setup_VersionListArg)(const std::string&, const boost::python::list&) = &MagnetFactory::setup;

		bool (MagnetFactory::* applyDBURT_filename)(const std::string&) = &MagnetFactory::applyDBURT;
		bool (MagnetFactory::* applyDBURTQuadOnly_filename)(const std::string&) = &MagnetFactory::applyDBURTQuadOnly;
		bool (MagnetFactory::* applyDBURTCorOnly_filename)(const std::string&) = &MagnetFactory::applyDBURTCorOnly;
		bool (MagnetFactory::* applyDBURT_filepath_filename)(const std::string&, const std::string&) = &MagnetFactory::applyDBURT;
		bool (MagnetFactory::* applyDBURTQuadOnly_filepath_filename)(const std::string&, const std::string&) = &MagnetFactory::applyDBURTQuadOnly;
		bool (MagnetFactory::* applyDBURTCorOnly_filepath_filename)(const std::string&, const std::string&) = &MagnetFactory::applyDBURTCorOnly;


		// woot fuctcion pointers 
		STATE(MagnetFactory:: * setKSetP_name)(const std::string & name, const double value) = &MagnetFactory::setKSetP;
		//STATE(MagnetFactory:: * setKSetP_names)(const std::vector<std::string> & names, const double value) = &MagnetFactory::setKSetP;
		//STATE MagnetFactory::setKSetP(const std::string & name, const double value)
		//std::map<std::string, STATE> MagnetFactory::setKSetP(const std::vector<std::string> & names, const double value)

		/*NEED constRUCTOR THAT TAKES VERSION??*/
		//MagnetFactory(std::string VERSION);




		boost::python::class_<MagnetFactory, boost::noncopyable>("MagnetFactory", boost::python::no_init)
			//.def(boost::python::init<STATE>())
			//.def(boost::python::init<STATE, TYPE>())
			//.def(boost::python::init<STATE, boost::python::list>())
			.def("setup", setup_NoArg)
			.def("setup", setup_VersionArg)
			.def("setup", setup_TypeArg)
			.def("setup", setup_VersionTypeArg)
			.def("setup", setup_ListArg)
			.def("setup", setup_VersionListArg)
			//.add_property("magnetMap", &MagnetFactory::magnetMap)
			.def("getMagnet", &MagnetFactory::getMagnet, boost::python::return_value_policy<boost::python::reference_existing_object>())
			// NO you can't do this is you can't create a map of Magnet references 
			//.def("getMagnets", &MagnetFactory::getMagnets_Py, boost::python::return_value_policy<boost::python::reference_existing_object>())
			
			.def("setKSetP",setKSetP_name)
			.def("setKSetP",&MagnetFactory::setKSetP_Py)
			.def("setKSetP",&MagnetFactory::setKSetP_area_Py)
			.def("setKSetP",&MagnetFactory::setKSetP_Areas_py)


			.def("getKDipP", &MagnetFactory::getKDipP)
			.def("getKDipP", &MagnetFactory::getKDipPs_Py)
			.def("getKDipP", &MagnetFactory::getAllKDipP_Py)

			.def("getIntStr_mm", &MagnetFactory::getIntStr_mm)
			.def("getIntStr_mm", &MagnetFactory::getIntStr_mms_Py)
			.def("getIntStr_mm", &MagnetFactory::getAllIntStr_mm_Py)

			.def("getIntStr", &MagnetFactory::getIntStr)
			.def("getIntStr", &MagnetFactory::getIntStrs_Py)
			.def("getIntStr", &MagnetFactory::getAllIntStr_Py)


			.def("getKSetP", &MagnetFactory::getKSetP)
			.def("getKSetP", &MagnetFactory::getKSetPs_Py)
			.def("getKSetP", &MagnetFactory::getAllKSetP_Py)

			.def("getKAng", &MagnetFactory::getKAng)
			.def("getKAng", &MagnetFactory::getKAngs_Py)
			.def("getKAng", &MagnetFactory::getAllKAng_Py)



			.def("getKmrad", &MagnetFactory::getKmrad)
			.def("getKmrad", &MagnetFactory::getKmrad_Py)
			.def("getKmrad", &MagnetFactory::getAllKmrad_Py)


			.def("getKVal", &MagnetFactory::getKVal)
			.def("getKVal", &MagnetFactory::getKVals_Py)
			.def("getKVal", &MagnetFactory::getAllKVal_Py)


			.def("getAllMagnetNames",&MagnetFactory::getAllMagnetNames_Py)

			// for backwards compatability with VC_Controllers
			.def("getMagnetNames",&MagnetFactory::getAllMagnetNames_Py)
					
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

			.def("isAType", &MagnetFactory::isAType)
			.def("isAQuad", &MagnetFactory::isAQuad)
			.def("isADip", &MagnetFactory::isADip)
			.def("isASol", &MagnetFactory::isASol)
			.def("isABSol", &MagnetFactory::isABSol)
			.def("isAVCor", &MagnetFactory::isAVCor)
			.def("isAHCor", &MagnetFactory::isAHCor)
			.def("isACor", &MagnetFactory::isACor)


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
			
			.def("applyDBURT",			applyDBURT_filename)
			.def("applyDBURTQuadOnly",	applyDBURTQuadOnly_filename)
			.def("applyDBURTCorOnly",	applyDBURTCorOnly_filename)

			.def("applyDBURT",			applyDBURT_filepath_filename)
			.def("applyDBURTQuadOnly",	applyDBURTQuadOnly_filepath_filename)
			.def("applyDBURTCorOnly",	applyDBURTCorOnly_filepath_filename)


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