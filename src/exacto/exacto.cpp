#include "exacto.h"

// Prototipo del auxiliar recursivo.
cmf exacto_rec(const vector<nodo> &nodos,
               const vector<indice_nodo> &clique,
               unsigned pos);

cmf exacto(const vector<nodo> &nodos) {
    return exacto_rec(nodos, vector<indice_nodo>(), 0);
}

// Devuelvo la CMF que se obtiene al agregarle nodos con índice en el intervalo
// [pos + 1, n] a la clique inicial, donde n es la cantidad de nodos en el grafo.
cmf exacto_rec(const vector<nodo> &nodos,
               const vector<indice_nodo> &clique_inicial,
               unsigned pos) {

    // Caso base. Terminé de evaluar todos los nodos del grafo.
    if(pos == nodos.size()) return nueva_cmf(0, vector<indice_nodo>());

    // Mejor CMF hallada hasta el momento
    cmf mejor_cmf;

    // Evalúo todas las cliques posibles resultantes de agregar los nodos con
    // índice en el intervalo [pos + 1, n] a la clique inicial.
    for (unsigned i = pos; i < nodos.size(); ++i) {

        // ¿Sigo teniendo una clique si agrego el nodo i-ésimo a la clique
        // inicial?
        if(agregandoSigueSiendoClique(nodos, clique_inicial, i)) {

            // Considero la CMF obtenida al agregarle el i-ésimo nodo a la
            // clique inicial.
            cmf agregandoIesimo = nueva_cmf(0, clique_inicial);
            indices_nodos(agregandoIesimo).push_back(i);
            frontera(agregandoIesimo) =
                    cardinalFrontera(nodos, indices_nodos(agregandoIesimo));

            // Resuelvo el subproblema de hallar la CMF obtenida de agregarle a
            // la nueva clique algún subconjunto de los nodos con índice en el
            // intervalo [pos + 1, n].
            cmf subproblema =
                    exacto_rec(nodos, indices_nodos(agregandoIesimo), i + 1);

            // Decido cuál de las dos cliques anteriores considero como
            // candidata a clique de frontera máxima, es decir, cuál de las dos
            // cliques tiene frontera más grande.
            cmf &candidata =
                    frontera(agregandoIesimo) > frontera(subproblema) ?
                            agregandoIesimo : subproblema;

            // Si hallamos una clique con frontera mayor a la de la mejor
            // solución hallada hasta el momento, actualizamos la mejor
            // solución.
            if(frontera(candidata) > frontera(mejor_cmf)) {
                mejor_cmf = candidata;
            }
        }
    }

    return mejor_cmf;
}
