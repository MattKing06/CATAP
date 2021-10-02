#include <boost/test/unit_test.hpp>
#include <ValveFactory.h>
BOOST_AUTO_TEST_SUITE(ValveFactoryTests)

BOOST_AUTO_TEST_CASE(setting_up_valve_factory_and_get_states)
{
	ValveFactory valvFac = ValveFactory(STATE::VIRTUAL);
	valvFac.setup("nominal");
	std::this_thread::sleep_for(std::chrono::seconds(5));
	if (valvFac.hasBeenSetup)
	{
		for (auto&& valve : valvFac.valveMap)
		{
			if (ca_state(valve.second.pvStructs.at(ValveRecords::STA).CHID) == cs_conn)
			{
				auto state = valvFac.getValveState(valve.first);
				BOOST_CHECK_NE(state, STATE::ERR);
			}
			else
			{
				valvFac.messenger.printMessage(valve.first, ValveRecords::STA, " could not connect to EPICS");
			}
		}
	}
	else
	{
		valvFac.messenger.printMessage("Could not set up valve factory");
	}
}


BOOST_AUTO_TEST_CASE(getting_hardware_state_from_valve_factory)
{
	ValveFactory valvFac = ValveFactory(STATE::VIRTUAL);
	valvFac.setup("nominal");
	std::this_thread::sleep_for(std::chrono::seconds(5));
	if (valvFac.hasBeenSetup)
	{
		auto stateMap = valvFac.getSnapshot();
		for (auto item : stateMap)
		{
			std::string name = item.first;
			STATE state = item.second.get<STATE>(ValveRecords::STA);
			BOOST_CHECK_EQUAL(state, valvFac.getValveState(name));
		}
	}
	else
	{
		valvFac.messenger.printMessage("Could not set up valve factory");
	}

}



BOOST_AUTO_TEST_SUITE_END()