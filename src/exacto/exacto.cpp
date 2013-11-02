#include <iostream>
#include "exacto.h"
#include <utility>
#include <tuple>

#define tam_frontera(e) get<0>(e)
#define clique(e) get<1>(e)
#define c_frontera(e) get<2>(e)

using namespace std;

bool parteDelClique(vector<int> &clique, vector<nodo> &nodos, int nodoActual) {
	for (unsigned i = 0; i < clique.size(); ++i) {
		if(nodos[clique[i]].adyacentes.find(nodoActual) == nodos[clique[i]].adyacentes.end()) {
			return false;
		}
	}
	return true;
}

void printClique(vector<int> &clique) {
	cout << "Clique" << endl;
	cout << "[";
	for (unsigned i = 0; i < clique.size(); ++i) {
		cout << clique[i];
		if(i + 1 < clique.size()) {
			cout << ",";
		}
	}
	cout << "]" << endl;
}

pair<int,vector<int> > fronteraMaxima(vector<nodo> &nodos, vector<int> &clique, unsigned pos) {
	if(pos == nodos.size()) return make_pair(0, vector<int>());
	int fronteraMax = 0;
	vector<int> *maxClique = 0;
	for (unsigned i = pos; i < nodos.size(); ++i) {
		if(parteDelClique(clique, nodos, i)) {
			vector<int> cliqueTmp(clique);
			cliqueTmp.push_back(i);
			pair<int, vector<int> > fronteraRec = fronteraMaxima(nodos, cliqueTmp, pos + 1);
			int tmpFrontera = 0;
			for (unsigned j = 0; j < cliqueTmp.size(); ++j) {
				tmpFrontera += nodos[cliqueTmp[j]].adyacentes.size();
			}
			tmpFrontera -= (cliqueTmp.size()-1)*cliqueTmp.size();
			if(max(tmpFrontera, fronteraRec.first) > fronteraMax) {
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