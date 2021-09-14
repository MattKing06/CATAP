#include <boost/test/unit_test.hpp>
#include "ScreenFactory.h"
#include <string>
#include <vector>
#include <stdlib.h>
#include <chrono>
#include <thread>

BOOST_AUTO_TEST_SUITE(ScreenFactoryTestSuite)


BOOST_AUTO_TEST_CASE(screen_factory_setup_test)
{
	ScreenFactory fac = ScreenFactory(STATE::PHYSICAL);
	BOOST_CHECK_EQUAL(true, fac.setup("nominal"));	//
}



BOOST_AUTO_TEST_CASE(insertYAG_test)
{
	ScreenFactory fac = ScreenFactory(STATE::PHYSICAL);
	fac.insertYAG("INJ-YAG-04");

}




//BOOST_AUTO_TEST_CASE(lighting_factory_get_states_test)
//{
//	LightingFactory fac = LightingFactory(STATE::PHYSICAL);
//	fac.setup("nominal");
//	auto states = fac.getLightingState("ALL_LIGHTS");
//	for (const auto& item : states)
//	{
//		std::cout << item.first << " = " << item.second << std::endl;
//	}	//
//}

//ScreenFactoryTestSuite/screen_factory_setup_test

BOOST_AUTO_TEST_SUITE_END();
