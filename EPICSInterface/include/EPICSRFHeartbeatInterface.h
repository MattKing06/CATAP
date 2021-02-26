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
	/*! Default Constuctor  */
	EPICSRFHeartbeatInterface();
	/*! Destructor */
	~EPICSRFHeartbeatInterface();
	// TODO copy contructor needed? 
	
	/*! hardwar ethat owns this interface */
	std::string ownerName;

	/* messenger, shared by all interfacess in this particualr factory  */
	static LoggingSystem messenger;

	// TODO move to base calss? 
	/*! sets the updat efunciton pointer */
	void retrieveUpdateFunctionForRecord(const pvStruct& pv)const;

	/*! update the keep alive vlue  */
	static void update_KEEP_ALIVE(const struct event_handler_args args);
	/*! map of record to update function pointer. Used to link keywords in master lattice to epics update functions*/
	std::map<std::string, updateFunctionPtr> updateFunctionMap = 
	{ {RFHeartbeatRecords::KEEP_ALIVE, this->update_KEEP_ALIVE }
	};


};

#endif //EPICS_RF_HEARTBEAT_INTERFACE_H_