#ifndef LLRF_PV_RECORDS_
#define LLRF_PV_RECORDS_
#include <string>
#include <vector>

#include "GlobalStateEnums.h"
#include "GlobalConstants.h"


namespace LLRFRecords
{
extern std::string const KEEP_ALIVE;
extern std::string const AMP_FF;
extern std::string const TRIG_SOURCE;
extern std::string const AMP_SP;
extern std::string const PHI_FF                 ; 
extern std::string const PHI_SP 				; 
extern std::string const RF_OUTPUT				; 
extern std::string const FF_PHASE_LOCK_STATE	; 
extern std::string const FF_AMP_LOCK_STATE		; 
extern std::string const TIME_VECTOR			; 
extern std::string const PULSE_OFFSET			; 
extern std::string const PULSE_LENGTH			; 
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

extern std::vector<std::string> llrfRecordList;
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