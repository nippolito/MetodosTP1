#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <utility> // para pair
#include <map>
#include <cmath> 


using namespace std;

class Rala{
public:
	Rala(int size ){
		n = size;

		vector< map<int, double>* > conecciones (n, NULL);
		vector< map<int, double>* > transpuesta (n, NULL);

		for(int i = 0 ; i < n ; i ++){
			conecciones[i] = new map<int,double> ();
			transpuesta[i] = new map<int,double> ();
		}

		conex = conecciones;
		transp = transpuesta;
	}

	~Rala(){
		for(int i = 0 ; i < n ; i ++){
			delete(conex[i]);
			delete(transp[i]);
		}		
	}

	int n;
	vector< map<int, double>* > conex; // lista de adyacencia. 

	vector< map<int, double>* > transp; // transpuesta
	// uso pair ya que necesitamos saber la columna a la que corresponde cada elemento para elimGauss
	// pair.first = elemento, pair.second = columna


};


void mostrarVectorPair(map<int,double>* m){
	// cout << "longitud de vec: " << vec.size() << endl;
	cout << "[";
	for(map<int,double>::iterator it = m->begin(); it != m->end(); it++){
		map<int,double>::iterator next = it;
		next ++; 
		string comaOrEnd = next == m->end() ? "" : ", ";  
		cout << "(" << it->first << "," << it -> second << ")" << comaOrEnd;
	}
	cout << "]" << endl;
}

void mostrarRala(Rala* matriz){
	for(int link = 0; link < matriz->n; link++){
		mostrarVectorPair(matriz->conex[link]);
	}
}

void insertarElemento(Rala* A, int fila, int columna, double valor ){
	A -> conex[fila] -> insert(pair<int,double>(columna,valor));
	A -> transp[columna] -> insert(pair<int,double>(fila,valor));
}

// devuelve el grado de la página j (o sea, la cantidad de elems en la columna j, o #linksSalientes)
// es O((#conexiones)logn)
int gradoSalida(struct Rala* A, int j){
	int res = 0;
	int n = A->n;
	for(int i = 0; i < n; i++){
		if( A->conex[i] -> find(j) != A->conex[i] -> end()){
			res++;
		}
	}
	return res;
}

// suma las matrices A y B y devuelve la suma en C
void sumaMatricial(Rala* A, Rala* B, Rala* C){
	cout << "MATRIZ A: " << endl;
	mostrarRala(A);
	cout << "MATRIZ B: " << endl;
	mostrarRala(B);
	
	int n = A->n;
	for(int i = 0; i < n; i++){
		map<int, double>* filA = A->conex[i];
		map<int, double>* filB = B->conex[i];

		map<int, double>::iterator itA = filA->begin();
		map<int, double>::iterator itB = filB->begin();
		

		while(itA != filA->end() || itB != filB->end()){
			if( itB == filB->end()  ||  ((itA -> first < itB -> first) && itA != filA->end()) ){
				insertarElemento(C, i, itA->first, itA->second);
				itA ++;
			}
			else if( itA == filA->end() || ((itA -> first > itB -> first) && itB != filB->end()) ){
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

double multiplicarFilas(map<int,double>* filA, map<int,double>* colB, int n){
	double ac = 0;
	map<int, double>::iterator itA = filA->begin();
	while(itA != filA->end()){
		map<int, double>::iterator itElemB = colB->find(itA->first);
		if(itElemB != colB->end()){
			ac += itA->second * itElemB->second;
		}
		itA++;
	}
	return ac;
}

// multiplica las matrices A y B y devuelve la multiplicación en C
void multiplicacionMatricial(Rala* A, Rala* B, Rala* C){
	int n = A->n;
	for(int i = 0; i < n; i++){
		for(int j = 0 ; j < n ; j ++){
			map<int,double>* filA = A->conex[i];
			map<int,double>* colB = B->transp[j];

			double multRes = multiplicarFilas(filA, colB, n);
			if(multRes != 0 ){
				insertarElemento(C,i,j,multRes);
			}
		}
	}
}

// modifica A
void multiplicacionPorEscalar(Rala* A, double valor ){
	for (int i = 0; i < A->conex.size(); i++){
		for(map<int,double>::iterator it = (A -> conex[i])->begin(); it != (A -> conex[i])->end(); it++){
			it -> second *= valor;
		}
	}
}

int maxIndexInMapFromKey(map<int,double>* m, int fila, int n ){
	double max = 0;
	for(int i = fila, i < n; i++){
		map<int,double>::iterator it =  m->find(i);
		if( it != m->end()){
			if(abs(it->second) > abs(max)){
					max = i;			
			}
		}
	}
	if(max == 0){
		return -1;
	}
	return max;
}

int firstIndexWithValueDifferentThatZeroFrom(map<int,double>* m, int i, int n){
	for(int j = i ; j < n ; j ++){
		if(m->find(j) != m->end()) return j;
	}
	return -1;
}

void elimincacionGaussiana(Rala* A){
	for(int i = 0  ; i < A->n ; i ++){
		int filaMax = maxIndexInMapFromKey(A->transp[i], i), A->n;
		if(filaMax != -1){
//--------------------  Reacomodar posiciones.

			map<int,double> * mapTemp = A->conex[i];
			A -> conex[i] = A -> conex[filaMax];
			A -> conex[i] = mapTemp;


			map<int,double>* itI = A -> transp[i]->find(i);
			map<int,double>* itM = A -> transp[i]->find(filaMax);

			double dI = 0 ;
			double dM = 0 ;

			if(itI != A -> end()){
				dI = itI -> second;
				m->erase(itI);
			}
			if(itM != A -> end()){
				dM = itM -> second;
				m->erase(itM);
			}

			if(dI != 0 ){
				m->insert(pair<int,double>(i,dM));
			}
			if(dM != 0 ){
				m->insert(pair<int,double>(i,dI));
			}

//-------------------- 	transformar los ceros	
			int dz = firstIndexWithValueDifferentThatZeroFrom(m, i+1, A->n);
			if(dz == -1) break;
			for(map<int,double>::iterator it = m->find(dz); it != m->end(); it++){
				it->second =  
			}
			




		}
	}
}



//-------------------------------------------------------------GENERADORES

//Genera una matriz aleatoria con varios paramtros:

//proba: Es la probabilidad de agregar un elemento en cada columna. Tambien se puede ver como la DENSIDAD de una matriz.
//		 Toma valores de 0 a 10. Si es 0, sera una matriz nula, y si es 10 tendra un valor distinto a 0 para cada Aij.
//
//Fmin y Fman es el rango de valores que pueden tomar los elementos de la matriz.

int generarMatrizAleatoria(Rala* A, int proba, double fMin, double fMax){
	int n = A->n;
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


void TestGeneradores(int prob, int min, int max){
	Rala A = Rala(5);
	generarMatrizAleatoria(&A, prob, min, max);
	mostrarRala(&A);

}

void Test1ParaSuma(){ 	// pasa, todo OK
	Rala A = Rala(3);

	Rala B = Rala(3);

	insertarElemento(&A, 0, 0, 4);
	insertarElemento(&A, 0, 2, 2);
	insertarElemento(&A, 1, 2, 1);
	insertarElemento(&A, 2, 1, 2);

	insertarElemento(&B, 0, 2, 5);
	insertarElemento(&B, 1, 0, 3);
	insertarElemento(&B, 1, 1, 2);
	insertarElemento(&B, 2, 2, 3);

	Rala C = Rala(A.n);
	cout << "MATRIZ RESULTADO VACIA: " << endl;
	mostrarRala(&C);
	sumaMatricial(&A, &B, &C);
	cout << "MATRIZ FINAL:" << endl;
	mostrarRala(&C);
}

void Test1ParaMult(){ 	// pasa, todo OK
	Rala A = Rala(3);

	Rala B = Rala(3);

	insertarElemento(&A, 0, 2, 3);
	insertarElemento(&A, 1, 0, 2);
	insertarElemento(&A, 1, 1, 1);
	insertarElemento(&A, 2, 1, 2);
	insertarElemento(&B, 0, 0, 4);
	insertarElemento(&B, 0, 1, 2);
	insertarElemento(&B, 1, 2, 1);
	insertarElemento(&B, 2, 0, 1);
	insertarElemento(&B, 2, 2, 3);

	mostrarRala(&A);
	mostrarRala(&B);

	Rala C = Rala(A.n);
	multiplicacionMatricial(&A, &B, &C);
	cout << endl;
	mostrarRala(&C);	
}

void Test1ParaMultPorEsc(){ 	// pasa, todo OK
	Rala A = Rala(3);

	insertarElemento(&A, 0, 2, 3);
	insertarElemento(&A, 1, 0, 2);
	insertarElemento(&A, 1, 1, 1);
	insertarElemento(&A, 2, 1, 2);

	cout << endl;
	mostrarRala(&A);

	double valor = 2.7378;
	multiplicacionPorEscalar(&A, valor);

	cout << endl;
	mostrarRala(&A);	
}


int main(){

	srand(time(NULL));
	TestGeneradores(1, 0.0, 100.0);
	//Test1ParaSuma();
	//Test1ParaMultPorEsc();

	return 0;
}