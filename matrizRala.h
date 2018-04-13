#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <utility> // para pair
#include <map>
#include <cmath>
#include <stdio.h> 
#include <float.h>

using namespace std;

class Rala{
public:
	Rala(int size ){
		n = size;

		for(int i = 0 ; i < n ; i ++){
			conex.push_back(map<int,double> ());
		}
	}

	int n;
	vector< map<int, double> > conex; // lista de adyacencia. 
	// uso pair ya que necesitamos saber la columna a la que corresponde cada elemento para elimGauss
	// pair.first = elemento, pair.second = columna

};

// inserta elemento en la matriz A.
void insertarElemento(Rala& A, int fila, int columna, double valor ){
	map<int,double>::iterator it = A.conex[fila].find(columna);
	if( it == A.conex[fila].end() ){
		A.conex[fila].insert(pair<int,double>(columna,valor));			
	}else{
		it -> second = valor;
	}
}

// devuelve el grado de la página j (o sea, la cantidad de elems en la columna j, o #linksSalientes)
// es O((#conexiones)logn)
int gradoSalida(struct Rala& A, int j){
	int res = 0;
	int n = A.n;
	for(int i = 0; i < n; i++){
		if( A.conex[i].find(j) != A.conex[i].end()){
			res++;
		}
	}
	return res;
}

bool AlmostEqualRelative(double A, double B, double maxRelDiff)
{
    // Calculate the difference.
    double diff = fabs(A - B);
    A = fabs(A);
    B = fabs(B);
    // Find the largest
    float largest = (B > A) ? B : A;
 
    if (diff <= largest * maxRelDiff)
        return true;
    return false;
}


//--------------------------------------------------------FUNCIONES PARA MOSTRAR MATRICES

void mostrarVector(vector<double>& vec){
	cout << "[ ";
	for (int i = 0; i < vec.size()-1; ++i)
	{
		cout << vec[i] << ", ";
	}
	cout << vec[vec.size()-1] <<" ]" << endl;
}

void mostrarVectorEnteros(vector<double>& v ){
	int n  = v.size();
	cout << "[";
	for(int i =  0 ; i < n ; i ++){
		string comaOrEnd = i == n-1 ? "" : ", "; 
		cout <<  v[i] << comaOrEnd;
	}
	cout << "]" << endl;
}


void mostrarVectorPair(map<int,double>& m, int n){
	// cout << "longitud de vec: " << vec.size() << endl;
	cout << "[";
	for(int i =  0 ; i < n ; i ++){
		map<int,double>::iterator it = m.find(i);
		string comaOrEnd = i == n-1 ? "" : ", "; 
		if(it != m.end()){
			cout <<  it -> second << comaOrEnd;
		}else{
			cout << "0" << comaOrEnd;
		}
	}
	cout << "]" << endl;
}

void mostrarRala(Rala & matriz){
	for(int link = 0; link < matriz.n; link++){
		mostrarVectorPair(matriz.conex[link], matriz.n);
	}
}


//--------------------------------------------------------GENERADORES DE MATRICES P/CÁLCULO PAGERANK 


// A tiene que ser matriz nula. W matriz de conectividad.
// Devuelve la doagonal en A
int generarMatrizDiagonalD(Rala & A, Rala & W){
	if (A.n != W.n){ return -1;}
	int n = W.n;

		for (int fila = 0; fila < n; ++fila)
		{
			int grado = gradoSalida(W, fila);
			if(grado != 0){
				
				double valor = 1.0/grado;
				insertarElemento(A, fila, fila, valor);
			}
			
		}

		return 1;	
}

// genera el vector e que es necesario para el cálculo del pageRank
vector<double> generarVectorE(int n){
	vector <double> result(n, 1.0);
	return result;
}

Rala CrearIdentidad(int n ){
	Rala res = Rala(n);
	for(int i = 0 ; i < n; i++){
		insertarElemento(res, i, i, 1 );
	}
	return res;
}

// crea en At la matriz transpuesta de A
void createTranspose(Rala& A, Rala& At){
	int n = A.n;
	for(int i = 0; i < n ; i ++){
		for(map<int,double>::iterator it = A.conex[i].begin() ; it != (A.conex[i]).end(); it ++){
			insertarElemento(At, it->first, i, it -> second);
		}
	}
}


//--------------------------------------------------------FUNCIONES PARA REALIZAR CÁLCULOS MATRICIALES


// suma las matrices A y B y devuelve la suma en C
//Complejidad : O(n^2)
void sumaMatricial(Rala& A, Rala& B, Rala& C){ 
	int n = A.n;
	for(int i = 0; i < n; i++){
		map<int, double> filA = A.conex[i];
		map<int, double> filB = B.conex[i];

		map<int, double>::iterator itA = filA.begin();
		map<int, double>::iterator itB = filB.begin();
		

		while(itA != filA.end() || itB != filB.end()){
			if( itB == filB.end()  ||  ((itA -> first < itB -> first) && itA != filA.end()) ){
				insertarElemento(C, i, itA->first, itA->second);
				itA ++;
			}
			else if( itA == filA.end() || ((itA -> first > itB -> first) && itB != filB.end()) ){
				insertarElemento(C, i, itB->first, itB->second);
				itB ++;
			}
			else{
				insertarElemento(C, i, itA->first, itA->second + itB -> second);
				itA ++;
				itB ++;
			}
		}
	}
}

// auxiliar para la multiplicación matricial
// utilizada para obtener el elemento C_filAcolB de la multiplicación entre A y B
// Complejidad  O(n )
double multiplicarFilas(map<int,double>& fila, map<int,double>& col, int n){
	double ac = 0;
	map<int, double>::iterator itF = fila.begin();
	map<int, double>::iterator itC = col.begin();

	while(itF != fila.end() && itC != col.end()){
		if(itF -> first < itC ->first) itF++;
		else if(itC -> first < itF->first) itC ++;
		else{
			ac += itF->second * itC -> second;
			itC++;
			itF++;
		}
	}

	return ac;
}

// multiplica las matrices A y B. Devuelve la multiplicación en C
// COMPLEJIDAD O(n^3)
void multiplicacionMatricial(Rala& A, Rala& B, Rala& C){
	int n = A.n;
	Rala transp = Rala(n);
	createTranspose(B, transp);
	for(int i = 0; i < n; i++){
		for(int j = 0 ; j < n ; j ++){
			map<int,double> filA = A.conex[i];
			map<int,double> colB = transp.conex[j];
			double multRes = multiplicarFilas(filA, colB, n);
			if(fabs(multRes) > 0){
				insertarElemento(C,i,j,multRes);
			}
		}
	}
}

// A = A*valor
// modifica A
void multiplicacionPorEscalar(Rala& A, double valor){
	for (int i = 0; i < A.conex.size(); i++){
		for(map<int,double>::iterator it = (A.conex[i]).begin(); it != (A.conex[i]).end(); it++){
			it -> second *= valor;
		}
	}
}

// Ax = Y
// Modifica vector res con el resultado de Ax
void multiplicacionPorVector(Rala& A, vector<double>& vecArg, vector<double>& vecRes){
	for (int i = 0; i < A.n; i++){
		map<int,double>::iterator itRow = A.conex[i].begin();
		double sumaFilaColumna = 0.0;
		for (; itRow != A.conex[i].end() ; itRow++)
		{
			sumaFilaColumna += itRow->second * vecArg[itRow->first];
		}
		vecRes[i] = sumaFilaColumna;
	}
}




void reduceRowFromPivotFix(map<int,double>& row, map<int,double>& pivot, int fila, int col, int n, vector<double> & conjunta, int & entra, int & noentra){
	map<int,double>::iterator itPivot = pivot.find(col); // siempre lo encuentra pues la matriz es inversible
	map<int,double>::iterator itRow = row.find(col);	// siempre lo encuentra pues el código de EG evita llamar a esta función si no lo hace
	double pivotBase = itPivot->second;
	double rowBase = itRow->second;
	double coeficiente = rowBase / pivotBase;

	// forzar la primer conversion a 0, pues por errores de precisión
	// puede dar true el (abs((itRow -> second) - coeficiente * (itPivot -> second )) > 0)
	// haciendo que no se borre el elemento que sí o sí hay que triangular

	map<int,double>::iterator itAux = itRow;
	itAux ++;
	row.erase(itRow->first);
	itRow = itAux;
	// if(col == 1994){
	// 	cout << "quepasaaa5" << endl;
	// 	cout << itPivot->second << endl;
	// }
	itPivot ++;
	
	// if(col == 1994){
	// 	cout << "quepasaaa3" << endl;
	// }
								
	conjunta[fila] -=  conjunta[col] * coeficiente; 

	while(itPivot != pivot.end() ){
		// recorro la fila del pivote pues si hay un cero en una columna, no hay que hacer nada
		if( itRow == row.end()){
			// si llegaste al final de la fila que estabas modificando, entonces hubo varios ceros antes
			// que ahora vas a tener que modificar
			for(;itPivot != pivot.end(); itPivot ++){
				row.insert(pair<int,double>(itPivot->first, -1 * (itPivot->second) * coeficiente));
			}
		}
		else{
			if(itRow -> first == itPivot -> first){
				// si están ambos en la misma col, realizo los cálculos
				// (en que va a ser modificado puede volverse cero, en ese caso lo borro)
				std::map<int, double>::iterator itAux = itRow;
				itAux ++;
				if(fabs((itRow -> second) - coeficiente * (itPivot -> second)) > DBL_EPSILON){
					// si los elems son distintos
					noentra++;
					itRow -> second = (itRow -> second) - coeficiente * (itPivot -> second );

				}else{
					// si los elems son iguales
					entra++;
					row.erase(itRow);
				}
				itPivot ++;
				itRow = itAux; 
			}
			else if (itRow -> first > itPivot -> first) {
				row.insert(pair<int,double>(itPivot->first, -1 * (itPivot->second) * coeficiente));
				itPivot ++;
			}
			else if (itRow -> first < itPivot -> first) {
				itRow ++;
			}
		}
	}
}

//--------------------------------------------------------TIPOS DE COMPARACIÓN DOUBLES

// fabs((itRow -> second) - coeficiente * (itPivot -> second)) < DBL_EPSILON (si es true entonces son iguales)
	// > error de 0.0005 con -O3
	// > tiempo de 20 segs con -O3
	// > entra: 30675036 vs noentra: 174272.
// fabs((itRow -> second) - coeficiente * (itPivot -> second)) > 0 ----> evalua siempre a true, o sea siempre a distintos
// itRow->second == coeficiente * itPivot->second ----> se traba en la columna 1994
// fabs((itRow -> second) - coeficiente * (itPivot -> second)) < FLT_EPSILON ---> se traba en la columna 1998
// AlmostEqualRelative(itRow->second, coeficiente * itPivot->second, FLT_EPSILON)
	// > error de 0.0005 con -O3
	// > tiempo de 21 segs con -O3
	// > entra: 31797450 vs noentra: 465663
// AlmostEqualRelative(itRow->second, coeficiente * itPivot->second, DLT_EPSILON) --> se traba en col 1998

//--------------------------------------------------------ELIMINACIÓN GAUSSIANA + CÁLCULO PAGERANK


// esta es EG común (sin pivoteo)
// modifica la matriz de entrada A, y en Linv guarda lo que sería L de la fact LU (pero multiplicado por -1)
// Precondición >>>> asumimos que le pasamos matrices en las que se puede aplicar EG (pues por TP eso vale siempre)
void eliminacionGaussiana(Rala & A, vector<double> & conjunta){
	int n = A.n ;
	int entra = 0;
	int noentra = 0;
	for(int col = 0  ; col < n ; col ++){
		// transformar los ceros y hago las restas correspondientes (Ejemplo: F2 = F2 - 3F1)	
		map<int,double> pivot = A.conex[col];
		cout << "va por la col: ";
		cout << col << endl;

		for(int j = col+1; j < A.n ; j++){
			if(A.conex[j].begin() -> first == col ){
				reduceRowFromPivotFix(A.conex[j],pivot, j, col ,n, conjunta, entra, noentra);
			}
		}
	}
	cout << "entra: ";
	cout << entra << endl;
	cout << "noentra: ";
	cout << noentra << endl;
}

// NO TIENE QUE TENER CEROS EN LA DIAGONAL
// Resuelve la ecuación lineal A*res = conjunta
// Devuelve el resultado en res (que vendría a ser el vector x)
void solveLinearEquations(Rala& A, vector<double> & conjunta, vector<double> & res , int n ){
	eliminacionGaussiana(A, conjunta);
	
	for(int i = n-1; i >= 0 ; i --){
		//double ac = conjunta [i];
		double ac = 0 ;
		for(int j = n-1 ; j > i ; j --){
			map<int,double>::iterator it2  = (A.conex[i]).find(j);
			if(it2 != (A.conex[i]).end() ){
				ac += (res[j] * (it2 -> second));	
			}
		}
		res[i] = (conjunta[i] - ac )/ (A.conex[i]).find(i)->second;
	} 
}

// resuelve el PageRank
// entradas: matriz de conectividad W, vector res para la salida (debe tener size n)
// salida: el vector de entrada res
void resolverPageRank(Rala& W, vector<double>& res, double p){
	int n = W.n;
	Rala WxDxp = Rala(n);
	Rala MatrizAIgualar = Rala(n);
	Rala I = CrearIdentidad(n);
	Rala D  = Rala(n);
	vector<double> e = generarVectorE(n);	//Creo e 
	
	double prob = p * (-1); //uso -p para poder multiplicar directamente y luego usar sumaMatricial.

	generarMatrizDiagonalD(D, W); //Creo D según la matriz de conectividad W


	multiplicacionMatricial(W, D, WxDxp); // WD => WxDxp
	multiplicacionPorEscalar(WxDxp, prob); // WxDxp = -pWD

	sumaMatricial(I, WxDxp, MatrizAIgualar);		// WxDxp = (I + (-pWD))

	solveLinearEquations(MatrizAIgualar, e, res, n);	//Resuelvo ecuacion y devuelvo resultado en res pasado por parametro.


	// Ahora tengo que normalizar, calculando la normaUno y luego dividiendo a todos por ella
	double normaUno = 0;
	for(int i = 0; i < n; i++){
		normaUno = normaUno + res[i]; 
	}

	for(int i = 0; i < n; i++){
		res[i] = res[i] / normaUno;
	}


	// MUY IMPORTANTE!!!!! ACÁ NOS FALTA NORMALIZAR EL VECTOR RES	
}

