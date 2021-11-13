#include <boost/test/unit_test.hpp>
#include <CameraFactory.h>
BOOST_AUTO_TEST_SUITE(CameraFactoryTest)


//--run_test=CameraFactoryTest/setting_up_camera_factory_print_names
BOOST_AUTO_TEST_CASE(setting_up_camera_factory_print_names)
{
	
	CameraFactory fac = CameraFactory(STATE::PHYSICAL);
	fac.setup("nominal");
}

BOOST_AUTO_TEST_CASE(start_up_then_monitor_arrays_data)
{
	CameraFactory fac = CameraFactory(STATE::PHYSICAL);
	fac.setup("nominal");
}


BOOST_AUTO_TEST_CASE(toggle_noise_floor)
{
	CameraFactory fac = CameraFactory(STATE::PHYSICAL);
	fac.setup("nominal");
	auto names = fac.getCameraNames();
	for (auto i = 0; i < 5; ++i)
	{
		bool success = fac.toggleUseFloor("EBT-INJ-DIA-CAM-02");
		std::cout << i << " toggleUseFloor returned " << success << std::endl;
	}
}

BOOST_AUTO_TEST_CASE(save_image_buffer)
{
	CameraFactory fac = CameraFactory(STATE::PHYSICAL);
	fac.setup("nominal");
	std::string cam_name = "INJ-CAM-10";
	Camera& inj_10 = fac.getCamera(cam_name);
	inj_10.saveImageBuffer();
}

BOOST_AUTO_TEST_SUITE_END()
