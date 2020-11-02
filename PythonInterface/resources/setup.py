from setuptools import setup, find_namespace_packages, dist
from setuptools.command.install import install
import os

# force setuptools to recognize that this is
# actually a binary distribution
class BinaryDistribution(dist.Distribution):
    def has_ext_modules(empty):
        return True

# optional, use README.md as long_description
this_directory = os.path.abspath(os.path.dirname(__file__))
with open(os.path.join(this_directory, 'README.md')) as f:
    long_description = f.read()

setup(
    # this package is called mymodule
    name='CATAP',

    # this package contains one module,
    # which resides in the subdirectory mymodule
    # packages=find_packages(['CATAP.BPM,CATAP.Charge,CATAP.DataTypes','CATAP.GlobalStates',
              # 'CATAP.GlobalTypes','CATAP.HardwareFactory','CATAP.IMG','CATAP.LaserMirror',
              # 'CATAP.LED','CATAP.Lighting','CATAP.Magnet','CATAP.RFHeartbeat',
			  # 'CATAP.RFModulator', 'CATAP.RFProtection','CATAP.Screen','CATAP.Shutter',
			  # 'CATAP.Stage','CATAP.Valve']),
    packages=find_namespace_packages(),
    # make sure the shared library is included
    package_data={'BPM':['CATAP/BPM/_BPM.pyd'], 'Charge':['CATAP/Charge/_Charge.pyd'],
                  'DataTypes':['CATAP/DataTypes/_DataTypes.pyd'],
                  'GlobalStates':['CATAP/GlobalStates/_GlobalStates.pyd'],
                  'GlobalTypes':['CATAP/GlobalTypes/_GlobalTypes.pyd'],
                  'HardwareFactory':['CATAP/HardwareFactory/_HardwareFactory.pyd'],
                  'CATAP/IMG/IMG':['_IMG.pyd'],'LaserMirror':['CATAP/LaserMirror/_LaserMirror.pyd'],
                  'LED':['CATAP/LED/_LED.pyd'],'Lighting':['CATAP/Lighting/_Lighting.pyd'],
                  'Magnet':['CATAP/Magnet/_Magnet.pyd'],'RFHeartbeat':['CATAP/RFHeartbeat/_RFHeartbeat.pyd'],
                  'RFModulator':['CATAP/RFModulator/_RFModulator.pyd'], 
				  'RFProtection':['CATAP/RFProtection/_RFProtection.pyd'],
                  'Screen':['CATAP/Screen/_Screen.pyd'],'Shutter':['CATAP/Shutter/_Shutter.pyd'],
                  'Stage':['CATAP/Stage/_Stage.pyd'],'Valve':['CATAP/Valve/_Valve.pyd']},
    include_package_data=True,

    description="This is CATAP",
    # optional, the contents of README.md that were read earlier
    long_description=long_description,
    long_description_content_type="text/markdown",

    # See class BinaryDistribution that was defined earlier
    distclass=BinaryDistribution,

    version='0.0.1',
    url='http://example.com/',
    author='...',
    author_email='...@example.com',
    # ...
)