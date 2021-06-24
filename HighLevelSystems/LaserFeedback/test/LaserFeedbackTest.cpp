#include <boost/test/unit_test.hpp>
#include <LaserFeedback.h>

BOOST_AUTO_TEST_SUITE(LaserFeedbackTestSuite)

BOOST_AUTO_TEST_CASE(setup_test)
{
	LaserFeedback laserFeedback(STATE::VIRTUAL);
	bool status = laserFeedback.setup("nominal");

}

BOOST_AUTO_TEST_SUITE_END();