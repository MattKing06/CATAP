import sys
import os

for path, directories, files in os.walk('./'):
    if "__pycache__" not in path:
        sys.path.append(path)
