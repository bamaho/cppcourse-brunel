import numpy as np
import matplotlib.pyplot as pl

data = np.genfromtxt('simulationData.txt')

select= np.array([d for d in data if d[1] < 30])
data1= select.transpose()
#pl.subplot(2,1,1)
pl.scatter(data1[0],data1[1],alpha=0.8, edgecolors='none');
pl.xlabel('time [ms]');
pl.ylabel('neuron identifier');
#pl.xaxis.set_visible(False)
xmin, xmax = pl.xlim()
pl.show();


data1=data.transpose()
#pl.subplot(2,1,2)
pl.xlabel('time [ms]');
pl.ylabel('number of spikes per 0.1 ms');
n, bins, patches = pl.hist(data1[0], (xmax-xmin)*10, normed=0, alpha=0.75)
pl.show();






