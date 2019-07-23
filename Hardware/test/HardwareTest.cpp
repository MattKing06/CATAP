#define BOOST_TEST_MODULE HardwareTest

#include <boost/test/included/unit_test.hpp>
#include <boost/test/parameterized_test.hpp>
#include <boost/bind.hpp>
#include "Hardware.h"
#include <vector>

BOOST_AUTO_TEST_SUITE(HardwareTestSuite)

BOOST_AUTO_TEST_CASE(empty_hardware_test)
{
	Hardware component = Hardware();
	BOOST_CHECK_EQUAL(component.getHardwareType(), "UNKNOWN");
	BOOST_CHECK_EQUAL(component.getMachineArea(), "UNKNOWN");
	BOOST_CHECK_EQUAL(component.getSpecificHardwareParameters().empty(), true);
	auto pvStructMap = component.getPVStructs();	
	BOOST_CHECK_EQUAL(pvStructMap.empty(), true);
}

BOOST_AUTO_TEST_SUITE_END()
