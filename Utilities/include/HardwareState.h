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
#include <boost/variant.hpp>
#include <boost/python/dict.hpp>

struct convert_to_py : public boost::static_visitor<boost::python::object>
{
	//STATE, TYPE, double, long, int, unsigned short, const std::string
	template<typename T>
	boost::python::object operator()(T value) const { return static_cast<boost::python::object>(value); }
	//TYPE operator()(TYPE value) { return value; }
	//double operator()(double value) { return value; }
	//long operator()(long value) { return value; }
	//int operator()(int value) { return value; }
	//unsigned short operator()(unsigned short value) { return value; }
	//std::string operator()(std::string value) { return value; }
};

class HardwareState
{
public:
	HardwareState();
	HardwareState(const HardwareState& copyHardwareState);
	~HardwareState();
	
	std::map<std::string, boost::variant<STATE, TYPE, double, long, int, unsigned short, std::string>> state;
	

	template<typename T>
	T get(const std::string PV)
	{
		if (GlobalFunctions::entryExists(state, PV))
		{
			return boost::get<T>(state[PV]);
		}
	}

	boost::python::dict getState_Py()
	{
		std::map < std::string, boost::python::object> pyValueMap;
		for (auto& item : state)
		{
			const std::string name = item.first;
			boost::python::object pyValue = boost::apply_visitor(convert_to_py{}, state[name]);
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