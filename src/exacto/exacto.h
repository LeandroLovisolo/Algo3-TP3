#ifndef EXACTO_H
#define EXACTO_H

#include <vector>
#include <set>
#include <algorithm>
#include <tuple>

using namespace std;

struct nodo{
	set<int> adyacentes;
};

pair<int,vector<int> > fronteraMaxima(vector<nodo> &nodos, vector<int> &clique, unsigned pos);

#endif // EXACTO_H
