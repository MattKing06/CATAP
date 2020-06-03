# This is the top-level __init__.py for CATAP modules
# that is copied over to the CATAP directory post-build

import sys
import os

__all__ = list()
CATAP_DIR = os.path.dirname(os.path.abspath(__file__))
for path, directories, files in os.walk(CATAP_DIR):
    if "__pycache__" not in path:
        sys.path.append(os.path.abspath(path))
    if directories is not []:
        for directory in directories:
            if "__pycache__" not in directory:
                __all__.append(directory)