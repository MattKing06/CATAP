#ifndef SCREEN_PYTHON_INTERFACE_H_
#define SCREEN_PYTHON_INTERFACE_H_
#include "Screen.h"
#include "ScreenFactory.h"
#include "PythonTypeConversions.h"

#include <boost/python.hpp>

//using namespace boost::PYTHON;
//using namespace boost;
namespace BOOST_PYTHON_SCREEN_INCLUDE
{
	void expose_screen_object() {

		// bpm exposure
		boost::python::class_<Screen, boost::python::bases<Hardware>, boost::noncopyable>("Screen", boost::python::no_init)
			.add_property("name", &Screen::getScreenName);
			//.add_property("x", &BPM::getXFromPV)
			//.add_property("y", &BPM::getYFromPV)
			//.add_property("data", &BPM::getData)
			//.add_property("q", &BPM::getQ)
			//.add_property("xpvbuffer", &BPM::getXPVBuffer)
			//.add_property("ypvbuffer", &BPM::getYPVBuffer)
			//.add_property("databuffer", &BPM::getDataBuffer)
			//.add_property("qbuffer", &BPM::getQBuffer)
			//.add_property("xpvvector", &BPM::getXPVVector)
			//.add_property("ypvvector", &BPM::getYPVVector)
			//.add_property("datavector", &BPM::getDataVector)
			//.add_property("qvector", &BPM::getQVector)
			//.add_property("xbuffer", &BPM::getXPVBuffer)
			//.add_property("ybuffer", &BPM::getYPVBuffer)
			//.add_property("databuffer", &BPM::getDataBuffer)
			//.add_property("qbuffer", &BPM::getQBuffer)
			//.add_property("position", &BPM::getPosition)
			//.add_property("resolution", &BPM::getResolution)
			//.add_property("status", &BPM::getStatus)
			//.add_property("statusbuffer", &BPM::getStatusBuffer)
			//.add_property("statusvector", &BPM::getStatusVector)
			//.add_property("ra1", &BPM::getRA1)
			//.add_property("ra2", &BPM::getRA2)
			//.add_property("rd1", &BPM::getRD1)
			//.add_property("rd2", &BPM::getRD2)
			//.add_property("sa1", &BPM::getSA1, &BPM::setSA1)
			//.add_property("sa2", &BPM::getSA2, &BPM::setSA2)
			//.add_property("sd1", &BPM::getSD1, &BPM::setSD1)
			//.add_property("sd2", &BPM::getSD2, &BPM::setSD2)
			//.add_property("buffersize", &BPM::getBufferSize, &BPM::setBufferSize)
			//.def("getXFromPV", &BPM::getXFromPV)
			//.def("getYFromPV", &BPM::getYFromPV)
			//.def("getData", &BPM::getData)
			//.def("getQ", &BPM::getQ)
			//.def("getStatus", &BPM::getStatus)
			//.def("getXPVBuffer", &BPM::getXPVBuffer)
			//.def("getYPVBuffer", &BPM::getYPVBuffer)
			//.def("getDataBuffer", &BPM::getDataBuffer)
			//.def("getQBuffer", &BPM::getQBuffer)
			//.def("getStatusBuffer", &BPM::getStatusBuffer)
			//.def("getXPVVector", &BPM::getXPVVector)
			//.def("getYPVVector", &BPM::getYPVVector)
			//.def("getDataVector", &BPM::getDataVector)
			//.def("getQVector", &BPM::getQVector)
			//.def("getStatusVector", &BPM::getStatusVector)
			//.def("monitorForNShots", &BPM::monitorForNShots)
			//.def("ismonitoring", &BPM::ismonitoring)
			//.def("ismonitoringXPV", &BPM::ismonitoringXPV)
			//.def("ismonitoringYPV", &BPM::ismonitoringYPV)
			//.def("ismonitoringData", &BPM::ismonitoringData)
			//.def("reCalAttenuation", &BPM::reCalAttenuation)
			//.def("isXPVBufferFull", &BPM::isXPVBufferFull)
			//.def("isYPVBufferFull", &BPM::isYPVBufferFull)
			//.def("isDataBufferFull", &BPM::isDataBufferFull)
			//.def("getRA1", &BPM::getRA1)
			//.def("getRA2", &BPM::getRA2)
			//.def("getRD1", &BPM::getRD1)
			//.def("getRD2", &BPM::getRD2)
			//.def("getSA1", &BPM::getSA1)
			//.def("getSA2", &BPM::getSA2)
			//.def("getSD1", &BPM::getSD1)
			//.def("getSD2", &BPM::getSD2)
			//.def("setSA1", &BPM::setSA1)
			//.def("setSA2", &BPM::setSA2)
			//.def("setSD1", &BPM::setSD1)
			//.def("setSD2", &BPM::setSD2);

	}

	//typedef std::pair<int, int> IntPair;
	//// This is our previous registratiOn functiOn (as above) using the cleaner typedef.
	//to_PYTHON_cOnverter<IntPair, PairToTupleCOnverter<int, int> >();
	//// Registers a cOnverter for a pair of int-pairs.
	//to_PYTHON_cOnverter<std::pair<IntPair, IntPair>, PairToTupleCOnverter<IntPair, IntPair> >();




	
	void expose_screen_factory_object() {

		//screen Factory Exposure
		boost::python::class_<ScreenFactory>("ScreenFactory", boost::python::no_init)
			.def(boost::python::init<STATE>())
			.def("setup", &ScreenFactory::setup)
			.add_property("screenMap", &ScreenFactory::screenMap)
			.def("getScreen", &ScreenFactory::getScreen, boost::python::return_value_policy<boost::python::reference_existing_object>())
			.def("getAllScreens", &ScreenFactory::getAllScreens);
	}


}
#endif