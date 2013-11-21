#include "familias.h"
#include <iostream>
#include "gtest/gtest.h"
#include "exacto/exacto.h"

bool sonDoblementeAdyacentes(const nodo &m, const nodo &n) {
	return sonAdyacentes(m, n) && sonAdyacentes(n, m);
}

TEST(familias, k) {
	vector<nodo> k4 = k(4);

	ASSERT_EQ(4, k4.size());
	ASSERT_TRUE(sonDoblementeAdyacentes(k4[0], k4[1]));
	ASSERT_TRUE(sonDoblementeAdyacentes(k4[0], k4[2]));
	ASSERT_TRUE(sonDoblementeAdyacentes(k4[0], k4[3]));
	ASSERT_TRUE(sonDoblementeAdyacentes(k4[1], k4[2]));
	ASSERT_TRUE(sonDoblementeAdyacentes(k4[1], k4[3]));
	ASSERT_TRUE(sonDoblementeAdyacentes(k4[2], k4[3]));
}

TEST(familias, graph_union) {
	vector<nodo> k4 = k(4);
	vector<nodo> k3 = k(3);
	vector<nodo> Gunion = graph_union(k3, k4);
	ASSERT_EQ(7, Gunion.size());
	//Existen las mismas adyacencias para k3?
	for (unsigned i = 0; i < k3.size(); ++i) {
		// Misma cantidad de adyacentes
		ASSERT_EQ(Gunion[i].adyacentes.size(), k3[i].adyacentes.size());
		// Mismos adyacentes
		for(set<indice_nodo>::iterator it = Gunion[i].adyacentes.begin(); it != Gunion[i].adyacentes.end(); ++it) {
			ASSERT_TRUE(k3[i].adyacentes.find(*it) != k3[i].adyacentes.end());
		}
	}
	// Existen las mismas adyacencias para k4?
	for (unsigned i = k4.size(); i < Gunion.size(); ++i) {
		// Misma cantidad de adyacentes
		ASSERT_EQ(Gunion[i].adyacentes.size(), k4[i - k4.size()].adyacentes.size());
		// Mismos adyacentes
		for(set<indice_nodo>::iterator it = Gunion[i].adyacentes.begin(); it != Gunion[i].adyacentes.end(); ++it) {
			ASSERT_TRUE(k4[i - k3.size()].adyacentes.find(*it - k3.size()) != k4[i - k3.size()].adyacentes.end());
		}
	}
}

TEST(familias, graph_join) {
	vector<nodo> k4 = k(4);
	vector<nodo> k3 = k(3);
	vector<nodo> Gjoin = graph_join(k3, k4);
	ASSERT_EQ(Gjoin.size(), k3.size() + k4.size());
	//Veo que todos los nodos de k3 sean adyacentes a los de k4
	for (unsigned i = 0; i < k3.size(); ++i) {
		// Cantidad correcta de adyacentes
		ASSERT_EQ(Gjoin[i].adyacentes.size(), k3[i].adyacentes.size() + k4.size());
		// Adyacentes correctos
		for(unsigned j = k3.size(); j < Gjoin.size(); ++j) {
			ASSERT_TRUE(Gjoin[i].adyacentes.find(j) != Gjoin[i].adyacentes.end());
		}
	}
	//Veo que los nodos que pertenecían al k4 sean adyacentes a los del k3
	for (unsigned i = k3.size(); i < Gjoin.size(); ++i) {
		// Cantidad correcta de adyacentes
		ASSERT_EQ(Gjoin[i].adyacentes.size(), k4[i - k3.size()].adyacentes.size() + k3.size());
		// Adyacentes correctos
		for(unsigned j = 0; j < k3.size(); ++j) {
			ASSERT_TRUE(Gjoin[i].adyacentes.find(j) != Gjoin[i].adyacentes.end());
		}
	}
}

TEST(familias, knm) {
	vector<nodo> KnmGraph = knm(5, 8);
	// Los índices están bien puestos
	for (unsigned i = 0; i < KnmGraph.size(); ++i) {
		ASSERT_EQ(KnmGraph[i].indice, i);
	}
	
	// V1 está conectada con todo V2
	for (unsigned i = 0; i < 5; ++i) {
		ASSERT_EQ(KnmGraph[i].adyacentes.size(), 8);
		for (unsigned j = 5; j < KnmGraph.size(); ++j) {
			ASSERT_TRUE(KnmGraph[i].adyacentes.find(j) != KnmGraph[i].adyacentes.end());
		}
	}

	//V2 está conectado con todo V1
	for (unsigned i = 5; i < KnmGraph.size(); ++i) {
		ASSERT_EQ(KnmGraph[i].adyacentes.size(), 5);
		for (unsigned j = 0; j < 5; ++j) {
			ASSERT_TRUE(KnmGraph[i].adyacentes.find(j) != KnmGraph[i].adyacentes.end());
		}
	}
}

TEST(familias, knclawCalculoEcuacion) {
	int n = 4, m = 2;
	vector<nodo> grafo = kn_union_claw_m_complemento(n,m);
	// for (unsigned i = 0; i < grafo.size(); ++i) {
	// 	cout << "Nodo " << i << endl;
	// 	for (set<indice_nodo>::iterator it = grafo[i].adyacentes.begin(); it != grafo[i].adyacentes.end(); ++it) {
	// 		cout << *it << " ";
	// 	}
	// 	cout << endl;
	// }
	cmf sol = exacto(grafo);
	if(n >= m) {
		int c = m;
		int ecuacion = n*m;
		ASSERT_EQ(c, sol.second.size());
		ASSERT_EQ(sol.first, ecuacion);
	}
	else {
		int c = (m+n+1)/2;
		int ecuacion = (m - c)*c + n * c;
		ASSERT_EQ(sol.first, ecuacion);
		ASSERT_EQ(sol.second.size(), c);
	}

	// for (unsigned i = 0; i < sol.second.size(); ++i){
	// 	cout << (sol.second)[i] << " " << endl;
	// }
}

TEST(familias, producto_cartesiano) {
	// Grafo *-*
	vector<nodo> g(2);
	for(size_t i = 0; i < g.size(); i++) g[i].indice = i;
	g[0].adyacentes.insert(1);
	g[1].adyacentes.insert(0);

	// Grafo *-*-*
	vector<nodo> h(3);
	for(size_t i = 0; i < h.size(); i++) h[i].indice = i;
	h[0].adyacentes.insert(1);
	h[1].adyacentes.insert(0);
	h[1].adyacentes.insert(2);
	h[2].adyacentes.insert(1);

	// Grafo *-*-*
	//       | | |
	//       *-*-*
	vector<nodo> gh = producto_cartesiano(g, h);

	ASSERT_EQ(g.size() * h.size(), gh.size());
	ASSERT_TRUE(sonDoblementeAdyacentes(gh[0], gh[1]));
	ASSERT_TRUE(sonDoblementeAdyacentes(gh[0], gh[3]));
	ASSERT_TRUE(sonDoblementeAdyacentes(gh[1], gh[2]));
	ASSERT_TRUE(sonDoblementeAdyacentes(gh[1], gh[4]));
	ASSERT_TRUE(sonDoblementeAdyacentes(gh[2], gh[5]));
	ASSERT_TRUE(sonDoblementeAdyacentes(gh[3], gh[4]));
	ASSERT_TRUE(sonDoblementeAdyacentes(gh[4], gh[5]));

	ASSERT_FALSE(sonDoblementeAdyacentes(gh[0], gh[4]));
	ASSERT_FALSE(sonDoblementeAdyacentes(gh[1], gh[5]));
	ASSERT_FALSE(sonDoblementeAdyacentes(gh[1], gh[3]));
	ASSERT_FALSE(sonDoblementeAdyacentes(gh[2], gh[4]));
	ASSERT_FALSE(sonDoblementeAdyacentes(gh[0], gh[5]));
	ASSERT_FALSE(sonDoblementeAdyacentes(gh[2], gh[3]));
	ASSERT_FALSE(sonDoblementeAdyacentes(gh[0], gh[2]));
	ASSERT_FALSE(sonDoblementeAdyacentes(gh[3], gh[5]));
}

TEST(familias, complemento) {

	// Grafo *--*
	//       |  |
	//       |  |
	//       *--*
	vector<nodo> g(4);
    for(size_t i = 0; i < g.size(); i++) g[i].indice = i;
	g[0].adyacentes.insert(1);
	g[0].adyacentes.insert(2);
	g[1].adyacentes.insert(0);
	g[1].adyacentes.insert(3);
	g[2].adyacentes.insert(0);
	g[2].adyacentes.insert(3);
	g[3].adyacentes.insert(1);
	g[3].adyacentes.insert(2);

	// Grafo *  *
	//        \/
	//        /\
	//       *  *
	vector<nodo> h = complemento(g);

	ASSERT_TRUE(sonDoblementeAdyacentes(h[0], h[3]));
	ASSERT_TRUE(sonDoblementeAdyacentes(h[1], h[2]));

	ASSERT_FALSE(sonDoblementeAdyacentes(h[0], h[1]));
	ASSERT_FALSE(sonDoblementeAdyacentes(h[0], h[2]));
	ASSERT_FALSE(sonDoblementeAdyacentes(h[3], h[1]));
	ASSERT_FALSE(sonDoblementeAdyacentes(h[3], h[2]));	
}

GTEST_API_ int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
