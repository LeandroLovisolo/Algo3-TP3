#ifndef TABU_H
#define TABU_H

#include "common.h"

using namespace std;

cmf tabu(const vector<nodo> &nodos,
		 const vector<indice_nodo> &solucionInicial,
		 unsigned movimientosTabu,
		 unsigned cantidadDeNodosTabu);

#endif // TABU_H
