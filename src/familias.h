#ifndef FAMILIAS_H
#define FAMILIAS_H

#include "common.h"

vector<nodo> lattice(int m, int n);

vector<nodo> k(int n);
vector<nodo> producto_cartesiano(const vector<nodo> &g, const vector<nodo> &h);
vector<nodo> complemento(const vector<nodo> &g);

#endif // FAMILIAS_H
