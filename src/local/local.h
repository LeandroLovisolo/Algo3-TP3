#ifndef LOCAL_H
#define LOCAL_H

#include <set>
#include <vector>
#include <utility>

using namespace std;

struct nodo{
	int numero;
	set<int> adyacentes;
};


pair<int, vector<int> > local(vector<nodo> &nodos, vector<int> solucionInicial);

#endif // LOCAL_H
