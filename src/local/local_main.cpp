#include <iostream>

#include "local.h"
#include "../golosa/golosa.h"

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
		if(cin.eof()) break;
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

		pair<int, vector<int> > resGolosa = golosa(nodos);
		pair<int, vector<int> > res = local(nodos, resGolosa.second);
		cout << res.first << " " << res.second.size() << " ";
		for (unsigned i = 0; i < res.second.size(); ++i) {
			cout << res.second[i]+1;
			if(i + 1 < res.second.size()) cout << " ";
		}
		cout << endl;
	}
	return 0;
}