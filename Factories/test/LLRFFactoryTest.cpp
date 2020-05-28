#include <boost/test/unit_test.hpp>
#include <ValveFactory.h>
#include <LLRFFactory.h>
BOOST_AUTO_TEST_SUITE(LLRFFactoryTest)

BOOST_AUTO_TEST_CASE(setting_up_llrf_factory_print_names)
{
	LLRFFactory fac = LLRFFactory(STATE::VIRTUAL);
	fac.setup("nominal");
	auto names = fac.getLLRFNames();
	for (auto&& item : fac.getLLRFNames())
	{
		fac.messenger.printDebugMessage("LLRF Object name : " + item);
	}
}




BOOST_AUTO_TEST_SUITE_END()