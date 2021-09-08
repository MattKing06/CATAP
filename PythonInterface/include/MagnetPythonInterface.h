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
	void expose_magnet_object() 
	{
		bool(Magnet::*isREADIequalValue_ml_tolerance)(const double)const = &Magnet::isREADIequalValue;
		bool(Magnet::*isREADIequalValue_passed_tolerance)(const double, const double)const = &Magnet::isREADIequalValue;


		bool(Magnet::*degauss_user_deg_values)(const boost::python::list&,  double) = &Magnet::degauss;
		bool(Magnet::*degauss_user_end_values)( double) = &Magnet::degauss;
		bool(Magnet::*degauss_to_zero)(const bool) = &Magnet::degauss;



		bool is_registered = (0 != boost::python::converter::registry::query(boost::python::type_id<Magnet>())->to_python_target_type());
		if (is_registered) return;
		// magnet exposure
		boost::python::class_<Magnet, boost::python::bases<Hardware>, 
			boost::noncopyable>("Magnet", boost::python::no_init)
			// Note some variables are exposed as properties and with functions 
			// TODO make these complete 
			.add_property("SETI", &Magnet::getSETI, &Magnet::SETI)
			.add_property("K_SET_P", &Magnet::getKSetP, &Magnet::setKSetP)

			.def("K_DIP_P", &Magnet::getKDipP)
			.def("INT_STR_MM", &Magnet::getIntStr_mm)
			.def("INT_STR", &Magnet::getIntStr)
			.def("K_ANG", &Magnet::getKAng)
			.def("K_VAL", &Magnet::getKVal)
			.def("K_MRAD", &Magnet::getKmrad)

			.add_property("psu_state", &Magnet::getPSUState, &Magnet::setPSUState)
			.add_property("READI", &Magnet::getREADI)
			.add_property("SETI", &Magnet::getSETI)
			.add_property("name", &Magnet::getHardwareName)
			.add_property("manufacturer", &Magnet::getManufacturer)
			.add_property("serial_number", &Magnet::getSerialNumber)
			.add_property("min_i", &Magnet::getMinI)
			.add_property("max_i", &Magnet::getMaxI)
			.add_property("is_degaussing", &Magnet::isDegaussing)
			.add_property("position", &Magnet::getPosition)
			.add_property("magnetic_length", &Magnet::getMagneticLength)
			.add_property("mag_type", &Magnet::getMagType)
			.add_property("magnet_type", &Magnet::getMagnetType)
			.add_property("position", &Magnet::getPosition)
			.add_property("magnetic_length", &Magnet::getMagneticLength)
			//.add_property("epicsInterface", &Magnet::epicsInterface)
			.def("switchOff", &Magnet::switchOff)
			.def("switchOn", &Magnet::switchOn)
			.def("setPSUState", &Magnet::setPSUState)
			.def("getPSUState", &Magnet::getPSUState)
			.def("getSETI", &Magnet::getSETI)
			.def("SETIZero", &Magnet::SETIZero)
			.def("SETI", &Magnet::SETI)
			.def("getFieldIntegralCoefficients", &Magnet::getFieldIntegralCoefficients_Py)
			.def("getREADI", &Magnet::getREADI)
			.def("getREADITolerance", &Magnet::getREADITolerance)
			.def("setREADITolerance", &Magnet::setREADITolerance)
			.def("getMinI", &Magnet::getMinI)
			.def("getMaxI", &Magnet::getMaxI)
			
			
			.def("degauss", degauss_user_deg_values,
				(boost::python::arg("self"), boost::python::arg("degauss_values"), boost::python::arg("reset_value")=0.0))
			.def("degauss", degauss_user_end_values,
				(boost::python::arg("self"), boost::python::arg("reset_value") = 0.0))
			.def("degauss", degauss_to_zero, (boost::python::arg("self"), boost::python::arg("reset_to_zero")))



			.def("isDegaussing", &Magnet::isDegaussing)
			.def("getDegaussValues", &Magnet::getDegaussValues_Py)
			.def("getDegaussTolerance", &Magnet::getDegaussTolerance)
			.def("getFieldIntegralCoefficients", &Magnet::getFieldIntegralCoefficients_Py)
			.def("setDegaussValues", &Magnet::setDegaussValues_Py)
			.def("setDegaussTolerance", &Magnet::setDegaussTolerance)
			.def("getLastDegaussSuccess", &Magnet::getLastDegaussSuccess)


			.def("setREADITolerance", &Magnet::setREADITolerance)
			.def("offlineSetILKState", &Magnet::offlineSetIlkState)
			.def("offlineSetILKState", &Magnet::getFullPSUName)
			.def("getMagnetType", &Magnet::getMagnetType)
			.def("getFullPSUName", &Magnet::getFullPSUName)

			.def("resetILK", &Magnet::resetILK)

			.def("debugMessagesOff", &Magnet::debugMessagesOff)

			//.def("setMagnetSnapshot", &Magnet::setMagnetSnapshot)
			//.def("getMagnetSnapshot", &Magnet::getMagnetSnapshot)
			
			.def("isREADIequalValue", isREADIequalValue_ml_tolerance)
			.def("isREADIequalValue", isREADIequalValue_passed_tolerance)
			
			.def("matchesSnapshot", &Magnet::matchesSnapshot)
			//.def("isInSETIandPSUState", &Magnet::isInSETIandPSUState)
			.def("getManufacturer", &Magnet::getManufacturer)
			.def("getMagneticLength", &Magnet::getMagneticLength)
			.def("getMagneticLength", &Magnet::getMagneticLength)
			
			.def("getAliases", &Magnet::getAliases_Py)
			.def("messagesOn", &Magnet::messagesOn)
			.def("messagesOff", &Magnet::messagesOff)
			;
	}

	//typedef std::pair<int, int> IntPair;
	//// This is our previous registratiOn functiOn (as above) using the cleaner typedef.
	//to_PYTHON_cOnverter<IntPair, PairToTupleCOnverter<int, int> >();
	//// Registers a cOnverter for a pair of int-pairs.
	//to_PYTHON_cOnverter<std::pair<IntPair, IntPair>, PairToTupleCOnverter<IntPair, IntPair> >();




	
	void expose_magnet_factory_object() 
	{

		bool is_registered = (0 != boost::python::converter::registry::query(boost::python::type_id<MagnetFactory>())->to_python_target_type());
		if (is_registered) return;



		//Magnet Factory Exposure
		STATE(MagnetFactory::*swtichOnSingle)(const std::string&) = &MagnetFactory::switchOn;
		STATE(MagnetFactory::*switchOffSingle)(const std::string&) = &MagnetFactory::switchOff;

		STATE(MagnetFactory::*SETI_single)(const std::string&, const double&) = &MagnetFactory::SETI;
		//void(MagnetFactory::*SETIMultiple)(const std::string&, const double&) = &MagnetFactory::SETI;


		STATE(MagnetFactory::*saveSnapshot_nofile)(const std::string&) = &MagnetFactory::saveSnapshot;
		STATE(MagnetFactory::*saveSnapshot_withfile)(const std::string&, const std::string&, const std::string&) = &MagnetFactory::saveSnapshot;
		STATE(MagnetFactory::*applySnaphot_withDict)(const boost::python::dict&, TYPE) = &MagnetFactory::applySnaphot;
		STATE(MagnetFactory::*applySnaphot_withfile)(const std::string&, const std::string&, TYPE) = &MagnetFactory::applySnaphot;



		std::string(MagnetFactory::*getManufacturer_single)(const std::string&)const = &MagnetFactory::getManufacturer;
		//int(MagnetFactory::*getserial_number_single)(const std::string&)const = &MagnetFactory::getserial_number;
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

		bool(MagnetFactory::*degauss_single_bool)(const std::string &, const bool)= &MagnetFactory::degauss;
		bool(MagnetFactory::*degauss_single_to_Value)(const std::string &, const double)= &MagnetFactory::degauss;

		// OVERLOADED SETUP FUNCTIONS TO ALLOW USER FULL CONTROL AND "FUTURE PROOVED VERSION PARAMETER" 
		bool (MagnetFactory::*setup_NoArg)() = &MagnetFactory::setup;
		bool (MagnetFactory::*setup_VersionArg)(const std::string&) = &MagnetFactory::setup;
		bool (MagnetFactory::*setup_TypeArg)(TYPE)= &MagnetFactory::setup;
		bool (MagnetFactory::*setup_VersionTypeArg)(const std::string&, TYPE  )= &MagnetFactory::setup;
		bool (MagnetFactory::*setup_ListArg)(const boost::python::list&) = &MagnetFactory::setup;
		bool (MagnetFactory::*setup_VersionListArg)(const std::string&, const boost::python::list&) = &MagnetFactory::setup;

		STATE(MagnetFactory::*setKSetP_name)(const std::string & name, const double value) = &MagnetFactory::setKSetP;
		//STATE(MagnetFactory:: * setKSetP_names)(const std::vector<std::string> & names, const double value) = &MagnetFactory::setKSetP;
		//STATE MagnetFactory::setKSetP(const std::string & name, const double value)
		//std::map<std::string, STATE> MagnetFactory::setKSetP(const std::vector<std::string> & names, const double value)

		/*TODO NEED constRUCTOR THAT TAKES VERSION??*/
		//MagnetFactory(std::string VERSION);


		boost::python::class_<MagnetFactory, boost::noncopyable>("MagnetFactory", boost::python::no_init)
			//.def(boost::python::init<STATE>())
			//.def(boost::python::init<STATE, TYPE>())
			//.def(boost::python::init<STATE, boost::python::list>())
			.def(boost::python::init<STATE>())
			.def(boost::python::init<STATE, const std::string>())
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
			.def("getIntStr_mm", &MagnetFactory::getIntStrs_mm_Py)
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
			.def("getAllDipoleKAng", &MagnetFactory::getAllDipoleKAng_Py)



			.def("getKmrad", &MagnetFactory::getKmrad)
			.def("getKmrad", &MagnetFactory::getKmrad_Py)
			.def("getKmrad", &MagnetFactory::getAllKmrad_Py)
			.def("getAllCorrectorKmrad", &MagnetFactory::getAllCorrectorKmrad_Py)
			.def("getAllHCorrectorKmrad", &MagnetFactory::getAllHCorrKmrad_Py)
			.def("getAllVCorrectorKmrad", &MagnetFactory::getAllVCorrKmrad_Py)


			.def("getKVal", &MagnetFactory::getKVal)
			.def("getKVal", &MagnetFactory::getKVals_Py)
			.def("getKVal", &MagnetFactory::getAllKVal_Py)
			.def("getAllQuadKVals", &MagnetFactory::getAllQuadKVals_Py)


			.def("getAllMagnetNames",&MagnetFactory::getAllMagnetNames_Py)
			.def("getAllVCorrectorNames",&MagnetFactory::getAllVCorrectorNames_Py)
			.def("getAllHCorrectorNames",&MagnetFactory::getAllHCorrectorNames_Py)
			.def("getAllCorrectorNames",&MagnetFactory::getAllCorrectorNames_Py)
			.def("getAllQuadNames",&MagnetFactory::getAllQuadNames_Py)
			.def("getAllSolNames",&MagnetFactory::getAllSolNames_Py)
			.def("getAllDipoleNames",&MagnetFactory::getAllDipoleNames_Py)


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
		
			.def("degauss", degauss_single_bool)
			.def("degauss", degauss_single_to_Value)
			.def("degauss", &MagnetFactory::degauss_Py)



			.def("resetILK", &MagnetFactory::resetILK)
			.def("resetAllILK", &MagnetFactory::resetAllILK)

			
			.def("getAliases", &MagnetFactory::getAliases_Py1)
			.def("getAliases", &MagnetFactory::getAliases_Py2)
			

			.def("getManufacturer", getManufacturer_single)
			.def("getManufacturer_Py", &MagnetFactory::getManufacturer_Py)

			//.def("getserial_number", getserial_number_single)
			//.def("getserial_number", &MagnetFactory::getserial_number_Py)

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
			

			.def("saveSnapshot", &MagnetFactory::saveSnapshot_Pydict, 
				(boost::python::arg("self"), boost::python::arg("snapshot_dict"), boost::python::arg("comments") = ""))
			.def("saveSnapshot", &MagnetFactory::saveSnapshot_Pyfile,
				(boost::python::arg("self"), boost::python::arg("filepath"), boost::python::arg("filename"), boost::python::arg("snapshot_dict"), boost::python::arg("comments") = ""))
			.def("saveSnapshot", saveSnapshot_nofile,  (boost::python::arg("self"), boost::python::arg("comments") = ""))
			.def("saveSnapshot", saveSnapshot_withfile,  
				(boost::python::arg("self"), boost::python::arg("filepath"), boost::python::arg("filename"), boost::python::arg("comments") = ""))
			.def("getSnapshot", &MagnetFactory::getSnapshot_Py, (boost::python::args("self")))
			.def("getSnapshot", &MagnetFactory::getSnapshotFromFile_Py, (boost::python::args("self"), boost::python::args("filepath"), boost::python::args("filename")))
			.def("loadSnapshot", &MagnetFactory::loadSnapshot, (boost::python::args("self", "filepath","filename")))
			.def("loadSnapshot", &MagnetFactory::loadSnapshot_Py, (boost::python::args("self", "snapshot_dict")))
			.def("applySnaphot", applySnaphot_withDict, (boost::python::arg("self"), boost::python::arg("snapshot_dict"), boost::python::arg("magnet_type") = TYPE::MAGNET))
			.def("applySnaphot", applySnaphot_withfile, 
				(boost::python::arg("self"), boost::python::arg("filepath"), boost::python::arg("filename"), boost::python::arg("magnet_type") = TYPE::MAGNET))
			.def("checkLastAppliedSnapshot", &MagnetFactory::checkLastAppliedSnapshot, (boost::python::args("self")))

				
			.def("getSetWrongSETI", &MagnetFactory::getSetWrongSETI_Py, (boost::python::args("self")))
			.def("getSetWrongPSU", &MagnetFactory::getSetWrongPSU_Py, (boost::python::args("self")))
			.def("getSetWrongPSU", &MagnetFactory::getSetWrongPSU_Py, (boost::python::args("self")))
			.def("checkLastAppliedSnapshotReturnResults", &MagnetFactory::checkLastAppliedSnapshotReturnResults_Py, 
				(boost::python::arg("self"), boost::python::arg("magnet_type") = TYPE::MAGNET))

			.def("getDefaultMagnetSnapshotPath", &MagnetFactory::getDefaultMagnetSnapshotPath, (boost::python::args("self")))
			.def("getDefaultMachineSnapshotPath", &MagnetFactory::getDefaultMachineSnapshotPath, (boost::python::args("self")))




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