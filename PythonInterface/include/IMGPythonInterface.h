#ifndef IMG_PYTHON_INTERFACE_H_
#define IMG_PYTHON_INTERFACE_H_

#include <IMG.h>
#include <IMGFactory.h>
#include <PythonTypeConversions.h>
#include <boost/python.hpp>


namespace BOOST_PYTHON_IMG_INCLUDE
{
	void expose_img_object() {
		boost::python::class_<IMG, boost::python::bases<Hardware>, boost::noncopyable>("IMG", "IMG object", boost::python::no_init)
			.add_property("pressure", &IMG::getIMGPressure,"IMG - Pressure attribute")
			.add_property("name", &IMG::getHardwareName, "IMG - IMG Name attribute")
			.add_property("state", &IMG::getIMGState, &IMG::setIMGState, "IMG - IMG State attribute")
			.def("getPressure", &IMG::getIMGPressure,"IMG - Get IMG Pressure", boost::python::arg("self"))
			.def("debugMessagesOn", &IMG::debugMessagesOn, "IMG - Debug Messages On", boost::python::arg("self"))
			.def("debugMessagesOff", &IMG::debugMessagesOff, "IMG - Debug Messages Off", boost::python::arg("self"))
			.def("messagesOn", &IMG::messagesOn, "IMG - Messages On", boost::python::arg("self"))
			.def("messagesOff", &IMG::messagesOff, "IMG - Messages Off", boost::python::arg("self"))
			;
	
	}
	void expose_img_factory_object() 
	{
		boost::python::class_<IMGFactory, boost::noncopyable>("IMGFactory","IMG Factory", boost::python::no_init)
			.def(boost::python::init<STATE>(boost::python::arg("mode")))
			.def("setup", &IMGFactory::setup,"IMG Factory- Setup", (boost::python::arg("self"), boost::python::arg("version")))
			.def("getIMG", &IMGFactory::getIMG, "IMG Factory- Get IMG by name", (boost::python::arg("self"), boost::python::arg("name")), boost::python::return_value_policy<boost::python::reference_existing_object>())
			.def("getAllNames", &IMGFactory::getAllIMGNames_Py, "IMG Factory- Get all IMG names of the Map", (boost::python::arg("self")))
			.def("getPressure", &IMGFactory::getIMGPressure, "IMG Factory- Get IMG Pressure of an IMG", (boost::python::arg("self"), boost::python::arg("name")))
			.def("getPressures", &IMGFactory::getIMGPressures_Py, "IMG Factory- Get IMG Pressure of a subset of IMGs", (boost::python::arg("self"), boost::python::arg("names")))
			.def("getAllPressures", &IMGFactory::getAllIMGPressure_Py, "IMG Factory- Get the IMG Pressures of the whole set of IMGs in the Map", (boost::python::arg("self")))
			.def("getState", &IMGFactory::getIMGState, "IMG Factory- Get IMG State of an IMG", (boost::python::arg("self"), boost::python::arg("name")))
			.def("getStates", &IMGFactory::getIMGStates_Py, "IMG Factory- Get IMG States of a subset of IMGs", (boost::python::arg("self"), boost::python::arg("names")))
			.def("getAllStates", &IMGFactory::getAllIMGStates_Py, "IMG Factory- Get the IMG States of the whole set of IMGs in the Map", (boost::python::arg("self")))
			.def("debugMessagesOn", &IMGFactory::debugMessagesOn, "IMG Factory - Debug Messages On",  (boost::python::arg("self")))
			.def("debugMessagesOff", &IMGFactory::debugMessagesOff, "IMG Factory - Debug Messages Off", (boost::python::arg("self")))
			.def("messagesOn", &IMGFactory::messagesOn, "IMG Factory - Messages On", (boost::python::arg("self")))
			.def("messagesOff", &IMGFactory::messagesOff, "IMG Factory - Messages Off", (boost::python::arg("self")))
			;
	}
}

#endif //IMG_PYTHON_INTERFACE_H_