#include <boost/test/unit_test.hpp>
#include <ValveFactory.h>
BOOST_AUTO_TEST_SUITE(ValveFactoryTests)

BOOST_AUTO_TEST_CASE(setting_up_valve_factory_and_get_states)
{
	ValveFactory valvFac = ValveFactory(STATE::VIRTUAL);
	valvFac.setup("nominal");
	auto stateMap = valvFac.getAllValveStates();
	for (auto&& item : stateMap)
	{
		valvFac.messenger.printDebugMessage(item.first, " : ", ENUM_TO_STRING(item.second));
	}
}


BOOST_AUTO_TEST_CASE(getting_hardware_state_from_valve_factory)
{
	ValveFactory valvFac = ValveFactory(STATE::PHYSICAL);
	valvFac.setup("nominal");
	auto stateMap = valvFac.getSnapshot();
	for (auto&& item : stateMap)
	{
		std::string name = item.first;
		STATE state = item.second.get<STATE>(name);
	}
}



BOOST_AUTO_TEST_SUITE_END()