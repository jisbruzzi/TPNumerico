//============================================================================
// Name        : numerico.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================
#include <cmath>
#include <iostream>
#include <chrono>
#include <fstream>
using namespace std;
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
		if(abs(semilla[i])>mayor){
			mayor=abs(semilla[i]);
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
		semilla[i] =gSeiden * parametroSOR + semilla[i] * (1-parametroSOR);

	}

	return semilla;
}

/**
 * Leandro, hacé que registre los errores relativos si y sólo si erroresRelativos!=NULL
 * así ahorramos tiempo
 */
void resolver( int tamanio, float w, float rTol, int &iteraciones, int &nanos, float* &solucion, float* &erroresRelativos){
	auto begin = std::chrono::high_resolution_clock::now();
	float* b=generarB(tamanio,11);
	float* xActual=generarSemilla(tamanio);
	float* xAnterior=NULL;
	int i=0;
	do{

		//mostrarSemilla(xActual,tamanio);
		delete[] xAnterior;
		xAnterior=copiarSemilla(xActual,tamanio);
		iteracion(xActual,w,tamanio,b);
		i++;
		if(i>1000){
			break;
		}
	}while(!convergencia(xAnterior,xActual,tamanio,rTol));
	solucion=xActual;
	iteraciones=i;
	auto end = std::chrono::high_resolution_clock::now();
	nanos=std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count();
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

void punto4(int tamanio, float wInicial, float wFinal, float wPaso,float rTol){
	int cantidadDePruebas=(wFinal-wInicial)/wPaso+1;
	float* wGuardados=new float[cantidadDePruebas];
	float* milisGuardados=new float[cantidadDePruebas];
	int* iteracionesGuardadas=new int[cantidadDePruebas];


	float wActual=wInicial;
	float wMejor=0;
	float iteracionesMejor=1000;

	int i=0;

	while(wActual<wFinal){
		int iteraciones, nanos;
		float* solucion, *errores;
		errores=NULL;
		resolver(tamanio,wActual,rTol,iteraciones,nanos, solucion, errores);
		delete[] solucion;
		wGuardados[i]=wActual;
		cout<<wActual<<endl;
		milisGuardados[i]=(float) nanos/1000000;
		iteracionesGuardadas[i]=iteraciones;
		if(iteraciones<iteracionesMejor){
			wMejor=wActual;
			iteracionesMejor=iteraciones;
		}
		wActual+=wPaso;
		i++;
	}

	ofstream archivo;
	archivo.open("salidaNumerico.csv");
	archivo<<"w";
	for(int j=0;j<cantidadDePruebas;j++){
		archivo<<","<<wGuardados[j];
	}
	archivo<<endl;

	archivo<<"milis";
	for(int j=0;j<cantidadDePruebas;j++){
		archivo<<","<<milisGuardados[j];
	}
	archivo<<endl;

	archivo<<"iteraciones";
	for(int j=0;j<cantidadDePruebas;j++){
		archivo<<","<<iteracionesGuardadas[j];
	}
	archivo<<endl;

	archivo.close();

	cout<<"El mejor w fue:"<<wMejor<<" con iteraciones:"<<iteracionesMejor;
}


int main() {

	/*
	int iteraciones, nanos;
	float* solucion, * errores;
	cout<<"resolviendo:"<<endl;
	resolver( 100, 1, 0.01, iteraciones, nanos, solucion, errores);
	cout<<"Solución:"<<endl;
	mostrarSemilla(solucion, 100);
	cout<<"tardé "<<(float) nanos/1000000<<" milisegundos"<<endl;
	cout<<"hice "<<iteraciones<<" iteraciones";
	*/
	punto4(100,0.1,5,0.05,0.01);
	return 0;
}
