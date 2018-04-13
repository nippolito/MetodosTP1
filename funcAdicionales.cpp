#include <iostream>
#include <fstream>
#include <chrono>
#include "matrizRala.h"


using namespace std;

void reduceRowFromPivotLinv(map<int,double>& row, map<int,double>& pivot, int fila, int col, int n, Rala & linv, map<int,double>::iterator& currentItRow){
	map<int,double>::iterator itPivot = pivot.find(col); // siempre lo encuentra pues la matriz es inversible
	map<int,double>::iterator itRow = row.find(col);	// siempre lo encuentra pues el código de EG evita llamar a esta función si no lo hace
	double pivotBase = itPivot->second;
	double rowBase = itRow->second;
	double coeficiente = rowBase / pivotBase;
	currentItRow ++;
					
	
	//cout << "insertando elemento en posicion [" << fila <<"," << col <<"]" <<endl;
	insertarElemento(linv, fila, col, coeficiente); 

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
void eliminacionGaussianaLinv(Rala & A, Rala & linv){
	int n = A.n ;

	vector<map<int,double>::iterator> iteradores (n);
	for(int j = 0 ; j < n ; j ++){
		iteradores[j] = A.conex[j].begin();
	}

	for(int col = 0  ; col < n ; col ++){
		// transformar los ceros y hago las restas correspondientes (Ejemplo: F2 = F2 - 3F1)	
		map<int,double> pivot = A.conex[col];
		for(int j = col+1; j < A.n ; j++){
			if(iteradores[j] != A.conex[j].end()){
				if(iteradores[j]->first == col ){
					reduceRowFromPivotLinv(A.conex[j],pivot, j, col ,n, linv, iteradores[j]);
				}
			}
		}
	}
}


int main(){
	int n = 3;
	Rala A = Rala(n);
	Rala Linv = CrearIdentidad(n);

	/*
	2 7 9 = 1   0   0     2   7    9
	1 3 6 = 1/2 1   0  *  0  -1/2  3/2
	4 5 8 = 2   18  0     0   0    -31
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

	eliminacionGaussianaLinv(A, Linv);
	mostrarRala(A);

	mostrarRala(Linv);

	

}