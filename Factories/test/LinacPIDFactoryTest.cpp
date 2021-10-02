#include <boost/test/unit_test.hpp>
#include <LinacPIDFactory.h>
BOOST_AUTO_TEST_SUITE(LinacPIDFactoryTest)


BOOST_AUTO_TEST_CASE(setting_up_linac_pid_factory_print_names)
{
	LinacPIDFactory fac = LinacPIDFactory(STATE::VIRTUAL);
	fac.messagesOff();
	fac.debugMessagesOff();
	fac.setup("nominal");
	for (auto&& item : fac.getAllLinacPIDNames())
	{
		fac.messenger.printDebugMessage("Linac PID Object name : " + item);
	}
}
BOOST_AUTO_TEST_SUITE_END()