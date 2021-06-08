#include <LinacPIDPVRecords.h>

namespace LinacPIDRecords
{
	std::string const AVG_PHASE = "AVG_PHASE";
	std::string const CH3_PHASE_WEIGHT = "CH3_PHASE_WEIGHT";
	std::string const CH6_PHASE_WEIGHT = "CH6_PHASE_WEIGHT";
	std::string const SCAN = "SCAN";
	std::string const PID_OVAL = "PID_OVAL";
	std::string const CH3_PHASE_WRAPPED = "CH3_PHASE_WRAPPED";
	std::string const CH6_PHASE_WRAPPED = "CH6_PHASE_WRAPPED";


	std::vector<std::string> linacPIDRecordList = { AVG_PHASE, CH3_PHASE_WEIGHT, CH6_PHASE_WEIGHT,
													   SCAN, PID_OVAL, CH3_PHASE_WRAPPED,
													   CH6_PHASE_WRAPPED };
}