from CATAP.HardwareFactory import *

# create a hardware factory for CLARA hardware
HF = HardwareFactory(STATE.PHYSICAL)
# turn off messaging
HF.messagesOff()
HF.debugMessagesOff()
# setup the Valve hardware and factory
HF.setup("Valve", "Nominal")
# get the Valve factory
VF = HF.getValveFactory()
# get all of the valve names into a list
valve_name_list = VF.getAllValveNames()
# print the name list
print("Valve Name List: ", valve_name_list)
# create a sub-list of valve names
valve_name_sub_list = valve_name_list[0:5]
# get states of valves in the sub-list
print(VF.getValveStates(valve_name_sub_list))
# loop over list and print the valve state for each valve by-name
for valve_name in valve_name_list:
    print(valve_name, " : ", VF.getValveState(valve_name))
# get a the "INJ-VAC-VALV-06" valve
valve_object = VF.getValve("EBT-INJ-VAC-VALV-06")
# print the name property for the valve
print("Valve Name: ", valve_object.name)
# print the state property for the valve
print("Valve State: ", valve_object.state)
