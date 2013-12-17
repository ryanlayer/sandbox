#!/usr/bin/env python
from optparse import OptionParser
import threading
import Queue
import time, random
import sys
import contextlib
import subprocess
#from subprocess import Popen, PIPE

parser = OptionParser()

parser.add_option("-w",
    "--wait",
    dest="wait_time",
    type="float",
    default=0.25,
    help="Poll wait time in seconds (Default 0.25)")

parser.add_option("-c",
    "--cmd",
    dest="cmds",
    help="Comma seperated list of commands")

parser.add_option("-b",
    "--buf",
    dest="buf",
    default=10,
    type="int",
    help="Thread output buffer size (Default 10)")


(options, args) = parser.parse_args()

if not options.cmds:
    parser.error('Commands not given')


class Worker(threading.Thread):
    def __init__(self, queue, cmd):
        self.cmd = cmd
        self.__queue = queue
        threading.Thread.__init__(self)

    def run(self):
        proc = subprocess.Popen(self.cmd, stdout=subprocess.PIPE)

        buff = []
        buff_size = options.buf 
        while True:
            line = proc.stdout.readline()
            if not line: break
            if (len(buff) < buff_size):
                buff.append(line.rstrip())
            else:
                self.__queue.put(buff)
                buff = []
                buff.append(line.rstrip())
        if len(buff) > 0:
            self.__queue.put(buff)

queue = Queue.Queue(0)

cmds = options.cmds.split(',')
for cmd in cmds:
    p_cmd = cmd.split()
    Worker(queue,p_cmd).start() # start a worker

while threading.active_count() > 1:
    while (queue.qsize() > 0): 
        for o in queue.get():
            print o
    time.sleep(options.wait_time)

while (queue.qsize() > 0): 
    for o in queue.get():
        print o
    time.sleep(options.wait_time)
 
