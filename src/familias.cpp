#include "familias.h"

// Genera un grafo completo de n nodos.
vector<nodo> k(int n) {
	vector<nodo> nodos(n);
	for(int i = 0; i < n; i++) {
		nodos[i].indice = i;
		for(int j = 0; j < n; j++) {
			if(j != i) nodos[i].adyacentes.insert(j);
		}
	}
	return nodos;
}

// Ver http://mathworld.wolfram.com/GraphCartesianProduct.html.
vector<nodo> producto_cartesiano(const vector<nodo> &G1, const vector<nodo> &G2) {
	vector<nodo> P(G1.size() * G2.size());
	for(size_t i = 0; i < P.size(); i++) P[i].indice = i;

	// Recorro todos los pares de nodos ((u1, u2), (v1, v2))
	for(size_t u1 = 0; u1 < G1.size(); u1++)
	for(size_t u2 = 0; u2 < G2.size(); u2++)
	for(size_t v1 = 0; v1 < G1.size(); v1++)
	for(size_t v2 = 0; v2 < G2.size(); v2++) {
		// Calculo los índices de (u1, u2) y (v1, v2) en el grafo P.
		int u1u2 = G2.size() * u1 + u2;
		int v1v2 = G2.size() * v1 + v2;

		// Los vértices (u1, u2) y (v1, v2) son adyacentes si y sólo si:
		// - u1 = v1 y u2 es adyacente con v2, ó
		// - u2 = v2 y u1 es adyacente con v1.
		if((u1 == v1 && sonAdyacentes(G2[u2], G2[v2])) ||
		   (u2 == v2 && sonAdyacentes(G1[u1], G1[v1]))) {
			P[u1u2].adyacentes.insert(v1v2);
		}
	}

	return P;
}

vector<nodo> complemento(const vector<nodo> &g) {
	vector<nodo> h(g.size());
	for(size_t i = 0; i < h.size(); i++) h[i].indice = i;

	for(size_t i = 0; i < g.size(); i++) {
		for(size_t j = 0; j < g.size(); j++) {
			if(i != j && !sonAdyacentes(g[i], g[j])) {
				h[i].adyacentes.insert(j);
			}
		}
	}

	return h;
}

// Ver http://mathworld.wolfram.com/LatticeGraph.html.
vector<nodo> lattice(int m, int n) {
	return producto_cartesiano(k(m), k(n));
}
