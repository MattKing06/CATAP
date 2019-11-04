#define BOOST_TEST_MODULE BPMTest

#include <boost/test/included/unit_test.hpp>
#include <boost/test/parameterized_test.hpp>
#include <boost/bind.hpp>
#include <string>
#include <iostream>
#include "BPM.h"
#include <vector>

BOOST_AUTO_TEST_SUITE(BPMTestSuite)

BOOST_AUTO_TEST_CASE(creating_empty_bpm_test)
{
	BPM bpm = BPM();
	BOOST_CHECK_EQUAL(bpm.getHardwareName().c_str(), "");
	auto pvStructMap = bpm.getPVStructs();
	BOOST_CHECK_EQUAL(pvStructMap.empty(), true);
}

//BOOST_AUTO_TEST_CASE(setting_empty_magnet_psu_state_test)
//{
//	Magnet mag = Magnet();
//	mag.setPSUState(STATE::OFF);
//	BOOST_CHECK_EQUAL(mag.getPSUState(), STATE::OFF);
//}

BOOST_AUTO_TEST_SUITE_END()
