#include <algorithm>

#include "common.h"

int cardinalFrontera(const vector<nodo> &nodos, const vector<indice_nodo> &clique) {
    int frontera = 0;
    for (size_t i = 0; i < clique.size(); ++i) {
        frontera += nodos[clique[i]].adyacentes.size();
    }
    return frontera - (clique.size() - 1) * clique.size();
}

bool estaEnLaClique(const vector<indice_nodo> &clique, indice_nodo nodo) {
    return find(clique.begin(), clique.end(), nodo) != clique.end();
}

bool sonAdyacentes(const nodo &m, const nodo &n) {
    return m.adyacentes.find(n.indice) != m.adyacentes.end();
}

bool agregandoSigueSiendoClique(const vector<nodo> &nodos,
                                const vector<indice_nodo> &clique,
                                indice_nodo nodo) {
    for (size_t i = 0; i < clique.size(); ++i) {
        if(!sonAdyacentes(nodos[clique[i]], nodos[nodo])) return false;
    }
    return true;
}

bool intercambiandoSigueSiendoClique(const vector<nodo> &nodos,
                                     const vector<indice_nodo> &clique,
                                     indice_nodo nodoViejo,
                                     indice_nodo nodoNuevo) {
    for (size_t i = 0; i < clique.size(); ++i) {
        if(i != (size_t) nodoViejo &&
           !sonAdyacentes(nodos[clique[i]], nodos[nodoNuevo])) return false;
    }
    return true;
}
