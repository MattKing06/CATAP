#!/bin/bash
pip3 wheel .
scp -r *.whl ujo48515@172.16.114.88:/var/www/html/wheels/CATAP/
