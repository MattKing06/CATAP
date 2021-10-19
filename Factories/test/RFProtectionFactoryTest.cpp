#include <boost/test/unit_test.hpp>
#include <RFProtectionFactory.h>


//struct fixture {
//	fixture()
//	{
//		RFPFactory = RFProtectionFactory(STATE::VIRTUAL);
//		BOOST_TEST_MESSAGE("setup fixture");
//	}
//	~fixture() { BOOST_TEST_MESSAGE("teardown fixture"); }
//
//	RFProtectionFactory RFPFactory;
//};

BOOST_AUTO_TEST_SUITE(RFProtectionFactoryTestSuite)
BOOST_AUTO_TEST_CASE(start_test)
{
	BOOST_TEST_MESSAGE("------	RUNNING RF PROTECTION FACTORY TESTS	------");
}

BOOST_AUTO_TEST_CASE(get_all_rf_protection_names_from_factory_test)
{
	RFProtectionFactory RFPFactory = RFProtectionFactory(STATE::VIRTUAL);
	bool hasSetup = RFPFactory.setup("nominal");
	BOOST_CHECK_NE(hasSetup, false);
	BOOST_CHECK_NE(RFPFactory.getAllRFProtectionNames().empty(), true);
}


BOOST_AUTO_TEST_SUITE_END()