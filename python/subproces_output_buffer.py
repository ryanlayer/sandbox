#!/usr/bin/env python
from optparse import OptionParser
import threading
import Queue
import time, random
import sys
from subprocess import Popen, PIPE

parser = OptionParser()

parser.add_option("-w",
    "--wait",
    dest="wait",
    type="float",
    help="Poll wait time")

parser.add_option("-c",
    "--cmd",
    dest="cmds",
    help="Comma seperated list of commands")

(options, args) = parser.parse_args()

if not options.wait:
    parser.error('Wait time not given')

if not options.cmds:
    parser.error('Commands not given')


class Worker(threading.Thread):
    def __init__(self, queue, cmd):
        self.cmd = cmd
        self.__queue = queue
        threading.Thread.__init__(self)

    def run(self):
        #arg1 = [sys.executable, "-u"]
        #arg1 = [sys.executable, "-u"]
        #arg1 = ["/bin/ls","-1"]
        #arg1 = [sys.executable, "-u"]
        #arg1.extend(self.cmd)
#        arg1_t = [sys.executable, "-u", 
#            "./dum.py", "-i","1", \
#                    "-w", "0.5", \
#                    "-o", "5", \
#                    "-t", "10"]
#        proc = Popen([sys.executable, "-u", 
#            "./dum.py", "-i","1", \
#                    "-w", "0.5", \
#                    "-o", "5", \
#                    "-t", "10"],\
#            stdout=PIPE, bufsize=1)

        proc = Popen(self.cmd, stdout=PIPE, bufsize=1)
        #proc = Popen(self.cmd, stdout=PIPE, bufsize=1).stdout


        buff = []
        buff_size = 3
        #for line in iter(proc.stdout.readline, b''):
        #for line in iter(proc.readline, b''):
        while True:
            line = proc.stdout.readline()
            if not line: break
            print "XX"
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
        print "--"
    time.sleep(0.25)

while (queue.qsize() > 0): 
    for o in queue.get():
        print o
    print "~~"
    time.sleep(0.25)
 
