#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <utility> // para pair
#include <map>
#include <cmath> 

using namespace std;

double epsilon = 0.000000000001;

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


//--------------------------------------------------------FUNCIONES PARA MOSTRAR MATRICES


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
			if(abs(multRes) > 0 ){
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


// realiza la resta de filas en eliminación gausseana (Ejemplo: F3 = F3 - 2F1)
// pasás la fila y columna donde está el pivote (fila sirve para modificar Linv)
// así como la fila entera del pivote (pivot) y la fila entera a modificar (row)
// la columna te sirve para saber dónde comenzar la resta de filas 
void reduceRowFromPivot(map<int,double>& row, map<int,double>& pivot, int fila, int col, int n, vector<double> & conjunta, map<int,double>::iterator& currentItRow){
	map<int,double>::iterator itPivot = pivot.find(col); // siempre lo encuentra pues la matriz es inversible
	map<int,double>::iterator itRow = row.find(col);	// siempre lo encuentra pues el código de EG evita llamar a esta función si no lo hace
	double pivotBase = itPivot->second;
	double rowBase = itRow->second;
	double coeficiente = rowBase / pivotBase;
	
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
				if( abs((itRow -> second) - coeficiente * (itPivot -> second )) > 0  ){
					itRow -> second = (itRow -> second) - coeficiente * (itPivot -> second );

				}else{
					//currentItRow ++;
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

void reduceRowFromPivotFix(map<int,double>& row, map<int,double>& pivot, int fila, int col, int n, vector<double> & conjunta, map<int,double>::iterator& currentItRow){
	map<int,double>::iterator itPivot = pivot.find(col); // siempre lo encuentra pues la matriz es inversible
	map<int,double>::iterator itRow = currentItRow;	// siempre lo encuentra pues el código de EG evita llamar a esta función si no lo hace
	double pivotBase = itPivot->second;
	double rowBase = itRow->second;
	double coeficiente = rowBase / pivotBase;
	
	currentItRow ++;
								
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
				if( abs((itRow -> second) - coeficiente * (itPivot -> second )) > 0  ){
					itRow -> second = (itRow -> second) - coeficiente * (itPivot -> second );

				}else{
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


//--------------------------------------------------------ELIMINACIÓN GAUSSIANA + CÁLCULO PAGERANK


// esta es EG común (sin pivoteo)
// modifica la matriz de entrada A, y en Linv guarda lo que sería L de la fact LU (pero multiplicado por -1)
// Precondición >>>> asumimos que le pasamos matrices en las que se puede aplicar EG (pues por TP eso vale siempre)
void eliminacionGaussiana(Rala & A, vector<double> & conjunta){
	int n = A.n ;

	vector<map<int,double>::iterator> iteradores (n);
	for(int j = 0 ; j < n ; j ++){
		iteradores[j] = A.conex[j].begin();
	}

	for(int col = 0  ; col < n ; col ++){
		// transformar los ceros y hago las restas correspondientes (Ejemplo: F2 = F2 - 3F1)	
		map<int,double> pivot = A.conex[col];

		for(int j = col+1; j < A.n ; j++){
			cout << "Viendo de pivotear fila " << j << " con fila " <<col << endl;
			cout << iteradores[col]->first << endl;
			cout << iteradores[j]->first << endl;
				
			if(iteradores[j] != A.conex[j].end()){
				
				if(iteradores[j]->first == col ){
			
					//iteradores[j] ++;	
					reduceRowFromPivotFix(A.conex[j],pivot, j, col ,n, conjunta, iteradores[j]);
				}
			}
		}
	}
}

// esta es EG con pivot-eo parcial
// modifica la matriz de entrada A
// En Linv guarda lo que sería L de la fact LU (pero multiplicado por -1)
// En Permu guarda el vector de permutaciones
/*void eliminacionGaussianaPivoteoParcial(Rala & A, Rala & Linv , Rala & Permu){
	int n = A.n ;
	for(int i = 0  ; i < n ; i ++){
		int filaMax = maxIndexInMapFromKey(&A, i, A.n);
		if(filaMax != -1){
			//--------------------  Reacomodar posiciones.
			map<int,double> * mapTempA = (A.conex)[i];
			map<int,double> * mapTempL = (Linv.conex)[i];
			map<int,double> * mapTempP = (Linv.conex)[i];
			
			
			(A.conex)[i] = (A.conex)[filaMax];
			(A.conex)[filaMax] = mapTempA;

			(Linv.conex)[i] = (Linv.conex)[filaMax];
			(Linv.conex)[filaMax] = mapTempL;

			(Permu.conex)[i] = (Permu.conex)[filaMax];
			(Permu.conex)[filaMax] = mapTempL;
			//-------------------- 	transformar los ceros y hago las restas correspondientes
			map<int,double>* pivot = A.conex[i];

			for(int j = i+1; j < A.n ; j++){
				map<int,double> * row = A.conex[j];
				if(row -> find(i) != row->end()){
					reduceRowFromPivot(row,pivot, j, i ,n, &Linv);
				}
			}
		}
	}
	Rala C = Rala(n);

	Rala id = CrearIdentidad(n);
	sumaMatricial(Linv, id, C);
	Linv = C;
}
*/

// NO TIENE QUE TENER CEROS EN LA DIAGONAL
// Resuelve la ecuación lineal A*res = conjunta
// Devuelve el resultado en res (que vendría a ser el vector x)
void solveLinearEquations(Rala& A, vector<double> & conjunta, vector<double> & res , int n ){
	// std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
	eliminacionGaussiana(A, conjunta);
	// std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
 // 	std::chrono::duration<double, std::milli> elapsed_seconds = end - start;
	// cout << "eliminacionGaussiana toma : " << (elapsed_seconds).count() << endl;

	// start = std::chrono::system_clock::now();
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
	// end = std::chrono::system_clock::now();
 // 	elapsed_seconds = end - start;
	// cout << "Resolver el sistema toma : " << (elapsed_seconds).count() << endl;

}

// resuelve el PageRank
// entradas: matriz de conectividad W, vector res para la salida (debe tener size n)
// salida: el vector de entrada res
void resolverPageRank(Rala& W, vector<double>& res, double p){
	// std::chrono::time_point<std::chrono::system_clock> start, end;

	int n = W.n;
	// start = std::chrono::system_clock::now();
	Rala WxDxp = Rala(n);
	Rala MatrizAIgualar = Rala(n);
	Rala I = CrearIdentidad(n);
	Rala D  = Rala(n);
	vector<double> e = generarVectorE(n);	//Creo e 
	
	// end = std::chrono::system_clock::now();
	// std::chrono::duration<double, std::milli> elapsed_seconds = end - start;
	// cout << "Crear matrices toma : " << (elapsed_seconds).count() << endl;
	
	double prob = p * (-1); //uso -p para poder multiplicar directamente y luego usar sumaMatricial.

	generarMatrizDiagonalD(D, W); //Creo D según la matriz de conectividad W


	// start = std::chrono::system_clock::now();
	multiplicacionMatricial(W, D, WxDxp); // WD => WxDxp

	// end = std::chrono::system_clock::now();
	// elapsed_seconds = end - start;
	// cout << "Multiplicar matrices toma : " << (elapsed_seconds).count() << endl;

	// start = std::chrono::system_clock::now();
	multiplicacionPorEscalar(WxDxp, prob); // WxDxp = -pWD

	// end = std::chrono::system_clock::now();
	// elapsed_seconds = end - start;
	// cout << "Multiplicar matriz por escalar toma : " << (elapsed_seconds).count() << endl;

	// start = std::chrono::system_clock::now();
	sumaMatricial(I, WxDxp, MatrizAIgualar);		// WxDxp = (I + (-pWD))
	
	// end = std::chrono::system_clock::now();
	// elapsed_seconds = end - start;
	// cout << "Sumar matrices toma : " << (elapsed_seconds).count() << endl;

	// en este punto ya tengo calculado el (I - pWD) en la matriz rala WxDxp
	

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
