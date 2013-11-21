#include <queue>
#include <limits>

#include "tabu.h"

class nodos_tabu {
public:
	nodos_tabu(size_t tam);
	void marcar(indice_nodo nodo);
	bool es_tabu(indice_nodo nodo);

private:
	set<indice_nodo> conjunto;
	queue<indice_nodo> cola;
	size_t tam;
};

nodos_tabu::nodos_tabu(size_t tam) : tam(tam) { }

void nodos_tabu::marcar(indice_nodo nodo) {
	conjunto.insert(nodo);
	cola.push(nodo);

	// Si me excedo del tamaño máximo, quito el elemento más viejo.
	if(cola.size() > tam) {
		conjunto.erase(cola.front());
		cola.pop();
	}
}

bool nodos_tabu::es_tabu(indice_nodo nodo) {
	return conjunto.find(nodo) != conjunto.end();
}

enum operacion { AGREGAR, ELIMINAR, INTERCAMBIAR };

cmf tabu(const vector<nodo> &nodos,
		 const vector<int> &cliqueInicial,
		 unsigned movimientosTabu,
		 unsigned cantidadDeNodosTabu) {

	// Solución correspondiente a la iteración actual del algoritmo.
	cmf solucion = nueva_cmf(cardinalFrontera(nodos, cliqueInicial),
			                 cliqueInicial);

	// Mejor solución hallada hasta el momento.
	cmf mejorSolucion = nueva_cmf(0, vector<indice_nodo>());

	// Colección de nodos marcados como tabú.
	nodos_tabu tabu(cantidadDeNodosTabu);

	// Fase del algoritmo.
	bool faseTabu = false;

	// Iteraciones del algoritmo restantes bajo fase tabú. Cuando se nos
	// agotan devolvemos la mejor solución hallada hasta el momento.
	unsigned movimientosTabuRestantes = movimientosTabu;

	// Partimos de una clique inicial, buscamos la operación que más aristas
	// contribuye a la frontera (agregar, eliminar o intercambiar un nodo) y
	// repetimos hasta que no podamos hacer crecer más la frontera. Luego
	// comienza la fase tabú, que encuentra una solución todavía mejor a la
	// mejor solución hallada hasta el momento, o se rinde luego de agotar los
	// movimientos tabú restantes y devuelve la mejor solución hallada hasta
	// el momento.
	while(true) {

		// Operación hallada (y parámetros) que más aumenta la frontera.
		operacion op;
		indice_nodo nodoAAgregar;
		indice_nodo nodoAEliminar;
		pair<indice_nodo, indice_nodo> nodosAIntercambiar;

		// Cantidad de aristas que aporta la operación. Considero aportes
		// negativos.
		int aporte = numeric_limits<int>::min();

		// Operación AGREGAR.
		for(unsigned i = 0; i < nodos.size(); ++i) {
			if(tabu.es_tabu(nodos[i].indice)) continue;

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
			if(tabu.es_tabu(nodos[i].indice)) continue;

			// Calculo cuántas aristas aporta el i-ésimo nodo de la clique.
			indice_nodo iEsimo = indices_nodos(solucion)[i];
			int aporteIEsimo = nodos[iEsimo].adyacentes.size() -
					           (indices_nodos(solucion).size() - 1);

			// Consideramos el resto de los nodos del grafo.
			for (unsigned j = 0; j < nodos.size(); ++j) {
				if(tabu.es_tabu(nodos[j].indice)) continue;

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
			if(tabu.es_tabu(indices_nodos(solucion)[i])) continue;

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

		// ¿Llegué a un máximo local?
		if(aporte <= 0) {

			// Si no estaba en fase tabú, marco la solución actual como
			// mejor hasta el momento e inicio la fase tabú.
			if(!faseTabu) {
				mejorSolucion = solucion;
				faseTabu = true;
			}

			// Marco como tabú el nodo afectado por la operación que menos
			// me empeora la solución actual.
			switch(op) {
				case AGREGAR:      tabu.marcar(nodoAAgregar); break;
				case ELIMINAR:     tabu.marcar(nodoAEliminar); break;
				case INTERCAMBIAR: tabu.marcar(nodosAIntercambiar.first);
			}

			// Si me quedé sin movimientos tabú, devuelvo la mejor solución
			// hallada hasta el momento.
			movimientosTabuRestantes--;
			if(movimientosTabuRestantes == 0) return mejorSolucion;
		}

		// No estoy en un máximo local, o sea que hay un aporte positivo.
		// Si estaba en fase tabú y el aporte supera la mejor solución hallada
		// hasta el momento, finalizo el modo tabú y el algoritmo continúa
		// normalmente.
		else if(faseTabu && (aporte + frontera(solucion) > frontera(mejorSolucion))) {
			faseTabu = false;
			movimientosTabuRestantes = movimientosTabu;
		}

		// Realizo la operación que más aristas contribuye.
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
}
