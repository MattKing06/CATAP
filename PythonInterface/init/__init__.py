import sys
import os

CATAP_DIR = os.path.dirname(os.path.abspath(__file__))
for path, directories, files in os.walk(CATAP_DIR):
    if "__pycache__" not in path:
        sys.path.append(os.path.abspath(path))
