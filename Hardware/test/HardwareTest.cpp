#define BOOST_TEST_MODULE HardwareTest

#include <boost/test/included/unit_test.hpp>
#include <boost/test/parameterized_test.hpp>
#include <boost/bind.hpp>
#include "Hardware.h"
#include <vector>

BOOST_AUTO_TEST_SUITE(HardwareTestSuite)

BOOST_AUTO_TEST_CASE(dummy_hardware_test)
{
	BOOST_ASSERT(true && true); 
}

BOOST_AUTO_TEST_SUITE_END()