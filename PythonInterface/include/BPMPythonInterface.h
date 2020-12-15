#ifndef BPM_PYTHON_INTERFACE_H_
#define BPM_PYTHON_INTERFACE_H_
#include "BPM.h"
#include "BPMFactory.h"
#include "PythonTypeConversions.h"

#include <boost/python.hpp>

//using namespace boost::PYTHON;
//using namespace boost;
namespace BOOST_PYTHON_BPM_INCLUDE
{
	void expose_bpm_object() {
		bool is_registered = (0 != boost::python::converter::registry::query(boost::python::type_id<BPM>())->to_python_target_type());
		if (is_registered) return;
		// bpm exposure
		boost::python::class_<BPM, boost::python::bases<Hardware>, boost::noncopyable>("BPM", boost::python::no_init)
			.add_property("name", &BPM::getBPMName)
			.add_property("x", &BPM::getXFromPV, &BPM::setXPVVirtual)
			.add_property("y", &BPM::getYFromPV, &BPM::setYPVVirtual)
			.add_property("data", &BPM::getData)
			.add_property("q", &BPM::getQ)
			.add_property("xpvbuffer", &BPM::getXPVBuffer)
			.add_property("ypvbuffer", &BPM::getYPVBuffer)
			.add_property("databuffer", &BPM::getDataBuffer)
			.add_property("qbuffer", &BPM::getQBuffer)
			.add_property("xpvvector", &BPM::getXPVVector)
			.add_property("ypvvector", &BPM::getYPVVector)
			.add_property("datavector", &BPM::getDataVector)
			.add_property("qvector", &BPM::getQVector)
			.add_property("xbuffer", &BPM::getXPVBuffer)
			.add_property("ybuffer", &BPM::getYPVBuffer)
			.add_property("databuffer", &BPM::getDataBuffer)
			.add_property("qbuffer", &BPM::getQBuffer)
			.add_property("position", &BPM::getPosition)
			.add_property("resolution", &BPM::getResolution)
			.add_property("status", &BPM::getStatus)
			.add_property("statusbuffer", &BPM::getStatusBuffer)
			.add_property("statusvector", &BPM::getStatusVector)
			.add_property("ra1", &BPM::getRA1)
			.add_property("ra2", &BPM::getRA2)
			.add_property("rd1", &BPM::getRD1)
			.add_property("rd2", &BPM::getRD2)
			.add_property("sa1", &BPM::getSA1, &BPM::setSA1)
			.add_property("sa2", &BPM::getSA2, &BPM::setSA2)
			.add_property("sd1", &BPM::getSD1, &BPM::setSD1)
			.add_property("sd2", &BPM::getSD2, &BPM::setSD2)
			.add_property("buffersize", &BPM::getBufferSize, &BPM::setBufferSize)
			.def("getXFromPV", &BPM::getXFromPV)
			.def("getYFromPV", &BPM::getYFromPV)
			.def("getData", &BPM::getData)
			.def("getQ", &BPM::getQ)
			.def("getStatus", &BPM::getStatus)
			.def("getXPVBuffer", &BPM::getXPVBuffer)
			.def("getYPVBuffer", &BPM::getYPVBuffer)
			.def("getDataBuffer", &BPM::getDataBuffer)
			.def("getQBuffer", &BPM::getQBuffer)
			.def("getStatusBuffer", &BPM::getStatusBuffer)
			.def("getXPVVector", &BPM::getXPVVector)
			.def("getYPVVector", &BPM::getYPVVector)
			.def("getDataVector", &BPM::getDataVector)
			.def("getQVector", &BPM::getQVector)
			.def("getStatusVector", &BPM::getStatusVector)
			.def("monitorForNShots", &BPM::monitorForNShots)
			.def("ismonitoring", &BPM::ismonitoring)
			.def("ismonitoringXPV", &BPM::ismonitoringXPV)
			.def("ismonitoringYPV", &BPM::ismonitoringYPV)
			.def("ismonitoringData", &BPM::ismonitoringData)
			.def("reCalAttenuation", &BPM::reCalAttenuation)
			.def("isXPVBufferFull", &BPM::isXPVBufferFull)
			.def("isYPVBufferFull", &BPM::isYPVBufferFull)
			.def("isDataBufferFull", &BPM::isDataBufferFull)
			.def("getRA1", &BPM::getRA1)
			.def("getRA2", &BPM::getRA2)
			.def("getRD1", &BPM::getRD1)
			.def("getRD2", &BPM::getRD2)
			.def("getSA1", &BPM::getSA1)
			.def("getSA2", &BPM::getSA2)
			.def("getSD1", &BPM::getSD1)
			.def("getSD2", &BPM::getSD2)
			.def("setSA1", &BPM::setSA1)
			.def("setSA2", &BPM::setSA2)
			.def("setSD1", &BPM::setSD1)
			.def("setSD2", &BPM::setSD2);

	}

	//typedef std::pair<int, int> IntPair;
	//// This is our previous registratiOn functiOn (as above) using the cleaner typedef.
	//to_PYTHON_cOnverter<IntPair, PairToTupleCOnverter<int, int> >();
	//// Registers a cOnverter for a pair of int-pairs.
	//to_PYTHON_cOnverter<std::pair<IntPair, IntPair>, PairToTupleCOnverter<IntPair, IntPair> >();




	
	void expose_bpm_factory_object() {

		bool is_registered = (0 != boost::python::converter::registry::query(boost::python::type_id<BPMFactory>())->to_python_target_type());
		if (is_registered) return;
		//bpm Factory Exposure
		boost::python::class_<BPMFactory>("BPMFactory", boost::python::no_init)
			.def(boost::python::init<STATE>())
			.def("setup", &BPMFactory::setup)
			.add_property("bpmMap", &BPMFactory::bpmMap)
			.def("getBPM", &BPMFactory::getBPM, boost::python::return_value_policy<boost::python::reference_existing_object>())
			.def("getBPMs", &BPMFactory::getBPMs)
			.def("getAllBPMs", &BPMFactory::getAllBPMs)
			.def("getRA1", &BPMFactory::getRA1)
			.def("getRA2", &BPMFactory::getRA2)
			.def("getRD1", &BPMFactory::getRD1)
			.def("getRD2", &BPMFactory::getRD2)
			.def("getSA1", &BPMFactory::getSA1)
			.def("getSA2", &BPMFactory::getSA2)
			.def("getSD1", &BPMFactory::getSD1)
			.def("getSD2", &BPMFactory::getSD2)
			.def("setSA1", &BPMFactory::setSA1)
			.def("setSA2", &BPMFactory::setSA2)
			.def("setSD1", &BPMFactory::setSD1)
			.def("setSD2", &BPMFactory::setSD2)
			.def("monitorForNShots", &BPMFactory::monitorForNShots)
			.def("ismonitoringXPV", &BPMFactory::ismonitoringXPV)
			.def("ismonitoringYPV", &BPMFactory::ismonitoringYPV)
			.def("ismonitoringData", &BPMFactory::ismonitoringData)
			.def("ismonitoring", &BPMFactory::ismonitoring)
			.def("getPosition", &BPMFactory::getPosition)
			.def("getResolution", &BPMFactory::getResolution)
			.def("getX", &BPMFactory::getX)
			.def("getXFromPV", &BPMFactory::getXFromPV)
			.def("getY", &BPMFactory::getY)
			.def("getYFromPV", &BPMFactory::getYFromPV)
			.def("getQ", &BPMFactory::getQ)
			.def("getStatus", &BPMFactory::getStatus)
			.def("getData", &BPMFactory::getData_Py)
			.def("getXYPosition", &BPMFactory::getXYPosition)
			.def("getAllBPMNames", &BPMFactory::getAllBPMNames_Py)
			.def("getXPVVector", &BPMFactory::getXPVVector_Py)
			.def("getYPVVector", &BPMFactory::getYPVVector_Py)
			.def("getQVector", &BPMFactory::getQVector_Py)
			.def("getStatusVector", &BPMFactory::getStatusVector_Py)
			.def("getDataVector", &BPMFactory::getDataVector_Py)
			.def("getXPVBuffer", &BPMFactory::getXPVBuffer_Py)
			.def("getYPVBuffer", &BPMFactory::getYPVBuffer_Py)
			.def("getQBuffer", &BPMFactory::getQBuffer_Py)
			.def("getStatusBuffer", &BPMFactory::getStatusBuffer_Py)
			.def("getDataBuffer", &BPMFactory::getDataBuffer_Py)
			//.def("getXYPositiOnVector", &BPMFactory::getXYPositiOnVector_Py)
			.def("getResolutions", &BPMFactory::getResolutions_Py)
			.def("getPositions", &BPMFactory::getPositions_Py)
			.def("getXPVVectors", &BPMFactory::getXPVVectors_Py)
			.def("getYPVVectors", &BPMFactory::getYPVVectors_Py)
			.def("getQVectors", &BPMFactory::getQVectors_Py)
			.def("getStatusVectors", &BPMFactory::getStatusVectors_Py)
			.def("getDataVectors", &BPMFactory::getDataVectors_Py)
			.def("getXPVBuffers", &BPMFactory::getXPVBuffers_Py)
			.def("getYPVBuffers", &BPMFactory::getYPVBuffers_Py)
			.def("getQBuffers", &BPMFactory::getQBuffers_Py)
			.def("getStatusBuffers", &BPMFactory::getStatusBuffers_Py)
			.def("getDataBuffers", &BPMFactory::getDataBuffers_Py)
			.def("getAllX", &BPMFactory::getAllX_Py)
			.def("getAllXFromPV", &BPMFactory::getAllXFromPV_Py)
			.def("getAllY", &BPMFactory::getAllY_Py)
			.def("getAllYFromPV", &BPMFactory::getAllYFromPV_Py)
			.def("getAllQ", &BPMFactory::getAllQ_Py)
			.def("getAllStatus", &BPMFactory::getAllStatus_Py)
			.def("getAllData", &BPMFactory::getAllData_Py)
			.def("getAllXYPosition", &BPMFactory::getAllXYPosition_Py)
			.def("getAllResolution", &BPMFactory::getAllResolution_Py)
			.def("getAllPosition", &BPMFactory::getAllPosition_Py)
			.def("getAllXPVVector", &BPMFactory::getAllXPVVector_Py)
			.def("getAllYPVVector", &BPMFactory::getAllYPVVector_Py)
			.def("getAllQVector", &BPMFactory::getAllQVector_Py)
			.def("getAllStatusVector", &BPMFactory::getAllStatusVector_Py)
			.def("getAllDataVector", &BPMFactory::getAllDataVector_Py)
			.def("getAllXPVBuffer", &BPMFactory::getAllXPVBuffer_Py)
			.def("getAllYPVBuffer", &BPMFactory::getAllYPVBuffer_Py)
			.def("getAllQBuffer", &BPMFactory::getAllQBuffer_Py)
			.def("getAllStatusBuffer", &BPMFactory::getAllStatusBuffer_Py)
			.def("getAllDataBuffer", &BPMFactory::getAllDataBuffer_Py)
			.def("getAllXYPositionVectors", &BPMFactory::getAllXYPositionVectors_Py);

	}


}
#endif