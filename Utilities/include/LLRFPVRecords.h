#ifndef LLRF_PV_RECORDS_
#define LLRF_PV_RECORDS_
#include <string>
#include <vector>

#include "GlobalStateEnums.h"
#include "GlobalConstants.h"


namespace LLRFRecords
{
extern std::string const HEART_BEAT;
extern std::string const AMP_FF;
extern std::string const TRIG_SOURCE;
extern std::string const AMP_SP;
extern std::string const MAX_AMP_SP;
extern std::string const PHI_FF                 ; 
extern std::string const PHI_SP 				; 
extern std::string const RF_OUTPUT				; 
extern std::string const FF_PHASE_LOCK_STATE	; 
extern std::string const FF_AMP_LOCK_STATE		; 
extern std::string const TIME_VECTOR			; 
extern std::string const PULSE_OFFSET			; 
extern std::string const LLRF_PULSE_DURATION			; 
extern std::string const INTERLOCK				; 
extern std::string const PULSE_SHAPE			; 
extern std::string const PULSE_SHAPE_APPLY		; 
extern std::string const CH1_INTERLOCK_STATUS  	; 
extern std::string const CH1_INTERLOCK_ENABLE	; 
extern std::string const CH1_INTERLOCK_U		; 
extern std::string const CH1_INTERLOCK_P		; 
extern std::string const CH1_INTERLOCK_PDBM		; 
extern std::string const CH2_INTERLOCK_STATUS	; 
extern std::string const CH2_INTERLOCK_ENABLE	; 
extern std::string const CH2_INTERLOCK_U		; 
extern std::string const CH2_INTERLOCK_P		; 
extern std::string const CH2_INTERLOCK_PDBM		; 
extern std::string const CH3_INTERLOCK_STATUS	; 
extern std::string const CH3_INTERLOCK_ENABLE	; 
extern std::string const CH3_INTERLOCK_U		; 
extern std::string const CH3_INTERLOCK_P		; 
extern std::string const CH3_INTERLOCK_PDBM		; 
extern std::string const CH4_INTERLOCK_STATUS	; 
extern std::string const CH4_INTERLOCK_ENABLE	; 
extern std::string const CH4_INTERLOCK_U		; 
extern std::string const CH4_INTERLOCK_P		; 
extern std::string const CH4_INTERLOCK_PDBM		; 
extern std::string const CH5_INTERLOCK_STATUS	; 
extern std::string const CH5_INTERLOCK_ENABLE	; 
extern std::string const CH5_INTERLOCK_U		; 
extern std::string const CH5_INTERLOCK_P		; 
extern std::string const CH5_INTERLOCK_PDBM		; 
extern std::string const CH6_INTERLOCK_STATUS	; 
extern std::string const CH6_INTERLOCK_ENABLE	; 
extern std::string const CH6_INTERLOCK_U		; 
extern std::string const CH6_INTERLOCK_P		; 
extern std::string const CH6_INTERLOCK_PDBM		; 
extern std::string const CH7_INTERLOCK_STATUS	; 
extern std::string const CH7_INTERLOCK_ENABLE	; 
extern std::string const CH7_INTERLOCK_U		; 
extern std::string const CH7_INTERLOCK_P		; 
extern std::string const CH7_INTERLOCK_PDBM		; 
extern std::string const CH8_INTERLOCK_STATUS	; 
extern std::string const CH8_INTERLOCK_ENABLE	; 
extern std::string const CH8_INTERLOCK_U		; 
extern std::string const CH8_INTERLOCK_P		; 
extern std::string const CH8_INTERLOCK_PDBM		; 
extern std::string const LLRF_TRACES			; 
extern std::string const LLRF_TRACES_SCAN		; 
extern std::string const LLRF_TRACES_ACQM		; 



extern std::string const CH1_PWR_REM;
extern std::string const CH2_PWR_REM;
extern std::string const CH3_PWR_REM;
extern std::string const CH4_PWR_REM;
extern std::string const CH5_PWR_REM;
extern std::string const CH6_PWR_REM;
extern std::string const CH7_PWR_REM;
extern std::string const CH8_PWR_REM;
extern std::string const CH1_AMP_DER;
extern std::string const CH2_AMP_DER;
extern std::string const CH3_AMP_DER;
extern std::string const CH4_AMP_DER;
extern std::string const CH5_AMP_DER;
extern std::string const CH6_AMP_DER;
extern std::string const CH7_AMP_DER;
extern std::string const CH8_AMP_DER;
extern std::string const CH1_PWR_LOC;
extern std::string const CH2_PWR_LOC;
extern std::string const CH3_PWR_LOC;
extern std::string const CH4_PWR_LOC;
extern std::string const CH5_PWR_LOC;
extern std::string const CH6_PWR_LOC;
extern std::string const CH7_PWR_LOC;
extern std::string const CH8_PWR_LOC;

extern std::string const CH1_PHASE_REM;
extern std::string const CH2_PHASE_REM;
extern std::string const CH3_PHASE_REM;
extern std::string const CH4_PHASE_REM;
extern std::string const CH5_PHASE_REM;
extern std::string const CH6_PHASE_REM;
extern std::string const CH7_PHASE_REM;
extern std::string const CH8_PHASE_REM;
									  
extern std::string const CH1_PHASE_DER;
extern std::string const CH2_PHASE_DER;
extern std::string const CH3_PHASE_DER;
extern std::string const CH4_PHASE_DER;
extern std::string const CH5_PHASE_DER;
extern std::string const CH6_PHASE_DER;
extern std::string const CH7_PHASE_DER;
extern std::string const CH8_PHASE_DER;






extern std::string const ONE_TRACE_SCAN;
extern std::string const ONE_TRACE_ACQM;


extern const std::string ONE_RECORD_KLYSTRON_FORWARD_POWER;
extern const std::string ONE_RECORD_KLYSTRON_FORWARD_PHASE;
extern const std::string ONE_RECORD_KLYSTRON_REVERSE_POWER;
extern const std::string ONE_RECORD_KLYSTRON_REVERSE_PHASE;
extern const std::string ONE_RECORD_LRRG_CAVITY_FORWARD_POWER;
extern const std::string ONE_RECORD_LRRG_CAVITY_FORWARD_PHASE;
extern const std::string ONE_RECORD_LRRG_CAVITY_REVERSE_POWER;
extern const std::string ONE_RECORD_LRRG_CAVITY_REVERSE_PHASE;
extern const std::string ONE_RECORD_HRRG_CAVITY_FORWARD_POWER;
extern const std::string ONE_RECORD_HRRG_CAVITY_FORWARD_PHASE;
extern const std::string ONE_RECORD_HRRG_CAVITY_REVERSE_POWER;
extern const std::string ONE_RECORD_HRRG_CAVITY_REVERSE_PHASE;
extern const std::string ONE_RECORD_HRRG_CAVITY_PROBE_POWER;
extern const std::string ONE_RECORD_HRRG_CAVITY_PROBE_PHASE;
extern const std::string ONE_RECORD_L01_CAVITY_FORWARD_POWER;
extern const std::string ONE_RECORD_L01_CAVITY_FORWARD_PHASE;
extern const std::string ONE_RECORD_L01_CAVITY_REVERSE_POWER;
extern const std::string ONE_RECORD_L01_CAVITY_REVERSE_PHASE;
extern const std::string ONE_RECORD_L01_CAVITY_PROBE_POWER;
extern const std::string ONE_RECORD_L01_CAVITY_PROBE_PHASE;
extern const std::string ONE_RECORD_CALIBRATION_POWER;
extern const std::string ONE_RECORD_CALIBRATION_PHASE;


extern std::string const CH1_PWR_REM_SCAN		; 
extern std::string const CH1_PWR_REM			; 
extern std::string const CH1_PWR_REM_ACQM		; 
extern std::string const CH1_PHASE_REM_SCAN		; 
extern std::string const CH1_PHASE_REM_ACQM		; 
extern std::string const CH1_PHASE_REM			; 
extern std::string const CH1_AMP_DER_SCAN		; 
extern std::string const CH1_PHASE_DER_SCAN		; 
extern std::string const CH1_PWR_LOC_SCAN		; 
extern std::string const CH2_PWR_REM_SCAN		; 
extern std::string const CH2_PWR_REM			; 
extern std::string const CH2_PWR_REM_ACQM		; 
extern std::string const CH2_PHASE_REM_SCAN		; 
extern std::string const CH2_PHASE_REM_ACQM		; 
extern std::string const CH2_PHASE_REM			; 
extern std::string const CH2_AMP_DER_SCAN		; 
extern std::string const CH2_PHASE_DER_SCAN		; 
extern std::string const CH2_PWR_LOC_SCAN		; 
extern std::string const CH3_PWR_REM_SCAN		; 
extern std::string const CH3_PWR_REM			; 
extern std::string const CH3_PWR_REM_ACQM		; 
extern std::string const CH3_PHASE_REM_SCAN		; 
extern std::string const CH3_PHASE_REM_ACQM		; 
extern std::string const CH3_PHASE_REM			; 
extern std::string const CH3_AMP_DER_SCAN		; 
extern std::string const CH3_PHASE_DER_SCAN		; 
extern std::string const CH3_PWR_LOC_SCAN		; 
extern std::string const CH4_PWR_REM_SCAN		; 
extern std::string const CH4_PWR_REM			; 
extern std::string const CH4_PWR_REM_ACQM		; 
extern std::string const CH4_PHASE_REM_SCAN		; 
extern std::string const CH4_PHASE_REM_ACQM		; 
extern std::string const CH4_PHASE_REM			; 
extern std::string const CH4_AMP_DER_SCAN		; 
extern std::string const CH4_PHASE_DER_SCAN		; 
extern std::string const CH4_PWR_LOC_SCAN		; 
extern std::string const CH5_PWR_REM_SCAN		; 
extern std::string const CH5_PWR_REM			; 
extern std::string const CH5_PWR_REM_ACQM		; 
extern std::string const CH5_PHASE_REM_SCAN		; 
extern std::string const CH5_PHASE_REM_ACQM		; 
extern std::string const CH5_PHASE_REM			; 
extern std::string const CH5_AMP_DER_SCAN		; 
extern std::string const CH5_PHASE_DER_SCAN		; 
extern std::string const CH5_PWR_LOC_SCAN		; 
extern std::string const CH6_PWR_REM_SCAN		; 
extern std::string const CH6_PWR_REM			; 
extern std::string const CH6_PWR_REM_ACQM		; 
extern std::string const CH6_PHASE_REM_SCAN		; 
extern std::string const CH6_PHASE_REM_ACQM		; 
extern std::string const CH6_PHASE_REM			; 
extern std::string const CH6_AMP_DER_SCAN		; 
extern std::string const CH6_PHASE_DER_SCAN		; 
extern std::string const CH6_PWR_LOC_SCAN		; 
extern std::string const CH7_PWR_REM_SCAN		; 
extern std::string const CH7_PWR_REM;
extern std::string const CH7_PWR_REM_ACQM;
extern std::string const CH7_PHASE_REM_SCAN;
extern std::string const CH7_PHASE_REM_ACQM;
extern std::string const CH7_PHASE_REM;
extern std::string const CH7_AMP_DER_SCAN;
extern std::string const CH7_PHASE_DER_SCAN;
extern std::string const CH7_PWR_LOC_SCAN;
extern std::string const CH8_PWR_REM_SCAN;
extern std::string const CH8_PWR_REM;
extern std::string const CH8_PWR_REM_ACQM;
extern std::string const CH8_PHASE_REM_SCAN;
extern std::string const CH8_PHASE_REM_ACQM;
extern std::string const CH8_PHASE_REM;
extern std::string const CH8_AMP_DER_SCAN;
extern std::string const CH8_PHASE_DER_SCAN;
extern std::string const CH8_PWR_LOC_SCAN;              

	extern const std::vector<std::string> llrfRecordList;
	extern const std::vector<std::string> llrfMonitorRecordsList;


	extern std::string const LLRF_CH1_INTERLOCK;
		extern std::string const LLRF_CH2_INTERLOCK;
		extern std::string const LLRF_CH3_INTERLOCK;
		extern std::string const LLRF_CH4_INTERLOCK;
		extern std::string const LLRF_CH5_INTERLOCK;
		extern std::string const LLRF_CH6_INTERLOCK;
		extern std::string const LLRF_CH7_INTERLOCK;
		extern std::string const LLRF_CH8_INTERLOCK;



	/*	const std::string LRRG_CAVITY_REVERSE_PHASE = "LRRG_CAVITY_REVERSE_PHASE";
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

		const std::string KLYSTRON_REVERSE_PHASE = "KLYSTRON_REVERSE_PHASE";
		const std::string KLYSTRON_REVERSE_POWER = "KLYSTRON_REVERSE_POWER";
		const std::string KLYSTRON_FORWARD_PHASE = "KLYSTRON_FORWARD_PHASE";
		const std::string KLYSTRON_FORWARD_POWER = "KLYSTRON_FORWARD_POWER";


		const std::string  KRPOW = "KRPOW";
		const std::string  KRPHA = "KRPHA";
		const std::string  KFPOW = "KFPOW";
		const std::string  KFPHA = "KFPHA";
		const std::string  CRPOW = "CRPOW";
		const std::string  CRPHA = "CRPHA";
		const std::string  CPPOW = "CPPOW";
		const std::string  CPPHA = "CPPHA";
		const std::string  CFPOW = "CFPOW";
		const std::string  CFPHA = "CFPHA";*/


}

namespace LLRFState
{
	struct LLRFStateStruct
	{   // proviude a default constructor
		LLRFStateStruct() 

		{};
	};
}

#endif // LLRF_PV_RECORDS_