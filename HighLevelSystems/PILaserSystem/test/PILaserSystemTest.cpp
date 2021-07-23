#include <boost/test/unit_test.hpp>
#include <PILaserSystem.h>

BOOST_AUTO_TEST_SUITE(PILaserSystemTestSuite)

BOOST_AUTO_TEST_CASE(setup_test)
{
	PILaserSystem PILaserSystem(STATE::VIRTUAL);
	bool status = PILaserSystem.setup("nominal");

}

BOOST_AUTO_TEST_SUITE_END();