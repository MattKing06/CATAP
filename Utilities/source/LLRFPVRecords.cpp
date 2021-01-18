#include <LLRFPVRecords.h>


namespace LLRFRecords
{
	const std::string HEART_BEAT = "HEART_BEAT";
	const std::string AMP_FF = "AMP_FF";
	const std::string TRIG_SOURCE = "TRIG_SOURCE";
	const std::string AMP_SP = "AMP_SP";
	const std::string MAX_AMP_SP = "MAX_AMP_SP";
	const std::string PHI_FF = "PHI_FF";
	const std::string PHI_SP = "PHI_SP";
	const std::string RF_OUTPUT = "RF_OUTPUT";
	const std::string FF_PHASE_LOCK_STATE = "FF_PHASE_LOCK_STATE";
	const std::string FF_AMP_LOCK_STATE = "FF_AMP_LOCK_STATE";
	const std::string TIME_VECTOR = "TIME_VECTOR";
	const std::string PULSE_OFFSET = "PULSE_OFFSET";
	const std::string LLRF_PULSE_DURATION = "LLRF_PULSE_DURATION";
	const std::string INTERLOCK = "INTERLOCK";
	const std::string PULSE_SHAPE = "PULSE_SHAPE";
	const std::string PULSE_SHAPE_APPLY = "PULSE_SHAPE_APPLY";
	const std::string CH1_INTERLOCK_STATUS = "CH1_INTERLOCK_STATUS";
	const std::string CH1_INTERLOCK_ENABLE = "CH1_INTERLOCK_ENABLE";
	const std::string CH1_INTERLOCK_U = "CH1_INTERLOCK_U";
	const std::string CH1_INTERLOCK_P = "CH1_INTERLOCK_P";
	const std::string CH1_INTERLOCK_PDBM = "CH1_INTERLOCK_PDBM";
	const std::string CH2_INTERLOCK_STATUS = "CH2_INTERLOCK_STATUS";
	const std::string CH2_INTERLOCK_ENABLE = "CH2_INTERLOCK_ENABLE";
	const std::string CH2_INTERLOCK_U = "CH2_INTERLOCK_U";
	const std::string CH2_INTERLOCK_P = "CH2_INTERLOCK_P";
	const std::string CH2_INTERLOCK_PDBM = "CH2_INTERLOCK_PDBM";
	const std::string CH3_INTERLOCK_STATUS = "CH3_INTERLOCK_STATUS";
	const std::string CH3_INTERLOCK_ENABLE = "CH3_INTERLOCK_ENABLE";
	const std::string CH3_INTERLOCK_U = "CH3_INTERLOCK_U";
	const std::string CH3_INTERLOCK_P = "CH3_INTERLOCK_P";
	const std::string CH3_INTERLOCK_PDBM = "CH3_INTERLOCK_PDBM";
	const std::string CH4_INTERLOCK_STATUS = "CH4_INTERLOCK_STATUS";
	const std::string CH4_INTERLOCK_ENABLE = "CH4_INTERLOCK_ENABLE";
	const std::string CH4_INTERLOCK_U = "CH4_INTERLOCK_U";
	const std::string CH4_INTERLOCK_P = "CH4_INTERLOCK_P";
	const std::string CH4_INTERLOCK_PDBM = "CH4_INTERLOCK_PDBM";
	const std::string CH5_INTERLOCK_STATUS = "CH5_INTERLOCK_STATUS";
	const std::string CH5_INTERLOCK_ENABLE = "CH5_INTERLOCK_ENABLE";
	const std::string CH5_INTERLOCK_U = "CH5_INTERLOCK_U";
	const std::string CH5_INTERLOCK_P = "CH5_INTERLOCK_P";
	const std::string CH5_INTERLOCK_PDBM = "CH5_INTERLOCK_PDBM";
	const std::string CH6_INTERLOCK_STATUS = "CH6_INTERLOCK_STATUS";
	const std::string CH6_INTERLOCK_ENABLE = "CH6_INTERLOCK_ENABLE";
	const std::string CH6_INTERLOCK_U = "CH6_INTERLOCK_U";
	const std::string CH6_INTERLOCK_P = "CH6_INTERLOCK_P";
	const std::string CH6_INTERLOCK_PDBM = "CH6_INTERLOCK_PDBM";
	const std::string CH7_INTERLOCK_STATUS = "CH7_INTERLOCK_STATUS";
	const std::string CH7_INTERLOCK_ENABLE = "CH7_INTERLOCK_ENABLE";
	const std::string CH7_INTERLOCK_U = "CH7_INTERLOCK_U";
	const std::string CH7_INTERLOCK_P = "CH7_INTERLOCK_P";
	const std::string CH7_INTERLOCK_PDBM = "CH7_INTERLOCK_PDBM";
	const std::string CH8_INTERLOCK_STATUS = "CH8_INTERLOCK_STATUS";
	const std::string CH8_INTERLOCK_ENABLE = "CH8_INTERLOCK_ENABLE";
	const std::string CH8_INTERLOCK_U = "CH8_INTERLOCK_U";
	const std::string CH8_INTERLOCK_P = "CH8_INTERLOCK_P";
	const std::string CH8_INTERLOCK_PDBM = "CH8_INTERLOCK_PDBM";
	const std::string LLRF_TRACES = "LLRF_TRACES";
	const std::string LLRF_TRACES_SCAN = "LLRF_TRACES_SCAN";
	const std::string LLRF_TRACES_ACQM = "LLRF_TRACES_ACQM";


	std::string const CH1_PHASE_REM = "CH1_PHASE_REM";
	std::string const CH2_PHASE_REM = "CH2_PHASE_REM";
	std::string const CH3_PHASE_REM = "CH3_PHASE_REM";
	std::string const CH4_PHASE_REM = "CH4_PHASE_REM";
	std::string const CH5_PHASE_REM = "CH5_PHASE_REM";
	std::string const CH6_PHASE_REM = "CH6_PHASE_REM";
	std::string const CH7_PHASE_REM = "CH7_PHASE_REM";
	std::string const CH8_PHASE_REM = "CH8_PHASE_REM";


	std::string const CH1_PWR_REM = "CH1_PWR_REM";
	std::string const CH2_PWR_REM = "CH2_PWR_REM";
	std::string const CH3_PWR_REM = "CH3_PWR_REM";
	std::string const CH4_PWR_REM = "CH4_PWR_REM";
	std::string const CH5_PWR_REM = "CH5_PWR_REM";
	std::string const CH6_PWR_REM = "CH6_PWR_REM";
	std::string const CH7_PWR_REM = "CH7_PWR_REM";
	std::string const CH8_PWR_REM = "CH8_PWR_REM";


	std::string const CH1_AMP_DER_ACQM = "CH1_AMP_DER_ACQM";
	std::string const CH2_AMP_DER_ACQM = "CH2_AMP_DER_ACQM";
	std::string const CH3_AMP_DER_ACQM = "CH3_AMP_DER_ACQM";
	std::string const CH4_AMP_DER_ACQM = "CH4_AMP_DER_ACQM";
	std::string const CH5_AMP_DER_ACQM = "CH5_AMP_DER_ACQM";
	std::string const CH6_AMP_DER_ACQM = "CH6_AMP_DER_ACQM";
	std::string const CH7_AMP_DER_ACQM = "CH7_AMP_DER_ACQM";
	std::string const CH8_AMP_DER_ACQM = "CH8_AMP_DER_ACQM";
	std::string const CH1_AMP_DER_SCAN = "CH1_AMP_DER_SCAN";
	std::string const CH2_AMP_DER_SCAN = "CH2_AMP_DER_SCAN";
	std::string const CH3_AMP_DER_SCAN = "CH3_AMP_DER_SCAN";
	std::string const CH4_AMP_DER_SCAN = "CH4_AMP_DER_SCAN";
	std::string const CH5_AMP_DER_SCAN = "CH5_AMP_DER_SCAN";
	std::string const CH6_AMP_DER_SCAN = "CH6_AMP_DER_SCAN";
	std::string const CH7_AMP_DER_SCAN = "CH7_AMP_DER_SCAN";
	std::string const CH8_AMP_DER_SCAN = "CH8_AMP_DER_SCAN";

	std::string const CH1_PHASE_DER_ACQM = "CH1_PHASE_DER_ACQM";
	std::string const CH2_PHASE_DER_ACQM = "CH2_PHASE_DER_ACQM";
	std::string const CH3_PHASE_DER_ACQM = "CH3_PHASE_DER_ACQM";
	std::string const CH4_PHASE_DER_ACQM = "CH4_PHASE_DER_ACQM";
	std::string const CH5_PHASE_DER_ACQM = "CH5_PHASE_DER_ACQM";
	std::string const CH6_PHASE_DER_ACQM = "CH6_PHASE_DER_ACQM";
	std::string const CH7_PHASE_DER_ACQM = "CH7_PHASE_DER_ACQM";
	std::string const CH8_PHASE_DER_ACQM = "CH8_PHASE_DER_ACQM";
	std::string const CH1_PHASE_DER_SCAN = "CH1_PHASE_DER_SCAN";
	std::string const CH2_PHASE_DER_SCAN = "CH2_PHASE_DER_SCAN";
	std::string const CH3_PHASE_DER_SCAN = "CH3_PHASE_DER_SCAN";
	std::string const CH4_PHASE_DER_SCAN = "CH4_PHASE_DER_SCAN";
	std::string const CH5_PHASE_DER_SCAN = "CH5_PHASE_DER_SCAN";
	std::string const CH6_PHASE_DER_SCAN = "CH6_PHASE_DER_SCAN";
	std::string const CH7_PHASE_DER_SCAN = "CH7_PHASE_DER_SCAN";
	std::string const CH8_PHASE_DER_SCAN = "CH8_PHASE_DER_SCAN";
	std::string const CH1_PHASE_REM_ACQM = "CH1_PHASE_REM_ACQM";
	std::string const CH2_PHASE_REM_ACQM = "CH2_PHASE_REM_ACQM";
	std::string const CH3_PHASE_REM_ACQM = "CH3_PHASE_REM_ACQM";
	std::string const CH4_PHASE_REM_ACQM = "CH4_PHASE_REM_ACQM";
	std::string const CH5_PHASE_REM_ACQM = "CH5_PHASE_REM_ACQM";
	std::string const CH6_PHASE_REM_ACQM = "CH6_PHASE_REM_ACQM";
	std::string const CH7_PHASE_REM_ACQM = "CH7_PHASE_REM_ACQM";
	std::string const CH8_PHASE_REM_ACQM = "CH8_PHASE_REM_ACQM";
	std::string const CH1_PHASE_REM_SCAN = "CH1_PHASE_REM_SCAN";
	std::string const CH2_PHASE_REM_SCAN = "CH2_PHASE_REM_SCAN";
	std::string const CH3_PHASE_REM_SCAN = "CH3_PHASE_REM_SCAN";
	std::string const CH4_PHASE_REM_SCAN = "CH4_PHASE_REM_SCAN";
	std::string const CH5_PHASE_REM_SCAN = "CH5_PHASE_REM_SCAN";
	std::string const CH6_PHASE_REM_SCAN = "CH6_PHASE_REM_SCAN";
	std::string const CH7_PHASE_REM_SCAN = "CH7_PHASE_REM_SCAN";
	std::string const CH8_PHASE_REM_SCAN = "CH8_PHASE_REM_SCAN";
	std::string const CH1_PWR_LOC_ACQM = "CH1_PWR_LOC_ACQM";
	std::string const CH2_PWR_LOC_ACQM = "CH2_PWR_LOC_ACQM";
	std::string const CH3_PWR_LOC_ACQM = "CH3_PWR_LOC_ACQM";
	std::string const CH4_PWR_LOC_ACQM = "CH4_PWR_LOC_ACQM";
	std::string const CH5_PWR_LOC_ACQM = "CH5_PWR_LOC_ACQM";
	std::string const CH6_PWR_LOC_ACQM = "CH6_PWR_LOC_ACQM";
	std::string const CH7_PWR_LOC_ACQM = "CH7_PWR_LOC_ACQM";
	std::string const CH8_PWR_LOC_ACQM = "CH8_PWR_LOC_ACQM";
	std::string const CH1_PWR_LOC_SCAN= "CH1_PWR_LOC_SCAN";
	std::string const CH2_PWR_LOC_SCAN = "CH2_PWR_LOC_SCAN";
	std::string const CH3_PWR_LOC_SCAN = "CH3_PWR_LOC_SCAN";
	std::string const CH4_PWR_LOC_SCAN = "CH4_PWR_LOC_SCAN";
	std::string const CH5_PWR_LOC_SCAN = "CH5_PWR_LOC_SCAN";
	std::string const CH6_PWR_LOC_SCAN = "CH6_PWR_LOC_SCAN";
	std::string const CH7_PWR_LOC_SCAN = "CH7_PWR_LOC_SCAN";
	std::string const CH8_PWR_LOC_SCAN = "CH8_PWR_LOC_SCAN";

	std::string const CH1_PWR_REM_ACQM = "CH1_PWR_REM_ACQM";
	std::string const CH2_PWR_REM_ACQM = "CH2_PWR_REM_ACQM";
	std::string const CH3_PWR_REM_ACQM = "CH3_PWR_REM_ACQM";
	std::string const CH4_PWR_REM_ACQM = "CH4_PWR_REM_ACQM";
	std::string const CH5_PWR_REM_ACQM = "CH5_PWR_REM_ACQM";
	std::string const CH6_PWR_REM_ACQM = "CH6_PWR_REM_ACQM";
	std::string const CH7_PWR_REM_ACQM = "CH7_PWR_REM_ACQM";
	std::string const CH8_PWR_REM_ACQM = "CH8_PWR_REM_ACQM";

	std::string const CH1_PWR_REM_SCAN = "CH1_PWR_REM_SCAN";
	std::string const CH2_PWR_REM_SCAN = "CH2_PWR_REM_SCAN";
	std::string const CH3_PWR_REM_SCAN = "CH3_PWR_REM_SCAN";
	std::string const CH4_PWR_REM_SCAN = "CH4_PWR_REM_SCAN";
	std::string const CH5_PWR_REM_SCAN = "CH5_PWR_REM_SCAN";
	std::string const CH6_PWR_REM_SCAN = "CH6_PWR_REM_SCAN";
	std::string const CH7_PWR_REM_SCAN = "CH7_PWR_REM_SCAN";
	std::string const CH8_PWR_REM_SCAN = "CH8_PWR_REM_SCAN";

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
CH1_INTERLOCK_STATUS,
CH1_INTERLOCK_ENABLE,
CH1_INTERLOCK_U,
CH1_INTERLOCK_P,
CH1_INTERLOCK_PDBM,
CH2_INTERLOCK_STATUS,
CH2_INTERLOCK_ENABLE,
CH2_INTERLOCK_U,
CH2_INTERLOCK_P,
CH2_INTERLOCK_PDBM,
CH3_INTERLOCK_STATUS,
CH3_INTERLOCK_ENABLE,
CH3_INTERLOCK_U,
CH3_INTERLOCK_P,
CH3_INTERLOCK_PDBM,
CH4_INTERLOCK_STATUS,
CH4_INTERLOCK_ENABLE,
CH4_INTERLOCK_U,
CH4_INTERLOCK_P,
CH4_INTERLOCK_PDBM,
CH5_INTERLOCK_STATUS,
CH5_INTERLOCK_ENABLE,
CH5_INTERLOCK_U,
CH5_INTERLOCK_P,
CH5_INTERLOCK_PDBM,
CH6_INTERLOCK_STATUS,
CH6_INTERLOCK_ENABLE,
CH6_INTERLOCK_U,
CH6_INTERLOCK_P,
CH6_INTERLOCK_PDBM,
CH7_INTERLOCK_STATUS,
CH7_INTERLOCK_ENABLE,
CH7_INTERLOCK_U,
CH7_INTERLOCK_P,
CH7_INTERLOCK_PDBM,
CH8_INTERLOCK_STATUS,
CH8_INTERLOCK_ENABLE,
CH8_INTERLOCK_U,
CH8_INTERLOCK_P,
CH8_INTERLOCK_PDBM,
LLRF_TRACES,
LLRF_TRACES_SCAN,
LLRF_TRACES_ACQM,

CH1_AMP_DER_ACQM,
	CH2_AMP_DER_ACQM,
	CH3_AMP_DER_ACQM,
	CH4_AMP_DER_ACQM,
	CH5_AMP_DER_ACQM,
	CH6_AMP_DER_ACQM,
	CH7_AMP_DER_ACQM,
	CH8_AMP_DER_ACQM,
	CH1_AMP_DER_SCAN,
	CH2_AMP_DER_SCAN,
	CH3_AMP_DER_SCAN,
	CH4_AMP_DER_SCAN,
	CH5_AMP_DER_SCAN,
	CH6_AMP_DER_SCAN,
	CH7_AMP_DER_SCAN,
	CH8_AMP_DER_SCAN,

	CH1_PHASE_DER_ACQM,
	CH2_PHASE_DER_ACQM,
	CH3_PHASE_DER_ACQM,
	CH4_PHASE_DER_ACQM,
	CH5_PHASE_DER_ACQM,
	CH6_PHASE_DER_ACQM,
	CH7_PHASE_DER_ACQM,
	CH8_PHASE_DER_ACQM,

	CH1_PHASE_DER_SCAN,
	CH2_PHASE_DER_SCAN,
	CH3_PHASE_DER_SCAN,
	CH4_PHASE_DER_SCAN,
	CH5_PHASE_DER_SCAN,
	CH6_PHASE_DER_SCAN,
	CH7_PHASE_DER_SCAN,
	CH8_PHASE_DER_SCAN,

	CH1_PHASE_REM_ACQM,
	CH2_PHASE_REM_ACQM,
	CH3_PHASE_REM_ACQM,
	CH4_PHASE_REM_ACQM,
	CH5_PHASE_REM_ACQM,
	CH6_PHASE_REM_ACQM,
	CH7_PHASE_REM_ACQM,
	CH8_PHASE_REM_ACQM,

	CH1_PHASE_REM_SCAN,
	CH2_PHASE_REM_SCAN,
	CH3_PHASE_REM_SCAN,
	CH4_PHASE_REM_SCAN,
	CH5_PHASE_REM_SCAN,
	CH6_PHASE_REM_SCAN,
	CH7_PHASE_REM_SCAN,
	CH8_PHASE_REM_SCAN,

	CH1_PWR_LOC_ACQM,
	CH2_PWR_LOC_ACQM,
	CH3_PWR_LOC_ACQM,
	CH4_PWR_LOC_ACQM,
	CH5_PWR_LOC_ACQM,
	CH6_PWR_LOC_ACQM,
	CH7_PWR_LOC_ACQM,
	CH8_PWR_LOC_ACQM,
	CH1_PWR_LOC_SCAN,
	CH2_PWR_LOC_SCAN,
	CH3_PWR_LOC_SCAN,
	CH4_PWR_LOC_SCAN,
	CH5_PWR_LOC_SCAN,
	CH6_PWR_LOC_SCAN,
	CH7_PWR_LOC_SCAN,
	CH8_PWR_LOC_SCAN,

	CH1_PWR_REM_ACQM,
	CH2_PWR_REM_ACQM,
	CH3_PWR_REM_ACQM,
	CH4_PWR_REM_ACQM,
	CH5_PWR_REM_ACQM,
	CH6_PWR_REM_ACQM,
	CH7_PWR_REM_ACQM,
	CH8_PWR_REM_ACQM,

	CH1_PWR_REM_SCAN,
	CH2_PWR_REM_SCAN,
	CH3_PWR_REM_SCAN,
	CH4_PWR_REM_SCAN,
	CH5_PWR_REM_SCAN,
	CH6_PWR_REM_SCAN,
	CH7_PWR_REM_SCAN,
	CH8_PWR_REM_SCAN
};


	const std::vector<std::string>  llrfMonitorRecordsList = { HEART_BEAT, AMP_FF,
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
	CH1_INTERLOCK_STATUS,
	CH1_INTERLOCK_ENABLE,
	CH1_INTERLOCK_U,
	CH1_INTERLOCK_P,
	CH1_INTERLOCK_PDBM,
	CH2_INTERLOCK_STATUS,
	CH2_INTERLOCK_ENABLE,
	CH2_INTERLOCK_U,
	CH2_INTERLOCK_P,
	CH2_INTERLOCK_PDBM,
	CH3_INTERLOCK_STATUS,
	CH3_INTERLOCK_ENABLE,
	CH3_INTERLOCK_U,
	CH3_INTERLOCK_P,
	CH3_INTERLOCK_PDBM,
	CH4_INTERLOCK_STATUS,
	CH4_INTERLOCK_ENABLE,
	CH4_INTERLOCK_U,
	CH4_INTERLOCK_P,
	CH4_INTERLOCK_PDBM,
	CH5_INTERLOCK_STATUS,
	CH5_INTERLOCK_ENABLE,
	CH5_INTERLOCK_U,
	CH5_INTERLOCK_P,
	CH5_INTERLOCK_PDBM,
	CH6_INTERLOCK_STATUS,
	CH6_INTERLOCK_ENABLE,
	CH6_INTERLOCK_U,
	CH6_INTERLOCK_P,
	CH6_INTERLOCK_PDBM,
	CH7_INTERLOCK_STATUS,
	CH7_INTERLOCK_ENABLE,
	CH7_INTERLOCK_U,
	CH7_INTERLOCK_P,
	CH7_INTERLOCK_PDBM,
	CH8_INTERLOCK_STATUS,
	CH8_INTERLOCK_ENABLE,
	CH8_INTERLOCK_U,
	CH8_INTERLOCK_P,
	CH8_INTERLOCK_PDBM,
	LLRF_TRACES_SCAN,
	LLRF_TRACES_ACQM,


	CH1_AMP_DER_ACQM,
	CH2_AMP_DER_ACQM,
	CH3_AMP_DER_ACQM,
	CH4_AMP_DER_ACQM,
	CH5_AMP_DER_ACQM,
	CH6_AMP_DER_ACQM,
	CH7_AMP_DER_ACQM,
	CH8_AMP_DER_ACQM,
	CH1_AMP_DER_SCAN,
	CH2_AMP_DER_SCAN,
	CH3_AMP_DER_SCAN,
	CH4_AMP_DER_SCAN,
	CH5_AMP_DER_SCAN,
	CH6_AMP_DER_SCAN,
	CH7_AMP_DER_SCAN,
	CH8_AMP_DER_SCAN,

	CH1_PHASE_DER_ACQM,
	CH2_PHASE_DER_ACQM,
	CH3_PHASE_DER_ACQM,
	CH4_PHASE_DER_ACQM,
	CH5_PHASE_DER_ACQM,
	CH6_PHASE_DER_ACQM,
	CH7_PHASE_DER_ACQM,
	CH8_PHASE_DER_ACQM,

	CH1_PHASE_DER_SCAN,
	CH2_PHASE_DER_SCAN,
	CH3_PHASE_DER_SCAN,
	CH4_PHASE_DER_SCAN,
	CH5_PHASE_DER_SCAN,
	CH6_PHASE_DER_SCAN,
	CH7_PHASE_DER_SCAN,
	CH8_PHASE_DER_SCAN,

	CH1_PHASE_REM_ACQM,
	CH2_PHASE_REM_ACQM,
	CH3_PHASE_REM_ACQM,
	CH4_PHASE_REM_ACQM,
	CH5_PHASE_REM_ACQM,
	CH6_PHASE_REM_ACQM,
	CH7_PHASE_REM_ACQM,
	CH8_PHASE_REM_ACQM,

	CH1_PHASE_REM_SCAN,
	CH2_PHASE_REM_SCAN,
	CH3_PHASE_REM_SCAN,
	CH4_PHASE_REM_SCAN,
	CH5_PHASE_REM_SCAN,
	CH6_PHASE_REM_SCAN,
	CH7_PHASE_REM_SCAN,
	CH8_PHASE_REM_SCAN,

	CH1_PWR_LOC_ACQM,
	CH2_PWR_LOC_ACQM,
	CH3_PWR_LOC_ACQM,
	CH4_PWR_LOC_ACQM,
	CH5_PWR_LOC_ACQM,
	CH6_PWR_LOC_ACQM,
	CH7_PWR_LOC_ACQM,
	CH8_PWR_LOC_ACQM,
	CH1_PWR_LOC_SCAN,
	CH2_PWR_LOC_SCAN,
	CH3_PWR_LOC_SCAN,
	CH4_PWR_LOC_SCAN,
	CH5_PWR_LOC_SCAN,
	CH6_PWR_LOC_SCAN,
	CH7_PWR_LOC_SCAN,
	CH8_PWR_LOC_SCAN,

	CH1_PWR_REM_ACQM,
	CH2_PWR_REM_ACQM,
	CH3_PWR_REM_ACQM,
	CH4_PWR_REM_ACQM,
	CH5_PWR_REM_ACQM,
	CH6_PWR_REM_ACQM,
	CH7_PWR_REM_ACQM,
	CH8_PWR_REM_ACQM,

	CH1_PWR_REM_SCAN,
	CH2_PWR_REM_SCAN,
	CH3_PWR_REM_SCAN,
	CH4_PWR_REM_SCAN,
	CH5_PWR_REM_SCAN,
	CH6_PWR_REM_SCAN,
	CH7_PWR_REM_SCAN,
	CH8_PWR_REM_SCAN

	};

}

