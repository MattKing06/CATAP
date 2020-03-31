#include <boost/test/unit_test.hpp>
#include <boost/test/parameterized_test.hpp>
#include <boost/bind.hpp>
#include <string>
#include <iostream>
#include "Screen.h"
#include <vector>
#include <stdlib.h>
#include <chrono>
#include <thread>

BOOST_AUTO_TEST_SUITE(ScreenTestSuite)

BOOST_AUTO_TEST_CASE(creating_empty_screen_test)
{
	Screen scr = Screen();
	BOOST_CHECK_EQUAL(scr.getHardwareName().c_str(), "");
	auto pvStructMap = scr.getPVStructs();
	BOOST_CHECK_EQUAL(pvStructMap.empty(), true);
}

BOOST_AUTO_TEST_SUITE_END()
