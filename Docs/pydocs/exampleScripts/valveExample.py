from CATAP import *

# create a hardware factory for CLARA hardware
HF = HardwareFactory(STATE.PHYSICAL)
# turn on messaging
HF.messagesOn()
# setup the Valve hardware and factory
HF.setup("Valve", "Nominal")
# get the Valve factory
VF = HF.getValveFactory()
# get all of the valve names into a list
valv_name_list = VF.getAllValveNames()
# print the name list
print(valv_name_list)
# loop over list and print the valve state for each valve by-name
for valve_name in valv_name_list:
    print(VF.getValveState(valve_name))
# get a the "INJ-VAC-VALV-06" valve
valve_object = VF.getValve("EBT-INJ-VAC-VALV-06")
# print the name property for the valve
print(valve_object.name)
# print the state property for the valve
print(valve_object.state)
