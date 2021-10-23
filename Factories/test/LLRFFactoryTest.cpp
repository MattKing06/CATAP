#include <boost/test/unit_test.hpp>
#include <ValveFactory.h>
#include <LLRFFactory.h>
BOOST_AUTO_TEST_SUITE(LLRFFactoryTest)

BOOST_AUTO_TEST_CASE(setting_up_llrf_factory_print_names)
{
	LLRFFactory fac = LLRFFactory(STATE::PHYSICAL);
	//fac.messagesOff();
	//fac.debugMessagesOff();
	fac.setup("nominal");
	auto names = fac.getLLRFNames();
	for (auto&& item : fac.getLLRFNames())
	{
		fac.messenger.printDebugMessage("LLRF Object name : " + item);
	}
	std::cout << "finished" << std::endl;
}

BOOST_AUTO_TEST_CASE(get_all_trace_SCAN)
{
	LLRFFactory fac = LLRFFactory(STATE::PHYSICAL);
	//fac.messagesOff();
	//fac.debugMessagesOff();

	std::vector<TYPE> llrf_types{ TYPE::LRRG_GUN, TYPE::L01};

	fac.setup("nominal");
	LLRF gun = fac.getLLRF("CLA-GUN-LRF-CTRL-01");
	LLRF l01 = fac.getLLRF("CLA-L01-LRF-CTRL-01");

	std::map<std::string, STATE> data = gun.getAllTraceSCAN();
	for (auto&& item : data)
	{
		fac.messenger.printDebugMessage("GUN data ", item.first, " = ", item.second, " is ", ENUM_TO_STRING(item.second));
	}
	std::map<std::string, STATE> data2 = l01.getAllTraceSCAN();
	for (auto&& item : data2)
	{
		fac.messenger.printDebugMessage("L01 data2 ", item.first, " = ", item.second, " is ", ENUM_TO_STRING(item.second));
	}
	std::cout << "finished" << std::endl;
}



BOOST_AUTO_TEST_SUITE_END()