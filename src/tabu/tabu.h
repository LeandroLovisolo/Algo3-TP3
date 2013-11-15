#ifndef TABU_H
#define TABU_H

#include <set>
#include <vector>
#include <utility>
#include "../common/common.h"

using namespace std;

pair<int, vector<int> > tabu(vector<nodo> &nodos, vector<int> solucionInicial, unsigned movimientosTabu, unsigned tamTabu);

#endif // TABU_H
