#ifndef LINAC_PID_RECORDS
#define LINAC_PID_RECORDS

#include <GlobalConstants.h>
#include <string>
#include <vector>
/*! @addtogroup utils*/
/*!@{*/

namespace LinacPIDRecords
{
	extern std::string const AVG_PHASE;
	extern std::string const FORWARD_PHASE_WEIGHT;
	extern std::string const PROBE_PHASE_WEIGHT;
	extern std::string const SCAN;
	extern std::string const PID_OVAL;
	extern std::string const FORWARD_PHASE_WRAPPED;
	extern std::string const PROBE_PHASE_WRAPPED;

	extern std::vector<std::string> linacPIDRecordList;
	extern std::vector<std::string> linacMonitorRecordsList;	

}

/*!@}*/

#endif //! LINAC_PID_RECORDS