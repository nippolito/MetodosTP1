#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <utility> // para pair
#include <unordered_map>
#include <cmath> 

using namespace std;

class Rala{
public:
	Rala(int size ){
		n = size;

		vector< unordered_map<int, double>* > conecciones (n, NULL);

		for(int i = 0 ; i < n ; i ++){
			conecciones[i] = new unordered_map<int,double> ();
		}

		conex = conecciones;
	}

	~Rala(){
		for(int i = 0 ; i < n ; i ++){
			delete(conex[i]);
		}		
	}

	int n;
	vector< unordered_map<int, double>* > conex; // lista de adyacencia. 

	vector< unordered_map<int, double>* > transp; // transpuesta
	// uso pair ya que necesitamos saber la columna a la que corresponde cada elemento para elimGauss
	// pair.first = elemento, pair.second = columna


};

// inserta elemento en la matriz A.
void insertarElemento(Rala& A, int fila, int columna, double valor ){
	
	unordered_map<int,double>::iterator it = A.conex[fila] -> find(columna);
	if( it != A.conex[fila] -> end() ){
		it -> second = valor;
	}else{
		A.conex[fila] -> insert(pair<int,double>(columna,valor));	
	}
}

// devuelve el grado de la página j (o sea, la cantidad de elems en la columna j, o #linksSalientes)
// es O((#conexiones)logn)
int gradoSalida(struct Rala& A, int j){
	int res = 0;
	int n = A.n;
	for(int i = 0; i < n; i++){
		if( A.conex[i] -> find(j) != A.conex[i] -> end()){
			res++;
		}
	}
	return res;
}


//--------------------------------------------------------FUNCIONES PARA MOSTRAR MATRICES


void mostrarVectorEnteros(vector<double> v ){
	int n  = v.size();
	cout << "[";
	for(int i =  0 ; i < n ; i ++){
		string comaOrEnd = i == n-1 ? "" : ", "; 
		cout <<  v[i] << comaOrEnd;
	}
	cout << "]" << endl;
}


void mostrarVectorPair(unordered_map<int,double>* m, int n){
	// cout << "longitud de vec: " << vec.size() << endl;
	cout << "[";
	for(int i =  0 ; i < n ; i ++){
		unordered_map<int,double>::iterator it = m -> find(i);
		string comaOrEnd = i == n-1 ? "" : ", "; 
		if(it != m->end()){
			cout <<  it -> second << comaOrEnd;
		}else{
			cout <<"0" << comaOrEnd;
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
		for(unordered_map<int,double>::iterator it = A.conex[i] -> begin() ; it != (A.conex[i]) -> end(); it ++){
			insertarElemento(At, i, it->first, it -> second);
		}
	}
}


//--------------------------------------------------------FUNCIONES PARA REALIZAR CÁLCULOS MATRICIALES


// suma las matrices A y B y devuelve la suma en C
void sumaMatricial(Rala & A, Rala & B, Rala & C){
	int n = A.n;
	for(int i = 0; i < n; i++){
		unordered_map<int, double>* filA = A.conex[i];
		unordered_map<int, double>* filB = B.conex[i];
		for(int j = 0 ; j < n; j++){
			unordered_map<int, double>::iterator itA = filA->find(j) ;
			unordered_map<int, double>::iterator itB = filB->find(j) ;

			if(itA != filA -> end() && itB == filB -> end()){
				insertarElemento(C, i, j, itA->second);
			}
			else if(itA == filA -> end() && itB != filB -> end()){
				insertarElemento(C, i, j, itB->second);
			}
			else if(itA != filA -> end() && itB != filB -> end()){
				insertarElemento(C, i, j, itA->second + itB->second);
			}
		}
	}
}

// auxiliar para la multiplicación matricial
// utilizada para obtener el elemento C_filAcolB de la multiplicación entre A y B
double multiplicarFilas(unordered_map<int,double>* fila, unordered_map<int,double>* col, int n){
	double ac = 0;
	for(int i = 0 ; i < n ; i ++){
		unordered_map<int, double>::iterator itF = fila -> find(i);
		unordered_map<int, double>::iterator itC = col  -> find(i);
		if(itF != fila -> end() && itC != col -> end()){
			ac += itF->second * itC -> second;
		} 
	}
	return ac;
}

// multiplica las matrices A y B. Devuelve la multiplicación en C
void multiplicacionMatricial(Rala& A, Rala& B, Rala& C){
	int n = A.n;
	Rala transp = Rala(n);
	createTranspose(B, transp);
	for(int i = 0; i < n; i++){
		for(int j = 0 ; j < n ; j ++){
			unordered_map<int,double>* filA = A.conex[i];
			unordered_map<int,double>* colB = transp.conex[j];

			double multRes = multiplicarFilas(filA, colB, n);
			if(multRes != 0 ){
				insertarElemento(C,i,j,multRes);
			}
		}
	}
}

// multiplica a la matriz por un vector columna (devuelve un vector columna como resultado)
vector<double> replicarMovimientosEnVector(Rala& A, vector<double>* v){
	int n = A.n ;
	vector<double> res (n);
	for(int i = 0; i < n; i ++){
		unordered_map<int,double>* row = A.conex[i];
		double ac = 0.0 ;
		for(unordered_map<int,double>::iterator it  = row->begin(); it != row->end(); it++){
			ac += (it->second) * (*v)[it->first];
		}
              (*v)[i] = ac;
  		}
	for(int i = 0 ; i < n ; i ++){
	    res[i] = (*v)[i];
    }

	return res;
}

// A = A*valor
// modifica A
void multiplicacionPorEscalar(Rala& A, double valor){
	for (int i = 0; i < A.conex.size(); i++){
		for(unordered_map<int,double>::iterator it = (A.conex[i])->begin(); it != (A.conex[i])->end(); it++){
			it -> second *= valor;
		}
	}
}

// devuelve el elemento de abs máximo que haya en la columna, considerando las filas col <= fila < n
int maxIndexInMapFromKey(Rala& r, int col, int n ){
	double maxIndex = -1;
	double maxVal = 0;
	for(int i = col; i < n; i++){
		unordered_map<int,double>::iterator it =  r.conex[i]->find(col);
		if( it != (r.conex[i])->end()){
			if(abs(it->second) > abs(maxVal)){
					maxVal = it->second;
					maxIndex = i;			
			}
		}
	}
	return maxIndex;
}

// realiza la resta de filas en eliminación gausseana (Ejemplo: F3 = F3 - 2F1)
// pasás la fila y columna donde está el pivote (fila sirve para modificar Linv)
// así como la fila entera del pivote (pivot) y la fila entera a modificar (row)
// la columna te sirve para saber dónde comenzar la resta de filas 
void reduceRowFromPivot(unordered_map<int,double>* row, unordered_map<int,double>* pivot, int fila, int col, int n, Rala & Linv){
	unordered_map<int,double>::iterator itPivot = pivot->find(col);
	unordered_map<int,double>::iterator itRow = row->find(col);
	double pivotBase = itPivot->second;
	double rowBase = itRow->second;
	double coeficiente = rowBase / pivotBase;
	//cout << "EL COEFICIENTE ES : " << coeficiente << endl;
	
	insertarElemento(Linv, fila, col, coeficiente * -1);
	
	// ciclo que realiza la resta de filas correctamente. Siempre comenzando desde la columna del pivote
	for(unordered_map<int,double>::iterator it = itPivot; it != pivot -> end(); it ++){
		itRow = row->find(it->first);
		if(itRow != row -> end()){
			if( (itRow -> second) - coeficiente * (it -> second ) != 0 ){
				itRow -> second = (itRow -> second) - coeficiente * (it -> second );
			}else{
				row->erase(itRow);
			}
		}else{
			row -> insert(pair<int,double>(it->first, -1 * (it->second) * coeficiente));
		}
	}
}

int firstIndexWithValueDifferentThatZeroFrom(unordered_map<int,double>* m, int i, int n){
	for(int j = i ; j < n ; j ++){
		if(m->find(j) != m->end()) return j;
	}
	return -1;
}


//--------------------------------------------------------ELIMINACIÓN GAUSSIANA + CÁLCULO PAGERANK

// esta es EG común (sin pivoteo)
// modifica la matriz de entrada A, y en Linv guarda lo que sería L de la fact LU (pero multiplicado por -1)
// Precondición >>>> asumimos que le pasamos matrices en las que se puede aplicar EG
// si le pasamos una matriz en la que NO se puede aplicar EG es probable que explote
// pues el pivote por ahí es cero, pero hay otro número en la columna distinto de 0, 
// por lo que maxIndexInMapFromKey no devuelve -1, y el algoritmo sigue y termina dividiendo por 0.
void eliminacionGaussiana(Rala & A, Rala & Linv){
	int n = A.n ;
	for(int i = 0  ; i < n ; i ++){
		int filaMax = maxIndexInMapFromKey(A, i, A.n); // se usa para saber si en la columna son todos ceros o no
		if(filaMax != -1){
			// transformar los ceros y hago las restas correspondientes (Ejemplo: F2 = F2 - 3F1)	
			unordered_map<int,double>* pivot = A.conex[i];

			for(int j = i+1; j < A.n ; j++){
				unordered_map<int,double> * row = A.conex[j];
				if(row -> find(i) != row->end()){
					reduceRowFromPivot(row,pivot, j, i ,n, Linv); // realiza resta de filas
				}
			}
		}else{
			cout << "Capo, esta matriz no tiene fact LU, no puedo aplicarle EG. Rescatate amigo" << endl;
			cout << "Voy a tener que tirar un break por esta flasheada" << endl;
			break;
		}
	}
}

// esta es EG con pivoteo parcial
// modifica la matriz de entrada A
// En Linv guarda lo que sería L de la fact LU (pero multiplicado por -1)
// En Permu guarda el vector de permutaciones
/*void eliminacionGaussianaPivoteoParcial(Rala & A, Rala & Linv , Rala & Permu){
	int n = A.n ;
	for(int i = 0  ; i < n ; i ++){
		int filaMax = maxIndexInMapFromKey(&A, i, A.n);
		if(filaMax != -1){
			//--------------------  Reacomodar posiciones.
			unordered_map<int,double> * unordered_mapTempA = (A.conex)[i];
			unordered_map<int,double> * unordered_mapTempL = (Linv.conex)[i];
			unordered_map<int,double> * unordered_mapTempP = (Linv.conex)[i];
			
			
			(A.conex)[i] = (A.conex)[filaMax];
			(A.conex)[filaMax] = unordered_mapTempA;

			(Linv.conex)[i] = (Linv.conex)[filaMax];
			(Linv.conex)[filaMax] = unordered_mapTempL;

			(Permu.conex)[i] = (Permu.conex)[filaMax];
			(Permu.conex)[filaMax] = unordered_mapTempL;
			//-------------------- 	transformar los ceros y hago las restas correspondientes
			unordered_map<int,double>* pivot = A.conex[i];

			for(int j = i+1; j < A.n ; j++){
				unordered_map<int,double> * row = A.conex[j];
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
	Rala L = CrearIdentidad(n);
	eliminacionGaussiana(A, L);
	replicarMovimientosEnVector(L, &conjunta);

	for(int i = n-1; i >= 0 ; i --){
		double ac = conjunta [i];
		for(int j = n-1 ; j > i ; j --){
			unordered_map<int,double>::iterator it2  = (A.conex[i]) -> find(j);
			if(it2 != (A.conex[i])->end() ){
				ac -= (res[j] * (it2 -> second));	
			}
		}
		res[i] = ac / (A.conex[i])->find(i)->second;
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
	Rala I2 = CrearIdentidad(n);
	Rala D  = Rala(n);
	Rala L = CrearIdentidad(n);
	vector<double> e = generarVectorE(n);	//Creo e 

	double prob = p * (-1); //uso -p para poder multiplicar directamente y luego usar sumaMatricial.

	generarMatrizDiagonalD(D, W); //Creo D según la matriz de conectividad W
	multiplicacionMatricial(W, D, WxDxp); // WD => WxDxp
	multiplicacionPorEscalar(WxDxp, prob); // WxDxp = -pWD

	sumaMatricial(I, WxDxp, MatrizAIgualar);		// WxDxp = (I + (-pWD))
	
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
