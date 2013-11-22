#include <iostream>

#include "gtest/gtest.h"

#include "local/local.h"
#include "golosa/golosa.h"
#include "familias.h"


vector<vector<nodo>> generar_grafos() {
	vector<vector<nodo>> grafos;

	int max = 20;

	// Familia lattice
	for(int m = 5; m <= max; m += 5)
	for(int n = 5; n <= max; n += 5) {
		grafos.push_back(lattice(m, n));
	}

	// Familia (K_n U Claw_m)^c
	for(int m = 5; m <= max; m += 5)
	for(int n = 5; n <= max; n += 5) {
		grafos.push_back(kn_union_claw_m_complemento(m, n));
	}

	// Familia lollipop
	for(int m = 5; m <= max; m += 5)
	for(int n = 5; n <= max; n += 5) {
		grafos.push_back(lollipop(m, n));
	}

	// Familia cagaGolosas
	for(int n = 1; n <= max; n++) {
		grafos.push_back(cagaGolosas(n));
	}

	return grafos;
}


TEST(Parametros, Local) {
	cout << "Generando grafos..." << endl;
	vector<vector<nodo>> grafos = generar_grafos();
	cout << "Listo" << endl;

	// Solución inicial: nodo de mayor grado
	int suma_fronteras_nodo_mayor_grado = 0;
	for(size_t i = 0; i < grafos.size(); i++) {
		cout << borrar_linea
			 << "Solución inicial: nodo de mayor grado - grafo " << i << " de " << grafos.size() << flush;

		indice_nodo indice_nodo_mayor_grado = 0;
		unsigned mayor_grado = 0;
		for(size_t j = 0; j < grafos[i].size(); j++)
			if(grafos[i][j].adyacentes.size() > mayor_grado)
				indice_nodo_mayor_grado = j;

		cmf solucion = local(grafos[i], vector<indice_nodo>({indice_nodo_mayor_grado}));
		suma_fronteras_nodo_mayor_grado += frontera(solucion);
	}

	// Solución inicial: heurística golosa
	int suma_fronteras_solucion_golosa = 0;
	for(size_t i = 0; i < grafos.size(); i++) {
		cout << borrar_linea
		     << "Solución inicial: heurística golosa - grafo " << i << " de " << grafos.size() << flush;

		cmf solucion_inicial = golosa(grafos[i]);
		cmf solucion = local(grafos[i], indices_nodos(solucion_inicial));
		suma_fronteras_solucion_golosa += frontera(solucion);
	}

	cout << borrar_linea;
	cout << "Suma de fronteras para solución inicial \"nodo de mayor grado\": "
		 << suma_fronteras_nodo_mayor_grado << endl;
	cout << "Suma de fronteras para solución inicial \"heurística golosa\": "
		 << suma_fronteras_solucion_golosa << endl;
}

GTEST_API_ int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
