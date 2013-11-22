#include "local.h"

using namespace std;

enum operacion { AGREGAR, ELIMINAR, INTERCAMBIAR };

cmf local(const vector<nodo> &nodos, const vector<indice_nodo> &cliqueInicial) {
    cmf solucion = nueva_cmf(cardinalFrontera(nodos, cliqueInicial),
                             cliqueInicial);

    // Partimos de una clique inicial, buscamos la operación que más aristas
    // contribuye a la frontera (agregar, eliminar o intercambiar un nodo) y
    // repetimos hasta que no podamos hacer crecer más la frontera.
    while(true) {

        // Operación hallada (y parámetros) que más aumenta la frontera.
        operacion op;
        indice_nodo nodoAAgregar;
        indice_nodo nodoAEliminar;
        pair<indice_nodo, indice_nodo> nodosAIntercambiar;

        // Cantidad de aristas que aporta la operación.
        int aporte = 0;

        // Operación AGREGAR.
        for(unsigned i = 0; i < nodos.size(); ++i) {

            // ¿Puedo agregar el nodo i-ésimo a la clique?
            if(!estaEnLaClique(indices_nodos(solucion), i) &&
               agregandoSigueSiendoClique(nodos, indices_nodos(solucion), i)) {

                // Calculo cuántas aristas me aporta a la frontera.
                int aporteAgregar = nodos[i].adyacentes.size() -
                                   2 * indices_nodos(solucion).size();

                // Decido si me quedo con la operación.
                if(aporteAgregar > aporte) {
                    op = AGREGAR;
                    nodoAAgregar = i;
                    aporte = aporteAgregar;
                }
            }
        }

        // Operación INTERCAMBIAR.
        for(unsigned i = 0; i < indices_nodos(solucion).size(); ++i) {

            // Calculo cuántas aristas aporta el i-ésimo nodo de la clique.
            indice_nodo iEsimo = indices_nodos(solucion)[i];
            int aporteIEsimo = nodos[iEsimo].adyacentes.size() -
                               (indices_nodos(solucion).size() - 1);

            // Consideramos el resto de los nodos del grafo.
            for (unsigned j = 0; j < nodos.size(); ++j) {

                // ¿Si lo intercambio por el i-ésimo, sigue siendo clique?
                if(!estaEnLaClique(indices_nodos(solucion), j) &&
                   intercambiandoSigueSiendoClique(
                           nodos, indices_nodos(solucion), i, j)) {

                    // Calculo cuántas aristas aportaría el j-ésimo nodo
                    // del grafo.
                    int aporteJEsimo = nodos[j].adyacentes.size() -
                                       (indices_nodos(solucion).size() - 1);

                    // Calculo el aporte neto de intercambiar ambos nodos.
                    int aporteNeto = aporteJEsimo - aporteIEsimo;

                    // Decido si me quedo con ésta operación.
                    if(aporteNeto > aporte) {
                        aporte = aporteNeto;
                        op = INTERCAMBIAR;
                        nodosAIntercambiar = make_pair(i, j);
                    }
                }
            }
        }

        // Operación ELIMINAR
        for(unsigned i = 0; i < indices_nodos(solucion).size(); ++i) {

            // Calculo cuántas aristas agrega a la frontera el eliminar el
            // i-ésimo nodo de la clique.
            indice_nodo iEsimo = indices_nodos(solucion)[i];
            int aporteEliminar = 2 * (indices_nodos(solucion).size() - 1) -
                                 nodos[iEsimo].adyacentes.size();

            if(aporteEliminar > aporte) {
                op = ELIMINAR;
                nodoAEliminar = i;
                aporte = aporteEliminar;
            }
        }

        // Si alcanzamos un máximo local, terminamos.
        if(aporte == 0) break;

        // En caso contrario, realizo la operación que más aristas contribuye.
        switch(op) {
            case AGREGAR:
            indices_nodos(solucion).push_back(nodoAAgregar);
            break;

            case ELIMINAR:
            indices_nodos(solucion).erase(
                    indices_nodos(solucion).begin() + nodoAEliminar);
            break;

            case INTERCAMBIAR:
            indices_nodos(solucion)[nodosAIntercambiar.first] =
                    nodosAIntercambiar.second;
        }

        frontera(solucion) += aporte;
    }

    return solucion;
}
