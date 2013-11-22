#include "familias.h"

// Genera los grafos usados para optimizar las heurísticas
vector<vector<nodo>> generar_grafos() {
	vector<vector<nodo>> grafos;

	int initial = 2;
	int max = 11;
	int increment = 1;

	// Familia lattice
	for(int m = initial; m <= max; m += increment)
	for(int n = initial; n <= max; n += increment) {
		grafos.push_back(lattice(m, n));
	}

	// Familia (K_n U Claw_m)^c
	for(int m = initial; m <= max; m += increment)
	for(int n = initial; n <= max; n += increment) {
		grafos.push_back(kn_union_claw_m_complemento(m, n));
	}

	// Familia lollipop
	for(int m = initial; m <= max; m += increment)
	for(int n = initial; n <= max; n += increment) {
		grafos.push_back(lollipop(m, n));
	}

	// Familia cagaGolosas
	for(int n = initial; n <= max; n += increment) {
		grafos.push_back(cagaGolosas(n));
	}

	return grafos;
}



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

// Genera grafo bipartito completo
vector<nodo> knm(int n, int m) {
	vector<nodo> nodos(n + m);
	// Índices correspondientes
	for (int i = 0; i < n + m; ++i) {
		nodos[i].indice = i;
	}

	//Nodos adyacencias para bipartito completo
	for (int i = 0; i < n; ++i) {
		for (int j = n; j < n + m; ++j) {
			nodos[i].adyacentes.insert(j);
			nodos[j].adyacentes.insert(i);
		}
	}
	return nodos;
}


// Genera claw o estrella, http://mathworld.wolfram.com/ClawGraph.html
vector<nodo> claw(int n) {
	return knm(1,n);
}

// Graph union http://mathworld.wolfram.com/GraphUnion.html
// Unión disjunta, V = V1 U V2 y X = X1 U X2
vector<nodo> graph_union(const vector<nodo> &G1, const vector<nodo> &G2) {
	vector<nodo> Gunion(G1.size() + G2.size());
	// Agrego G1
	for (unsigned i = 0; i < G1.size(); ++i) {
		Gunion[i] = G1[i];
	}
	// Agrego G2
	for (unsigned i = G1.size(); i < G1.size() + G2.size(); ++i)	{
		Gunion[i].indice = i;
		// Actualizo adyacencias
		for(set<indice_nodo>::iterator it = G2[i - G1.size()].adyacentes.begin(); it != G2[i - G1.size()].adyacentes.end(); ++it) {
			Gunion[i].adyacentes.insert(*it + G1.size());
		}
	}
	return Gunion;
}

// Ver http://mathworld.wolfram.com/GraphJoin.html
// G = G1 + G2, todos los de G1 adyacentes a todos los de G2
vector<nodo> graph_join(const vector<nodo> &G1, const vector<nodo> &G2) {
	vector<nodo> Gunion = graph_union(G1, G2);
	for (unsigned i = 0; i < G1.size(); ++i) {
		for(unsigned j = 0; j < G2.size(); ++j) {
			Gunion[i].adyacentes.insert(G1.size() + j);
			Gunion[G1.size() + j].adyacentes.insert(i);
		}
	}
	return Gunion;
}

// Ver http://www.graphclasses.org/smallgraphs.html#K2Uclaw
vector<nodo> kn_union_claw_m_complemento(int n, int m) {
	vector<nodo> kn = k(n);
	vector<nodo> k1_n = claw(m);
	vector<nodo> Gunion = graph_union(kn, k1_n);
	return complemento(Gunion);
}

// Path graph http://mathworld.wolfram.com/PathGraph.html
vector<nodo> path(int n) {
	vector<nodo> Gpath(n);
	for(unsigned i = 1; i < (unsigned) n; ++i) {
		Gpath[i].indice = i;
		Gpath[i].adyacentes.insert(i - 1);
		Gpath[i - 1].adyacentes.insert(i);
	}
	return Gpath;
}

// Lollipop graph http://mathworld.wolfram.com/LollipopGraph.html
// n para el kn, m para el largo del path, m y n > 0
vector<nodo> lollipop(int n, int m) {
	vector<nodo> kn = k(n);
	// Agrego el path
	vector<nodo> Gpath = path(m);
	vector<nodo> Gunion = graph_union(kn, Gpath);
	//Conecto el path al kn
	Gunion[n].adyacentes.insert(n - 1);
	Gunion[n - 1].adyacentes.insert(n);
	return Gunion;
}

// Ver http://mathworld.wolfram.com/FanGraph.html
// F n,m  = Kn complemento + Path m
vector<nodo> fan(int n, int m) {
	vector<nodo> kn = k(n);
	vector<nodo> knComp = complemento(kn);
	vector<nodo> Pgraph = path(m);
	return graph_join(knComp, Pgraph);
}

// Grafo ciclo
vector<nodo> cicle(int n) {
	vector<nodo> Cgraph = path(n);
	Cgraph[n -1].adyacentes.insert(0);
	Cgraph[0].adyacentes.insert(n - 1);
	return Cgraph;
}

//Centipede graph http://mathworld.wolfram.com/Centipede.html
vector<nodo> centipede(int n) {
	vector<nodo> Cgraph(2*n);
	for (int i = 0; i < n; i += 2) {
		Cgraph[i].indice = i;
		Cgraph[i+1].indice = i+1;
		Cgraph[i].adyacentes.insert(i+1);
		Cgraph[i+1].adyacentes.insert(i);
		if(i != 0) {
			Cgraph[i].adyacentes.insert(i-2);
			Cgraph[i-2].adyacentes.insert(i);
		}
	}
	return Cgraph;
}

// N tamaño del nodo de mayor grado (estrella)
vector<nodo> cagaGolosas(int n) {
	vector<nodo> estrella = claw(n);
	vector<nodo> clique = k((n+1)/2);
	int nodosFronteraPorVertice = n-((n+1)/2);
	for (unsigned i = 0; i < (unsigned) (n+1)/2; ++i) {
		for (int j = 0; j < nodosFronteraPorVertice; ++j) {
			nodo nuevo;
			clique.push_back(nuevo);
			clique[clique.size() - 1].indice = clique.size() - 1;
			clique[clique.size() - 1].adyacentes.insert(i);
			clique[i].adyacentes.insert((indice_nodo) clique.size() - 1);
		}
	}
	vector<nodo> Gunion = graph_union(estrella, clique);
	// Tomo el clique un nodo de la "frontera" y lo uno con un nodo "frontera" de la estrella
	Gunion[1].adyacentes.insert((indice_nodo) Gunion.size() - 1);
	Gunion[Gunion.size() - 1].adyacentes.insert(1);
	return Gunion;
}
