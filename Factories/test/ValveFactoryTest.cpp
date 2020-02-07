#include <boost/test/unit_test.hpp>
#include <ValveFactory.h>
BOOST_AUTO_TEST_SUITE(ValveFactoryTests)

BOOST_AUTO_TEST_CASE(setting_up_valve_factory_and_get_states)
{
	ValveFactory valvFac = ValveFactory(STATE::PHYSICAL);
	valvFac.setup("nominal");
	auto stateMap = valvFac.getAllValveStates();
	for (auto&& item : stateMap)
	{
		valvFac.messenger.printDebugMessage(item.first, " : ", item.second);
	}
}




BOOST_AUTO_TEST_SUITE_END(ValveFactoryTests)