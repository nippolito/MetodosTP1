#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <utility> // para pair

using namespace std;

struct Rala{
	int n;
	vector<vector<pair<float, int> > > conex; // lista de adyacencia. 
	// uso pair ya que necesitamos saber la columna a la que corresponde cada elemento para elimGauss
	// pair.first = elemento, pair.second = columna
};

// Cabe aclarar que la struct puede cambiar, por eso me frené en la implementación hasta que definamos bien.
// Sólo dejé acá un par de funciones boludas

void crearRala(struct Rala* matriz, int n){
	matriz->n = n;

	vector<vector<pair<float, int> > > conecciones (n, vector<pair<float, int> > (0));

	matriz->conex = conecciones;
}

void mostrarVectorPair(vector<pair<float, int > >& vec){
	// cout << "longitud de vec: " << vec.size() << endl;
	cout << "[";
	int n = vec.size();
	for(int i = 0 ; i < n ; i ++){
		string comaOrEnd = i == n-1 ? "" : ", ";  
		cout << "(" << vec[i].first << "," << vec[i].second << ")" << comaOrEnd;
	}
	cout << "]" << endl;
}

void mostrarRala(Rala* matriz){
	for(int link = 0; link < matriz->n; link++){
		mostrarVectorPair(matriz->conex[link]);
	}
}

//Busqueda binaria de j en el .second en el vector pasado como parametro.
//Si no se encuentra el elemento, devuelve el indice -1
//ver que NO puede haber repetidos (no puede haber varias columnas "2")
//Considerar que tome como parametro un puntero al vector para evitar copiar toda la estructura.
int buscarColumnaEnFila(vector<pair<float, int> >& vec, int j){ 

	int desde = 0;
	int hasta = vec.size()-1;
	int pivote;
	bool encontrado = false;
	int res = -1;

	if(vec.size() == 0) {
		cout << "EL SIZE ES CERO" << endl;
		return -1;};

	if(vec[desde].second > j || vec[hasta].second < j){return res;}

	while(!encontrado){
		if(vec[hasta].second == j){res = hasta; encontrado = true;} 
		if(vec[desde].second == j){res = desde; encontrado = true;}

		if((hasta - desde) == 1 ){
			encontrado = true;
		}

		pivote = (hasta + desde)/2;

		
		if(vec[pivote].second == j){

			encontrado = true;
			res = pivote;

		} else {

			if(vec[pivote].second<j){
				desde = pivote;
			}


			if(vec[pivote].second>j){
				hasta = pivote;
			}

		}

	}

	return res;
}

int buscarPosicionParaAgregarOrdenado(vector<pair<float, int> >& vec, int  columna){


	int desde = 0;
	int hasta = vec.size()-1;
	int pivote;
	bool encontrado = false;
	int res = -1;

	if(vec[desde].second >  columna){return 0;}
	if(vec[hasta].second <  columna){return hasta+1;}
	while(!encontrado){
	
		if(vec[hasta].second ==  columna){encontrado = true;} 
		if(vec[desde].second ==  columna){encontrado = true;}

		if((hasta - desde) == 1 ){
			encontrado = true;
			res = hasta;
		}

		pivote = (hasta + desde)/2;

		if(vec[pivote].second ==  columna){
			encontrado = true;

		} else {

			if(vec[pivote].second< columna){
				desde = pivote;
			}
			if(vec[pivote].second> columna){
				hasta = pivote;
			}

		}

	}

	return res;
}

void insertarElemento(Rala* A, int fila, int columna, float valor ){

	if(A->conex[fila].size() == 0){
		A->conex[fila].push_back(make_pair(valor, columna));
	}else{
		int i = buscarPosicionParaAgregarOrdenado(A->conex[fila], columna);
		if(i == A->conex[fila].size()){ 
			A->conex[fila].push_back(make_pair(valor, columna));
		}
		else {
			A->conex[fila].insert(A->conex[fila].begin()+i, make_pair(valor, columna));
		}
	}
}

// devuelve el grado de la página j (o sea, la cantidad de elems en la columna j, o #linksSalientes)
// es O((#conexiones)logn)
int gradoPag(struct Rala* A, int j){
	int res = 0;
	int n = A->n;
	for(int i = 0; i < n; i++){
		vector<pair<float, int> > filaI = A->conex[i];
		if(buscarColumnaEnFila(filaI, j) != -1){
			res++;
		}
	}
	return res;
}

// suma las matrices A y B y devuelve la suma en C
void sumaMatricial(Rala* A, Rala* B, Rala* C){
	mostrarRala(A);
	mostrarRala(B);
	int n = A->n;
	crearRala(C, n);
	for(int i = 0; i < n; i++){
		vector<pair<float, int> > filA = A->conex[i];
		vector<pair<float, int> > filB = B->conex[i];
		int indA = 0;
		int indB = 0;
		while(indA < filA.size() && indB < filB.size()){
			pair<float, int> parA = filA[indA];
			pair<float, int> parB = filB[indB];
			if(parA.second == parB.second){
				int suma = parA.first + parB.first;
				insertarElemento(C, i, parA.second, suma);
				indA++;
				indB++;
			}
			if(parA.second < parB.second){
				insertarElemento(C, i, parA.second, parA.first);
				indA++;
			}
			if(parA.second > parB.second){
				insertarElemento(C, i, parB.second, parB.first);
				indB++;
			}
		}
		if(indA < filA.size()){
			for(int j = indA; j < filA.size(); j++){
				pair<float, int> ppA = filA[indA];
				insertarElemento(C, i, ppA.second, ppA.first);
			}
		}
		if(indB < filB.size()){
			for(int j = indB; j < filB.size(); j++){
				pair<float, int> ppB = filB[indB];
				insertarElemento(C, i, ppB.second, ppB.first);
			}
		}
	}
}

// ojo que la dejé llena de couts, no estoy seguro si es el código ya que el buscarColumnaEnFila está fallando
void multiplicacionMatricial(Rala* A, Rala* B, Rala* C){
	int n = A->n;
	crearRala(C, n);
	for(int i = 0; i < n * n; i++){
		// i/n es la fila en la que tiene que ir el elem en C
		// i%n es la columna en la que tiene que ir el elem en C
		vector<pair<float, int> > filA = A->conex[(i / n)];
		int colC = i % n; // columnas van de 0 a n-1
		float acum = 0;
		for(int j = 0; j < filA.size(); j++){
			float valAct = filA[j].first; // es el valor de A que tengo que multiplicar por uno de B
			int colAct = filA[j].second; // la fila de B en la que se tiene que encontrar el elemento a multiplicar por el de A
			vector<pair<float, int> > filaRevisar = B->conex[colAct];
			int k = buscarColumnaEnFila(filaRevisar, colC);
			cout << "fila: " << colAct << endl;
			cout << "col: " << colC << endl;
			cout << k << endl;
			if(k != -1){ // si en la fila correspondiente de B, la columna que busco tenía un elem
				acum = acum + (valAct * filaRevisar[k].first);
			}
		}
		if(acum != 0){ // si la multiplicación no dio 0, entonces agrego el elem a C
			insertarElemento(C, i/n, colC, acum);
		}
	}
}

void multiplicacionPorEscalar(Rala* A, float valor ){
	for (int i = 0; i < A->conex.size(); ++i)
	{
		for (int j = 0; j < A->conex[i].size(); ++j)
		{
			A->conex[i][j].first = A->conex[i][j].first * valor;   
		}
	}
}



void Test1ParaSuma(){ 	// pasa, todo OK
	Rala A;
	crearRala(&A, 3);

	Rala B;
	crearRala(&B, 3);

	insertarElemento(&A, 0, 0, 4);
	insertarElemento(&A, 0, 2, 2);
	insertarElemento(&A, 1, 2, 1);
	insertarElemento(&A, 2, 1, 2);
	insertarElemento(&B, 0, 2, 5);
	insertarElemento(&B, 1, 0, 3);
	insertarElemento(&B, 1, 1, 2);
	insertarElemento(&B, 2, 2, 3);

	Rala C;
	sumaMatricial(&A, &B, &C);
	mostrarRala(&C);
}

void Test1ParaMult(){
	Rala A;
	crearRala(&A, 3);

	Rala B;
	crearRala(&B, 3);

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

	Rala C;
	multiplicacionMatricial(&A, &B, &C);
	cout << endl;
	mostrarRala(&C);	
}


int main(){
	// Test1ParaSuma();
	// vector<vector<pair<float, int> > > matA = (&A)->conex;

	// Rala* B;
	// crearRala(&B, 3);
	// vector<vector<pair<float, int> > > matB = B->conex;

	// Test1ParaMult();
	vector<pair<float, int> > p;
	p.push_back(make_pair(1,0));
	p.push_back(make_pair(3,2));
	cout << buscarColumnaEnFila(p, 2) << endl; // está devolviendo -1



	return 0;
}