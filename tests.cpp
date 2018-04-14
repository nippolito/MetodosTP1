#include <iostream>
#include <fstream>
#include <chrono>
#include "matrizRala.h"

int no = 0;
int si = 0;

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
			si++;
			for (int j = 0; j < diagonal.n; j++)
			{
				insertarElemento(ez, j, i, (1-p)/diagonal.n);

			}
		}
		else
		{
			no++;
			for (int j = 0; j < diagonal.n; j++)
			{
				insertarElemento(ez, j, i, 1.0/diagonal.n);
			}
		}
	}
	cout << "no: ";
	cout << no << endl;
	cout << "si: ";
	cout << si << endl;	
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

void testNipoParaPeter(){
	Rala W = Rala(5);
	generarMatrizConectividad(&W, 2);
	Rala Diagonal = Rala(5);
	generarMatrizDiagonalD(Diagonal, W);

	cout << "W es" << endl;
	mostrarRala(W);
	cout << "Diagonal es" << endl;
	mostrarRala(Diagonal);

	Rala ez = Rala(5);
	crearMatrizEZ(Diagonal, ez, 0.4);

	cout << "EZ es" << endl;
	mostrarRala(ez);
}


//-------------------------------------------------------------TESTS GENERALES


void testPeter(){
	int n = 3;
	Rala A = Rala(n);
	
	/*
	2 7 9 = 1   0   0     2   7    9
	1 3 6 = 1/2 1   0  *  0  -1/2  3/2
	4 5 8 = 2   18  0     0   0    -37
	*/
	insertarElemento(A, 0, 0, 2);
	insertarElemento(A, 0, 1, 7);
	insertarElemento(A, 0, 2, 9);

	insertarElemento(A, 1, 0, 1);
	insertarElemento(A, 1, 1, 3);
	insertarElemento(A, 1, 2, 6);

	insertarElemento(A, 2, 0, 4);
	insertarElemento(A, 2, 1, 5);
	insertarElemento(A, 2, 2, 8);
	vector<double> vect(n, 0);
	eliminacionGaussiana(A, vect);
	mostrarRala(A);
}

void testNipo(){ // OK
	int n = 3;
	Rala A = Rala(n);

	insertarElemento(A, 0, 0, 4);
	insertarElemento(A, 0, 1, 2);
	insertarElemento(A, 0, 2, 4);

	insertarElemento(A, 1, 0, 2);
	insertarElemento(A, 1, 1, 4);
	insertarElemento(A, 1, 2, 2);

	insertarElemento(A, 2, 1, 1);
	insertarElemento(A, 2, 2, 7);	

	vector<double> vect(n, 0);
	eliminacionGaussiana(A, vect);
	mostrarRala(A);
}

void TestGeneradores(int prob){
	
	Rala A = Rala(5);
	generarMatrizConectividad(&A, prob);
	mostrarRala(A);
}

void Test1ParaSuma(){ 	// pasa, todo OK
	Rala A = Rala(3);

	Rala B = Rala(3);

	insertarElemento(A, 0, 0, 4);
	insertarElemento(A, 0, 2, 2);
	insertarElemento(A, 1, 2, 1);
	insertarElemento(A, 2, 1, 2);

	insertarElemento(B, 0, 2, 5);
	insertarElemento(B, 1, 0, 3);
	insertarElemento(B, 1, 1, 2);
	insertarElemento(B, 2, 2, 3);

	cout << "Matriz A" << endl;
	mostrarRala(A);

	cout << "Matriz B" << endl;
	mostrarRala(B);

	Rala C = Rala(A.n);
	sumaMatricial(A, B, C);
	cout << "MATRIZ FINAL:" << endl;
	mostrarRala(C);
}

void Test1ParaMult(){ 	// pasa, todo OK
	Rala A = Rala(3);

	Rala B = Rala(3);

	insertarElemento(A, 0, 2, 3);
	insertarElemento(A, 1, 0, 2);
	insertarElemento(A, 1, 1, 1);
	insertarElemento(A, 2, 1, 2);
	insertarElemento(B, 0, 0, 1);
	insertarElemento(B, 0, 1, 2);
	insertarElemento(B, 1, 2, 1);
	insertarElemento(B, 2, 0, 1);
	insertarElemento(B, 2, 2, 3);

	cout << "Matriz A" << endl;
	mostrarRala(A);
	cout << "Matriz B" << endl;
	mostrarRala(B);

	Rala C = Rala(A.n);
	multiplicacionMatricial(A, B, C);
	cout << endl;
	cout << "MATRIZ FINAL:" << endl;
	mostrarRala(C);	
}

void Test1ParaMultPorEsc(){ 	// pasa, todo OK
	Rala A = Rala(3);

	insertarElemento(A, 0, 2, 3);
	insertarElemento(A, 1, 0, 2);
	insertarElemento(A, 1, 1, 1);
	insertarElemento(A, 2, 1, 2);

	cout << endl;
	mostrarRala(A);

	double valor = 3;
	multiplicacionPorEscalar(A, valor);

	cout << endl;
	mostrarRala(A);	
}

void TestEliminacionGaussiana(){
	Rala A = Rala(3);
	Rala Linv = Rala(3);
	Rala P = CrearIdentidad(3);
	/*
	5 0 4
	3 4 4
	0 2 4

	*/

	insertarElemento(A, 0, 0, 5);
	insertarElemento(A, 0, 2, 4);


	insertarElemento(A, 1, 0, 3);
	insertarElemento(A, 1, 1, 4);
	insertarElemento(A, 1, 2, 4);

	insertarElemento(A, 2, 1, 2);
	insertarElemento(A, 2, 2, 4);

	vector<double> b;
	b.push_back(17);
	b.push_back(23);
	b.push_back(16);



	cout << "Matriz A original:" << endl;
	mostrarRala(A);

	eliminacionGaussiana(A, b);
	cout<< "\nMatriz A con EG: " << endl;
	mostrarRala(A);
}

void TestSolveLinearEquatinos(){
	//System  => A . x = b 
	int n = 3;

	Rala A = Rala(n);
	Rala Linv = CrearIdentidad(n);

	/*
	(x1,x2,3) = (1,2,3)
	5 0 4 = 17
	3 4 4 = 23
	0 2 4 = 16
	*/
	insertarElemento(A, 0, 0, 5);
	insertarElemento(A, 0, 2, 4);

	insertarElemento(A, 1, 0, 3);
	insertarElemento(A, 1, 1, 4);
	insertarElemento(A, 1, 2, 4);

	insertarElemento(A, 2, 1, 2);
	insertarElemento(A, 2, 2, 4);


	vector<double> b;
	b.push_back(17);
	b.push_back(23);
	b.push_back(16);

	
	vector<double> res (n); 

	solveLinearEquations(A, b,res,n);

	cout << "\nVector Respuesta: "<< endl;
	mostrarVectorEnteros(res);
}

void TestEcuaciones(){
	Rala W = Rala(5);
	generarMatrizConectividad(&W, 5);
	vector<double> res(5,0.0);
	resolverPageRank(W, res, 1.0);
	cout << endl;

	cout << "RESULTADO FINAL: " << endl;
	
	for (int i = 0; i < res.size(); ++i)
	{
		cout << res [i] << endl;
	}	
}

// Tiempos según p? Resultados según p (bajo el mismo grafo)?
// Tiempos según la granularidad de la matriz (i.e la cantidad de ceros)?
// Resultados en casos específicos?
// Cuánto explota nuestra struct el hecho de que sean ralas en casos de matrices aleatorias edd?
// 		> Podemos ver cuántos ceros se encuentra en cada columna al hacer EG
// 		> Podemos ver cuántos ceros se encuentra al iterar principalmente por el pivote y no por la fila en particular (en la resta de filas) 


//-------------------------------------------------------------TESTS CÁTEDRA


void Test15SegCatedra(){
	// levanto los archivos
	fstream ent ("Enunciado/tests_tp1/test_15_segundos.txt", ios::in);
	fstream sal ("salida_test_15_seg.txt", ios::out);

	//creo la matriz rala
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

	// corro el código que resuelve el pageRank + mido tiempos

	std::chrono::time_point<std::chrono::system_clock> start, end;

	double p = 0.9;
	vector<double> res(n, 0);
	start = std::chrono::system_clock::now();
	resolverPageRank(W, res, p);
	end = std::chrono::system_clock::now();

	std::chrono::duration<double, std::milli> elapsed_seconds = end-start;

	cout << "el tiempo del de 15 segs fue: ";
	cout << elapsed_seconds.count() << endl;


	// guardo en el archivo de salida los resultados
	sal << p << endl;
	for(int i = 0; i < n; i++){
		sal << res[i] << endl;
	}

	ent.close();
	sal.close();
}

// no lo corrí ni terminé el código
void Test30SegCatedra(){
	// levanto los archivos
	fstream ent ("Enunciado/tests_tp1/test_30_segundos.txt", ios::in);
	fstream sal ("salida_test_30_seg.txt", ios::out);

	//creo la matriz Rala
	int n;
	int m;
	ent >> n;
	ent >> m;
	Rala W = Rala(n);

	// lleno la matriz Rala W con todas las conexiones, , o sea la matriz de conectividad
	for(int i = 0; i < m; i++){
		int source;
		int dest;
		ent >> source;
		ent >> dest;
		insertarElemento(W, dest - 1, source - 1, 1);
	}

	// corro el código que resuelve el pageRank

	std::chrono::time_point<std::chrono::system_clock> start, end;

	double p = 0.8;
	vector<double> res(n, 0);
	start = std::chrono::system_clock::now();
	resolverPageRank(W, res, p);
	end = std::chrono::system_clock::now();

	std::chrono::duration<double, std::milli> elapsed_seconds = end-start;

	cout << "el tiempo del de 30 segs fue: ";
	cout << elapsed_seconds.count() << endl;


	// guardo en el archivo de salida los resultados
	sal << p << endl;
	for(int i = 0; i < n; i++){
		sal << res[i] << endl;
	}

	ent.close();
	sal.close();
}

// no lo corrí ni terminé el código
void TestAleatorioCatedra(){
	// levanto los archivos
	fstream ent ("Enunciado/tests_tp1/test_aleatorio.txt", ios::in);
	fstream sal ("salida_test_aleatorio.txt", ios::out);

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

	// corro el código que resuelve el pageRank

	std::chrono::time_point<std::chrono::system_clock> start, end;

	double p = 0.85;
	vector<double> res(n, 0);
	start = std::chrono::system_clock::now();
	resolverPageRank(W, res, p);
	end = std::chrono::system_clock::now();

	std::chrono::duration<double, std::milli> elapsed_seconds = end-start;

	cout << "el tiempo del test aleatorio fue: ";
	cout << elapsed_seconds.count() << endl;


	// guardo en el archivo de salida los resultados
	sal << p << endl;
	for(int i = 0; i < n; i++){
		sal << res[i] << endl;
	}

	ent.close();
	sal.close();
}

// este lo corrí y devuelve todo igual
void TestAleatorioDesordenadoCatedra(){
	// levanto los archivos
	fstream ent ("Enunciado/tests_tp1/test_aleatorio_desordenado.txt", ios::in);
	fstream sal ("salida_test_aleatorio_desordenado.txt", ios::out);

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

	// corro el código que resuelve el pageRank

	std::chrono::time_point<std::chrono::system_clock> start, end;

	double p = 0.76;
	vector<double> res(n, 0);
	start = std::chrono::system_clock::now();
	resolverPageRank(W, res, p);
	end = std::chrono::system_clock::now();

	std::chrono::duration<double, std::milli> elapsed_seconds = end-start;

	cout << "el tiempo del test aleatorio_desordenado fue: ";
	cout << elapsed_seconds.count() << endl;


	// guardo en el archivo de salida los resultados
	sal << p << endl;
	for(int i = 0; i < n; i++){
		sal << res[i] << endl;
	}

	ent.close();
	sal.close();
}

// no lo corrí ni terminé el código
void TestCompleto(){
	// levanto los archivos
	fstream ent ("Enunciado/tests_tp1/test_completo.txt", ios::in);
	fstream sal ("salida_test_completo.txt", ios::out);

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

	// corro el código que resuelve el pageRank

	std::chrono::time_point<std::chrono::system_clock> start, end;

	double p = 0.5;
	vector<double> res(n, 0);
	start = std::chrono::system_clock::now();
	resolverPageRank(W, res, p);
	end = std::chrono::system_clock::now();

	std::chrono::duration<double, std::milli> elapsed_seconds = end-start;

	cout << "el tiempo del test completo fue: ";
	cout << elapsed_seconds.count() << endl;


	// guardo en el archivo de salida los resultados
	sal << p << endl;
	for(int i = 0; i < n; i++){
		sal << res[i] << endl;
	}

	ent.close();
	sal.close();
}

// no lo corrí ni terminé el código
void TestSinLinksCatedra(){
	// levanto los archivos
	fstream ent ("Enunciado/tests_tp1/test_sin_links.txt", ios::in);
	fstream sal ("salida_test_sin_links.txt", ios::out);

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

	// corro el código que resuelve el pageRank

	std::chrono::time_point<std::chrono::system_clock> start, end;

	double p = 0.64;
	vector<double> res(n, 0);
	start = std::chrono::system_clock::now();
	resolverPageRank(W, res, p);
	end = std::chrono::system_clock::now();

	std::chrono::duration<double, std::milli> elapsed_seconds = end-start;

	cout << "el tiempo del test sin links fue: ";
	cout << elapsed_seconds.count() << endl;


	// guardo en el archivo de salida los resultados
	sal << p << endl;
	for(int i = 0; i < n; i++){
		sal << res[i] << endl;
	}

	ent.close();
	sal.close();
}

// no lo corrí ni terminé el código
void TestTrivialCatedra(){
	// levanto los archivos
	fstream ent ("Enunciado/tests_tp1/test_trivial.txt", ios::in);
	fstream sal ("salida_test_trivial.txt", ios::out);

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

	// corro el código que resuelve el pageRank

	std::chrono::time_point<std::chrono::system_clock> start, end;

	double p = 0.3;
	vector<double> res(n, 0);
	start = std::chrono::system_clock::now();
	resolverPageRank(W, res, p);
	end = std::chrono::system_clock::now();

	std::chrono::duration<double, std::milli> elapsed_seconds = end-start;

	cout << "el tiempo del test trivial fue: ";
	cout << elapsed_seconds.count() << endl;


	// guardo en el archivo de salida los resultados
	sal << p << endl;
	for(int i = 0; i < n; i++){
		sal << res[i] << endl;
	}

	ent.close();
	sal.close();
}

void compararResultados15Segs(){
	double errorFinal;
	fstream catedra ("Enunciado/tests_tp1/test_15_segundos.txt.out", ios::in);
	fstream nuestro ("salida_test_15_seg.txt", ios::in);
	fstream errores ("error_test_15_seg.txt", ios::out);

	int p;
	catedra >> p;
	nuestro >> p;
	for(int i = 0; i < 2001; i++){
		double resICatedra;
		double resINuestro;
		catedra >> resICatedra;
		nuestro >> resINuestro;
		double errorI = fabs(resICatedra - resINuestro);
		errorFinal += errorI;
		errores << errorI << endl;
	}

	errorFinal = errorFinal / 2000;
	cout << "nuestro promedio de error en el de 15 segundos es es: ";
	cout << errorFinal << endl;

	catedra.close();
	nuestro.close();
	errores.close();
}

void compararResultados30Segs(){
	double errorFinal;
	fstream catedra ("Enunciado/tests_tp1/test_30_segundos.txt.out", ios::in);
	fstream nuestro ("salida_test_30_seg.txt", ios::in);
	fstream errores ("error_test_30_seg.txt", ios::out);

	int p;
	catedra >> p;
	nuestro >> p;
	for(int i = 0; i < 3000; i++){
		double resICatedra;
		double resINuestro;
		catedra >> resICatedra;
		nuestro >> resINuestro;
		double errorI = fabs(resICatedra - resINuestro);
		errorFinal += errorI;
		errores << errorI << endl;
	}

	errorFinal = errorFinal / 3000;
	cout << "nuestro promedio de error en el de 30 segundos es: ";
	cout << errorFinal << endl;

	catedra.close();
	nuestro.close();
	errores.close();
}

void TestDensidadTiempos(){
	// levanto los archivos
	//fstream ent ("Enunciado/tests_tp1/test_completo.txt", ios::in);
	fstream salidaDensTiempo ("salida_test_densidad_tiempo.csv", ios::out);
	

	for (int i = 0; i < 10; i++){
		
		for(int cantRepeticiones = 0 ; cantRepeticiones < 1 ; cantRepeticiones++){ 

		/*Cant repeticiones??*/
			if(cantRepeticiones == 0){
				cout<< "RALA CON DENSIDAD : " <<  i <<endl;
			}
	
		Rala W = Rala(500); //2500 paginas para todos los casos (el test de 15 tenia 2000 y el de 30 3000).
		generarMatrizConectividad(&W, i);

		
		double p = 0.5; // !?!?!? P FIJO, PERO CUAL?

		vector<double> res(500, 0);
		std::chrono::time_point<std::chrono::system_clock> start, end;

		start = std::chrono::system_clock::now();
		resolverPageRank(W, res, p);
		end = std::chrono::system_clock::now();

		std::chrono::duration<double, std::milli> elapsed_seconds = end-start;

		salidaDensTiempo << i << "," << elapsed_seconds.count() << endl;
		

		}
	
	}
	
	salidaDensTiempo.close();
}

//resta dos vectores y los normaliza en funcion a norma 1
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

// dejo fijo densidad y p. Varío n desde 10 hasta 1000 (aumentando de a 10) con 10 iteraciones en cada uno.
// Testeo |Ax-x| en cada uno y de paso printeo el tiempo ya que estamos (el que tarda en resolver el pageRank)
void testNormaAxMenosX(){
	srand(500); 	// semilla 500 porque pinta
	fstream sal("test_NormaAxMenosX.csv", ios::out);
	int hasta = 1000;
	int densidad = 5;
	double p = 0.5; // densidad matrices

	sal << "n,normaAxMenosX,TiempoPR" << endl;

	for(int i = 10; i < hasta + 1; i = i + 10){
		cout << "voy por n: ";
		cout << i << endl;
		for(int j = 0; j < 10; j++){
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
			sal << elapsed_seconds.count() << endl;
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
	// Test1ParaSuma();
	// Test1ParaMult();
	// Test1ParaMultPorEsc();
	// TestEcuaciones();
	// testPeter();
	// testNipo();
	// TestSolveLinearEquatinos();
	//TestGeneradores(7);
	//TestEliminacionGaussiana();
	
	
	// TestAleatorioDesordenadoCatedra();
	// Test15SegCatedra();
	// compararResultados15Segs();
	
	// Test30SegCatedra();
	// compararResultados30Segs();
	// TestAleatorioCatedra();

	// TestAleatorioDesordenadoCatedra();
	// TestCompleto();
	// TestSinLinksCatedra();
	// TestTrivialCatedra();

	// testComparadorDeResultados();
	// testNormaAxMenosX();
	// testCambiaPrecisionSegunP();
	testNipoParaPeter();

	return 0;
}