#include <boost/test/unit_test.hpp>
#include <LaserFeedback.h>

BOOST_AUTO_TEST_SUITE(LaserFeedbackTestSuite)

BOOST_AUTO_TEST_CASE(setup_test)
{
	LaserFeedback laserFeedback(STATE::VIRTUAL);
	laserFeedback.magnets.setup("nominal");
	laserFeedback.cameras.setup("nominal");
	laserFeedback.shutters.setup("nominal");
}

BOOST_AUTO_TEST_SUITE_END();