#ifndef EPICS_RF_HEARTBEAT_INTERFACE_H_
#define EPICS_RF_HEARTBEAT_INTERFACE_H_
#include <EPICSInterface.h>
#include <RFHeartbeatPVRecords.h>
#include "EPICSInterface.h"
#include "RFHeartbeat.h"


typedef void(*updateFunctionPtr)(struct event_handler_args args);

class EPICSRFHeartbeatInterface : public EPICSInterface
{
public:
	EPICSRFHeartbeatInterface();
	~EPICSRFHeartbeatInterface();
	std::string ownerName;

	static LoggingSystem messenger;

	void retrieveUpdateFunctionForRecord(const pvStruct& pv)const;

	static void update_KEEP_ALIVE(const struct event_handler_args args);
	

	std::map<std::string, updateFunctionPtr> updateFunctionMap = 
	{
		// TODO uncommenting the below line gives a weird linker error 
		// ??????? 

		/*
		
		Severity	Code	Description	Project	File	Line	Suppression State
		Error	LNK2019	unresolved external symbol 
		"class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> > const RFHeartbeatRecords::KEEP_ALIVE" 
		(?KEEP_ALIVE@RFHeartbeatRecords@@3V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@B) 
		referenced in function "public: __cdecl 
		EPICSRFHeartbeatInterface::EPICSRFHeartbeatInterface(void)" 
		(??0EPICSRFHeartbeatInterface@@QEAA@XZ)	
		_RFHeartbeat	
		C:\Users\dlerlp\Documents\CATAP_Build\PythonInterface\EPICSInterface.lib(EPICSRFHeartbeatInterface.obj)	1


		*/

		//{RFHeartbeatRecords::KEEP_ALIVE, this->update_KEEP_ALIVE}
	};


};

#endif //EPICS_RF_HEARTBEAT_INTERFACE_H_