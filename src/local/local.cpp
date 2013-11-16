#include <algorithm>
#include <iostream>

#include "local.h"

using namespace std;

enum operacion { AGREGAR, ELIMINAR, INTERCAMBIAR };

pair<int, vector<int> > local(vector<nodo> &nodos, vector<int> solucionInicial) {
	int frontera = cardinalFrontera(nodos, solucionInicial);
	//Supongo que la vencidad es la misma solución excepto por un elemento cambiado

	while(true) {
		cout << endl;
		operacion op;
		int nodoAfectado;
		int nodoAdicional;
		int aporteAFrontera = 0;

		// Operación AGREGAR
		for (unsigned i = 0; i < nodos.size(); ++i) {
			if(!estaEnLaClique(i, solucionInicial) && agregandoSigueSiendoClique(solucionInicial, nodos, i)) {
				int aporte = nodos[i].adyacentes.size() - 2 * solucionInicial.size();
				if(aporte > aporteAFrontera) {
					op = AGREGAR;
					nodoAfectado = i;
					aporteAFrontera = aporte;
				}
			}
		}
		// Operación INTERCAMBIAR
		for (unsigned i = 0; i < solucionInicial.size(); ++i) {
			//Calculamos cuánto aporta i a la frontera
			int aportaI = nodos[solucionInicial[i]].adyacentes.size() - (solucionInicial.size() - 1);
			for (unsigned j = 0; j < nodos.size(); ++j) {
				//No tiene sentido intercambiar por el mismo
				if(!estaEnLaClique(j, solucionInicial) && intercambiandoSigueSiendoClique(solucionInicial, nodos, i, j)) {
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

			// Nuevo aporte: restamos al tamaño de la frontera la cantidad de aristas adyacentes al nodo que eliminamos
			// y sumamos las aristas entre el nodo que eliminamos y el resto de la clique.
			int nuevoAporteAFrontera = 2 * (solucionInicial.size() - 1) - nodos[solucionInicial[i]].adyacentes.size();

			if(nuevoAporteAFrontera > aporteAFrontera) {
				op = ELIMINAR;
				nodoAfectado = i;
				aporteAFrontera = nuevoAporteAFrontera;
			}
		}
		if(aporteAFrontera == 0) break;

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

	return make_pair(frontera, solucionInicial);
}