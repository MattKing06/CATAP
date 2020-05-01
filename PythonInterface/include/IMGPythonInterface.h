#ifndef IMG_PYTHON_INTERFACE_H_
#define IMG_PYTHON_INTERFACE_H_

#include <IMG.h>
#include <IMGFactory.h>
#include <PythonTypeConversions.h>
#include <boost/python.hpp>


namespace BOOST_PYTHON_IMG_INCLUDE
{

	void expose_img_object() {
		boost::python::class_<IMG, boost::python::bases<Hardware>, boost::noncopyable>("IMG", boost::python::no_init)
			.add_property("Pressure", &IMG::getIMGPressure)
			.add_property("name", &IMG::getHardwareName)
			.add_property("state", &IMG::getIMGState, &IMG::setIMGState)
			.def("getIMGPressure", &IMG::getIMGPressure)
			.def("debugMessagesOn", &Valve::debugMessagesOn, boost::python::arg("self"))
			.def("debugMessagesOff", &Valve::debugMessagesOff, boost::python::arg("self"))
			.def("messagesOn", &Valve::messagesOn, boost::python::arg("self"))
			.def("messagesOff", &Valve::messagesOff, boost::python::arg("self"))
			;
	
	}
	void expose_img_factory_object() 
	{
		boost::python::class_<IMGFactory, boost::noncopyable>("IMGFactory", boost::python::no_init)
			.def(boost::python::init<STATE>(boost::python::arg("mode")))
			.def("setup", &IMGFactory::setup, (boost::python::arg("self"), boost::python::arg("version")))
			.def("getIMG", &IMGFactory::getIMG, (boost::python::arg("self"), boost::python::arg("name")), boost::python::return_value_policy<boost::python::reference_existing_object>())
			.def("getAllIMGNames", &IMGFactory::getAllIMGNames_Py, (boost::python::arg("self")))
			.def("getIMGPressure", &IMGFactory::getIMGPressure, (boost::python::arg("self"), boost::python::arg("name")))
			.def("getIMGPressures", &IMGFactory::getIMGPressures_Py, (boost::python::arg("self"), boost::python::arg("names")))
			.def("getAllIMGPressure", &IMGFactory::getAllIMGPressure_Py, (boost::python::arg("self")))
			.def("debugMessagesOn", &IMGFactory::debugMessagesOn, (boost::python::arg("self")))
			.def("debugMessagesOff", &IMGFactory::debugMessagesOff, (boost::python::arg("self")))
			.def("messagesOn", &IMGFactory::messagesOn, (boost::python::arg("self")))
			.def("messagesOff", &IMGFactory::messagesOff, (boost::python::arg("self")))
			;
	}



}

#endif //IMG_PYTHON_INTERFACE_H_