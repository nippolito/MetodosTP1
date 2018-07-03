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


void crearCSV(vector<double> vec, string fileName){
	ofstream ata;

	//nombreHardcodeado para no cambiar parametros de entrada
	ata.open (fileName);

	for (int i = 0; i < vec.size(); ++i){
		ata << vec[i] << endl;
		// for (int j = 0; j < A.m; ++j)
		// {
		// 	map<int,double>::iterator it = A.conex[i].find(j);
		// 	if (it != A.conex[i].end())
		// 	{
		// 		ata << it->second;
		// 	}else{
		// 		ata << "0";
		// 	}

		// 	if(j == (A.m - 1)){ ata << "\n";} else { ata << ",";}
		// }
	}

	ata.close();
}

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

// ------------------NUEVOS GENERADORES PARA REENTREGA----------------

// crea la matriz de conectividad del grafo estrella.
// el 0 va a ser el x5
void generarGrafoEstrella(Rala& A){
	int n = A.n;
	int elemCentro = 0;
	for(int i = 0; i < n; i++){
		if(i != elemCentro){
			insertarElemento(A, elemCentro, i, 1);
		}
	}
}
// funciona OK

// crea la matriz de conectividad del grafo diente de león
// el n-1 va a ser el x6 del gráfico de grego, y el 0 va a ser el x5
void generarGrafoDienteDeLeon(Rala& A){
	int n = A.n;
	int elemCentro = 0;
	for(int i = 0; i < n - 1; i++){
		if(i != elemCentro){
			insertarElemento(A, elemCentro, i, 1);
		}
	}
	insertarElemento(A, n - 1, elemCentro, 1);
}
// funciona OK

// crea la matriz de conectividad del grafo retroalimentación 1
// el 1 va a ser el x2 del gráfico de grego
void generarGrafoRetro1(Rala& A){
	int n = A.n;
	generarGrafoDienteDeLeon(A);
	insertarElemento(A, 1, n-1, 1);
}
// funciona OK

// crea la matriz de conectividad del grafo retroalimentación 2
// el 1 va a ser el x2 del gráfico de grego
void generarGrafoRetro2(Rala& A){
	int n = A.n;
	generarGrafoDienteDeLeon(A);
	insertarElemento(A, 0, n-1, 1);
}
// funciona OK

// crea la matriz de conectividad del grafo pirámide
// el 0 es x5, y el n-1 es x6
void generarGrafoPiramide(Rala& A){
	int n = A.n;
	generarGrafoDienteDeLeon(A);
	insertarElemento(A, 1, n-2, 1);
	insertarElemento(A, n-2, 1, 1);
	for(int i = 1; i < n - 2; i++){
		insertarElemento(A, i, i+1, 1);
		insertarElemento(A, i+1, i, 1);
	}
}
// lo testié con un único caso y creo que está OK. Habría que ficharlo mejor 

// crea la matriz de conectividad del grafo simétrico
void generarGrafoSimetrico(Rala& A){
	int n = A.n;
	generarGrafoEstrella(A);
	for(int i = 1; i < n; i++){
		insertarElemento(A, i, 0, 1);
	}
}
// funciona OK


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
	resolverPageRank(W, res, p);

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
		errorTot += pow(res[i], 2);
	}

	errorTot = sqrt(errorTot);

	// errorTot es la norma 1 de (multiplicacionAporRes - res) == |Ax - x|_1

	// errorTot = errorTot / res.size();

	cout << errorTot << endl;

	// mostrarVector(res);
}

void testComparadorDeResultados(){
	fstream ent ("../Enunciado/tests_tp1/test_15_segundos.txt", ios::in);
	
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

	double p = 0.9;
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

void testNormaAxMenosXN2000(){
	srand(500); 	// semilla 500 porque pinta
	fstream sal("test_NormaAxMenosX2000.csv", ios::out);
	int desde = 2000;
	int hasta = 2000;
	int densidad = 2;
	double p = 0.4; // probabilidad para testear

	sal << "n,normaAxMenosX,TiempoPR,densidad" << endl;

	// testeo con dos densidades por el tema de que con densidad 5 no habrá cj en 0 muy probablemente
	for(int i = desde; i < hasta + 1; i = i + 10){
		cout << "voy por n: ";
		cout << i << endl;
		for(int j = 0; j < 20; j++){
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

	sal.close();
}

// fijo n = 1000 y densidad también fijada (en realidad dos valores distintos), y voy variando p desde 0.1 a 0.9 para descubrir
// si varía el error relativo según p
void testCambiaPrecisionSegunP(){
	srand(600); 	// semilla 600 porque pinta
	fstream sal("test_cambiaPrecisionSegunP2.csv", ios::out);
	double densidad; // densidad matrices
	double p = 0.05;
	int sizeMatriz = 800;

	sal << "n,normaAxMenosX,TiempoPR,p,densidad" << endl;

	for(int varia = 0; varia < 2; varia++){
		if(varia == 0){
			densidad = 2;
		}else{
			densidad = 5;
		}

		for(int i = 1000; i < 1019; i++){
			// serían 19 iteraciones de este ciclo. p = 0.05 a p = 0.95
			cout << "voy por p: ";
			cout << p << endl;
			for(int j = 0; j < 5; j++){
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
				sal << p;
				sal << ",";
				if(densidad == 2){
					sal << 0.2 << endl;
				}else{
					sal << 0.5 << endl;
				}
			}
			p = p + 0.05;
		}
	}
	sal.close();
}

// fijo n = 1000 y densidad también fijada (en realidad dos valores distintos), y voy variando p desde 0.1 a 0.9 para descubrir
// si varía el error relativo según p
void testCambiaPrecisionSegunDensidad(){
	srand(600); 	// semilla 600 porque pinta
	fstream sal("test_cambiaPrecisionSegunDensidad.csv", ios::out);
	double p = 0.4;
	int sizeMatriz = 800;

	sal << "n,normaAxMenosX,TiempoPR,densidad" << endl;

	for(int densidad = 1; densidad < 11; densidad++){
		cout << "voy por densidad: ";
		cout << densidad << endl;
		for(int j = 0; j < 10; j++){
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
			if(densidad == 1) sal << 0.1 << endl;
			if(densidad == 2) sal << 0.2 << endl;
			if(densidad == 3) sal << 0.3 << endl;
			if(densidad == 4) sal << 0.4 << endl;
			if(densidad == 5) sal << 0.5 << endl;
			if(densidad == 6) sal << 0.6 << endl;
			if(densidad == 7) sal << 0.7 << endl;
			if(densidad == 8) sal << 0.8 << endl;
			if(densidad == 9) sal << 0.9 << endl;
			if(densidad == 10) sal << 1 << endl;
		}
	}
	sal.close();
}

void testTiemposSegunN(){
	srand(500); 	// semilla 500 porque pinta
	fstream sal("test_tiemposSegunN.csv", ios::out);
	int desde = 10;
	int hasta = 1000;
	int densidad = 5;
	double p = 0.4;
	sal << "n,TiempoPR,densidad,p" << endl;

	for(int i = desde; i < hasta + 1; i = i + 10){
		cout << "voy por n: ";
		cout << i << endl;
		for(int j = 0; j < 20; j++){
			// 20 repeticiones cada matriz

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

			// errorTot es la norma 2 de (multiplicacionAporRes - res) == |Ax - x|

			sal << elapsed_seconds.count();
			sal << ",";
			sal << densidad;
			sal << ",";
			sal << p << endl;
		}
	}		
}


//----------------NUEVOS TESTS PARA REENTREGA-----------------

void variaPGrafoAleatorio(int n){
	double p = 0;

	Rala W1 = Rala(n);
	Rala W2 = Rala(n);
	Rala W3 = Rala(n);

	// pruebo densidad 8, 5 y 2
	generarMatrizConectividad(&W1, 8);
	generarMatrizConectividad(&W2, 5);
	generarMatrizConectividad(&W3, 2);

	for(int i = 0; i < 19; i++){
		vector<double> res1(n, 0);
		vector<double> res2(n, 0);
		vector<double> res3(n, 0);

		resolverPageRank(W1, res1, p);
		resolverPageRank(W2, res2, p);
		resolverPageRank(W3, res3, p);

		string fileName1 = "out/aleatorios/dens8_p" + to_string((int) (p * 101)) + ".csv"; 
		string fileName2 = "out/aleatorios/dens5_p" + to_string((int) (p * 101)) + ".csv"; 
		string fileName3 = "out/aleatorios/dens2_p" + to_string((int) (p * 101)) + ".csv"; 

		crearCSV(res1, fileName1);
		crearCSV(res2, fileName2);
		crearCSV(res3, fileName3);

		p = p - 0.05;
	}
}

void variaPGrafoEstrella(int n){
	double p = 0.95;

	Rala W1 = Rala(n);
	generarGrafoEstrella(W1);

	for(int i = 0; i < 19; i++){
		vector<double> res1(n, 0);

		resolverPageRank(W1, res1, p);

		string fileName1 = "out/estrella/p" + to_string((int) (p * 101)) + ".csv"; 

		crearCSV(res1, fileName1);

		p = p - 0.05;
	}
}

void pruebaMaxMinVariandoPDensidadBaja(int n, int cantIteraciones){
	double p = 1;

	vector<double> promedioMaximosPorCadaPDBaja(20, 0);

	vector<double> promedioMaximosPorCadaPDMedia(20, 0);

	vector<double> promedioMaximosPorCadaPDAlta(20, 0);

	vector<double> promedioMinimosPorCadaPDBaja(20, 0);

	vector<double> promedioMinimosPorCadaPDMedia(20, 0);

	vector<double> promedioMinimosPorCadaPDAlta(20, 0);

	for(int j = 0; j < 1; j++){
		Rala W1 = Rala(n);
		Rala W2 = Rala(n);
		Rala W3 = Rala(n);


		cout << "Iteracion numero " << j << endl;
		// pruebo densidad 8, 5 y 2
		generarMatrizConectividad(&W1, 8);
		generarMatrizConectividad(&W2, 5);
		generarMatrizConectividad(&W3, 2);

		for(int i = 0; i < 20; i++){

			cout << "Voy por p " << i * 0.05 << endl;
			vector<double> resAlta(n, 0);
			vector<double> resMedia(n, 0);
			vector<double> resBaja(n, 0);

			

			resolverPageRank(W1, resAlta, p);
			resolverPageRank(W2, resMedia, p);
			resolverPageRank(W3, resBaja, p);

			cout << "Calculo page ranks" << endl;

			double maxAlto = 0;
			double maxMedio = 0;
			double maxBaja = 0;
			double minAlto = 1;
			double minMedio = 1;
			double minBajo = 1;

			for (int k = 0; k < n; ++k)
			{
				if (resAlta[k] > maxAlto)
					maxAlto = resAlta[k];
				if (resAlta[k] < minAlto)
					minAlto = resAlta[k];

				if (resMedia[k] > maxMedio)
					maxMedio = resMedia[k];
				if (resMedia[k] < minMedio)
					minMedio = resMedia[k];

				if(resBaja[k] < 0)
					//cout << resBaja[k] << endl;

				if (resBaja[k] > maxBaja)
					maxBaja = resBaja[k];
				if (resBaja[k] < minBajo)
					minBajo = resBaja[k];
			}

			//cout << minBajo << endl;
			//cout << maxBaja << endl;

			promedioMaximosPorCadaPDBaja[i] += maxBaja;
			promedioMinimosPorCadaPDBaja[i] += minBajo;

			promedioMaximosPorCadaPDMedia[i] += maxMedio;
			promedioMinimosPorCadaPDMedia[i] += minMedio;

			promedioMaximosPorCadaPDAlta[i] += maxAlto;
			promedioMinimosPorCadaPDAlta[i] += minAlto;

			p = p + 0.05;
		}	
	}

	cout << "Escribiendo csvs" << endl;
	string fileName1 = "out/aleatorios/maxMinsP/dens8.csv"; 
	string fileName2 = "out/aleatorios/maxMinsP/dens5.csv"; 
	string fileName3 = "out/aleatorios/maxMinsP/dens2.csv"; 

	ofstream dens8;
	ofstream dens5;
	ofstream dens2;

	//nombreHardcodeado para no cambiar parametros de entrada
	dens8.open (fileName1);
	dens5.open (fileName2);
	dens2.open (fileName3);

	dens8 << "p,minPageRank,maxPageRank" << endl;
	dens5 << "p,minPageRank,maxPageRank" << endl;
	dens2 << "p,minPageRank,maxPageRank" << endl;


	for (int i = 0; i < 20; ++i)
	{
		promedioMaximosPorCadaPDBaja[i] = promedioMaximosPorCadaPDBaja[i]/cantIteraciones;
		promedioMinimosPorCadaPDBaja[i] = promedioMinimosPorCadaPDBaja[i]/cantIteraciones;

		dens2 << i * 0.05 << "," << promedioMinimosPorCadaPDBaja[i] << "," << promedioMaximosPorCadaPDBaja[i] << endl;

		promedioMaximosPorCadaPDMedia[i] = promedioMaximosPorCadaPDMedia[i]/cantIteraciones;
		promedioMinimosPorCadaPDMedia[i] = promedioMinimosPorCadaPDMedia[i]/cantIteraciones;

		dens5 << i * 0.05 << "," << promedioMinimosPorCadaPDMedia[i] << "," << promedioMaximosPorCadaPDMedia[i] << endl;

		promedioMaximosPorCadaPDAlta[i] = promedioMaximosPorCadaPDAlta[i]/cantIteraciones;
		promedioMinimosPorCadaPDAlta[i] = promedioMinimosPorCadaPDAlta[i]/cantIteraciones;

		dens8 << i * 0.05 << "," << promedioMinimosPorCadaPDAlta[i] << "," << promedioMaximosPorCadaPDAlta[i] << endl;
	}

	dens8.close();
	dens5.close();
	dens2.close();


	
}

int main(){
	srand(time(NULL)); // COMENTAR ESTO PARA CORRER MI TEST
	
	// testNormaAxMenosX();
	// testCambiaPrecisionSegunP();
	// testComparadorDeResultados();
	// testTiemposSegunN();
	// testNormaAxMenosXN2000();
	// testCambiaPrecisionSegunDensidad();

	// A PARTIR DE ACÁ, TODO LO QUE ESTÁ ABAJO FUE HECHO PARA REENTREGA

	// Rala W = Rala(6);
	// generarGrafoEstrella(W);
	// generarGrafoDienteDeLeon(W);
	// generarGrafoRetro1(W);
	// generarGrafoRetro2(W);
	// generarGrafoSimetrico(W);
	// generarGrafoPiramide(W);	
	// mostrarRala(W);

	pruebaMaxMinVariandoPDensidadBaja(10, 100);

	// variaPGrafoAleatorio(10);
	//variaPGrafoEstrella(10);

	return 0;
}