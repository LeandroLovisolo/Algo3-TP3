#include "local.h"

int cardinalFrontera(vector<nodo> &nodos, vector<int> &solucionInicial) {
	int frontera = 0;
	for (unsigned i = 0; i < solucionInicial.size(); ++i) {
		frontera += nodos[solucionInicial[i]].adyacentes.size();
	}
	return frontera - (solucionInicial.size()-1)*solucionInicial.size();
}

bool parteDelClique(vector<int> &clique, vector<nodo> &nodos, int nodoActual) {
	for (unsigned i = 0; i < clique.size(); ++i) {
		if(nodos[clique[i]].adyacentes.find(nodoActual) == nodos[clique[i]].adyacentes.end()) {
			return false;
		}
	}
	return true;
}

pair<int, vector<int> > local(vector<nodo> &nodos, vector<int> solucionInicial) {
	bool cambio = true;
	int frontera = cardinalFrontera(nodos, solucionInicial);
	pair<unsigned, unsigned> intercambio(0,0);
	//Supongo que la vencidad es la misma solución excepto por un elemento cambiado
	int aporteAFrontera = 0;
	while(cambio) {
		for (unsigned i = 0; i < solucionInicial.size(); ++i) {
			//Calculamos cuánto aporta i a la frontera
			int aportaI = nodos[solucionInicial[i]].adyacentes.size() - solucionInicial.size();
			for (unsigned j = 0; j < nodos.size(); ++j) {
				//No tiene sentido intercambiar por el mismo
				if(j != (unsigned) solucionInicial[i] && parteDelClique(solucionInicial, nodos, j)) {
					int aportaJ = nodos[j].adyacentes.size() - solucionInicial.size();
					if((aportaJ - aportaI) > aporteAFrontera) {
						aporteAFrontera = (aportaJ - aportaI);
						intercambio.first = i;
						intercambio.second = j;
					}
				}
			}
		}
		if(aporteAFrontera == 0) cambio = false;
		solucionInicial[intercambio.first] = intercambio.second;
		// intercambio.first = 0;
		// intercambio.second = 0;
		frontera += aporteAFrontera;
	}
	return make_pair(frontera, solucionInicial);
}