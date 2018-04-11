#include <iostream>
#include <fstream>
#include "matrizRala.h"




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


int generarMatrizConectividad(Rala A, int proba){
	int n = A.n;
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
					insertarElemento(A, fila, columna, 1);
					}	
				}	
			}
		}

		return 1;
	
	} else {

		return 1;
	
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


//-------------------------------------------------------------TESTS GENERALES


void TestGeneradores(int prob){
	
	Rala A = Rala(5);
	generarMatrizConectividad(A, prob);
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

	Rala C = Rala(A.n);
	cout << "MATRIZ RESULTADO VACIA: " << endl;
	mostrarRala(C);
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
	insertarElemento(B, 0, 0, 4);
	insertarElemento(B, 0, 1, 2);
	insertarElemento(B, 1, 2, 1);
	insertarElemento(B, 2, 0, 1);
	insertarElemento(B, 2, 2, 3);

	mostrarRala(A);
	mostrarRala(B);

	Rala C = Rala(A.n);
	multiplicacionMatricial(A, B, C);
	cout << endl;
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

	double valor = 2.7378;
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
	generarMatrizConectividad(W, 5);
	vector<double> res(5,0.0);
	resolverPageRank(W, res, 1.0);
	cout << endl;

	cout << "RESULTADO FINAL: " << endl;
	
	for (int i = 0; i < res.size(); ++i)
	{
		cout << res [i] << endl;
	}	
}


//-------------------------------------------------------------TESTS CÁTEDRA
// Falta medir bien tiempos para ver qué onda
// Falta comparar con un script los resultados de la cátedra vs los nuestros


// lo corrí y las primeras y últimas 100 lineas son iguales a los outs de la cátedra
// no medí tiempos, pero habrá tardado en correrse (todo entero) entre 3 y 5 minutos
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

	// corro el código que resuelve el pageRank

	double p = 0.9;
	vector<double> res(n, 0);
	resolverPageRank(W, res, p);


	// guardo en el archivo de salida los resultados
	sal << p << endl;
	for(int i = 0; i < n; i++){
		sal << res[i] << endl;
	}
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

	double p = 0.76;
	vector<double> res(n, 0);
	resolverPageRank(W, res, p);

	// guardo en el archivo de salida los resultados
	sal << p << endl;
	for(int i = 0; i < n; i++){
		sal << res[i] << endl;
	}
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
}


int main(){
	srand(time(NULL));
	// TestEcuaciones();
	TestSolveLinearEquatinos();
	//TestGeneradores(7);
	//TestEliminacionGaussiana();
	//Test1ParaSuma();
	//Test1ParaMultPorEsc();
	// TestAleatorioDesordenadoCatedra();
	// Test15SegCatedra();

	return 0;
}