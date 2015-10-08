/*
 * MatrizDeElastica.h
 *
 *  Created on: 08/10/2015
 *      Author: Jose
 */

#ifndef MATRIZDEELASTICA_H_
#define MATRIZDEELASTICA_H_
#include "Semilla.h"
class MatrizDeElastica {
public:
	MatrizDeElastica(uint tamanio);
	virtual ~MatrizDeElastica();
	Semilla* iteracion(Semilla* semilla);
	int valorPosicion(int fila,int columna);
};

#endif /* MATRIZDEELASTICA_H_ */
