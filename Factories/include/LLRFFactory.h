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


	std::vector<TYPE> machineAreas;
	void cutLLRFMapByMachineAreas();

	void updateAliasNameMap(const LLRF& llrf);
	std::map<std::string, std::string> alias_name_map;

	std::map<std::string, LLRF> LLRFMap;


	LLRF dummy_llrf;

	std::vector<double> dummy_trace_data;

};

#endif // RF_MODULATOR_FACTORY_H_