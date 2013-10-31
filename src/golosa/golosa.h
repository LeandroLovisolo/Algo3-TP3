#include <set>
#include <utility>
#include <vector>

using namespace std;

struct nodo{
	int numero;
	set<int> adyacentes;
};

pair<int, vector<int> > golosa(vector<nodo> &nodos);