from CATAP.EPICSTools import *

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
# From this object, the value, buffer, buffer average, 
# and buffer standard deviation can be accessed:
print("Current READI Value: ", readi_monitor.getValue())
print("Current READI Buffer: ", readi_monitor.getBuffer())
print("READI Buffer Average: ", readi_monitor.getBufferAverage())
print("READI Buffer Standard Deviation: ", readi_monitor.getBufferStdDev())

# Putting to a PV can only be done through the EPICSTools object
seti_pv = "CLA-C2V-MAG-HCOR-01:SETI"
ET.put(seti_pv, 10.0)

# Getting a PV value can only be done through the EPICSTools object
getseti_pv = "CLA-C2V-MAG-HCOR-01:GETSETI"
ET.get(getseti_pv)

# Example of using monitor, put, and get:
spower_pv = "CLA-C2V-MAG-HCOR-01:SPOWER"
ET.put(spower_pv, 1)
while(readi_monitor.getValue() != ET.get(getseti)):
    print("READI VALUE: ", readi_monitor.getValue()
print("READI VALUE: ", readi_monitor.getValue(), " SETI VALUE: ", ET.get(getseti_pv))