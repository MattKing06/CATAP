#ifndef IMG_RECORDS
#define IMG_RECORDS
#include <string>
#include <vector>

#include "GlobalStateEnums.h"
#include "GlobalConstants.h"

/*! @addtogroup utils*/
/*!@{*/

/*!
	Namespace to define keys of "pv_record_map" found in MasterLattice Config Files for IMGs.
*/

namespace IMGRecords
{
	/*Defined at std::string const so that the value of the variable cannot be changed.*/
	
	/*! Used to search for "Pressure" key in pv_record_map */
	
	extern std::string const Pressure;
	
	/*! Used to search for "Sta" key in pv_record_map */
	
	extern std::string const Sta;
	
	extern std::vector<std::string> imgRecordList;
}
/*! @}*/

/*! @addtogroup utils*/

/*!@{*/

/*!
	Namespace that holds the IMGStateStruct
*/
namespace IMGState
{
	/*! Used to store states (OK, ERR) for multiple IMG objects. */
	
	struct IMGStateStruct
	{   
		IMGStateStruct() :
			numIMGs(GlobalConstants::zero_sizet)
		{};
		size_t numIMGs;

	};
}

/*! @}*/

#endif