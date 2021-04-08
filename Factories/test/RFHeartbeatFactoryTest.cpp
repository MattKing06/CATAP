#include <boost/test/unit_test.hpp>
#include <RFHeartbeatFactory.h>

BOOST_AUTO_TEST_SUITE(RFHeartbeatFactoryTestSuite)

BOOST_AUTO_TEST_CASE(start_rf_heartbeat_factory_tests)
{
	BOOST_TEST_MESSAGE("------	RUNNING RF_HEARTBEAT FACTORY TESTS	------");
}

BOOST_AUTO_TEST_CASE(rf_heartbeat_factory_instantiation_test)
{
	RFHeartbeatFactory fac = RFHeartbeatFactory(STATE::VIRTUAL);
	fac.messagesOn();
	fac.debugMessagesOn();
	bool status = fac.setup("nominal");
	if (status)
	{
	}
}

BOOST_AUTO_TEST_SUITE_END()