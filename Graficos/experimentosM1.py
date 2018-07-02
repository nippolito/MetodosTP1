# -*- coding: utf-8 -*-

import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
import random
import math
import numpy as np

def expAxMenosXTam():
	df1 = pd.read_csv('test_NormaAxMenosX.csv')

	Dens2 = df1[df1['densidad'] == 0.2]
	Dens5 = df1[df1['densidad'] == 0.5]

	xdata  = Dens2['n']

	ydata2 = Dens2['normaAxMenosX']
	ydata5 = Dens5['normaAxMenosX']

	plt.plot(xdata, ydata2, "r.", alpha = 0.5, label='|Ax - x|, densidad 0.2')
	plt.plot(xdata, ydata5, "b.", alpha = 0.5, label='|Ax - x|, densidad 0.5')

	plt.xlabel('Tamanio de fila matriz')
	plt.ylabel('|Ax-x|')
	plt.yscale('log')

	plt.legend()
	plt.show()

def expTiempo():
	df1 = pd.read_csv('test_tiemposSegunN.csv')

	enes = range(10, 1001, 10)

	todos = df1[df1['p'] == 0.4]
	todos_count = todos.groupby('n').mean()
	List1 = todos_count.TiempoPR.tolist()

	dataf1 = pd.DataFrame({'PageRank': List1, 'Size': enes})
	dataf1.astype(float)
	dataf1.plot(title='Tiempos segun N', x='Size')
	# dataf1.plot(title='', x='CantVertices', logy=True, kind='scatter', colorbar=True)
	plt.ylabel('Tiempo en ms')
	plt.xlabel('Size de matriz')

	plt.legend()
	plt.show()

def expAxMenosXDens():
	df1 = pd.read_csv('test_cambiaPrecisionSegunDensidad.csv')

	xdata  = df1['densidad']

	ydata = df1['normaAxMenosX']

	plt.plot(xdata, ydata, "r.", alpha = 0.5, label='|Ax - x|')

	plt.xlabel('Densidad matriz conectividad')
	plt.ylabel('|Ax-x|')
	plt.yscale('log')

	plt.legend()
	plt.show()

def expAxMenosXProba():
	df1 = pd.read_csv('test_cambiaPrecisionSegunPDens2.csv')
	df2 = pd.read_csv('test_cambiaPrecisionSegunPDens5.csv')

	xdata1  = df1['p']
	xdata2 = df2['p']

	ydata = df1['normaAxMenosX']
	ydata2 = df2['normaAxMenosX']

	plt.plot(xdata1, ydata, "m^", alpha = 0.5, label='|Ax - x| densidad 0.2')
	plt.plot(xdata2, ydata2, "y^", alpha = 0.5, label='|Ax - x| densidad 0.5')

	plt.xlabel('Parametro p utilizado')
	plt.ylabel('|Ax-x|')
	plt.yscale('log')

	plt.legend()
	plt.show()

# expAxMenosXTam()
# expTiempo()
# expAxMenosXDens()
expAxMenosXProba()
