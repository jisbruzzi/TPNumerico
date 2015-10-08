/*
 * Semilla.h
 *
 *  Created on: 08/10/2015
 *      Author: Jose
 */

#ifndef SEMILLA_H_
#define SEMILLA_H_
typedef unsigned int uint;

class Semilla {
private:
	int tamanio;
public:
	Semilla(int tamanio);
	virtual ~Semilla();
	float* numeros;
	int get_tamanio();
};

#endif /* SEMILLA_H_ */
