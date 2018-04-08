#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <utility> // para pair
#include <map>
#include <cmath> 


using namespace std;

class Rala {
public:
	Rala(int size) {
		n = size;

		vector< map<int, double>* > conecciones (n, NULL);

		for(int i = 0 ; i < n ; i ++){
			conecciones[i] = new map<int,double> ();
		}

		conex = conecciones;
	}

	~Rala() {
		for (int i = 0; i < n; i++) {
			delete(conex[i]);
		}		
	}

	int n;
	vector< map<int, double>* > conex; // lista de adyacencia. 

	vector< map<int, double>* > transp; // transpuesta
	// uso pair ya que necesitamos saber la columna a la que corresponde cada elemento para elimGauss
	// pair.first = elemento, pair.second = columna


};


void mostrarVectorEnteros(vector<double> v ){
	int n  = v.size();
	cout << "[";
	for(int i =  0 ; i < n ; i ++){
		string comaOrEnd = i == n-1 ? "" : ", "; 
		cout <<  v[i] << comaOrEnd;
	}
	cout << "]" << endl;
}


void mostrarVectorPair(map<int,double>* m, int n){
	// cout << "longitud de vec: " << vec.size() << endl;
	cout << "[";
	for (int i = 0; i < n; i++) {
		map<int, double>::iterator it = m->find(i);
		const char* comaOrEnd = i == n - 1 ? "" : ", ";
		if (it != m->end()) {
			cout << it->second << comaOrEnd;
		}
		else {
			cout << "0";
			cout << comaOrEnd;
		}
	}
	cout << "]" << endl;
}

void mostrarRala(Rala* matriz) {
	for (int link = 0; link < matriz->n; link++) {
		mostrarVectorPair(matriz->conex[link], matriz->n);
	}
}

void insertarElemento(Rala* A, int fila, int columna, double valor ){
	
	map<int,double>::iterator it = A -> conex[fila] -> find(columna);
	if( it != A -> conex[fila] -> end() ){
		it -> second = valor;
	}else{
		A -> conex[fila] -> insert(pair<int,double>(columna,valor));	
	}
}

Rala CrearIdentidad(int n ){
	Rala res = Rala(n);
	for(int i = 0 ; i < n; i++){
		insertarElemento(&res, i, i, 1 );
	}
	return res;
}


// devuelve el grado de la página j (o sea, la cantidad de elems en la columna j, o #linksSalientes)
// es O((#conexiones)logn)
int gradoSalida(struct Rala* A, int j) {
	int res = 0;
	int n = A->n;
	for (int i = 0; i < n; i++) {
		if (A->conex[i]->find(j) != A->conex[i]->end()) {
			res++;
		}
	}
	return res;
}

// suma las matrices A y B y devuelve la suma en C
void sumaMatricial(Rala* A, Rala* B, Rala* C) {
	cout << "MATRIZ A: " << endl;
	mostrarRala(A);
	cout << "MATRIZ B: " << endl;
	mostrarRala(B);

	int n = A->n;
	for (int i = 0; i < n; i++) {
		map<int, double>* filA = A->conex[i];
		map<int, double>* filB = B->conex[i];

		map<int, double>::iterator itA = filA->begin();
		map<int, double>::iterator itB = filB->begin();


		while (itA != filA->end() || itB != filB->end()) {
			if (itB == filB->end() || ((itA->first < itB->first) && itA != filA->end())) {
				insertarElemento(C, i, itA->first, itA->second);
				itA++;
			}
			else if (itA == filA->end() || ((itA->first > itB->first) && itB != filB->end())) {
				insertarElemento(C, i, itB->first, itB->second);
				itB++;
			}
			else {
				insertarElemento(C, i, itA->first, itA->second + itB->second);
				itA++;
				itB++;
			}
		}
	}
}

void createTranspose(Rala* A, Rala* At){
	int n = A -> n;
	for(int i = 0; i < n ; i ++){
		for(map<int,double>::iterator it = (A -> conex[i]) -> begin() ; it != (A -> conex[i]) -> end(); it ++){
			insertarElemento(At, i, it->first, it -> second);
		}
	}
}

double multiplicarFilas(map<int,double>* filA, map<int,double>* colB, int n){
	double ac = 0;
	map<int, double>::iterator itA = filA->begin();
	while (itA != filA->end()) {
		map<int, double>::iterator itElemB = colB->find(itA->first);
		if (itElemB != colB->end()) {
			ac += itA->second * itElemB->second;
		}
		itA++;
	}
	return ac;
}

// multiplica las matrices A y B y devuelve la multiplicación en C
void multiplicacionMatricial(Rala* A, Rala* B, Rala* C) {
	int n = A->n;
	Rala transp = Rala(n);
	createTranspose(B, &transp);
	for(int i = 0; i < n; i++){
		for(int j = 0 ; j < n ; j ++){
			map<int,double>* filA = A->conex[i];
			map<int,double>* colB = transp.conex[j];

			double multRes = multiplicarFilas(filA, colB, n);
			if (multRes != 0) {
				insertarElemento(C, i, j, multRes);
			}
		}
	}
}


//modifica v ! 
vector<double> multiplicarMatrizPorVector(Rala* A, vector<double>* v){
	int n = A -> n ;
	vector<double> res (n);
	for(int i = 0; i < n; i ++){
		map<int,double>* row = A ->conex[i];
		double ac = 0 ;
		for(map<int,double>::iterator it  = row->begin(); it != row->end(); it++){
			ac += (it->second) * (*v)[it->first];
		}
		(*v)[i] = ac;
	}
	for(int i = 0 ; i < n ; i ++){
		res[i] = (*v)[i];
	}
	return res;
}

// modifica A
void multiplicacionPorEscalar(Rala* A, double valor) {
	for (int i = 0; i < A->conex.size(); i++) {
		for (map<int, double>::iterator it = (A->conex[i])->begin(); it != (A->conex[i])->end(); it++) {
			it->second *= valor;
		}
	}
}

int maxIndexInMapFromKey(Rala* r, int col, int n) {
	double maxIndex = -1;
	double maxVal = 0;
	for (int i = col; i < n; i++) {
		map<int, double>::iterator it = r->conex[i]->find(col);
		if (it != (r->conex[i])->end()) {
			if (abs(it->second) > abs(maxVal)) {
				maxVal = it->second;
				maxIndex = i;
			}
		}
	}
	return maxIndex;
}

int firstIndexWithValueDifferentThatZeroFrom(map<int, double>* m, int i, int n) {
	for (int j = i; j < n; j++) {
		if (m->find(j) != m->end()) return j;
	}
	return -1;
}

void reduceRowFromPivot(map<int, double>* row, map<int, double>* pivot, int col, int n) {
	map<int, double>::iterator itPivot = pivot->find(col); //deberian ser lo mismo que it begin
	map<int, double>::iterator itRow = row->find(col); //deberian ser lo mismo que it begin



void reduceRowFromPivot(map<int,double>* row, map<int,double>* pivot, int fila, int col, int n, Rala* Linv){
	map<int,double>::iterator itPivot = pivot->find(col);
	map<int,double>::iterator itRow = row->find(col);
	double pivotBase = itPivot->second;
	double rowBase = itRow->second;
	double coeficiente = rowBase / pivotBase;

	insertarElemento(Linv, fila, col, coeficiente * -1);

	for (map<int, double>::iterator it = itPivot; it != pivot->end(); it++) {
		//lo cambio asi no hacemos una busqueda log n por cada elemento en el map Pivot
		while (itRow != row->end() && itRow->first < it->first)
			itRow++;
		
		//itRow = row->find(it->first);
		if (itRow == row->end() || itRow->first > it->first ) {
			row->insert(pair<int, double>(it->first, -1 * (it->second) * coeficiente)); 
		}
		else {
			double restaObtenida = (itRow->second) - coeficiente * (it->second);
			if (restaObtenida != 0)
				itRow->second = restaObtenida;
		}
	}
}



//ELIMINACION GAUSIANA

void elimincacionGaussianaSinPivoteo(Rala & A, Rala & Linv){
	int n = A.n ;
	for(int i = 0  ; i < n ; i ++){
		int filaMax = maxIndexInMapFromKey(&A, i, A.n);
		if(filaMax != -1){
//-------------------- 	transformar los ceros	
			map<int,double>* pivot = A.conex[i];

			for(int j = i+1; j < A.n ; j++){
				map<int,double> * row = A.conex[j];
				if(row -> find(i) != row->end()){
					reduceRowFromPivot(row,pivot, j, i ,n, &Linv);
				}
			}
		}
		cout << "Matriz en la iteracion " << i << " es: " << endl;
		mostrarRala(&A);
	}
}


void elimincacionGaussiana(Rala & A, Rala & Linv , Rala & Permu){
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
//-------------------- 	transformar los ceros	
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
	sumaMatricial(&Linv, &id, &C);
	Linv = C;
}

//NO TIENE QUE TENER CEROS EN LA DIAGONAL

void solveLinearEquations(Rala* A, vector<double> & conjunta, vector<double> & res , int n ){
	

	for(int i = n-1; i >= 0 ; i --){
		double ac = conjunta [i];
		for(int j = n-1 ; j > i ; j --){
			map<int,double>::iterator it2  = (A -> conex[i]) -> find(j);
			if(it2 != (A -> conex[i])->end() ){
				ac -= (res[j] * (it2 -> second));	
			}
		}
		res[i] = ac / (A -> conex[i])->find(i)->second;
	} 
}



//-------------------------------------------------------------GENERADORES
//TODOS LOS GENERADORES TOMAN UNA MATRIZ NULA COMO ENTRADA!


//Genera una matriz aleatoria con varios paramtros:

//proba: Es la probabilidad de agregar un elemento en cada columna. Tambien se puede ver como la DENSIDAD de una matriz.
//		 Toma valores de 0 a 10. Si es 0, sera una matriz nula, y si es 10 tendra un valor distinto a 0 para cada Aij.
//
//Fmin y Fman es el rango de valores que pueden tomar los elementos de la matriz.
//Ver que si no es posible hacerse, devuelve un -1


int generarMatrizAleatoria(Rala* A, int proba, double fMin, double fMax) {
	int n = A->n;
	int rangoProba = proba;
	if (rangoProba > 10 || rangoProba < 0 || fMin > fMax) { return -1; }

	if (rangoProba == 10) { rangoProba = 9; }

	if (rangoProba != 0) {

		for (int fila = 0; fila < n; ++fila)
		{
			for (int columna = 0; columna < n; ++columna)
			{
				int prob = rand() % 10;
				if (prob <= rangoProba) {

					double valor = (fMax - fMin) * ((double)rand() / (double)RAND_MAX) + fMin;
					insertarElemento(A, fila, columna, valor);
				}
			}
		}

		return 1;

	}
	else {

		return 1;
	
	}	
}


int generarMatrizConectividad(Rala* A, int proba){
	int n = A->n;
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

int generarMatrizIdentidad(Rala* A){
	int n = A->n;

		for (int fila = 0; fila < n; ++fila)
		{
			insertarElemento(A, fila, fila, 1);
		}

		return 1;
	
}

//A TIENE QUE SER MATRIZ NULA. W MATRIZ DE CONECTIVIDAD
int generarMatrizDiagonalD(Rala* A, Rala* W){
	if (A->n != W->n){ return -1;}
	int n = W->n;

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

vector<int> generarVectorE(int n){
	vector <int> result(n, 1);
	return result;
}






//-------------------------------------------------------------GENERADORES



void TestGeneradores(int prob){
	
	Rala A = Rala(5);
	generarMatrizConectividad(&A, prob);
	mostrarRala(&A);
}

void Test1ParaSuma() { 	// pasa, todo OK
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

void Test1ParaMult() { 	// pasa, todo OK
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

void Test1ParaMultPorEsc() { 	// pasa, todo OK
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

void TestEliminacionGaussiana() {
	Rala A = Rala(3);
	Rala Linv = Rala(3);
	Rala P = CrearIdentidad(3);
/*
5 0 4
3 4 4
0 2 4

*/

	insertarElemento(&A, 0, 0, 5);
	insertarElemento(&A, 0, 2, 4);


	insertarElemento(&A, 1, 0, 3);
	insertarElemento(&A, 1, 1, 4);
	insertarElemento(&A, 1, 2, 4);

	insertarElemento(&A, 2, 1, 2);
	insertarElemento(&A, 2, 2, 4);




	cout << "Matriz A original:" << endl;
	mostrarRala(&A);

	elimincacionGaussianaSinPivoteo(A, Linv);
	cout<< "\nMatriz A con EG: " << endl;
	mostrarRala(&A);

	cout << "\nMatriz L-Inv: "<< endl;
	mostrarRala(&Linv);
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
	insertarElemento(&A, 0, 0, 5);
	insertarElemento(&A, 0, 2, 4);

	insertarElemento(&A, 1, 0, 3);
	insertarElemento(&A, 1, 1, 4);
	insertarElemento(&A, 1, 2, 4);

	insertarElemento(&A, 2, 1, 2);
	insertarElemento(&A, 2, 2, 4);


///
	vector<double> b;
	b.push_back(17);
	b.push_back(23);
	b.push_back(16);

	
	vector<double> res (n); 

	cout << "Matriz A original:" << endl;
	mostrarRala(&A);

	elimincacionGaussianaSinPivoteo(A, Linv);

	cout<< "\nMatriz A con EG: " << endl;
	mostrarRala(&A);

	cout << "\nMatriz L-Inv : "<< endl;
	mostrarRala(&Linv);

	cout <<"\nVector B : " << endl;
	mostrarVectorEnteros(b);


	multiplicarMatrizPorVector(&Linv,&b); 

	cout <<"\nVector B despues de multiplicar por L-Inv: " << endl;
	mostrarVectorEnteros(b);

	solveLinearEquations(&A, b,res,n);

	cout << "\nVector Respuesta: "<< endl;
	mostrarVectorEnteros(res);


}


int main(){
	srand(time(NULL));
	TestSolveLinearEquatinos();
	//TestGeneradores(7);
	//TestEliminacionGaussiana();
	//Test1ParaSuma();
	//Test1ParaMultPorEsc();

	return 0;
}