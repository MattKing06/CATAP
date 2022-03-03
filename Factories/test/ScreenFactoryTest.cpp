#include <boost/test/unit_test.hpp>
#include "ScreenFactory.h"
#include <string>
#include <vector>
#include <stdlib.h>
#include <chrono>
#include <thread>

BOOST_AUTO_TEST_SUITE(ScreenFactoryTestSuite)


BOOST_AUTO_TEST_CASE(insertYAG_test)
{
	ScreenFactory fac = ScreenFactory(STATE::VIRTUAL);
	fac.setup("nominal");
	fac.insertYAG("BA1-YAG-03");
	
	int counter = 0;
	while (fac.isYAGNotIn("BA1-YAG-03"))
	{
		counter += 1;
	}
	std::cout << " YAG IS IN  " << std::endl;
	if (fac.isYAGIn("BA1-YAG-03"))
	{
		fac.moveScreenOut("BA1-YAG-03");
		while (fac.isYAGIn("BA1-YAG-03"))
		{
			counter += 1;
		}
		std::cout << " YAG IS NOT IN  " << std::endl;

	}
	std::cout << " fin " << std::endl;
}


BOOST_AUTO_TEST_CASE(move_all_yag_in_then_out)
{
	ScreenFactory fac = ScreenFactory(STATE::VIRTUAL);
	fac.setup("nominal");
	fac.messagesOff();
	fac.debugMessagesOff();
	std::vector<std::string> all_screens = fac.getAllScreenNames();
	for (const auto& screen : all_screens)
	{
		bool sent = fac.insertYAG(screen);
		if (!sent)
		{
			std::cout << "!!ERROR!! could not insertYAG command for screen = " << screen << std::endl;
		}
	}
	int counter = 0;
	for (const auto& screen : all_screens)
	{
		while (fac.isYAGNotIn(screen))
		{
			counter += 1;
			if (screen == "EBT-INJ-DIA-YAG-10")
			{
				break;
			}
		}
		std::cout << screen << " is now in. counter = " << counter << std::endl;
	}
	std::cout << "ALL SCREENS IN. NOW Moving them OUT  " << std::endl;
	fac.moveAllScreensOut();
	for (const auto& screen : all_screens)
	{
		while (fac.isYAGIn(screen))
		{
			counter += 1;
			if (screen == "EBT-INJ-DIA-YAG-10")
			{
				break;
			}
		}

		std::cout << screen << " is now out. counter = " << counter << std::endl;
	}
	std::cout << "ALL SCREENS OUT " << std::endl;
	std::cout << "FIN" << std::endl;
}


//BOOST_AUTO_TEST_CASE(lighting_factory_get_states_test)
//{
//	LightingFactory fac = LightingFactory(STATE::PHYSICAL);
//	fac.setup("nominal");
//	auto states = fac.getLightingState("ALL_LIGHTS");
//	for (const auto& item : states)
//	{
//		std::cout << item.first << " = " << item.second << std::endl;
//	}	//
//}

//ScreenFactoryTestSuite/screen_factory_setup_test

BOOST_AUTO_TEST_SUITE_END();
