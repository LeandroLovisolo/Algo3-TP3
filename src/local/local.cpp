#include <algorithm>

#include "local.h"

using namespace std;

int cardinalFrontera(vector<nodo> &nodos, vector<int> &solucionInicial) {
	int frontera = 0;
	for (unsigned i = 0; i < solucionInicial.size(); ++i) {
		frontera += nodos[solucionInicial[i]].adyacentes.size();
	}
	return frontera - (solucionInicial.size()-1)*solucionInicial.size();
}

bool estaEnLaClique(int nodo, vector<int> clique) {
  	return find(clique.begin(), clique.end(), nodo) != clique.end();
}

bool parteDelCliqueLocal(vector<int> &clique, vector<nodo> &nodos, int nodoCandidato, int nodoActual) {
	for (unsigned i = 0; i < clique.size(); ++i) {
		if(i != (unsigned) nodoActual) {
			if(nodos[clique[i]].adyacentes.find(nodoCandidato) == nodos[clique[i]].adyacentes.end()) {
				return false;
			}
		}
	}
	return true;
}

pair<int, vector<int> > local(vector<nodo> &nodos, vector<int> solucionInicial) {
	int frontera = cardinalFrontera(nodos, solucionInicial);
	pair<unsigned, unsigned> intercambio(0,0);
	//Supongo que la vencidad es la misma solución excepto por un elemento cambiado
	while(true) {
		int aporteAFrontera = 0;
		for (unsigned i = 0; i < solucionInicial.size(); ++i) {
			//Calculamos cuánto aporta i a la frontera
			int aportaI = nodos[solucionInicial[i]].adyacentes.size() - solucionInicial.size() - 1;
			for (unsigned j = 0; j < nodos.size(); ++j) {
				//No tiene sentido intercambiar por el mismo
				if(!estaEnLaClique(j, solucionInicial) && parteDelCliqueLocal(solucionInicial, nodos, i, j)) {
					int aportaJ = nodos[j].adyacentes.size() - solucionInicial.size() - 1;
					if((aportaJ - aportaI) > aporteAFrontera) {
						aporteAFrontera = (aportaJ - aportaI);
						intercambio.first = i;
						intercambio.second = j;
					}
				}
			}
		}
		if(aporteAFrontera == 0) break;
		solucionInicial[intercambio.first] = intercambio.second;
		frontera += aporteAFrontera;
	}
	return make_pair(frontera, solucionInicial);
}