#include <iostream>
#include <fstream>
#include <chrono>
#include "../matrizRala.h"

//-------------------------------------------------------------GENERADORES DE MATRICES ALEATORIAS
//TODOS LOS GENERADORES TOMAN UNA MATRIZ NULA COMO ENTRADA!

//Genera una matriz aleatoria con varios paramtros:

//proba: Es la probabilidad de agregar un elemento en cada columna. Tambien se puede ver como la DENSIDAD de una matriz.
//		 Toma valores de 0 a 10. Si es 0, sera una matriz nula, y si es 10 tendra un valor distinto a 0 para cada Aij.
//
//Fmin y Fman es el rango de valores que pueden tomar los elementos de la matriz.
//Ver que si no es posible hacerse, devuelve un -1


int generarMatrizAleatoria(Rala& A, int proba, double fMin, double fMax){
	int n = A.n;
	int rangoProba = proba;
	if(rangoProba > 10 || rangoProba < 0 || fMin > fMax ){return -1;}

	if (rangoProba == 10){rangoProba = 9; }
	
	if(rangoProba != 0){

		for (int fila = 0; fila < n; ++fila)
		{
			for (int columna = 0; columna < n; ++columna)
			{
				int prob = rand() % 10;
				if(prob <= rangoProba){

					double valor = (fMax - fMin) * ( (double)rand() / (double)RAND_MAX ) + fMin;
					insertarElemento(A, fila, columna, valor );
				}
			}
		}

		return 1;
	
	} else {

		return 1;
	
	}	
}

// proba es la densidad (en el rango 0 a 10)
int generarMatrizConectividad(Rala* A, int proba){
	int n = A ->n;
	int rangoProba = proba;
	if(rangoProba > 10 || rangoProba < 0){return -1;}
	
	if(rangoProba != 0){

		for (int fila = 0; fila < n; ++fila)
		{
			for (int columna = 0; columna < n; ++columna)
			{	
				if(fila != columna){
					int prob = (rand() % 10)+1;
					if(prob <= rangoProba){
					insertarElemento(*A, fila, columna, 1);
					}	
				}	
			}
		}

		return 1;
	
	} else {

		return 1;
	
	}	

}

void crearMatrizEZ(Rala& diagonal, Rala& ez, double p){
	for (int i = 0; i < diagonal.n; i++)
	{
		if (diagonal.conex[i].find(i)->second > 0)
		{
			for (int j = 0; j < diagonal.n; j++)
			{
				insertarElemento(ez, j, i, (1-p)/diagonal.n);

			}
		}
		else
		{
			for (int j = 0; j < diagonal.n; j++)
			{
				insertarElemento(ez, j, i, 1.0/diagonal.n);
			}
		}
	}
}

int generarMatrizIdentidad(Rala A){
	int n = A.n;

		for (int fila = 0; fila < n; ++fila)
		{
			insertarElemento(A, fila, fila, 1);
		}

		return 1;
	
}

void generarMatrizDeViajeroAleatorio(Rala& Wm, Rala& A, double p){
	//creo matriz diagonal
	Rala Diagonal = Rala(Wm.n);
	generarMatrizDiagonalD(Diagonal, Wm);

	//creo WD = pWD
	Rala WD = Rala(Wm.n);
	multiplicacionMatricial(Wm, Diagonal, WD);
	multiplicacionPorEscalar(WD, p);
	
	//creo matriz e * z traspuesto
	Rala ez = Rala(Wm.n);
	crearMatrizEZ(Diagonal, ez, p);

	sumaMatricial(WD, ez, A);

}


//-------------------------------------------------------------TESTS


//resta dos vectores
void normalizarDiferenciaVectores(vector<double>& a, vector<double>& b){
	double sumaNormalizar = 0.0;

	for (int i = 0; i < a.size(); i++)
	{
		a[i] = abs(a[i] - b[i]);
		// ahora en cada a[i] tengo el error de cada x_i específico
		// sumaNormalizar += a[i];

	}
	// for (int i = 0; i < a.size(); i++)
	// {
	// 	a[i] = a[i]/sumaNormalizar;
	// }
}

void comparadorDeResultados(Rala& W, double p){
	//genero vector resultado de page rank
	vector<double> res(W.n, 0);

	//genero una matriz A para viajero aleatorio
	Rala A = Rala(W.n);
	generarMatrizDeViajeroAleatorio(W, A, p);

	//multiplico la matriz por el vector resultado y guardo el nuevo resultado
	vector<double> multiplicacionAporRes(A.n, 0);
	multiplicacionPorVector(A, res, multiplicacionAporRes);

	normalizarDiferenciaVectores(res, multiplicacionAporRes);
	// ahora en cada elemento de res tengo el error relativo de ese res_i, si sumo todos tengo la norma 1

	double errorTot = 0;
	for(int i = 0; i < res.size(); i++){
		errorTot += res[i];
	}

	// errorTot es la norma 1 de (multiplicacionAporRes - res) == |Ax - x|_1

	// errorTot = errorTot / res.size();

	cout << errorTot << endl;

	// mostrarVector(res);
}

void testComparadorDeResultados(){
	fstream ent ("Enunciado/tests_tp1/test_aleatorio_desordenado.txt", ios::in);
	
	//creo la matriz Rala
	int n;
	int m;
	ent >> n;
	ent >> m;
	Rala W = Rala(n);

	// lleno la matriz Rala W con todas las conexiones, o sea la matriz de conectividad
	for(int i = 0; i < m; i++){
		int source;
		int dest;
		ent >> source;
		ent >> dest;
		insertarElemento(W, dest - 1, source - 1, 1);
	}

	double p = 0.76;
	vector<double> res(n, 0);
	comparadorDeResultados(W, p);
}

// dejo fijo densidad (primero 0.2 y luego 0.5) y p. Varío n desde 10 hasta 1000 (aumentando de a 10) con 10 iteraciones en cada uno.
// Testeo |Ax-x| en cada uno y de paso printeo el tiempo ya que estamos (el que tarda en resolver el pageRank)
void testNormaAxMenosX(){
	srand(500); 	// semilla 500 porque pinta
	fstream sal("test_NormaAxMenosX.csv", ios::out);
	int desde = 10;
	int hasta = 1000;
	int densidad;
	double p = 0.4; // probabilidad para testear

	sal << "n,normaAxMenosX,TiempoPR,densidad" << endl;

	for(int varia = 0; varia < 2; varia++){
		if(varia == 0) densidad = 2;
		if(varia == 1) densidad = 5;
		// testeo con dos densidades por el tema de que con densidad 5 no habrá cj en 0 muy probablemente
		for(int i = desde; i < hasta + 1; i = i + 10){
			cout << "voy por n: ";
			cout << i << endl;
			for(int j = 0; j < 10; j++){
				// 10 repeticiones cada matriz

				int n = i;
				sal << n;
				sal << ",";
				Rala W = Rala(n);
				generarMatrizConectividad(&W, densidad);
				vector<double> res(n, 0);

				// resuelvo pageRank y tomo el tiempo
				std::chrono::time_point<std::chrono::system_clock> start, end;
				start = std::chrono::system_clock::now();
				resolverPageRank(W, res, p);
				end = std::chrono::system_clock::now();
				std::chrono::duration<double, std::milli> elapsed_seconds = end-start;

				//genero una matriz A para viajero aleatorio
				Rala A = Rala(W.n);
				generarMatrizDeViajeroAleatorio(W, A, p);

				//multiplico la matriz por el vector resultado y guardo el nuevo resultado
				vector<double> multiplicacionAporRes(A.n, 0);
				multiplicacionPorVector(A, res, multiplicacionAporRes);

				normalizarDiferenciaVectores(res, multiplicacionAporRes);
				// ahora en cada elemento de res tengo el error relativo de ese res_i, 

				double errorTot = 0;
				for(int i = 0; i < res.size(); i++){
					errorTot += pow(res[i],2);
				}

				errorTot = sqrt(errorTot);

				// errorTot es la norma 2 de (multiplicacionAporRes - res) == |Ax - x|

				sal << errorTot;
				sal << ",";
				sal << elapsed_seconds.count();
				sal << ",";
				if(densidad == 2){
					sal << 0.2 << endl;
				}else{
					sal << 0.5 << endl;
				}
				
			}
		}		
	}

	sal.close();
}

// fijo n = 1000 y densidad también fijada, y voy variando p desde 0.1 a 0.9 para descubrir
// si varía el error relativo según p
void testCambiaPrecisionSegunP(){
	srand(600); 	// semilla 600 porque pinta
	fstream sal("test_cambiaPrecisionSegunP2.csv", ios::out);
	double densidad = 5; // densidad matrices
	double p = 0.5;
	int sizeMatriz = 1000;

	sal << "n,normaAxMenosX,TiempoPR,p" << endl;

	for(int i = 1000; i < 1009; i++){
		cout << "voy por p: ";
		cout << p << endl;
		for(int j = 0; j < 30; j++){
			sal << sizeMatriz;
			sal << ",";
			Rala W = Rala(sizeMatriz);
			generarMatrizConectividad(&W, densidad);
			vector<double> res(sizeMatriz, 0);

			// resuelvo pageRank y tomo el tiempo
			std::chrono::time_point<std::chrono::system_clock> start, end;
			start = std::chrono::system_clock::now();
			resolverPageRank(W, res, p);
			end = std::chrono::system_clock::now();
			std::chrono::duration<double, std::milli> elapsed_seconds = end-start;

			//genero una matriz A para viajero aleatorio
			Rala A = Rala(W.n);
			generarMatrizDeViajeroAleatorio(W, A, p);

			//multiplico la matriz por el vector resultado y guardo el nuevo resultado
			vector<double> multiplicacionAporRes(A.n, 0);
			multiplicacionPorVector(A, res, multiplicacionAporRes);

			normalizarDiferenciaVectores(res, multiplicacionAporRes);
			// ahora en cada elemento de res tengo el error relativo de ese res_i, 

			double errorTot = 0;
			for(int k = 0; k < res.size(); k++){
				errorTot += pow(res[k],2);
			}

			errorTot = sqrt(errorTot);

			// errorTot es la norma 2 de (multiplicacionAporRes - res) == |Ax - x|

			sal << errorTot;
			sal << ",";
			sal << elapsed_seconds.count();
			sal << ",";
			sal << p << endl;
		}
		p = p + 0.1;
	}
	sal.close();
}

int main(){
	srand(time(NULL)); // COMENTAR ESTO PARA CORRER MI TEST
	
	testNormaAxMenosX();
	// testCambiaPrecisionSegunP();

	return 0;
}