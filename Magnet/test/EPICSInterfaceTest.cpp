#define BOOST_TEST_MODULE EPICSInterfaceTest

#include <boost/test/unit_test.hpp>
#include "EPICSInterface.h"
#include <string>
// EPICS include
#ifndef __CINT__
#include <cadef.h>
#endif


BOOST_AUTO_TEST_CASE(epics_interface_test)
{
	BOOST_TEST((true && true));
}