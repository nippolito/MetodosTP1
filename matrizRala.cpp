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

	vector<vector<pair<float, int> > > conecciones(n, vector<pair<float, int> > (0, make_pair(0, 0)));
	matriz->conex = conecciones;
}

void mostrarVectorPair(vector<pair<float, int > >& vec){
	// cout << "longitud de vec: " << vec.size() << endl;

	if(vec.size() == 1){
		cout << "[(" << vec[0].first << "," << vec[0].second << ")]" << endl;
	}else{
		cout << "[(" << vec[0].first << "," << vec[0].second << "), ";	
	}
	for(int i = 1; i < vec.size(); i++){
		if(i < vec.size() - 1){
			cout << "(" << vec[i].first << "," << vec[i].second << "), ";
		}else{
			cout << "(" << vec[i].first << "," << vec[i].second << ")]" << endl;
		}
	}
}

void mostrarRala(Rala* matriz){
	for(int link = 0; link < matriz->n; link++){
		mostrarVectorPair(matriz->conex[link]);
	}
}

// agrega un link de la página j a la página i
// esta operación va a depender de la matriz rala que elijamos
void agregarLink(Rala* matriz, int i, int j){
}

// el siguiente algoritmo puede cambiar dependiendo de la estructura que elijamos.

// devuelve el grado de la página j (o sea, la cantidad de elems en la columna j, o #linksSalientes)
// es O(#conexiones)
int gradoPag(struct Rala* matriz, int j){
	int res = 0;
	for(int i = 0; i < matriz->n; i++){
		vector<pair<float, int> > linksEnt = matriz->conex[i];
		for(int k = 0; k < linksEnt.size(); k++){
			if(linksEnt[k].second == j) res++;
		}
	}
	return res;
}

int main(){
	return 0;
}