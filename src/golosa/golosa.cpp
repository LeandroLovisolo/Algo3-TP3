#include <algorithm>

#include "golosa.h"

indice_nodo nodoDeMayorGrado(const vector<nodo> &nodos) {
    vector<nodo>::const_iterator result =
            max_element(nodos.begin(),
                        nodos.end(),
                        [] (const nodo &a, const nodo &b) {
                            return a.adyacentes.size() < b.adyacentes.size();
                        });
    return result->indice;
}

cmf golosa(const vector<nodo> &nodos) {
    // Solución que voy a devolver, incialmente vacía.
    cmf clique;

    // Parto del nodo de mayor grado.
    indice_nodo actual = nodoDeMayorGrado(nodos);

    while(true) {
        // Agrego el nodo actual a la solución.
        indices_nodos(clique).push_back(actual);

        // Recorro los nodos adyacentes al actual y me quedo con el que más
        // me hace crecer la frontera de la clique si lo agrego a ésta.

        // Nodo adyacente que más aporta a la frontera hasta el momento
        // (candidato a agregarlo a la clique.)
        indice_nodo candidato;

        // Aporte de dicho nodo.
        int aporteCandidato = 0;

        // Recorro la lista de adyacencia del nodo actual.
        for(set<indice_nodo>::iterator it = nodos[actual].adyacentes.begin();
            it != nodos[actual].adyacentes.end();
            ++it) {

            // Nodo adyacente actual
            indice_nodo adyacente = *it;

            // Si puedo agregarlo a la clique, lo considero.
            if(agregandoSigueSiendoClique(
                    nodos, indices_nodos(clique), adyacente) &&
               !estaEnLaClique(indices_nodos(clique), adyacente)) {

                // Calculo la cantidad de aristas que aportaría este nodo.
                int aporte = nodos[adyacente].adyacentes.size() -
                             2 * indices_nodos(clique).size();

                // Si ésta cantidad supera al aporte del que más aportaba
                // hasta el momento, lo guardamos como nuevo candidato.
                if (aporte > aporteCandidato) {
                    aporteCandidato = aporte;
                    candidato = adyacente;
                }
            }
        }

        // Si encontramos un nodo tal que al agregarlo a la clique crece la
        // frontera, volvemos a empezar recorriendo la lista de adyacencia de
        // dicho nodo.
        if(aporteCandidato > 0) actual = candidato;

        // En caso contrario, alcanzamos un óptimo local. El algoritmo termina.
        else break;
    }

    // Calculo el tamaño de la frontera y devuelvo la solución.
    frontera(clique) = cardinalFrontera(nodos, indices_nodos(clique));
    return clique;
}
