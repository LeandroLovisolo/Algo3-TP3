#include <iostream>

#include "golosa.h"

using namespace std;

int main() {
	while(true) {
        // Leo la cantidad de vertices.
		unsigned n;
		cin >> n;
		if(cin.eof()) break;

        // Leo la cantidad de aristas.
		unsigned m;
		cin >> m;

        // Ignoro el resto de la línea.
		string s;
		getline(cin, s);

        // Armo el grafo.
		vector<nodo> nodos;
		nodos.resize(n);
		for(unsigned i = 0; i < m; ++i) {
			unsigned v, w;
			cin >> v; // Leo nodo v.
			cin >> w; // Leo nodo w.
			nodos[v-1].adyacentes.insert(w-1);
			nodos[w-1].adyacentes.insert(v-1);
            // Ignoro el resto de la línea.
			getline(cin, s);
		}
		for (unsigned i = 0; i < n; ++i) {
			nodos[i].numero = i;
		}
		pair<int, vector<int> > res = golosa(nodos);
		cout << "Frontera: " << res.first << endl;
		cout << "Res:" << endl;
		for (unsigned i = 0; i < res.second.size(); ++i) {
			cout << "," << res.second[i]+1;
		}
		cout << endl;
	}
	return 0;
}
