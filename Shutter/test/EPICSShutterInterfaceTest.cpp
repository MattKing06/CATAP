#define BOOST_TEST_MODULE EPICSShutterInterfaceTest

#include <boost/test/unit_test.hpp>
#include <string>
// EPICS include
#ifndef __CINT__
#include <cadef.h>
#endif


BOOST_AUTO_TEST_CASE(epics_shutter_interface_test)
{
	BOOST_TEST((true && true));
}