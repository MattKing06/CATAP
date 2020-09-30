import time
print('Importing CATAP HW Factory and IMG HW successfully')
from CATAP.HardwareFactory import *
from CATAP.IMG import *

time.sleep(2)
print('Imported CATAP HW Factory and IMG HW successfully')
print('++++++++++++++++++++++++++++++++++++++++++++++++++')
time.sleep(2)


# create a hardware factory for Virtual CLARA hardware
hw_factory = HardwareFactory(STATE.VIRTUAL)
# turn off messaging
hw_factory.messagesOff()
hw_factory.debugMessagesOff()
# setup the IMG hardware and factory
hw_factory.setup("IMG", "Nominal")
img_factory = hw_factory.getIMGFactory()
# get all of the IMG names into a list
img_names = img_factory.getAllNames()
print('All img names are listed {}\n'.format(img_names))
# create a sub-list of IMG names
img_name_sub_list = img_names[0:5]
# get states and pressures of imgs in the sub-list
print(img_factory.getStates(img_name_sub_list))
print(img_factory.getPressures(img_name_sub_list))
# loop over list and print the img state and pressure for each img by-name
for img_name in img_names:
    print(img_name, " : ", img_factory.getState(img_name))
    print("Pressure ",img_name, " : ", img_factory.getPressure(img_name))
# get a the "INJ-VAC-IMG-01" IMG object
img_object = img_factory.getIMG("EBT-LLV-VAC-IMG-01")
# print the name property for the img
print("IMG Name: ", img_object.name)
# print the state and pressure property for the img
print("IMG State: ", img_object.state)
print("IMG Pressure: ", img_object.pressure)
