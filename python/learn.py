from sklearn.naive_bayes import *
from sklearn.ensemble import RandomForestClassifier
import numpy as np


X = [ \
	[.9,.1,.2],
	[.8,.2,.2],
	[.9,.1,.2],
	[.8,.2,.2],
	[.9,.1,.2],
	[.8,.2,.2],
	[.9,.1,.2],
	[.8,.2,.2],
	[.1,.9,.2],
	[.2,.9,.2],
	[.1,.9,.2],
	[.2,.9,.2],
	[.1,.9,.2],
	[.2,.9,.2],
	[.1,.9,.2],
	[.2,.9,.2],
]

Y = [ \
	1, \
	1, \
	1, \
	1, \
	1, \
	1, \
	1, \
	1, \
	0, \
	0, \
	0, \
	0, \
	0, \
	0, \
	0, \
	0, \
]

h = [ .9, .1,.2 ]
	

X = np.array(X)
Y = np.array(Y)
h = np.array(h)


clf = BernoulliNB()
clf.fit(X, Y)
print clf.classes_
print clf.predict_proba( h )

rf = RandomForestClassifier()
rf.fit(X, Y)
print rf.classes_
print rf.predict_proba( h )

