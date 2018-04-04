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
		
		while(itA != filA->end() && itB != filB->end()){
			if(itB == filB->end() ||  itA -> first < itB -> first){
				insertarElemento(C, i, itA->first, itA->second);
				itA ++;
			}
			else if( itA == filA->end() || itA -> first > itB -> first){
				insertarElemento(C, i, itB->first, itB->second);
				itB ++;
			}
			else{
				insertarElemento(C, i, itA->first, itB->second + itB -> second);
				itA ++;
				itB ++;
			}
		}
	}
}

double multiplicarFilas(map<int,double>* filA, map<int,double>* colB, int n){
	double ac = 0;
	for(int i = 0 ; i < n ; i ++){
		map<int,double>::iterator itA = filA->find(i);
		map<int,double>::iterator itB = colB->find(i);
		if( itA != filA->end() && itB != colB->end()){
			ac += itA->second * itB->second;
		}
	}
	return ac;
}

// ojo que la dejé llena de couts, no estoy seguro si es el código ya que el buscarColumnaEnFila está fallando
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
	sumaMatricial(&A, &B, &C);
	mostrarRala(&C);
}

void Test1ParaMult(){
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


int main(){
	// Test1ParaSuma();
	// vector<vector<pair<double, int> > > matA = (&A)->conex;

	// Rala* B;
	// crearRala(&B, 3);
	// vector<vector<pair<double, int> > > matB = B->conex;

	// Test1ParaMult();
	vector<pair<double, int> > p;
	p.push_back(make_pair(1,0));
	p.push_back(make_pair(3,2));



	return 0;
}