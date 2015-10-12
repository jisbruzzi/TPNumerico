#include <cmath>
#include <iostream>
#include <fstream>
using namespace std;
using namespace std;

const int tamanio = 10;
float rTol = 0.01;
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
	/**
	 * Entra un chino a un quiosco atendido por un gallego.
	 * El chino le dice "Chi hu mi nig li Marlboro?"
	 * Y el gallego lo contesta... Que querés un atado de qué?
	 */
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

float getNormaInfinito(float* vector){
	float norma=0;
	for(int i=0;i<tamanio;i++){
		if(abs(vector[i])>norma){
			norma=abs(vector[i]);
		}
	}
	return norma;
}

float calcularError(float alfa, float beta){
	return abs(beta-alfa)/abs(beta);
}

float gaussSeidel() {
	//Despejo X(i) y lo calculo con el valor de las semillas.
	//Luego lo guardo como nuevo valor de semilla.

	float valorGaussSeidel = 0;

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
		valorGaussSeidel = xi/getValMatriz(i,i);
	}
	return valorGaussSeidel;
}

void metodoSOR(float w, ofstream *archivo){
	inicializarVector();
	float error = 1;
	float sor;
	float xi;
	float xgs;
	float alfa;
	float beta;

	while(error>rTol){
		alfa = getNormaInfinito(vecSemilla);

		for(int i=0; i<tamanio; i++){
			xi = vecSemilla[i]; //Valor de la iteracion anterior
			xgs = gaussSeidel(); //Valor iteracion GS

			sor = (1-w)*xi + w*xgs;
			vecSemilla[i] = sor;
		}

		beta = getNormaInfinito(vecSemilla);
		error = calcularError(alfa, beta);

		//Guardo X(i) en el CSV
		*archivo<<"X(i) | Iteracion i";
		for(int i=0;i<tamanio;i++){
			*archivo<<";"<<vecSemilla[i];
		}
		*archivo<<endl;

		//Guardo el error en el CSV
		*archivo<<"Error";
		*archivo<<";"<<error;
		*archivo<<endl;
	}
}

void criterioDeCorte(ofstream *archivo){
	float w=1;
	while(w<2){
		metodoSOR(w, archivo);

		//Aviso el cambio de w en el CSV
		*archivo<<"Valor de w:";
		*archivo<<";"<<w;
		*archivo<<endl;

		w+=0.05;
	}
}

int main() {
	//Creo el archivo CSV
	ofstream archivo;
	archivo.open("Datos.csv");


	//Todas las funciones del programa
	calcularFi(&archivo);
	criterioDeCorte(&archivo);

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
