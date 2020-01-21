#define BOOST_TEST_MODULE HardwareTest

#include <boost/test/included/unit_test.hpp>
#include <boost/test/parameterized_test.hpp>
#include <boost/bind.hpp>
#include "Hardware.h"
#include <vector>

BOOST_AUTO_TEST_SUITE(HardwareTestSuite)

BOOST_AUTO_TEST_CASE(empty_hardware_test)
{
	Hardware compOnent = Hardware();
	std::string expectedMachineArea = "";
	std::string expectedHardwareType = "";
	BOOST_CHECK_EQUAL(compOnent.getHardwareType(), expectedHardwareType);
	BOOST_CHECK_EQUAL(compOnent.getMachineArea(), expectedMachineArea);
	BOOST_CHECK_EQUAL(compOnent.getSpecificHardwareParameters().empty(), true);
	auto pvStructMap = compOnent.getPVStructs();	
	BOOST_CHECK_EQUAL(pvStructMap.empty(), true);
}

BOOST_AUTO_TEST_SUITE_END()
