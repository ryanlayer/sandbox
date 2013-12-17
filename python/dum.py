#!/usr/bin/env python
import sys
import time
import numpy as np
import random

from optparse import OptionParser

parser = OptionParser()

parser.add_option("-i",
    "--id",
    dest="pid",
    type="int",
    help="Process id")

parser.add_option("-w",
    "--wait",
    dest="wait",
    type="float",
    help="Wait time in seconds")

parser.add_option("-o",
    "--outputs",
    dest="outputs",
    type="int",
    help="Number of outputs per itteration")

parser.add_option("-t",
    "--total",
    dest="total",
    type="int",
    help="Total number of itterations")


(options, args) = parser.parse_args()

if not options.pid:
    parser.error('Process id not given')

if not options.wait:
    parser.error('Wait time not given')

if not options.outputs:
    parser.error('Number of outputs per itteration not given')
    
if not options.total:
    parser.error('Total numer of itteration not given')


for i in range(options.total):
    for j in range(options.outputs):
        print options.pid,random.random() 
    time.sleep(options.wait)
