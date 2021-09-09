#ifndef HARDWARE_STATE_H
#define HARDWARE_STATE_H

#include "yaml-cpp/parser.h"
#include "yaml-cpp/yaml.h"
#include <GlobalFunctions.h>
#include <GlobalConstants.h>
#include <GlobalTypeEnums.h>
#include <GlobalStateEnums.h>
#include <PythonTypeConversions.h>
#include <map>
#include <string>
#include <iostream>
#include <boost/variant.hpp>
#include <boost/python/dict.hpp>

struct convert_to_py : public boost::static_visitor<boost::python::object>
{
	template<typename T>
	boost::python::object operator()(T value) const { return static_cast<boost::python::object>(value); }
};

class HardwareSnapshot
{
public:
	HardwareSnapshot();
	HardwareSnapshot(const HardwareSnapshot& copyHardwareState);
	~HardwareSnapshot();
	
	std::map<std::string, boost::variant<STATE, TYPE, double, long, int, unsigned short, std::string, bool, size_t>> state;
	

	template<typename T>
	T get(const std::string PV)const
	{
		if (GlobalFunctions::entryExists(state, PV))
		{
			return boost::get<T>(state.at(PV));
		}
	}
	// Convert a state into a YAML Node (std::map<std::string, boost::variant<STATE, TYPE, double, long, int, unsigned short, std::string>>)
	YAML::Node getYAMLNode()const
	{
		YAML::Node return_node;
		for (auto& item : state)
		{
			const std::string record_name = item.first;
			//std::cout << record_name;
			if (item.second.type() == typeid(STATE))
			{
				//std::cout << " is a STATE" << std::endl;
				return_node[record_name] = ENUM_TO_STRING(boost::get< STATE >(state.at(item.first)));
			}
			else if (item.second.type() == typeid(TYPE))
			{
				//std::cout << " is a TYPE" << std::endl; 
				return_node[record_name] = ENUM_TO_STRING(boost::get< TYPE >(state.at(item.first)));
			}
			else if (item.second.type() == typeid(bool))
			{
				//std::cout << " is a bool" << std::endl;
				return_node[record_name] = boost::get<bool>(state.at(item.first));
			}
			else if (item.second.type() == typeid(size_t))
			{
				//std::cout << " is a size_t" << std::endl;
				return_node[record_name] = boost::get<size_t>(state.at(item.first));
			}
			else if (item.second.type() == typeid(long))
			{
				//std::cout << " is a long" << std::endl;
				return_node[record_name] = boost::get<long>(state.at(item.first));
			}
			else if (item.second.type() == typeid(int))
			{
				//std::cout << " is a int " << std::endl;
				return_node[record_name] = boost::get< int >(state.at(item.first));
			}
			else if (item.second.type() == typeid(unsigned short))
			{
				//std::cout << " is a unsigned short " << std::endl;
				return_node[record_name] = boost::get< unsigned short >(state.at(item.first));
			}
			else if (item.second.type() == typeid(std::string))
			{
				//std::cout << " is a string " << std::endl;
				return_node[record_name] = boost::get< std::string>(state.at(item.first));
			}
			else if (item.second.type() == typeid(double))
			{
				//std::cout << " is a double " << std::endl;
				return_node[record_name] = boost::get< double>(state.at(item.first));
			}
			else
			{
			}
		}
		return return_node;
	}
	// TODO this could be renamed, maybe to convertHarwdraeSnapshotToPYDict or something more explanatory 
	boost::python::dict getSnapshot_Py()const 
	{
		std::map < std::string, boost::python::object> pyValueMap;
		for (auto& item : state)
		{
			const std::string name = item.first;
			boost::python::object pyValue = boost::apply_visitor(convert_to_py{}, state.at(name));
			pyValueMap[name] = pyValue;
		}
		return to_py_dict(pyValueMap);
	}
	template<typename T>
	void update(const std::string& PV, T value)  
	{
		if (GlobalFunctions::entryExists(state, PV))
		{
			state[PV] = value;
		}
		else
		{
			add(PV, value);
		}
	}
	template<typename T>
	void add(const std::string& PV, T value) 
	{
		if (GlobalFunctions::entryExists(state, PV))
		{
			update(PV, value);
		}
		else
		{
			std::pair<std::string, T> PVandValue(PV, value);
			state.insert(PVandValue);
		}
	}
	// Probably need to find the most appropriate map structure for nested states.
	std::map<std::string, std::string> readStateFromYaml();
	void writeStateFromMap(std::map<std::string, std::string> mapToWrite);
};




#endif