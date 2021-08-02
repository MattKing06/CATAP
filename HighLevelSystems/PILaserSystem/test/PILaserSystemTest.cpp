#include <boost/test/unit_test.hpp>
#include <PILaserSystem.h>

BOOST_AUTO_TEST_SUITE(PILaserSystemTestSuite)

BOOST_AUTO_TEST_CASE(setup_test)
{
	PILaserSystem PILaserSystem(STATE::VIRTUAL);
	bool status = PILaserSystem.setup("nominal");

}

BOOST_AUTO_TEST_CASE(set_all_running_stats_size_test)
{
	PILaserSystem PIL(STATE::PHYSICAL);

	bool status = PIL.setup("");
	PIL.setAllRunningStatSize(20);
	LaserEnergyMeter lem = PIL.getEnergyMeter();
	std::cout << "PIL size: " << PIL.PILaserSystem_RS_size << " LEM size: " << lem.getEnergyRunningStats().getMaxCount() << std::endl;
	BOOST_CHECK_EQUAL(PIL.PILaserSystem_RS_size, lem.getEnergyRunningStats().getMaxCount());
}

BOOST_AUTO_TEST_CASE(move_laser_mirror_test)
{
	PILaserSystem PIL(STATE::PHYSICAL);
	bool status = PIL.setup("");
	LaserMirror mirror = PIL.getLaserMirror();
	mirror.setHStep(5.0);
	mirror.moveLeft();
}

BOOST_AUTO_TEST_CASE(get_10_image_test)
{
	PILaserSystem PIL(STATE::PHYSICAL);

	bool status = PIL.setup("");
	Camera cam = PIL.getVirtualCathodeCamera();
	//get 10 images 
	for (int i = 0; i < 10; i++)
	{
		cam.updateImageData();
		std::vector<long> imagedata = cam.getImageData();
		boost::python::list imagedata_Py = cam.getImageData_Py();
	}
}

BOOST_AUTO_TEST_SUITE_END();