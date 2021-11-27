#include <LLRFPVRecords.h>


namespace LLRFRecords
{
	std::string const HEART_BEAT = "HEART_BEAT";
	std::string const AMP_FF = "AMP_FF";
	std::string const TRIG_SOURCE = "TRIG_SOURCE";
	std::string const AMP_SP = "AMP_SP";
	std::string const MAX_AMP_SP = "MAX_AMP_SP";
	std::string const LLRF_PULSE_DURATION = "LLRF_PULSE_DURATION";
	std::string const PHI_FF = "PHI_FF";
	std::string const PHI_SP = "PHI_SP";
	std::string const RF_OUTPUT = "RF_OUTPUT";
	std::string const FF_PHASE_LOCK_STATE = "FF_PHASE_LOCK_STATE";
	std::string const FF_AMP_LOCK_STATE = "FF_AMP_LOCK_STATE";
	std::string const TIME_VECTOR = "TIME_VECTOR";
	std::string const PULSE_OFFSET = "PULSE_OFFSET";
	std::string const INTERLOCK = "INTERLOCK";
	std::string const PULSE_SHAPE = "PULSE_SHAPE";
	std::string const PULSE_SHAPE_APPLY = "PULSE_SHAPE_APPLY";

	std::string const ONE_TRACE_SCAN = "ONE_TRACE_SCAN";
	std::string const ONE_TRACE_ACQM = "ONE_TRACE_ACQM";


	const std::string LRRG_CAVITY_REVERSE_PHASE = "LRRG_CAVITY_REVERSE_PHASE";
	const std::string HRRG_CAVITY_REVERSE_PHASE = "HRRG_CAVITY_REVERSE_PHASE";
	const std::string L01_CAVITY_REVERSE_PHASE = "L01_CAVITY_REVERSE_PHASE";
	const std::string CAVITY_REVERSE_PHASE = "CAVITY_REVERSE_PHASE";

	const std::string LRRG_CAVITY_FORWARD_PHASE = "LRRG_CAVITY_FORWARD_PHASE";
	const std::string HRRG_CAVITY_FORWARD_PHASE = "HRRG_CAVITY_FORWARD_PHASE";
	const std::string L01_CAVITY_FORWARD_PHASE = "L01_CAVITY_FORWARD_PHASE";
	const std::string CAVITY_FORWARD_PHASE = "CAVITY_FORWARD_PHASE";

	const std::string LRRG_CAVITY_REVERSE_POWER = "LRRG_CAVITY_REVERSE_POWER";
	const std::string HRRG_CAVITY_REVERSE_POWER = "HRRG_CAVITY_REVERSE_POWER";
	const std::string L01_CAVITY_REVERSE_POWER = "L01_CAVITY_REVERSE_POWER";
	const std::string CAVITY_REVERSE_POWER = "CAVITY_REVERSE_POWER";

	const std::string LRRG_CAVITY_FORWARD_POWER = "LRRG_CAVITY_FORWARD_POWER";
	const std::string HRRG_CAVITY_FORWARD_POWER = "HRRG_CAVITY_FORWARD_POWER";
	const std::string L01_CAVITY_FORWARD_POWER = "L01_CAVITY_FORWARD_POWER";
	const std::string CAVITY_FORWARD_POWER = "CAVITY_FORWARD_POWER";

	const std::string CAVITY_PROBE_POWER = "CAVITY_PROBE_POWER";
	const std::string HRRG_CAVITY_PROBE_POWER = "HRRG_CAVITY_PROBE_POWER";
	const std::string L01_CAVITY_PROBE_POWER = "L01_CAVITY_PROBE_POWER";

	const std::string CAVITY_PROBE_PHASE = "CAVITY_PROBE_PHASE";
	const std::string HRRG_CAVITY_PROBE_PHASE = "HRRG_CAVITY_PROBE_PHASE";
	const std::string L01_CAVITY_PROBE_PHASE = "L01_CAVITY_PROBE_PHASE";


	const std::string  KRPOW = "KRPOW";
	const std::string  KRPHA = "KRPHA";
	const std::string  KFPOW = "KFPOW";
	const std::string  KFPHA = "KFPHA";
	const std::string  CRPOW = "CRPOW";
	const std::string  CRPHA = "CRPHA";
	const std::string  CPPOW = "CPPOW";
	const std::string  CPPHA = "CPPHA";
	const std::string  CFPOW = "CFPOW";
	const std::string  CFPHA = "CFPHA";

	const std::string KLYSTRON_REVERSE_PHASE = "KLYSTRON_REVERSE_PHASE";
	const std::string KLYSTRON_REVERSE_POWER = "KLYSTRON_REVERSE_POWER";
	const std::string KLYSTRON_FORWARD_PHASE = "KLYSTRON_FORWARD_PHASE";
	const std::string KLYSTRON_FORWARD_POWER = "KLYSTRON_FORWARD_POWER";

	const std::string ONE_RECORD_TRACE_PREFIX = "ONE_RECORD_";
	const std::string ONE_RECORD_KLYSTRON_FORWARD_POWER = "ONE_RECORD_KLYSTRON_FORWARD_POWER";
	const std::string ONE_RECORD_KLYSTRON_FORWARD_PHASE = "ONE_RECORD_KLYSTRON_FORWARD_PHASE";
	const std::string ONE_RECORD_KLYSTRON_REVERSE_POWER = "ONE_RECORD_KLYSTRON_REVERSE_POWER";
	const std::string ONE_RECORD_KLYSTRON_REVERSE_PHASE = "ONE_RECORD_KLYSTRON_REVERSE_PHASE";
	const std::string ONE_RECORD_LRRG_CAVITY_FORWARD_POWER = "ONE_RECORD_LRRG_CAVITY_FORWARD_POWER";
	const std::string ONE_RECORD_LRRG_CAVITY_FORWARD_PHASE = "ONE_RECORD_LRRG_CAVITY_FORWARD_PHASE";
	const std::string ONE_RECORD_LRRG_CAVITY_REVERSE_POWER = "ONE_RECORD_LRRG_CAVITY_REVERSE_POWER";
	const std::string ONE_RECORD_LRRG_CAVITY_REVERSE_PHASE = "ONE_RECORD_LRRG_CAVITY_REVERSE_PHASE";
	const std::string ONE_RECORD_HRRG_CAVITY_FORWARD_POWER = "ONE_RECORD_HRRG_CAVITY_FORWARD_POWER";
	const std::string ONE_RECORD_HRRG_CAVITY_FORWARD_PHASE = "ONE_RECORD_HRRG_CAVITY_FORWARD_PHASE";
	const std::string ONE_RECORD_HRRG_CAVITY_REVERSE_POWER = "ONE_RECORD_HRRG_CAVITY_REVERSE_POWER";
	const std::string ONE_RECORD_HRRG_CAVITY_REVERSE_PHASE = "ONE_RECORD_HRRG_CAVITY_REVERSE_PHASE";
	const std::string ONE_RECORD_HRRG_CAVITY_PROBE_POWER   = "ONE_RECORD_HRRG_CAVITY_PROBE_POWER";
	const std::string ONE_RECORD_HRRG_CAVITY_PROBE_PHASE   = "ONE_RECORD_HRRG_CAVITY_PROBE_PHASE";
	const std::string ONE_RECORD_L01_CAVITY_FORWARD_POWER = "ONE_RECORD_L01_CAVITY_FORWARD_POWER";
	const std::string ONE_RECORD_L01_CAVITY_FORWARD_PHASE = "ONE_RECORD_L01_CAVITY_FORWARD_PHASE";
	const std::string ONE_RECORD_L01_CAVITY_REVERSE_POWER = "ONE_RECORD_L01_CAVITY_REVERSE_POWER";
	const std::string ONE_RECORD_L01_CAVITY_REVERSE_PHASE = "ONE_RECORD_L01_CAVITY_REVERSE_PHASE";
	const std::string ONE_RECORD_L01_CAVITY_PROBE_POWER   = "ONE_RECORD_L01_CAVITY_PROBE_POWER";
	const std::string ONE_RECORD_L01_CAVITY_PROBE_PHASE   = "ONE_RECORD_L01_CAVITY_PROBE_PHASE";
	const std::string ONE_RECORD_CALIBRATION_POWER = "ONE_RECORD_CALIBRATION_POWER";
	const std::string ONE_RECORD_CALIBRATION_PHASE = "ONE_RECORD_CALIBRATION_PHASE";
	const std::vector<std::string>all_one_record_trace_names = {
		ONE_RECORD_KLYSTRON_REVERSE_POWER,	  ONE_RECORD_KLYSTRON_REVERSE_PHASE,
		ONE_RECORD_KLYSTRON_FORWARD_POWER,    ONE_RECORD_KLYSTRON_FORWARD_PHASE,
		ONE_RECORD_LRRG_CAVITY_FORWARD_POWER, ONE_RECORD_LRRG_CAVITY_FORWARD_PHASE,
		ONE_RECORD_LRRG_CAVITY_REVERSE_POWER, ONE_RECORD_LRRG_CAVITY_REVERSE_PHASE,
		ONE_RECORD_HRRG_CAVITY_FORWARD_POWER, ONE_RECORD_HRRG_CAVITY_FORWARD_PHASE,
		ONE_RECORD_HRRG_CAVITY_REVERSE_POWER, ONE_RECORD_HRRG_CAVITY_REVERSE_PHASE,
		ONE_RECORD_HRRG_CAVITY_PROBE_POWER,   ONE_RECORD_HRRG_CAVITY_PROBE_PHASE,
		ONE_RECORD_L01_CAVITY_FORWARD_POWER,  ONE_RECORD_L01_CAVITY_FORWARD_PHASE,
		ONE_RECORD_L01_CAVITY_REVERSE_POWER,  ONE_RECORD_L01_CAVITY_REVERSE_PHASE,
		ONE_RECORD_L01_CAVITY_PROBE_POWER,	  ONE_RECORD_L01_CAVITY_PROBE_PHASE,
		ONE_RECORD_CALIBRATION_POWER,		  ONE_RECORD_CALIBRATION_PHASE };

	///  master lattice keys 
	std::string const CH1_INTERLOCK_STATUS = "CH1_INTERLOCK_STATUS";
	std::string const CH1_INTERLOCK_ENABLE = "CH1_INTERLOCK_ENABLE";
	std::string const CH1_INTERLOCK_U = "CH1_INTERLOCK_U";
	std::string const CH1_INTERLOCK_P = "CH1_INTERLOCK_P";
	std::string const CH1_INTERLOCK_PDBM = "CH1_INTERLOCK_PDBM";
	std::string const CH2_INTERLOCK_STATUS = "CH2_INTERLOCK_STATUS";
	std::string const CH2_INTERLOCK_ENABLE = "CH2_INTERLOCK_ENABLE";
	std::string const CH2_INTERLOCK_U = "CH2_INTERLOCK_U";
	std::string const CH2_INTERLOCK_P = "CH2_INTERLOCK_P";
	std::string const CH2_INTERLOCK_PDBM = "CH2_INTERLOCK_PDBM";
	std::string const CH3_INTERLOCK_STATUS = "CH3_INTERLOCK_STATUS";
	std::string const CH3_INTERLOCK_ENABLE = "CH3_INTERLOCK_ENABLE";
	std::string const CH3_INTERLOCK_U = "CH3_INTERLOCK_U";
	std::string const CH3_INTERLOCK_P = "CH3_INTERLOCK_P";
	std::string const CH3_INTERLOCK_PDBM = "CH3_INTERLOCK_PDBM";
	std::string const CH4_INTERLOCK_STATUS = "CH4_INTERLOCK_STATUS";
	std::string const CH4_INTERLOCK_ENABLE = "CH4_INTERLOCK_ENABLE";
	std::string const CH4_INTERLOCK_U = "CH4_INTERLOCK_U";
	std::string const CH4_INTERLOCK_P = "CH4_INTERLOCK_P";
	std::string const CH4_INTERLOCK_PDBM = "CH4_INTERLOCK_PDBM";
	std::string const CH5_INTERLOCK_STATUS = "CH5_INTERLOCK_STATUS";
	std::string const CH5_INTERLOCK_ENABLE = "CH5_INTERLOCK_ENABLE";
	std::string const CH5_INTERLOCK_U = "CH5_INTERLOCK_U";
	std::string const CH5_INTERLOCK_P = "CH5_INTERLOCK_P";
	std::string const CH5_INTERLOCK_PDBM = "CH5_INTERLOCK_PDBM";
	std::string const CH6_INTERLOCK_STATUS = "CH6_INTERLOCK_STATUS";
	std::string const CH6_INTERLOCK_ENABLE = "CH6_INTERLOCK_ENABLE";
	std::string const CH6_INTERLOCK_U = "CH6_INTERLOCK_U";
	std::string const CH6_INTERLOCK_P = "CH6_INTERLOCK_P";
	std::string const CH6_INTERLOCK_PDBM = "CH6_INTERLOCK_PDBM";
	std::string const CH7_INTERLOCK_STATUS = "CH7_INTERLOCK_STATUS";
	std::string const CH7_INTERLOCK_ENABLE = "CH7_INTERLOCK_ENABLE";
	std::string const CH7_INTERLOCK_U = "CH7_INTERLOCK_U";
	std::string const CH7_INTERLOCK_P = "CH7_INTERLOCK_P";
	std::string const CH7_INTERLOCK_PDBM = "CH7_INTERLOCK_PDBM";
	std::string const CH8_INTERLOCK_STATUS = "CH8_INTERLOCK_STATUS";
	std::string const CH8_INTERLOCK_ENABLE = "CH8_INTERLOCK_ENABLE";
	std::string const CH8_INTERLOCK_U = "CH8_INTERLOCK_U";
	std::string const CH8_INTERLOCK_P = "CH8_INTERLOCK_P";
	std::string const CH8_INTERLOCK_PDBM = "CH8_INTERLOCK_PDBM";
	std::string const LLRF_TRACES = "LLRF_TRACES";
	std::string const LLRF_TRACES_SCAN = "LLRF_TRACES_SCAN";
	std::string const LLRF_TRACES_ACQM = "LLRF_TRACES_ACQM";


	// these are generic names used to key the interlock objects in the LLRF, 
	std::string const CH1 = "CH1";
	std::string const CH2 = "CH2";
	std::string const CH3 = "CH3";
	std::string const CH4 = "CH4";
	std::string const CH5 = "CH5";
	std::string const CH6 = "CH6";
	std::string const CH7 = "CH7";
	std::string const CH8 = "CH8";

	// these are generic names used to key the interlock objects in the LLRF, 
	std::string const LLRF_CH1_INTERLOCK = "LLRF_CH1_INTERLOCK";
	std::string const LLRF_CH2_INTERLOCK = "LLRF_CH2_INTERLOCK";
	std::string const LLRF_CH3_INTERLOCK = "LLRF_CH3_INTERLOCK";
	std::string const LLRF_CH4_INTERLOCK = "LLRF_CH4_INTERLOCK";
	std::string const LLRF_CH5_INTERLOCK = "LLRF_CH5_INTERLOCK";
	std::string const LLRF_CH6_INTERLOCK = "LLRF_CH6_INTERLOCK";
	std::string const LLRF_CH7_INTERLOCK = "LLRF_CH7_INTERLOCK";
	std::string const LLRF_CH8_INTERLOCK = "LLRF_CH8_INTERLOCK";

	// these are the power remote for each channel
	std::string const CH1_PWR_REM = "CH1_PWR_REM";
	std::string const CH2_PWR_REM = "CH2_PWR_REM";
	std::string const CH3_PWR_REM = "CH3_PWR_REM";
	std::string const CH4_PWR_REM = "CH4_PWR_REM";
	std::string const CH5_PWR_REM = "CH5_PWR_REM";
	std::string const CH6_PWR_REM = "CH6_PWR_REM";
	std::string const CH7_PWR_REM = "CH7_PWR_REM";
	std::string const CH8_PWR_REM = "CH8_PWR_REM";
	// these are the power local for each channel
	std::string const CH1_PWR_LOC = "CH1_PWR_LOC";
	std::string const CH2_PWR_LOC = "CH2_PWR_LOC";
	std::string const CH3_PWR_LOC = "CH3_PWR_LOC";
	std::string const CH4_PWR_LOC = "CH4_PWR_LOC";
	std::string const CH5_PWR_LOC = "CH5_PWR_LOC";
	std::string const CH6_PWR_LOC = "CH6_PWR_LOC";
	std::string const CH7_PWR_LOC = "CH7_PWR_LOC";
	std::string const CH8_PWR_LOC = "CH8_PWR_LOC";
	// these are the amplitude derivative for each channel
	std::string const CH1_AMP_DER = "CH1_AMP_DER";
	std::string const CH2_AMP_DER = "CH2_AMP_DER";
	std::string const CH3_AMP_DER = "CH3_AMP_DER";
	std::string const CH4_AMP_DER = "CH4_AMP_DER";
	std::string const CH5_AMP_DER = "CH5_AMP_DER";
	std::string const CH6_AMP_DER = "CH6_AMP_DER";
	std::string const CH7_AMP_DER = "CH7_AMP_DER";
	std::string const CH8_AMP_DER = "CH8_AMP_DER";
	// these are the phase remote for each channel
	std::string const CH1_PHASE_REM = "CH1_PHASE_REM";
	std::string const CH2_PHASE_REM = "CH2_PHASE_REM";
	std::string const CH3_PHASE_REM = "CH3_PHASE_REM";
	std::string const CH4_PHASE_REM = "CH4_PHASE_REM";
	std::string const CH5_PHASE_REM = "CH5_PHASE_REM";
	std::string const CH6_PHASE_REM = "CH6_PHASE_REM";
	std::string const CH7_PHASE_REM = "CH7_PHASE_REM";
	std::string const CH8_PHASE_REM = "CH8_PHASE_REM";
	// these are the phase derivative for each channel
	std::string const CH1_PHASE_DER = "CH1_PHASE_DER";
	std::string const CH2_PHASE_DER = "CH2_PHASE_DER";
	std::string const CH3_PHASE_DER = "CH3_PHASE_DER";
	std::string const CH4_PHASE_DER = "CH4_PHASE_DER";
	std::string const CH5_PHASE_DER = "CH5_PHASE_DER";
	std::string const CH6_PHASE_DER = "CH6_PHASE_DER";
	std::string const CH7_PHASE_DER = "CH7_PHASE_DER";
	std::string const CH8_PHASE_DER = "CH8_PHASE_DER";

	// scan and acqm for each channel
	const std::string 	CH1_PWR_REM_SCAN = "CH1_PWR_REM_SCAN";
	const std::string 	CH1_PWR_REM_ACQM = "CH1_PWR_REM_ACQM";
	const std::string 	CH1_PHASE_REM_SCAN = "CH1_PHASE_REM_SCAN";
	const std::string 	CH1_PHASE_REM_ACQM = "CH1_PHASE_REM_ACQM";
	const std::string 	CH1_AMP_DER_SCAN = "CH1_AMP_DER_SCAN";
	const std::string 	CH1_AMP_DER_ACQM = "CH1_AMP_DER_ACQM";
	const std::string 	CH1_PHASE_DER_SCAN = "CH1_PHASE_DER_SCAN";
	const std::string 	CH1_PHASE_DER_ACQM = "CH1_PHASE_DER_ACQM";
	const std::string 	CH1_PWR_LOC_SCAN = "CH1_PWR_LOC_SCAN";
	const std::string 	CH1_PWR_LOC_ACQM = "CH1_PWR_LOC_ACQM";

	const std::string 	CH2_PWR_REM_SCAN = "CH2_PWR_REM_SCAN";
	const std::string 	CH2_PWR_REM_ACQM = "CH2_PWR_REM_ACQM";
	const std::string 	CH2_PHASE_REM_SCAN = "CH2_PHASE_REM_SCAN";
	const std::string 	CH2_PHASE_REM_ACQM = "CH2_PHASE_REM_ACQM";
	const std::string 	CH2_AMP_DER_SCAN = "CH2_AMP_DER_SCAN";
	const std::string 	CH2_AMP_DER_ACQM = "CH2_AMP_DER_ACQM";
	const std::string 	CH2_PHASE_DER_SCAN = "CH2_PHASE_DER_SCAN";
	const std::string 	CH2_PHASE_DER_ACQM = "CH2_PHASE_DER_ACQM";
	const std::string 	CH2_PWR_LOC_SCAN = "CH2_PWR_LOC_SCAN";
	const std::string 	CH2_PWR_LOC_ACQM = "CH2_PWR_LOC_ACQM";

	const std::string 	CH3_PWR_REM_SCAN = "CH3_PWR_REM_SCAN";
	const std::string 	CH3_PWR_REM_ACQM = "CH3_PWR_REM_ACQM";
	const std::string 	CH3_PHASE_REM_SCAN = "CH3_PHASE_REM_SCAN";
	const std::string 	CH3_PHASE_REM_ACQM = "CH3_PHASE_REM_ACQM";
	const std::string 	CH3_AMP_DER_SCAN = "CH3_AMP_DER_SCAN";
	const std::string 	CH3_AMP_DER_ACQM = "CH3_AMP_DER_ACQM";
	const std::string 	CH3_PHASE_DER_SCAN = "CH3_PHASE_DER_SCAN";
	const std::string 	CH3_PHASE_DER_ACQM = "CH3_PHASE_DER_ACQM";
	const std::string 	CH3_PWR_LOC_SCAN = "CH3_PWR_LOC_SCAN";
	const std::string 	CH3_PWR_LOC_ACQM = "CH3_PWR_LOC_ACQM";

	const std::string 	CH4_PWR_REM_SCAN = "CH4_PWR_REM_SCAN";
	const std::string 	CH4_PWR_REM_ACQM = "CH4_PWR_REM_ACQM";
	const std::string 	CH4_PHASE_REM_SCAN = "CH4_PHASE_REM_SCAN";
	const std::string 	CH4_PHASE_REM_ACQM = "CH4_PHASE_REM_ACQM";
	const std::string 	CH4_AMP_DER_SCAN = "CH4_AMP_DER_SCAN";
	const std::string 	CH4_AMP_DER_ACQM = "CH4_AMP_DER_ACQM";
	const std::string 	CH4_PHASE_DER_SCAN = "CH4_PHASE_DER_SCAN";
	const std::string 	CH4_PHASE_DER_ACQM = "CH4_PHASE_DER_ACQM";
	const std::string 	CH4_PWR_LOC_SCAN = "CH4_PWR_LOC_SCAN";
	const std::string 	CH4_PWR_LOC_ACQM = "CH4_PWR_LOC_ACQM";

	const std::string 	CH5_PWR_REM_SCAN = "CH5_PWR_REM_SCAN";
	const std::string 	CH5_PWR_REM_ACQM = "CH5_PWR_REM_ACQM";
	const std::string 	CH5_PHASE_REM_SCAN = "CH5_PHASE_REM_SCAN";
	const std::string 	CH5_PHASE_REM_ACQM = "CH5_PHASE_REM_ACQM";
	const std::string 	CH5_AMP_DER_SCAN = "CH5_AMP_DER_SCAN";
	const std::string 	CH5_AMP_DER_ACQM = "CH5_AMP_DER_ACQM";
	const std::string 	CH5_PHASE_DER_SCAN = "CH5_PHASE_DER_SCAN";
	const std::string 	CH5_PHASE_DER_ACQM = "CH5_PHASE_DER_ACQM";
	const std::string 	CH5_PWR_LOC_SCAN = "CH5_PWR_LOC_SCAN";
	const std::string 	CH5_PWR_LOC_ACQM = "CH5_PWR_LOC_ACQM";

	const std::string 	CH6_PWR_REM_SCAN = "CH6_PWR_REM_SCAN";
	const std::string 	CH6_PWR_REM_ACQM = "CH6_PWR_REM_ACQM";
	const std::string 	CH6_PHASE_REM_SCAN = "CH6_PHASE_REM_SCAN";
	const std::string 	CH6_PHASE_REM_ACQM = "CH6_PHASE_REM_ACQM";
	const std::string 	CH6_AMP_DER_SCAN = "CH6_AMP_DER_SCAN";
	const std::string 	CH6_AMP_DER_ACQM = "CH6_AMP_DER_ACQM";
	const std::string 	CH6_PHASE_DER_SCAN = "CH6_PHASE_DER_SCAN";
	const std::string 	CH6_PHASE_DER_ACQM = "CH6_PHASE_DER_ACQM";
	const std::string 	CH6_PWR_LOC_SCAN = "CH6_PWR_LOC_SCAN";
	const std::string 	CH6_PWR_LOC_ACQM = "CH6_PWR_LOC_ACQM";

	const std::string 	CH7_PWR_REM_SCAN = "CH7_PWR_REM_SCAN";
	const std::string 	CH7_PWR_REM_ACQM = "CH7_PWR_REM_ACQM";
	const std::string 	CH7_PHASE_REM_SCAN = "CH7_PHASE_REM_SCAN";
	const std::string 	CH7_PHASE_REM_ACQM = "CH7_PHASE_REM_ACQM";
	const std::string 	CH7_AMP_DER_SCAN = "CH7_AMP_DER_SCAN";
	const std::string 	CH7_AMP_DER_ACQM = "CH7_AMP_DER_ACQM";
	const std::string 	CH7_PHASE_DER_SCAN = "CH7_PHASE_DER_SCAN";
	const std::string 	CH7_PHASE_DER_ACQM = "CH7_PHASE_DER_ACQM";
	const std::string 	CH7_PWR_LOC_SCAN = "CH7_PWR_LOC_SCAN";
	const std::string 	CH7_PWR_LOC_ACQM = "CH7_PWR_LOC_ACQM";

	const std::string 	CH8_PWR_REM_SCAN = "CH8_PWR_REM_SCAN";
	const std::string 	CH8_PWR_REM_ACQM = "CH8_PWR_REM_ACQM";
	const std::string 	CH8_PHASE_REM_SCAN = "CH8_PHASE_REM_SCAN";
	const std::string 	CH8_PHASE_REM_ACQM = "CH8_PHASE_REM_ACQM";
	const std::string 	CH8_AMP_DER_SCAN = "CH8_AMP_DER_SCAN";
	const std::string 	CH8_AMP_DER_ACQM = "CH8_AMP_DER_ACQM";
	const std::string 	CH8_PHASE_DER_SCAN = "CH8_PHASE_DER_SCAN";
	const std::string 	CH8_PHASE_DER_ACQM = "CH8_PHASE_DER_ACQM";
	const std::string 	CH8_PWR_LOC_SCAN = "CH8_PWR_LOC_SCAN";
	const std::string 	CH8_PWR_LOC_ACQM = "CH8_PWR_LOC_ACQM";

	
	const std::vector<std::string> llrfRecordList = { HEART_BEAT, AMP_FF,
TRIG_SOURCE,
AMP_SP,
MAX_AMP_SP,
PHI_FF,
PHI_SP,
RF_OUTPUT,
FF_PHASE_LOCK_STATE,
FF_AMP_LOCK_STATE,
TIME_VECTOR,
PULSE_OFFSET,
LLRF_PULSE_DURATION,
INTERLOCK,
PULSE_SHAPE,
PULSE_SHAPE_APPLY,
	LLRF_TRACES,
	LLRF_TRACES_SCAN,
	LLRF_TRACES_ACQM,
CH1_INTERLOCK_STATUS,CH2_INTERLOCK_STATUS,CH3_INTERLOCK_STATUS,CH4_INTERLOCK_STATUS,
CH5_INTERLOCK_STATUS,CH6_INTERLOCK_STATUS,CH7_INTERLOCK_STATUS,CH8_INTERLOCK_STATUS,

CH1_INTERLOCK_P, CH2_INTERLOCK_P, CH3_INTERLOCK_P, CH4_INTERLOCK_P,
CH5_INTERLOCK_P, CH6_INTERLOCK_P, CH7_INTERLOCK_P, CH8_INTERLOCK_P,

CH1_INTERLOCK_U, CH2_INTERLOCK_U, CH3_INTERLOCK_U, CH4_INTERLOCK_U,
CH5_INTERLOCK_U, CH6_INTERLOCK_U, CH7_INTERLOCK_U, CH8_INTERLOCK_U,


CH1_INTERLOCK_ENABLE, CH2_INTERLOCK_ENABLE, CH3_INTERLOCK_ENABLE, CH4_INTERLOCK_ENABLE, 
CH5_INTERLOCK_ENABLE, CH6_INTERLOCK_ENABLE, CH7_INTERLOCK_ENABLE, CH8_INTERLOCK_ENABLE,

CH1_INTERLOCK_PDBM, CH2_INTERLOCK_PDBM, CH3_INTERLOCK_PDBM, CH4_INTERLOCK_PDBM,
CH5_INTERLOCK_PDBM, CH6_INTERLOCK_PDBM, CH7_INTERLOCK_PDBM, CH8_INTERLOCK_PDBM,

// SCAN paramter for each possible trace  (one record SCAN is handled above!
CH1_PWR_REM_SCAN, CH1_PHASE_REM_SCAN, CH1_AMP_DER_SCAN, CH1_PHASE_DER_SCAN, CH1_PWR_LOC_SCAN,
CH2_PWR_REM_SCAN, CH2_PHASE_REM_SCAN, CH2_AMP_DER_SCAN, CH2_PHASE_DER_SCAN, CH2_PWR_LOC_SCAN,
CH3_PWR_REM_SCAN, CH3_PHASE_REM_SCAN, CH3_AMP_DER_SCAN, CH3_PHASE_DER_SCAN, CH3_PWR_LOC_SCAN,
CH4_PWR_REM_SCAN, CH4_PHASE_REM_SCAN, CH4_AMP_DER_SCAN, CH4_PHASE_DER_SCAN, CH4_PWR_LOC_SCAN,
CH5_PWR_REM_SCAN, CH5_PHASE_REM_SCAN, CH5_AMP_DER_SCAN, CH5_PHASE_DER_SCAN, CH5_PWR_LOC_SCAN,
CH6_PWR_REM_SCAN, CH6_PHASE_REM_SCAN, CH6_AMP_DER_SCAN, CH6_PHASE_DER_SCAN, CH6_PWR_LOC_SCAN,
CH7_PWR_REM_SCAN, CH7_PHASE_REM_SCAN, CH7_AMP_DER_SCAN, CH7_PHASE_DER_SCAN, CH7_PWR_LOC_SCAN,
CH8_PWR_REM_SCAN, CH8_PHASE_REM_SCAN, CH8_AMP_DER_SCAN, CH8_PHASE_DER_SCAN, CH8_PWR_LOC_SCAN,
// ACQM paramter for each possible trace  (one record ACQM is handled bove!
CH1_PWR_REM_ACQM, CH1_PHASE_REM_ACQM, CH1_AMP_DER_ACQM, CH1_PHASE_DER_ACQM, CH1_PWR_LOC_ACQM,
CH2_PWR_REM_ACQM, CH2_PHASE_REM_ACQM, CH2_AMP_DER_ACQM, CH2_PHASE_DER_ACQM, CH2_PWR_LOC_ACQM,
CH3_PWR_REM_ACQM, CH3_PHASE_REM_ACQM, CH3_AMP_DER_ACQM, CH3_PHASE_DER_ACQM, CH3_PWR_LOC_ACQM,
CH4_PWR_REM_ACQM, CH4_PHASE_REM_ACQM, CH4_AMP_DER_ACQM, CH4_PHASE_DER_ACQM, CH4_PWR_LOC_ACQM,
CH5_PWR_REM_ACQM, CH5_PHASE_REM_ACQM, CH5_AMP_DER_ACQM, CH5_PHASE_DER_ACQM, CH5_PWR_LOC_ACQM,
CH6_PWR_REM_ACQM, CH6_PHASE_REM_ACQM, CH6_AMP_DER_ACQM, CH6_PHASE_DER_ACQM, CH6_PWR_LOC_ACQM,
CH7_PWR_REM_ACQM, CH7_PHASE_REM_ACQM, CH7_AMP_DER_ACQM, CH7_PHASE_DER_ACQM, CH7_PWR_LOC_ACQM,
CH8_PWR_REM_ACQM, CH8_PHASE_REM_ACQM, CH8_AMP_DER_ACQM, CH8_PHASE_DER_ACQM, CH8_PWR_LOC_ACQM
	};



	const std::vector<std::string>  llrfMonitorRecordsList = { 
		AMP_FF,
		TRIG_SOURCE,
		AMP_SP,
		MAX_AMP_SP,
		PHI_FF,
		PHI_SP,
		RF_OUTPUT,
		FF_PHASE_LOCK_STATE,
		FF_AMP_LOCK_STATE,
		TIME_VECTOR,
		PULSE_OFFSET,
		LLRF_PULSE_DURATION,
		INTERLOCK,
		PULSE_SHAPE,
		// /// //  NO NEVER ! just a command PULSE_SHAPE_APPLY
		//LLRF_TRACES,
		LLRF_TRACES_SCAN,
		LLRF_TRACES_ACQM,

	// interloks status, for each channel POWER only 
	CH1_INTERLOCK_STATUS,	CH2_INTERLOCK_STATUS,	CH3_INTERLOCK_STATUS,	CH4_INTERLOCK_STATUS,
	CH5_INTERLOCK_STATUS,	CH6_INTERLOCK_STATUS,	CH7_INTERLOCK_STATUS,	CH8_INTERLOCK_STATUS,

	CH1_INTERLOCK_P, CH2_INTERLOCK_P, CH3_INTERLOCK_P, CH4_INTERLOCK_P, CH5_INTERLOCK_P, CH6_INTERLOCK_P, CH7_INTERLOCK_P, CH8_INTERLOCK_P, 

	CH1_INTERLOCK_U, CH2_INTERLOCK_U, CH3_INTERLOCK_U, CH4_INTERLOCK_U, CH5_INTERLOCK_U, CH6_INTERLOCK_U, CH7_INTERLOCK_U, CH8_INTERLOCK_U,

	CH1_INTERLOCK_ENABLE, CH2_INTERLOCK_ENABLE, CH3_INTERLOCK_ENABLE, CH4_INTERLOCK_ENABLE, 
	CH5_INTERLOCK_ENABLE, CH6_INTERLOCK_ENABLE, CH7_INTERLOCK_ENABLE, CH8_INTERLOCK_ENABLE,

	CH5_INTERLOCK_PDBM, CH6_INTERLOCK_PDBM, CH7_INTERLOCK_PDBM, CH8_INTERLOCK_PDBM,
	CH1_INTERLOCK_PDBM, CH2_INTERLOCK_PDBM, CH3_INTERLOCK_PDBM, CH4_INTERLOCK_PDBM,

	//// SCAN paramter for each possible trace  (one record SCAN is handled bove!
	CH1_PWR_REM_SCAN, CH1_PHASE_REM_SCAN, CH1_AMP_DER_SCAN, CH1_PHASE_DER_SCAN, CH1_PWR_LOC_SCAN,
	CH2_PWR_REM_SCAN, CH2_PHASE_REM_SCAN, CH2_AMP_DER_SCAN, CH2_PHASE_DER_SCAN, CH2_PWR_LOC_SCAN,
	CH3_PWR_REM_SCAN, CH3_PHASE_REM_SCAN, CH3_AMP_DER_SCAN, CH3_PHASE_DER_SCAN, CH3_PWR_LOC_SCAN,
	CH4_PWR_REM_SCAN, CH4_PHASE_REM_SCAN, CH4_AMP_DER_SCAN, CH4_PHASE_DER_SCAN, CH4_PWR_LOC_SCAN,
	CH5_PWR_REM_SCAN, CH5_PHASE_REM_SCAN, CH5_AMP_DER_SCAN, CH5_PHASE_DER_SCAN, CH5_PWR_LOC_SCAN,
	CH6_PWR_REM_SCAN, CH6_PHASE_REM_SCAN, CH6_AMP_DER_SCAN, CH6_PHASE_DER_SCAN, CH6_PWR_LOC_SCAN,
	CH7_PWR_REM_SCAN, CH7_PHASE_REM_SCAN, CH7_AMP_DER_SCAN, CH7_PHASE_DER_SCAN, CH7_PWR_LOC_SCAN,
	CH8_PWR_REM_SCAN, CH8_PHASE_REM_SCAN, CH8_AMP_DER_SCAN, CH8_PHASE_DER_SCAN, CH8_PWR_LOC_SCAN,

	//// ACQM paramter for each possible trace  (one record ACQM is handled bove!
	CH1_PWR_REM_ACQM, CH1_PHASE_REM_ACQM, CH1_AMP_DER_ACQM, CH1_PHASE_DER_ACQM, CH1_PWR_LOC_ACQM,
	CH2_PWR_REM_ACQM, CH2_PHASE_REM_ACQM, CH2_AMP_DER_ACQM, CH2_PHASE_DER_ACQM, CH2_PWR_LOC_ACQM,
	CH3_PWR_REM_ACQM, CH3_PHASE_REM_ACQM, CH3_AMP_DER_ACQM, CH3_PHASE_DER_ACQM, CH3_PWR_LOC_ACQM,
	CH4_PWR_REM_ACQM, CH4_PHASE_REM_ACQM, CH4_AMP_DER_ACQM, CH4_PHASE_DER_ACQM, CH4_PWR_LOC_ACQM,
	CH5_PWR_REM_ACQM, CH5_PHASE_REM_ACQM, CH5_AMP_DER_ACQM, CH5_PHASE_DER_ACQM, CH5_PWR_LOC_ACQM,
	CH6_PWR_REM_ACQM, CH6_PHASE_REM_ACQM, CH6_AMP_DER_ACQM, CH6_PHASE_DER_ACQM, CH6_PWR_LOC_ACQM,
	CH7_PWR_REM_ACQM, CH7_PHASE_REM_ACQM, CH7_AMP_DER_ACQM, CH7_PHASE_DER_ACQM, CH7_PWR_LOC_ACQM,
	CH8_PWR_REM_ACQM, CH8_PHASE_REM_ACQM, CH8_AMP_DER_ACQM, CH8_PHASE_DER_ACQM, CH8_PWR_LOC_ACQM

	};


	//lrrg_names = 


	//std::map<std::string, TYPE> const llrf_gun_types = {
	//	   {TYPE::VELA_GUN, GUN},
	//	   {TYPE::CLARA_GUN, GUN},
	//	   {TYPE::HRRG, GUN},
	//	   {TYPE::LRRG, GUN},
	//	   {TYPE::CLARA_HRRG, GUN},
	//	   {TYPE::CLARA_LRRG, GUN},
	//	   {TYPE::VELA_HRRG, GUN},
	//	   {TYPE::VELA_LRRG, GUN},
	//	   {TYPE::LRRG_GUN, GUN},
	//	   {TYPE::INJ, GUN},
	//	   {TYPE::INJ, GUN},
	//	   {TYPE::VELA_INJ, GUN}
	//};
	//	;
	//extern std::map<TYPE, std::string> const llrf_type_to_string_map;


	const std::vector<std::string> all_scan_pvs{
		CH1_PWR_REM_SCAN, CH1_PHASE_REM_SCAN, CH1_AMP_DER_SCAN, CH1_PHASE_DER_SCAN, CH1_PWR_LOC_SCAN,
		CH2_PWR_REM_SCAN, CH2_PHASE_REM_SCAN, CH2_AMP_DER_SCAN, CH2_PHASE_DER_SCAN, CH2_PWR_LOC_SCAN,
		CH3_PWR_REM_SCAN, CH3_PHASE_REM_SCAN, CH3_AMP_DER_SCAN, CH3_PHASE_DER_SCAN, CH3_PWR_LOC_SCAN,
		CH4_PWR_REM_SCAN, CH4_PHASE_REM_SCAN, CH4_AMP_DER_SCAN, CH4_PHASE_DER_SCAN, CH4_PWR_LOC_SCAN,
		CH5_PWR_REM_SCAN, CH5_PHASE_REM_SCAN, CH5_AMP_DER_SCAN, CH5_PHASE_DER_SCAN, CH5_PWR_LOC_SCAN,
		CH6_PWR_REM_SCAN, CH6_PHASE_REM_SCAN, CH6_AMP_DER_SCAN, CH6_PHASE_DER_SCAN, CH6_PWR_LOC_SCAN,
		CH7_PWR_REM_SCAN, CH7_PHASE_REM_SCAN, CH7_AMP_DER_SCAN, CH7_PHASE_DER_SCAN, CH7_PWR_LOC_SCAN,
		CH8_PWR_REM_SCAN, CH8_PHASE_REM_SCAN, CH8_AMP_DER_SCAN, CH8_PHASE_DER_SCAN, CH8_PWR_LOC_SCAN };


	const std::map<STATE, unsigned short> state_to_llrf_SCAN_map =
	{
		{ STATE::PASSIVE, 0},
		{ STATE::EVENT, 1 },
		{ STATE::IO_INTR, 2 },
		{ STATE::TEN, 3 },
		{ STATE::FIVE, 4 },
		{ STATE::TWO, 5 },
		{ STATE::ONE, 6 },
		{ STATE::ZERO_POINT_FIVE, 7 },
		{ STATE::ZERO_POINT_TWO, 8 },
		{ STATE::ZERO_POINT_ONE, 9 },
		{ STATE::ZERO_POINT_ZERO_FIVE, 10 },
	};
	const std::map<STATE, unsigned short> state_to_llrf_ACQM_map =
	{
		{ STATE::NOW, 1},
		{ STATE::EVENT, 2 }
	};

}

