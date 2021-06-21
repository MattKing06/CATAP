#include <LinacPIDPVRecords.h>

namespace LinacPIDRecords
{
	std::string const AVG_PHASE = "AVG_PHASE";
	std::string const FORWARD_PHASE_WEIGHT = "FORWARD_PHASE_WEIGHT";
	std::string const PROBE_PHASE_WEIGHT = "PROBE_PHASE_WEIGHT";
	std::string const SCAN = "SCAN";
	std::string const PID_OVAL = "PID_OVAL";
	std::string const FORWARD_PHASE_WRAPPED = "FORWARD_PHASE_WRAPPED";
	std::string const PROBE_PHASE_WRAPPED = "PROBE_PHASE_WRAPPED";


	std::vector<std::string> linacPIDRecordList = { AVG_PHASE, PROBE_PHASE_WEIGHT, PROBE_PHASE_WRAPPED,
													   SCAN, PID_OVAL, FORWARD_PHASE_WRAPPED,
													   FORWARD_PHASE_WEIGHT };

	std::vector<std::string> linacMonitorRecordsList{ AVG_PHASE, PROBE_PHASE_WEIGHT, PROBE_PHASE_WRAPPED,
													   SCAN, PID_OVAL, FORWARD_PHASE_WRAPPED,
													   FORWARD_PHASE_WEIGHT };

}