//============================================================================
// Name        : numerico.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
using namespace std;

int main() {
	cout<<"aasssdd"<<endl;
	return 0;
}

float calcularFi(int i, int tamanio, int g){
	float EI= exp(6*log(137));
	float L=43.7;
	cout<<EI;
	cout<<L;

	float x =(i*L)/tamanio;
	float q = 5000 + g - 10*x*x + 450*x;
	float f=(q/EI)*((L/tamanio)*(L/tamanio)*(L/tamanio)*(L/tamanio));

	cout<<"Hola";
	return f;
}

int getValorPosicion(int fila, int col, int tamanio){
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
			return 5;
		}
		if (col==3){
			return 1;
		}
		else {
			return 0;
		}
	}

	//Para 1<i<n-1
	if (fila>1 and fila<tamanio-2){
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

	//Para i=n-1
	if (fila==tamanio-2){
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

	if (fila==tamanio-1){
		if (col==tamanio-1){
			return 1;
		}
		else{
			return 0;
		}
	}

}


int valorPosicion(int fila, int col,int tamanio){
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

	return 0;

}
float* copiarSemilla(float* semilla,int tamanio){
	float* nueva=new float[tamanio];
	for(int i=0;i<tamanio;i++){
		nueva[i]=semilla[i];
	}
	return nueva;
}

float normaInfinito(float* semilla, int tamanio){
	float mayor=0;
	for(int i=0;i<tamanio;i++){
		if(semilla[i]>mayor){
			mayor=semilla[i];
		}
	}
	return mayor;
}
float* restaSemillas(float* semillaActual, float* semillaAnterior,int tamanio){
	float* actual=copiarSemilla(semillaActual,tamanio);
	for(int i=0;i<tamanio;i++){
		actual[i]-=semillaAnterior[i];
	}
	return actual;
}
bool convergencia(float* semillaAnterior, float* semillaActual,int tamanio, float rTol){
	float* resta=restaSemillas(semillaActual, semillaAnterior, tamanio);
	float normaSuperior=normaInfinito(resta,tamanio);
	delete [] resta;
	float normaInferior=normaInfinito(semillaActual,tamanio);
	return (normaSuperior/normaInferior)<=rTol;
}

float* iteracion(float* semilla, float parametroSOR, int tamanio, float*vectorB){
	for(int i=0; i<tamanio;i++){
		float sumaIzquierda=0, sumaDerecha=0;

		for(int j=0; j<i-1; j++){
			sumaIzquierda+=valorPosicion(i,j, tamanio)*semilla[j];
		}

		for(int j=i+1; j<tamanio; j++){
			sumaDerecha+=valorPosicion(i,j, tamanio)*semilla[j];
		}

		float gSeiden= ( vectorB[i] - sumaIzquierda - sumaDerecha)/valorPosicion(i,i, tamanio);
		semilla[i] =( gSeiden * parametroSOR + gSeiden * (1-parametroSOR));
	}

	return semilla;
}
