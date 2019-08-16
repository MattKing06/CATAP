#define BOOST_TEST_MODULE MagnetTest

#include <boost/test/included/unit_test.hpp>
#include <boost/test/parameterized_test.hpp>
#include <boost/bind.hpp>
#include <string>
#include <iostream>
#include "Magnet.h"
#include <vector>

BOOST_AUTO_TEST_SUITE(MagnetTestSuite)

BOOST_AUTO_TEST_CASE(creating_empty_magnet_test)
{
	Magnet mag = Magnet();
	BOOST_CHECK_EQUAL(mag.getFullPVName().c_str(), "");
	auto pvStructMap = mag.getPVStructs();
	BOOST_CHECK_EQUAL(pvStructMap.empty(), true);
}

BOOST_AUTO_TEST_SUITE_END()
