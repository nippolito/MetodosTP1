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
# expAxMenosXProba()

# ----------------------REENTREGA-------------------

def expMaxMinEstrellaN100():
	df1 = pd.read_csv('../out/estrella/maxmin_n100.csv')

	xdata = df1['p']
	ydata = df1['max'] - df1['min']

	plt.plot(xdata, ydata, "r", alpha = 0.8, label='|maxRank - minRank|')
	plt.plot(0.5, 0.7)

	plt.xlabel('p')
	plt.ylabel('|maxRank - minRank|')
	plt.title('Grafo estrella 100 nodos')

	plt.legend()
	plt.show()

def expMaxMinDienteLeonN100():
	df1 = pd.read_csv('../out/diente_leon/maxmin_n100.csv')

	xdata = df1['p']
	ydata = df1['max'] - df1['min']

	plt.plot(xdata, ydata, "g", alpha = 0.8, label='|maxRank - minRank|')
	plt.plot(0.5, 0.7)

	plt.xlabel('p')
	plt.ylabel('|maxRank - minRank|')
	plt.title('Grafo diente de leon 100 nodos')

	plt.legend()
	plt.show()

def expMaxMinRetro1N100():
	df1 = pd.read_csv('../out/retro_1/maxmin_n100.csv')

	xdata = df1['p']
	ydata = df1['max'] - df1['min']

	plt.plot(xdata, ydata, "b", alpha = 0.8, label='|maxRank - minRank|')
	plt.plot(0.5, 0.7)

	plt.xlabel('p')
	plt.ylabel('|maxRank - minRank|')
	plt.title('Grafo retroalimentacion 1 100 nodos')

	plt.legend()
	plt.show()

def expMaxMinRetro2N100():
	df1 = pd.read_csv('../out/retro_2/maxmin_n100.csv')

	xdata = df1['p']
	ydata = df1['max'] - df1['min']

	plt.plot(xdata, ydata, "c", alpha = 0.8, label='|maxRank - minRank|')
	plt.plot(0.5, 0.7)

	plt.xlabel('p')
	plt.ylabel('|maxRank - minRank|')
	plt.title('Grafo retroalimentacion 2 100 nodos')

	plt.legend()
	plt.show()

def expMaxMinPiramideN100():
	df1 = pd.read_csv('../out/piramide/maxmin_n100.csv')

	xdata = df1['p']
	ydata = df1['max'] - df1['min']

	plt.plot(xdata, ydata, "m", alpha = 0.8, label='|maxRank - minRank|')
	plt.plot(0.5, 0.7)

	plt.xlabel('p')
	plt.ylabel('|maxRank - minRank|')
	plt.title('Grafo piramide 100 nodos')

	plt.legend()
	plt.show()

def expMaxMinSimetricoN100():
	df1 = pd.read_csv('../out/simetrico/maxmin_n100.csv')

	xdata = df1['p']
	ydata = df1['max'] - df1['min']

	plt.plot(xdata, ydata, "y", alpha = 1, label='|maxRank - minRank|')
	plt.plot(0.5, 0.7)

	plt.xlabel('p')
	plt.ylabel('|maxRank - minRank|')
	plt.title('Grafo simetrico 100 nodos')

	plt.legend()
	plt.show()

def expMaxMinTodosJuntosN100():
	df1 = pd.read_csv('dens2.csv')
	df2 = pd.read_csv('dens5.csv')
	df3 = pd.read_csv('dens8.csv')

	xdata1 = df1['p']
	ydata1 = df1['maxPageRank'] - df1['minPageRank']
	xdata2 = df2['p']
	ydata2 = df2['maxPageRank'] - df2['minPageRank']
	xdata3 = df3['p']
	ydata3 = df3['maxPageRank'] - df3['minPageRank']
	

	plt.plot(xdata1, ydata1, "r", alpha = 0.8, label='densidad baja')
	plt.plot(xdata2, ydata2, "g", alpha = 0.8, label='densidad media')
	plt.plot(xdata3, ydata3, "b", alpha = 0.8, label='densidad alta')
	
	plt.plot(0.005, 0.015)

	plt.xlabel('p')
	plt.ylabel('|maxRank - minRank|')
	plt.title('p vs |maxRank - minRank|, n = 100')
	plt.grid()
	plt.legend()
	plt.show()	

def expPageRanksVspDensBaja():
	df1 = pd.read_csv('dens2.csv')
	

	xdata1 = df1['p']
	ydata1 = df1['minPageRank']
	ydata2 = df1['maxPageRank'] 
	

	plt.plot(xdata1, ydata1, "r", alpha = 0.8, label='minRank')
	plt.plot(xdata1, ydata2, "g", alpha = 0.8, label='maxRank')
	
	
	plt.plot(0.005, 0.02)

	plt.xlabel('p')
	plt.ylabel('page rank')
	plt.title('p vs page rank, n = 100, densidad baja')
	plt.grid()
	plt.legend()
	plt.show()	

def expPageRanksVspDensMedia():
	df1 = pd.read_csv('dens5.csv')
	

	xdata1 = df1['p']
	ydata1 = df1['minPageRank']
	ydata2 = df1['maxPageRank'] 
	

	plt.plot(xdata1, ydata1, "r", alpha = 0.8, label='minRank')
	plt.plot(xdata1, ydata2, "g", alpha = 0.8, label='maxRank')
	
	
	plt.plot(0.005, 0.02)

	plt.xlabel('p')
	plt.ylabel('page rank')
	plt.title('p vs page rank, n = 100, densidad media')
	plt.grid()
	plt.legend()
	plt.show()	

def expPageRanksVspDensAlta():
	df1 = pd.read_csv('dens8.csv')
	

	xdata1 = df1['p']
	ydata1 = df1['minPageRank']
	ydata2 = df1['maxPageRank'] 
	

	plt.plot(xdata1, ydata1, "r", alpha = 0.8, label='minRank')
	plt.plot(xdata1, ydata2, "g", alpha = 0.8, label='maxRank')
	
	
	plt.plot(0.005, 0.02)

	plt.xlabel('p')
	plt.ylabel('page rank')
	plt.title('p vs page rank, n = 100, densidad alta')
	plt.grid()
	plt.legend()
	plt.show()	

#expMaxMinEstrellaN100()
#expMaxMinDienteLeonN100()
#expMaxMinRetro1N100()
#expMaxMinRetro2N100()
#expMaxMinPiramideN100()
#expMaxMinSimetricoN100()
expMaxMinTodosJuntosN100()
expPageRanksVspDensBaja()
expPageRanksVspDensMedia()
expPageRanksVspDensAlta()


