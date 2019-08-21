#ifndef PYTHON_TYPE_CONVERSIONS_H_
#define PYTHON_TYPE_CONVERSIONS_H_

#include <boost/python.hpp>
#include <vector>
#include <map>
template< typename typeOfNewVector>
inline
std::vector<typeOfNewVector> to_std_vector(const boost::python::object& iterable)
{
	return std::vector<typeOfNewVector>(boost::python::stl_input_iterator<typeOfNewVector>(iterable),
		boost::python::stl_input_iterator<typeOfNewVector>());
}
template<class typeOfVectorToConvert>
inline
boost::python::list to_py_list(std::vector<typeOfVectorToConvert> vector)
{
	typename std::vector<typeOfVectorToConvert>::iterator iter;
	boost::python::list newList;
	for (iter = vector.begin(); iter != vector.end(); ++iter)
	{
		newList.append(*iter);
	}
	return newList;
}
template<class key, class value>
inline
boost::python::dict to_py_dict(std::map<key, value> map)
{
	typename std::map<key, value>::iterator iter;
	boost::python::dict newDictionary;
	for (iter = map.begin(); iter != map.end(); ++iter)
	{
		newDictionary[iter->first] = iter->second;
	}
	return newDictionary;
}

#endif