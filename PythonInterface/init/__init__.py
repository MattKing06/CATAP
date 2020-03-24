from CATAP import *
import sys
import os

directory_list = next(os.walk('./CATAP'))[1]
directory_list.remove('__pycache__')
print(directory_list)
for directory in directory_list:
    sys.path.append(os.path.join(os.path.dirname(__file__), directory))
