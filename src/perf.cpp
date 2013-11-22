#include <iostream>
#include <fstream>
#include <algorithm>

#include "exacto/exacto.h"
#include "local/local.h"
#include "golosa/golosa.h"
#include "tabu/tabu.h"
#include "familias.h"

///////////////////////////////////////////////////////////////////////////////
// Estructuras y código auxiliar para realizar mediciones.                   //
///////////////////////////////////////////////////////////////////////////////

class medidor;

struct algoritmo {
	string nombre;
	cmf (*fn)(const vector<nodo> &nodo);

	algoritmo(string nombre, cmf (*fn)(const vector<nodo> &nodo))
			: nombre(nombre), fn(fn) {}
};

struct familia {
	string nombre;
	void (*fn)(medidor &m);

	familia(string nombre, void (*fn)(medidor &m))
			: nombre(nombre), fn(fn) {}
};

class medidor {
public:
	medidor(algoritmo algo, familia fam);
	~medidor();
	void medir(const vector<nodo> &nodos, const vector<int> &parametros);

private:
	algoritmo algo;
	familia fam;
	ofstream csv;
};

medidor::medidor(algoritmo algo, familia fam)
		: algo(algo), fam(fam) {

	// Genero nombre de archivo base.
	string filename = "mediciones/" + algo.nombre + "-" + fam.nombre + ".csv";

	// Lo paso a minúsculas.
	transform(filename.begin(), filename.end(), filename.begin(), ::tolower);

	// Reemplazo espacios por guiones.
	for(size_t i = 0; i < filename.length(); i++)
		if(filename[i] == ' ') filename[i] = '-';

	csv.open(filename);
}

medidor::~medidor() {
	csv.close();
}

void medidor::medir(const vector<nodo> &nodos, const vector<int> &parametros) {
	// Imprimo la tupla de parámetros actual.
	for(size_t i = 0; i < parametros.size(); i++) cout << parametros[i] << " ";
	cout << flush;

	// Ejecuto el algoritmo y mido el tiempo que demora.
    clock_t tiempo = clock();
	cmf resultado = algo.fn(nodos);
	tiempo = clock() - tiempo;
    double milisegundos = tiempo * 1000.0 / CLOCKS_PER_SEC;

    // Escribo la tupla de parámetros.
    for(size_t i = 0; i < parametros.size(); i++) csv << parametros[i] << ", ";

    // Escribo el tiempo demorado y el tamaño de la frontera de la CMF devuelta.
    csv << milisegundos << ", " << frontera(resultado) << endl;

	// Limpio la última linea impresa a la salida estándar.
	cout << borrar_linea;
}

///////////////////////////////////////////////////////////////////////////////
// Algoritmos y familias de grafos.                                          //
///////////////////////////////////////////////////////////////////////////////

cmf local_golosa(const vector<nodo> &nodos) {
	cmf clique_inicial = golosa(nodos);
	return local(nodos, indices_nodos(clique_inicial));
}

cmf tabu_golosa(const vector<nodo> &nodos) {
	cmf clique_inicial = golosa(nodos);
	return tabu(nodos, indices_nodos(clique_inicial), 10, 10);
}

void familia_k(medidor &m) {
	for(int n = 10; n < 100; n++) {
		vector<nodo> nodos = k(n);
		m.medir(nodos, vector<int>({n}));
	}
}

void familia_lattice_i(medidor &medidor) {
	int m = 5;
	for(int n = 10; n < 100; n++) {
		vector<nodo> nodos = lattice(m, n);
		medidor.medir(nodos, vector<int>{n});
	}
}

void familia_lattice_ii(medidor &medidor) {
	int m = 10;
	for(int n = 10; n < 100; n++) {
		vector<nodo> nodos = lattice(m, n);
		medidor.medir(nodos, vector<int>{n});
	}
}

void familia_lattice_iii(medidor &medidor) {
	int m = 15;
	for(int n = 10; n < 100; n++) {
		vector<nodo> nodos = lattice(m, n);
		medidor.medir(nodos, vector<int>{n});
	}
}

void familia_lattice_iv(medidor &medidor) {
	int m = 20;
	for(int n = 10; n < 100; n++) {
		vector<nodo> nodos = lattice(m, n);
		medidor.medir(nodos, vector<int>{n});
	}
}

///////////////////////////////////////////////////////////////////////////////
// Punto de entrada.                                                         //
///////////////////////////////////////////////////////////////////////////////

int main() {
	vector<algoritmo> algoritmos({algoritmo("Goloso", golosa),
                                  algoritmo("Local",  local_golosa),
                                  algoritmo("Tabú",   tabu_golosa)});

	vector<familia> familias({familia("Grafo completo", familia_k),
	                          familia("Lattice I",      familia_lattice_i),
	                          familia("Lattice II",     familia_lattice_ii),
	                          familia("Lattice III",    familia_lattice_iii),
	                          familia("Lattice IV",     familia_lattice_iv)});

	for(vector<algoritmo>::iterator algoritmo = algoritmos.begin();
		algoritmo != algoritmos.end();
		algoritmo++)
	for(vector<familia>::iterator familia = familias.begin();
		familia != familias.end();
		familia++) {

		cout << algoritmo->nombre << " / " << familia->nombre << endl;

		medidor m(*algoritmo, *familia);
		familia->fn(m);
	}

	return 0;
}
