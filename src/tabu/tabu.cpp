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
	//Hay 3 tipos de vecindad:
	//Vecindad con un nuevo elemento agregado
	//Vecindad sin un elemento
	//Vecindad con un elemento intercambiado
	int frontera = cardinalFrontera(nodos, solucionInicial);

	//----------> Variables del tabú <-------------
	unsigned movimientosTabuRestantes = movimientosTabu;  //movimientos tabú restantes por hacer
	bool faseTabu = false;			//estoy en "modo tabú", es decir, utilizando elementos que no mejoran mi solución
	vector<int> *mejorSolucion = 0; //mejor clique hasta el momento
	int mejorSolucionFrontera = 0; //mejor frontera hasta el momento 
	set<int> conjTabu;   //conjunto de elementos tabú
	queue<int> colaTabu; //colaTabú para saber cuál elemento eliminar cuando pasa el máximo
	//

	while(true) {
		operacion op;
		int nodoAfectado;
		int nodoAdicional;
		//comienza con un aporte muy chico para tener en cuenta operaciones que no mejoren la frontera
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

		//--------> Utilización del método tabú <---------
		if(aporteAFrontera <= 0) { //Ninguno de los pasos me mejora la frontera
			if(!faseTabu) {
				faseTabu = true; //entro en modo tabú
				if(mejorSolucion != 0) delete mejorSolucion;
				mejorSolucion = new vector<int>(solucionInicial); //guardo la mejor solución hasta el momento
				mejorSolucionFrontera = frontera;	//guardo la mejor frontera hasta el momento
			}
			conjTabu.insert(nodoAfectado); //el nodoAfectado ahora es tabú
			colaTabu.push(nodoAfectado); //agrego a la cola tabú
			//Si mi cola tabú es mayor de lo que quiero, la achico
			if(colaTabu.size() > tamTabu) { 
				int primero = colaTabu.front();
				colaTabu.pop();
				conjTabu.erase(primero);
			}
			movimientosTabuRestantes--;
			//Me quedo sin movimientos restantes tabú
			if(movimientosTabuRestantes == 0) {
				vector<int> sol(*mejorSolucion);
				delete mejorSolucion;
				return make_pair(mejorSolucionFrontera, sol);
			}

		}
		//Si los movimientos que hice hasta ahora mejoraron la mejor solución que tenía..
		else if((aporteAFrontera + frontera > mejorSolucionFrontera) && faseTabu){
			movimientosTabuRestantes = movimientosTabu;
			faseTabu = false;
		}
		//

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
