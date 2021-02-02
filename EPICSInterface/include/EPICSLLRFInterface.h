#pragma once
#include "EPICSInterface.h"
#include "LLRF.h"
#include "LLRFPVRecords.h"

class LLRF;
typedef void(*updateFunctionPtr)(struct event_handler_args args);
class EPICSLLRFInterface : public EPICSInterface
{
public:
    EPICSLLRFInterface();
    EPICSLLRFInterface(const EPICSLLRFInterface& copyInterface);
    ~EPICSLLRFInterface();
    std::string ownerName;

    void retrieveupdateFunctionForRecord(pvStruct& pvStruct) const;


    static void update_HEART_BEAT(const struct event_handler_args args);
    static void update_AMP_FF(const struct event_handler_args args);
    static void update_TRIG_SOURCE(const struct event_handler_args args);
    static void update_AMP_SP(const struct event_handler_args args);
    static void update_MAX_AMP_SP(const struct event_handler_args args);
    static void update_PHI_FF(const struct event_handler_args args);
    static void update_PHI_SP(const struct event_handler_args args);
    static void update_RF_OUTPUT(const struct event_handler_args args);
    static void update_FF_PHASE_LOCK_STATE(const struct event_handler_args args);
    static void update_FF_AMP_LOCK_STATE(const struct event_handler_args args);
    static void update_TIME_VECTOR(const struct event_handler_args args);
    static void update_PULSE_OFFSET(const struct event_handler_args args);
    static void update_LLRF_PULSE_DURATION(const struct event_handler_args args);
    static void update_INTERLOCK(const struct event_handler_args args);
    static void update_PULSE_SHAPE(const struct event_handler_args args);




    static void updateInterLockStatus(const std::string& ch, const std::string& CH, const struct event_handler_args& args);
    static void update_CH1_INTERLOCK_STATUS(const struct event_handler_args args);
    static void update_CH2_INTERLOCK_STATUS(const struct event_handler_args args);
    static void update_CH3_INTERLOCK_STATUS(const struct event_handler_args args);
    static void update_CH4_INTERLOCK_STATUS(const struct event_handler_args args);
    static void update_CH5_INTERLOCK_STATUS(const struct event_handler_args args);
    static void update_CH6_INTERLOCK_STATUS(const struct event_handler_args args);
    static void update_CH7_INTERLOCK_STATUS(const struct event_handler_args args);
    static void update_CH8_INTERLOCK_STATUS(const struct event_handler_args args);

    static void updateInterLockPDBM(const std::string& ch, const std::string& CH, const struct event_handler_args& args);
    static void update_CH1_INTERLOCK_PDBM(const struct event_handler_args args);
    static void update_CH2_INTERLOCK_PDBM(const struct event_handler_args args);
    static void update_CH3_INTERLOCK_PDBM(const struct event_handler_args args);
    static void update_CH4_INTERLOCK_PDBM(const struct event_handler_args args);
    static void update_CH5_INTERLOCK_PDBM(const struct event_handler_args args);
    static void update_CH6_INTERLOCK_PDBM(const struct event_handler_args args);
    static void update_CH7_INTERLOCK_PDBM(const struct event_handler_args args);
    static void update_CH8_INTERLOCK_PDBM(const struct event_handler_args args);

    static void updateInterLockP(const std::string& ch, const std::string& CH, const struct event_handler_args& args);
    static void update_CH1_INTERLOCK_P(const struct event_handler_args args);
    static void update_CH2_INTERLOCK_P(const struct event_handler_args args);
    static void update_CH3_INTERLOCK_P(const struct event_handler_args args);
    static void update_CH4_INTERLOCK_P(const struct event_handler_args args);
    static void update_CH5_INTERLOCK_P(const struct event_handler_args args);
    static void update_CH6_INTERLOCK_P(const struct event_handler_args args);
    static void update_CH7_INTERLOCK_P(const struct event_handler_args args);
    static void update_CH8_INTERLOCK_P(const struct event_handler_args args);


    static void updateInterLockU(const std::string& ch, const std::string& CH, const struct event_handler_args& args);
    static void update_CH1_INTERLOCK_U(const struct event_handler_args args);
    static void update_CH2_INTERLOCK_U(const struct event_handler_args args);
    static void update_CH3_INTERLOCK_U(const struct event_handler_args args);
    static void update_CH4_INTERLOCK_U(const struct event_handler_args args);
    static void update_CH5_INTERLOCK_U(const struct event_handler_args args);
    static void update_CH6_INTERLOCK_U(const struct event_handler_args args);
    static void update_CH7_INTERLOCK_U(const struct event_handler_args args);
    static void update_CH8_INTERLOCK_U(const struct event_handler_args args);

    static void updateInterLockENABLE(const std::string& ch, const std::string& CH, const struct event_handler_args& args);
    static void update_CH1_INTERLOCK_ENABLE(const struct event_handler_args args);
    static void update_CH2_INTERLOCK_ENABLE(const struct event_handler_args args);
    static void update_CH3_INTERLOCK_ENABLE(const struct event_handler_args args);
    static void update_CH4_INTERLOCK_ENABLE(const struct event_handler_args args);
    static void update_CH5_INTERLOCK_ENABLE(const struct event_handler_args args);
    static void update_CH6_INTERLOCK_ENABLE(const struct event_handler_args args);
    static void update_CH7_INTERLOCK_ENABLE(const struct event_handler_args args);
    static void update_CH8_INTERLOCK_ENABLE(const struct event_handler_args args);


    static void updateChannnelSCAN(const std::string& ch, const std::string& CH, const struct event_handler_args& args);
    static void update_CH1_PWR_REM_SCAN(const struct event_handler_args args);
    static void update_CH1_PHASE_REM_SCAN(const struct event_handler_args args);
    static void update_CH1_AMP_DER_SCAN(const struct event_handler_args args);
    static void update_CH1_PHASE_DER_SCAN(const struct event_handler_args args);
    static void update_CH1_PWR_LOC_SCAN(const struct event_handler_args args);
    static void update_CH2_PWR_REM_SCAN(const struct event_handler_args args);
    static void update_CH2_PHASE_REM_SCAN(const struct event_handler_args args);
    static void update_CH2_AMP_DER_SCAN(const struct event_handler_args args);
    static void update_CH2_PHASE_DER_SCAN(const struct event_handler_args args);
    static void update_CH2_PWR_LOC_SCAN(const struct event_handler_args args);
    static void update_CH3_PWR_REM_SCAN(const struct event_handler_args args);
    static void update_CH3_PHASE_REM_SCAN(const struct event_handler_args args);
    static void update_CH3_AMP_DER_SCAN(const struct event_handler_args args);
    static void update_CH3_PHASE_DER_SCAN(const struct event_handler_args args);
    static void update_CH3_PWR_LOC_SCAN(const struct event_handler_args args);
    static void update_CH4_PWR_REM_SCAN(const struct event_handler_args args);
    static void update_CH4_PHASE_REM_SCAN(const struct event_handler_args args);
    static void update_CH4_AMP_DER_SCAN(const struct event_handler_args args);
    static void update_CH4_PHASE_DER_SCAN(const struct event_handler_args args);
    static void update_CH4_PWR_LOC_SCAN(const struct event_handler_args args);
    static void update_CH5_PWR_REM_SCAN(const struct event_handler_args args);
    static void update_CH5_PHASE_REM_SCAN(const struct event_handler_args args);
    static void update_CH5_AMP_DER_SCAN(const struct event_handler_args args);
    static void update_CH5_PHASE_DER_SCAN(const struct event_handler_args args);
    static void update_CH5_PWR_LOC_SCAN(const struct event_handler_args args);
    static void update_CH6_PWR_REM_SCAN(const struct event_handler_args args);
    static void update_CH6_PHASE_REM_SCAN(const struct event_handler_args args);
    static void update_CH6_AMP_DER_SCAN(const struct event_handler_args args);
    static void update_CH6_PHASE_DER_SCAN(const struct event_handler_args args);
    static void update_CH6_PWR_LOC_SCAN(const struct event_handler_args args);
    static void update_CH7_PWR_REM_SCAN(const struct event_handler_args args);
    static void update_CH7_PHASE_REM_SCAN(const struct event_handler_args args);
    static void update_CH7_AMP_DER_SCAN(const struct event_handler_args args);
    static void update_CH7_PHASE_DER_SCAN(const struct event_handler_args args);
    static void update_CH7_PWR_LOC_SCAN(const struct event_handler_args args);
    static void update_CH8_PWR_REM_SCAN(const struct event_handler_args args);
    static void update_CH8_PHASE_REM_SCAN(const struct event_handler_args args);
    static void update_CH8_AMP_DER_SCAN(const struct event_handler_args args);
    static void update_CH8_PHASE_DER_SCAN(const struct event_handler_args args);
    static void update_CH8_PWR_LOC_SCAN(const struct event_handler_args args);




    static void updateChannnelACQM(const std::string& ch, const std::string& CH, const struct event_handler_args& args);
    static void update_CH1_PWR_REM_ACQM(const struct event_handler_args args);
    static void update_CH1_PHASE_REM_ACQM(const struct event_handler_args args);
    static void update_CH1_AMP_DER_ACQM(const struct event_handler_args args);
    static void update_CH1_PHASE_DER_ACQM(const struct event_handler_args args);
    static void update_CH1_PWR_LOC_ACQM(const struct event_handler_args args);
    static void update_CH2_PWR_REM_ACQM(const struct event_handler_args args);
    static void update_CH2_PHASE_REM_ACQM(const struct event_handler_args args);
    static void update_CH2_AMP_DER_ACQM(const struct event_handler_args args);
    static void update_CH2_PHASE_DER_ACQM(const struct event_handler_args args);
    static void update_CH2_PWR_LOC_ACQM(const struct event_handler_args args);
    static void update_CH3_PWR_REM_ACQM(const struct event_handler_args args);
    static void update_CH3_PHASE_REM_ACQM(const struct event_handler_args args);
    static void update_CH3_AMP_DER_ACQM(const struct event_handler_args args);
    static void update_CH3_PHASE_DER_ACQM(const struct event_handler_args args);
    static void update_CH3_PWR_LOC_ACQM(const struct event_handler_args args);
    static void update_CH4_PWR_REM_ACQM(const struct event_handler_args args);
    static void update_CH4_PHASE_REM_ACQM(const struct event_handler_args args);
    static void update_CH4_AMP_DER_ACQM(const struct event_handler_args args);
    static void update_CH4_PHASE_DER_ACQM(const struct event_handler_args args);
    static void update_CH4_PWR_LOC_ACQM(const struct event_handler_args args);
    static void update_CH5_PWR_REM_ACQM(const struct event_handler_args args);
    static void update_CH5_PHASE_REM_ACQM(const struct event_handler_args args);
    static void update_CH5_AMP_DER_ACQM(const struct event_handler_args args);
    static void update_CH5_PHASE_DER_ACQM(const struct event_handler_args args);
    static void update_CH5_PWR_LOC_ACQM(const struct event_handler_args args);
    static void update_CH6_PWR_REM_ACQM(const struct event_handler_args args);
    static void update_CH6_PHASE_REM_ACQM(const struct event_handler_args args);
    static void update_CH6_AMP_DER_ACQM(const struct event_handler_args args);
    static void update_CH6_PHASE_DER_ACQM(const struct event_handler_args args);
    static void update_CH6_PWR_LOC_ACQM(const struct event_handler_args args);
    static void update_CH7_PWR_REM_ACQM(const struct event_handler_args args);
    static void update_CH7_PHASE_REM_ACQM(const struct event_handler_args args);
    static void update_CH7_AMP_DER_ACQM(const struct event_handler_args args);
    static void update_CH7_PHASE_DER_ACQM(const struct event_handler_args args);
    static void update_CH7_PWR_LOC_ACQM(const struct event_handler_args args);
    static void update_CH8_PWR_REM_ACQM(const struct event_handler_args args);
    static void update_CH8_PHASE_REM_ACQM(const struct event_handler_args args);
    static void update_CH8_AMP_DER_ACQM(const struct event_handler_args args);
    static void update_CH8_PHASE_DER_ACQM(const struct event_handler_args args);
    static void update_CH8_PWR_LOC_ACQM(const struct event_handler_args args);


















    ////static void update_PULSE_SHAPE_APPLY(const struct event_handler_args args);

    static void update_trace_SCAN(const struct event_handler_args& args, std::pair<epicsTimeStamp, STATE >& scan);
    static void update_trace_ACQM(const struct event_handler_args& args, std::pair<epicsTimeStamp, STATE >& acqm);
    
    static void update_LLRF_TRACES_SCAN(const struct event_handler_args args);
    static void update_LLRF_TRACES_ACQM(const struct event_handler_args args);

    static std::map<std::string, STATE> interlock_status_map;
    static std::map<std::string, STATE> interlock_enable_map;

    static void update_LLRF_TRACES(const struct event_handler_args args);

    static LoggingSystem messenger;


    // NB this map is to replace the if-else tree typically used. 
    // it seesm in VS you can only have 127 if-else statements 
    // https://stackoverflow.com/questions/11508013/blocks-nested-too-deeply
    const std::map<std::string, updateFunctionPtr> updateFunctionMap  =  {
    {LLRFRecords::HEART_BEAT , this->update_HEART_BEAT                  },
    {LLRFRecords::AMP_FF, this->update_AMP_FF							},
    {LLRFRecords::TRIG_SOURCE, this->update_TRIG_SOURCE					},
    {LLRFRecords::AMP_SP, this->update_AMP_SP							},
    {LLRFRecords::MAX_AMP_SP, this->update_MAX_AMP_SP                   },
    {LLRFRecords::PHI_FF, this->update_PHI_FF							},
    {LLRFRecords::PHI_SP, this->update_PHI_SP							},
    {LLRFRecords::RF_OUTPUT, this->update_RF_OUTPUT						},
    {LLRFRecords::FF_PHASE_LOCK_STATE, this->update_FF_PHASE_LOCK_STATE },
    {LLRFRecords::FF_AMP_LOCK_STATE, this->update_FF_AMP_LOCK_STATE     },
    {LLRFRecords::TIME_VECTOR, this->update_TIME_VECTOR					},
    {LLRFRecords::PULSE_OFFSET, this->update_PULSE_OFFSET				},
    {LLRFRecords::LLRF_PULSE_DURATION, this->update_LLRF_PULSE_DURATION },
    {LLRFRecords::INTERLOCK, this->update_INTERLOCK						},
    {LLRFRecords::PULSE_SHAPE, this->update_PULSE_SHAPE					},
    //{LLRFRecords::LLRF_TRACES, this->update_LLRF_TRACES },
    {LLRFRecords::LLRF_TRACES_SCAN, this->update_LLRF_TRACES_SCAN	    },
    {LLRFRecords::LLRF_TRACES_ACQM, this->update_LLRF_TRACES_ACQM		},


    {LLRFRecords::CH1_INTERLOCK_STATUS,  this->update_CH1_INTERLOCK_STATUS},
    {LLRFRecords::CH2_INTERLOCK_STATUS,  this->update_CH2_INTERLOCK_STATUS},
    {LLRFRecords::CH3_INTERLOCK_STATUS,  this->update_CH3_INTERLOCK_STATUS},
    {LLRFRecords::CH4_INTERLOCK_STATUS,  this->update_CH4_INTERLOCK_STATUS},
    {LLRFRecords::CH5_INTERLOCK_STATUS,  this->update_CH5_INTERLOCK_STATUS},
    {LLRFRecords::CH6_INTERLOCK_STATUS,  this->update_CH6_INTERLOCK_STATUS},
    {LLRFRecords::CH7_INTERLOCK_STATUS,  this->update_CH7_INTERLOCK_STATUS},
    {LLRFRecords::CH8_INTERLOCK_STATUS,  this->update_CH8_INTERLOCK_STATUS},

    {LLRFRecords::CH1_INTERLOCK_P,  this->update_CH1_INTERLOCK_P},
    {LLRFRecords::CH2_INTERLOCK_P,  this->update_CH2_INTERLOCK_P},
    {LLRFRecords::CH3_INTERLOCK_P,  this->update_CH3_INTERLOCK_P},
    {LLRFRecords::CH4_INTERLOCK_P,  this->update_CH4_INTERLOCK_P},
    {LLRFRecords::CH5_INTERLOCK_P,  this->update_CH5_INTERLOCK_P},
    {LLRFRecords::CH6_INTERLOCK_P,  this->update_CH6_INTERLOCK_P},
    {LLRFRecords::CH7_INTERLOCK_P,  this->update_CH7_INTERLOCK_P},
    {LLRFRecords::CH8_INTERLOCK_P,  this->update_CH8_INTERLOCK_P},

    {LLRFRecords::CH1_INTERLOCK_U,  this->update_CH1_INTERLOCK_U},
    {LLRFRecords::CH2_INTERLOCK_U,  this->update_CH2_INTERLOCK_U},
    {LLRFRecords::CH3_INTERLOCK_U,  this->update_CH3_INTERLOCK_U},
    {LLRFRecords::CH4_INTERLOCK_U,  this->update_CH4_INTERLOCK_U},
    {LLRFRecords::CH5_INTERLOCK_U,  this->update_CH5_INTERLOCK_U},
    {LLRFRecords::CH6_INTERLOCK_U,  this->update_CH6_INTERLOCK_U},
    {LLRFRecords::CH7_INTERLOCK_U,  this->update_CH7_INTERLOCK_U},
    {LLRFRecords::CH8_INTERLOCK_U,  this->update_CH8_INTERLOCK_U},


    {LLRFRecords::CH1_INTERLOCK_ENABLE,  this->update_CH1_INTERLOCK_ENABLE},
    {LLRFRecords::CH2_INTERLOCK_ENABLE,  this->update_CH2_INTERLOCK_ENABLE},
    {LLRFRecords::CH3_INTERLOCK_ENABLE,  this->update_CH3_INTERLOCK_ENABLE},
    {LLRFRecords::CH4_INTERLOCK_ENABLE,  this->update_CH4_INTERLOCK_ENABLE},
    {LLRFRecords::CH5_INTERLOCK_ENABLE,  this->update_CH5_INTERLOCK_ENABLE},
    {LLRFRecords::CH6_INTERLOCK_ENABLE,  this->update_CH6_INTERLOCK_ENABLE},
    {LLRFRecords::CH7_INTERLOCK_ENABLE,  this->update_CH7_INTERLOCK_ENABLE},
    {LLRFRecords::CH8_INTERLOCK_ENABLE,  this->update_CH8_INTERLOCK_ENABLE},





    {LLRFRecords::CH1_PWR_REM_SCAN,      this->update_CH1_PWR_REM_SCAN      },
    {LLRFRecords::CH1_PHASE_REM_SCAN,    this->update_CH1_PHASE_REM_SCAN    },
    {LLRFRecords::CH1_AMP_DER_SCAN,      this->update_CH1_AMP_DER_SCAN      },
    {LLRFRecords::CH1_PHASE_DER_SCAN,    this->update_CH1_PHASE_DER_SCAN    },
    {LLRFRecords::CH1_PWR_LOC_SCAN,      this->update_CH1_PWR_LOC_SCAN      },
    {LLRFRecords::CH2_PWR_REM_SCAN,      this->update_CH2_PWR_REM_SCAN      },
    {LLRFRecords::CH2_PHASE_REM_SCAN,    this->update_CH2_PHASE_REM_SCAN    },
    {LLRFRecords::CH2_AMP_DER_SCAN,      this->update_CH2_AMP_DER_SCAN      },
    {LLRFRecords::CH2_PHASE_DER_SCAN,    this->update_CH2_PHASE_DER_SCAN    },
    {LLRFRecords::CH2_PWR_LOC_SCAN,      this->update_CH2_PWR_LOC_SCAN      },
    {LLRFRecords::CH3_PWR_REM_SCAN,      this->update_CH3_PWR_REM_SCAN      },
    {LLRFRecords::CH3_PHASE_REM_SCAN,    this->update_CH3_PHASE_REM_SCAN    },
    {LLRFRecords::CH3_AMP_DER_SCAN,      this->update_CH3_AMP_DER_SCAN      },
    {LLRFRecords::CH3_PHASE_DER_SCAN,    this->update_CH3_PHASE_DER_SCAN    },
    {LLRFRecords::CH3_PWR_LOC_SCAN,      this->update_CH3_PWR_LOC_SCAN      },
    {LLRFRecords::CH4_PWR_REM_SCAN,      this->update_CH4_PWR_REM_SCAN      },
    {LLRFRecords::CH4_PHASE_REM_SCAN,    this->update_CH4_PHASE_REM_SCAN    },
    {LLRFRecords::CH4_AMP_DER_SCAN,      this->update_CH4_AMP_DER_SCAN      },
    {LLRFRecords::CH4_PHASE_DER_SCAN,    this->update_CH4_PHASE_DER_SCAN    },
    {LLRFRecords::CH4_PWR_LOC_SCAN,      this->update_CH4_PWR_LOC_SCAN      },
    {LLRFRecords::CH5_PWR_REM_SCAN,      this->update_CH5_PWR_REM_SCAN      },
    {LLRFRecords::CH5_PHASE_REM_SCAN,    this->update_CH5_PHASE_REM_SCAN    },
    {LLRFRecords::CH5_AMP_DER_SCAN,      this->update_CH5_AMP_DER_SCAN      },
    {LLRFRecords::CH5_PHASE_DER_SCAN,    this->update_CH5_PHASE_DER_SCAN    },
    {LLRFRecords::CH5_PWR_LOC_SCAN,      this->update_CH5_PWR_LOC_SCAN      },
    {LLRFRecords::CH6_PWR_REM_SCAN,      this->update_CH6_PWR_REM_SCAN      },
    {LLRFRecords::CH6_PHASE_REM_SCAN,    this->update_CH6_PHASE_REM_SCAN    },
    {LLRFRecords::CH6_AMP_DER_SCAN,      this->update_CH6_AMP_DER_SCAN      },
    {LLRFRecords::CH6_PHASE_DER_SCAN,    this->update_CH6_PHASE_DER_SCAN    },
    {LLRFRecords::CH6_PWR_LOC_SCAN,      this->update_CH6_PWR_LOC_SCAN      },
    {LLRFRecords::CH7_PWR_REM_SCAN,      this->update_CH7_PWR_REM_SCAN      },
    {LLRFRecords::CH7_PHASE_REM_SCAN,    this->update_CH7_PHASE_REM_SCAN    },
    {LLRFRecords::CH7_AMP_DER_SCAN,      this->update_CH7_AMP_DER_SCAN      },
    {LLRFRecords::CH7_PHASE_DER_SCAN,    this->update_CH7_PHASE_DER_SCAN    },
    {LLRFRecords::CH7_PWR_LOC_SCAN,      this->update_CH7_PWR_LOC_SCAN      },
    {LLRFRecords::CH8_PWR_REM_SCAN,      this->update_CH8_PWR_REM_SCAN      },
    {LLRFRecords::CH8_PHASE_REM_SCAN,    this->update_CH8_PHASE_REM_SCAN    },
    {LLRFRecords::CH8_AMP_DER_SCAN,      this->update_CH8_AMP_DER_SCAN      },
    {LLRFRecords::CH8_PHASE_DER_SCAN,    this->update_CH8_PHASE_DER_SCAN    },
    {LLRFRecords::CH8_PWR_LOC_SCAN,      this->update_CH8_PWR_LOC_SCAN      },




    {LLRFRecords::CH1_PWR_REM_ACQM,      this->update_CH1_PWR_REM_ACQM      },
    {LLRFRecords::CH1_PHASE_REM_ACQM,    this->update_CH1_PHASE_REM_ACQM    },
    {LLRFRecords::CH1_AMP_DER_ACQM,      this->update_CH1_AMP_DER_ACQM      },
    {LLRFRecords::CH1_PHASE_DER_ACQM,    this->update_CH1_PHASE_DER_ACQM    },
    {LLRFRecords::CH1_PWR_LOC_ACQM,      this->update_CH1_PWR_LOC_ACQM      },
    {LLRFRecords::CH2_PWR_REM_ACQM,      this->update_CH2_PWR_REM_ACQM      },
    {LLRFRecords::CH2_PHASE_REM_ACQM,    this->update_CH2_PHASE_REM_ACQM    },
    {LLRFRecords::CH2_AMP_DER_ACQM,      this->update_CH2_AMP_DER_ACQM      },
    {LLRFRecords::CH2_PHASE_DER_ACQM,    this->update_CH2_PHASE_DER_ACQM    },
    {LLRFRecords::CH2_PWR_LOC_ACQM,      this->update_CH2_PWR_LOC_ACQM      },
    {LLRFRecords::CH3_PWR_REM_ACQM,      this->update_CH3_PWR_REM_ACQM      },
    {LLRFRecords::CH3_PHASE_REM_ACQM,    this->update_CH3_PHASE_REM_ACQM    },
    {LLRFRecords::CH3_AMP_DER_ACQM,      this->update_CH3_AMP_DER_ACQM      },
    {LLRFRecords::CH3_PHASE_DER_ACQM,    this->update_CH3_PHASE_DER_ACQM    },
    {LLRFRecords::CH3_PWR_LOC_ACQM,      this->update_CH3_PWR_LOC_ACQM      },
    {LLRFRecords::CH4_PWR_REM_ACQM,      this->update_CH4_PWR_REM_ACQM      },
    {LLRFRecords::CH4_PHASE_REM_ACQM,    this->update_CH4_PHASE_REM_ACQM    },
    {LLRFRecords::CH4_AMP_DER_ACQM,      this->update_CH4_AMP_DER_ACQM      },
    {LLRFRecords::CH4_PHASE_DER_ACQM,    this->update_CH4_PHASE_DER_ACQM    },
    {LLRFRecords::CH4_PWR_LOC_ACQM,      this->update_CH4_PWR_LOC_ACQM      },
    {LLRFRecords::CH5_PWR_REM_ACQM,      this->update_CH5_PWR_REM_ACQM      },
    {LLRFRecords::CH5_PHASE_REM_ACQM,    this->update_CH5_PHASE_REM_ACQM    },
    {LLRFRecords::CH5_AMP_DER_ACQM,      this->update_CH5_AMP_DER_ACQM      },
    {LLRFRecords::CH5_PHASE_DER_ACQM,    this->update_CH5_PHASE_DER_ACQM    },
    {LLRFRecords::CH5_PWR_LOC_ACQM,      this->update_CH5_PWR_LOC_ACQM      },
    {LLRFRecords::CH6_PWR_REM_ACQM,      this->update_CH6_PWR_REM_ACQM      },
    {LLRFRecords::CH6_PHASE_REM_ACQM,    this->update_CH6_PHASE_REM_ACQM    },
    {LLRFRecords::CH6_AMP_DER_ACQM,      this->update_CH6_AMP_DER_ACQM      },
    {LLRFRecords::CH6_PHASE_DER_ACQM,    this->update_CH6_PHASE_DER_ACQM    },
    {LLRFRecords::CH6_PWR_LOC_ACQM,      this->update_CH6_PWR_LOC_ACQM      },
    {LLRFRecords::CH7_PWR_REM_ACQM,      this->update_CH7_PWR_REM_ACQM      },
    {LLRFRecords::CH7_PHASE_REM_ACQM,    this->update_CH7_PHASE_REM_ACQM    },
    {LLRFRecords::CH7_AMP_DER_ACQM,      this->update_CH7_AMP_DER_ACQM      },
    {LLRFRecords::CH7_PHASE_DER_ACQM,    this->update_CH7_PHASE_DER_ACQM    },
    {LLRFRecords::CH7_PWR_LOC_ACQM,      this->update_CH7_PWR_LOC_ACQM      },
    {LLRFRecords::CH8_PWR_REM_ACQM,      this->update_CH8_PWR_REM_ACQM      },
    {LLRFRecords::CH8_PHASE_REM_ACQM,    this->update_CH8_PHASE_REM_ACQM    },
    {LLRFRecords::CH8_AMP_DER_ACQM,      this->update_CH8_AMP_DER_ACQM      },
    {LLRFRecords::CH8_PHASE_DER_ACQM,    this->update_CH8_PHASE_DER_ACQM    },
    {LLRFRecords::CH8_PWR_LOC_ACQM,      this->update_CH8_PWR_LOC_ACQM      }


























    //{LLRFRecords::PULSE_SHAPE_APPLY,this->update_PULSE_SHAPE_APPLY      },
   // {LLRFRecords::CH1_INTERLOCK_STATUS, this->update_CH1_INTERLOCK_STATUS},
   // {LLRFRecords::CH1_INTERLOCK_ENABLE,this->update_CH1_INTERLOCK_ENABLE},
   // {LLRFRecords::CH1_INTERLOCK_U, this->update_CH1_INTERLOCK_U         },
   // {LLRFRecords::CH1_INTERLOCK_P,this->update_CH1_INTERLOCK_P      },
   // {LLRFRecords::CH1_INTERLOCK_PDBM,this->update_CH1_INTERLOCK_PDBM},
   // {LLRFRecords::CH2_INTERLOCK_STATUS,this->update_CH2_INTERLOCK_STATUS },
   // {LLRFRecords::CH2_INTERLOCK_ENABLE, this->update_CH2_INTERLOCK_ENABLE },
   // {LLRFRecords::CH2_INTERLOCK_U, this->update_CH2_INTERLOCK_U     },
   // {LLRFRecords::CH2_INTERLOCK_P,this->update_CH2_INTERLOCK_P      },
   // {LLRFRecords::CH2_INTERLOCK_PDBM,this->update_CH2_INTERLOCK_PDBM},
   // {LLRFRecords::CH3_INTERLOCK_STATUS,this->update_CH3_INTERLOCK_STATUS },
   // {LLRFRecords::CH3_INTERLOCK_ENABLE,this->update_CH3_INTERLOCK_ENABLE },
   // {LLRFRecords::CH3_INTERLOCK_U,this->update_CH3_INTERLOCK_U      },
   // {LLRFRecords::CH3_INTERLOCK_P, this->update_CH3_INTERLOCK_P     },
   // {LLRFRecords::CH3_INTERLOCK_PDBM, this->update_CH3_INTERLOCK_PDBM },
   // {LLRFRecords::CH4_INTERLOCK_STATUS,this->update_CH4_INTERLOCK_STATUS },
   // {LLRFRecords::CH4_INTERLOCK_ENABLE,this->update_CH4_INTERLOCK_ENABLE },
   // {LLRFRecords::CH4_INTERLOCK_U, this->update_CH4_INTERLOCK_U     },
   // {LLRFRecords::CH4_INTERLOCK_P,this->update_CH4_INTERLOCK_P      },
   // {LLRFRecords::CH4_INTERLOCK_PDBM, this->update_CH4_INTERLOCK_PDBM },
   // {LLRFRecords::CH5_INTERLOCK_STATUS,this->update_CH5_INTERLOCK_STATUS },
   // {LLRFRecords::CH5_INTERLOCK_ENABLE, this->update_CH5_INTERLOCK_ENABLE},
   // {LLRFRecords::CH5_INTERLOCK_U, this->update_CH5_INTERLOCK_U     },
   // {LLRFRecords::CH5_INTERLOCK_P, this->update_CH5_INTERLOCK_P     },
   // {LLRFRecords::CH5_INTERLOCK_PDBM, this->update_CH5_INTERLOCK_PDBM},
   // {LLRFRecords::CH6_INTERLOCK_STATUS, this->update_CH6_INTERLOCK_STATUS},
   // {LLRFRecords::CH6_INTERLOCK_ENABLE, this->update_CH6_INTERLOCK_ENABLE},
   // {LLRFRecords::CH6_INTERLOCK_U, this->update_CH6_INTERLOCK_U     },
   // {LLRFRecords::CH6_INTERLOCK_P, this->update_CH6_INTERLOCK_P     },
   // {LLRFRecords::CH6_INTERLOCK_PDBM, this->update_CH6_INTERLOCK_PDBM },
   // {LLRFRecords::CH7_INTERLOCK_STATUS, this->update_CH7_INTERLOCK_STATUS},
   // {LLRFRecords::CH7_INTERLOCK_ENABLE, this->update_CH7_INTERLOCK_ENABLE},
   // {LLRFRecords::CH7_INTERLOCK_U, this->update_CH7_INTERLOCK_U     },
   // {LLRFRecords::CH7_INTERLOCK_P,            this->update_CH7_INTERLOCK_P},
   // {LLRFRecords::CH7_INTERLOCK_PDBM,            this->update_CH7_INTERLOCK_PDBM},
   // {LLRFRecords::CH8_INTERLOCK_STATUS,            this->update_CH8_INTERLOCK_STATUS},
   // {LLRFRecords::CH8_INTERLOCK_ENABLE,            this->update_CH8_INTERLOCK_ENABLE},
   // {LLRFRecords::CH8_INTERLOCK_U,            this->update_CH8_INTERLOCK_U},
   // {LLRFRecords::CH8_INTERLOCK_P,            this->update_CH8_INTERLOCK_P},
   // {LLRFRecords::CH8_INTERLOCK_PDBM,            this->update_CH8_INTERLOCK_PDBM},
   // {LLRFRecords::LLRF_TRACES,            this->update_LLRF_TRACES},
   // {LLRFRecords::LLRF_TRACES_SCAN,            this->update_LLRF_TRACES_SCAN},
   // {LLRFRecords::LLRF_TRACES_ACQM,            this->update_LLRF_TRACES_ACQM},
   // {LLRFRecords::CH1_PWR_REM_SCAN,            this->update_CH1_PWR_REM_SCAN},
   // {LLRFRecords::CH1_PWR_REM,            this->update_CH1_PWR_REM},
   // {LLRFRecords::CH1_PWR_REM_ACQM,            this->update_CH1_PWR_REM_ACQM},
   // {LLRFRecords::CH1_PHASE_REM_SCAN,            this->update_CH1_PHASE_REM_SCAN},
   // {LLRFRecords::CH1_PHASE_REM_ACQM,            this->update_CH1_PHASE_REM_ACQM},
   // {LLRFRecords::CH1_PHASE_REM,            this->update_CH1_PHASE_REM},
   // {LLRFRecords::CH1_AMP_DER_SCAN,            this->update_CH1_AMP_DER_SCAN},
   // {LLRFRecords::CH1_PHASE_DER_SCAN,            this->update_CH1_PHASE_DER_SCAN},
   // {LLRFRecords::CH1_PWR_LOC_SCAN,            this->update_CH1_PWR_LOC_SCAN},
   // {LLRFRecords::CH2_PWR_REM_SCAN,            this->update_CH2_PWR_REM_SCAN},
   // {LLRFRecords::CH2_PWR_REM,            this->update_CH2_PWR_REM},
   // {LLRFRecords::CH2_PWR_REM_ACQM,            this->update_CH2_PWR_REM_ACQM},
   // {LLRFRecords::CH2_PHASE_REM_SCAN,           this->update_CH2_PHASE_REM_SCAN},
   // {LLRFRecords::CH2_PHASE_REM_ACQM,            this->update_CH2_PHASE_REM_ACQM},
   // {LLRFRecords::CH2_PHASE_REM,            this->update_CH2_PHASE_REM},
   // {LLRFRecords::CH2_AMP_DER_SCAN,            this->update_CH2_AMP_DER_SCAN},
   // {LLRFRecords::CH2_PHASE_DER_SCAN,           this->update_CH2_PHASE_DER_SCAN},
   // {LLRFRecords::CH2_PWR_LOC_SCAN,            this->update_CH2_PWR_LOC_SCAN},
   // {LLRFRecords::CH3_PWR_REM_SCAN,            this->update_CH3_PWR_REM_SCAN},
   // {LLRFRecords::CH3_PWR_REM,            this->update_CH3_PWR_REM},
   // {LLRFRecords::CH3_PWR_REM_ACQM,            this->update_CH3_PWR_REM_ACQM},
   // {LLRFRecords::CH3_PHASE_REM_SCAN,            this->update_CH3_PHASE_REM_SCAN},
   // {LLRFRecords::CH3_PHASE_REM_ACQM,            this->update_CH3_PHASE_REM_ACQM},
   // {LLRFRecords::CH3_PHASE_REM,            this->update_CH3_PHASE_REM},
   // {LLRFRecords::CH3_AMP_DER_SCAN,            this->update_CH3_AMP_DER_SCAN},
   // {LLRFRecords::CH3_PHASE_DER_SCAN,            this->update_CH3_PHASE_DER_SCAN},
   // {LLRFRecords::CH3_PWR_LOC_SCAN,            this->update_CH3_PWR_LOC_SCAN},
   // {LLRFRecords::CH4_PWR_REM_SCAN,            this->update_CH4_PWR_REM_SCAN},
   // {LLRFRecords::CH4_PWR_REM,            this->update_CH4_PWR_REM},
   // {LLRFRecords::CH4_PWR_REM_ACQM,            this->update_CH4_PWR_REM_ACQM},
   // {LLRFRecords::CH4_PHASE_REM_SCAN,            this->update_CH4_PHASE_REM_SCAN},
   // {LLRFRecords::CH4_PHASE_REM_ACQM,            this->update_CH4_PHASE_REM_ACQM},
   // {LLRFRecords::CH4_PHASE_REM,            this->update_CH4_PHASE_REM},
   // {LLRFRecords::CH4_AMP_DER_SCAN,            this->update_CH4_AMP_DER_SCAN},
   // {LLRFRecords::CH4_PHASE_DER_SCAN,            this->update_CH4_PHASE_DER_SCAN},
   // {LLRFRecords::CH4_PWR_LOC_SCAN,            this->update_CH4_PWR_LOC_SCAN},
   // {LLRFRecords::CH5_PWR_REM_SCAN,            this->update_CH5_PWR_REM_SCAN},
   // {LLRFRecords::CH5_PWR_REM,                this->update_CH5_PWR_REM},
   // {LLRFRecords::CH5_PWR_REM_ACQM,            this->update_CH5_PWR_REM_ACQM},
   // {LLRFRecords::CH5_PHASE_REM_SCAN,            this->update_CH5_PHASE_REM_SCAN},
   // {LLRFRecords::CH5_PHASE_REM_ACQM,            this->update_CH5_PHASE_REM_ACQM},
   // {LLRFRecords::CH5_PHASE_REM,            this->update_CH5_PHASE_REM},
   // {LLRFRecords::CH5_AMP_DER_SCAN,            this->update_CH5_AMP_DER_SCAN},
   // {LLRFRecords::CH5_PHASE_DER_SCAN,            this->update_CH5_PHASE_DER_SCAN},
   // {LLRFRecords::CH5_PWR_LOC_SCAN,            this->update_CH5_PWR_LOC_SCAN},
   // {LLRFRecords::CH6_PWR_REM_SCAN,            this->update_CH6_PWR_REM_SCAN},
   // {LLRFRecords::CH6_PWR_REM,            this->update_CH6_PWR_REM},
   // {LLRFRecords::CH6_PWR_REM_ACQM,            this->update_CH6_PWR_REM_ACQM},
   // {LLRFRecords::CH6_PHASE_REM_SCAN,            this->update_CH6_PHASE_REM_SCAN},
   // {LLRFRecords::CH6_PHASE_REM_ACQM,            this->update_CH6_PHASE_REM_ACQM},
   // {LLRFRecords::CH6_PHASE_REM,            this->update_CH6_PHASE_REM},
   // {LLRFRecords::CH6_AMP_DER_SCAN,            this->update_CH6_AMP_DER_SCAN},
   // {LLRFRecords::CH6_PHASE_DER_SCAN,            this->update_CH6_PHASE_DER_SCAN},
   // {LLRFRecords::CH6_PWR_LOC_SCAN,            this->update_CH6_PWR_LOC_SCAN},
   // {LLRFRecords::CH7_PWR_REM_SCAN,            this->update_CH7_PWR_REM_SCAN},
   // {LLRFRecords::CH7_PWR_REM,            this->update_CH7_PWR_REM},
   // {LLRFRecords::CH7_PWR_REM_ACQM,            this->update_CH7_PWR_REM_ACQM},
   // {LLRFRecords::CH7_PHASE_REM_SCAN,            this->update_CH7_PHASE_REM_SCAN},
   // {LLRFRecords::CH7_PHASE_REM_ACQM,            this->update_CH7_PHASE_REM_ACQM},
   // {LLRFRecords::CH7_PHASE_REM,            this->update_CH7_PHASE_REM},
   // {LLRFRecords::CH7_AMP_DER_SCAN,            this->update_CH7_AMP_DER_SCAN},
   // {LLRFRecords::CH7_PHASE_DER_SCAN,            this->update_CH7_PHASE_DER_SCAN},
   // {LLRFRecords::CH7_PWR_LOC_SCAN,            this->update_CH7_PWR_LOC_SCAN},
   // {LLRFRecords::CH8_PWR_REM_SCAN,            this->update_CH8_PWR_REM_SCAN},
   // {LLRFRecords::CH8_PWR_REM,            this->update_CH8_PWR_REM},
   // {LLRFRecords::CH8_PWR_REM_ACQM,            this->update_CH8_PWR_REM_ACQM},
   // {LLRFRecords::CH8_PHASE_REM_SCAN,            this->update_CH8_PHASE_REM_SCAN},
   // {LLRFRecords::CH8_PHASE_REM_ACQM,            this->update_CH8_PHASE_REM_ACQM},
   // {LLRFRecords::CH8_PHASE_REM,            this->update_CH8_PHASE_REM},
   // {LLRFRecords::CH8_AMP_DER_SCAN,            this->update_CH8_AMP_DER_SCAN},
   // {LLRFRecords::CH8_PHASE_DER_SCAN,this->update_CH8_PHASE_DER_SCAN },
   // {LLRFRecords::CH8_PWR_LOC_SCAN, this->update_CH8_PWR_LOC_SCAN}
        };




    // 


};



