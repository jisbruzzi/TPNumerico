/*
 * MatrizDeElastica.cpp
 *
 *  Created on: 08/10/2015
 *      Author: Jose
 */

#include "MatrizDeElastica.h"

int tamanio;

MatrizDeElastica::MatrizDeElastica(uint tamanio) {
	this->tamanio = tamanio;
}

MatrizDeElastica::~MatrizDeElastica() {
	// TODO Auto-generated destructor stub
}

int MatrizDeElastica::valorPosicion(uint fila, uint col){
	//Para i=0
	if (fila==0){
		if (col==0){
		return 1;
		}
		else{
			return 0;
		}
	}

	//Para i=1
	if (fila==1){
		if ((col==0) || (col==2)){
			return -4;
		}
		if (col==1){
			return 6;
		}
		if (col==3){
			return 1;
		}
		else {
			return 0;
	}

	//Para 1<i<n-1
	if (fila>1 or fila<tamanio-1){
			if (col==fila){
				return 6;
			}
			if (col==(fila-1) || (col==(fila+1))){
				return -4;
			}
			if (col==(fila-2) || (col==(fila+2))){
				return 1;
			}
			else{
				return 0;
			}
		}
	}

	//Para i=n-1
	if (fila==tamanio-1){
		if (col==(tamanio-3)){
			return 1;
		}
		if (col==(tamanio-2)){
			return -4;
		}
		if (col==(tamanio-1)){
			return 5;
		}
		if (col==tamanio){
			return -4;
		}
		else{
			return 0;
		}
	}

	//Para i=n
	if (fila==tamanio){
		if (col==tamanio){
			return 1;
		}
		else{
			return 0;
		}
	}

}

