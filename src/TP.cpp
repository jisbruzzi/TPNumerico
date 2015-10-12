#include <cmath>
#include <iostream>
#include <fstream>
using namespace std;
using namespace std;

const int tamanio = 5;
float vecFi[tamanio];
float vecSemilla[tamanio];

void inicializarVector(){
	//La inicializo en el vector 0,0,0,...
	for(int i=0; i<tamanio; i++){
		vecSemilla[i]=0;
	}
}

int getValMatriz(int fila, int col){
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

void calcularFi(ofstream *archivo){
	const int numGrupo = 11;
	const float EI = 137e6;
	const float L = 43.7;
	int n = tamanio;

	for(int i=0; i<tamanio; i++){
		float X = ((i*L)/n);
		float fi = ((5000 + numGrupo - 10*(pow(X,2)) + 450 * X)/EI) * pow((L/n),4);
		vecFi[i] = fi;
	}

	//Guardo F(i) en el CSV
	*archivo<<"F(i)";
	for(int i=0;i<tamanio;i++){
		*archivo<<";"<<vecFi[i];
	}
	*archivo<<endl;
}

void gaussSeidel(ofstream *archivo) {
	//Despejo X(i) y lo calculo con el valor de las semillas.
	//Luego lo guardo como nuevo valor de semilla.

	//float valorGaussSeidel = 0;
	for(int i=0; i<tamanio; i++){
		int j=0;
		float xi = 0;
		while(j<tamanio){
			if (i!=j){
				xi = xi - (getValMatriz(i,j) * vecSemilla[j]);
			}
			j++;
		}
		xi = vecFi[i] + xi;
		vecSemilla[i] = xi/getValMatriz(i,i);
		//valorGaussSeidel = xi/getValMatriz(i,i);
	}
	//return valorGaussSeidel;

	/* Si agrego "ofstream *archivo" a este metodo puedo ver que andar bien
	 * pero necesito que sea una funcion, asi que le saco el void para usarlo
	 * en la ecuacion de SOR
	 */

	//Guardo X(i) en el CSV
	*archivo<<"X(i) | Iteracion i";
	for(int i=0;i<tamanio;i++){
		*archivo<<";"<<vecSemilla[i];
	}
	*archivo<<endl;
}

void metodoSOR(int cantIteraciones, ofstream *archivo){
	inicializarVector();
	int i = 0;

	//Prueba solo para ver que anda GaussSeidel
	while(i<cantIteraciones){
		gaussSeidel(archivo);
		i++;
	}


	/*
	 * 	int j=0;
	float w=1;
	while(j<cantIteraciones) {
		gaussSeidel();
		//for(int i = 0; i<tamanio; i++){
			//vecSemilla[i] = (1-w)*vecSemilla[i]+w*gaussSeidel();
		//}

		//Guardo Iteraciones(i) en el CSV
		*archivo<<"X(i) | Iteracion i";
		for(int i=0;i<tamanio;i++){
			*archivo<<";"<<vecSemilla[i];
		}
		*archivo<<endl;

		j++;
	}
	*/
}

int main() {
	//Creo el archivo CSV
	ofstream archivo;
	archivo.open("Datos.csv");


	//Todas las funciones del programa
	calcularFi(&archivo);
	metodoSOR(15, &archivo);

	//Pruebo que la matriz funciona
	for(int i=0; i<tamanio; i++){
		for(int j=0; j<tamanio; j++){
			cout<<getValMatriz(i,j);
		}
		cout<<endl;
	}


	//Al final cierro el archivo
	archivo.close();
	return 0;
}
