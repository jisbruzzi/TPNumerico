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
#include <list>
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
	const int numGrupo = 11;
	const float EI = 137e6;
	const float L = 43.7;
	int n = tamanio-1;
	float X = ((i*L)/n);
	return ((5000 + numGrupo - 10*(pow(X,2)) + 450 * X)/EI) * pow((L/n),4);
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
	/*
	float sumaDeCuadrados=0;
	for(int i=0;i<tamanio;i++){
		sumaDeCuadrados=semilla[i]*semilla[i];
	}
	return sqrt(sumaDeCuadrados);
	*/

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
float error(float* semillaAnterior, float* semillaActual,int tamanio){
	float* resta=restaSemillas(semillaActual, semillaAnterior, tamanio);
	float normaSuperior=normaInfinito(resta,tamanio);
	delete [] resta;
	float normaInferior=normaInfinito(semillaActual,tamanio);
	return (normaSuperior/normaInferior);
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
list<float>* resolver( int tamanio, float w, float rTol, int &iteraciones, int &nanos, float* &solucion, bool registrarErrores=false){
	list<float>* errores;
	if(registrarErrores){
		errores=new list<float>;
	}
	auto begin = std::chrono::high_resolution_clock::now();
	float* b=generarB(tamanio,11);
	float* xActual=generarSemilla(tamanio);
	float* xAnterior=NULL;
	int i=0;
	float errorActual=1;
	do{

		//mostrarSemilla(xActual,tamanio);
		delete[] xAnterior;
		xAnterior=copiarSemilla(xActual,tamanio);
		iteracion(xActual,w,tamanio,b);
		i++;
		if(i>1000){
			break;
		}
		errorActual=error(xAnterior,xActual,tamanio);

		if(registrarErrores){
			errores->push_back(errorActual);
		}

	}while(errorActual>rTol);
	solucion=xActual;
	iteraciones=i;
	auto end = std::chrono::high_resolution_clock::now();
	nanos=std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count();
	delete[] b;
	return errores;
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
	ofstream archivo;
	archivo.open("salidaNumericoPunto4.csv");
	archivo<<"w,milis,iteraciones"<<endl;

	float wMejor=0;
	float iteracionesMejor=1000;

	for(float w=wInicial;w<wFinal;w+=wPaso){

		int iteraciones, nanos;
		float* solucion, *errores;
		errores=NULL;

		resolver(tamanio,w,rTol,iteraciones,nanos, solucion, errores);
		delete[] solucion;

		cout<<w<<endl;

		if(iteraciones<iteracionesMejor){
			wMejor=w;
			iteracionesMejor=iteraciones;
		}

		archivo<<w<<","<<(float) nanos/1000000<<","<<iteraciones<<endl;


	}

	archivo.close();

	cout<<"El mejor w fue:"<<wMejor<<" con iteraciones:"<<iteracionesMejor;
}


float lambda(float p, float eNuevo, float eAnterior){
	return eNuevo/pow(eAnterior,p);
}

float mediaLambda(float p, list<float>* errores){
	float media=0;
	list<float>::const_iterator iterador;
	float anterior=0;
	for(iterador=errores->begin();iterador!=errores->end();++iterador){
		if(anterior!=0){
			float l=lambda(p,*iterador,anterior);
			media+=l/errores->size();
		}
		anterior=*iterador;
	}
	return media;
}

float varianzaLambda(float p, list<float>* errores){
	float media=mediaLambda(p,errores);
	float varianza=0;
	list<float>::const_iterator iterador;
	float anterior=0;
	for(iterador=errores->begin();iterador!=errores->end();++iterador){
		if(anterior!=0){
			float l=lambda(p,*iterador,anterior);
			varianza+=(l-media)*(l-media)/errores->size();
		}
		anterior=*iterador;
	}
	return varianza;
}


void punto7(int tamanio, float w, float rTol, float* &solucion,float pInicial, float pFinal, float pPaso){
	int iteraciones,nanos;
	list<float>* errores=resolver(tamanio,w,rTol,iteraciones,nanos,solucion,true);

	ofstream archivo;
	archivo.open("salidaNumericoPunto5.csv");
	for(float p=pInicial;p<pFinal;p+=pPaso){
		archivo<<p<<","<<varianzaLambda(p,errores)<<endl;
	}
	archivo.close();

}
void escribirSemilla(float* vector, int tamanio){
	ofstream archivo;
	archivo.open("ultimoEscribirSemilla.csv");
	for(int i=0;i<tamanio;i++){
		archivo<<vector[i]<<";"<<endl;
	}
	archivo.close();
}


int main() {
	punto4(6,1,2,0.05,0.01);
	return 0;
}
