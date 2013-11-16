#ifndef COMMON_H
#define COMMON_H
#include <set>
#include <vector>
#include <algorithm>

using namespace std;

struct nodo{
	int numero;
	set<int> adyacentes;
};

int cardinalFrontera(vector<nodo> &nodos, vector<int> &solucionInicial);
bool estaEnLaClique(int nodo, vector<int> clique);
bool sonAdyacentes(nodo &m, nodo &n);
bool agregandoSigueSiendoClique(vector<int> &clique, vector<nodo> &nodos, int nodo);
bool intercambiandoSigueSiendoClique(vector<int> &clique, vector<nodo> &nodos, int nodoViejo, int nodoNuevo);

#endif // COMMON_H