#!/usr/bin/python

import threading, Queue



def do_work(item, db, R):
	#print item in db, threading.currentThread().getName()
	t_id = threading.currentThread().getName()
	if t_id not in R:
		R[t_id] = []
	R[t_id].append((item, item in db))
	#if 
	#if item in db:
		#R[threading.currentThread().getName()] = 1
	#else:
		#R[threading.currentThread().getName()] = 0


def worker():
	while True:
		item = q.get()
   		do_work(item, db, R)
   		q.task_done()

q = Queue.Queue()
db = [4,5,6]
R = {}


for i in range(5):
	t = threading.Thread(target=worker,name=i+1)
	t.daemon = True
	t.start()

for item in range(100):
	q.put(item)

q.join()       # block until all tasks are done
for r in R:
	print r,len(R[r])
