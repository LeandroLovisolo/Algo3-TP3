#ifndef FAMILIAS_H
#define FAMILIAS_H

#include "common.h"

vector<vector<nodo>> generar_grafos();

vector<nodo> lattice(int m, int n);
vector<nodo> claw(int n);
vector<nodo> kn_union_claw_m_complemento(int n, int m);
vector<nodo> path(int n);
vector<nodo> lollipop(int n, int m);
vector<nodo> fan(int n, int m);
vector<nodo> cicle(int n);
vector<nodo> centipede(int n);
vector<nodo> cagaGolosas(int n);

vector<nodo> k(int n);
vector<nodo> knm(int n, int m);
vector<nodo> producto_cartesiano(const vector<nodo> &g, const vector<nodo> &h);
vector<nodo> complemento(const vector<nodo> &g);
vector<nodo> graph_union(const vector<nodo> &G1, const vector<nodo> &G2);
vector<nodo> graph_join(const vector<nodo> &G1, const vector<nodo> &G2);

#endif // FAMILIAS_H
