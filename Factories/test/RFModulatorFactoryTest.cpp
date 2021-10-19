#include <boost/test/unit_test.hpp>
#include <RFModulatorFactory.h>

BOOST_AUTO_TEST_SUITE(RFModulatorFactoryTestSuite)

BOOST_AUTO_TEST_CASE(start_rf_modulator_factory_tests)
{
	BOOST_TEST_MESSAGE("------	RUNNING RF_MODULATOR FACTORY TESTS	------");
}


BOOST_AUTO_TEST_CASE(setting_up_llrf_factory_print_names)
{
	RFModulatorFactory fac = RFModulatorFactory(STATE::PHYSICAL);
	fac.debugMessagesOff();
	fac.messagesOff();
	fac.setup("nominal");
	auto names = fac.getAllRFModulatorNames();
	for (auto&& item : names)
	{
		fac.messenger.printDebugMessage("RF Modulator Object name : " + item);
	}
}

BOOST_AUTO_TEST_CASE(check_gun_modulator_hex_code_update_test)
{
	RFModulatorFactory fac = RFModulatorFactory(STATE::PHYSICAL);
	fac.setup("nominal");
}


BOOST_AUTO_TEST_SUITE_END()