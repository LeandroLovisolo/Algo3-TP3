#include <iostream>
#include "exacto.h"

using namespace std;

bool parteDelClique(vector<int> &clique, vector<nodo> &nodos, int nodoActual) {
	for (unsigned i = 0; i < clique.size(); ++i) {
		if(nodos[clique[i]].adyacentes.find(nodoActual) == nodos[clique[i]].adyacentes.end()) {
			return false;
		}
	}
	return true;
}

int fronteraMaxima(vector<nodo> &nodos, vector<bool> &nodosUsados, vector<int> &clique) {
	int frontera = 0;
	for (unsigned i = 0; i < nodosUsados.size(); ++i) {
		if(!nodosUsados[i] && parteDelClique(clique, nodos, i)) {
			nodosUsados[i] = true;
			vector<int> cliqueTmp(clique);
			cliqueTmp.push_back(i);
			int fronteraTmp = fronteraMaxima(nodos, nodosUsados, cliqueTmp);
			int nuevosEnFrontera = nodos[i].adyacentes.size();
			if(clique.size() != 1) {
				nuevosEnFrontera -= 2*clique.size();
			}
			frontera = max(fronteraTmp + nuevosEnFrontera, frontera);
		}
	}
	return frontera;

}



/*int main () {
	vector<nodo> nodos;
	 Ejemplo 1, res -> 4
	nodo v1;
	v1.adyacentes.insert(2);
	v1.adyacentes.insert(3);
	nodo v2;
	v2.adyacentes.insert(1);
	v2.adyacentes.insert(3);
	nodo v3;
	v3.adyacentes.insert(2);
	v3.adyacentes.insert(1);
	nodo v4;
	v4.adyacentes.insert(3);
	v4.adyacentes.insert(5);
	nodo v5;
	v5.adyacentes.insert(3);
	v5.adyacentes.insert(4);
	nodos.push_back(v1);
	nodos.push_back(v2);
	nodos.push_back(v3);
	nodos.push_back(v4);
	nodos.push_back(v5);

	//Ejemplo 2, res -> 5
	nodo v1;
	v1.adyacentes.insert(3);
	nodo v2;
	v2.adyacentes.insert(3);
	nodo v3;
	v3.adyacentes.insert(1);
	v3.adyacentes.insert(2);
	v3.adyacentes.insert(4);
	nodo v4;
	v4.adyacentes.insert(3);
	v4.adyacentes.insert(5);
	v4.adyacentes.insert(6);
	v4.adyacentes.insert(7);
	nodo v5;
	v5.adyacentes.insert(4);
	nodo v6;
	v6.adyacentes.insert(4);
	nodo v7;
	v7.adyacentes.insert(4);
	nodos.push_back(v1);
	nodos.push_back(v2);
	nodos.push_back(v3);
	nodos.push_back(v4);
	nodos.push_back(v5);
	nodos.push_back(v6);
	nodos.push_back(v7);
	vector<bool> nodosUsados;
	nodosUsados.resize(nodos.size(), false);
	vector<int> clique;
	cout << "Frontera max " << fronteraMaxima(nodos, nodosUsados, clique) << endl;
}
	*/