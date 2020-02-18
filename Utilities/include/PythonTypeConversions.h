#ifndef PYTHON_TYPE_COnVERSIONS_H_
#define PYTHON_TYPE_COnVERSIONS_H_

#include <boost/python.hpp>
#include <boost/circular_buffer.hpp>
#include <vector>
#include <map>

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
boost::python::list to_py_list(std::vector<typeOfVectorToCOnvert> vector)
{
	typename std::vector<typeOfVectorToCOnvert>::iterator iter;
	boost::python::list newList;
	for (iter = vector.begin(); iter != vector.end(); ++iter)
	{
		newList.append(*iter);
	}
	return newList;
}
template<class typeOfVectorToCOnvert>
inline
boost::python::list to_py_list(boost::circular_buffer<typeOfVectorToCOnvert> buffer)
{
	typename boost::circular_buffer<typeOfVectorToCOnvert>::iterator iter;
	boost::python::list newList;
	for (iter = buffer.begin(); iter != buffer.end(); ++iter)
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
	boost::python::dict newDictiOnary;
	for (iter = map.begin(); iter != map.end(); ++iter)
	{
		newDictiOnary[iter->first] = iter->second;
	}
	return newDictiOnary;
}

//template<class key, class value>
//inline
//boost::python::dict to_py_dict_ref(std::map<key, value> map)
//{
//	typename std::map<key, value>::iterator iter;
//	boost::python::dict newDictiOnary;
//	for (iter = map.begin(); iter != map.end(); ++iter)
//	{
//		newDictiOnary[iter->first] = &(iter->second);
//	}
//	return newDictiOnary;
//}
/*!@}*/
#endif