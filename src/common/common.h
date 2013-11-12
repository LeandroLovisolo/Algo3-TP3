#ifndef COMMON_H
#define COMMON_H
#include <set>

struct nodo{
	int numero;
	std::set<int> adyacentes;
};


#endif // COMMON_H