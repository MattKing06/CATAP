#include <boost/test/unit_test.hpp>
#include <CameraFactory.h>
BOOST_AUTO_TEST_SUITE(CameraFactoryTest)


BOOST_AUTO_TEST_CASE(setting_up_camera_factory_print_names)
{
	CameraFactory fac = CameraFactory(STATE::VIRTUAL);
	fac.setup("nominal");
	for (auto&& item : fac.getCameraNames())
	{
		fac.messenger.printDebugMessage("Camera Object name : " + item);
	}
}
BOOST_AUTO_TEST_SUITE_END()