#!/bin/bash
python3.8 -m pip wheel .
scp -r *.whl ujo48515@172.16.114.88:/var/www/html/wheels/CATAP/
