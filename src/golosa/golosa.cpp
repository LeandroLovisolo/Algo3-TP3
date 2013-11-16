#include "golosa.h"
#include <utility>
#include <vector>
#include <set>
#include <algorithm>
#include <iostream>

using namespace std;

static bool compare(nodo &a, nodo &b) {
    return a.adyacentes.size() < b.adyacentes.size();
}

//Puede pertenecer el nodo actual al clique?
bool parteDelClique(vector<int> &clique, vector<nodo> &nodos, int nodoActual) {
	for (unsigned i = 0; i < clique.size(); ++i) {
		if(nodos[clique[i]].adyacentes.find(nodoActual) == nodos[clique[i]].adyacentes.end()) {
			return false;
		}
	}
	return true;
}
//Me fijo si ese adyacente ya estaba en el clique
bool enElclique(vector<int> &clique, int pos) {
	for (unsigned i = 0; i < clique.size(); ++i) {
		if(clique[i] == pos) {
			return true;	
		}
	}
	return false;
}

//La idea es, me paro en el nodo de máxima adyacencia, lo agrego y me voy moviendo en nodos que me aporten más
//elementos a mi frontera
pair<int, vector<int> > golosa(vector<nodo> &nodos) {
	//Consigo el nodo con más adyacentes
	vector<nodo>::iterator result = max_element(nodos.begin(), nodos.end(), compare);
	int actual = result->numero;
	int frontera = 0;
	vector<int> resultado;
	bool cambio = true;
	while(cambio == true) {
		int maxFronteraLocal = 0; //Guarda la frontera máxima de los adyacentes
	 	int maxAdyacenteLocal; //Guarda el índice del adyacente de frontera máxima
	 	resultado.push_back(actual); //Guardo en el resultado el vértice actual
	 	//Veo si la frontera me incrementó, si no incrementó, no cambio nada y termino
	 	cambio = false;
	 	//Recorro los adyacentes para ver cuál elegir
	 	for (set<int>::iterator it=nodos[actual].adyacentes.begin(); it!=nodos[actual].adyacentes.end(); ++it) {
	 		//Checkeo que pertenezcan al clique, es decir, que sea adyacente a todos los elementos del clique
	 		if(agregandoSigueSiendoClique(resultado, nodos, *it) && !estaEnLaClique(*it, resultado)) {
	 			//Si pertenece al clique, checkeo cuanto aportaría a la frontera
	 			//Solo me quedo con el que me aporte una cantidad máxima
	 			int nuevaFronteraLocal = nodos[*it].adyacentes.size() - 2 * resultado.size();
	 			if (nuevaFronteraLocal > maxFronteraLocal) {
	 				maxFronteraLocal = nuevaFronteraLocal;
	 				maxAdyacenteLocal = *it;
	 				cambio = true;
	 			}
	 		}
	 	}
	 	actual = maxAdyacenteLocal;
	 	if(!cambio) break;
	}
 	for (unsigned i = 0; i < resultado.size(); ++i) {
 		frontera += nodos[resultado[i]].adyacentes.size();
 	}
 	frontera -= (resultado.size()-1)*resultado.size();
	return make_pair(frontera, resultado);
}