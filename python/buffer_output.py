#import subprocess, time, os, sys
##cmd = ["rsync.exe", "-vaz", "-P", "source/" ,"dest/"]
#cmd = [sys.executable,"./dum.py", "-i", "1", "-w", "0.5", "-o", "5", "-t", "10"]
#
#p = subprocess.Popen(cmd,
#    stdout=subprocess.PIPE,
#    stderr=subprocess.STDOUT)
#
#while True:
#    out = p.stdout.read(1)
#    if out == '' and p.poll() != None:
#        break
#    if out != '':
#        sys.stdout.write(out)
#        sys.stdout.flush()

import sys
from subprocess import Popen, PIPE

proc = Popen([sys.executable, "-u", "./dum.py", "-i", "1", "-w", "0.5", "-o", "5", "-t", "10"],\
    stdout=PIPE, bufsize=1)

buff = []
buff_size = 3
for line in iter(proc.stdout.readline, b''):
    if (len(buff) < buff_size):
        buff.append(line.rstrip())
    else:
        print buff
        buff = []
        buff.append(line.rstrip())
print buff
