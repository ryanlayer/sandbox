#!/usr/bin/python

import re
import string
import random
import datetime

def id_generator(size=6, chars=string.ascii_uppercase):
	return ''.join(random.choice(chars) for x in range(size))

s = id_generator(100000)

chars = set('ABCD')

c = datetime.datetime.now()
if re.search('A|B|C|D',s):
	print 'Found'
else:
	print 'Not found'
d = datetime.datetime.now()

s = id_generator(100000)

e = datetime.datetime.now()
if re.search('A|B|C|D',s):
	print 'Found'
else:
	print 'Not found'
f = datetime.datetime.now()

s = id_generator(100000)
a = datetime.datetime.now()

if any((c in chars) for c in s):
	print 'Found'
else:
	print 'Not Found'
b = datetime.datetime.now()



print b - a
print d - c
print f - e
