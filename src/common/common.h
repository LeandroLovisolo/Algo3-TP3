#ifndef COMMON_H
#define COMMON_H

#include <set>
#include <vector>
#include <utility>

using namespace std;

// Índice de un nodo en el vector de nodos de un grafo.
typedef int indice_nodo;

// Nodo. Mantiene su índice en el vector de nodos del grafo y los índices de
// los nodos adyacentes al mismo.
struct nodo {
	indice_nodo indice;
	set<indice_nodo> adyacentes;
};

// CMF, o clique de máxima frontera.
typedef pair<int, vector<indice_nodo>> cmf;

// Constructor de CMF.
inline cmf nueva_cmf(int frontera, const vector<indice_nodo> &indices) {
	return make_pair(frontera, indices);
}

// Permiten leer/escribir los campos de una CMF.
inline int& frontera(cmf& c) { return c.first; }
inline vector<indice_nodo>& indices_nodos(cmf& c) { return c.second; }

// Dado un grafo y los índices de los nodos del mismo que forman alguna clique,
// devuelve el tamaño de su frontera
int cardinalFrontera(const vector<nodo> &nodos,
		             const vector<indice_nodo> &clique);
bool estaEnLaClique(const vector<indice_nodo> &clique, indice_nodo nodo);
bool sonAdyacentes(nodo &m, nodo &n);
bool agregandoSigueSiendoClique(const vector<nodo> &nodos,
		                        const vector<indice_nodo> &clique,
		                        indice_nodo nodo);
bool intercambiandoSigueSiendoClique(const vector<nodo> &nodos,
								     const vector<indice_nodo> &clique,
		                             indice_nodo nodoViejo,
		                             indice_nodo nodoNuevo);

#endif // COMMON_H
