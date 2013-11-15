#include <algorithm>
#include <iostream>
#include <queue>
#include "tabu.h"

using namespace std;

int cardinalFrontera(vector<nodo> &nodos, vector<int> &solucionInicial) {
	int frontera = 0;
	for (unsigned i = 0; i < solucionInicial.size(); ++i) {
		frontera += nodos[solucionInicial[i]].adyacentes.size();
	}
	return frontera - (solucionInicial.size()-1)*solucionInicial.size();
}

bool noEsTabu(set<int> &tabu, nodo &n) {
	return tabu.find(n.numero) == tabu.end();

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

enum operacion { AGREGAR, ELIMINAR, INTERCAMBIAR };

pair<int, vector<int> > tabu(vector<nodo> &nodos, vector<int> solucionInicial, unsigned movimientosTabu, unsigned tamTabu) {
	int frontera = cardinalFrontera(nodos, solucionInicial);
	//Supongo que la vencidad es la misma solución excepto por un elemento cambiado
	unsigned movimientosTabuRestantes = movimientosTabu;
	bool faseTabu = false;
	vector<int> *mejorSolucion = 0;
	int mejorSolucionFrontera = 0;
	set<int> conjTabu;
	queue<int> colaTabu;
	while(true) {
		operacion op;
		int nodoAfectado;
		int nodoAdicional;
		int aporteAFrontera = - (nodos.size()*nodos.size());

		// Operación AGREGAR
		for (unsigned i = 0; i < nodos.size(); ++i) {
			if(!estaEnLaClique(i, solucionInicial) && 
				agregandoSigueSiendoClique(solucionInicial, nodos, i) && noEsTabu(conjTabu, nodos[i])) {
				int nuevoAporteAFrontera = nodos[i].adyacentes.size() - 2 * solucionInicial.size();
				if(nuevoAporteAFrontera > aporteAFrontera) {
					op = AGREGAR;
					nodoAfectado = i;
					aporteAFrontera = nuevoAporteAFrontera;
				}
			}
		}
		// Operación INTERCAMBIAR
		for (unsigned i = 0; i < solucionInicial.size(); ++i) {
			//Calculamos cuánto aporta i a la frontera
			int aportaI = nodos[solucionInicial[i]].adyacentes.size() - (solucionInicial.size() - 1);
			for (unsigned j = 0; j < nodos.size(); ++j) {
				//No tiene sentido intercambiar por el mismo
				if(!estaEnLaClique(j, solucionInicial) &&
					intercambiandoSigueSiendoClique(solucionInicial, nodos, i, j) && noEsTabu(conjTabu, nodos[j])) {
					int aportaJ = nodos[j].adyacentes.size() - (solucionInicial.size() - 1);
					if((aportaJ - aportaI) > aporteAFrontera) {
						aporteAFrontera = (aportaJ - aportaI);
						op = INTERCAMBIAR;
						nodoAfectado = i;
						nodoAdicional = j;
					}
				}
			}
		}
		// Operación ELIMINAR
		for (unsigned i = 0; i < solucionInicial.size(); ++i) {
			if(noEsTabu(conjTabu, nodos[i])) {
				// Nuevo aporte: restamos al tamaño de la frontera la cantidad de aristas adyacentes al nodo que eliminamos
				// y sumamos las aristas entre el nodo que eliminamos y el resto de la clique.
				int nuevoAporteAFrontera = 2 * (solucionInicial.size() - 1) - nodos[solucionInicial[i]].adyacentes.size();

				if(nuevoAporteAFrontera > aporteAFrontera) {
					op = ELIMINAR;
					nodoAfectado = i;
					aporteAFrontera = nuevoAporteAFrontera;
				}
			}
		}

		if(aporteAFrontera <= 0) {
			if(!faseTabu) {
				faseTabu = true;
				if(mejorSolucion != 0) delete mejorSolucion;
				mejorSolucion = new vector<int>(solucionInicial);
				for (unsigned i = 0; i < solucionInicial.size(); ++i) {
					cout << solucionInicial[i] << " ";
				}
				cout << endl;
				mejorSolucionFrontera = frontera;
			}
			conjTabu.insert(nodoAfectado);
			colaTabu.push(nodoAfectado);
			if(colaTabu.size() > tamTabu) {
				int primero = colaTabu.front();
				colaTabu.pop();
				conjTabu.erase(primero);
			}
			movimientosTabuRestantes--;
			if(movimientosTabuRestantes == 0) {
				vector<int> sol(*mejorSolucion);
				delete mejorSolucion;
				return make_pair(mejorSolucionFrontera, sol);
			}

		}
		else if((aporteAFrontera + frontera > mejorSolucionFrontera) && faseTabu){
			movimientosTabuRestantes = movimientosTabu;
			faseTabu = false;
		}

		switch(op) {
			case AGREGAR:
			solucionInicial.push_back(nodoAfectado);
			break;

			case ELIMINAR:
			solucionInicial.erase(solucionInicial.begin() + nodoAfectado);
			break;

			case INTERCAMBIAR:
			solucionInicial[nodoAfectado] = nodoAdicional;
		}

		frontera += aporteAFrontera;
	}
}
