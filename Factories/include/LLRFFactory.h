#ifndef LLRF_FACTORY_H_
#define LLRF_FACTORY_H_
#include <ConfigReader.h>
#include <LLRF.h>
#include <GlobalStateEnums.h>
#include <vector>

class LLRFFactory
{
public:
	LLRFFactory();
	LLRFFactory(STATE mode);
	LLRFFactory(const LLRFFactory& copyFactory);
	~LLRFFactory();
	LoggingSystem messenger;


	/*! default setup function, uses default values to read files and connect to EPICS etc.
	@param[out] bool, for success or failure	*/
	bool setup();
	/*! setup function using version parameter
	@param[in] version (a placeholder for future extensions)
	@param[out] bool, for success or failure	*/
	bool setup(const std::string& version);
	/*! setup function using machineArea parameter
	@param[in] machineArea, only setup magnets that match machineArea
	@param[out] bool, for success or failure	*/
	bool setup(TYPE machineArea);
	/*! setup function using version and machineArea parameters
	@param[in] version (a placeholder for future extensions)
	@param[in] machineArea, only setup magnets that match machineArea
	@param[out] bool, for success or failure	*/
	bool setup(const std::string& version, TYPE machineArea);
	/*! setup function using std::vector of machineAreas
	@param[in] machineAreas, only setup magnets that match an area in machineAreas
	@param[out] bool, for success or failure	*/
	bool setup(const std::vector<TYPE>& machineAreas);
	/*! setup function using python::list of machineAreas
	@param[in] machineAreas, only setup magnets that match an area in machineAreas
	@param[out] bool, for success or failure	*/
	bool setup(const boost::python::list& machineAreas);
	/*! setup function using python::list of machineAreas
	@param[in] version, (a placeholder for future extensions)
	@param[in] machineAreas, only setup magnets that match an area in machineAreas
	@param[out] bool, for success or failure	*/
	bool setup(const std::string& version, const boost::python::list& machineAreas);
	/*! setup function using std::vector of machineAreas
	@param[in] version, (a placeholder for future extensions)
	@param[in] machineAreas, only setup magnets that match an area in machineAreas
	@param[out] bool, for success or failure	*/
	bool setup(const std::string& version, const std::vector<TYPE>& machineAreas);


	std::vector<std::string> getLLRFNames();
	boost::python::list getLLRFNames_Py();

	// get a LLRF object (GUN, L01, etc... ) 
	LLRF& getLLRF(const std::string& llrf_name);


	bool setPhi(const std::string& llrf_name, double value);
	bool setAmp(const std::string& llrf_name, double value);
	bool setAmpMW(const std::string& llrf_name, double value);
	bool setPhiDEG(const std::string& llrf_name, double value);
	double getPhi(const std::string& llrf_name)const;
	double getAmp(const std::string& llrf_name)const;
	double getAmpMW(const std::string& llrf_name)const;
	double getPhiDEG(const std::string& llrf_name)const;

	//// TODOD this should be in the LLRF object and this version one needs a llrf obejct name passed 
	///*! get the name alises for this LLRF
	//@param[out] names, vector containing  all the alias names */
	//std::vector<std::string> getAliases() const;
	///*! get the name alises for this LLRF (python version)
	//	@param[out] names, python list containing all the alias names */
	//boost::python::list getAliases_Py() const;


	std::map<std::string, std::vector<double>> getAllTraceData(const std::string& llrf_name)const;
	std::pair<std::string, std::vector<double>> getTraceData(const std::string& llrf_name, const std::string& trace_name)const;
	std::vector<double> getTraceValues(const std::string& name)const;
	std::vector<double> getCavRevPwr(const std::string& llrf_name)const;
	std::vector<double> getCavFwdPwr(const std::string& llrf_name)const;
	std::vector<double> getKlyRevPwr(const std::string& llrf_name)const;
	std::vector<double> getKlyFwdPwr(const std::string& llrf_name)const;
	std::vector<double> getCavRevPha(const std::string& llrf_name)const;
	std::vector<double> getCavFwdPha(const std::string& llrf_name)const;
	std::vector<double> getKlyRevPha(const std::string& llrf_name)const;
	std::vector<double> getKlyFwdPha(const std::string& llrf_name)const;
	std::vector<double> getProbePwr(const std::string& llrf_name)const;
	std::vector<double> getProbePha(const std::string& llrf_name)const;
	boost::python::dict getAllTraceData_Py(const std::string& name);
	boost::python::dict getTraceData_Py(const std::string& llrf_name, const std::string& trace_name);
	boost::python::list getTraceValues_Py(const std::string& name)const;
	boost::python::list getCavRevPwr_Py(const std::string& llrf_name)const;
	boost::python::list getCavFwdPwr_Py(const std::string& llrf_name)const;
	boost::python::list getKlyRevPwr_Py(const std::string& llrf_name)const;
	boost::python::list getKlyFwdPwr_Py(const std::string& llrf_name)const;
	boost::python::list getCavRevPha_Py(const std::string& llrf_name)const;
	boost::python::list getCavFwdPha_Py(const std::string& llrf_name)const;
	boost::python::list getKlyRevPha_Py(const std::string& llrf_name)const;
	boost::python::list getKlyFwdPha_Py(const std::string& llrf_name)const;
	boost::python::list getProbePha_Py(const std::string& llrf_name)const;
	boost::python::list getProbePwr_Py(const std::string& llrf_name)const;



	size_t getIndex(const std::string& name, const double time) const;
	double getTime (const std::string& name, const size_t index) const;
	//--------------------------------------------------------------------------------------------------
	/*  ___  __        __   ___           ___            __
		 |  |__)  /\  /  ` |__      |\/| |__   /\  |\ | /__`
		 |  |  \ /~~\ \__, |___     |  | |___ /~~\ | \| .__/
	*/
	bool setMeanStartEndTime(const std::string& llrf_name, const std::string& trace_name, const double start, const double end);
	bool setMeanStartIndex  (const std::string& llrf_name, const std::string& trace_name, size_t  value);
	bool setMeanStopIndex   (const std::string& llrf_name, const std::string& trace_name, size_t  value);
	size_t getMeanStartIndex(const std::string& llrf_name, const std::string& trace_name)const;
	size_t getMeanStopIndex (const std::string& llrf_name, const std::string& trace_name)const;
	double getMean          (const std::string& llrf_name, const std::string& trace_name)const;
	double getCutMean       (const std::string& llrf_name, const std::string& trace_name)const;
	double getMeanStartTime (const std::string& llrf_name, const std::string& trace_name)const;
	double getMeanStopTime  (const std::string& llrf_name, const std::string& trace_name)const;


	bool setTraceDataBufferSize(const std::string& llrf_name, const std::string& trace_name, const size_t new_size);



	std::string getFullName(const std::string& name_to_check) const;


	void debugMessagesOn();
	void debugMessagesOff();
	void messagesOn();
	void messagesOff();
	bool isDebugOn();
	bool isMessagingOn();


	/*! Flag to say whether the LLRFFactory setup function has been completed successfully.*/
	bool hasBeenSetup; // PUBLIC as it is used by hardwarefactory, could make it a friend
private:

	void populateLLRFMap();

	/*! The kind of LLRFFactory that is created (PHYSICAL, VIRTUAL, OFFLINE), this variable is passed to the
	LLRF hardware objects when they are created by ConfigReader*/
	STATE mode;
	/*! ConfigReader to parse YAML config files and create associated LLRF objects*/
	ConfigReader reader;


	/*! the machine areas (i.e. exactly which cavities) to create LLRF objects for */
	std::vector<TYPE> machineAreas;
	/*! cut the LLRFMap to only those in machineAreas  */
	void cutLLRFMapByMachineAreas();
	/*! after the LLRFMap is cut the full setup of each LLRF object can be started */
	void setupLLRFAfterMachineAreaSet();

	void updateAliasNameMap(const LLRF& llrf);

	void setupChannels();

	std::map<std::string, std::string> alias_name_map;

	std::map<std::string, LLRF> LLRFMap;


	LLRF dummy_llrf;

	std::vector<double> dummy_trace_data;

	void retrieveMonitorStatus(pvStruct& pvStruct);

	// keep the PVs to monitor in a vector, for LLRF *everything* is monitored ( i think) 
	// TODOD moe strings into better namespace 
	const std::vector<std::string> pv_to_monitor {
"HEART_BEAT",
"AMP_FF",
"TRIG_SOURCE",
"AMP_SP",
"PHI_FF",
"PHI_SP",
"RF_OUTPUT",
"FF_PHASE_LOCK_STATE",
"FF_AMP_LOCK_STATE",
"TIME_VECTOR",
"PULSE_OFFSET"
"LLRF_PULSE_DURATION",
"INTERLOCK",
"PULSE_SHAPE",
"PULSE_SHAPE_APPLY",
//"CH1_INTERLOCK_STATUS",
//"CH1_INTERLOCK_ENABLE",
//"CH1_INTERLOCK_U",
//"CH1_INTERLOCK_P",
//"CH1_INTERLOCK_PDBM",
//"CH2_INTERLOCK_STATUS",
//"CH2_INTERLOCK_ENABLE",
//"CH2_INTERLOCK_U",
//"CH2_INTERLOCK_P",
//"CH2_INTERLOCK_PDBM",
//"CH3_INTERLOCK_STATUS",
//"CH3_INTERLOCK_ENABLE",
//"CH3_INTERLOCK_U",
//"CH3_INTERLOCK_P",
//"CH3_INTERLOCK_PDBM",
//"CH4_INTERLOCK_STATUS",
//"CH4_INTERLOCK_ENABLE",
//"CH4_INTERLOCK_U",
//"CH4_INTERLOCK_P",
//"CH4_INTERLOCK_PDBM",
//"CH5_INTERLOCK_STATUS",
//"CH5_INTERLOCK_ENABLE",
//"CH5_INTERLOCK_U",
//"CH5_INTERLOCK_P",
//"CH5_INTERLOCK_PDBM",
//"CH6_INTERLOCK_STATUS",
//"CH6_INTERLOCK_ENABLE",
//"CH6_INTERLOCK_U",
//"CH6_INTERLOCK_P",
//"CH6_INTERLOCK_PDBM",
//"CH7_INTERLOCK_STATUS",
//"CH7_INTERLOCK_ENABLE",
//"CH7_INTERLOCK_U",
//"CH7_INTERLOCK_P",
//"CH7_INTERLOCK_PDBM",
//"CH8_INTERLOCK_STATUS",
//"CH8_INTERLOCK_ENABLE",
//"CH8_INTERLOCK_U",
//"CH8_INTERLOCK_P",
//"CH8_INTERLOCK_PDBM",
//"LLRF_TRACES",
//"LLRF_TRACES_SCAN",
//"LLRF_TRACES_ACQM",
//"CH1_PWR_REM_SCAN",
////"CH1_PWR_REM",
//"CH1_PWR_REM_ACQM",
//"CH1_PHASE_REM_SCAN",
//"CH1_PHASE_REM_ACQM",
////"CH1_PHASE_REM",
//"CH1_AMP_DER_SCAN",
//"CH1_PHASE_DER_SCAN",
//"CH1_PWR_LOC_SCAN",
//"CH2_PWR_REM_SCAN",
////"CH2_PWR_REM",
//"CH2_PWR_REM_ACQM",
//"CH2_PHASE_REM_SCAN",
//"CH2_PHASE_REM_ACQM",
////"CH2_PHASE_REM",
//"CH2_AMP_DER_SCAN",
//"CH2_PHASE_DER_SCAN",
//"CH2_PWR_LOC_SCAN",
//"CH3_PWR_REM_SCAN",
////"CH3_PWR_REM",
//"CH3_PWR_REM_ACQM",
//"CH3_PHASE_REM_SCAN",
//"CH3_PHASE_REM_ACQM",
////"CH3_PHASE_REM",
//"CH3_AMP_DER_SCAN",
//"CH3_PHASE_DER_SCAN",
//"CH3_PWR_LOC_SCAN",
//"CH4_PWR_REM_SCAN",
////"CH4_PWR_REM",
//"CH4_PWR_REM_ACQM",
//"CH4_PHASE_REM_SCAN",
//"CH4_PHASE_REM_ACQM",
////"CH4_PHASE_REM",
//"CH4_AMP_DER_SCAN",
//"CH4_PHASE_DER_SCAN",
//"CH4_PWR_LOC_SCAN",
//"CH5_PWR_REM_SCAN",
////"CH5_PWR_REM",
//"CH5_PWR_REM_ACQM",
//"CH5_PHASE_REM_SCAN",
//"CH5_PHASE_REM_ACQM",
////"CH5_PHASE_REM",
//"CH5_AMP_DER_SCAN",
//"CH5_PHASE_DER_SCAN",
//"CH5_PWR_LOC_SCAN",
//"CH6_PWR_REM_SCAN",
////"CH6_PWR_REM",
//"CH6_PWR_REM_ACQM",
//"CH6_PHASE_REM_SCAN",
//"CH6_PHASE_REM_ACQM",
//"CH6_PHASE_REM",
//"CH6_AMP_DER_SCAN",
//"CH6_PHASE_DER_SCAN",
//"CH6_PWR_LOC_SCAN",
//"CH7_PWR_REM_SCAN",
////"CH7_PWR_REM",
//"CH7_PWR_REM_ACQM",
//"CH7_PHASE_REM_SCAN",
//"CH7_PHASE_REM_ACQM",
////"CH7_PHASE_REM",
//"CH7_AMP_DER_SCAN",
//"CH7_PHASE_DER_SCAN",
//"CH7_PWR_LOC_SCAN",
//"CH8_PWR_REM_SCAN",
////"CH8_PWR_REM",
//"CH8_PWR_REM_ACQM",
//"CH8_PHASE_REM_SCAN",
//"CH8_PHASE_REM_ACQM",
////"CH8_PHASE_REM",
//"CH8_AMP_DER_SCAN",
//"CH8_PHASE_DER_SCAN",
//"CH8_PWR_LOC_SCAN"	
};

};

#endif // RF_MODULATOR_FACTORY_H_