import os
import sys
import subprocess
import time
import numpy as np
from copy import copy, deepcopy
from paramiko import *

print('Importing CATAP HW Factory and IMG HW successfully')
from CATAP.HardwareFactory import *
from CATAP.IMG import *

time.sleep(2)
print('Imported CATAP HW Factory and IMG HW successfully')
print('++++++++++++++++++++++++++++++++++++++++++++++++++')
time.sleep(2)


class VmTesting(object):
    path = ''
    ip = '10.10.0.12'
    user = 'vsim'
    password = 'password'

    def __init__(self):
        self.hw_factory = HardwareFactory(STATE.VIRTUAL)
        self.img_factory = self.hw_factory.getIMGFactory()
        self.img_names = []
        self.single_pressure = 0
        self.img_pressures = {}

    @property
    def img_names(self):
        return self.__img_names

    @property
    def single_pressure(self):
        return self.__single_pressure

    @property
    def img_pressures(self):
        return self.__img_pressures

    @img_names.setter
    def img_names(self, names):
        self.__img_names = deepcopy(names)

    @single_pressure.setter
    def single_pressure(self, press):
        self.__single_pressure = press

    @img_pressures.setter
    def img_pressures(self, press_set):
        self.__img_pressures = deepcopy(press_set)

    @classmethod
    def setup_vm(cls):
        subprocess.call('ping ' + cls.ip)
        # client = SSHClient()
        # client.set_missing_host_key_policy(AutoAddPolicy())
        # client.connect(cls.ip, username=cls.user, password=cls.password)
        # stdin, stdout, stderr = client.exec_command('screen -ls')

    def turning_messaging_off(self):
        self.hw_factory.messagesOff()
        self.hw_factory.debugMessagesOff()

    def img_all_names(self):
        self.img_names = self.img_factory.getAllIMGNames()
        print('All img names are listed {}\n'.format(self.img_names))

    def get_pressure_from_img_name(self, img_name):
        img_obj = self.img_factory.getIMG(img_name)
        return img_obj.getPressure()

    def get_all_img_pressures(self):
        self.img_pressures = self.img_factory.getAllIMGPressures()
        print('All img Pressures are listed {}\n'.format(self.img_pressures))

    def get_some_img_pressures(self):
        print('for IMGs = {}, the Pressures are {} \n'.format(self.img_names[::2],
                                                            self.img_factory.getIMGPressures(self.img_names[::2])))

    def get_all_img_pressures_from_names(self):
        for img_n in self.img_names:
            print(
                ' Pressure: Factory level = {}, HW level = {}, for {} \n'.format(self.img_factory.getIMGPressure(img_n),
                                                                                self.get_pressure_from_img_name(img_n),
                                                                                img_n))

    def test_img_via_hf(self):
        self.turning_messaging_off()
        print('+++++++++++ IMG via the HW Factory +++++++++++++++++\n')
        self.img_all_names()
        time.sleep(2)
        self.get_all_img_pressures()
        time.sleep(2)
        self.get_some_img_pressures()
        time.sleep(2)
        self.get_all_img_pressures_from_names()
        time.sleep(2)

    @staticmethod
    def test_img_via_img_package():
        print('+++ setup of the img factory via the IMG package ++++\n')
        img_f = IMGFactory(STATE.VIRTUAL)
        img_f.setup("")  # you can put any string in here as it isn't dealt with
        time.sleep(2)
        print('(IMG package) All img names are listed {}\n'.format(img_f.getAllIMGNames()))
        time.sleep(2)
        print('(IMG package) All img Pressures are listed {}\n'.format(img_f.getAllIMGPressures()))
        time.sleep(2)
        print('(IMG package) All img Pressures are listed {}\n'.format(img_f.getAllIMGPressures()))
        for img_n in img_f.getAllIMGNames():
            print(
                '(IMG Package) Pressure: Factory level = {}, for {} \n'.format(img_f.getIMGPressure(img_n), img_n))
            time.sleep(2)

    def main(self):
        print('+++++++++++ Setting up the VM +++++++++++++++++\n')
        self.setup_vm()
        print('+++++++++++ IMG via the HW Factory +++++++++++++++++\n')
        self.test_img_via_hf()
        print('+++++++++++ IMG via the IMG Package +++++++++++++++++\n')
        self.test_img_via_img_package()
        time.sleep(3)
        print('+++++++++++ The end +++++++++++++++++\n')


if __name__ == '__main__':
    vm_test = VmTesting()
    vm_test.main()
