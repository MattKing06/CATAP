from CATAP.EPICSTools import *
import time
import random

# Creating an EPICSTools object without the STATE argument
# will force CATAP to use the Virtual EPICS instance.
# i.e. ET = EPICSTools()
#
# Adding a STATE argument will specify which EPICS instance to use.
ET = EPICSTools(STATE.VIRTUAL)

# Monitoring a PV is done via EPICSTools by providing the PV:
array_pv = "CLA-GUN-LRF-CTRL-01:app:time_vector"
ET.monitor(array_pv)

# You can access the PV, COUNT, and TYPE information for a PV using getEPICSInfo:
epics_info = ET.getEPICSInfo(array_pv)
print(epics_info)

# To access the monitor object, it can be retrieved using EPICSTools:
time_vector_monitor = ET.getMonitor(array_pv)

print(array_pv, " connected? : ", time_vector_monitor.isConnected())

# Example for Putting to array, getting array value from the monitor,
# and getting the buffer of arrays from the monitor object:
for i in range(0,5):
    array_to_put = [random.random()*10]*epics_info['COUNT']
    ET.putArray(array_pv, array_to_put)
    time.sleep(1.0)
print(time_vector_monitor.getArray())
print(time_vector_monitor.getArrayBuffer())

# Array PVs can be expensive to monitor over the network
# so it is advised to stop monitoring the PV when it is 
# not needed:
ET.stopMonitoring(array_pv)

# Now when putting to the PV, the monitor will keep the old value:
array_to_put = [random.random()*10]*epics_info['COUNT']
ET.putArray(array_pv, array_to_put)

if time_vector_monitor.getValue() != ET.getArray(array_pv):
    print('Stoppped Monitoring')

# To restart the monitoring of a PV, the following can be done:
ET.restartMonitoring(array_pv)

# Now when putting to the PV, the monitor will be updated:
array_to_put = [random.random()*10]*epics_info['COUNT']
ET.putArray(array_pv, array_to_put)
time.sleep(1.0)
if time_vector_monitor.getValue() == ET.getArray(array_pv):
    print('Restarted Monitoring')