/*
 * Semilla.cpp
 *
 *  Created on: 08/10/2015
 *      Author: Jose
 */

#include "Semilla.h"

Semilla::Semilla(int tamanio) {
	this->numeros=new float [tamanio];
	this->tamanio=tamanio;
	// TODO Auto-generated constructor stub

}

Semilla::~Semilla() {
	delete [] numeros;
	// TODO Auto-generated destructor stub
}

int Semilla::get_tamanio(){
	return tamanio;
}
