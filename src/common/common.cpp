#include "common.h"
#include <vector>

int cardinalFrontera(vector<nodo> &nodos, vector<int> &clique) {
	int frontera = 0;
	for (unsigned i = 0; i < clique.size(); ++i) {
		frontera += nodos[clique[i]].adyacentes.size();
	}
	return frontera - (clique.size()-1)*clique.size();
}
bool estaEnLaClique(int nodo, vector<int> clique) {
  	return find(clique.begin(), clique.end(), nodo) != clique.end();
}

bool sonAdyacentes(nodo &m, nodo &n) {
	return m.adyacentes.find(n.numero) != m.adyacentes.end();
}

bool agregandoSigueSiendoClique(vector<int> &clique, vector<nodo> &nodos, int nodo) {
	for (unsigned i = 0; i < clique.size(); ++i) {
		if(!sonAdyacentes(nodos[clique[i]], nodos[nodo])) return false;
	}
	return true;
}

bool intercambiandoSigueSiendoClique(vector<int> &clique, vector<nodo> &nodos, int nodoViejo, int nodoNuevo) {
	for (unsigned i = 0; i < clique.size(); ++i) {
		if(i != (unsigned) nodoViejo && !sonAdyacentes(nodos[clique[i]], nodos[nodoNuevo])) return false;
	}
	return true;
}