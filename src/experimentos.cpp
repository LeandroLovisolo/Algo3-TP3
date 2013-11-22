#include <iostream>
#include <tuple>

#include "gtest/gtest.h"

#include "exacto/exacto.h"
#include "golosa/golosa.h"
#include "local/local.h"
#include "tabu/tabu.h"
#include "familias.h"

enum clique_inicial { NODO_DE_MAYOR_GRADO, SOLUCION_GOLOSA };

vector<clique_inicial> cliques_iniciales({NODO_DE_MAYOR_GRADO, SOLUCION_GOLOSA});

std::ostream& operator<< (std::ostream& os, const clique_inicial& ci) {
    if(ci == NODO_DE_MAYOR_GRADO) os <<  "Nodo de mayor grado";
    else os << "Solución de la heurística golosa";
    return os;
}

indice_nodo nodo_de_mayor_grado(const vector<nodo> &nodos) {
	indice_nodo indice_nodo_mayor_grado = 0;
	unsigned mayor_grado = 0;
	for(size_t i = 0; i < nodos.size(); i++)
		if(nodos[i].adyacentes.size() > mayor_grado)
			indice_nodo_mayor_grado = i;
	return indice_nodo_mayor_grado;
}

vector<indice_nodo> obtener_clique_inicial(const vector<nodo> &nodos, clique_inicial ci) {
	if(ci == NODO_DE_MAYOR_GRADO) return vector<indice_nodo>({nodo_de_mayor_grado(nodos)});
	else {
		cmf sol_golosa = golosa(nodos);
		return indices_nodos(sol_golosa);
	}
}

TEST(Parametros, Local) {
	vector<vector<nodo>> grafos = generar_grafos();

	clique_inicial mejor_clique_inicial;
	int suma_frontera_mejor_clique_inicial = 0;

	for(size_t ci = 0; ci < cliques_iniciales.size(); ci++) {
		int suma_frontera = 0;

		for(size_t i = 0; i < grafos.size(); i++) {
			cout << borrar_linea
				 << "Evaluando clique inicial = \"" << cliques_iniciales[ci] << "\" "
				 << "grafo " << i << " de " << grafos.size() << flush;

			cmf solucion = local(grafos[i], obtener_clique_inicial(grafos[i], cliques_iniciales[ci]));
			suma_frontera += frontera(solucion);
		}

		if(suma_frontera > suma_frontera_mejor_clique_inicial) {
			suma_frontera_mejor_clique_inicial = suma_frontera;
			mejor_clique_inicial = cliques_iniciales[ci];
		}
	}

	cout << endl
		 << "Mejor clique inicial: " << mejor_clique_inicial << endl
		 << "Suma total de fronteras: " << suma_frontera_mejor_clique_inicial << endl << endl;
}

TEST(Parametros, Tabu) {
	vector<vector<nodo>> grafos = generar_grafos();

	tuple<clique_inicial, int, int> mejores_parametros;
	int suma_frontera_mejores_parametros = 0;

	for(size_t ci = 0; ci < cliques_iniciales.size(); ci++)
	for(int movimientos_tabu = 1; movimientos_tabu <= 10; movimientos_tabu++)
	for(int nodos_tabu = 1; nodos_tabu <= 10; nodos_tabu++) {

		cout << borrar_linea
			 << "Evaluando clique inicial = \"" << cliques_iniciales[ci] << "\", "
			 << "movimientos tabú = " << movimientos_tabu << ", "
			 << "nodos tabú = " << nodos_tabu << "... " << flush;

		int suma_frontera = 0;

		for(size_t i = 0; i < grafos.size(); i++) {
			cmf solucion = tabu(grafos[i],
					            obtener_clique_inicial(grafos[i], cliques_iniciales[ci]),
					            movimientos_tabu,
					            nodos_tabu);
			suma_frontera += frontera(solucion);
		}

		cout << "suma_frontera = " << suma_frontera << endl;

		if(suma_frontera > suma_frontera_mejores_parametros) {
			suma_frontera_mejores_parametros = suma_frontera;
			mejores_parametros = make_tuple(cliques_iniciales[ci], movimientos_tabu, nodos_tabu);
		}
	}

	cout << endl
	     << "Mejores parámetros hallados: " << endl
	     << "  clique_ini = "   << get<0>(mejores_parametros) << endl
	     << "  movimientos_tabu = " << get<1>(mejores_parametros) << endl
		 << "  nodos_tabu = "       << get<2>(mejores_parametros) << endl
		 << "Suma total de fronteras: " << suma_frontera_mejores_parametros << endl << endl;
}

TEST(Parametros, Exacto) {
	vector<vector<nodo>> grafos = generar_grafos();

	for(size_t i = 0; i < grafos.size(); i++) {
		cout << "Grafo " << i << "/" << grafos.size() << "... " << flush;
		cmf solucion = exacto(grafos[i]);
		cout << "frontera = " << frontera(solucion) << endl;
	}
}

GTEST_API_ int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
