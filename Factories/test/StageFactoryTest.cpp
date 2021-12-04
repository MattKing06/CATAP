#include <boost/test/unit_test.hpp>
#include <StageFactory.h>
#include <cadef.h>

BOOST_AUTO_TEST_SUITE(StageFactoryTestSuite)

BOOST_AUTO_TEST_CASE(start_stage_factory_tests)
{
	BOOST_TEST_MESSAGE("------	RUNNING STAGE FACTORY TESTS	------");
}


BOOST_AUTO_TEST_CASE(construct_stage_factory_test)
{
	StageFactory fac = StageFactory(STATE::PHYSICAL);
	fac.setup("nominal");
	for (auto& stage : fac.stageMap)
	{
		std::cout << stage.first << std::endl;
		std::cout << "min_pos: " << stage.second.getMinPosition() << std::endl;
		std::cout << "max_pos: " << stage.second.getMaxPosition() << std::endl;
	}
	std::cout << "moving stage" << std::endl;
	std::string stageName = "EBT-BA1-MOT-COFF-H-04";
	Stage& stage = fac.getStage(stageName);
	std::cout << "getting stage: " << stageName << std::endl;
	std::pair<epicsTimeStamp, double> currentPosition = stage.getCurrentPosition();
	std::cout << "current pos: " << currentPosition.second << std::endl;
	stage.moveOverRange(59.0, 63.0, 4.0);
	//stage.setNewPosition(5.0);
	std::pair<epicsTimeStamp, double> newPosition = stage.getCurrentPosition();
	std::cout << "new pos: " << newPosition.second << std::endl;

}

BOOST_AUTO_TEST_CASE(get_all_stage_devices_and_positions_test)
{
	StageFactory fac = StageFactory(STATE::PHYSICAL);
	fac.setup("nominal");
	std::string stageName = "EBT-BA1-MOT-COFF-H-04";
	Stage& stage = fac.getStage(stageName);
	auto devicesAndPositions = stage.getDevicesAndPositions();
	for (auto& device : devicesAndPositions)
	{
		std::cout << "DEVICE: " << device.first << "/// POSITION: " << device.second << std::endl;
	}
}

BOOST_AUTO_TEST_SUITE_END()