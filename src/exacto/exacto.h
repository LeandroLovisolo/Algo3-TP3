#ifndef EXACTO_H
#define EXACTO_H

#include <vector>
#include <set>
#include <algorithm>

using namespace std;

struct nodo{
	set<int> adyacentes;
};

int fronteraMaxima(vector<nodo> &nodos, vector<bool> &nodosUsados, vector<int> &clique);

#endif // EXACTO_H
