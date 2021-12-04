#include <boost/test/unit_test.hpp>
#include <LightingFactory.h>

BOOST_AUTO_TEST_SUITE(LightingFactoryTestSuite)

BOOST_AUTO_TEST_CASE(start_lighting_factory_tests)
{
	BOOST_TEST_MESSAGE("------	RUNNING LIGHTING FACTORY TESTS	------");
}


BOOST_AUTO_TEST_CASE(lighting_factory_setup_test)
{
	LightingFactory fac = LightingFactory(STATE::PHYSICAL);
	BOOST_CHECK_EQUAL(true, fac.setup("nominal"));	//
}


BOOST_AUTO_TEST_CASE(lighting_factory_get_states_test)
{
	LightingFactory fac = LightingFactory(STATE::PHYSICAL);
	fac.setup("nominal");
	auto states = fac.getLightingState("ALL_LIGHTS");
	for (const auto& item : states)
	{
		std::cout << item.first << " = " << item.second << std::endl;
	}	//
}

BOOST_AUTO_TEST_SUITE_END()