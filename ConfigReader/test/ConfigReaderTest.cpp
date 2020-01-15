#define BOOST_TEST_MODULE ConfigReaderTest

#include <boost/test/included/unit_test.hpp>
#include <boost/test/parameterized_test.hpp>
#include <boost/bind.hpp>
#include "boost/filesystem.hpp"
#include "ConfigReader.h"
#include "Magnet.h"
#include <vector>

BOOST_AUTO_TEST_SUITE(ConfigReaderTestSuite)

	BOOST_AUTO_TEST_CASE(config_reader_parsing_file_not_found_test)
	{
		std::string undefinedHardwareType = "UNDEFINED";
		std::string undefinedFilename = "UNDEFINED";
		BOOST_CHECK_THROW(ConfigReader reader(undefinedFilename, STATE::VIRTUAL); , std::runtime_error);
	}

	BOOST_AUTO_TEST_CASE(config_reader_parsing_master_lattice_yaml_document_test)
	{
		std::string hardwareType = "Magnet";
		std::string nameHCOR = "VM-CLA-C2V-MAG-HCOR-01";
		std::string nameVCOR = "VM-CLA-C2V-MAG-HCOR-01";
		ConfigReader reader(hardwareType, STATE::VIRTUAL);
		std::map<std::string, Magnet> returnedParams;
		while (reader.hasMoreFilesToParse())
		{
			reader.parseNextYamlFile(returnedParams);
		}
		BOOST_CHECK(returnedParams.at(nameHCOR).getHardwareName() == nameHCOR);
		BOOST_CHECK(returnedParams.at(nameVCOR).getHardwareName() == nameVCOR);

	}

BOOST_AUTO_TEST_SUITE_END()