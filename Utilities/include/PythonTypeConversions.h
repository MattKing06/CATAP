#ifndef PYTHON_TYPE_CONVERSIONS_H_
#define PYTHON_TYPE_CONVERSIONS_H_



#include <boost/python.hpp>
#include <boost/python/dict.hpp>
#include <boost/python/list.hpp>
#include <boost/python/tuple.hpp>
#include <boost/circular_buffer.hpp>
#include <boost/iterator/zip_iterator.hpp>
#include <vector>
#include <map>
#include <utility>




/*! @addtogroup utils*/
/*!@{*/
template< typename typeOfNewVector>
inline
std::vector<typeOfNewVector> to_std_vector(const boost::python::object& iterable)
{
	return std::vector<typeOfNewVector>(boost::python::stl_input_iterator<typeOfNewVector>(iterable),
		boost::python::stl_input_iterator<typeOfNewVector>());
}
template<class typeOfVectorToCOnvert>
inline
boost::python::list to_py_list(const std::vector<typeOfVectorToCOnvert>& vector)
{
	typename std::vector<typeOfVectorToCOnvert>::iterator iter;
	boost::python::list newList;
	//for (iter = vector.begin(); iter != vector.end(); ++iter)
	for(auto&& iter = vector.begin(); iter != vector.end(); ++iter)
	{
		newList.append(*iter);
	}
	return newList;
}
template<class typeOfVectorToCOnvert>
inline
boost::python::list to_py_list(const boost::circular_buffer<typeOfVectorToCOnvert>& buffer)
{
	//typename boost::circular_buffer<typeOfVectorToCOnvert>::iterator iter;
	boost::python::list newList;
	for (auto&& iter = buffer.begin(); iter != buffer.end(); ++iter)
	{
		newList.append(*iter);
	}
	return newList;
}

template<class value1, class value2>
inline
boost::python::list to_py_list(const std::pair<value1, value2>& pair)
{
	//typename boost::circular_buffer<typeOfVectorToCOnvert>::iterator iter;
	boost::python::list newList;
	newList.append(pair.first);
	newList.append(pair.second);
	return newList;
}

template<class key, class value>
inline
boost::python::dict to_py_dict(const std::map<key, value>& map)
{
	//typename std::map<key, value>::iterator iter;
	boost::python::dict newDictiOnary;
	for(auto&& iter = map.begin(); iter != map.end(); ++iter)
	{
		newDictiOnary[iter->first] = iter->second;
	}
	return newDictiOnary;
}



template<class key, class value>
inline
boost::python::dict to_py_dict_pair(const std::map<key, std::pair<value, value>>& map)
{
	//typename std::map<key, value>::iterator iter;
	boost::python::dict newDictiOnary;
	for (auto&& iter : map)
	{
		boost::python::list newList;
		newList.append(iter.second.first);
		newList.append(iter.second.second);
		newDictiOnary[iter.first] = newList;
	}
	return newDictiOnary;
}
//template<class key, class value>
//inline
//boost::python::dict to_py_dict_pair(const std::map<key, std::pair<value, value>>& map)
//{
//	//typename std::map<key, value>::iterator iter;
//	boost::python::dict newDictiOnary;
//	for (auto&& iter : map)
//	{
//		boost::python::list newList;
//		newList.append(iter.second.first);
//		newList.append(iter.second.second);
//		newDictiOnary[iter.first] = newList;
//	}
//	return newDictiOnary;
//}


template<class key, class value1, class value2>
inline
boost::python::dict to_py_dict(const std::map<key, std::pair<value1, value2>>& map)
{
	boost::python::dict newDictiOnary;
	for (auto&& iter : map)
	{
		newDictiOnary[iter.first] = to_py_list<value1,value2>(iter.second);
	}
	return newDictiOnary;
}
//template<class value>
//inline
//std::map<std::string, std::pair<value, value>> to_std_map_pair(const boost::python::dict& dict)
//{	// TODO this is well dodgy 
//	//https://stackoverflow.com/questions/57114920/boost-python-3-iterate-over-dict
//	std::map<std::string, std::pair<value, value>> r;
//	boost::python::list keys = boost::python::list(dict.keys());
//	for (int i = 0; i < len(keys); ++i) 
//	{
//		boost::python::extract<std::string> extractor(keys[i]);
//		if (extractor.check()) 
//		{
//			std::string key1 = extractor();
//			boost::python::list l = boost::python::list(dict[key1]);
//			if (len(l) == 2)
//			{
//				std::vector<value> v = to_std_vector<value>(l);
//				std::pair<value, value> p( v[0], v[1]);
//				r[key1] = p;
//			}
//		}
//	}
//	return r;
//}
template<class strin, class value>
inline
std::map<strin, std::pair<value, value>> to_std_map_pair(const boost::python::dict& dict)
{    // TODO this is well dodgy
	//https://stackoverflow.com/questions/57114920/boost-python-3-iterate-over-dict
	std::map<strin, std::pair<value, value>> r;
	boost::python::list keys = boost::python::list(dict.keys());
	for (int i = 0; i < len(keys); ++i)
	{
		boost::python::extract<std::string> extractor(keys[i]);
		if (extractor.check())
		{
			std::string key = extractor();
			boost::python::list l = boost::python::list(dict[key]);
			if (len(l) == 2)
			{
				std::vector<value> v = to_std_vector<value>(l);
				std::pair<value, value> p(v[0], v[1]);
				r[key] = p;
			}
		}
	}
	return r;
}



//template<class key, std::vector<class value>>
template<class key, class value>
inline
boost::python::dict to_py_dict(const std::map<key, std::vector<value>>& map)
{
	//typename std::map<key, std::vector<class value>>::iterator iter;
	boost::python::dict newDictiOnary;
	for (auto&& iter = map.begin(); iter != map.end(); ++iter)
	{
		newDictiOnary[iter->first] = to_py_list<value>(iter->second);
	}
	return newDictiOnary;
}

template<class key, class value>
inline
boost::python::dict to_py_dict(const std::pair<key, std::vector<value>>& pair)
{
	boost::python::dict newDictiOnary;
	newDictiOnary[pair.first] = to_py_list<value>(pair.second);
	return newDictiOnary;
}

template<class value>
inline
boost::python::dict to_py_dict(const std::pair<std::string, std::vector<value>>& pair)
{
	boost::python::dict newDictiOnary;
	newDictiOnary[pair.first] = to_py_list<value>(pair.second);
	return newDictiOnary;
}

template<typename key, typename value>
std::map<key, value> to_std_map(const boost::python::dict& map_in)
{
	std::map<key, value> r;
	std::vector<key> map_keys = to_std_vector<key>(map_in.keys());
	std::vector < value > map_values = to_std_vector<value>(map_in.values());
	auto key_i = map_keys.begin();
	auto val_i = map_values.begin();
	while (key_i != map_keys.end() && val_i != map_values.end())
	{
		r[*key_i] = *val_i;
		++key_i;
		++val_i;
	}
	return r;
}
/*!@}*/

#endif //PYTHON_TYPE_CONVERSIONS_H_