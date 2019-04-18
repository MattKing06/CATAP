#include "Magnet.h"
#include <vector>
#include "ConfigReader.h"

Magnet::Magnet(std::string knownNameOfMagnet)
{
	//assuming known name
	ConfigReader configReader(knownNameOfMagnet);
	std::vector<std::tuple<std::string, std::string> > magnetParamtersAndValuesVector = configReader.parseYamlFile();
	
}