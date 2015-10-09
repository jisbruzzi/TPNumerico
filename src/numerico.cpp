//============================================================================
// Name        : numerico.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================
#include <cmath>
#include <iostream>
using namespace std;



void mostrarSemilla( float* v, int tamanio){
	cout<<endl;
	for(int i=0;i<tamanio;i++){
		cout<<v[i]<<";"<<endl;
	}
	cout<<endl;
}

float calcularFi(int i, int tamanio, int g){
	if(i==0 || i==tamanio-1){
		return 0;
	}
	float EI= exp(6*log(137));
	float L=43.7;

	float x =(i*L)/tamanio;
	float q = 5000 + g - 10*x*x + 450*x;
	float f=(q/EI)*((L/tamanio)*(L/tamanio)*(L/tamanio)*(L/tamanio));

	return f;
}

float* generarSemilla(int tamanio){
	float* s=new float[tamanio];
	for(int i=0;i<tamanio;i++){
		s[i]=0;
	}
	return s;
}

float* generarB(int tamanio,int g){
	float* b=new float [tamanio];
	for(int i=0;i<tamanio;i++){
		b[i]=calcularFi(i,tamanio,g);
	}
	return b;
}


int valorPosicion(int fila, int col, int tamanio){
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

	return 0;
}

/*
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

*/
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

		for(int j=0; j<i; j++){
			sumaIzquierda+=valorPosicion(i,j, tamanio)*semilla[j];

		}

		for(int j=i+1; j<tamanio; j++){
			sumaDerecha+=valorPosicion(i,j, tamanio)*semilla[j];
		}

		float gSeiden= ( vectorB[i] - sumaIzquierda - sumaDerecha)/valorPosicion(i,i, tamanio);
		semilla[i] =gSeiden;//( gSeiden * parametroSOR + gSeiden * (1-parametroSOR));

	}

	return semilla;
}


void resolver( int tamanio, float w, float rTol, int &iteraciones, int &milis, float* &solucion, float* &erroresRelativos){
	float* b=generarB(tamanio,11);
	float* xActual=generarSemilla(tamanio);
	float* xAnterior=generarSemilla(tamanio);
	int i=0;
	do{
		i++;
		mostrarSemilla(xActual,tamanio);
		delete[] xAnterior;
		xAnterior=copiarSemilla(xActual,tamanio);
		iteracion(xActual,w,tamanio,b);
	}while(!convergencia(xAnterior,xActual,tamanio,rTol));
	solucion=xActual;


}

void mostrarMatriz(int tamanio){
	cout<<endl;
	cout<<"----------------"<<endl;
	for(int i=0;i<tamanio;i++){
		for(int j=0;j<tamanio;j++){
			cout<<valorPosicion(i,j, tamanio)<<";";
		}
		cout<<endl;
	}
	cout<<endl<<"-----------------"<<endl;
}


int main() {
	int iteraciones, milis;
	float* solucion, * errores;
	mostrarMatriz(5);
	mostrarSemilla(generarB(5,11),5);
	cout<<"resolviendo:"<<endl;
	resolver( 5, 1, 0.00001, iteraciones, milis, solucion, errores);
	cout<<"final:"<<endl;
	mostrarSemilla(solucion, 5);
	return 0;
	/*
	 * Ésta matriz de 5x5 debería dar:
	 * 0;-7/8;-3/2;-13/8;1
	 * Y da cualquiera
	 */
}
