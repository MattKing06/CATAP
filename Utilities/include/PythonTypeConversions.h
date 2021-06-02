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
template<class typeOfVectorToCOnvert>
inline
boost::python::list to_py_list_vector_of_vectors(const std::vector<std::vector<typeOfVectorToCOnvert>>& buffer)
{
	boost::python::list newList;
	for (auto&& iter = buffer.begin(); iter != buffer.end(); ++iter)
	{
		newList.append( to_py_list<double>(*iter));
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

template<class typeOfVectorToConvert>
inline
boost::python::list to_py_list(const boost::circular_buffer<std::vector<typeOfVectorToConvert>>& buffer)
{
	boost::python::list newList;
	for (auto&& vector : buffer)
	{
		boost::python::list vecList;
		for (auto& item : vector)
		{
			vecList.append(item);
		}
		newList.append(vecList);
	}
	return newList;
}


template<class mapkey, class mapvalue>
inline
boost::python::dict to_py_dict(const std::map<mapkey, mapvalue>& map)
{
	//typename std::map<key, value>::iterator iter;
	boost::python::dict newDictiOnary;
	for(auto&& iter = map.begin(); iter != map.end(); ++iter)
	{
		newDictiOnary[iter->first] = iter->second;
	}
	return newDictiOnary;
}



template<class mapkey, class mapvalue>
inline
boost::python::dict to_py_dict_pair(const std::map<mapkey, std::pair<mapvalue, mapvalue>>& map)
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


template<class mapkey, class value1, class value2>
inline
boost::python::dict to_py_dict(const std::map<mapkey, std::pair<value1, value2>>& map)
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

template<class primary_key, class secondary_key, class value>
inline
std::map<primary_key, std::pair<secondary_key, value>> to_std_map_pair(const boost::python::dict& dict)
{    // TODO this is well dodgy
	//https://stackoverflow.com/questions/57114920/boost-python-3-iterate-over-dict
	std::map<primary_key, std::pair<secondary_key, value>> r;
	boost::python::list keys = boost::python::list(dict.keys());
	for (int i = 0; i < len(keys); ++i)
	{
		boost::python::extract<std::string> extractor(keys[i]);
		if (extractor.check())
		{
			std::string key = extractor();
			boost::python::object o = boost::python::object(dict[key]);
			boost::python::list l = boost::python::list(dict[key]);
			if (len(l) == 2)
			{
				std::vector<value> v = to_std_vector<value>(l);
				std::pair<value, value> p(v[0], v[1]);
				r[key] = p;
			}
			else
			{
				boost::python::dict items(o);
				std::map<secondary_key, value> entry = to_std_map<secondary_key, value>(items);
				for (auto& item : entry)
				{
					std::cout << "KEY: " << item.first << std::endl;
					std::cout << "VALUES: " << item.second << std::endl;
					std::pair<secondary_key, value> pair(item.first, item.second);
					r[key] = pair;
				}
			}
		}
	}
	return r;
}



//template<class key, std::vector<class value>>
template<class mapkey, class mapvalue>
inline
boost::python::dict to_py_dict(const std::map<mapkey, std::vector<mapvalue>>& map)
{
	//typename std::map<key, std::vector<class value>>::iterator iter;
	boost::python::dict newDictiOnary;
	for (auto&& iter = map.begin(); iter != map.end(); ++iter)
	{
		newDictiOnary[iter->first] = to_py_list<mapvalue>(iter->second);
	}
	return newDictiOnary;
}

template<class mapkey, class mapvalue>
inline
boost::python::dict to_py_dict(const std::pair<mapkey, std::vector<mapvalue>>& pair)
{
	boost::python::dict newDictiOnary;
	newDictiOnary[pair.first] = to_py_list<mapvalue>(pair.second);
	return newDictiOnary;
}

template<class mapvalue>
inline
boost::python::dict to_py_dict(const std::pair<std::string, std::vector<mapvalue>>& pair)
{
	boost::python::dict newDictiOnary;
	newDictiOnary[pair.first] = to_py_list<mapvalue>(pair.second);
	return newDictiOnary;
}

template<typename mapkey, typename mapvalue>
inline
std::map<mapkey, mapvalue> to_std_map(const boost::python::dict& map_in)
{
	std::map<mapkey, mapvalue> r;
	std::vector<mapkey> map_keys = to_std_vector<mapkey>(map_in.keys());
	std::vector < mapvalue > map_values = to_std_vector<mapvalue>(map_in.values());
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

