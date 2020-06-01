#include <boost/test/unit_test.hpp>
#include <boost/test/parameterized_test.hpp>
#include <boost/bind.hpp>
#include <string>
#include <iostream>
#include <vector>
#include <IMG.h>
#include <boost/test/parameterized_test.hpp>
#include <boost/bind.hpp>
#include<boost/range.hpp>

BOOST_AUTO_TEST_SUITE(IMGTestSuite)

BOOST_AUTO_TEST_CASE(start_img_tests)
{
	BOOST_TEST_MESSAGE("------	RUNNING IMG TESTS	------");
}

BOOST_AUTO_TEST_CASE(creating_empty_img_test)
{
	BOOST_TEST_MESSAGE("------ IMG HW: CREATING AN EMPTY IMG TEST	------");
	IMG img = IMG();
	BOOST_CHECK_EQUAL(img.getHardwareName().c_str(), "");
	auto pvStructMap = img.getPVStructs();
	BOOST_CHECK_EQUAL(pvStructMap.empty(), true);
}

BOOST_AUTO_TEST_CASE(getting_pressure_img_test)
{	
	BOOST_TEST_MESSAGE("------	IMG HW: GETTING THE PRESSURE OF IMG	------");
	IMG img = IMG();
	BOOST_CHECK(isnan(img.getIMGPressure()) != true);
}

BOOST_AUTO_TEST_CASE(getting_setting_state_img_test)
{
	BOOST_TEST_MESSAGE("------	IMG HW: GETTING AND SETTING STATE OF IMG	------");
	IMG img = IMG();
	STATE error = STATE::ERR;
	STATE ok = STATE::OK;
	img.setIMGState(STATE::OK);
	BOOST_CHECK_EQUAL(img.getIMGState(), ok);
	img.setIMGState(STATE::ERR);
	BOOST_CHECK_EQUAL(img.getIMGState(), error);
}

BOOST_AUTO_TEST_CASE(getting_img_aliases_test)
{
	BOOST_TEST_MESSAGE("------	IMG HW: GETTING THE ALIASES of IMG	------");
	IMG img = IMG();
	auto allAliases = img.getAliases();
	for (auto alias : allAliases)
		BOOST_CHECK(boost::size(alias)!= NULL);
}

BOOST_AUTO_TEST_SUITE_END()