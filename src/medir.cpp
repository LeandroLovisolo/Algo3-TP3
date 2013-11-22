#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <dirent.h>

#include "familias.h"
#include "exacto/exacto.h"
#include "golosa/golosa.h"
#include "local/local.h"
#include "tabu/tabu.h"

vector<string> ls(string path) {
	vector<string> filenames;

	DIR *dir = opendir(path.c_str());
	struct dirent *ent;
	while((ent = readdir(dir)) != NULL) {
		filenames.push_back(ent->d_name);
	}
	closedir(dir);

	return filenames;
}

bool ends_with(const string& s, const string& suffix) {
    return s.rfind(suffix) == (s.size()-suffix.size());
}

vector<nodo> cargar_grafo(string filename) {
	ifstream in(filename);

	// Leo la cantidad de vertices.
	unsigned n;
	in >> n;

	// Leo la cantidad de aristas.
	unsigned m;
	in >> m;

	// Ignoro el resto de la línea.
	string s;
	getline(in, s);

	// Armo el grafo.
	vector<nodo> nodos;
	nodos.resize(n);
	for(unsigned i = 0; i < m; ++i) {
		unsigned v, w;
		in >> v; // Leo nodo v.
		in >> w; // Leo nodo w.
		nodos[v-1].adyacentes.insert(w-1);
		nodos[w-1].adyacentes.insert(v-1);

		// Ignoro el resto de la línea.
		getline(in, s);
	}

	// Cargo los índices.
	for (unsigned i = 0; i < n; ++i) nodos[i].indice = i;

	return nodos;
}

int cargar_tam_solucion(string filename) {
	ifstream in(filename);
	int tam;
	in >> tam;
	return tam;

}

vector<pair<vector<nodo>, int>> cargar_grafos_testing() {
	vector<pair<vector<nodo>, int>> pares_grafo_tam_solucion;

	// Leo los archivos .in
	vector<string> filenames = ls("data");
	vector<string>::iterator end = remove_if(filenames.begin(), filenames.end(),
			[] (string filename) { return filename == ".." || !ends_with(filename, ".in"); });
	filenames = vector<string>(filenames.begin(), end);

	for(size_t i = 0; i < filenames.size(); i++) {
		// Contenido del grafo
		string filename_in = "data/" + filenames[i];

		// Output esperado
		string filename_out = filename_in;
		filename_out.replace(filename_out.end() - 2, filename_out.end(), "out");

		pares_grafo_tam_solucion.push_back(
				make_pair(cargar_grafo(filename_in),
						  cargar_tam_solucion(filename_out)));
	}

	return pares_grafo_tam_solucion;
}

cmf local_optimizada(const vector<nodo> &nodos) {
	cmf solucion_golosa = golosa(nodos);
	return local(nodos, indices_nodos(solucion_golosa));
}

cmf tabu_optimizada(const vector<nodo> &nodos) {
	cmf solucion_golosa = golosa(nodos);
	return tabu(nodos, indices_nodos(solucion_golosa), 4, 1);
}

typedef cmf (*fn_algoritmo)(const vector<nodo> &nodos);

int main() {
	vector<vector<nodo>> grafos_entrenamiento = generar_grafos();
	vector<pair<vector<nodo>, int>> grafos_testing = cargar_grafos_testing();

	vector<pair<string, fn_algoritmo>> algoritmos({
		make_pair("golosa", golosa),
		make_pair("local", local_optimizada),
		make_pair("tabu", tabu_optimizada)});

	for(size_t i = 0; i < algoritmos.size(); i++) {

		/////////////////////////////
		// Grafos de entrenamiento //
		/////////////////////////////

		ofstream csv;
		csv.open("mediciones/" + algoritmos[i].first + "-entrenamiento.csv");

		for(size_t j = 0; j < grafos_entrenamiento.size(); j++) {
			cout << algoritmos[i].first << ": grafo de entrenamiento "
				 << (j + 1) << "/" << grafos_entrenamiento.size() << endl;

		    clock_t tiempo = clock();
			cmf resultado = algoritmos[i].second(grafos_entrenamiento[j]);
			tiempo = clock() - tiempo;
		    double milisegundos = tiempo * 1000.0 / CLOCKS_PER_SEC;

		    csv << milisegundos << ", " << frontera(resultado) << endl;
		}

		csv.close();

		///////////////////////
		// Grafos de testing //
		///////////////////////

		csv.open("mediciones/" + algoritmos[i].first + "-testing.csv");

		for(size_t j = 0; j < grafos_testing.size(); j++) {
			cout << algoritmos[i].first << ": grafo de testing "
				 << (j + 1) << "/" << grafos_testing.size() << endl;

		    clock_t tiempo = clock();
			cmf resultado = algoritmos[i].second(grafos_testing[j].first);
			tiempo = clock() - tiempo;
		    double milisegundos = tiempo * 1000.0 / CLOCKS_PER_SEC;

		    csv << milisegundos << ", " << frontera(resultado) << ", " << grafos_testing[j].second << endl;
		}

		csv.close();
	}

	//////////////////////////////////////////////
	// Grafos de entrenamiento: solución exacta //
	//////////////////////////////////////////////

	ofstream csv;
	csv.open("mediciones/exacto.csv");

	for(size_t j = 0; j < grafos_entrenamiento.size(); j++) {
		cout << "exacto: grafo de entrenamiento "
			 << (j + 1) << "/" << grafos_entrenamiento.size() << endl;

	    clock_t tiempo = clock();
		cmf resultado = exacto(grafos_entrenamiento[j]);
		tiempo = clock() - tiempo;
	    double milisegundos = tiempo * 1000.0 / CLOCKS_PER_SEC;

	    csv << milisegundos << ", " << frontera(resultado) << endl;
	}

	csv.close();

	////////////////////////////////////////////////
	// Algoritmo exacto: grafos generados al azar //
	////////////////////////////////////////////////

	csv.open("mediciones/exacto-random.csv");

	for(int nodos = 1; nodos <= 50; nodos ++) {
		int aristas = nodos * nodos / 3;

		cout << "exacto: grafo random de "
			 << nodos << " nodos y "
			 << aristas << " aristas" << endl;

		double promedio = 0;
		int n;
		for(n = 0; n < 10; n++) {
			vector<nodo> grafo(nodos);
			for(int i = 0; i < nodos; i++) grafo[i].indice = i;

			int aristas_random = 0;
			while(aristas_random < aristas) {
				int u = rand() % nodos, v = rand() % nodos;
				if(u != v && !sonAdyacentes(grafo[u], grafo[v])) {
					grafo[u].adyacentes.insert(v);
					grafo[v].adyacentes.insert(u);
					aristas_random++;
				}
			}

			clock_t tiempo = clock();
			cmf resultado = exacto(grafo);
			tiempo = clock() - tiempo;
			double milisegundos = tiempo * 1000.0 / CLOCKS_PER_SEC;
			promedio += milisegundos;
		}
		promedio /= n;

		csv << nodos << ", " << promedio << endl;
	}

	csv.close();

	return 0;
}
