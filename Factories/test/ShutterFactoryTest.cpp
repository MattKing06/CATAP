#include <boost/test/unit_test.hpp>
#ifdef BUILD_PYTHON
#include <boost/python/dict.hpp>
#endif //BUILD_PYTHON
#include <ShutterFactory.h>

BOOST_AUTO_TEST_SUITE(ShutterFactoryTestSuite)

BOOST_AUTO_TEST_CASE(start_shutter_factory_tests)
{
	BOOST_TEST_MESSAGE("------	RUNNING SHUTTER FACTORY TESTS	------");
}

BOOST_AUTO_TEST_CASE(setting_up_shutter_factory_and_get_get_shutterstate_dictionaries)
{
	ShutterFactory shut_fac = ShutterFactory(STATE::VIRTUAL);
	shut_fac.setup("nominal");
	//auto map = shut_fac.getShutterStateDictionaries();
	auto t = shut_fac.getShutterStates();
	for (auto&& item : shut_fac.getShutterStates())
	{
		shut_fac.messenger.printDebugMessage("item.name = ", item.second.name);
		shut_fac.messenger.printDebugMessage("item.cmi = ", item.second.Cmi);
		shut_fac.messenger.printDebugMessage("item.state = ", item.second.state);
	}
}
BOOST_AUTO_TEST_SUITE_END()