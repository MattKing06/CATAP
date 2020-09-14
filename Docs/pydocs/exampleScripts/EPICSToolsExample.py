from CATAP.EPICSTools import *
import random
import time
# Creating an EPICSTools object without the STATE argument
# will force CATAP to use the Virtual EPICS instance.
# i.e. ET = EPICSTools()
#
# Adding a STATE argument will specify which EPICS instance to use.
ET = EPICSTools(STATE.VIRTUAL)

# Monitoring a PV is done via EPICSTools by providing the PV:
readi_pv = "CLA-C2V-MAG-HCOR-01:READI"
ET.monitor(readi_pv)

# To access the monitor object, it can be accessed via EPICSTools:
readi_monitor = ET.getMonitor(readi_pv)
print("Current READI Value: ", readi_monitor.getValue())


# Putting to a PV can only be done through the EPICSTools object
seti_pv = "CLA-C2V-MAG-HCOR-01:SETI"
set_current = int(random.random() * 10)
ET.put(seti_pv, set_current)

# Getting a PV value can only be done through the EPICSTools object
getseti_pv = "CLA-C2V-MAG-HCOR-01:GETSETI"
ET.get(getseti_pv)

# Example of using monitor, put, and get:

# Turn on HCOR-01
spower_pv = "CLA-C2V-MAG-HCOR-01:SPOWER"
ET.put(spower_pv, 1)
# Wait for HCOR-01 READI value to reach SETI
while(readi_monitor.getValue() != ET.get(getseti_pv)):
    print("READI VALUE: ", readi_monitor.getValue())
    time.sleep(0.1)
print("READI VALUE: ", readi_monitor.getValue(), " SETI VALUE: ", ET.get(getseti_pv))

# From the monitor object, buffer, buffer average, 
# and buffer standard deviation can be accessed:
print("Current READI Buffer: ", readi_monitor.getBuffer())
print("READI Buffer Average: ", readi_monitor.getBufferAverage())
print("READI Buffer Standard Deviation: ", readi_monitor.getBufferStdDev())