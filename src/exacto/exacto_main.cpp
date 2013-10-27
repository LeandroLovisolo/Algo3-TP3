#include <iostream>
#include "exacto.h"

using namespace std;

int main()
{
    cout << "Test" << endl;
    while(true) {
        // Leo la cantidad de vertices.
        unsigned n;
        cin >> n;
        if(cin.eof()) break;
        cout << "Vertices" << endl;

        // Leo la cantidad de aristas.
        unsigned m;
        cin >> m;

        // Ignoro el resto de la línea.
        string s;
        getline(cin, s);

        // Armo el grafo.
        vector<nodo> nodos;
        nodos.resize(n);
        cout << "llego" << endl;
        for(unsigned i = 0; i < m; ++i) {

            unsigned v, w;
            cin >> v; // Leo nodo v.
            cin >> w; // Leo nodo w.
            nodos[v-1].adyacentes.insert(w-1);
            nodos[w-1].adyacentes.insert(v-1);
            // Ignoro el resto de la línea.
            getline(cin, s);
        }

        set<int>::iterator j;

        for (unsigned i = 0; i < n; ++i){
        j = nodos[i].adyacentes.begin();
        cout << "adaycentes del nodo " << i+1 << endl;
                while (j != nodos[i].adyacentes.end()){
                cout << (*j)+1 << endl;
                j++;
            }
        }

        vector<bool> nodosUsados;
    	nodosUsados.resize(nodos.size(), false);
    	vector<int> clique;
    	cout << "Frontera max " << fronteraMaxima(nodos, nodosUsados, clique) << endl;

    }

    return 0;
}
