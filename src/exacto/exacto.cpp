#include <iostream>
#include "exacto.h"
#include <utility>
#include <tuple>

#define tam_frontera(e) get<0>(e)
#define clique(e) get<1>(e)
#define c_frontera(e) get<2>(e)

using namespace std;

//Verifica si el nodo actual podría pertenecer al clique
bool parteDelClique(vector<int> &clique, vector<nodo> &nodos, int nodoActual) {
	for (unsigned i = 0; i < clique.size(); ++i) {
		if(nodos[clique[i]].adyacentes.find(nodoActual) == nodos[clique[i]].adyacentes.end()) {
			return false;
		}
	}
	return true;
}

//Devuelve el tamaño de la frontera del clique
int frontera(vector<nodo> &nodos, vector<int> &clique) {
	int size = 0;
	for (unsigned j = 0; j < clique.size(); ++j) {
		size += nodos[clique[j]].adyacentes.size();
	}
	size -= (clique.size()-1)*clique.size();
	return size;
}

pair<int,vector<int> > fronteraMaxima(vector<nodo> &nodos, vector<int> &clique, unsigned pos) {
	//Caso base, termino de recorrer el vector de nodos
	if(pos == nodos.size()) return make_pair(0, vector<int>());
	int fronteraMax = 0; //Guarda la frontera máxima
	vector<int> *maxClique = 0; //Guarda el clique máximo
	//Recorro los nodos que faltan y creo los subconjuntos
	for (unsigned i = pos; i < nodos.size(); ++i) {
		//Me fijo si agregando el nodo i, sigo teniendo un clique
		if(parteDelClique(clique, nodos, i)) {
			//Backup del clique y agrego el nodo actual
			vector<int> cliqueTmp(clique);
			cliqueTmp.push_back(i);

			//Llamo recursivamente a fronteraMaxima con el nuevo clique que tiene el nodo i agregado
			pair<int, vector<int> > fronteraRec = fronteraMaxima(nodos, cliqueTmp, i + 1);

			//Calculo el tamaño de la frontera con el nodo i agregado
			int tmpFrontera = frontera(nodos, cliqueTmp);

			//Comparo la frontera con el nodo i agregado vs la frontera máxima que tiene el nodo i y los
			//anteriores en el clique
			if(max(tmpFrontera, fronteraRec.first) > fronteraMax) {
				//Guardo la frontera máxima y el clique máximo
				fronteraMax = max(tmpFrontera, fronteraRec.first);
				if(maxClique != 0) delete maxClique;
				if(tmpFrontera > fronteraRec.first) {
					maxClique = new vector<int>(cliqueTmp);
				}
				else {
					maxClique = new vector<int>(fronteraRec.second);
				}
			}
		}
	}
	if(maxClique == 0) {
		return make_pair(fronteraMax, vector<int>());
	}
	else {
		vector<int> res(*maxClique);
		delete maxClique;
		return make_pair(fronteraMax, res);
	}

}