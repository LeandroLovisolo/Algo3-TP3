#include <iostream>
#include "exacto.h"
#include <utility>
#include <tuple>

#define tam_frontera(e) get<0>(e)
#define clique(e) get<1>(e)
#define c_frontera(e) get<2>(e)

using namespace std;

pair<int,vector<int> > exacto(vector<nodo> &nodos, vector<int> &clique, unsigned pos) {
	//Caso base, termino de recorrer el vector de nodos
	if(pos == nodos.size()) return make_pair(0, vector<int>());
	int fronteraMax = 0; //Guarda la frontera máxima
	vector<int> *maxClique = 0; //Guarda el clique máximo
	//Recorro los nodos que |faltan y creo los subconjuntos
	for (unsigned i = pos; i < nodos.size(); ++i) {
		//Me fijo si agregando el nodo i, sigo teniendo un clique
		if(agregandoSigueSiendoClique(clique, nodos, i)) {
			//Backup del clique y agrego el nodo actual
			vector<int> cliqueTmp(clique);
			cliqueTmp.push_back(i);

			//Llamo recursivamente a exacto con el nuevo clique que tiene el nodo i agregado
			pair<int, vector<int> > fronteraRec = exacto(nodos, cliqueTmp, i + 1);

			//Calculo el tamaño de la frontera con el nodo i agregado
			int candidatoFronteraMax = cardinalFrontera(nodos, cliqueTmp);

			//Comparo la frontera con el nodo i agregado vs la frontera máxima que tiene el nodo i y los
			//anteriores en el clique
			if(max(candidatoFronteraMax, fronteraRec.first) > fronteraMax) {
				//Guardo la frontera máxima y el clique máximo
				fronteraMax = max(candidatoFronteraMax, fronteraRec.first);
				if(maxClique != 0) delete maxClique;
				if(candidatoFronteraMax > fronteraRec.first) {
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