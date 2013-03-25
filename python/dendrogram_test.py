import numpy as np
from scipy.cluster.hierarchy import dendrogram
from scipy.cluster.hierarchy import linkage
import matplotlib.pyplot as plt
#from augmented_dendrogram import augmented_dendrogram

def augmented_dendrogram(*args, **kwargs):

	ddata = dendrogram(*args, **kwargs)

#	if not kwargs.get('no_plot', False):
#		for i, d in zip(ddata['icoord'], ddata['dcoord']):
#			x = 0.5 * sum(i[1:3])
#			y = d[1]
#			plt.plot(x, y, 'ro')
#			plt.annotate("%.3g" % y, (x, y), xytext=(0, -8),
#                         textcoords='offset points',
#                         va='top', ha='center')
#
	return ddata


# Generate a random sample of `n` points in 2-d.
np.random.seed(12312)
n = 100
x = np.random.multivariate_normal([0, 0], np.array([[4.0, 2.5], [2.5, 1.4]]),
                                  size=(n,))
#print x
#y = ((1 2 3 4) (5 6 7 8))
y= np.array( 
[[0,0.024660349,0.004922136],
[0.030837445,0,0.027316199],
[0.013020413,0.026084964,0],
[0,0.024706808,0.004896842]])
#[1.68891E-05,0.024617674,0.00482922]])
#[0.00020065,0.024576607,0.005145246]])
#[[1,0.975339651,0.995077864],
#[0.969162555,1,0.972683801],
#[0.986979587,0.973915036,1]])

#[[1,0.969162555,0.986979587,1,0.999983111,0.99979935],
#[0.975339651,1,0.973915036,0.975293192,0.975382326,0.975423393],
#[0.995077864,0.972683801,1,0.995103158,0.99517078,0.994854754]])

#plt.figure(1, figsize=(6, 5))
#plt.clf()
#plt.scatter(x[:, 0], x[:, 1])
#plt.axis('equal')
#plt.grid(True)
#
#print x
#
#
#
linkage_matrix = linkage(y, "single")
#
#plt.figure(2, figsize=(10, 4))
#plt.clf()
#
#plt.subplot(1, 2, 1)
#show_leaf_counts = False
ddata = dendrogram(linkage_matrix,
               color_threshold=1,
               p=6,
               truncate_mode='lastp',
               )
#ddata = augmented_dendrogram(linkage_matrix,
               #color_threshold=1,
               #p=6,
               #truncate_mode='lastp',
               #)
#plt.title("show_leaf_counts = %s" % show_leaf_counts)
#
#plt.subplot(1, 2, 2)
#show_leaf_counts = True
#ddata = augmented_dendrogram(linkage_matrix,
#               color_threshold=1,
#               p=6,
#               truncate_mode='lastp',
#               show_leaf_counts=show_leaf_counts,
#               )
#plt.title("show_leaf_counts = %s" % show_leaf_counts)
#
plt.show()
