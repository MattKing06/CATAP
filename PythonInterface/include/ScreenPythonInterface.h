#ifndef SCREEN_PYTHON_INTERFACE_H_
#define SCREEN_PYTHON_INTERFACE_H_
#include "Screen.h"
#include "ScreenFactory.h"
#include "PythonTypeConversions.h"

#include <boost/python.hpp>
#include <boost/python/suite/indexing/map_indexing_suite.hpp>

//using namespace boost::PYTHON;
//using namespace boost;
namespace BOOST_PYTHON_SCREEN_INCLUDE
{

	void expose_screen_object() {
		bool is_registered = (0 != boost::python::converter::registry::query(boost::python::type_id<Screen>())->to_python_target_type());
		if (is_registered) return;
		// bpm exposure
		boost::python::class_<Screen, boost::python::bases<Hardware>, boost::noncopyable>("Screen", boost::python::no_init)
			.add_property("name", &Screen::getScreenName)
			.add_property("screenStateH", &Screen::getStateH)
			.add_property("screenStateV", &Screen::getStateV)
			.add_property("screenState", &Screen::getState)
			.add_property("screenSetStateH", &Screen::getSetStateH)
			.add_property("screenSetStateV", &Screen::getSetStateV)
			.add_property("screenSetState", &Screen::getSetState)
			.add_property("actposH", &Screen::getACTPOSH)
			.add_property("actposV", &Screen::getACTPOSV)
			.add_property("actpos", &Screen::getACTPOS)
			.add_property("tgtposH", &Screen::getTGTPOSH)
			.add_property("tgtposV", &Screen::getTGTPOSV)
			.add_property("tgtpos", &Screen::getTGTPOS)
			.add_property("devcentH", &Screen::getDEVCENTH)
			.add_property("devcentV", &Screen::getDEVCENTV)
			.add_property("devcent", &Screen::getDEVCENT)
			.add_property("jdiffH", &Screen::getJDiffH)
			.add_property("jdiffV", &Screen::getJDiffV)
			.add_property("jdiff", &Screen::getJDiff)
			.add_property("jogH", &Screen::getJOGH)
			.add_property("jogV", &Screen::getJOGV)
			.add_property("enV", &Screen::getENV)
			.add_property("enH", &Screen::getENH)
			.add_property("exV", &Screen::getEXV)
			.add_property("exH", &Screen::getEXH)
			.add_property("triggerH", &Screen::getTriggerH)
			.add_property("triggerV", &Screen::getTriggerV)
			.add_property("trigger", &Screen::getTrigger)
			.add_property("readyH", &Screen::getReadyH)
			.add_property("readyV", &Screen::getReadyV)
			.add_property("ready", &Screen::getReady)
			.add_property("movingH", &Screen::getMovingH)
			.add_property("movingV", &Screen::getMovingV)
			.add_property("moving", &Screen::getMoving)
			.add_property("maxposH", &Screen::getMaxPosH)
			.add_property("maxposV", &Screen::getMaxPosV)
			.add_property("maxpos", &Screen::getMaxPos)
			.add_property("getSetStateH", &Screen::getSetStateH)
			.add_property("getSetStateV", &Screen::getSetStateV)
			.add_property("getSetState", &Screen::getSetState)
			.add_property("getStateH", &Screen::getStateH)
			.add_property("getStateV", &Screen::getStateV)
			.add_property("getState", &Screen::getState)
			.def("isHOut", &Screen::isHOut)
			.def("isVOut", &Screen::isVOut)
			.def("isHIn", &Screen::isHIn)
			.def("isVIn", &Screen::isVIn)
			.def("is_HandV_OUT", &Screen::is_HandV_OUT)
			.def("isScreenIn", &Screen::isScreenIn)
			.def("isHMoving", &Screen::isHMoving)
			.def("isVMoving", &Screen::isVMoving)
			.def("isPMoving", &Screen::isPMoving)
			.def("isMoving", &Screen::isMoving)
			.def("isClearForBeam", &Screen::isClearForBeam)
			.def("isMover", &Screen::isMover)
			.def("isVMover", &Screen::isVMover)
			.def("isHVMover", &Screen::isHVMover)
			.def("isPneumatic", &Screen::isPneumatic)
			.def("getScreenState", &Screen::getScreenState)
			.def("getScreenStatePair", &Screen::getScreenStatePair)
			.def("getScreenSetState", &Screen::getScreenSetState)
			.def("getScreenSetStatePair", &Screen::getScreenSetStatePair)
			.def("getScreenType", &Screen::getScreenType)
			.def("getAvailableDevices", &Screen::getAvailableDevices)
			.def("isScreenInState", &Screen::isScreenInState)
			.def("isHElement", &Screen::isHElement)
			.def("isVElement", &Screen::isVElement)
			.def("isPElement", &Screen::isPElement)
			.def("isHEnabled", &Screen::isHEnabled)
			.def("isVEnabled", &Screen::isVEnabled)
			.def("isRFCageIn", &Screen::isRFCageIn)
			.def("getTGTPOSH", &Screen::getTGTPOSH)
			.def("getTGTPOSV", &Screen::getTGTPOSV)
			.def("getTGTPOS", &Screen::getTGTPOS)
			.def("getACTPOSH", &Screen::getACTPOSH)
			.def("getACTPOSV", &Screen::getACTPOSV)
			.def("getACTPOS", &Screen::getACTPOS)
			.def("getDEVCENTH", &Screen::getDEVCENTH)
			.def("getDEVCENTV", &Screen::getDEVCENTV)
			.def("getDEVCENT", &Screen::getDEVCENT)
			.def("getJDiffH", &Screen::getJDiffH)
			.def("getJDiffV", &Screen::getJDiffV)
			.def("getJDiff", &Screen::getJDiff)
			.def("getJDiffH", &Screen::getJDiffH)
			.def("getMaxPosH", &Screen::getMaxPosH)
			.def("getMaxPosV", &Screen::getMaxPosV)
			.def("getMaxPos", &Screen::getMaxPos)
			.def("getReadyH", &Screen::getReadyH)
			.def("getReadyV", &Screen::getReadyV)
			.def("getReady", &Screen::getReady)
			.def("getENH", &Screen::getENH)
			.def("getENV", &Screen::getReadyH)
			.def("getEXH", &Screen::getReadyV)
			.def("getEXV", &Screen::getReady)
			.def("getTriggerH", &Screen::getTriggerH)
			.def("getTriggerV", &Screen::getTriggerV)
			.def("getTrigger", &Screen::getTrigger)
			.def("getMovingH", &Screen::getMovingH)
			.def("getMovingV", &Screen::getMovingV)
			.def("getMoving", &Screen::getMoving)
			//.def("getDevicePosition", &Screen::getDevicePosition)
			.def("getPosition", &Screen::getPosition)
			.def("isVELAPneumatic", &Screen::isVELAPneumatic)
			.def("isVELAHVMover", &Screen::isVELAHVMover)
			.def("isCLARAHVMover", &Screen::isCLARAHVMover)
			.def("isCLARAVMover", &Screen::isCLARAVMover)
			.def("get_H_ACTPOS", &Screen::get_H_ACTPOS)
			.def("get_V_ACTPOS", &Screen::get_V_ACTPOS)
			.def("makeReadEqualSet", &Screen::makeReadEqualSet)
			.def("makeSetEqualRead", &Screen::makeSetEqualRead)
			.def("moveScreenOut", &Screen::moveScreenOut)
			.def("resetPosition", &Screen::resetPosition)
			.def("setScreenSDEV", &Screen::setScreenSDEV)
			.def("setScreenTrigger", &Screen::setScreenTrigger)
			.def("setScreenTriggerWDir", &Screen::setScreenTriggerWDir)
			.def("setScreenSetState", &Screen::setScreenSetState)
			.def("moveScreenTo", &Screen::moveScreenTo)
			.def("insertYAG", &Screen::insertYAG)
			.def("jogScreen", &Screen::jogScreen)
			.def("setPosition", &Screen::setPosition)
			.def("setTGTPOS", &Screen::setTGTPOS)
			.def("setTRIGGER", &Screen::setTRIGGER)
			.def("setJOG", &Screen::setJOG)
			.def("setSDEV", &Screen::setSDEV)
			.def("setEN", &Screen::setEN)
			.def("setEX", &Screen::setEX)
			.def("moveScreenTo", &Screen::moveScreenTo)
			.def("getAliases", &Screen::getAliases_Py)
			.def("getCameraName", &Screen::getCameraName)
			;
	}
	
	void expose_screen_factory_object() {
		bool is_registered = (0 != boost::python::converter::registry::query(boost::python::type_id<ScreenFactory>())->to_python_target_type());
		if (is_registered) return;
		//screen Factory Exposure
		boost::python::class_<ScreenFactory>("ScreenFactory", boost::python::no_init)
			.def(boost::python::init<STATE>())
			.def(boost::python::init<STATE, const std::string>())
			//.def(boost::python::init<TYPE>())
			.def("setup", &ScreenFactory::setup)
			.add_property("screenMap", &ScreenFactory::screenMap)
			.def("getScreen", &ScreenFactory::getScreen, boost::python::return_value_policy<boost::python::reference_existing_object>())
			//.def("getAllScreens", &ScreenFactory::getAllScreens_Py)
			.def("getCameraName", &ScreenFactory::getCameraName)
			.def("getCameraName", &ScreenFactory::getCameraNames_Py)
			.def("getAllCameraNames", &ScreenFactory::getAllCameraNames_Py)
			.def("getAllScreenNames", &ScreenFactory::getAllScreenNames_Py)
			.def("getAllScreenStates", &ScreenFactory::getAllScreenStates_Py)
			.def("getAllScreenSetStates", &ScreenFactory::getAllScreenSetStates_Py)
			//.def("getScreens", &ScreenFactory::getScreens_Py)
			.def("getScreenStates", &ScreenFactory::getScreenStates_Py)
			.def("getScreenSetStates", &ScreenFactory::getScreenSetStates_Py)
			.def("isHOut", &ScreenFactory::isHOut)
			.def("isVOut", &ScreenFactory::isVOut)
			.def("isHIn", &ScreenFactory::isHIn)
			.def("isVIn", &ScreenFactory::isVIn)
			.def("is_HandV_OUT", &ScreenFactory::is_HandV_OUT)
			.def("isScreenIn", &ScreenFactory::isScreenIn)
			.def("isHMoving", &ScreenFactory::isHMoving)
			.def("isVMoving", &ScreenFactory::isVMoving)
			.def("isPMoving", &ScreenFactory::isPMoving)
			.def("isMoving", &ScreenFactory::isMoving)
			.def("isClearForBeam", &ScreenFactory::isClearForBeam)
			.def("isMover", &ScreenFactory::isMover)
			.def("isVMover", &ScreenFactory::isVMover)
			.def("isHVMover", &ScreenFactory::isHVMover)
			.def("isPneumatic", &ScreenFactory::isPneumatic)
			.def("getScreenState", &ScreenFactory::getScreenState)
			.def("getScreenStatePair", &ScreenFactory::getScreenStatePair)
			.def("getScreenSetState", &ScreenFactory::getScreenSetState)
			.def("getScreenSetStatePair", &ScreenFactory::getScreenSetStatePair)
			.def("getScreenType", &ScreenFactory::getScreenType)
			.def("getAvailableDevices", &ScreenFactory::getAvailableDevices_Py)
			.def("isScreenInState", &ScreenFactory::isScreenInState)
			.def("isHElement", &ScreenFactory::isHElement)
			.def("isVElement", &ScreenFactory::isVElement)
			.def("isPElement", &ScreenFactory::isPElement)
			.def("isHEnabled", &ScreenFactory::isHEnabled)
			.def("isVEnabled", &ScreenFactory::isVEnabled)
			.def("isRFCageIn", &ScreenFactory::isRFCageIn)
			.def("getACTPOS", &ScreenFactory::getACTPOS)
			.def("getJDiff", &ScreenFactory::getJDiff)
			.def("isYAGIn", &ScreenFactory::isYAGIn)
			//.def("getDevicePosition", &ScreenFactory::getDevicePosition)
			.def("getPosition", &ScreenFactory::getPosition)
			.def("isVELAPneumatic", &ScreenFactory::isVELAPneumatic)
			.def("isVELAHVMover", &ScreenFactory::isVELAHVMover)
			.def("isCLARAHVMover", &ScreenFactory::isCLARAHVMover)
			.def("isCLARAVMover", &ScreenFactory::isCLARAVMover)
			.def("get_H_ACTPOS", &ScreenFactory::get_H_ACTPOS)
			.def("get_V_ACTPOS", &ScreenFactory::get_V_ACTPOS)
			.def("makeReadEqualSet", &ScreenFactory::makeReadEqualSet)
			.def("makeSetEqualRead", &ScreenFactory::makeSetEqualRead)
			.def("moveScreenOut", &ScreenFactory::moveScreenOut)
			.def("resetPosition", &ScreenFactory::resetPosition)
			.def("setScreenSDEV", &ScreenFactory::setScreenSDEV)
			.def("setScreenTrigger", &ScreenFactory::setScreenTrigger)
			.def("setScreenTriggerWDir", &ScreenFactory::setScreenTriggerWDir)
			.def("setScreenSetState", &ScreenFactory::setScreenSetState)
			.def("moveScreenTo", &ScreenFactory::moveScreenTo)
			.def("jogScreen", &ScreenFactory::jogScreen)
			.def("setPosition", &ScreenFactory::setPosition)
			.def("setTGTPOS", &ScreenFactory::setTGTPOS)
			.def("setScreenTrigger", &ScreenFactory::setScreenTrigger)
			.def("setScreenSDEV", &ScreenFactory::setScreenSDEV)
			.def("setEN", &ScreenFactory::setEN)
			.def("setEX", &ScreenFactory::setEX)
			.def("moveScreenTo", &ScreenFactory::moveScreenTo)
			.def("insertYAG", &ScreenFactory::insertYAG)
			.def("getAliases", &ScreenFactory::getAliases_Py)
			;
	}


}
#endif