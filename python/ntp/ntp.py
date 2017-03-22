#!/usr/bin/python
from __future__ import division
from sys import argv
from datetime import *  
# When I investigate UTC and localtime,can't recognize the time of ms
# So I record the script
# ./ntp.py 1490152364552
# utcfromtimestamp(tmstmp) 2017-03-22 03:12:44.552000
if (len(argv) == 2):
    value = argv[1];
    print "utc from timestamp",datetime.utcfromtimestamp((int(value) / 1000))
else:
    print "NTP timestamp convert to date sample"
    print "  Enter cmd: ./ntp.py timestamp"
