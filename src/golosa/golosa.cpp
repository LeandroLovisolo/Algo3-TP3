#include "golosa.h"
#include <utility>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;


static bool compare(nodo &a, nodo &b) {
    return a.adyacentes.size() < b.adyacentes.size();
}

bool parteDelClique(vector<int> &clique, vector<nodo> &nodos, int nodoActual) {
	for (unsigned i = 0; i < clique.size(); ++i) {
		if(nodos[clique[i]].adyacentes.find(nodoActual) == nodos[clique[i]].adyacentes.end()) {
			return false;
		}
	}
	return true;
}

//Cantidad de elementos aportados a la frontera
int enFrontera(set<int> &frontera, nodo &n) {
	int cantidad = 0;
	for(set<int>::iterator it=n.adyacentes.begin(); it!=n.adyacentes.end(); ++it) {
		//Si no está en la frontera, aporta uno más a la frontera
		if(frontera.find(*it) == frontera.end()) ++cantidad;
	}
	return cantidad;
}
//Agrega los adyacentes del nodo a la frontera, solo si no existían antes
void agregarAFrontera(set<int> &frontera, nodo &n) {
	for(set<int>::iterator it=n.adyacentes.begin(); it!=n.adyacentes.end(); ++it) {
		if(frontera.find(*it) == frontera.end()) frontera.insert(*it);
	}
}

//La idea es, me paro en el nodo de máxima adyacencia, lo agrego y me voy moviendo en nodos que me aporten más
//elementos a mi frontera
pair<int, vector<int> > golosa(vector<nodo> &nodos) {
	//Consigo el nodo con más adyacentes
	vector<nodo>::iterator result = max_element(nodos.begin(), nodos.end(), compare);
	int actual = result->numero;
	set<int> c_frontera;
	//Agrego el vértice actual a la frontera para que no se tome como
	// uno más de la frontera al elegir el nuevo adyacente
	c_frontera.insert(actual);
	vector<int> resultado;
	bool cambio = true;
	while(cambio == true) {
		int maxFrontera = 0; //Guarda la frontera máxima de los adyacentes
	 	int maxAdyacente; //Guarda el índice del adyacente de frontera máxima
	 	resultado.push_back(actual); //Guardo en el resultado el vértice actual
	 	//Agrego los adyacentes del nodo actual a la frontera
	 	agregarAFrontera(c_frontera, nodos[actual]);
	 	//Veo si la frontera me incrementó, si no incrementó, no cambio nada y termino
	 	cambio = false;
	 	//Recorro los adyacentes para ver cuál elegir
	 	for (set<int>::iterator it=nodos[actual].adyacentes.begin(); it!=nodos[actual].adyacentes.end(); ++it) {
	 		//Checkeo que pertenezcan al clique, es decir, que sea adyacente a todos los elementos del clique
	 		if(parteDelClique(resultado, nodos, *it)) {
	 			//Si pertenece al clique, checkeo cuanto aportaría a la frontera
	 			//Solo me quedo con el que me aporte una cantidad máxima
	 			int nuevaFrontera = enFrontera(c_frontera, nodos[*it]);
	 			if (nuevaFrontera > maxFrontera) {
	 				maxFrontera = nuevaFrontera;
	 				maxAdyacente = nodos[*it].numero;
	 				cambio = true;
	 			}
	 		}
	 	}
	 	actual = maxAdyacente;
	 	if(!cambio) break;
	}
	return make_pair(c_frontera.size() - resultado.size(), resultado);
}