#include <boost/test/unit_test.hpp>
#include <boost/test/parameterized_test.hpp>
#include <boost/bind.hpp>
#include <string>
#include <iostream>
#include "LaserHWP.h"
#include <vector>
//#include <stdlib.h>
#include <chrono>
#include <thread>

BOOST_AUTO_TEST_SUITE(LaserHWPTestSuite)

BOOST_AUTO_TEST_CASE(creating_empty_laser_hwp_test)
{
	LaserHWP laser = LaserHWP();
	BOOST_CHECK_EQUAL(laser.getHardwareName().c_str(), "");
	auto pvStructMap = laser.getPVStructs();
	BOOST_CHECK_EQUAL(pvStructMap.empty(), true);
}

BOOST_AUTO_TEST_SUITE_END()
