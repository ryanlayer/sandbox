#!/usr/bin/env python
import random
import sys

if len(sys.argv) < 4:
    print "usage:\t" + sys.argv[0] + " <length> <delim> <char 1> <char 2> <...>"
    exit(1)

out_len = int(sys.argv[1])
delim = sys.argv[2].decode("string_escape")

alphabet = sys.argv[3:]

rand_out = ""
for i in xrange(0,out_len):
    if i!=0:
        rand_out += delim
    rand_out += random.choice(alphabet)

print rand_out
